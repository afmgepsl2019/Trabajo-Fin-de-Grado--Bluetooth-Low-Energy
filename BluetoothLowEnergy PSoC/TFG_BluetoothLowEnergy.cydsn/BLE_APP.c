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
#include "main.h"
#include "BLE_APP.h"
#include "LiquidCrystal_I2C.h"

// Variables referentes a las notificaciones de las características y de la conexión del dispositivo.
uint8 startNotificationHumidity;
uint8 startNotificationTemperature;
uint8 startNotificationProximity;
uint8 deviceConnected = 0;

CYBLE_CONN_HANDLE_T connectionHandle;

void BLECallBack ( uint32 event, void *eventParam )       //Función de devolución de llamada BLE.
{
    
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParamHumidity;    //Mantener los comandos enviados por el cliente GATT.
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParamTemperature; //Mantener los comandos enviados por el cliente GATT.
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParamProximity;   //Mantener los comandos enviados por el cliente GATT.
    
    switch (event) 
    {
    //Manejar eventos de conexión.
        
        
        /**********************************************************
        *                     Eventos Generales
        ***********************************************************/
        
        case CYBLE_EVT_STACK_ON:
        
        
        /**********************************************************
        *                       Eventos GAP
        ***********************************************************/
        
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:           // Estado de desconexión del dispositivo.
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);  // Reiniciar publicidad.
            deviceConnected = 0;                          // Conexión del dispositivo, no activa.
            clear();                                      // Se limpia la LCD.
            setCursor(0,0);                               // Primera linea LCD.
            LCD_print("    CONEXION");                    // Se imprime en la LCD.
            setCursor(0,1);                               // Segunda linea LCD.
            LCD_print(" NO ESTABLECIDA");                 // Se imprime en la LCD.
            break;
            
        /**********************************************************
        *                       Eventos GATT
        ***********************************************************/
            
        case CYBLE_EVT_GATT_CONNECT_IND:                   // Estado de conexión establecida.
            connectionHandle = *(CYBLE_CONN_HANDLE_T *)eventParam;
            deviceConnected = 1;                           // Conexión del dispositivo, activa.
            clear();                                       // Se limpia la LCD.
            LCD_print("    CONEXION");                     // Se imprime en la LCD.
            setCursor(0,1);                                // Segunda linea LCD.
            LCD_print("  ESTABLECIDA");                    // Se imprime en la LCD.
            break;
        
        case CYBLE_EVT_GATTS_WRITE_REQ:                    // Estado de escritura con respuesta.
            
            //Variables para activar la notificación de las características.
            wrReqParamHumidity = (CYBLE_GATTS_WRITE_REQ_PARAM_T *)eventParam;
            wrReqParamTemperature = (CYBLE_GATTS_WRITE_REQ_PARAM_T *)eventParam;
            wrReqParamProximity = (CYBLE_GATTS_WRITE_REQ_PARAM_T *)eventParam;
            
            //Proceso para activar la notificación de la característica de la humedad.
            if (wrReqParamHumidity-> handleValPair.attrHandle == CYBLE_DHT22_HUMIDITY_HUMIDITYCCCD_DESC_HANDLE) 
            {
                CyBle_GattsWriteAttributeValue(&wrReqParamHumidity -> handleValPair, 0, &connectionHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
                startNotificationHumidity = wrReqParamHumidity-> handleValPair.value.val[0];
            }
            
            
            //Proceso para activar la notificación de la característica de la temperatura. 
            else if (wrReqParamTemperature-> handleValPair.attrHandle == CYBLE_DHT22_TEMPERATURE_TEMPERATURECCCD_DESC_HANDLE) 
            {
                CyBle_GattsWriteAttributeValue(&wrReqParamTemperature -> handleValPair, 0, &connectionHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
                startNotificationTemperature = wrReqParamTemperature-> handleValPair.value.val[0];
            }
            
            
            //Proceso para activar la notificación de la característica de la proximidad. 
            else if (wrReqParamProximity-> handleValPair.attrHandle == CYBLE_PROX_SENSOR_PROXIMITY_PROXIMITYCCCD_DESC_HANDLE) 
            {
                CyBle_GattsWriteAttributeValue(&wrReqParamProximity -> handleValPair, 0, &connectionHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
                startNotificationProximity = wrReqParamProximity-> handleValPair.value.val[0];
            }
            
            
            CyBle_GattsWriteRsp(connectionHandle);
            break;
            
            default:
            break;            
    }       
}

/* [] END OF FILE */
