
 /*
     Óra és hőmérő
      ATMEGA328  mikrovezérlõre  (Arduino Pro mini, 5V 16MHz)
      + LEDmátrix kijelző  MAX7219 vezérlővel
     2016.03.24. --> 2017.08.06  --> 2018.08.30. --> 2019.08.12.      KL
   */

//  # define F_CPU 16000000UL     // 16Mhz kvarc

#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "kocka_Max7219_4.h"


# define BAL 128    // "bal" (le) PD7 lábon   (7)
# define JOBB 32   // "jobb" (fel) PD5 lábon   (5)
# define ALL 64   // "kozep-all" (vissza)  PD6 lábon  (6)
# define IND 16    // "indul-start" (set) PD4 lábon   (4)
# define KAPCS PIND

# define kesleltet delay


/*************************************************************************************/

byte szamlal=0;
byte gora=0;
byte gperc=0;
byte gmasperc=0;
byte kijelzo7=0;
char ora_jar=1;   //  normál óra üzemmód
char ora_set=0; // az óra beállítása
char perc_set=0;  // a perc beállítása
int  gfok=0;
char gelojel=0;
char homero=0;   // hőmérséklet kijelzése ( ha értéke 1)

const int foktar[35]={-65,-50,-45,-40,-35,-30,-25,-20,-15,-10,-5,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,150};
const int fesz[35]={0,30,40,52,66,84,106,132,161,195,233,275,320,366,415,464,512,559,604,647,687,723,757,786,813,837,859,878,894,909,922,933,943,952,1023};

byte data4[] = {0b00000110, 0b00000111,0b01111111, 0b00000110};

/********************************************************************************************************************/


void kezdo_beall(void)
{
      
    CLKPR=0x84;       // clock=oszc/16
    DDRC = 0x00; // PORTC-n  NTC (hőmérő), PC1-NTC (analóg bemenet !)
    DDRB= 0xff;         // PORTB digitális kimenet,    
    DDRD= 0x00;              // PORTD digitális bemenet
    PORTB= 0;
   //  PORTD= 0xff;
    

    // timer, interrupt   
    // timer1  (16bites)
    TCCR1A=0x00;  // CTC 4. mód, komp. érétk elérése után nullázás    WGM10=0   WGM11=0
    TCCR1B=0x0D;  // Clock/1024  ----  WGM12=1    WGM13=0
    OCR1A= 3124; //  clock/(1024*3125)  megszaktítás kb.5-ször/sec.
    TIMSK1=0x02;  // megszakítás engedélyezése, komp. reg.A    OCIE1A=1
    
    /*
     // timer2   (8bites)
    TCCR2A=0x02;  // CTC mód, komp. érték elérése után nullázás    WGM01=1
 
    TCCR2B=0x05;  // Clock/1024  ----  számlálás kb. 1/15 millisecundumonként  CS22=1 CS21=0  CS20=1
    OCR2A= 124; //  megszaktítás Clock/(1024*125) ---16000000/(1024*125)= 125    125-szor/sec.
    TIMSK2=0x02;  // megszakítás engedélyezése, komp. reg.A    OCIEA=1    */

 /*  ADC_Init    */
 
      /*  ADMUX reg.   --->   |REFS1|REFS0|ADLAR|-|MUX3|MUX2|MUX1|MUX0|     

               REFS1|REFS0 -- Uref   00-AREF    01-AVC,AREF-C    11-internal 1,1V 
               MUXn  -- csatorna választás   -->  0-7  ->  ADC0-ADC7 
               ADLAR -- az eredmény 10 bitje hogyan helyezkedik el a 2x8bites eredmény regiszterekben  (ADCH és ADCL)
                          ADLAR=0 -->  utolsó 10 bit,   ADLAR=1 -->  első 10 bit 

         ADCSRA reg.   --->   |ADEN|ADSC|ADATE|ADIF|ADIE|ADPS2|ADPS1|ADPS0|

                ADPS2-ADPS0   clock    000 --> Fosc/2   001 --> Fosc/2    010 --> Fosc/4    011 --> Fosc/8
                                        100 --> Fosc/16   101 --> Fosc/32    110 --> Fosc/64    111 --> Fosc/128
          
                 ADEN engedélyezés,   ADIE  -- interrupt enable,   ADSC --  A/D start,        */

    ADMUX= 0b01000001;  // Uref: AVCC + AREF-kondi,  ADLAR=0,    ADC1 (PC1)  lesz a bemeneti csatorna,

    ADCSRA= 0b10000011;  //  ADEN=1  engedélyezés   ADPSn -- 011 --> Fosc/8

  

    kesik_ms(300);


}


/********************************************************************************************************************/



void ntc_lekerdez() {

    char i=0;
    char negativ=0;
    int poti=0;
    long fok;


         // ADC4 NTC (hõérzékelõ)  

         ADCSRA=ADCSRA|64;  // ADSC bit --> 1   A/D indítása
         while((ADCSRA&64)!=0);   // várakozás A/D eredményre,  amikor  0 lesz
         poti=ADCL;   // eredmény alsó 8 bitje
         poti+=ADCH<<8;     // eredmény:  + a két felső bit
                        
         if(poti<0) poti=0;
         if(poti>1023) poti=1023;
         i=0;
         while(poti>fesz[i]) i++;
         fok=100*foktar[i];               //  -65  -----    +150
         if(i>1 && i<34) fok=fok-500*(fesz[i]-poti)/(fesz[i]-fesz[i-1]);
         if(i==34) fok=fok-4000*(fesz[i]-poti)/(fesz[i]-fesz[i-1]);
         if(i==1) fok=fok-1500*(fesz[i]-poti)/(fesz[i]-fesz[i-1]);
         if(i==0) fok=-6500;
         if(fok<0) { negativ=1; fok=-fok;  }     // elõjel beállítása
         if(fok%10>5)   gfok=fok/10+1;
         else   gfok=fok/10;     // kerekítés egy tizedes jegyre
         gelojel=negativ;

}


/********************************************************************************************************************/
 
void setup() {
  // put your setup code here, to run once:

    char i=0;
    char allle=0;

    char ciklus=0;

    cli();   // megszakítás tiltás
    kezdo_beall();
    kocka_setup();
    kesik_ms(1000);
    sei();          // globális megszakítás engedélyezés

    while(1)
    {

      if(homero)             //  hõmérõ kijelzés
        {
         if(ciklus%15==0) ntc_lekerdez();               // hõmérséklet lekérdezése
         send_hom(gelojel, gfok);
        
        }
      else       //   idő kijelzés
        {
          if(ora_jar) 
            { send_ido(gora, gperc, gmasperc, szamlal, 0);  }
          if(ora_set) send_ido(gora, gperc, gmasperc, szamlal, 1);  // óra villogtatva
          if(perc_set) send_ido(gora, gperc, gmasperc, szamlal, 2);  // perc  villogtatva
        }

       /*  nyomógombok lekérdezése  */
      if(!allle && !(KAPCS&ALL))   //középső (ALL) kapcsoló  ---> felfutó él detektálás
        {
          allle=1; 
          homero=0;
          if(ora_jar) {ora_set=1; ora_jar=0; perc_set=0;}    //normál üzemmód -->  óra beállítása
          else if(ora_set) {ora_set=0; ora_jar=0; perc_set=1;}         //  óra beállítása --> perc beállítása
          else if(perc_set)      //  perc beállítása --> normál üzemmód (jár az óra)
            {
              cli();   // megszakítás tiltás
              ora_jar=1; ora_set=0; perc_set=0;
              szamlal=0; gmasperc=0;
              TCNT1=0; // számlálás előlről !
              sei();          // globális megszakítás engedélyezés
            } 
        }
      if(KAPCS&ALL)  allle=0;   //ALL kapcsoló elengedése
         
      if((ciklus%10==0)&&ora_set) 
        {                               // óra érték beállítása
             if((KAPCS&JOBB)==0) {gora++; if(gora>23) gora=0;  }        // fel  ---> növelés
             if((KAPCS&BAL)==0)  {if(gora>1) gora--; else gora=23;    }    // le ---> csökkentés
        }
      if((ciklus%10==0)&&perc_set) 
        {                               // perc érték beállítása
             if((KAPCS&JOBB)==0) {gperc++; if(gperc>59) gperc=0;  }          // fel  ---> növelés
             if((KAPCS&BAL)==0) {if(gperc>1) gperc--; else gperc=59;    }    // le ---> csökkentés
        }

      if(ora_jar&&((KAPCS&JOBB)==0))  homero=1;      //  le ---> hõmérõ kijelzés

      if(homero&&((KAPCS&BAL)==0))  homero=0;      //  fel ---> óra kijelzés
  
      ciklus++;
      kesik_ms(20);
    }


}




 /********************************************************************************************************************/



ISR(TIMER1_COMPA_vect)     //  ISR(SIG_OUTPUT_COMPARE0A)
 //TMR1 interrupt  -->  0,2 másodpercenként !!!
{
    szamlal++;
    if(szamlal==5) { szamlal=0; gmasperc++;  }   //timer1
    // if(szamlal==125) { szamlal=0; gmasperc++;  }    // timer2
    if(ora_jar)
    {
       if(gmasperc==60) { gmasperc=0; gperc++;  }
       if(gperc==60) { gperc=0; gora++;  }
       if(gora==24) { gora=0;  }
    }

}


/********************************************************************************************************************/



void loop() {
  // put your main code here, to run repeatedly:

}
