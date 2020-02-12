/**
 * @file    mainmavcon.cpp
 * @brief   'Arduino to Pixhawk' connector, base on powering data logs <- push data from BMS to appropriate mavlink slot 
 * @author  Marcin Mucha 
 * @details Mavlink messages: https://mavlink.io/en/messages/common.html 
 * @date    12.02.2020
*/

#include <Arduino.h>
#include <common/mavlink.h> 
#include <Wire.h>             // < communication with pixhawk
#include <random>             // < temporary lib to generate new data - test

#define DEBUGMESSAGES

// MAIN PART > \\

void setup(){

    
}

void loop(){



}

// < MAIN PART \\