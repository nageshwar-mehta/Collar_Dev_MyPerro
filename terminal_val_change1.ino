#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ArduinoJson.h>

// WiFi Credentials
#define WIFI_SSID "nagesh____"
#define WIFI_PASSWORD "rahul12345"

// Firebase Credentials
#define FIREBASE_HOST "https://myperrodemo-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "YOUR_DATABASE_SECRET"

FirebaseData firebaseData;
WiFiServer server(8080);
String sessionID; // Unique session variable

// Function to connect to WiFi
void connect_to_wifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

// Function to generate a unique session ID
void generateSessionID()
{
    sessionID = "session_" + String(random(1000, 9999)); // Unique session ID
    Serial.print("Generated Session ID: ");
    Serial.println(sessionID);
}

// Function to send data to Firebase
void sendDataToFirebase(String key, String value)
{
    String path = "/" + sessionID + "/" + key; // Store under sessionID
    if (Firebase.setString(firebaseData, path, value))
    {
        Serial.println("Data Sent to Firebase Successfully!");
    }
    else
    {
        Serial.print("Firebase Error: ");
        Serial.println(firebaseData.errorReason());
    }
}

// Function to receive data from client
String receive_from_client(WiFiClient &client)
{
    if (client.available())
    {
        String request = client.readStringUntil('\n'); // Read input
        request.trim(); // Remove extra spaces
        client.flush(); // Clear input buffer

        // Check if request contains only numbers (float/int)
        bool isNumeric = true;
        for (char c : request)
        {
            if (!isdigit(c) && c != '.')
            {
                isNumeric = false;
                break;
            }
        }

        if (isNumeric)
        {
            sendDataToFirebase("sensor_value", request); // Send valid data
            return request;
        }
        else
        {
            Serial.println("Invalid Input: Not a Number");
            return "";
        }
    }
    return "";
}


void setup()
{
    Serial.begin(115200);
    connect_to_wifi(); // Connect ESP to WiFi
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    generateSessionID(); // Generate a new session ID on reboot

    server.begin(); // Start server
}


void loop()
{
    WiFiClient client = server.available();
    if (client)
    {
        Serial.println("Client Connected!");
        while (client.connected())
        {
            String input = receive_from_client(client);
            if (!input.isEmpty())
            {
                Serial.print("Received and Sent to Firebase: ");
                Serial.println(input);
            }

            
            // Your main code 


        }
    }
}
