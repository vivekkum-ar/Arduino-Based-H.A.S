/* Interface L298N Motor Driver + ServoSG90 + TTP223 + One-Signal With ESP-8266
 * By Vivek
 * Gnd is essential for working
 */
//*************************************************************************************************************
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>
//*************************************************************************************************************
#define FIREBASE_HOST "homeautomation-1b009-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "AIWueohMlQNKbpD8qRoxuNrvNWkb2SlbGiuSLv08" //Your Firebase Database Secret goes here
#define WIFI_SSID "MOTO G5S"                                  //your WiFi SSID for which yout NodeMCU connects
#define WIFI_PASSWORD "44441199"                               //Password of your wifi network 
//*************************************************************************************************************
#define TPin1 12 // Pin for capactitive touch sensor 1
#define TPin2 13 // Pin for capactitive touch sensor 2
int TS1, TS2, Promise;
//*************************************************************************************************************
#define ENA 16 //4 
#define IN1 5 //0;
#define IN2 4 //2;
#define IN3 0 //0;
#define IN4 2 //2;
#define ENB 14 //4;
int Fan1, Fan2, fan1Temp, fan2Temp;
//*************************************************************************************************************
String ids = "aa0fc742-d63a-494d-bc26-dd5e431051ae"; // its not real IDS, please change
// end of user parameters 
const char* host = "onesignal.com";
const int httpsPort = 443;
String url = "/api/v1/notifications";
//*************************************************************************************************************
#define SERVO_PIN 15
Servo myServo; //initialisation class Servo
int angle = 0;
int lastAngle = 0;
//*************************************************************************************************************
void setup() {
  Serial.begin(9600);
// set all the motor control pins to outputs
pinMode(ENA, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(ENB, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
pinMode(TPin1, INPUT);
pinMode(TPin2, INPUT);
myServo.attach(SERVO_PIN);
myServo.write(0); 

  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected:");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
}
//*************************************************************************************************************
void firebasereconnect()
{
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  }
//*************************************************************************************************************
void autoDoorOp() {
      Serial.print("Opening: ");
      Serial.println(180);
      myServo.write(180);
      delay(5000);
      Serial.print("Closing: ");
      Serial.println(0);
      myServo.write(0);
}
//*************************************************************************************************************
void httpg(){
  HTTPClient http;    //Declare object of class HTTPClient
  String getData, Link;
  //GET Data
  getData =""; //"?status=" + ADCData + "&station=" + station ;  //Note "?" added at front
  Link = "http://akpassistant.000webhostapp.com/f2s.php" + getData;
  http.begin(Link);     //Specify request destination
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
  http.end();  //Close connection
}
//***********************************************************************************************************************************
void push(String msg) {
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;

  Serial.print("PUSH: connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  Serial.println("PUSH: try to send push notification...");
  String data = "{\"app_id\": \"b7245b92-ab9a-4eda-9a03-f839ad4829c2\",\"include_player_ids\":[\"" + ids + "\"],\"android_group\":\"Smart House\",\"large_icon\":\"https://res.cloudinary.com/akp-assistant/image/upload/v1619695374/SHlogo.png\",\"priority\":10,\"android_group_message\": {\"en\": \"" + "You have $[notif_count] warnings" + "\"},\"headings\": {\"en\": \"" + "Tresspasser Alert" + "\"},\"contents\": {\"en\": \"" + msg + "\"}}";
  Serial.print("PUSH: requesting URL: ");
  Serial.println(url);
  client.println(String("POST ") + url + " HTTP/1.1");
  client.print("Host:");
  client.println(host);
  client.println("User-Agent: esp8266.Arduino.IoTmanager");
  client.print("Content-Length: ");
  client.println(data.length());
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  client.println(data); 
}
//*************************************************************************************************************
void loop() {
  if (Firebase.failed())
      {
      Serial.print("setting number failed:");
      Serial.println(Firebase.error());
      firebasereconnect();
      return;
      }
 TS1 = digitalRead(TPin1);
 TS2 = digitalRead(TPin2);
 Promise=Firebase.getString("PM").toInt();  //Reading the value of the variable Status from the firebase
 Fan1=Firebase.getString("FN1").toInt();  //Reading the value of the variable Status from the firebase
 Fan2=Firebase.getString("FN2").toInt();  //Reading the value of the variable Status from the firebase
 angle = Firebase.getString("servo/angle").toInt();  //Reading the val of the variable from the firebase
 if(TS1 == HIGH || TS2 == HIGH) {
  if (Promise == 200){
    autoDoorOp();
  }
  else {
    httpg();
    push("Intruder Alert ! We've detected a tresspasser");
  }
 }
 else if(angle != lastAngle)                           // If, the Status is 0, turn Off the Relay4
    {                                      
      Serial.print("Angle changed: ");
      Serial.println(angle);
      myServo.write(angle);
      lastAngle = angle;
    }
 Serial.print("Database Data--");
  Serial.print(Fan1);
  Serial.print("--"); 
  Serial.println(Fan2);
  if(Fan1!=fan1Temp) // If, the Status is 1, turn on the Relay1
     {
       fan1Temp=Fan1;
       analogWrite(ENA, Fan1);
       Serial.println("Fan1 Speed: " + String(Fan1));
     }
 if(Fan1==fan1Temp) // If, the Status is 1, turn on the Relay1
     {
       Serial.println("Fan1 Speed: " + String(Fan1));
     }
 if(Fan2!=fan2Temp)                                                      // If, the Status is 0, turn Off the Relay1
    {                      
      fan2Temp=Fan2;                
      analogWrite(ENB, Fan2);
      Serial.println("Fan2 Speed: " + String(Fan2));
    }
 if(Fan2==fan2Temp)                                                    // If, the Status is 1, turn on the Relay1
     {
       Serial.println("Fan2 Speed: " + String(Fan2));
     }
}
