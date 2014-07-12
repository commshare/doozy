//    Copyright (C) 2011 Dirk Vanden Boer <dirk.vdb@gmail.com>
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

#ifndef ALBUM_ART_H
#define ALBUM_ART_H

#include <vector>
#include <string>

#include "utils/types.h"
#include "audio/audiometadata.h"

namespace doozy
{

class AlbumArt
{
public:
	AlbumArt();
    AlbumArt(AlbumArt&&) = default;
	AlbumArt(const std::string& albumId);

    void setAlbumArt(audio::Metadata::AlbumArt&& art);
    void setAlbumArt(const audio::Metadata::AlbumArt& art);
    
	std::vector<uint8_t>& getData();
	const std::vector<uint8_t>& getData() const;
	size_t getDataSize() const;
	std::string getAlbumId() const;

private:
	std::string                 m_AlbumId;
	audio::Metadata::AlbumArt   m_Art;
};

}

#endif