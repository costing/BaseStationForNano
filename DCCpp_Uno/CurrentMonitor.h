/**********************************************************************

CurrentMonitor.h
COPYRIGHT (c) 2013-2016 Gregg E. Berman

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

#ifndef CurrentMonitor_h
#define CurrentMonitor_h

#include "Arduino.h"

#define  CURRENT_SAMPLE_SMOOTHING   0.001
#define  CURRENT_SAMPLE_MAX         300   // 1.5A continuous
#define  CURRENT_ABS_MAX            700   // 3.5A peak / short current

#ifdef ARDUINO_AVR_NANO                        // Configuration for UNO
  #define  CURRENT_SAMPLE_TIME        10
#else                                         // Configuration for MEGA    
  #define  CURRENT_SAMPLE_TIME        1
#endif

struct CurrentMonitor{
  static long int sampleTime;
  int pin;
  float current;
  float currentShort;
  char *msg;
  CurrentMonitor(int, char *);
  static boolean checkTime();
  void check();
};

#endif
