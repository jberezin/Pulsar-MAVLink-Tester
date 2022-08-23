/*
 *  setup.cpp
 */

#include "debug_fns.h"
#include "SerSrvMnu_fns.h"
#include "hoverboard_fns.h"
#include "mavlink_fns.h"
/*
 *  setup()
 */

unsigned long  lastMotorInfo=0;

void setup()
{

    Serial.begin(115200);   // Initialise serial monitor/console port
    delay(5000);            // wait for Serial to come up so we don't miss initial messages
    enableDebugging(Serial);// enable my debugging print functions

    Serial.println("Pulsar MAVLink Tester");

    // mod by jeff berezin for 115k
    
 //   Serial1.begin(115200);   // Initialise Serial1  
   // Serial1.begin(57600, SERIAL_8N1, 4,12); // Initialise Serial1 port at 57600bps telem2 firebeetle 32e
//  Serial1.begin(115200, SERIAL_8N1, 26, 25);   // for ttgo tdisplay 2nd serial port because rover is using serial rather than usb to plug into pixhawk
  HoverSerial.begin(HOVER_SERIAL_BAUD, SERIAL_8N1,12,4);// PINS should be defines +++  talk to hoverboard
  Serial1.begin(115200, SERIAL_8N1, 16, 17);    //Talk to telemetry2 on Pixhawk for raw servo data via mavlink
                            // Arduino 2nd serial port (a.k.a Serial1) is wired to AutoPilot GPS2 port (see README.md for additional details)
 
    menuExit(); // display a message telling user how to open the memnus.
    mavlink_unrequest_datastream(MAV_DATA_STREAM_ALL);   // turnoff other crap
        
     mavlink_request_streaming_params_from_ap();
}
