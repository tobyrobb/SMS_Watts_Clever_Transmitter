// T robb jan15  http://www.tobyrobb.com/

// I have some Watts Clever remote control sockets
// http://www.wattsclever.com/products/easy-off-sockets
// they use a 433 mhz transmitter and receiver based on the HS1527 chip

// Using my sim 900 board and a 433mhz TX to emulate the transmitter

// borrowed code includes RCswitch,
// https://code.google.com/p/rc-switch/
// which helps us emulate the HS1527 which is in the Watts Clever remote
// it also includes a listener for listening to your remote, although power points can learn new codes

// TO USE:
// SMS exclamation mark followed by the command as listed in the string below
// eg     !ServerON

#include <RCSwitch.h>
#include <SoftwareSerial.h>

#define TXPin 2 //this pin is the data pin for your ordinary 433mhz TX

//lets define the TX codes here ,, I got these from my remote, by listening with RCswitch listen example
#define channel1ON    4072574
#define channel1OFF   4072566
#define channel2ON    4072572
#define channel2OFF   4072564
#define channel3ON    4072570
#define channel3OFF   4072562
#define channelAllON  4072569
#define channelAllOFF 4072561

SoftwareSerial SIM900(7, 8);  //serial port for the modem
RCSwitch mySwitch = RCSwitch(); //an instance of the 433mhz transmitter 

String serialString = ""; //somewhere to put the serial port string
String commandString = ""; //somewhere to put the command received

// these are the valid command strings we should recieve after an exclamation mark

String ServerON = "ServerON\r\n";
String ServerOFF = "ServerOFF\r\n";
String PcON = "PcON\r\n";
String PcOFF = "PcOFF\r\n";
String RadioON = "RadioON\r\n";
String RadioOFF = "RadioOFF\r\n";

void setup()
{
  Serial.begin(19200); // for serial monitor
  SIM900.begin(19200); // for GSM shield
  Serial.println("Beginning Setup");
  
  // Transmitter is connected to Arduino Pin #2  
  mySwitch.enableTransmit(TXPin);

  // Optional set pulse length.
  mySwitch.setPulseLength(306);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(5);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  
  Serial.println("Turning on modem - 8 seconds");
  SIM900power();  // turn on shield
  Serial.println("Wating 20 seconds for network logon");

  delay(20000);  // give time to log on to network.
  
  Serial.println("Set to SMS mode");
  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(2000);
  Serial.println("Setup Complete");
  delay(1000);
}

void loop()
{
  String content = "";
  char character;

  while(SIM900.available()) {
      character = SIM900.read();
      content.concat(character);
  }

  if (content != "") {

    //may as well look for commands
    
    int commandPos = content.indexOf('!');  //record the start position of the command 
    Serial.print("Command character found at position: ");
    Serial.println(commandPos);
    serialString = content; //copy the serial string into serialString
    
    commandString = serialString.substring(commandPos+1,serialString.length());  //copy the command out of the serialString
    
    Serial.print("commandString = ");
    Serial.println(commandString);
  
  //begin your command choices here
  
     if (commandString == PcON) 
        { 
            Serial.println("PC ON");
          mySwitch.send(channel1ON, 24);  
        }
  
     if (commandString == PcOFF) 
        { 
            Serial.println("PC OFF");
          mySwitch.send(channel1OFF, 24);  
        } 

     if (commandString == ServerON) 
        { 
          Serial.println("Server ON");
          mySwitch.send(channel2ON, 24);  
        } 
       
     if (commandString == ServerOFF) 
        { 
          Serial.println("Server OFF");
          mySwitch.send(channel2OFF, 24);  
        } 
        
     if (commandString == RadioON) 
        { 
            Serial.println("Radio ON");
          mySwitch.send(channel3ON, 24);  
        }   
  
     if (commandString == RadioOFF) 
        { 
            Serial.println("Radio OFF");
          mySwitch.send(channel3OFF, 24);  
        } 
   }
}


void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(7000);
}
 
 
