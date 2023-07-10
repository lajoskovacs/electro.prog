
// 8x8-as LEDmátrix   és MAX7219 vezérlő IC
// #include <Arduino.h>

#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LedControl.h"


#define DIN 10
#define CS 9
#define CLK 8
#define BE PINC
#define MIC PINC&4   //  mikrofon az A2 bemeneten (PC2) lábon

LedControl ledek=LedControl(DIN,CLK,CS,1);
unsigned int var=100;


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
 
  byte kezd=0; 
  byte szam;
 
  // CLKPR=0x84;       // clock=oszc/16
  DDRD= 0xFF;         // PORTD digitális    
  DDRB= 0xFF;   // POTRB digitális kimenet,  Max7219 
  DDRC= 0x00;   // A0 - A7 lábak bemenetek
  
  Serial.begin(9600);
  delay(1500);   
  Serial.println("starting now");
  
  ledek.shutdown(0,false);
  ledek.setIntensity(0,8);   // fényerő  0-15
  ledek.clearDisplay(0);
  
  for(byte i=0;i<8;i++)
      for(byte j=0;j<8;j++)
      {
       ledek.setLed(0,i,j,true);
       delay(20);
      }
  // ledek.clearDisplay(0);   
  for(byte i=1;i<7;i++)  { ledmatrix(0,i); delay(800);  }    
  
  delay(3000);
 
  ledek.clearDisplay(0);
  
  while(1) 
  {
    
    randomSeed(analogRead(0)); 
    ledmatrix(0,0);  
    Serial.println(BE);
    delay(900);
 
    // kezd=1;
    for(byte i=1;i<50;i++)
      { szam=rand()%6; ledmatrix(0,szam+1); delay(50);    }   // if((MIC)==0) kezd=0;

    szam=rand()%6;
    ledmatrix(0,szam+1);
    Serial.println(szam+1);
    delay(1000);
    while((MIC)==0); 
    ledek.clearDisplay(0);
    
    
  }
  
  
}



void loop() { 
  
  
  
}
