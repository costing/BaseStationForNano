/**********************************************************************

CurrentMonitor.cpp
COPYRIGHT (c) 2013-2016 Gregg E. Berman

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

#include "DCCpp_Uno.h"
#include "CurrentMonitor.h"
#include "Comm.h"

///////////////////////////////////////////////////////////////////////////////

CurrentMonitor::CurrentMonitor(int pin, char *msg){
    this->pin=pin;
    this->msg=msg;
    current=0;
  } // CurrentMonitor::CurrentMonitor
  
boolean CurrentMonitor::checkTime(){
  if(millis()-sampleTime<CURRENT_SAMPLE_TIME)            // no need to check current yet
    return(false);
  sampleTime=millis();                                   // note millis() uses TIMER-0.  For UNO, we change the scale on Timer-0.  For MEGA we do not.  This means CURENT_SAMPLE_TIME is different for UNO then MEGA
  return(true);  
} // CurrentMonitor::checkTime

// #define CURRENT_DEBUG

#ifdef CURRENT_DEBUG
float currentMax = 0;
float currentShortMax = 0;
#endif
  
void CurrentMonitor::check(){
  const int currentReading = analogRead(pin);
  
  current=currentReading*CURRENT_SAMPLE_SMOOTHING+current*(1.0-CURRENT_SAMPLE_SMOOTHING);        // compute new exponentially-smoothed current

  currentShort = currentReading * 0.25 + currentShort * 0.75;

  #ifdef CURRENT_DEBUG
  if (pin==CURRENT_MONITOR_PIN_MAIN && ((int) currentShort > (int) currentShortMax || (int) current > (int) currentMax) && digitalRead(SIGNAL_ENABLE_PIN_PROG)==HIGH){
    INTERFACE.print(currentReading);
    INTERFACE.print(";");
    INTERFACE.print(currentShort);
    INTERFACE.print("/");
    INTERFACE.print(currentShortMax);
    INTERFACE.print(",");
    INTERFACE.print(current);
    INTERFACE.print("/");
    INTERFACE.println(currentMax);
    currentMax = max(current, currentMax);
    currentShortMax = max(currentShort, currentShortMax);
  }
  #endif
  
  if((current>CURRENT_SAMPLE_MAX || currentShort>CURRENT_ABS_MAX) && digitalRead(SIGNAL_ENABLE_PIN_PROG)==HIGH){                    // current overload and Prog Signal is on (or could have checked Main Signal, since both are always on or off together)
    digitalWrite(SIGNAL_ENABLE_PIN_PROG,LOW);                                                     // disable both Motor Shield Channels
    digitalWrite(SIGNAL_ENABLE_PIN_MAIN,LOW);                                                     // regardless of which caused current overload
    INTERFACE.print(msg);                                                                            // print corresponding error message

    #ifdef CURRENT_DEBUG
    currentMax = 0;
    currentShortMax = 0;
    #endif
  }

} // CurrentMonitor::check  

long int CurrentMonitor::sampleTime=0;
