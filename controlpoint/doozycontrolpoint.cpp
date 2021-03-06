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

#include "doozycontrolpoint.h"

using namespace utils;
using namespace utils::stringops;

namespace doozy
{

ControlPoint::ControlPoint()
: m_Cp(m_Client)
, m_MediaServer(m_Client)
, m_RendererScanner(m_Client, upnp::Device::Type::MediaRenderer)
, m_ServerScanner(m_Client, upnp::Device::Type::MediaServer)
{
    run();
}

void ControlPoint::run()
{
    try
    {
        m_Client.initialize();
        m_ServerScanner.start();
        m_ServerScanner.refresh();
        m_RendererScanner.start();
        m_RendererScanner.refresh();
        m_Webserver = std::make_unique<upnp::WebServer>("/Users/dirk/Projects/doozy/controlpoint");

        m_Cp.setWebserver(*m_Webserver);
        m_Cp.activate();
        
        log::info("Webserver listening url: %s", m_Webserver->getWebRootUrl());
        
    }
    catch(std::exception& e)
    {
        log::error(e.what());
    }
}
    
void ControlPoint::stop()
{
    m_Cp.deactivate();
    m_Webserver.reset();
    m_RendererScanner.stop();
    m_ServerScanner.stop();
    m_Client.destroy();
}
    
void ControlPoint::GetRenderers(rpc::DeviceResponse& response)
{
    log::info("Get renderers");
    
    auto devs = m_RendererScanner.getDevices();
    std::vector<rpc::Device> rpcDevs;
    std::transform(devs.begin(), devs.end(), std::back_inserter(rpcDevs), [&] (const std::pair<std::string, const std::shared_ptr<upnp::Device>>& dev) -> rpc::Device {
        rpc::Device rpcDev;
        rpcDev.__set_name(dev.second->m_FriendlyName);
        rpcDev.__set_udn(dev.second->m_UDN);
        return rpcDev;
    });

    response.__set_devices(rpcDevs);
    log::info("Get renderers returned %d renderers", devs.size());
}
    
void ControlPoint::GetServers(rpc::DeviceResponse& response)
{
    log::info("Get servers");
    
    auto devs = m_ServerScanner.getDevices();
    std::vector<rpc::Device> rpcDevs;
    std::transform(devs.begin(), devs.end(), std::back_inserter(rpcDevs), [&] (const std::pair<std::string, const std::shared_ptr<upnp::Device>>& dev) -> rpc::Device {
        rpc::Device rpcDev;
        rpcDev.__set_name(dev.second->m_FriendlyName);
        rpcDev.__set_udn(dev.second->m_UDN);
        return rpcDev;
    });
    
    response.__set_devices(rpcDevs);
    log::info("Get servers returned %d servers", devs.size());
}

static rpc::ItemClass::type convertClass(upnp::Class c)
{
    switch (c)
    {
        case upnp::Class::Container:
            return rpc::ItemClass::Container;
        case upnp::Class::AudioContainer:
            return rpc::ItemClass::AudioContainer;
        case upnp::Class::VideoContainer:
            return rpc::ItemClass::VideoContainer;
        case upnp::Class::ImageContainer:
            return rpc::ItemClass::ImageContainer;
        case upnp::Class::Video:
            return rpc::ItemClass::VideoItem;
        case upnp::Class::Audio:
            return rpc::ItemClass::AudioItem;
        case upnp::Class::Image:
            return rpc::ItemClass::ImageItem;
        case upnp::Class::Generic:
            return rpc::ItemClass::Item;
        default:
            return rpc::ItemClass::Unknown;
    }
}

static rpc::Action::type convertAction(upnp::MediaRenderer::Action action)
{
    switch (action)
    {
        case upnp::MediaRenderer::Action::Play:     return rpc::Action::Play;
        case upnp::MediaRenderer::Action::Next:     return rpc::Action::Next;
        case upnp::MediaRenderer::Action::Previous: return rpc::Action::Previous;
        case upnp::MediaRenderer::Action::Seek:     return rpc::Action::Seek;
        case upnp::MediaRenderer::Action::Stop:     return rpc::Action::Stop;
        case upnp::MediaRenderer::Action::Pause:    return rpc::Action::Pause;
        default: return rpc::Action::Stop; // huh? :-)
    }
}

void ControlPoint::Browse(rpc::BrowseResponse& response, const rpc::BrowseRequest& request)
{
    log::info("browse %s %s", request.udn, request.containerid);

    m_MediaServer.setDevice(m_ServerScanner.getDevice(request.udn));
    auto items = m_MediaServer.getAllInContainer(request.containerid);
    
    for (auto& item : items)
    {
        rpc::Item i;
        i.id = item->getObjectId();
        i.title = item->getTitle();
        i.itemclass = convertClass(item->getClass());
        
        std::string url;
        if (item->getClass() == upnp::Class::AudioContainer)
        {
            url = item->getMetaData(upnp::Property::AlbumArt);
        }
        
        if (url.empty())
        {
            url = item->getAlbumArtUri(upnp::dlna::ProfileId::JpegThumbnail);
        }

        if (!url.empty())
        {
            i.__set_thumbnailurl(url);
        }

        response.items.push_back(i);
    }
}

void ControlPoint::Play(const rpc::PlayRequest& request)
{
    log::info("play %s %s %s", request.rendererudn, request.serverudn, request.containerid);

    m_Cp.setRendererDevice(m_RendererScanner.getDevice(request.rendererudn));
    m_MediaServer.setDevice(m_ServerScanner.getDevice(request.serverudn));
    m_Cp.playItems(m_MediaServer, m_MediaServer.getItemsInContainer(request.containerid));
}
    
void ControlPoint::GetRendererStatus(doozy::rpc::RendererStatus& status, const doozy::rpc::Device& dev)
{
    upnp::MediaRenderer renderer(m_Client);
    renderer.setDevice(m_RendererScanner.getDevice(dev.udn));
    
    auto item = renderer.getCurrentTrackInfo();
    auto actions = renderer.getAvailableActions();
    std::transform(actions.begin(), actions.end(), status.availableActions.begin(), [] (upnp::MediaRenderer::Action a) {
        return convertAction(a);
    });
    
    status.title = item->getTitle();
    status.artist = item->getMetaData(upnp::Property::Artist);
}
    
}