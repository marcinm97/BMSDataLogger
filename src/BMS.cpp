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

bool valid_bms_message(const bms_message& packet){
    const uint8_t* msgToValid = packet.getMessageToSend();

    if(msgToValid[HEADER_IDX] != HEADER_FLAG ||
       msgToValid[CMDCODE_IDX] != MESSAGE_TYPE)
        return false;

    const uint16_t rgstrNumber = TWO_UINT8_to_UINT16(msgToValid[RGSTR_LSB_IDX],
                                                     msgToValid[RGSTR_MSB_IDX]);
    // 0   <- min register
    // 599 - but minus number of registers, we wanna get <- max register

    if(!(rgstrNumber >= MIN_RGSTR &&
            rgstrNumber <= (MAX_RGSTR - msgToValid[RGSTR_NUMBER_IDX])))
        return false;

    if(!(msgToValid[RGSTR_NUMBER_IDX] > 0x00 && msgToValid[RGSTR_NUMBER_IDX] <= MAX_RGSTR_NUMBER))
        return false;

    return true;
}

void fill_bms_message_according_rgstr(bms_message* packet, uint16_t rgstr, uint8_t number){

    packet->setHeaderByte(HEADER_FLAG);
    packet->setMessageType(MESSAGE_TYPE);
    packet->setStartRegister(rgstr);
    packet->setNumberOfRegisters(number);

    packet->setCRC();
}