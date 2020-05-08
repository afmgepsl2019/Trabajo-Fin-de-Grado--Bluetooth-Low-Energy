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

import java.util.HashMap;

/**
 * This class includes a small subset of standard GATT attributes for demonstration purposes.
 */
public class SampleGattAttributes {
    private static HashMap<String, String> attributes = new HashMap();

    public static String DHT22_SERVICE = "51F76976-E4F8-4D9A-AA70-3168CC7AF890";
    public static String PROXIMITY_SERVICE = "1C9C78F8-DD31-4408-8CA4-766C8BE7C260";


    public static String HUMIDITY_MEASUREMENT = "51F76976-E4F8-4D9A-AA70-3168CC7AF891";
    public static String CLIENT_CHARACTERISTIC_CONFIG_HUM = "00002902-0000-1000-8000-00805F9B34FB";

    public static String TEMPERATURE_MEASUREMENT = "51F76976-E4F8-4D9A-AA70-3168CC7AF892";
    public static String CLIENT_CHARACTERISTIC_CONFIG_TEMP = "00002902-0000-1000-8001-0805F9B34FB0";

    public static String PROXIMITY_MEASUREMENT = "1C9C78F8-DD31-4408-8CA4-766C8BE7C261";
    public static String CLIENT_CHARACTERISTIC_CONFIG_PROX = "00002902-0000-1000-8002-00805F9B34FB";

    static {
        // Sample Services.
        attributes.put(DHT22_SERVICE, "Servicio sensor DHT22");
        attributes.put(PROXIMITY_SERVICE, "Servicio sensor proximidad");
        // Sample Characteristics.
        attributes.put(HUMIDITY_MEASUREMENT, "Medida humedad");
        attributes.put(CLIENT_CHARACTERISTIC_CONFIG_HUM, "CCCD Humedad");
        attributes.put(TEMPERATURE_MEASUREMENT, "Medida temperatura");
        attributes.put(CLIENT_CHARACTERISTIC_CONFIG_TEMP, "CCCD Temperatura");
        attributes.put(PROXIMITY_MEASUREMENT, "Medida proximidad");
        attributes.put(CLIENT_CHARACTERISTIC_CONFIG_PROX, "CCCD Proximidad");
    }

    public static String lookup(String uuid, String defaultName) {
        String name = attributes.get(uuid);
        return name == null ? defaultName : name;
    }
}
