/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.android.bluetoothlegatt;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;
import android.widget.TextView;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.UUID;

import static com.example.android.bluetoothlegatt.SampleGattAttributes.CLIENT_CHARACTERISTIC_CONFIG_HUM;
import static com.example.android.bluetoothlegatt.SampleGattAttributes.CLIENT_CHARACTERISTIC_CONFIG_TEMP;
import static com.example.android.bluetoothlegatt.SampleGattAttributes.CLIENT_CHARACTERISTIC_CONFIG_PROX;


/**
 * Servicio para gestionar la conexión y la comunicación de datos con un servidor GATT
 * alojado en un determinado dispositivo Bluetooth LE.
 */
public class BluetoothLeService extends Service {
    private final static String TAG = BluetoothLeService.class.getSimpleName();

    private BluetoothManager mBluetoothManager;
    private BluetoothAdapter mBluetoothAdapter;
    private String mBluetoothDeviceAddress;
    private BluetoothGatt mBluetoothGatt;
    private int mConnectionState = STATE_DISCONNECTED;

    private static final int STATE_DISCONNECTED = 0;
    private static final int STATE_CONNECTING = 1;
    private static final int STATE_CONNECTED = 2;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*
    private static String mDHT22ValueHum = "-1";
    private static String mDHT22ValueTemp = "-1";
    private static String mValueProx = "-1";
*/
    //Fecha y Hora
    Handler hand =  new Handler();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    public final static String ACTION_GATT_CONNECTED =
            "com.example.bluetooth.le.ACTION_GATT_CONNECTED";
    public final static String ACTION_GATT_DISCONNECTED =
            "com.example.bluetooth.le.ACTION_GATT_DISCONNECTED";
    public final static String ACTION_GATT_SERVICES_DISCOVERED =
            "com.example.bluetooth.le.ACTION_GATT_SERVICES_DISCOVERED";
    public final static String ACTION_DATA_AVAILABLE =
            "com.example.bluetooth.le.ACTION_DATA_AVAILABLE";
    public final static String EXTRA_DATA =
            "com.example.bluetooth.le.EXTRA_DATA";

    public final static UUID UUID_HUMIDITY_MEASUREMENT =
            UUID.fromString(SampleGattAttributes.HUMIDITY_MEASUREMENT);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    public final static UUID UUID_TEMPERATURE_MEASUREMENT =
            UUID.fromString(SampleGattAttributes.TEMPERATURE_MEASUREMENT);

    public final static UUID UUID_PROXIMITY_MEASUREMENT =
            UUID.fromString(SampleGattAttributes.PROXIMITY_MEASUREMENT);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Implementa métodos de devolución de llamada para eventos GATT que le interesan a la aplicación.
    // Por ejemplo, cambio de conexión y servicios descubiertos.
    private final BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            String intentAction;
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                intentAction = ACTION_GATT_CONNECTED;
                mConnectionState = STATE_CONNECTED;
                broadcastUpdate(intentAction);
                Log.i(TAG, "Conectado al servidor GATT.");
                // Attempts to discover services after successful connection.
                Log.i(TAG, "Intentando iniciar el descubrimiento del servicio:" +
                        mBluetoothGatt.discoverServices());

            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                intentAction = ACTION_GATT_DISCONNECTED;
                mConnectionState = STATE_DISCONNECTED;
                Log.i(TAG, "Desconectado del servidor GATT.");
                broadcastUpdate(intentAction);
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                broadcastUpdate(ACTION_GATT_SERVICES_DISCOVERED);
            } else {
                Log.w(TAG, "onServicesDiscovered recivido: " + status);
            }
        }

        @Override
        public void onCharacteristicRead(BluetoothGatt gatt,
                                         BluetoothGattCharacteristic characteristic,
                                         int status) {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                broadcastUpdate(ACTION_DATA_AVAILABLE, characteristic);
            }
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt,
                                            BluetoothGattCharacteristic characteristic) {
            broadcastUpdate(ACTION_DATA_AVAILABLE, characteristic);
        }
    };

    private void broadcastUpdate(final String action) {
        final Intent intent = new Intent(action);
        sendBroadcast(intent);
    }

    private void broadcastUpdate(final String action,
                                 final BluetoothGattCharacteristic characteristic) {
        final Intent intent = new Intent(action);
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SimpleDateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");
        Date date = new Date();
        String fechahora = dateFormat.format(date);
        //hand.postDelayed(this, 1000);
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



        if (UUID_HUMIDITY_MEASUREMENT.equals(characteristic.getUuid())){
            final int mDHT22ValueHum = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_SINT16, 0);
            intent.putExtra(EXTRA_DATA, String.valueOf(mDHT22ValueHum) + "% -- " + fechahora);
        }
        if (UUID_TEMPERATURE_MEASUREMENT.equals(characteristic.getUuid())){
            final int mDHT22ValueTemp = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_SINT16, 0);
            intent.putExtra(EXTRA_DATA, String.valueOf(mDHT22ValueTemp) + "ºC -- " + fechahora);
        }
        if (UUID_PROXIMITY_MEASUREMENT.equals(characteristic.getUuid())){
            final int mValueProx = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_SINT16, 0);
            intent.putExtra(EXTRA_DATA, String.valueOf(mValueProx) + " -- " + fechahora);
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
        else {
            // Para los demas perfiles escribir los datos en forma hexadecimal
            final byte[] data = characteristic.getValue();
            if (data != null && data.length > 0) {
                final StringBuilder stringBuilder = new StringBuilder(data.length);
                for(byte byteChar : data)
                    stringBuilder.append(String.format("%02X ", byteChar));
                intent.putExtra(EXTRA_DATA, new String(data) + "\n" + stringBuilder.toString());
            }
        }
*/
        sendBroadcast(intent);
    }

    public class LocalBinder extends Binder {
        BluetoothLeService getService() {
            return BluetoothLeService.this;
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    @Override
    public boolean onUnbind(Intent intent) {

        // Después de usar un dispositivo determinado, debe asegurarse de que se llame a BluetoothGatt.close ()
        // de modo que los recursos se limpien correctamente.
        // En este ejemplo particular, se invoca close () cuando la IU se desconecta del Servicio.
        close();
        return super.onUnbind(intent);
    }

    private final IBinder mBinder = new LocalBinder();

    /**
     * Initializes a reference to the local Bluetooth adapter.
     *
     * @return Return true if the initialization is successful.
     */
    public boolean initialize() {
        //Para API nivel 18 y superior, obtenga una referencia a BluetoothAdapter a través de BluetoothManager.
        if (mBluetoothManager == null) {
            mBluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
            if (mBluetoothManager == null) {
                Log.e(TAG, "No se puede inicializar BluetoothManager.");
                return false;
            }
        }

        mBluetoothAdapter = mBluetoothManager.getAdapter();
        if (mBluetoothAdapter == null) {
            Log.e(TAG, "No se puede obtener un adaptador Bluetooth.");
            return false;
        }

        return true;
    }

    /**
     * Se conecta al servidor GATT alojado en el dispositivo Bluetooth LE.
     *
     * @param address La dirección del dispositivo del dispositivo de destino.
     *
     * @return Devuelve verdadero si la conexión se inicia con éxito.
     * El resultado de la conexión se informa de forma asincrónica a través de la devolución de llamada
     * {@code BluetoothGattCallback # onConnectionStateChange (android.bluetooth.BluetoothGatt, int, int)}.
     */
    public boolean connect(final String address) {
        if (mBluetoothAdapter == null || address == null) {
            Log.w(TAG, "Adaptador Bluetooth no inicializado o dirección no especificada.");
            return false;
        }

        // Dispositivo previamente conectado. Intenta reconectarte.
        if (mBluetoothDeviceAddress != null && address.equals(mBluetoothDeviceAddress)
                && mBluetoothGatt != null) {
            Log.d(TAG, "Intentando usar un mBluetoothGatt existente para la conexión.");
            if (mBluetoothGatt.connect()) {
                mConnectionState = STATE_CONNECTING;
                return true;
            } else {
                return false;
            }
        }

        final BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        if (device == null) {
            Log.w(TAG, "Dispositivo no encontrado. No puede conectarse.");
            return false;
        }
        // No conectar directamente al dispositivo, por lo que se configura el parámetro autoConnect en falso.
        mBluetoothGatt = device.connectGatt(this, false, mGattCallback);
        Log.d(TAG, "Trying to create a new connection.");
        mBluetoothDeviceAddress = address;
        mConnectionState = STATE_CONNECTING;
        return true;
    }

    /**
     * Desconecta una conexión existente o cancela una conexión pendiente.
     * El resultado de la desconexión se informa de forma asíncrona a través de la devolución de llamada
     * {@code BluetoothGattCallback # onConnectionStateChange (android.bluetooth.BluetoothGatt, int, int)}.
     */
    public void disconnect() {
        if (mBluetoothAdapter == null || mBluetoothGatt == null) {
            Log.w(TAG, "BluetoothAdapter no inicializado");
            return;
        }
        mBluetoothGatt.disconnect();
    }

    /**
     * Después de usar un dispositivo BLE determinado, la aplicación debe llamar a este método para
     * garantizar que los recursos se liberen correctamente.
     */
    public void close() {
        if (mBluetoothGatt == null) {
            return;
        }
        mBluetoothGatt.close();
        mBluetoothGatt = null;
    }

    /**
     *
     * Solicite una lectura en una {@code BluetoothGattCharacteristic} dada.
     * El resultado de la lectura se informa de forma asincrónica a través de la devolución de llamada
     * {@code BluetoothGattCallback # onCharacteristicRead (android.bluetooth.BluetoothGatt, android.bluetooth.BluetoothGattCharacteristic, int)}.
     *
     * @param characteristic La característica para leer.
     */
    public void readCharacteristic(BluetoothGattCharacteristic characteristic) {
        if (mBluetoothAdapter == null || mBluetoothGatt == null) {
            Log.w(TAG, "BluetoothAdapter no inicializado");
            return;
        }
        mBluetoothGatt.readCharacteristic(characteristic);
    }

    /**
     * Habilita o deshabilita la notificación en una característica de entrega.
     *
     * @param characteristic Característica para actuar.
     * @param enabled Si es verdadero, habilite la notificación. Falso de lo contrario.
     */
    public void setCharacteristicNotification(BluetoothGattCharacteristic characteristic,
                                              boolean enabled) {
        if (mBluetoothAdapter == null || mBluetoothGatt == null) {
            Log.w(TAG, "BluetoothAdapter no inicializado");
            return;
        }
        mBluetoothGatt.setCharacteristicNotification(characteristic, enabled);
    //}

        if (UUID_HUMIDITY_MEASUREMENT.equals(characteristic.getUuid())
        || UUID_TEMPERATURE_MEASUREMENT.equals(characteristic.getUuid())
        || UUID_PROXIMITY_MEASUREMENT.equals(characteristic.getUuid())) {
            BluetoothGattDescriptor descriptor = characteristic.getDescriptor(
                    UUID.fromString(CLIENT_CHARACTERISTIC_CONFIG_HUM));
            descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
            mBluetoothGatt.writeDescriptor(descriptor);
        }
    }


    /**
     * Recupera una lista de servicios GATT compatibles en el dispositivo conectado.
     * Esto debe invocarse solo después de que {@code BluetoothGatt # discoverServices ()} se complete con éxito.
     *
     * @return Una {@code List} de servicios compatibles.
     */
    public List<BluetoothGattService> getSupportedGattServices() {
        if (mBluetoothGatt == null) return null;

        return mBluetoothGatt.getServices();
    }
}
