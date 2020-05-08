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
#include "stdio.h"
#include "main.h"
#include "BLE_APP.h" 
#include "LiquidCrystal_I2C.h" //Librería de la LCD

uint32_t Addr = 0x27; //Dirección de la LCD usada

//Variables del sensor de temperatura y humedad DHT22
static int temperatura=99;     //Temperatura total
static int temperatura1=0;     //Primer Byte de temperatura
static int temperatura2=0;     //Segundo Byte de temperatura
static int temperaturaant=200; //Temperatura inicial para la comparación

static int humedad=99;        //Humedad total
static int humedad1=0;        //Primer Byte de humedad
static int humedad2=0;        //Segundo Byte de humedad
static int humedadant=150;    //Humedad inicial para la comparación

//Variables del sensor de proximidad
extern uint16 CapSense_SensorSignal[CapSense_TOTAL_SENSOR_COUNT];

#define PROX_RANGE_INIT     50 //Proximidad máxima que se puede medir

//Variables conexión BLE
CYBLE_GATTS_HANDLE_VALUE_NTF_T notificationHandle;  //Humedad
CYBLE_GATTS_HANDLE_VALUE_NTF_T notificationHandle1; //Temperatura
CYBLE_GATTS_HANDLE_VALUE_NTF_T notificationHandle2; //Proximidad
CYBLE_CONN_HANDLE_T connectionHandle;

int lectura_DHT22(){                        //Obtener los datos de temperatura y humedad del sensor DHT22  
    
    //Variables para obtener datos de temperatura y humedad
    uint8 IState;
    uint8 bytes[5]; 
    uint8 cnt = 7; 
    uint8 idx = 0; 
    
    int i;
    int calc=0; 
    int timeout=0; 
    
    IState=CyEnterCriticalSection();  //Desactiva las interrupciones y devuelve un valor de estado de interrupción
   
    //Se pone a 0 los bytes donde se van a almacenar los datos de humedad y temperatura.
    for (i=0; i< 5; i++)
        bytes[i] = 0; 
                        
    /*Se realiza el "timing" del sensor DHT22, donde 1 representará cuando está arriba y 0 cuando está abajo, 
    por configuración previa comienza en 1.*/
        
    DHT_Write(0u);                    //Se cambia el valor a 0.
    CyDelay(19);                      //Estará en 0, 19 milisegundos.
    DHT_Write(1u);                    //Se cambia el valor a 1.
    
    while(DHT_Read()==1)              //Mientras que este en 1.
    { 
       timeout++;                     //La variable "timeout" aumenta.
       if(timeout>500)                //Si está en 1 más de 500 milisegundos, anuncia un error.
           goto r99;                  //Error de funcionamiento del sensor DHT22.
    } 
    
    while(DHT_Read()==0)              //Si en el proceso anterior no da error, el propio sensor debe cambiar el valor a 0.
    {         
       timeout++;                     //Una vez se cambie, la variable "timeout" aumenta de nuevo.
       if(timeout>500)                //Si está en 0 más de 500 milisegundos, anuncia un error.
           goto r99;                  //Error de funcionamiento del sensor DHT22.
    } 
    
    
    calc=timeout;                     // Si en el proceso anterior no da error, se iguala el valor de "timeout" a "calc".
    timeout=0;                        // Se actualiza el valor de "timeout" a 0.
    
    while(DHT_Read()==1);             // El sensor cambia otra vez el valor a 1 y comienza la lectura de datos.
    
    for (i=0; i<40; i++) { 
       timeout=0; 
       while(DHT_Read()==0); 
       while(DHT_Read()==1) 
           timeout++; 
       
       if ((timeout) > (calc/2)){ 
        bytes[idx] |= (1 << cnt); 
        }
    
       if (cnt == 0){
   		cnt = 7;    
   		idx++;      
   	    } 
    
   	    else cnt--; 
    } 
    
   //Se asignan los bytes obtenidos a los bytes que pertenecen a humedad y a temperatura.
    humedad1    = bytes[0]; 
    humedad2    = bytes[1]; 
    temperatura1 = bytes[2]; 
    temperatura2 = bytes[3]; 
    
    humedad     = humedad1 * 256 + humedad2;          //Cálculo humedad final.
    temperatura  = temperatura1 * 256 + temperatura2; //Cálculo temperatura final.
    
    CyExitCriticalSection(IState);                    //Restaura el estado de interrupción.
    
    CyDelay(1); 
    return 0;
        
    r99:                                              //Error en la lectura del sensor DHT22.
      
       humedad    = 99;                               //Al producirse un error la humedad toma el valor 99;
       temperatura = 99;                              //Al producirse un error la temperatura toma el valor 99;
       CyExitCriticalSection(IState);                 //Restaura el estado de interrupción.
       return 99; 
}

int main(void)
{
    //Definición de variables de proximidad
    int16 proximidad, max_proximidad; 
    int16 proximidadant=150;

    CyGlobalIntEnable;                                //Habilitar interruptores globales. 
    
    //Definición de variables tipo "char" usados para mostrar los datos leidos.
    char outputstring[40];                            //Humedad
    char outputstring1[40];                           //Temperatura
    char outputstring2[40];                           //Proximidad
    
    I2C_Start();                                      //Inicializamos I2C
    
    LiquidCrystal_I2C_init(Addr,16,2,0);              //Se inicializa la LCD.
   
    begin();                                          //Se activa la LCD
    
    CapSense_Start();                                 //Se inicia el "capsense" usado para medir proximidad.
        
    CyGlobalIntEnable;                                //Habilitar interruptores globales. 
    
    //Definición de funciones usadas para el sensor de proximidad.
    CapSense_EnableWidget(CapSense_SENSOR_PROXIMITYSENSOR0_0__PROX);
    CapSense_InitializeAllBaselines();
    CapSense_UpdateEnabledBaselines();
    CapSense_ScanEnabledWidgets();
    while(CapSense_IsBusy()!=0);
    
    max_proximidad = PROX_RANGE_INIT;                 // Se asigna a la variable "max_proximidad" la máxima que se puede medir (50).
    
    
    CyBle_Start(BLECallBack);                         // Se realiza la llamada al BLE.
    
    for(;;)
    { 
        CyBle_ProcessEvents();
        
        CapSense_UpdateEnabledBaselines();
        CapSense_ScanEnabledWidgets();
        
        while(CapSense_IsBusy()!=0);                      // Mientras capsense esté activo.
    
        proximidad = CapSense_SensorSignal[0];            // Se guarda en la variable "proximidad" el dato leido por el sensor.
        
        if(proximidad<=0){                                // Si la proximidad leida es menor que 0.
            proximidad=0;                                 // La proximidad que muestra el sensor es 0.
        }
        if(proximidad>max_proximidad){                    // Si la proximidad leida es mayor que el máximo (50).
            proximidad=max_proximidad;                    // La proximidad que muestra el sensor es 50.
        }
               
         
        lectura_DHT22();                                  // Se llama a la función de lectura del DHT22.
        
        if(deviceConnected & startNotificationHumidity){  // Si el dispositivo se conecta y se notifica en la característica de humedad.    
            
            humedad = humedad / 10;                       // Se calcula la humedad final que se manda por BLE.
            
            if (humedad!=humedadant){                     // Si la humedad leida no es igual al dato almacenado en la variable "humedadant". 
                clear();                                  // Se limpia lo que haya en la LCD.
                humedadant=humedad;                       // Se asigna el valor que se va a enviar por BLE a la variable "humedadant".
                setCursor(0,0);                               // Primera linea LCD.
                LCD_print("  HUMEDAD (%)");                    // Se imprime en la LCD.
                sprintf(outputstring, "      %.2i ", humedadant);  // Se muestra en la LCD el valor.
                setCursor(0,1);                           //Primera linea del LCD
                LCD_print(outputstring);  
            
                //Transmisión de datos de humedad por BLE
                notificationHandle.attrHandle = CYBLE_DHT22_HUMIDITY_CHAR_HANDLE;
                notificationHandle.value.val = (uint8*) &humedadant;
                notificationHandle.value.len = 2;
            
                CyBle_GattsNotification(connectionHandle, &notificationHandle);
                CyDelay(1000);
            
                CyBle_GattsWriteAttributeValue(&notificationHandle,0,&connectionHandle, 0);
            }
            
            else if(humedad==humedadant){                   // Si el dato leido es igual al dato almacenado en "humedadant".
                lectura_DHT22();                            // Se vuelve a leer el dato sin mandar nada.
                CyDelay(1000);                              // Retardo de 1 segundo.
            }
        }
        
        else if(deviceConnected & startNotificationTemperature){ // Si el dispositivo se conecta y se notifica en la característica de temperatura.    
            
            temperatura = temperatura / 10;                 // Se calcula la temperatura final que se manda por BLE.
            
            if (temperatura!=temperaturaant){               // Si la temperatura leida no es igual al dato almacenado en la variable "temperaturaant".
                clear();                                    // Se limpia lo que haya en la LCD.
                temperaturaant=temperatura;                 // Se asigna el valor que se va a enviar por BLE a la variable "temperaturaant".
                setCursor(0,0);                               // Primera linea LCD.
                LCD_print("TEMPERATURA (oC)");                    // Se imprime en la LCD.
                sprintf(outputstring1, "      %.2i", temperaturaant);  // Se muestra en la LCD el valor.
                setCursor(0,1);                             //Primera linea LCD. 
                LCD_print(outputstring1);
            
                //Transmisión de datos de temperatura por BLE
                notificationHandle1.attrHandle = CYBLE_DHT22_TEMPERATURE_CHAR_HANDLE;
                notificationHandle1.value.val = (uint8*) &temperaturaant;
                notificationHandle1.value.len = 2;
            
                CyBle_GattsWriteAttributeValue(&notificationHandle1,0,&connectionHandle, 0);
            
                CyBle_GattsNotification(connectionHandle, &notificationHandle1);
                CyDelay(1000);
            }
            else if (temperatura==temperaturaant){          // Si el dato leido es igual al dato almacenado en "temperaturaant".
                lectura_DHT22();                            // Se vuelve a leer el dato sin mandar nada.
                CyDelay(1000);                              // Retardo de 1 segundo.
            }
        }
        
        else if(deviceConnected & startNotificationProximity){ //Solo si cumple esto el sensor captara los datos.
            
            if (proximidad!=proximidadant){                 // Si el dispositivo se conecta y se notifica en la característica de proximidad.    
            
                proximidadant=proximidad;                   // Si la proximidad leida no es igual al dato almacenado en la variable "proximidadant".
                clear();                                    // Se limpia lo que haya en la LCD.
                setCursor(0,0);                               // Primera linea LCD.
                LCD_print("   PROXIMIDAD ");                    // Se imprime en la LCD.
                sprintf(outputstring2, "       %d ", proximidadant); // Se muestra en la LCD el valor.
                setCursor(0,1);                             //Primera linea LCD.
                LCD_print(outputstring2);
            
                //Transmisión de datos de proximidad por BLE.
                notificationHandle2.attrHandle = CYBLE_PROX_SENSOR_PROXIMITY_CHAR_HANDLE;
                notificationHandle2.value.val = (uint8*) &proximidadant;
                notificationHandle2.value.len = 2;
            
                CyBle_GattsWriteAttributeValue(&notificationHandle2,0,&connectionHandle, 0);
            
                CyBle_GattsNotification(connectionHandle, &notificationHandle2);
            }
            else if (proximidad==proximidadant){            // Si el dato leido es igual al dato almacenado en "proximidadant".
                lectura_DHT22();                            // Se vuelve a leer el dato sin mandar nada.
                //CyDelay(1000);                            // Retardo de 1 segundo.
            }
        }
    }
}


/* [] END OF FILE */
