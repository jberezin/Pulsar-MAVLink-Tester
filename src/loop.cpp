/*
 *  loop.cpp
 */

#include "main.h"
#include "mavlink_fns.h"
#include "SerSrvMnu_fns.h"
#include "hoverboard_fns.h"

/// Huskylens variables

#include "HUSKYLENS.h" //Import the required libraries

byte maxMotorSpeed = 50; // The normal motor speed
int motorOffset = 25;    // Factor to account for one side being more powerful
int objectWidth = 50;
int turnGain = 12;
int offCentre = 20;
int distGain = 6;

int leftLimit = 160 - offCentre;
int rightLimit = 160 + offCentre;

int leftSp = 0;
int rightSp = 0;

bool isTurning = false; // Track when turning left and right
bool isTurningLeft = true;

HUSKYLENS huskylens; // Create a new Huskeylens object
int ID1 = 2;         // The id for the "trained"  potted plant

/// end huskylens variables

// huskylens support functions

void printResult(HUSKYLENSResult result) // Display the results on the serial monitor
{
    if (result.command == COMMAND_RETURN_BLOCK)
    {

        Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW)
    {
        Serial.println(String() + F("Arrow:xOrigin=") + result.xOrigin + F(",yOrigin=") + result.yOrigin + F(",xTarget=") + result.xTarget + F(",yTarget=") + result.yTarget + F(",ID=") + result.ID);
    }
    else
    {
        Serial.println("Object unknown!");
    }
}

// end huskylens support functions

/*
 *  loop()
 */
void loop()
{ ///
    if (firstTime)
    {
        firstTime = 0;
        Wire.begin(); // Connect to Huskylens
        while (!huskylens.begin(Wire))  // this needs an iteration limit +++
        {
            Serial.println(F("Begin failed!"));
            Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protol Type>>I2C)"));
            Serial.println(F("2.Please recheck the connection."));
            delay(100);
        }
        huskylens.writeAlgorithm(ALGORITHM_OBJECT_RECOGNITION); // Switch the algorithm to object tracking.
    }

    ///

    int ledState = 0;
    if ((millis() - lastMotorInfo) > 2000)
    {                                                      // the flight controller may not have been ready at startup
        mavlink_unrequest_datastream(MAV_DATA_STREAM_ALL); // turnoff other crap

        mavlink_request_streaming_params_from_ap();
    }
    //#ifdef MAVLINK_DEBUG
    menuDo(); // check for and action user menus.
              //#endif
    // Serial.print(".");  // Print stuff to show we are running...

    if (MavRecOn)
        mavlink_receive();
    ///////////////// Huskylens stuff
    int32_t error;
    if (!huskylens.request()) // If connection to Huskylens isn't available
        Serial.println(F("Fail to request objects from HUSKYLENS!"));
    else if (!huskylens.isLearned()) // If an object hasn't yet been learned
    {
        Serial.println(F("Object not learned!"));
        //   setMotorSpeed (0,0);
    }
    else if (!huskylens.available()) // If the learned object isn't visible anymore
    {
        // Serial.println(F("Object disappeared!"));
       // setMotorSpeed(0, 0);
    }
    else // If object is being tracked
    {
        HUSKYLENSResult result = huskylens.read(); // Get the results of the object being tracked
        if (result.xCenter < leftLimit)
        {
            leftSp = -turnGain * (leftLimit - result.xCenter); //  turn right?
            rightSp = turnGain * (leftLimit - result.xCenter);
        }
        else if (result.xCenter > rightLimit)
        {
            leftSp = turnGain * (result.xCenter - rightLimit); // turn left?
            rightSp = -turnGain * (result.xCenter - rightLimit);
        }
        if (result.width < objectWidth)
        {
            leftSp = leftSp + (distGain * (objectWidth - result.width)); // determine speed to drive ahead?
            rightSp = rightSp + (distGain * (objectWidth - result.width));
        }

        // Range checks
        if (leftSp > maxMotorSpeed)
            leftSp = maxMotorSpeed;
        else if (leftSp < 0)
            leftSp = 0;
        if (rightSp > maxMotorSpeed)
            rightSp = maxMotorSpeed;
        else if (rightSp < 0)
            rightSp = 0;

        // Actually control the motor
       // setMotorSpeed(leftSp, rightSp);   +++ send a command to the hoverboard

        leftSp = 0;
        rightSp = 0;
        printResult(result);
    }

    ///////////////// end of Huskylens stuff

} // END - loop()