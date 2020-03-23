#ifndef BMS_H
#define BMS_H


#include <AltSoftSerial.h>
#include "crccontrol.h"

#define TWO_UINT8_to_UINT16(lsb, msb) (((uint16_t)lsb << 8) | msb)

#define BMS_BAUD_RATE 115200
#define BMS_MSG_LEN 8
#define HEADER_IDX 0
#define CMDCODE_IDX 1
#define RGSTR_MSB_IDX 2
#define RGSTR_LSB_IDX 3
#define NULL_IDX 4
#define RGSTR_NUMBER_IDX 5
#define CRC_LSB_IDX 6
#define CRC_MSB_IDX 7

#define EMPTY_VAL 0x00

#define HEADER_FLAG 0xAA
#define MESSAGE_TYPE 0x03
#define DEFAULT_REGISTER_FLAG 0x000A
#define RGSTR_NUMBER 0x05

#define MIN_RGSTR 0x0000
#define MAX_RGSTR 0x0257

#define MAX_RGSTR_NUMBER 0x7F

// TODO: increase abstraction lvl

/**
 * Main message using to request communication with
 * BMS module.
 */

struct bms_message{
    uint8_t message[BMS_MSG_LEN]{ 0x00 };

    bms_message(){ setBreakByte(); }

    void setHeaderByte(uint8_t header) { message[HEADER_IDX] = header; }
    void setMessageType(uint8_t type) { message[CMDCODE_IDX] = type; }

    void setStartRegister(uint16_t regnr);
    void setBreakByte() { message[NULL_IDX] = EMPTY_VAL; };
    void setNumberOfRegisters(uint8_t numbers) { message[RGSTR_NUMBER_IDX] = numbers; }

    void setCRC(); // eventually change to encapsulate version with class data type

    const uint8_t* getMessageToSend() const { return message; };
};


/**
 * We can storage received data
 */

struct bms_data_packet{
    float voltage;
    float current;
    float charging_current;
    float current_consumed;
};

/**
 * Function used to fill default BMS message.
 * Configure 10 - 15 registers, so we read data
 * from cell: 10 till cell number 15. How many cells
 * you wanna read is determine by RGSTR_NUMBER.
 * Function ensure CRC setup.
 * @param packet
 */

void fill_default_bms_message(bms_message* packet);


class TinyBMS{
    private:
        AltSoftSerial softwareSerial;

    public:
        TinyBMS(const int baudrate = BMS_BAUD_RATE) { softwareSerial.begin(baudrate); }
        
        bool requestBMSData(const bms_message& message);
        bms_data_packet readDataFromBMS();  
};

#endif