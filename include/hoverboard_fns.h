/*
 * hoverboard_fns.h
 * 
 * Header for hoverboard_fns.cpp
 * 
 */
#include "arduino.h"
#define HOVER_SERIAL_BAUD   115200      // [-] Baud rate for HoverSerial (used to communicate with the hoverboard)
#define SERIAL_BAUD         115200      // [-] Baud rate for built-in Serial (used for the Serial Monitor)
#define START_FRAME         0xABCD     	// [-] Start frme definition for reliable serial communication
#define TIME_SEND           100         // [ms] Sending time interval
#define SPEED_MAX_TEST      555         // [-] Maximum speed for testing
#define HoverSerial Serial2
 
 //#define MAVLINK_DEBUG  // uncomment if you want the various MAVLINK routines to execute their debugPrint... statements

extern unsigned long lastMotorInfo;

 extern void Send(int16_t uSteer, int16_t uSpeed);