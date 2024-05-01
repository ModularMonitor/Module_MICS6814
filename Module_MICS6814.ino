#include "mmics6814.h"
#include "Serial/packaging.h"
#include "Serial/flags.h"

using namespace CS;

PackagedWired* wire;
mMICS* mcs;
const auto this_device = device_id::MICS_6814_SENSOR;

constexpr int port_CO  = GPIO_NUM_27;
constexpr int port_NH3 = GPIO_NUM_26;
constexpr int port_NO2 = GPIO_NUM_25;

void callback(void*, const uint8_t, const char*, const uint8_t);
  
void setup() {
    Serial.begin(115200);
    
    mcs = new mMICS(port_CO, port_NH3, port_NO2);
    
    wire = new PackagedWired(config()
        .set_slave(this_device)
        .set_slave_callback(callback)
        .set_led(2)
    );
}

void callback(void* rw, const uint8_t expects, const char* received, const uint8_t length)
{
    if (length != sizeof(Requester)) return;
    
    PackagedWired& w = *(PackagedWired*) rw;
    Requester req(received);
    
    switch(req.get_offset()) {
    case 0:
    {
        FlagWrapper fw;
        if (mcs->has_issues())              fw |= device_flags::HAS_ISSUES;
        if (mcs->has_new_data_autoreset())  fw |= device_flags::HAS_NEW_DATA;
        
        Command cmd("#FLAGS", (uint64_t)fw);
        w.slave_reply_from_callback(cmd);
    }
    break;
    case 1:
    {
        const float val = mcs->get_co();
        Command cmd("/mics6814/co", val);
        w.slave_reply_from_callback(cmd);
    }
    break;
    case 2:
    {
        const float val = mcs->get_nh3();
        Command cmd("/mics6814/nh3", val);
        w.slave_reply_from_callback(cmd);
    }
    break;
    case 3:
    {
        const float val = mcs->get_no2();
        Command cmd("/mics6814/no2", val);
        w.slave_reply_from_callback(cmd);
    }
    break;
    default:
    {
        Command cmd; // invalid
        w.slave_reply_from_callback(cmd);
    }
    }
}

// unused
void loop() { vTaskDelete(NULL); }