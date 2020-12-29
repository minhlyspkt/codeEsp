/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-async-web-server-espasyncwebserver-library/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
// -------------- Global --------------

char eRead[25];
byte len;
const char* PARAM_INPUT_1 = "Output";
const char* PARAM_INPUT_2 = "Status";
String CodeId = "131220-2-4-33-L3";
String Password = "admin";
String Tocken = "eyJ0eXAiOiJKV1QiLCJhbGciOihVUyAtIFByaW";
// -------------- Functions --------------
String outputState(int output){
  if(digitalRead(output)){
    return "1";
  }
  else {
    return "0";
  }
}
// Saves string do EEPROM
void SaveString(int startAt, const char* id)
{
  for (byte i = 0; i <= strlen(id); i++)
  {
    EEPROM.write(i + startAt, (uint8_t) id[i]);
  }
  EEPROM.commit();
}

// Reads string from EEPROM
void ReadString(byte startAt, byte bufor)
{
  for (byte i = 0; i <= bufor; i++)
  {
    eRead[i] = (char)EEPROM.read(i + startAt);
  }
  len = bufor;
}

//Saves byte to EEPROM
void SaveByte(int startAt, byte val)
{
  EEPROM.write(startAt, val);
  EEPROM.commit();
}

//Reads byte from EEPROM
byte ReadByte(byte startAt)
{
  byte Read = -1;
  Read = EEPROM.read(startAt);
  return Read;
}
String encryption(String s) 
{ 
    int l = s.length(); 
    int b = ceil(sqrt(l)); 
    int a = floor(sqrt(l)); 
    String encrypted; 
    if (b * a < l) { 
        if (min(b, a) == b) { 
            b = b + 1; 
        } 
        else { 
            a = a + 1; 
        } 
    } 
  
    // Matrix to generate the  
    // Encrypted String 
    char arr[a][b]; 
    memset(arr, ' ', sizeof(arr)); 
    int k = 0; 
      
    // Fill the matrix row-wise 
    for (int j = 0; j < a; j++) { 
        for (int i = 0; i < b; i++) { 
            if (k < l){ 
                arr[j][i] = s[k]; 
            } 
            k++; 
        } 
    } 
  
    // Loop to generate  
    // encrypted string 
    for (int j = 0; j < b; j++) { 
        for (int i = 0; i < a; i++) { 
            encrypted = encrypted +  
                         arr[i][j]; 
        } 
    } 
    return encrypted; 
} 
  
// Function to decrypt the string 
String decryption(String s){ 
    int l = s.length(); 
    int b = ceil(sqrt(l)); 
    int a = floor(sqrt(l)); 
    String decrypted; 
  
    // Matrix to generate the  
    // Encrypted String 
    char arr[a][b]; 
    memset(arr, ' ', sizeof(arr)); 
    int k = 0; 
      
    // Fill the matrix column-wise 
    for (int j = 0; j < b; j++) { 
        for (int i = 0; i < a; i++) { 
            if (k < l){ 
                arr[j][i] = s[k]; 
            } 
            k++; 
        } 
    } 
  
    // Loop to generate  
    // decrypted string 
    for (int j = 0; j < a; j++) { 
        for (int i = 0; i < b; i++) { 
            decrypted = decrypted +  
                         arr[i][j]; 
        } 
    } 
    return decrypted; 
} 
// Create AsyncWebServer object on port 80
AsyncWebServer server(147);
// Set your Static IP address
IPAddress local_IP(192, 168, 1, 184);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

void setup(){
 
  // Serial port for debugging purposes
   EEPROM.begin(512);
  Serial.begin(115200);
  delay(1000);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
    byte x = -2;
  String ssidd = "";
  String passwordd = "";
   // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  // Connect to Wi-Fi
  Serial.println("\n\tConnecting to Wi-Fi");
      //SaveByte(2,strlen(ssid));
      //SaveByte(3,strlen(password));
      //SaveString(5,ssid);
      //SaveString(31,password);
     x = ReadByte(2);
    Serial.print("\t\tValue of byte at adress 2 = "); Serial.println(x);
   
    ReadString(5, x);
    for (byte i = 0; i < len; i++)
    {
      ssidd += eRead[i];
    }
    const char* ssid = ssidd.c_str();
    Serial.print("\t\tReaded ssid = "); Serial.println(ssid);
    x = ReadByte(3);
    Serial.print("\t\tValue of byte at adress 3 = "); Serial.println(x);
    ReadString(31, x);
    for (byte i = 0; i < len; i++)
    {
      passwordd += eRead[i];
    }
    const char* password = passwordd.c_str();
    Serial.print("\t\tReaded password = "); Serial.println(password);
    WiFi.disconnect();
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  server.on(
   "/131220-2-4-33-L3/Login",
    HTTP_POST,
    [](AsyncWebServerRequest * request){},
    NULL,
    [](AsyncWebServerRequest * request,uint8_t* data, size_t len, size_t index, size_t total) {
      String result;
      StaticJsonDocument<200> docRequest;
      StaticJsonDocument<200> docResponse;
      for (size_t i = 0; i < len; i++) {
        result += (char)data[i];
        //Serial.write(data[i]);
      }
      deserializeJson(docRequest, result);
       serializeJson(docRequest, Serial);
      JsonObject object = docRequest.as<JsonObject>();
      if(object["CodeId"] == CodeId && object["Password"] == Password)
      {
        docResponse["Status"] = true;
        docResponse["Tocken"] = Tocken;
        Serial.println();
        String Response = docResponse.as<String>();
        request->send(200, "application/json",Response);
      }
     //Serial.println(result);
     request->send(301, "application/json","{'Status' : 'False'}");
  });

  server.on(
   "/131220-2-4-33-L3/output",
    HTTP_POST,
    [](AsyncWebServerRequest * request){},
    NULL,
    [](AsyncWebServerRequest * request,uint8_t* data, size_t len, size_t index, size_t total) {
      String result;
      StaticJsonDocument<200> docRequest;
      StaticJsonDocument<200> docResponse;
      for (size_t i = 0; i < len; i++) {
        result += (char)data[i];
        //Serial.write(data[i]);
      }
      deserializeJson(docRequest, result);
       serializeJson(docRequest, Serial);
      JsonObject object = docRequest.as<JsonObject>();
      if(object["Tocken"] == Tocken)
      {
        docResponse["Status"] = true;
        docResponse["Tocken"] = Tocken;
        JsonObject data = docResponse.createNestedObject("data");
        data["2"] = outputState(2);
        data["4"] = outputState(4);
        data["33"] = outputState(33);
        Serial.println();
        String Response = docResponse.as<String>();
        request->send(200, "application/json",Response);
      }
     //Serial.println(result);
     request->send(301, "application/json","{'Status' : 'False'}");
  });

server.on(
   "/131220-2-4-33-L3",
    HTTP_POST,
    [](AsyncWebServerRequest * request){},
    NULL,
    [](AsyncWebServerRequest * request,uint8_t* data, size_t len, size_t index, size_t total) {
      String result;
      StaticJsonDocument<200> docRequest;
      StaticJsonDocument<200> docResponse;
      for (size_t i = 0; i < len; i++) {
        result += (char)data[i];
        //Serial.write(data[i]);
      }
      deserializeJson(docRequest, result);
       serializeJson(docRequest, Serial);
      JsonObject object = docRequest.as<JsonObject>();
      if(object["Tocken"] == Tocken)
      {
        String GPIO = object["GPIO"] ;
        String Status = object["Status"] ;
        digitalWrite(GPIO.toInt(), Status.toInt());
        docResponse["Status"] = true;
        docResponse["Tocken"] = Tocken;
        JsonObject data = docResponse.createNestedObject("data");
        data[GPIO] = outputState(GPIO.toInt());;
        Serial.println();
        String Response = docResponse.as<String>();
        request->send(200, "application/json",Response);
      }
     //Serial.println(result);
     request->send(301, "application/json","{'Status' : 'False'}");
  });

  // Start server
  server.begin();
}

void loop() {

}
