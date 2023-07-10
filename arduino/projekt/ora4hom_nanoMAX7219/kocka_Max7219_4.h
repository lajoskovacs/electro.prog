
// 8x8-as LEDmátrix   és MAX7219 vezérlő IC
// #include <Arduino.h>


#include "LedControl.h"


#define DIN 10		// PB2
#define CS 9		//  PB1
#define CLK 8		//  PB0

LedControl ledek=LedControl(DIN,CLK,CS,4);
unsigned int var=100;


const static byte ures [] = {
 B00000000,
 B00000000,
 B00000000,
 B00000000,
 B00000000,
 B00000000,
 B00000000,
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


const static byte minusz [] = {
 B00000000,
 B00000000,
 B00000000,
 B00111100,
 B00000000,
 B00000000,
 B00000000,
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


/********************************************************************************************************************/


void kesik_ms(unsigned int i)
  {
    while(i-->0)  
      for(int j=0;j<3160;j++) asm ( "nop \n" );
  }

void kesik_100us(unsigned int i)
  {
    while(i-->0)  
       for(int j=0;j<313;j++) asm ( "nop \n" );
       
  }
 
void kesik_10us(unsigned int i)
  {
    while(i-->0)  
       for(int j=0;j<31;j++) asm ( "nop \n" );
       
  }
  
void kesik_us(unsigned int i)
  {
    while(i-->0)  
       for(int j=0;j<4;j++) asm ( "nop \n" );
       
  } 


/********************************************************************************************************************/


/*
void szam_ki(byte digit, byte szam)
{
 for(byte i=0;i<8;i++)
       ledek.setRow(digit,i,szamok[szam][i]);

}   */


void ledmatrix(byte digit, byte szam) 
{ 
  switch (szam)
  {
  case 1: for(byte i=0;i<8;i++) ledek.setRow(digit,i,Sz1[i]);
          break;
  case 2: for(byte i=0;i<8;i++) ledek.setRow(digit,i,Sz2[i]);
          break;        
  case 3: for(byte i=0;i<8;i++) ledek.setRow(digit,i,Sz3[i]);
          break;
  case 4: for(byte i=0;i<8;i++) ledek.setRow(digit,i,Sz4[i]);
          break;
  case 5: for(byte i=0;i<8;i++) ledek.setRow(digit,i,Sz5[i]);
          break;        
  case 6: for(byte i=0;i<8;i++) ledek.setRow(digit,i,Sz6[i]);
          break;       
  case 7: for(byte i=0;i<8;i++) ledek.setRow(digit,i,Sz7[i]);
          break;
  case 8: for(byte i=0;i<8;i++) ledek.setRow(digit,i,Sz8[i]);
          break;        
  case 9: for(byte i=0;i<8;i++) ledek.setRow(digit,i,Sz9[i]);
          break;       
  case 0: for(byte i=0;i<8;i++) ledek.setRow(digit,i,Sz0[i]);
          break; 
  case 11: for(byte i=0;i<8;i++) ledek.setRow(digit,i,ures[i]);
          break;   
  case 99: for(byte i=0;i<8;i++) ledek.setRow(digit,i,minusz[i]);
          break;  
                
  }
}


/********************************************************************************************************************/

 
void send_ido(char ora, char perc, char mperc, char utem, char vill) {
         // ha vill=1 -> ora villog, ha vill=2 -> perc villog,  vill=0 -> semmi sem villog
     char szam1,szam2,szam3,szam4;
     char villog,pont;
    
       
     szam1=ora/10;
     szam2=ora%10;
     szam3=perc/10;
     szam4=perc%10;
     pont=(utem&2)<<6;  // fél másodpercig 1-es, fél másodpercig 0-as, (1Hz)
     villog=utem&1;  // negyed másodpercig 1-es, negyed  másodpercig 0-as, (2Hz)

    /*  perc  2. számjegye */ 
     if(vill==2 && villog==0) ledmatrix(0,11);       // ha perc_set -> villog
     else ledmatrix(0,szam4);                        //    egyebkent nem villog

    /*  perc  1. számjegye */ 
     if(vill==2 && villog==0) ledmatrix(1,11);        // ha perc_set -> villog
     else ledmatrix(1,szam3);                         //    egyebkent nem villog
     
     /*  óra  2. számjegye */ 
     if(vill==1 && villog==0) ledmatrix(2,11);        // ha ora_set -> villog 
     else ledmatrix(2,szam2);                       //    egyébként nem villog
     if(vill==0 && pont)         // ha ora_jar ->   + pont
        { ledek.setLed(2,3,6,true);  ledek.setLed(2,4,6,true); }

     /*  óra  1. számjegye */ 
     if(vill==1 && villog==0) ledmatrix(3,11);        // ha ora_set -> villog 
     else ledmatrix(3,szam1);                       //    egyébként nem villog
      
 }



/********************************************************************************************************************/
 
void send_hom(char elojel,int fok) {

        // elojel   0 - pozitív   1- negatív  !!

      byte pontkell=0;

      char szam1=0;                      // elsõ számjegy
      char szam2=0;                   // 2. számjegy
      char szam3=0;                 // 3. számjegy
      char szam4=0;             // tizedesjegy
      char i;

    
   

      if(!elojel) {         // pozitív hõmérséklet
            szam4=fok%10;          // tizedesjegy
            fok=fok/10;
            szam3=fok%10;
            fok=fok/10;
            szam2=fok%10;
            szam1=fok/10;
            if(szam1==0)  { szam1=11; if(szam2==0) szam2=11; } // üres !!
         }

       if(elojel) {
            szam4=fok%10;
            fok=fok/10;
            szam3=fok%10;
            szam2=fok/10;
            szam1=99;     //  '-'

         }


       /* hőmérséklet  4. számjegye */  
       ledmatrix(0,szam4);                        
       /* hőmérséklet  3. számjegye */  
       ledmatrix(1,szam3);          
       ledek.setLed(1,6,6,true);              //   + tizedespont           
        /* hőmérséklet  2.  és 1. számjegye */      
       ledmatrix(2,szam2);  

       ledmatrix(3,szam1);                         
  

 }



/********************************************************************************************************************/



void kocka_setup() {
 
  CLKPR=0x84;       // clock=oszc/16
  // DDRB= 0xFF;         // PORTB digitális kienet,  Max7219    

   
  ledek.shutdown(3,false);
  ledek.shutdown(2,false);
  ledek.shutdown(1,false);
  ledek.shutdown(0,false);
  ledek.setIntensity(3,8);   // fényerő  0-15
  ledek.setIntensity(2,8);   // fényerő  0-15
  ledek.setIntensity(1,8);   // fényerő  0-15
  ledek.setIntensity(0,8);   // fényerő  0-15
  ledek.clearDisplay(3);
  ledek.clearDisplay(2);
  ledek.clearDisplay(1);
  ledek.clearDisplay(0);

  for(byte i=0;i<8;i++)
       ledek.setRow(3,i,Hbetu[i]);
  for(byte i=0;i<8;i++)
       ledek.setRow(2,i,Ebetu[i]);
  for(byte i=0;i<8;i++)
       ledek.setRow(1,i,Lbetu[i]);
  for(byte i=0;i<8;i++)
       ledek.setRow(0,i,Obetu[i]);     
       
  kesik_ms(3000);
  ledek.clearDisplay(3);
  ledek.clearDisplay(2);
  ledek.clearDisplay(1);
  ledek.clearDisplay(0);

  
}
