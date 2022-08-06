/*
 *  setup.cpp
 */

#include "debug_fns.h"
#include "SerSrvMnu_fns.h"

/*
 *  setup()
 */

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);   // Initialise LED pin

    Serial.begin(115200);   // Initialise serial monitor/console port
    delay(5000);            // wait for Serial to come up so we don't miss initial messages
    enableDebugging(Serial);// enable my debugging print functions

    Serial.println("Pulsar MAVLink Tester");
    // mod by jeff berezin for 115k
    
 //   Serial1.begin(115200);   // Initialise Serial1  
   // Serial1.begin(57600, SERIAL_8N1, 4,12); // Initialise Serial1 port at 57600bps telem2 firebeetle 32e

   Serial1.begin(57600, SERIAL_8N1, 16, 17);    //ttgo hooked to telemetry 1 test
                            // Arduino 2nd serial port (a.k.a Serial1) is wired to AutoPilot GPS2 port (see README.md for additional details)
//Serial1.begin(57600);
    menuExit(); // display a message telling user how to open the menus.
}
