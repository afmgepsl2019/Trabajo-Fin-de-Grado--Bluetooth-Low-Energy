/***************************************************************************//**
* \file CYBLE_custom.h
* \version 3.60
* 
* \brief
*  Contains the function prototypes and constants for the Custom Service.
* 
********************************************************************************
* \copyright
* Copyright 2014-2019, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_BLE_CYBLE_CUSTOM_H)
#define CY_BLE_CYBLE_CUSTOM_H

#include "BLE_gatt.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Maximum supported Custom Services */
#define CYBLE_CUSTOMS_SERVICE_COUNT                  (0x02u)
#define CYBLE_CUSTOMC_SERVICE_COUNT                  (0x00u)
#define CYBLE_CUSTOM_SERVICE_CHAR_COUNT              (0x02u)
#define CYBLE_CUSTOM_SERVICE_CHAR_DESCRIPTORS_COUNT  (0x02u)

/* Below are the indexes and handles of the defined Custom Services and their characteristics */
#define CYBLE_DHT22_SERVICE_INDEX   (0x00u) /* Index of DHT22 service in the cyBle_customs array */
#define CYBLE_DHT22_HUMIDITY_CHAR_INDEX   (0x00u) /* Index of Humidity characteristic */
#define CYBLE_DHT22_HUMIDITY_HUMIDITYCCCD_DESC_INDEX   (0x00u) /* Index of Humiditycccd descriptor */
#define CYBLE_DHT22_HUMIDITY_CHARACTERISTIC_USER_DESCRIPTION_DESC_INDEX   (0x01u) /* Index of Characteristic User Description descriptor */
#define CYBLE_DHT22_TEMPERATURE_CHAR_INDEX   (0x01u) /* Index of Temperature characteristic */
#define CYBLE_DHT22_TEMPERATURE_TEMPERATURECCCD_DESC_INDEX   (0x00u) /* Index of Temperaturecccd descriptor */
#define CYBLE_DHT22_TEMPERATURE_CHARACTERISTIC_USER_DESCRIPTION_DESC_INDEX   (0x01u) /* Index of Characteristic User Description descriptor */

#define CYBLE_PROX_SENSOR_SERVICE_INDEX   (0x01u) /* Index of Prox_Sensor service in the cyBle_customs array */
#define CYBLE_PROX_SENSOR_PROXIMITY_CHAR_INDEX   (0x00u) /* Index of Proximity characteristic */
#define CYBLE_PROX_SENSOR_PROXIMITY_PROXIMITYCCCD_DESC_INDEX   (0x00u) /* Index of Proximitycccd descriptor */
#define CYBLE_PROX_SENSOR_PROXIMITY_CHARACTERISTIC_USER_DESCRIPTION_DESC_INDEX   (0x01u) /* Index of Characteristic User Description descriptor */


#define CYBLE_DHT22_SERVICE_HANDLE   (0x0010u) /* Handle of DHT22 service */
#define CYBLE_DHT22_HUMIDITY_DECL_HANDLE   (0x0011u) /* Handle of Humidity characteristic declaration */
#define CYBLE_DHT22_HUMIDITY_CHAR_HANDLE   (0x0012u) /* Handle of Humidity characteristic */
#define CYBLE_DHT22_HUMIDITY_HUMIDITYCCCD_DESC_HANDLE   (0x0013u) /* Handle of Humiditycccd descriptor */
#define CYBLE_DHT22_HUMIDITY_CHARACTERISTIC_USER_DESCRIPTION_DESC_HANDLE   (0x0014u) /* Handle of Characteristic User Description descriptor */
#define CYBLE_DHT22_TEMPERATURE_DECL_HANDLE   (0x0015u) /* Handle of Temperature characteristic declaration */
#define CYBLE_DHT22_TEMPERATURE_CHAR_HANDLE   (0x0016u) /* Handle of Temperature characteristic */
#define CYBLE_DHT22_TEMPERATURE_TEMPERATURECCCD_DESC_HANDLE   (0x0017u) /* Handle of Temperaturecccd descriptor */
#define CYBLE_DHT22_TEMPERATURE_CHARACTERISTIC_USER_DESCRIPTION_DESC_HANDLE   (0x0018u) /* Handle of Characteristic User Description descriptor */

#define CYBLE_PROX_SENSOR_SERVICE_HANDLE   (0x0019u) /* Handle of Prox_Sensor service */
#define CYBLE_PROX_SENSOR_PROXIMITY_DECL_HANDLE   (0x001Au) /* Handle of Proximity characteristic declaration */
#define CYBLE_PROX_SENSOR_PROXIMITY_CHAR_HANDLE   (0x001Bu) /* Handle of Proximity characteristic */
#define CYBLE_PROX_SENSOR_PROXIMITY_PROXIMITYCCCD_DESC_HANDLE   (0x001Cu) /* Handle of Proximitycccd descriptor */
#define CYBLE_PROX_SENSOR_PROXIMITY_CHARACTERISTIC_USER_DESCRIPTION_DESC_HANDLE   (0x001Du) /* Handle of Characteristic User Description descriptor */



#if(CYBLE_CUSTOMS_SERVICE_COUNT != 0u)
    #define CYBLE_CUSTOM_SERVER
#endif /* (CYBLE_CUSTOMS_SERVICE_COUNT != 0u) */
    
#if(CYBLE_CUSTOMC_SERVICE_COUNT != 0u)
    #define CYBLE_CUSTOM_CLIENT
#endif /* (CYBLE_CUSTOMC_SERVICE_COUNT != 0u) */

/***************************************
* Data Struct Definition
***************************************/

/**
 \addtogroup group_service_api_custom
 @{
*/

#ifdef CYBLE_CUSTOM_SERVER

/** Contains information about Custom Characteristic structure */
typedef struct
{
    /** Custom Characteristic handle */
    CYBLE_GATT_DB_ATTR_HANDLE_T customServCharHandle;
    /** Custom Characteristic Descriptors handles */
    CYBLE_GATT_DB_ATTR_HANDLE_T customServCharDesc[     /* MDK doesn't allow array with zero length */
        (CYBLE_CUSTOM_SERVICE_CHAR_DESCRIPTORS_COUNT == 0u) ? 1u : CYBLE_CUSTOM_SERVICE_CHAR_DESCRIPTORS_COUNT];
} CYBLE_CUSTOMS_INFO_T;

/** Structure with Custom Service attribute handles. */
typedef struct
{
    /** Handle of a Custom Service */
    CYBLE_GATT_DB_ATTR_HANDLE_T customServHandle;
    
    /** Information about Custom Characteristics */
    CYBLE_CUSTOMS_INFO_T customServInfo[                /* MDK doesn't allow array with zero length */
        (CYBLE_CUSTOM_SERVICE_CHAR_COUNT == 0u) ? 1u : CYBLE_CUSTOM_SERVICE_CHAR_COUNT];
} CYBLE_CUSTOMS_T;


#endif /* (CYBLE_CUSTOM_SERVER) */

#ifdef CYBLE_CUSTOM_CLIENT

/** Structure with discovered attributes information of Custom Service Descriptors */
typedef struct
{
    /** Custom Descriptor handle */
    CYBLE_GATT_DB_ATTR_HANDLE_T descHandle;
	/** Custom Descriptor 128 bit UUID */
	const void *uuid;           
    /** UUID Format - 16-bit (0x01) or 128-bit (0x02) */
	uint8 uuidFormat;
   
} CYBLE_CUSTOMC_DESC_T;

/** Structure with discovered attributes information of Custom Service Characteristics */
typedef struct
{
    /** Characteristic handle */
    CYBLE_GATT_DB_ATTR_HANDLE_T customServCharHandle;
	/** Characteristic end handle */
    CYBLE_GATT_DB_ATTR_HANDLE_T customServCharEndHandle;
	/** Custom Characteristic UUID */
	const void *uuid;           
    /** UUID Format - 16-bit (0x01) or 128-bit (0x02) */
	uint8 uuidFormat;
    /** Properties for value field */
    uint8  properties;
	/** Number of descriptors */
    uint8 descCount;
    /** Characteristic Descriptors */
    CYBLE_CUSTOMC_DESC_T * customServCharDesc;
} CYBLE_CUSTOMC_CHAR_T;

/** Structure with discovered attributes information of Custom Service */
typedef struct
{
    /** Custom Service handle */
    CYBLE_GATT_DB_ATTR_HANDLE_T customServHandle;
	/** Custom Service UUID */
	const void *uuid;           
    /** UUID Format - 16-bit (0x01) or 128-bit (0x02) */
	uint8 uuidFormat;
	/** Number of characteristics */
    uint8 charCount;
    /** Custom Service Characteristics */
    CYBLE_CUSTOMC_CHAR_T * customServChar;
} CYBLE_CUSTOMC_T;

#endif /* (CYBLE_CUSTOM_CLIENT) */


/***************************************
* External data references 
***************************************/

#ifdef CYBLE_CUSTOM_SERVER

/** Custom Services GATT DB handles structures */
extern const CYBLE_CUSTOMS_T cyBle_customs[CYBLE_CUSTOMS_SERVICE_COUNT];

#endif /* (CYBLE_CUSTOM_SERVER) */

#ifdef CYBLE_CUSTOM_CLIENT

/** Custom Services discovered attributes information */
extern CYBLE_CUSTOMC_T cyBle_customCServ[CYBLE_CUSTOMC_SERVICE_COUNT];

#endif /* (CYBLE_CUSTOM_CLIENT) */

/***************************************
* Private Function Prototypes
***************************************/

/** \cond IGNORE */
void CyBle_CustomInit(void);

#ifdef CYBLE_CUSTOM_CLIENT

void CyBle_CustomcDiscoverServiceEventHandler(const CYBLE_DISC_SRVC128_INFO_T *discServInfo);
void CyBle_CustomcDiscoverCharacteristicsEventHandler(uint16 discoveryService, const CYBLE_DISC_CHAR_INFO_T *discCharInfo);
CYBLE_GATT_ATTR_HANDLE_RANGE_T CyBle_CustomcGetCharRange(uint8 incrementIndex);
void CyBle_CustomcDiscoverCharDescriptorsEventHandler(const CYBLE_DISC_DESCR_INFO_T *discDescrInfo);

#endif /* (CYBLE_CUSTOM_CLIENT) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/
#define customServiceCharHandle         customServCharHandle
#define customServiceCharDescriptors    customServCharDesc
#define customServiceHandle             customServHandle
#define customServiceInfo               customServInfo
/** \endcond */

/** @} */

#endif /* CY_BLE_CYBLE_CUSTOM_H  */

/* [] END OF FILE */
