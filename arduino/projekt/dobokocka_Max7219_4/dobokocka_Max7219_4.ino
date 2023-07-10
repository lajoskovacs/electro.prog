
// 8x8-as LEDmátrix   és MAX7219 vezérlő IC
// #include <Arduino.h>

#include "LedControl.h"

//MAX7219
const byte DIN=10;
const byte CS=9;
const byte CLK=8;  //SCK

const byte jobb=4;  //nyomógomb 4-es lábon (jobb, fel)
const byte bal=2;
const byte stop=3;   // nyomógomb 3-as lábon (stop, set, exit)

LedControl ledek=LedControl(DIN,CLK,CS,4);
unsigned int var=100;

byte kezd=0; 
byte szam;
  


// a ledmátrixra kiírt jelek (betűk, számok)
// pontmátrix kódjai

const static byte Bbetu [] = {
 B01111110,
 B01000010,
 B01000010,
 B01111100,
 B01000010,
 B01000010,
 B01111110,
 B00000000
};

const static byte Ebetu [] = {
 B01111110,
 B01000000,
 B01000000,
 B01111100,
 B01000000,
 B01000000,
 B01111110,
 B00000000
};


const static byte Hbetu [] = {
 B01000010,
 B01000010,
 B01000010,
 B01111110,
 B01000010,
 B01000010,
 B01000010,
 B00000000
};


const static byte Lbetu [] = {
 B01000000,
 B01000000,
 B01000000,
 B01000000,
 B01000000,
 B01000000,
 B01111110,
 B00000000
};


const static byte Obetu [] = {
 B01111000,
 B10000100,
 B10000100,
 B10000100,
 B10000100,
 B10000100,
 B01111000,
 B00000000
};


const static byte ibetu [] = {
 B00100000,
 B00000000,
 B00100000,
 B00100000,
 B00100000,
 B00100000,
 B00100000,
 B00000000
};



const static byte Sz1 [] = {
 B00010000,
 B00110000,
 B01010000,
 B10010000,
 B00010000,
 B00010000,
 B00010000,
 B00000000
};

const static byte Sz2 [] = {
 B01110000,
 B10001000,
 B00001000,
 B00010000,
 B00100000,
 B01000000,
 B11111000,
 B00000000
};


const static byte Sz3 [] = {
 B01110000,
 B10001000,
 B00001000,
 B01110000,
 B00001000,
 B10001000,
 B01110000,
 B00000000
};

const static byte Sz4 [] = {
 B00010000,
 B00110000,
 B01010000,
 B10010000,
 B11111000,
 B00010000,
 B00010000,
 B00000000
};

const static byte Sz5 [] = {
 B11111000,
 B10000000,
 B11110000,
 B00001000,
 B00001000,
 B10001000,
 B01110000,
 B00000000
};


const static byte Sz6 [] = {
 B01110000,
 B10001000,
 B10000000,
 B11110000,
 B10001000,
 B10001000,
 B01110000,
 B00000000
};


const static byte Sz7 [] = {
 B11111000,
 B00001000,
 B00010000,
 B00100000,
 B01000000,
 B10000000,
 B10000000,
 B00000000
};


const static byte Sz8 [] = {
 B01110000,
 B10001000,
 B10001000,
 B01110000,
 B10001000,
 B10001000,
 B01110000,
 B00000000
};


const static byte Sz9 [] = {
 B01110000,
 B10001000,
 B10001000,
 B01111000,
 B00001000,
 B10001000,
 B01110000,
 B00000000
};


const static byte Sz0 [] = {
 B01110000,
 B10001000,
 B10001000,
 B10001000,
 B10001000,
 B10001000,
 B01110000,
 B00000000
};

// const static byte* szamok[]= {&Sz0,&Sz1,&Sz2,&Sz3,&Sz4,&Sz5,&Sz6,&Sz7,&Sz8,&Sz9};


const static byte Orul [] = {
 B00111100,
 B01000010,
 B10000001,
 B10100101,
 B10011001,
 B10000001,
 B01000010,
 B00111100

};

/*
void szam_ki(byte digit, byte szam)
{
 for(byte i=0;i<8;i++)
       ledek.setRow(digit,i,szamok[szam][i]);

}   */


void ledmatrix(byte matrix,byte x) 
{ 
  switch (x)
  {
  case 1: for(byte i=0;i<8;i++) ledek.setRow(matrix,i,Sz1[i]);
          break;
  case 2: for(byte i=0;i<8;i++) ledek.setRow(matrix,i,Sz2[i]);
          break;        
  case 3: for(byte i=0;i<8;i++) ledek.setRow(matrix,i,Sz3[i]);
          break;
  case 4: for(byte i=0;i<8;i++) ledek.setRow(matrix,i,Sz4[i]);
          break;
  case 5: for(byte i=0;i<8;i++) ledek.setRow(matrix,i,Sz5[i]);
          break;        
  case 6: for(byte i=0;i<8;i++) ledek.setRow(matrix,i,Sz6[i]);
          break;       
  case 0: for(byte i=0;i<8;i++) ledek.setRow(matrix,i,Orul[i]);
          break; 
  }
}



void setup() {
 
  pinMode(jobb,INPUT);
  pinMode(bal,INPUT);
  pinMode(stop,INPUT);
  
  Serial.begin(9600);
  delay(1500);   
  Serial.println("starting now");
  
  ledek.shutdown(0,false);
  ledek.setIntensity(0,8);   // 1. kijelző,  fényerő ( 0-15)
  ledek.clearDisplay(0);
  ledek.shutdown(1,false);
  ledek.setIntensity(1,8);   // 2. kijelző,fényerő  
  ledek.clearDisplay(1);
  ledek.shutdown(2,false);
  ledek.setIntensity(2,8);   // 3. kijelző,fényerő
  ledek.clearDisplay(2);
  ledek.shutdown(3,false);
  ledek.setIntensity(3,8);   // 4. kijelző,fényerő
  ledek.clearDisplay(3);
  
  // teszt, pontok sorban felkapcsolnak
  
  for(byte i=0;i<8;i++)
      for(byte j=0;j<8;j++)
      {
       ledek.setLed(3,i,j,true);
       delay(20);
      }
  ledek.clearDisplay(3);
  for(byte i=0;i<8;i++)
      for(byte j=0;j<8;j++)
      {
       ledek.setLed(2,i,j,true);
       delay(20);
      }
  ledek.clearDisplay(2);   
  for(byte i=0;i<8;i++)
      for(byte j=0;j<8;j++)
      {
       ledek.setLed(1,i,j,true);
       delay(20);
      }
  ledek.clearDisplay(1);   
  for(byte i=0;i<8;i++)
      for(byte j=0;j<8;j++)
      {
       ledek.setLed(0,i,j,true);
       delay(20);
      }
  ledek.clearDisplay(0);   
  
  for(byte i=0;i<8;i++)
       ledek.setRow(3,i,Hbetu[i]);
  for(byte i=0;i<8;i++)
       ledek.setRow(2,i,Ebetu[i]);
  for(byte i=0;i<8;i++)
       ledek.setRow(1,i,Lbetu[i]);
  for(byte i=0;i<8;i++)
       ledek.setRow(0,i,Obetu[i]);     
       
  delay(3000);
  ledek.clearDisplay(3);
  ledek.clearDisplay(2);
  ledek.clearDisplay(1);
  ledek.clearDisplay(0);
  

  
}



void loop() { 
     
    randomSeed(analogRead(0));   // random generátor véletlen beállítása
    

    delay(900);
    ledmatrix(0,0);  // örül :)
    while(digitalRead(jobb)==1);  //  !!! várunk a "jobb" gomb lenyomására
    ledek.clearDisplay(0);

    for(byte i=1;i<50;i++)		//  50 db véletlenszám gyorsan (mintha gurulna a kocka)
      { szam=rand()%6; ledmatrix(3,szam+1); delay(80);   }

    szam=rand()%6;    // a kijelzett véletlenszám !!
    ledmatrix(3,szam+1);
    
    delay(2000);
   
    
    
  
}
