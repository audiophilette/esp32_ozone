/*!
 * 
 * Candace Groenhke <audiophilette@protonmail.com>
 * (Pronounced "GRUE-nick" because nobody gets it right)
 *
 * with code stolen directly from the DFRobot Sample files
 * https://github.com/DFRobot/DFRobot_OzoneSensor
 * 
 * as well as https://deepbluembedded.com/
 *  
 * put your influxdb info in the "arduino secrets.h" file
 * as well as your wifi SSID and password.
 */

#include <esp_wifi.h>
#include <InfluxDbClient.h>
#include "driver/i2c.h"
#include "DFRobot_OzoneSensor.h"
#include "arduino_secrets.h"

#define I2C_MASTER_SCL_IO 36               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 33               /*!< gpio number for I2C master data  */

/// influx config here ///


// sensor config
Point sensor(INFLUXDB_MEASUREMENT);


// InfluxDB v1 database name 
//#define INFLUXDB_DB_NAME "database"

// InfluxDB client instance
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
// InfluxDB client instance for InfluxDB 1
// InfluxDBClient client(INFLUXDB_URL, INFLUXDB_DB_NAME);

#define COLLECT_NUMBER  90              // collect number, the collection range is 1-100
/*
 * select i2c device address 
 *   OZONE_ADDRESS_0  0x70
 *   OZONE_ADDRESS_1  0x71
 *   OZONE_ADDRESS_2  0x72
 *   OZONE_ADDRESS_3  0x73
 */
#define Ozone_IICAddress OZONE_ADDRESS_3
DFRobot_OzoneSensor Ozone;

void ConnectedToAP_Handler(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  Serial.println("Connected To The WiFi Network");
}
 
void GotIP_Handler(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}
 
void WiFi_Disconnected_Handler(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  Serial.println("Disconnected From WiFi Network");
  // Attempt Re-Connection
  WiFi.begin(SECRET_SSID, SECRET_PASS);
}

void setup() 
{
  Serial.begin(115200);
  while(!Ozone.begin(Ozone_IICAddress)){
    Serial.println("I2c device number error !");
    delay(1000);
  }
  Serial.println("I2c connect success !");

  /**
   * set measuer mode
   * MEASURE_MODE_AUTOMATIC         active  mode
   * MEASURE_MODE_PASSIVE           passive mode
   */
    Ozone.setModes(MEASURE_MODE_PASSIVE);



// WIFI STUFF
// check for the WiFi module:

  WiFi.mode(WIFI_STA);
  WiFi.onEvent(ConnectedToAP_Handler, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(GotIP_Handler, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFi_Disconnected_Handler, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  Serial.println("Connecting to WiFi Network ..");
}

void loop() 
{
  
  /**
   * Smooth data collection
   * COLLECT_NUMBER                 The collection range is 1-100
   */
  float ozoneConcentration = Ozone.readOzoneData(COLLECT_NUMBER);
  Serial.print("Ozone concentration is ");
  Serial.print(ozoneConcentration);
  Serial.println(" PPB.");

  // Store measured value into point
  sensor.clearFields();
  // Report RSSI of currently connected network
  sensor.addField(OZONE_FIELD, ozoneConcentration);
  // Print what are we exactly writing
  Serial.print("Writing: ");
  Serial.println(client.pointToLineProtocol(sensor));
  // Write point
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }


  delay(1000);
}
