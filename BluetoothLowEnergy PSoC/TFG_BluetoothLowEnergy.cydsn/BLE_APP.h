/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "project.h"

#define ERR_INVALID_PDU         0X04

uint8 startNotificationHumidity;
uint8 startNotificationTemperature;
uint8 startNotificationProximity;
extern uint8 deviceConnected;

void BLECallBack(uint32 event, void * eventParam);

/* [] END OF FILE */