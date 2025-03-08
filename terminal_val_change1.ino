#include <WiFi.h>

String ssid = "nagesh____";
String password = "rahul12345";
// Create WiFiServer object
WiFiServer server(8080);

// Function to connect to the WiFi network
void connect_to_wifi(const char* ssid, const char* password)
{
    // Begin WiFi connection with provided SSID and password
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");

    // Wait for the connection to establish
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(200);
        Serial.print("...");
    }

    // WiFi connected, print the IP address
    Serial.println();
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Start the server on port 8080
    server.begin();
}
String receive_from_client(WiFiClient &client)
{
    // Check if the client has sent any data
    if (client.available())
    {
        // Read the client's request
        String request = client.readStringUntil('\n');
        request.trim(); // Remove any leading/trailing whitespace
        client.flush(); // Clear the input buffer

        // Convert the request to uppercase
        request.toUpperCase();

        // Send back the uppercase request as confirmation
        client.print("sent: ");
        client.println(request);

        // If the request is "CLOSE", disconnect the client
        // if (request == "CLOSE")
        // { // Case insensitive check after conversion
        //     client.stop();
        //     Serial.println("Client disconnected");
        //     return "client disconnected";
        // }
        

        // Return the uppercase request
        return request;
    }
    // Return an empty string if no data is available
    return "";
}
int i=0;
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  connect_to_wifi(ssid.c_str(), password.c_str());
  Serial.println("ESP Started");
  delay(100);


}
void print1_to_10num(int d){
  for(int i=0;i<10;i++){
    Serial.println(i);
    delay(d);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED){
    connect_to_wifi(ssid.c_str(), password.c_str());
  }
  WiFiClient client = server.available();
  if(client){
    Serial.println("Client Connected");
    while(client.connected()){
      String input = receive_from_client(client);
      if(!input.isEmpty()){
        Serial.println(input);
      }
      //Your main code 
      print1_to_10num(1000);
    }
  }
  

  

}
