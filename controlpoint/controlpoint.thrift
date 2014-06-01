namespace cpp doozy.rpc

struct Device
{
    1: required string name;
    2: required string udn;
}

enum ItemClass
{
    Unknown,
    Container,
    AudioContainer,
    ImageContainer,
    VideoContainer,
    Item,
    AudioItem,
    ImageItem,
    VideoItem
}

struct Item
{
    1: required string id;
    2: required string title;
    3: required ItemClass itemclass;
    4: optional string thumbnailurl;
}

struct DeviceResponse
{
    1: required list<Device> devices;
}

struct BrowseRequest
{
    1: required string udn;
    2: required string containerid;
}

struct BrowseResponse
{
    1: required list<Item> items;
}

struct PlayRequest
{
    1: required string rendererudn;
    2: required string serverudn;
    3: required string containerid;
}

service ControlPoint
{
    DeviceResponse GetRenderers();
    DeviceResponse GetServers();
    BrowseResponse Browse(1:BrowseRequest req);
    void Play(1:PlayRequest req);
}