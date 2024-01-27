//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 01_Store_Data
/*
 *  Reference : https://randomnerdtutorials.com/esp32-firebase-realtime-database/
 */

//======================================== Including the libraries.
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
//======================================== 

//======================================== Insert your network credentials.
#define WIFI_SSID "Itachi Uchiha"
#define WIFI_PASSWORD "clankiller"
//======================================== 
#include <TinyGPS++.h>
#include <Wire.h>


#define ADXL345_ADDRESS 0x53
#define ADXL345_TO_READ 6
#define IMPACT_THRESHOLD 500 // adjust this value to set the impact threshold

byte buff[ADXL345_TO_READ];
int acc_x, acc_y, acc_z;

// Define GPS module object
TinyGPSPlus gps;



// Variables to store GPS data

//Provide the token generation process info.
#include "addons/TokenHelper.h"

//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Defines the Digital Pin of the "On Board LED".
#define On_Board_LED 2

// Insert Firebase project API Key
#define API_KEY "AIzaSyCM8weeIDi-TvP4PFLGy1ric7D2MZ81bxU"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://vmon-77a68-default-rtdb.asia-southeast1.firebasedatabase.app/" 

// Define Firebase Data object.
FirebaseData fbdo;

// Define firebase authentication.
FirebaseAuth auth;

// Definee firebase configuration.
FirebaseConfig config;

//======================================== Millis variable to send/store data to firebase database.
unsigned long sendDataPrevMillis = 0;
const long sendDataIntervalMillis = 10000; //--> Sends/stores data to firebase database every 10 seconds.
//======================================== 

// Boolean variable for sign in status.
bool signupOK = false;


int speed_vehicle=60;
float fuel=2.5;
double latitude=0.0;
double longitude=0.0;
bool isTheft=false;

//________________________________________________________________________________ VOID SETUP
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  pinMode(On_Board_LED, OUTPUT);

  //---------------------------------------- The process of connecting the WiFi on the ESP32 to the WiFi Router/Hotspot.
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("---------------Connection");
  Serial.print("Connecting to : ");
  Serial.println(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");

    digitalWrite(On_Board_LED, HIGH);
    delay(250);
    digitalWrite(On_Board_LED, LOW);
    delay(250);
  }
  digitalWrite(On_Board_LED, LOW);
  Serial.println();
  Serial.print("Successfully connected to : ");
  Serial.println(WIFI_SSID);
  //Serial.print("IP : ");
  //Serial.println(WiFi.localIP());
  Serial.println("---------------");
  //---------------------------------------- 

  // Assign the api key (required).
  config.api_key = API_KEY;

  // Assign the RTDB URL (required).
  config.database_url = DATABASE_URL;

  // Sign up.
  Serial.println();
  Serial.println("---------------Sign up");
  Serial.print("Sign up new user... ");
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  Serial.println("---------------");
  Wire.begin();
  // Configure the ADXL345 accelerometer
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x2D); // Power register
  Wire.write(0x08); // Measurement mode
  Wire.endTransmission();
  
  // Assign the callback function for the long running token generation task.
  config.token_status_callback = tokenStatusCallback; //--> see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
//________________________________________________________________________________ 

//________________________________________________________________________________ VOID LOOP
void loop() {
  // put your main code here, to run repeatedly:
   while (Serial.available() > 0) {
    if (gps.encode(Serial.read())) {
      if (gps.location.isValid()) {
        latitude = gps.location.lat();
        longitude = gps.location.lng();
      }
    }
  }
  
  // Read accelerometer data
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x32); // Starting with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission();
  Wire.requestFrom(ADXL345_ADDRESS, ADXL345_TO_READ);
  for(int i = 0; i < ADXL345_TO_READ; i++)
    buff[i] = Wire.read();

  // Convert raw data to acceleration values (in g's)
  acc_x = ((int)buff[1]<<8) | buff[0];
  acc_y = ((int)buff[3]<<8) | buff[2];
  acc_z = ((int)buff[5]<<8) | buff[4];
  acc_x = acc_x/256;
  acc_y = acc_y/256;
  acc_z = acc_z/256;

  // Check if impact is detected
  
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > sendDataIntervalMillis || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    //---------------------------------------- Generate random values.
    
    //---------------------------------------- 

    //---------------------------------------- 
    Serial.println();
    Serial.println("---------------Random Value");
    Serial.print("FUEL: ");
    Serial.println(fuel);
    Serial.print("SPEED: ");
    Serial.println(speed_vehicle);
    Serial.print("LONGITUDE: ");
    Serial.println(longitude);
    Serial.print("LATITUDE: ");
    Serial.println(latitude);
    Serial.print("ITTHEFT: ");
    Serial.println(isTheft);
    Serial.print("NAME: ");
    Serial.println("accelerometer");
    Serial.print(accel_x);
     Serial.println("accelerometer");
    Serial.print(accel_y);
     Serial.println("accelerometer");
    Serial.print(accel_z);
    Serial.println();
    Serial.println("---------------");
    //---------------------------------------- 

    //---------------------------------------- The process of sending/storing data to the firebase database.
    Serial.println();
    Serial.println("---------------Store Data");
    digitalWrite(On_Board_LED, HIGH);
    
    // Write an Int number on the database path test/random_Float_Val.
    if (Firebase.RTDB.setFloat(&fbdo, "Test/FUEL", fuel)) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "Test/SPEED", speed_vehicle)) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "Test/LATITUDE", latitude)) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "Test/LONGITUDE", longitude)) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    // Write an Float number on the database path test/random_Int_Val.
    if (Firebase.RTDB.setInt(&fbdo, "Test/ISTHEFT", isTheft)) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "/accel/x", acc_x)) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
      if (Firebase.RTDB.setFloat(&fbdo, "/accel/y", acc_y)) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "/accel/z", acc_z)) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    //firebase("balaji",20);
    digitalWrite(On_Board_LED, LOW);
    Serial.println("---------------");
    //---------------------------------------- 
  }
  delay(500);
}
}
/*void firebase(String text,unsigned int values)
{
   String t="Test/"+String(text);
   if (Firebase.RTDB.setFloat(&fbdo, t ,values )) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  
}*/
