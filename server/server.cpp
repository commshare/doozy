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

#include "server.h"

#include "common/settings.h"
//#include "devicedescriptions.h"
//#include "mediarendererdevice.h"

#include "utils/log.h"
#include "utils/readerfactory.h"
#include "utils/stringoperations.h"

#include "upnp/upnpwebserver.h"
#include "upnp/upnphttpreader.h"

#include "musiclibrary/musiclibraryfactory.h"
#include "musiclibrary/musiclibraryinterface.h"

using namespace utils;
using namespace utils::stringops;

namespace doozy
{

Server::Server()
: m_Stop(false)
{
    // make sure we can read http urls
    ReaderFactory::registerBuilder(std::unique_ptr<IReaderBuilder>(new upnp::HttpReaderBuilder()));
}

Server::~Server()
{
    stop();
}

void Server::run(const std::string& configFile)
{
    try
    {
        m_Stop = false;
        m_Client.initialize();
        
        // load settings
        Settings settings;
        settings.loadDefaultSettings();
        if (!configFile.empty())
        {
            log::info("Loading settings from: %s", configFile);
            settings.loadFromFile(configFile);
        }
        
        m_Lib.reset(MusicLibraryFactory::create(doozy::MusicLibraryType::FileSystem, settings));
        m_Lib->scan(true);
    
//        auto udn                = "uuid:" + settings.get("UDN");
//        auto friendlyName       = settings.get("FriendlyName");
//        auto audioOutput        = settings.get("AudioOutput");
//        auto audioDevice        = settings.get("AudioDevice");
//        auto description        = format(g_mediaRendererDevice.c_str(), m_Client.getIpAddress(), m_Client.getPort(), friendlyName, udn);
//        auto advertiseInterval  = 180;
//        
//        log::info("FriendlyName = %s", friendlyName);
//        log::info("AudioOutput = %s", audioOutput);
//        log::info("AudioDevice = %s", audioDevice);
//        
//        upnp::WebServer webserver("/opt/");
//        
//        webserver.addVirtualDirectory("Doozy");
//        addServiceFileToWebserver(webserver, "RenderingControlDesc.xml", g_rendererControlService);
//        addServiceFileToWebserver(webserver, "ConnectionManagerDesc.xml", g_connectionManagerService);
//        addServiceFileToWebserver(webserver, "AVTransportDesc.xml", g_avTransportService);
//        
//        MediaRendererDevice dev(udn, description, advertiseInterval, audioOutput, audioDevice, webserver);
//        dev.start();
        
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Condition.wait(lock, [this] () { return m_Stop == true; });
        
//        dev.stop();
//        webserver.removeVirtualDirectory("Doozy");
    }
    catch(std::exception& e)
    {
        log::error(e.what());
        m_Lib.reset();
    }
    
    m_Client.destroy();
}
    
void Server::stop()
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Stop = true;
    m_Condition.notify_all();
}
    
void Server::addServiceFileToWebserver(upnp::WebServer& webserver, const std::string& filename, const std::string& fileContents)
{
    webserver.addFile("Doozy", filename, "text/xml", fileContents);
}
    
}