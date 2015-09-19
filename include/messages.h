
/// network-layer: is handled by rf-driver

/// application-layer

struct Control // bitfield
{
    bool wantsAck  : 1;
    bool isAck     : 1;
    bool isRequest : 1;
    bool reserved  : 1;  // has tbd
};
//constexpr uint8_t Control_size = sizeof(Control);
//#if (sizeof(Control) != 1)
//#error "BITFIELD is larger than 1 Byte"
//#endif

// these commands have all a defined payload structure
enum class Command : uint8_t    // underlying type
{
    announce,       // new node tells what it got
    registerNode,   // get new ID and integrate new node into network,
    unregister,     // node has to reannounce
    //subscribe,      // same es register? tbd. maybe specialize
    shutdown,      // send to sleep for a specific time (µC + RF)
    //suspend,         // send to sleep for a specific time (µC) --> needed?

    ping,           // to each node, possible to trigger a tree-search
    sendRawData,
    sendSensor,
    sendTrigger,
    sendActor,      // normally to Node
    // sendUndefined,
    sendTime        // prefer as broadcast, so every node can sync
};


constexpr uint8_t   PAYLOAD_MAX     = ( 32 );

struct Message
{
    Control   control;
    Command   command;
    uint8_t   pLength;
    uint8_t   payload[PAYLOAD_MAX]; // TODO: should this contain a pointer to the payload instead?
    uint8_t   checksumH; // TODO: maybe reinterpret as uint16
	uint8_t   checksumL;
};
constexpr uint8_t   MessageHeaderSize = sizeof(Message) - PAYLOAD_MAX;

struct AnnounceHeader
{
    uint8_t lengthSensorList;   // has to follow after the header
    uint8_t lengthTriggerList;  // same
    uint8_t lengthActorList;    // same
    // TODO: should this contain pointers to the Lists?
};
constexpr size_t  AnnounceHeaderSize = sizeof(AnnounceHeader);

struct RegisterNodeHeader
{
    uint8_t     newID;
    uint16_t    updateInterval_s; // for Sensors, if 0 ... use default
    //uint8_t     hasToBe;

};

struct Unregister
{
    bool        shutdown   : 1;
    bool        reregister : 1;

};

struct Shutdown
{
    uint8_t     time_sec;

};

struct Ping
{
    uint8_t     senderID;
    uint8_t     receiverID;
    uint8_t     originatorID;    // who started this
    uint8_t     roundtrip_ms;
    uint8_t     magicNumber;     // to prevent endless loop, the one who did get this number before does not have to answer
    bool        spanTree    : 1; // receiver sends own ping
    bool        spanNetwork : 1;
};

struct SendRawData
{
    // something to declare here?
};

struct SendSensor
{
    uint16_t    included;   // bitfield
};

struct SendTrigger
{
    uint16_t    included;
};

struct SendActor
{
    uint16_t    included;
};

struct SendTime
{
    uint32_t unixtime_sec;
    uint32_t unixtime_ms;
};

/// Devices are divided into groups:
//   sensors,
//   triggers
//   actors
//   undefined
// --> every sensor needs it's own definition-struct. should contain datatype, update-rate, unit,

enum class Devices : std::uint8_t
{
    sensor_generic = 0,  // all sensors grouped

    battery_voltage_V,
    battery_voltage_mV,

    battery_current_A,
    battery_current_mA,
    battery_current_uA,

    time_unix_sec,
    time_unix_ms,

    acceleration_mg_x,
    acceleration_mg_y,
    acceleration_mg_z,

    barometer_pressure_pascal,
    barometer_pressure_mBar,
    barometer_pressure_uBar,
    barometer_altitude_m,
    barometer_altitude_cm,

    gyro_dps_x,
    gyro_dps_y,
    gyro_dps_z,
    gyro_mdps_x,
    gyro_mdps_y,
    gyro_mdps_z,

    magnetometer_uT_x,
    magnetometer_uT_y,
    magnetometer_uT_z,

    temperature_C,
    temperature_mC,

    airhumidity_relative,

    light_lux_value,
    light_lux_exp,

    light_intensity_clear,
    light_intensity_red,
    light_intensity_green,
    light_intensity_blue,


    trigger_generic = 64, //16384, //  all trigger grouped, bool values, can be from sensors... get send immediately

    switch_trigger,
    button_trigger,

    actor_generic = 128, //32768, // all actors grouped

    light_switch,
    motor_switch,


    undefined_generic = 192 //49152,

};
