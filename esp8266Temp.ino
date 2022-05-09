#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

bool led1IsOn = false;
bool led2IsOn = false;
bool led3IsOn = false;
const int LED_0 = 16;
const int LED_1 = 5;
const int LED_2 = 2;
const int oneWireBus = 4; 
const char* ssid = "server";
const char* password = "11111111";
float temperatureC = 0.0f;
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);


void setup() {
 
  Serial.begin(115200);
  pinMode(LED_0, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  delay(1000);
  digitalWrite(LED_0, HIGH);
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  delay(1000);
  digitalWrite(LED_0, LOW);
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  delay(10);
  sensors.begin();
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  sensors.requestTemperatures(); 
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
   // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  // Match the request
  controller(req, client);
  client.flush(); 
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += temperatureC;
  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
void controller(String req,  WiFiClient client){
   if (req.indexOf("/temperature") != -1){
     temperatureC = sensors.getTempCByIndex(0);
     Serial.println("Showing temperature");
    } else if(req.indexOf("/led1") != -1){
     setLedState(LED_0, led1IsOn);
     led1IsOn = !led1IsOn;
    } else if(req.indexOf("/led2") != -1){
      setLedState(LED_1, led2IsOn);
      led2IsOn = !led2IsOn;
    } else if(req.indexOf("/led3") != -1){
      setLedState(LED_2, led3IsOn);
      led3IsOn = !led3IsOn;
    } else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  }
void setLedState(int led, bool state){
  if(!state){
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);}
  }
