#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,16,2); 
SoftwareSerial nodemcu(2,3);

int parking1_slot1_ir_s = 5; // parking slot1 infrared sensor connected with pin number 4 of arduino
int parking1_slot2_ir_s = 6;

int parking2_slot1_ir_s = 7;
int parking2_slot2_ir_s = 8;

const int trig=12;
const int echo =11;

const int trig2=9;
const int echo2 =10;


String sensor1="0"; 
String sensor2="0"; 
String sensor3="0"; 
String sensor4="0"; 
String last="NULL";

int LED1=A2;
int LED2=A3;
int LED3=A0;
int LED4=A1;


String cdata =""; // complete data, consisting of sensors values
void setup()
{   
    lcd.begin();
    lcd.backlight();


    Serial.begin(9600); 
    nodemcu.begin(9600);

    pinMode(parking1_slot1_ir_s, INPUT);
    pinMode(parking1_slot2_ir_s, INPUT);

    pinMode(parking2_slot1_ir_s, INPUT);
    pinMode(parking2_slot2_ir_s, INPUT);
    pinMode(trig,OUTPUT);
    pinMode(echo,INPUT);
    pinMode(trig2,OUTPUT);
    pinMode(echo2,INPUT);

}
String k="a";
String entry="closed",entry2="closed";
void loop()
{  
    long duration,distance;
    digitalWrite(trig,HIGH);
    delayMicroseconds(1000);
    digitalWrite(trig,LOW);
    duration=pulseIn(echo,HIGH);
    distance=(duration/2)/29.1;
    //    
    //
    if(distance<10 && entry=="closed"){
        entry="opened";
        Serial.print("OP"+String(distance));
    }else if(distance>10 && entry=="opened" ){
        entry="closed";
        Serial.print("CL"+String(distance));
    }
    long duration2,distance2;
    digitalWrite(trig2,HIGH);
    delayMicroseconds(1000);
    digitalWrite(trig2,LOW);
    duration2=pulseIn(echo2,255);
    distance2=(duration2/2)/29.1;
    if(distance2<10 && entry2=="closed"){
        entry2="opened";
    }else if(distance2>10 && entry2=="opened"){
        entry2="closed";
    }




    p1slot1(); 
    p1slot2();

    p2slot1();
    p2slot2();
    int space=(sensor1=="0"?0:1)+(sensor2=="0"?0:1)+(sensor3=="0"?0:1)+(sensor4=="0"?0:1);

    space=4-space;
    String s=String(space);

    cdata = cdata + sensor1 +"," + sensor2 + ","+ sensor3 +","+ sensor4 + ","+s+" SPACES LEFT!,"+entry+","+entry2; 


    lcd.print(s+" LEFT!!");
    for(int i=0;i<2;i++)
    {lcd.print("        ");}

    Serial.println(s+" LEFT!!"); 

    Serial.println(cdata); 
    nodemcu.println(cdata);
    delay(800); // 100 milli seconds
    cdata = ""; 

    digitalWrite(parking1_slot1_ir_s, HIGH); 
    digitalWrite(parking1_slot2_ir_s, HIGH); 

    digitalWrite(parking2_slot1_ir_s, HIGH);
    digitalWrite(parking2_slot2_ir_s, HIGH);

}

void p1slot1()
{
    if( digitalRead(parking1_slot1_ir_s) == LOW) 
    {
        sensor1 = "255";
        analogWrite(LED1,255);

        delay(100); 
    } 
    if( digitalRead(parking1_slot1_ir_s) == HIGH)
    {
        sensor1 = "0";  
        analogWrite(LED1,0);
        delay(100);  
    }
}

void p1slot2() // parking 1 slot2
{
    if( digitalRead(parking1_slot2_ir_s) == LOW) 
    {
        sensor2 = "255"; 
        analogWrite(LED2,255);
        delay(100); 
    }
    if( digitalRead (parking1_slot2_ir_s) == HIGH)  
    {
        sensor2 = "0";  
        analogWrite(LED2,0);
        delay(100);
    } 
}
void p2slot1() // parking 1 slot3
{
    if( digitalRead(parking2_slot1_ir_s) == LOW) 
    {
        sensor3 = "255"; 
        analogWrite(LED3,255);

        delay(100); 
    }
    if( digitalRead(parking2_slot1_ir_s) == HIGH)  
    {
        sensor3 = "0";
        analogWrite(LED3,0);

        delay(100);
    } 
}

void p2slot2() // parking 1 slot3
{
    if( digitalRead(parking2_slot2_ir_s) == LOW) 
    {
        sensor4 = "255";
        analogWrite(LED4,255);

        delay(100); 
    }
    if( digitalRead(parking2_slot2_ir_s) == HIGH)  
    {
        sensor4 = "0";
        analogWrite(LED4,0);

        delay(100);
    } 
}
