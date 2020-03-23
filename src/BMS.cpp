#include "BMS.h"

void bms_message::setStartRegister(uint16_t regnr){
    uint8_t registerLSB = (regnr >> 8);
    uint8_t registerMSB = (regnr & 0x00FF);

    message[RGSTR_MSB_IDX] = registerMSB;
    message[RGSTR_LSB_IDX] = registerLSB;
}

void bms_message::setCRC(){
    uint16_t checkSum = CRC16(message, (uint16_t)(BMS_MSG_LEN - 1));

    uint8_t crcLSB = (checkSum >> 8);
    uint8_t crcMSB = (checkSum & 0x00FF);

    message[CRC_LSB_IDX] = crcLSB;
    message[CRC_MSB_IDX] = crcMSB;
}

void fill_default_bms_message(bms_message* packet){

    packet->setHeaderByte(HEADER_FLAG);
    packet->setMessageType(MESSAGE_TYPE);
    packet->setStartRegister(DEFAULT_REGISTER_FLAG);
    packet->setNumberOfRegisters(RGSTR_NUMBER);

    packet->setCRC();
}