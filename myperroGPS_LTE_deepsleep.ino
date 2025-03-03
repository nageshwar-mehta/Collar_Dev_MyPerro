#include <WiFi.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
//functions :
void enterDeepSleep() {
    Serial.println("Entering Deep Sleep Mode...");
    esp_sleep_enable_timer_wakeup(30000000); // Sleep for 30 seconds , taking value in micro secs
    esp_deep_sleep_start();
}
void sendLocationOverLTE(float lat, float lon) {
    String locationMsg = "Pet Location: " + String(lat, 6) + ", " + String(lon, 6);
    lteSerial.println("AT+CMGF=1"); // Set SMS mode
    delay(100);
    lteSerial.println("AT+CMGS=\"+919103553896\""); // Replace with actual mobile number
    delay(100);
    lteSerial.println(locationMsg);
    delay(100);
    lteSerial.write(26); // Send SMS
    Serial.println("SMS Sent!");
} 
void trackPetLocation() {
    while (true) {
        while (gpsSerial.available()) {
            gps.encode(gpsSerial.read());
            if (gps.location.isUpdated()) {
                float latitude = gps.location.lat();
                float longitude = gps.location.lng();
                Serial.printf("Location: Lat: %f, Lon: %f\n", latitude, longitude);
                sendLocationOverLTE(latitude, longitude);
                delay(100); // Send location every 100 miliseconds
            }
        }
    }
}

// WiFi Details
const char* ssid = "nagesh____";
const char* password = "rahul12345";

// Define GPS & LTE Serial Communication
HardwareSerial gpsSerial(1);  // GPS connected to UART1
HardwareSerial lteSerial(2);  // LTE connected to UART2


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  delay(1000);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);  // GPS TX = GPIO16, RX = GPIO17
  lteSerial.begin(115200, SERIAL_8N1, 18, 19); // LTE TX = GPIO18, RX = GPIO19

  Serial.println("Checking Geofence...");
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Pet Inside Safe Zone");
    } else {
        Serial.println("Pet Outside Geofence! Activating GPS and LTE...");
        trackPetLocation();//check function section
    }


}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()==WL_CONNECTED){
    serial.println("pet is back inside the geofence. Disabling GPS and LTE");
    enterDeepSleep(); 
  }

}
