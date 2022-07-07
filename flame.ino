#define TINY_GSM_MODEM_SIM800

#define SerialMon Serial
#include <SoftwareSerial.h> 
SoftwareSerial SerialAT(3,2);

#define DUMP_AT_COMMANDS //to dump the AT commands
#define TINY_GSM_DEBUG SerialMon
#define SMS_TARGET  "***********"
#define CALL_TARGET "***********"

#include <TinyGsmClient.h> //to include the library.
#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h> //debug codes that concern the gsm module
StreamDebugger debugger(SerialAT, SerialMon); //object of streamdebugger
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT); //AT is for the gsm module
#endif

uint8_t index;
bool error;

char* charLocation;
String text;
String location;

#define buz 9
#define flameSensor 8

void setup(){
   SerialAT.begin(9600);
  SerialMon.begin(9600);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,OUTPUT);
  digitalWrite(buz,LOW);
}

void loop()
{
  if(digitalRead(flameSensor == LOW)){
     
    sendSMS();
    makeCALL();
    digitalWrite(buz,HIGH);
    
  }
}

void sendSMS()
{
   #if defined(SMS_TARGET)  
  // This is only supported on SIMxxx series
  boolean res11 = modem.sendSMS_UTF16(SMS_TARGET, u"EMERGENCY! Fire Detected.",25);
  Serial.println("EMERGENCY! Fire detected.");
  
  #endif 
}
void makeCALL()
{
  boolean res1 = modem.callNumber(CALL_TARGET);
  
  if (res1) 
  {
    delay(1000L); 
    // Play DTMF A, duration 1000ms
    modem.dtmfSend('A', 1000); //dualtonemodulationfrequency.

    // Play DTMF 0...4, default duration (100ms)
    for (char tone='0'; tone<='4'; tone++) 
    {
      modem.dtmfSend(tone);
    }
    
    //delay(30000);
    delay(9000);
    res1 = modem.callHangup();
  }  
}
