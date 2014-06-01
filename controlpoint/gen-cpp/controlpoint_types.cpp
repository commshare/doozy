/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "controlpoint_types.h"

#include <algorithm>

namespace doozy { namespace rpc {

int _kItemClassValues[] = {
  ItemClass::Unknown,
  ItemClass::Container,
  ItemClass::AudioContainer,
  ItemClass::ImageContainer,
  ItemClass::VideoContainer,
  ItemClass::Item,
  ItemClass::AudioItem,
  ItemClass::ImageItem,
  ItemClass::VideoItem
};
const char* _kItemClassNames[] = {
  "Unknown",
  "Container",
  "AudioContainer",
  "ImageContainer",
  "VideoContainer",
  "Item",
  "AudioItem",
  "ImageItem",
  "VideoItem"
};
const std::map<int, const char*> _ItemClass_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(9, _kItemClassValues, _kItemClassNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));


Device::~Device() throw() {
}


void Device::__set_name(const std::string& val) {
  name = val;
}

void Device::__set_udn(const std::string& val) {
  udn = val;
}

const char* Device::ascii_fingerprint = "07A9615F837F7D0A952B595DD3020972";
const uint8_t Device::binary_fingerprint[16] = {0x07,0xA9,0x61,0x5F,0x83,0x7F,0x7D,0x0A,0x95,0x2B,0x59,0x5D,0xD3,0x02,0x09,0x72};

uint32_t Device::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_name = false;
  bool isset_udn = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->name);
          isset_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->udn);
          isset_udn = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_name)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_udn)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t Device::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("Device");

  xfer += oprot->writeFieldBegin("name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("udn", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->udn);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(Device &a, Device &b) {
  using ::std::swap;
  swap(a.name, b.name);
  swap(a.udn, b.udn);
}

Device::Device(const Device& other0) {
  name = other0.name;
  udn = other0.udn;
}
Device& Device::operator=(const Device& other1) {
  name = other1.name;
  udn = other1.udn;
  return *this;
}

Item::~Item() throw() {
}


void Item::__set_id(const std::string& val) {
  id = val;
}

void Item::__set_title(const std::string& val) {
  title = val;
}

void Item::__set_itemclass(const ItemClass::type val) {
  itemclass = val;
}

void Item::__set_thumbnailurl(const std::string& val) {
  thumbnailurl = val;
__isset.thumbnailurl = true;
}

const char* Item::ascii_fingerprint = "8CDAAA4EC40CD63F2ED04068E6960091";
const uint8_t Item::binary_fingerprint[16] = {0x8C,0xDA,0xAA,0x4E,0xC4,0x0C,0xD6,0x3F,0x2E,0xD0,0x40,0x68,0xE6,0x96,0x00,0x91};

uint32_t Item::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_id = false;
  bool isset_title = false;
  bool isset_itemclass = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->id);
          isset_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->title);
          isset_title = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast2;
          xfer += iprot->readI32(ecast2);
          this->itemclass = (ItemClass::type)ecast2;
          isset_itemclass = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->thumbnailurl);
          this->__isset.thumbnailurl = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_id)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_title)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_itemclass)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t Item::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("Item");

  xfer += oprot->writeFieldBegin("id", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("title", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->title);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("itemclass", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32((int32_t)this->itemclass);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.thumbnailurl) {
    xfer += oprot->writeFieldBegin("thumbnailurl", ::apache::thrift::protocol::T_STRING, 4);
    xfer += oprot->writeString(this->thumbnailurl);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(Item &a, Item &b) {
  using ::std::swap;
  swap(a.id, b.id);
  swap(a.title, b.title);
  swap(a.itemclass, b.itemclass);
  swap(a.thumbnailurl, b.thumbnailurl);
  swap(a.__isset, b.__isset);
}

Item::Item(const Item& other3) {
  id = other3.id;
  title = other3.title;
  itemclass = other3.itemclass;
  thumbnailurl = other3.thumbnailurl;
  __isset = other3.__isset;
}
Item& Item::operator=(const Item& other4) {
  id = other4.id;
  title = other4.title;
  itemclass = other4.itemclass;
  thumbnailurl = other4.thumbnailurl;
  __isset = other4.__isset;
  return *this;
}

DeviceResponse::~DeviceResponse() throw() {
}


void DeviceResponse::__set_devices(const std::vector<Device> & val) {
  devices = val;
}

const char* DeviceResponse::ascii_fingerprint = "006EFB9C0A4E436459CDFDF617590BB4";
const uint8_t DeviceResponse::binary_fingerprint[16] = {0x00,0x6E,0xFB,0x9C,0x0A,0x4E,0x43,0x64,0x59,0xCD,0xFD,0xF6,0x17,0x59,0x0B,0xB4};

uint32_t DeviceResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_devices = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->devices.clear();
            uint32_t _size5;
            ::apache::thrift::protocol::TType _etype8;
            xfer += iprot->readListBegin(_etype8, _size5);
            this->devices.resize(_size5);
            uint32_t _i9;
            for (_i9 = 0; _i9 < _size5; ++_i9)
            {
              xfer += this->devices[_i9].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          isset_devices = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_devices)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t DeviceResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("DeviceResponse");

  xfer += oprot->writeFieldBegin("devices", ::apache::thrift::protocol::T_LIST, 1);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->devices.size()));
    std::vector<Device> ::const_iterator _iter10;
    for (_iter10 = this->devices.begin(); _iter10 != this->devices.end(); ++_iter10)
    {
      xfer += (*_iter10).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(DeviceResponse &a, DeviceResponse &b) {
  using ::std::swap;
  swap(a.devices, b.devices);
}

DeviceResponse::DeviceResponse(const DeviceResponse& other11) {
  devices = other11.devices;
}
DeviceResponse& DeviceResponse::operator=(const DeviceResponse& other12) {
  devices = other12.devices;
  return *this;
}

BrowseRequest::~BrowseRequest() throw() {
}


void BrowseRequest::__set_udn(const std::string& val) {
  udn = val;
}

void BrowseRequest::__set_containerid(const std::string& val) {
  containerid = val;
}

const char* BrowseRequest::ascii_fingerprint = "07A9615F837F7D0A952B595DD3020972";
const uint8_t BrowseRequest::binary_fingerprint[16] = {0x07,0xA9,0x61,0x5F,0x83,0x7F,0x7D,0x0A,0x95,0x2B,0x59,0x5D,0xD3,0x02,0x09,0x72};

uint32_t BrowseRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_udn = false;
  bool isset_containerid = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->udn);
          isset_udn = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->containerid);
          isset_containerid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_udn)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_containerid)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t BrowseRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("BrowseRequest");

  xfer += oprot->writeFieldBegin("udn", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->udn);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("containerid", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->containerid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(BrowseRequest &a, BrowseRequest &b) {
  using ::std::swap;
  swap(a.udn, b.udn);
  swap(a.containerid, b.containerid);
}

BrowseRequest::BrowseRequest(const BrowseRequest& other13) {
  udn = other13.udn;
  containerid = other13.containerid;
}
BrowseRequest& BrowseRequest::operator=(const BrowseRequest& other14) {
  udn = other14.udn;
  containerid = other14.containerid;
  return *this;
}

BrowseResponse::~BrowseResponse() throw() {
}


void BrowseResponse::__set_items(const std::vector<Item> & val) {
  items = val;
}

const char* BrowseResponse::ascii_fingerprint = "F0604E84077F8A695D7FAADF9F917713";
const uint8_t BrowseResponse::binary_fingerprint[16] = {0xF0,0x60,0x4E,0x84,0x07,0x7F,0x8A,0x69,0x5D,0x7F,0xAA,0xDF,0x9F,0x91,0x77,0x13};

uint32_t BrowseResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_items = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->items.clear();
            uint32_t _size15;
            ::apache::thrift::protocol::TType _etype18;
            xfer += iprot->readListBegin(_etype18, _size15);
            this->items.resize(_size15);
            uint32_t _i19;
            for (_i19 = 0; _i19 < _size15; ++_i19)
            {
              xfer += this->items[_i19].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          isset_items = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_items)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t BrowseResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("BrowseResponse");

  xfer += oprot->writeFieldBegin("items", ::apache::thrift::protocol::T_LIST, 1);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->items.size()));
    std::vector<Item> ::const_iterator _iter20;
    for (_iter20 = this->items.begin(); _iter20 != this->items.end(); ++_iter20)
    {
      xfer += (*_iter20).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(BrowseResponse &a, BrowseResponse &b) {
  using ::std::swap;
  swap(a.items, b.items);
}

BrowseResponse::BrowseResponse(const BrowseResponse& other21) {
  items = other21.items;
}
BrowseResponse& BrowseResponse::operator=(const BrowseResponse& other22) {
  items = other22.items;
  return *this;
}

PlayRequest::~PlayRequest() throw() {
}


void PlayRequest::__set_rendererudn(const std::string& val) {
  rendererudn = val;
}

void PlayRequest::__set_serverudn(const std::string& val) {
  serverudn = val;
}

void PlayRequest::__set_containerid(const std::string& val) {
  containerid = val;
}

const char* PlayRequest::ascii_fingerprint = "AB879940BD15B6B25691265F7384B271";
const uint8_t PlayRequest::binary_fingerprint[16] = {0xAB,0x87,0x99,0x40,0xBD,0x15,0xB6,0xB2,0x56,0x91,0x26,0x5F,0x73,0x84,0xB2,0x71};

uint32_t PlayRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_rendererudn = false;
  bool isset_serverudn = false;
  bool isset_containerid = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->rendererudn);
          isset_rendererudn = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->serverudn);
          isset_serverudn = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->containerid);
          isset_containerid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_rendererudn)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_serverudn)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_containerid)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t PlayRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("PlayRequest");

  xfer += oprot->writeFieldBegin("rendererudn", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->rendererudn);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("serverudn", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->serverudn);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("containerid", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->containerid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(PlayRequest &a, PlayRequest &b) {
  using ::std::swap;
  swap(a.rendererudn, b.rendererudn);
  swap(a.serverudn, b.serverudn);
  swap(a.containerid, b.containerid);
}

PlayRequest::PlayRequest(const PlayRequest& other23) {
  rendererudn = other23.rendererudn;
  serverudn = other23.serverudn;
  containerid = other23.containerid;
}
PlayRequest& PlayRequest::operator=(const PlayRequest& other24) {
  rendererudn = other24.rendererudn;
  serverudn = other24.serverudn;
  containerid = other24.containerid;
  return *this;
}
}} // namespace
