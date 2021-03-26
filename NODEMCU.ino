#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>
#include <Servo.h>
#include <TinyGPS++.h>

Servo servo1,servo2,servo3,servo4,servo5,servo6;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
WidgetTerminal terminal(V3);
const int RXpin=4,TXpin=5; 
SoftwareSerial ssgps(RXpin,TXpin); //D1,D2


//char auth[] = "AchgYJbDu5IG_TpbkIbM3C3jcIOY6vau";
char auth[] = "Cj--EuczUexATBFYGngLlTFBh2rdWNOy";
//char auth[] = "7BwXBh4TJs9w0wHyU9My4SE9ARU0mhiz";              //Your Project authentication key

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "shubhamisback";
char pass[] = "hello123";
//char server[]= "192.168.43.22";
SimpleTimer timer;
WidgetMap myMap(V0);

String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors

int firstVal, secondVal,thirdVal; // sensors 
int led1,led2,led3,led4;
String latv="NULL",lngv="NULL";

void myTimerEvent()
{
    // You can send any value at any time.
    // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V1, millis() / 1000);

}

int selected=0;
String book_status="unbooked";

BLYNK_WRITE(V22)
{
    int Press=param.asInt();
    //  && book_status=="booked" 
    if(Press==1 && selected!=0){
        //Book
        opengate(selected);
    }else{
        //UNBOOK
        closegate(selected);
    }
}
BLYNK_WRITE(V21)
{
    int Press=param.asInt();
    if(Press==1 && selected!=0){
        //Book
        book_status="booked";
    }else{
        //UNBOOK
        book_status="unbooked";
    }
}
BLYNK_WRITE(V20)
{
    selected=param.asInt();
    switch (param.asInt())
    {
        case 1: { // Item 1
            Serial.println("Item 1 selected");
            break;
        }
        case 2: { // Item 2
            Serial.println("Item 2 selected");
            break;
        }
        case 3: { // Item 3
            Serial.println("Item 2 selected");
            break;
        }
        case 4: { // Item 4
            Serial.println("Item 2 selected");
            break;
        }    
    }
}

String space="0";

void setup()
{
    // Debug console
    Serial.begin(9600);
    Serial.println(F("Working"));
    ssgps.begin(GPSBaud);  
    Blynk.begin(auth, ssid, pass,IPAddress(192,168,43,22), 8081);

    timer.setInterval(1000L,sensorvalue1); 
    timer.setInterval(1000L,sensorvalue2); 
    timer.setInterval(1000L,sensorvalue3);
    timer.setInterval(1000L,sensorvalue4);
    timer.setInterval(1000L,setlat);
    timer.setInterval(1000L,setspace);
    timer.setInterval(1000L,showterminal); 

    servo1.attach(15); //D8
    servo2.attach(13); //D7
    servo3.attach(12); //D6
    servo4.attach(14); //D5
    servo5.attach(0); //D3
    servo6.attach(2); //D4

}
String latval="NULL",lngval="NULL";
String entry="closed",entry2="closed";
BLYNK_WRITE(V23){
    if(ssgps.available()>0){
        if(gps.encode(ssgps.read())){
            if(gps.location.isValid()){         
                latval=String(gps.location.lat()); lngval=String(gps.location.lng());
                Serial.println("Location:"+latval);
            }else{
                Serial.println("Location: INVALID");
            }

        }
    }
}
void loop()
{

    while(ssgps.available()>0){
        if(gps.encode(ssgps.read())){
            if(gps.location.isValid()){         
                latval=String(gps.location.lat()); lngval=String(gps.location.lng());
                Serial.println("Location:"+latval);
            }else{
                Serial.println("Location: INVALID");
            }

        }
    }
    if (Serial.available() == 0 ) 
    {
        Blynk.run();
        timer.run(); // Initiates BlynkTimer
    }

    if (Serial.available() > 0 ) 
    {
        rdata = Serial.read(); 
        myString = myString+ rdata; 
        if( rdata == '\n')
        {
            Serial.println(myString); 
            String l = getValue(myString, ',', 0);
            String m = getValue(myString, ',', 1);
            String n = getValue(myString, ',', 2);
            String o = getValue(myString, ',', 3);
            space=getValue(myString, ',', 4);
            if(latval!="NULL"){
                latv = latval;
                lngv = lngval;
            }
            //  entry = getValue(myString, ',', 5);
            //  entry2 = getValue(myString, ',', 6);
            if(String(getValue(myString, ',', 5))=="opened" && entry=="closed"){
                openEntryGate();
                entry="opened";
                Serial.print("OPENING!");
            }else if(String(getValue(myString, ',', 5))=="closed" && entry=="opened"){
                closeEntryGate();
                entry="closed";
                Serial.print("CLOSING!");
            }
            if(String(getValue(myString, ',', 6))=="opened" && entry2=="closed"){
                openExitGate();
                entry2="opened";
            }else if(String(getValue(myString, ',', 6))=="closed" && entry2=="opened"){
                closeExitGate();
                entry2="closed";
            }

            // these leds represents the leds used in Blynk application
            led1 = l.toInt();
            led2 = m.toInt();
            led3 = n.toInt();
            led4 = o.toInt();

            int stotal=(led1+led2+led3+led4)/255;
            showterminal();  
            myString = "";
            // end new code
        }
    }

}
void showterminal()
{
    if(myString!="")
    {Blynk.virtualWrite(V3, myString);}
}

void openEntryGate(){
    servo5.write(90);
}
void openExitGate(){
    servo6.write(90);
}

void closeEntryGate(){
    servo5.write(0);
}
void closeExitGate(){
    servo6.write(0);
}

void opengate(int n){
    //servo control
    switch(n){
        case 1:servo1.write(90);break;
        case 2:servo2.write(90);break;
        case 3:servo3.write(90);break;
        case 4:servo4.write(90);break;
    }

}
void closegate(int n){
    //servo control
    switch(n){
        case 1:servo1.write(00);break;
        case 2:servo2.write(00);break;
        case 3:servo3.write(00);break;
        case 4:servo4.write(00);break;
    }
}


void setlat(){
    if(latv!="NULL"){Blynk.virtualWrite(V1, String(latv));}
    if(lngv!="NULL"){Blynk.virtualWrite(V2, String(lngv));}
    myMap.location(1, latv.toFloat(), lngv.toFloat(), "GPS_Location");
}
void setspace(){
    Blynk.virtualWrite(V16, String(space));
}

void sensorvalue1()
{
    int sdata = led1;
    // You can send any value at any time.
    // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V10, sdata);

}
void sensorvalue2()
{
    int sdata = led2;
    // You can send any value at any time.
    // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V11, sdata);

}

void sensorvalue3()
{
    int sdata = led3;
    Blynk.virtualWrite(V12, sdata);

}

void sensorvalue4()
{
    int sdata = led4;
    Blynk.virtualWrite(V13, sdata);

}



String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}