/**
 * @file    mainmavcon.cpp
 * @brief   'Arduino to Pixhawk' connector, base on powering data logs <- push data from BMS to appropriate mavlink slot 
 * @author  Marcin Mucha 
 * @details Mavlink messages: https://mavlink.io/en/messages/common.html 
 * @date    06.03.2020
*/

#include <Arduino.h>
#include <common/mavlink.h> 
//#include <Wire.h>             // < communication with pixhawk via I2C
//#include "BMS.h"
#include "BMS.h"

#define DEBUGMESSAGES
#define MAX_VOLTAGE_DATA 10
#define PIXHAWK_I2C_ADDRESS 0x66
// POWER SETTINGS

// some unique battery parameters

//static inline uint16_t mavlink_msg_battery_status_pack(
//  uint8_t system_id, uint8_t component_id,
//  mavlink_message_t* msg,
//  uint8_t id, uint8_t battery_function, uint8_t type,
//  int16_t temperature, const uint16_t *voltages,
//  int16_t current_battery, int32_t current_consumed,
//  int32_t energy_consumed, int8_t battery_remaining ) mavlink - V1
//  aditionally 
//  int32_t time_remaining, uint8_t charge_state) mavlink V2 (the newest ver.)      

uint8_t sys_id = 1;
uint8_t comp_id = 200;

uint8_t batt_id = 0;
uint8_t battery_func = MAV_BATTERY_FUNCTION_ALL;   //< to find out 
uint8_t battery_type = MAV_BATTERY_TYPE_LION;      //< rather constant
int16_t _temperature = INT16_MAX;                  //< unknown value
int16_t curr_battery = 10;
int32_t curr_consumed = 0;
int32_t eng_consumed = -1;
int8_t bat_remaining = 0;
int32_t times_remaining = 0;
uint8_t charge_type = MAV_BATTERY_CHARGE_STATE_OK; //< dynamically changed

mavlink_message_t createMavlinkPowerMessage(const uint16_t* voltageData);


void setup(){

    Serial.begin(57600);  
           
}

void loop(){

    uint16_t voltage[MAX_VOLTAGE_DATA];               //< data buffer
    uint8_t messageBuffer[MAVLINK_MAX_PACKET_LEN];    //< frame buffer
    
    for(int voltIdx = 0; voltIdx < MAX_VOLTAGE_DATA; voltIdx++){
        voltage[voltIdx] = random(10,20);
    }

    mavlink_message_t message = createMavlinkPowerMessage(voltage);

    uint16_t bufferLength = mavlink_msg_to_send_buffer(messageBuffer, &message);
    Serial.write(messageBuffer, bufferLength);
}



mavlink_message_t createMavlinkPowerMessage(const uint16_t* voltageData){

    mavlink_message_t msg;

    mavlink_msg_battery_status_pack(sys_id,          comp_id,
                                    &msg,            batt_id,
                                    battery_func,    battery_type,
                                    _temperature,    voltageData,
                                    curr_battery,    curr_consumed,
                                    eng_consumed,    bat_remaining,
                                    times_remaining, charge_type
                                );
    return msg;
}

