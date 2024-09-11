#include "BluetoothSerial.h" 
#include <ESP32Servo.h>
const int servoPin = 18;    //Motor Servo menggunakan pin D18
const int pushbutton = 17;  //pushbutton untuk buka/tutup pintu manual 
const int indikator = 5;    //indikator sisten on/off
int sw=0; // variabel untuk penanda on/off sistem

Servo servo;
int pos = 0;  //posisi awal motor servo 0 derajat
bool IRSensor;//membuat variabel penampung data dari sensor Infra merah


BluetoothSerial ESP_BT; //Object for Bluetooth
int incoming;     //membuat variabel penampung data dari bluetooth
int sistem_aktif; //penanda sistem aktif/tidak

void setup() {
  ESP_BT.begin("Security_System");    //memberi nama modul bluetooth
  servo.attach(servoPin, 550, 2400);  //inisialisasi motor servo 
  pinMode(2,INPUT); //inisialisasi pin D2 sebagai INPUT SENSOR infra merah
  pinMode(19,OUTPUT);//inisialisasi pin D19 sebagai output ke BUZZER
  pinMode(pushbutton,INPUT_PULLUP);//inisialisasi sebagai input
  pinMode(indikator,OUTPUT);//inisialisasi sebagai output
  digitalWrite(19,HIGH);//pin D19 diset HIGH
  digitalWrite(indikator,HIGH);//pin indikator diset HIGH
}


void loop() {
//Jika pushbutton ditekan pertama kali
if(digitalRead(pushbutton)==0 && sw==0) 
{
  servo.write(180);// kunci pintu
  sistem_aktif=1;//sistem aktif
  digitalWrite(indikator,LOW);// nyalakan Led sebagai indikator sistem aktif
  sw=1;  // set sw=1 menandakan Led nyala
  delay(1000);//delay 1000 ms atau 1s
  } else //jika tidak ada object
if(digitalRead(pushbutton)==0 && sw==1){ //jika pushbutton ditekan kedua kali nya
  servo.write(0);//buka pintu
  sistem_aktif=0;//sistem tidak aktif
  digitalWrite(indikator,HIGH);  // padamkan Led indikator
  sw=0; // set sw=0, menandakan Led Padam
  delay(1000);//delay 1000ms atau 1s
  }

 
if (ESP_BT.available()) //Check if we receive anything from Bluetooth
  {
    incoming = ESP_BT.read(); //Read what we recevive 
    
    if (incoming == '0')      //lock & enable system
        {
         servo.write(180);//kunci pintu
         sistem_aktif=1;//sistem aktif
         digitalWrite(indikator, LOW);//LED indikator menyala
        }   
        
    if (incoming == '1')      //unlock
        {
        servo.write(0);//buka pintu
        sistem_aktif=0;//sistem tidak aktif
        digitalWrite(indikator, HIGH);//LED indikator padam
        }
  }
        //Jika sistem aktif
        if (sistem_aktif==1)
        {
          IRSensor = digitalRead(2);//baca sensor infra merah
          if (IRSensor == 0) //jika ada objek terdeteksi atau pintuu terdeteksi terbuka
          {
          digitalWrite(19,LOW);//BUZZER BUNYI
          delay(5000);//delay 5000 ms atau 5 s
          digitalWrite(19,HIGH);//BUZZER MATI
          } 
          else 
          {
          digitalWrite(19,HIGH);//BUZZER MATI
          }
         }

}
