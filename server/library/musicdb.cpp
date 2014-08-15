//    Copyright (C) 2009 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "musicdb.h"

#include <stdexcept>
#include <sstream>
#include <cassert>
#include <cstring>
#include <sqlite3.h>
#include <set>
#include <map>

#include "utils/fileoperations.h"
#include "utils/stringoperations.h"
#include "utils/numericoperations.h"
#include "utils/log.h"
#include "utils/trace.h"

#include <sqlpp11/sqlpp11.h>

#include "upnp/upnpitem.h"
#include "mimetypes.h"
#include "doozyscheme.h"

using namespace std;
using namespace utils;

//#define DEBUG_QUERIES

namespace doozy
{

namespace sql = sqlpp::sqlite3;

namespace
{

doozy::Objects objects;
doozy::Metadata metadata;

SQLPP_ALIAS_PROVIDER(numObjects);

template <typename T>
bool getIdFromResultIfExists(const T& result, std::string& id)
{
    bool hasResults = !result.empty();
    if (hasResults)
    {
        id = result.front().ObjectId;
    }
    
    return hasResults;
}

std::shared_ptr<sql::connection_config> createConfig(const std::string filename)
{
    auto config = std::make_shared<sql::connection_config>();
    config->path_to_database = filename;
    config->flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    
#ifdef DEBUG_QUERIES
    config->debug = true;
#endif

    return config;
}

// Queries
auto objectCountQuery = [] () {
    return select(count(objects.Id).as(numObjects))
           .from(objects)
           .where(true);
};

auto childCountQuery = [] () {
    return select(count(objects.Id).as(numObjects))
           .from(objects)
           .where(objects.ParentId == parameter(objects.ParentId));
};

auto addItemQuery = [] () {
    return insert_into(objects).set(
        objects.ObjectId    = parameter(objects.ObjectId),
        objects.ParentId    = parameter(objects.ParentId),
        objects.RefId       = parameter(objects.RefId),
        objects.Name        = parameter(objects.Name),
        objects.Class       = parameter(objects.Class),
        objects.MetaData    = sqlpp::verbatim<sqlpp::integer>("last_insert_rowid()")
    );
};

auto addMetadataQuery = [] () {
    return insert_into(metadata).set(
        metadata.ModifiedTime   = parameter(metadata.ModifiedTime),
        metadata.FilePath       = parameter(metadata.FilePath),
        metadata.FileSize       = parameter(metadata.FileSize),
        metadata.Title          = parameter(metadata.Title),
        metadata.Artist         = parameter(metadata.Artist),
        metadata.Genre          = parameter(metadata.Genre),
        metadata.MimeType       = parameter(metadata.MimeType),
        metadata.Duration       = parameter(metadata.Duration),
        metadata.Channels       = parameter(metadata.Channels),
        metadata.BitRate        = parameter(metadata.BitRate),
        metadata.SampleRate     = parameter(metadata.SampleRate),
        metadata.Thumbnail      = parameter(metadata.Thumbnail)
    );
};

auto itemExistsQuery = [] () {
    return select(objects.ObjectId)
           .from(metadata.left_outer_join(objects).on(objects.MetaData == metadata.Id))
           .where(metadata.FilePath == parameter(metadata.FilePath));
};

auto itemStatusQuery = [] () {
    return select(metadata.ModifiedTime)
           .from(metadata)
           .where(metadata.FilePath == parameter(metadata.FilePath));
};

auto itemPathQuery = [] () {
    return select(metadata.FilePath)
           .from(objects.left_outer_join(metadata).on(objects.MetaData == metadata.Id))
           .where(objects.ObjectId == parameter(objects.ObjectId));
};

auto uniqueContainerIdQuery = [] () {
    return select(count(objects.Id).as(numObjects))
           .from(objects)
           .where(objects.ParentId == parameter(objects.ParentId));
};

using ObjectCountQuery       = decltype(objectCountQuery());
using ChildCountQuery        = decltype(childCountQuery());
using AddItemQuery           = decltype(addItemQuery());
using AddMetadataQuery       = decltype(addMetadataQuery());
using ItemExistsQuery        = decltype(itemExistsQuery());
using ItemStatusQuery        = decltype(itemStatusQuery());
using ItemPathQuery          = decltype(itemPathQuery());
using UniqueContainerIdQuery = decltype(uniqueContainerIdQuery());

template <typename SelectType>
using PreparedStatement = decltype(((sql::connection*)nullptr)->prepare(*((SelectType*)nullptr)));

}

struct MusicDb::PreparedStatements
{
    PreparedStatement<ObjectCountQuery> objectCount;
    PreparedStatement<ChildCountQuery> childCount;
    PreparedStatement<AddItemQuery> addItem;
    PreparedStatement<AddMetadataQuery> addMetadata;
    PreparedStatement<ItemExistsQuery> itemExists;
    PreparedStatement<ItemStatusQuery> itemStatus;
    PreparedStatement<ItemPathQuery> itemPath;
    PreparedStatement<UniqueContainerIdQuery> uniqueContainerId;
};

MusicDb::MusicDb(const string& dbFilepath)
: m_db(createConfig(dbFilepath))
, m_statements(new PreparedStatements())
{
    utils::trace("Create Music database");
    
    createInitialDatabase();
    prepareStatements();

    utils::trace("Music database loaded");
}

MusicDb::~MusicDb()
{
}

void MusicDb::prepareStatements()
{
    m_statements->objectCount       = m_db.prepare(objectCountQuery());
    m_statements->childCount        = m_db.prepare(childCountQuery());
    m_statements->addItem           = m_db.prepare(addItemQuery());
    m_statements->addMetadata       = m_db.prepare(addMetadataQuery());
    m_statements->itemExists        = m_db.prepare(itemExistsQuery());
    m_statements->itemStatus        = m_db.prepare(itemStatusQuery());
    m_statements->itemPath          = m_db.prepare(itemPathQuery());
    m_statements->uniqueContainerId = m_db.prepare(uniqueContainerIdQuery());
}

void MusicDb::setWebRoot(const std::string& webRoot)
{
    m_webRoot = webRoot;
}

uint64_t MusicDb::getObjectCount()
{
    return m_db.run(m_statements->objectCount).front().numObjects;
}

uint64_t MusicDb::getChildCount(const std::string& id)
{
    m_statements->childCount.params.ParentId = id;
    return m_db.run(m_statements->childCount).front().numObjects;
}

uint64_t MusicDb::getUniqueIdInContainer(const std::string& containerId)
{
    m_statements->uniqueContainerId.params.ParentId = containerId;
    return m_db.run(m_statements->uniqueContainerId).front().numObjects;
}

void MusicDb::addItem(const LibraryItem& item)
{
    addMetadata(item);
    m_statements->addItem.params.ObjectId = item.objectId;
    m_statements->addItem.params.ParentId = sqlpp::tvin(item.parentId);
    m_statements->addItem.params.RefId    = sqlpp::tvin(item.refId);
    m_statements->addItem.params.Name     = item.name;
    m_statements->addItem.params.Class    = sqlpp::tvin(item.upnpClass);
    m_db.run(m_statements->addItem);
}

void MusicDb::addItems(const std::vector<LibraryItem>& items)
{
    
    m_db.start_transaction();
    
    for (auto& item : items)
    {
        addItem(item);
    }
    
    m_db.commit_transaction();
}

int64_t MusicDb::addMetadata(const LibraryItem& item)
{
    m_statements->addMetadata.params.ModifiedTime  = static_cast<int64_t>(item.modifiedTime);
    m_statements->addMetadata.params.FilePath      = sqlpp::tvin(item.path);
    m_statements->addMetadata.params.FileSize      = static_cast<int64_t>(item.fileSize);
    m_statements->addMetadata.params.Title         = sqlpp::tvin(item.title);
    m_statements->addMetadata.params.Artist        = sqlpp::tvin(item.artist);
    m_statements->addMetadata.params.Genre         = sqlpp::tvin(item.genre);
    m_statements->addMetadata.params.MimeType      = sqlpp::tvin(item.mimeType);
    m_statements->addMetadata.params.Duration      = item.duration;
    m_statements->addMetadata.params.Channels      = item.nrChannels;
    m_statements->addMetadata.params.BitRate       = item.bitrate;
    m_statements->addMetadata.params.SampleRate    = item.sampleRate;
    m_statements->addMetadata.params.Thumbnail     = sqlpp::tvin(item.thumbnail);
    
    return m_db.run(m_statements->addMetadata);
}

void MusicDb::updateItem(const LibraryItem& item)
{
    m_db.run(update(objects).set(
        objects.ObjectId   = item.objectId,
        objects.ParentId   = sqlpp::tvin(item.parentId),
        objects.RefId      = sqlpp::tvin(item.refId),
        objects.Name       = item.name,
        objects.Class      = sqlpp::tvin(item.upnpClass)
    ).where(true));
    
    m_db.run(update(metadata).set(
        metadata.ModifiedTime  = static_cast<int64_t>(item.modifiedTime),
        metadata.FilePath      = sqlpp::tvin(item.path),
        metadata.FileSize      = static_cast<int64_t>(item.fileSize),
        metadata.Title         = sqlpp::tvin(item.title),
        metadata.Artist        = sqlpp::tvin(item.artist),
        metadata.Genre         = sqlpp::tvin(item.genre),
        metadata.MimeType      = sqlpp::tvin(item.mimeType),
        metadata.Duration      = item.duration,
        metadata.Channels      = item.nrChannels,
        metadata.BitRate       = item.bitrate,
        metadata.SampleRate    = item.sampleRate,
        metadata.Thumbnail     = sqlpp::tvin(item.thumbnail)
    ).where(true));
}

bool MusicDb::itemExists(const string& filepath, string& objectId)
{
    m_statements->itemExists.params.FilePath = filepath;
    auto result = m_db.run(m_statements->itemExists);
    
    if (result.empty())
    {
        return false;
    }
    
    objectId = result.front().ObjectId;
    return true;
}

bool MusicDb::albumExists(const std::string& title, const std::string& artist, string& objectId)
{
    auto query = dynamic_select(m_db, objects.ObjectId)
                 .from(metadata.left_outer_join(objects).on(objects.MetaData == metadata.Id))
                 .dynamic_where(objects.Class == "object.container.album.musicAlbum" and objects.Name == title);
    
    if (artist.empty())
    {
        query.where.add(metadata.Artist.is_null());
    }
    else
    {
        query.where.add(metadata.Artist == artist);
    }

    return getIdFromResultIfExists(m_db.run(query), objectId);
}

ItemStatus MusicDb::getItemStatus(const std::string& filepath, uint64_t modifiedTime)
{
    m_statements->itemStatus.params.FilePath = filepath;
    auto result = m_db.run(m_statements->itemStatus);
    
    if (result.empty())
    {
        return ItemStatus::DoesntExist;
    }
    
    return result.front().ModifiedTime < modifiedTime ? ItemStatus::NeedsUpdate : ItemStatus::UpToDate;
}

std::string MusicDb::getItemPath(const std::string& objectId)
{
    m_statements->itemPath.params.ObjectId = objectId;
    auto result = m_db.run(m_statements->itemPath);
    
    if (result.empty())
    {
        throw std::runtime_error("No item in database with id: " + objectId);
    }

    return result.front().FilePath;
}

template <typename T>
upnp::ItemPtr MusicDb::parseItem(const T& row)
{
    assert(!m_webRoot.empty());

    auto item = std::make_shared<upnp::Item>();
    item->setObjectId(row.ObjectId);
    item->setTitle(row.Name);
    item->setParentId(row.ParentId);
    item->setRefId(row.RefId);
    item->setClass(row.Class);
    item->addMetaData(upnp::Property::Artist, row.Artist);

    if (!row.Title.is_null())
    {
        item->setTitle(row.Title);
    }

    item->addMetaData(upnp::Property::Genre, row.Genre);

    if (!item->isContainer())
    {
        // add the resource urls
        upnp::Resource res;
        auto mimeType = row.MimeType;
        res.setProtocolInfo(upnp::ProtocolInfo(stringops::format("http-get:*:%s:*", mimeType)));
        res.setUrl(stringops::format("%sMedia/%s.%s", m_webRoot, item->getObjectId(), mime::extensionFromType(mime::typeFromString(mimeType))));
        res.setSize(row.FileSize);
        res.setDuration(static_cast<uint32_t>(row.Duration));
        res.setNrAudioChannels(static_cast<uint32_t>(row.Channels));
        res.setBitRate(static_cast<uint32_t>(row.BitRate));
        res.setSampleRate(static_cast<uint32_t>(row.SampleRate));

        item->addResource(res);
    }
    else
    {
        item->addMetaData(upnp::Property::StorageUsed, "-1");
    }
    
    auto thumbnail = row.Thumbnail;
    if (!thumbnail.is_null())
    {
        item->addMetaData(upnp::Property::AlbumArt, fileops::combinePath(m_webRoot, thumbnail));
    }

    return item;
}

upnp::ItemPtr MusicDb::getItem(const std::string& id)
{
    const auto& result = m_db.run(
        select(objects.ObjectId, objects.Name, objects.ParentId, objects.RefId, objects.Class, all_of(metadata))
        .from(objects.left_outer_join(metadata).on(objects.MetaData == metadata.Id))
        .where(objects.ObjectId == id)
    );

    if (result.empty())
    {
        throw std::runtime_error("No track found in db with id: " + id);
    }

    return parseItem(result.front());
}

std::vector<upnp::ItemPtr> MusicDb::getItems(const std::string& parentId, uint32_t offset, uint32_t count)
{
    std::vector<upnp::ItemPtr> items;

    for (const auto& row : m_db.run(
        select(objects.ObjectId, objects.Name, objects.ParentId, objects.RefId, objects.Class, all_of(metadata))
        .from(objects.left_outer_join(metadata).on(objects.MetaData == metadata.Id))
        .where(objects.ParentId == parentId)
        .limit(count == 0 ? -1 : count)
        .offset(offset)
    ))
    {
        items.emplace_back(parseItem(row));
    }

    return items;
}

void MusicDb::removeItem(const std::string& id)
{
    m_db.run(
        remove_from(objects)
        .where(objects.ObjectId == id)
    );
}

void MusicDb::removeMetaData(int64_t id)
{
    m_db.run(
        remove_from(metadata)
        .where(metadata.Id == static_cast<int64_t>(id))
    );
}


void MusicDb::removeNonExistingFiles()
{
    std::vector<std::pair<std::string, int64_t>> files;

    for (const auto& row : m_db.run(select(objects.ObjectId, objects.MetaData, metadata.FilePath)
                                     .from(objects.left_outer_join(metadata).on(objects.MetaData == metadata.Id))
                                     .where(true)))
    {
        std::string path = row.FilePath;
        if (!path.empty() && !fileops::pathExists(path))
        {
            files.emplace_back(row.ObjectId, row.MetaData);
        }
    }
    
    for (auto& iter : files)
    {
        log::debug("Removed deleted file from database: %d", iter.first);
        removeItem(iter.first);
        removeMetaData(iter.second);
    }
}

//class SearchTrackData
//{
//public:
//    SearchTrackData(utils::ISubscriber<const Track&>& trackSubscriber, set<uint32_t>& foundIds)
//    : subscriber(trackSubscriber)
//    , ids(foundIds)
//    {
//    }
//
//    utils::ISubscriber<const Track&>& subscriber;
//    set<uint32_t>&      ids;
//};
//
//void MusicDb::searchLibrary(const std::string& search, utils::ISubscriber<const Track&>& trackSubscriber, utils::ISubscriber<const Album&>& albumSubscriber)
//{
//    std::lock_guard<std::recursive_mutex> lock(m_dbMutex);
//
//    set<uint32_t> albumIds;
//    sqlite3_stmt* pStmt = createStatement(
//        "SELECT Id "
//        "FROM albums "
//        "WHERE albums.AlbumArtist LIKE (SELECT '%' || ?1 || '%') "
//        "OR albums.Name LIKE (SELECT '%' || ?1 || '%');");
//    bindValue(pStmt, search, 1);
//    performQuery(pStmt, getAllAlbumIdsCb, &albumIds);
//
//    SearchTrackData data(trackSubscriber, albumIds);
//    pStmt = createStatement(
//        "SELECT tracks.Id, tracks.AlbumId, tracks.Title, tracks.Composer, tracks.Filepath, tracks.Year, tracks.TrackNr, tracks.DiscNr, tracks.Duration, tracks.BitRate, tracks.SampleRate, tracks.Channels, tracks.FileSize, tracks.ModifiedTime, artists.Name, albums.Name, albums.AlbumArtist, genres.Name "
//        "FROM tracks "
//        "LEFT OUTER JOIN albums ON tracks.AlbumId = albums.Id "
//        "LEFT OUTER JOIN artists ON tracks.ArtistId = artists.Id "
//        "LEFT OUTER JOIN genres ON tracks.GenreId = genres.Id "
//        "WHERE artists.Name LIKE (SELECT '%' || ?1 || '%')"
//        "OR tracks.title LIKE (SELECT '%' || ?1 || '%');");
//
//
//    bindValue(pStmt, search, 1);
//    performQuery(pStmt, searchTracksCb, &data);
//
//    vector<Album> albums;
//    pStmt = createStatement(
//        "SELECT albums.Id, albums.Name, albums.AlbumArtist, albums.Year, albums.Duration, albums.DateAdded, genres.Name "
//        "FROM albums "
//    	"LEFT OUTER JOIN genres ON albums.GenreId = genres.Id "
//        "WHERE albums.Id=?;");
//
//    for (set<uint32_t>::iterator iter = data.ids.begin(); iter != data.ids.end(); ++iter)
//    {
//        bindValue(pStmt, *iter, 1);
//        performQuery(pStmt, getAlbumListCb, &albums, false);
//
//        if (sqlite3_reset(pStmt) != SQLITE_OK)
//        {
//            sqlite3_finalize(pStmt);
//            throw runtime_error(string("Failed to reset statement: ") + sqlite3_errmsg(m_pDb));
//        }
//        if (sqlite3_clear_bindings(pStmt) != SQLITE_OK)
//        {
//            sqlite3_finalize(pStmt);
//            throw runtime_error(string("Failed to clear bindings: ") + sqlite3_errmsg(m_pDb));
//        }
//    }
//    sqlite3_finalize(pStmt);
//
//    for (size_t i = 0; i < albums.size(); ++i)
//    {
//        albumSubscriber.onItem(albums[i]);
//    }
//}

void MusicDb::clearDatabase()
{
    m_db.execute("DROP INDEX IF EXISTS metadata.pathIndex;");
    m_db.execute("DROP TABLE IF EXISTS objects;");
    m_db.execute("DROP TABLE IF EXISTS metadata;");

    createInitialDatabase();
}

void MusicDb::createInitialDatabase()
{
    m_db.execute("CREATE TABLE IF NOT EXISTS objects("
        "Id INTEGER PRIMARY KEY,"
        "ObjectId TEXT UNIQUE,"
        "ParentId TEXT,"
        "RefId TEXT,"
        "Name TEXT NOT NULL,"
        "Class TEXT,"
        "MetaData INTEGER,"
        "FOREIGN KEY (MetaData) REFERENCES metadata(id));");

    m_db.execute("CREATE TABLE IF NOT EXISTS metadata("
        "Id INTEGER PRIMARY KEY,"
        "Album TEXT,"
        "Artist TEXT,"
        "Title TEXT,"
        "AlbumArtist TEXT,"
        "Genre TEXT,"
        "Composer TEXT,"
        "Year INTEGER,"
        "TrackNr INTEGER,"
        "DiscNr INTEGER,"
        "AlbumOrder INTEGER,"
        "Duration INTEGER,"
        "MimeType TEXT,"
        "BitRate INTEGER,"
        "SampleRate INTEGER,"
        "Channels INTEGER,"
        "FileSize INTEGER,"
        "DateAdded INTEGER,"
        "ModifiedTime INTEGER,"
        "Thumbnail TEXT,"
        "FilePath TEXT);");

    m_db.execute("CREATE INDEX IF NOT EXISTS pathIndex ON metadata (FilePath);");
}

}
