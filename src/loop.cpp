/*
 *  loop.cpp
 */

#include "main.h"
#include "mavlink_fns.h"
#include "SerSrvMnu_fns.h"

/*
 *  loop()
 */
void loop()
{
    int ledState = 0;
    if((millis()-lastMotorInfo)>2000)
    {  // the flight controller may not have been ready at startup
        mavlink_unrequest_datastream(MAV_DATA_STREAM_ALL);   // turnoff other crap
        
        mavlink_request_streaming_params_from_ap();
    }
//#ifdef MAVLINK_DEBUG
    menuDo();   // check for and action user menus.
//#endif
    //Serial.print(".");  // Print stuff to show we are running...

    if (MavRecOn)   mavlink_receive();

 
 

} // END - loop()