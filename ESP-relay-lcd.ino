/* Interface I2C-LCD + Relay With ESP-8266
 * By Vivekkumar
 * Gnd is essential for working
 */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include<FirebaseArduino.h>
//*************************************************************************************************************
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define FIREBASE_HOST "homeautomation-1b009-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "AIWueohMlQNKbpD8qRoxuNrvNWkb2SlbGiuSLv08"//Your Firebase Database Secret goes here
#define WIFI_SSID "MOTO G5S"                                    //your WiFi SSID for which yout NodeMCU connects
#define WIFI_PASSWORD "44441199"                              //Password of your wifi network 
#define Relay1 2  //D5
#define Relay2 14  //D6
#define Relay3 12  //D7
#define Relay4 13  //D8
int rel1,rel2,rel3,rel4;
uint8_t check[8] = {0x0, 0x1 ,0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
int Fan1, Fan2, fan1Temp, fan2Temp;
//*************************************************************************************************************
void setup() 
{
  Serial.begin(9600);        // Select the same baud rate if you want to see the datas on Serial Monitor
  lcd.begin();// initialize the LCD [LCD code begins]
  lcd.backlight(); // Turn on the blacklight and print a message.
  lcd.createChar(0, cross);
  lcd.createChar(1, check);
  lcd.setCursor(0, 0);  // [LCD code ends]
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(Relay3,OUTPUT);
  pinMode(Relay4,OUTPUT);
  digitalWrite(Relay1,HIGH);
  digitalWrite(Relay2,HIGH);
  digitalWrite(Relay3,HIGH);
  digitalWrite(Relay4,HIGH);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("Connecting");
  lcd.print("Connecting");
  while (WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    lcd.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected:");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.print("Connected");

  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
//Here the varialbe"FB1","FB2","FB3" and "FB4" needs to be the one which is used in our Firebase and MIT App Inventor
  //Firebase.set("FB1",0);
  //Firebase.set("FB2",0); 
  //Firebase.set("FB3",0); 
  //Firebase.set("FB4",0); 
}
//*************************************************************************************************************
void firebasereconnect()
{
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  }
//*************************************************************************************************************
void updateLcd(int a,int b,int c,int d)
{ 
  lcd.clear();
  int Buff;
  int arrX[4]={0,8,0,8}, arrY[4]={0,0,1,1}, relFB[]={a,b,c,d};
  if(Fan1!=fan1Temp || Fan2!=fan2Temp){
    lcd.setCursor(0, 0);
    lcd.print("Fan1 Speed:" + String(Fan1));
    lcd.setCursor(0, 1);
    lcd.print("Fan2 Speed:" + String(Fan2));
    delay(1000);
  }
  lcd.clear();
  for(int j=0;j<4;j++)
  {
    Buff=j+1;
    lcd.setCursor(arrX[j], arrY[j]);
   if(relFB[j]==1){
    lcd.print("Rel-" + String(Buff) + ":");
    lcd.write(1);}
   else{
    lcd.print("Rel-" + String(Buff) + ":");
    lcd.write(0);
  }
}}
//*************************************************************************************************************
void loop() 
{
  if (Firebase.failed())
      {
      Serial.print("setting number failed:");
      Serial.println(Firebase.error());
      firebasereconnect();
      return;
      }
  rel1=Firebase.getString("FB1").toInt();  //Reading the value of the varialble from the firebase
  rel2=Firebase.getString("FB2").toInt();  //Reading the value of the varialble from the firebase
  rel3=Firebase.getString("FB3").toInt();  //Reading the value of the varialble from the firebase
  rel4=Firebase.getString("FB4").toInt();  //Reading the value of the varialble from the firebase
  Fan1=Firebase.getString("FN1").toInt();  //Reading the value of the varialble from the firebase
  Fan2=Firebase.getString("FN2").toInt();  //Reading the value of the varialble from the firebase
  updateLcd(rel1,rel2,rel3,rel4);
  Serial.print("Database Data--");
  Serial.print(rel1);
  Serial.print("--");
  Serial.print(rel2);
  Serial.print("--");
  Serial.print(rel3);
  Serial.print("--");
  Serial.print(rel4);
  Serial.print("--");
  Serial.print(Fan1);
  Serial.print("--");
  Serial.println(Fan2);
  if(rel1==1)                                                             // If, the Status is 1, turn on the Relay1
     {
      digitalWrite(Relay1,LOW);
      Serial.println("Relay 1 ON");
      }
 if(rel1==0)                                                      // If, the Status is 0, turn Off the Relay1
    {                                      
      digitalWrite(Relay1,HIGH);
      Serial.println("Relay 1 OFF");
    }
if(rel2==1)                                                             // If, the Status is 1, turn on the Relay2
     {
      digitalWrite(Relay2,LOW);
      Serial.println("Relay 2 ON");
    }
 if(rel2==0)                                                      // If, the Status is 0, turn Off the Relay2
    {                                      
      digitalWrite(Relay2,HIGH);
      Serial.println("Relay 2 OFF");
    }
if(rel3==1)                                                             // If, the Status is 1, turn on the Relay3
     {
      digitalWrite(Relay3,LOW);
      Serial.println("Relay 3 ON");
    }
if(rel3==0)                                                      // If, the Status is 0, turn Off the Relay3
    {                                      
      digitalWrite(Relay3,HIGH);
      Serial.println("Relay 3 OFF");
    }
if(rel4==1)                                                             // If, the Status is 1, turn on the Relay4
     {
      digitalWrite(Relay4,LOW);
      Serial.println("Relay 4 ON");
    }
if(rel4==0)                                                      // If, the Status is 0, turn Off the Relay4
    {                                      
      digitalWrite(Relay4,HIGH);
      Serial.println("Relay 4 OFF");
    }    
if(Fan1!=fan1Temp)                                                    // If, the Speed is updated, Update LCD 
     {
       fan1Temp=Fan1;
       Serial.println("Fan1 Speed: " + String(Fan1));
       delay(1000);
     }
  if(Fan1==fan1Temp)                                                    // If, the Speed is same(not updated), do nothing
     {
       Serial.println("Fan1 Speed: " + String(Fan1));
     }
 if(Fan2!=fan2Temp)                                                      // If, the Speed is updated, Update LCD
    {                      
      fan2Temp=Fan2;                
      Serial.println("Fan2 Speed: " + String(Fan2));
      delay(1000);
    }
if(Fan2==fan2Temp)                                                    // If, the Speed is same(not updated), do nothing
     {
       Serial.println("Fan2 Speed: " + String(Fan2));
     }
}
