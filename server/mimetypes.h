//    Copyright (C) 2013 Dirk Vanden Boer <dirk.vdb@gmail.com>
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


#ifndef DOOZY_MIME_TYPES_H
#define DOOZY_MIME_TYPES_H

#include <string>

namespace doozy
{
namespace mime
{

enum class Group
{
    Audio,
    Video,
    Image,
    Other
};

enum class Type
{
    AudioMp3,
    AudioM4a,
    AudioWma,
    AudioFlac,
    AudioWave,
    AudioPcm,
    Audio3gp,
    AudioOgg,
    
    VideoAvi,
    VideoMpeg,
    VideoMp4,
    VideoWmv,
    VideoMkv,
    VideoFlv,
    VideoMov,
    Video3gp,
    VideoTivo,
    
    ImageJpeg,
    ImagePng,
    ImageTiff,
    ImageBmp,
    
    Other
};

Group groupFromFile(const std::string& filePath);
Type typeFromFile(const std::string& filePath);
Type typeFromString(const std::string& type);
std::string extensionFromType(Type type);
std::string toString(Type type);

}
}

#endif
