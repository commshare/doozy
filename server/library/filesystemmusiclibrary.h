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

#ifndef FILESYSTEM_MUSIC_LIBRARY_H
#define FILESYSTEM_MUSIC_LIBRARY_H

#include <string>
#include <vector>
#include <thread>
#include <mutex>

#include "utils/types.h"
#include "musicdb.h"
#include "musiclibraryinterface.h"

namespace doozy
{

class ServerSettings;
class Scanner;

class FilesystemMusicLibrary : public IMusicLibrary
{
public:
    FilesystemMusicLibrary(const ServerSettings& settings);
    ~FilesystemMusicLibrary();

    uint32_t getObjectCount() override;
    uint32_t getObjectCountInContainer(const std::string& id) override;

    upnp::ItemPtr getItem(const std::string& id) override;
    std::vector<upnp::ItemPtr> getItems(const std::string& id, uint32_t offset, uint32_t count) override;

    void scan(bool startFresh) override;
    //void search(const std::string& search, utils::ISubscriber<const Track&>& trackSubscriber, utils::ISubscriber<const Album&>& albumSubscriber);

private:
    void cancelScanThread();
    void scannerThread();

    MusicDb                         m_Db;
    std::string                     m_LibraryPath;
    std::thread                     m_ScannerThread;
    std::mutex                      m_ScanMutex;
    std::unique_ptr<Scanner>        m_Scanner;
    bool                            m_Destroy;
    const ServerSettings&           m_Settings;
};

}

#endif
