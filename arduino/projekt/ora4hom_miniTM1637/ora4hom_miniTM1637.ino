
 /*
     Óra és hőmérő
      ATMEGA328  mikrovezérlõre  (Arduino Pro mini, 5V 16MHz)
      + soros vezérlős hétszegmenses kijelző (TM1637)
     2016.03.24.   --> 2017.08.06   --> 2018.08.30.        KL
   */

//  # define F_CPU 16000000UL     // 16Mhz kvarc

#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

# define BAL 128    // "bal" PD7 lábon
# define JOBB 32   // "jobb" PD5 lábon
# define ALL 64   // "kozep"  PD6 lábon
# define KAPCS PIND

#define DATA 1 //PB0  (TM1637 DIO)
#define CLOCK 2 //PB1   (TM1637 CLK)
#define KOMM PORTB   // TM1637 adás 
#define ANSW PINB   // vétel
#define SERLO 4 //PB2  (74164 shiftreg. low  soros be)
#define CLLO 8 //PB3  (74164 shiftreg. low  clock)
#define SERHI 16 //PB4  (74164 shiftreg. high  soros be)
#define CLHI 32 //PB5  (74164 shiftreg. high  clock)

#define LEPTET PORTB   //   74164 shift regiszter vezérlés

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


  
void komm_start(void)   //TM1637
  {
    KOMM=KOMM|CLOCK; // CLK=1
    KOMM=KOMM|DATA; // DIO=1
    kesik_100us(2);
    KOMM=KOMM&(~DATA); // DIO=0
  }

void komm_stop(void)    //TM1637
  {
    KOMM=KOMM&(~CLOCK); // CLK=0  
    kesik_100us(2);
    KOMM=KOMM&(~DATA); // DIO=0
    kesik_100us(2);
    KOMM=KOMM|CLOCK; // CLK=1
    kesik_100us(2);
    KOMM=KOMM|DATA; // DIO=1
  }

void komm_ask(void)   //TM1637
  {
    KOMM=KOMM&(~CLOCK); // CLK=0  
    kesik_100us(5);
    while(ANSW&DATA);    
    // Serial.print("a");
    KOMM=KOMM|CLOCK; // CLK=1
    kesik_100us(2);
    KOMM=KOMM&(~CLOCK); // CLK=0  

  }

void displ_set(void)
  {
    
    KOMM=0;
    komm_start();
    send_byte(0x40);   // 0x40 --> address++    0x44 --> fix address 
    komm_ask();
    komm_stop();

    komm_start();
    send_byte(0x80);   // 0x80 --> display OFF   0x88 --> display ON
    komm_ask();
    komm_stop();

    komm_start();
    send_byte(0x8c);   //   0x88 --> display ON
    komm_ask();
    komm_stop();
  }




void kezdo_beall(void)
{
      
    CLKPR=0x84;       // clock=oszc/16
    DDRC = 0x00; // PORTC-n  NTC (hőmérő), PC1-NTC (analóg bemenet !)
    DDRB= 0xFF;         // PORTB digitális kienet,  TM1637 kijelző és shift regiszterek     
    DDRD= 0x00;              // PORTD digitális bemenet, kapcsolók
    PORTB= 0; 
    

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

  /*  TM1637 setup */
    displ_set();

    kesik_ms(300);


}


/********************************************************************************************************************/

void send_byte(unsigned char adat)    //TM1637
  {
    for(char i=0;i<8;i++)
    {
      KOMM=KOMM&(~CLOCK); // CLK=0  
      if(adat&1) KOMM=KOMM|DATA; // DIO=1
      else KOMM=KOMM&(~DATA); // DIO=0
      kesik_100us(3);
      adat=adat>>1;
      KOMM=KOMM|CLOCK; // CLK=1
      kesik_100us(3);
    }
  }


void send_szam(unsigned int szam,unsigned char pont,unsigned char vill)
  {
    static const unsigned char szeg7[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
    // static const unsigned char szeg7[]={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6};
    byte pontkell=0;
/*
    komm_start();
    send_byte(0x80);   // 0x80 --> display OFF  
    komm_ask();
    komm_stop();
  */  
    komm_start();
    send_byte(0xc0);   // address
    komm_ask();
    
    if(pont==1) pontkell=128;
    if(szam<1000) send_byte(0);
    else send_byte(szeg7[(szam/1000)%10]);
    komm_ask();
    if(szam<100) send_byte(0);
    else send_byte(szeg7[(szam/100)%10]);
    komm_ask();
    if(szam<10) send_byte(0);
    else send_byte(szeg7[(szam/10)%10]);
    komm_ask();
    send_byte(szeg7[szam%10]|pontkell);
    komm_ask();  
    komm_stop();  
    
  }

void lepteto_LO(char szam)
{
  byte i;
  byte x;  // segéd változó
  
  // LEPTET&=~CLLO;  //  órajel  1->0
  for(i=0;i<8;i++) 
  {
        LEPTET&=~CLLO;  //  órajel  1->0
        x=szam&128;    // első bit leválasztása
        if (x==128) LEPTET|=SERLO;  // és kiküldése az also (low) a shift regiszterbe
          else LEPTET&=~SERLO;
        kesik_us(2);  
        LEPTET|=CLLO;  //  órajel  0->1
        kesik_us(2); 
        
        szam=szam<<1;  // léptetés balra  -->  a már kiküldött első bit levágása
    }
  
}


void lepteto_HI(char szam)
{
  byte i;
  byte x;  // segéd változó
  
  // LEPTET&=~CLHI;  //  órajel  1->0
  for(i=0;i<8;i++) 
  {
        LEPTET&=~CLHI;  //  órajel  1->0
        x=szam&128;    // első bit leválasztása
        if (x==128) LEPTET|=SERHI;  // és kiküldése az also (low) a shift regiszterbe
          else LEPTET&=~SERHI;
        kesik_us(2);  
        LEPTET|=CLHI;  //  órajel  0->1
        kesik_us(2); 
        
        szam=szam<<1;  // léptetés balra  -->  a már kiküldött első bit levágása
    }
   
}


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
    kesik_ms(15);
    sei();          // globális megszakítás engedélyezés

    while(1)
    {

      if(homero)             //  hõmérõ kijelzés
        {
         if(ciklus%15==0) ntc_lekerdez();               // hõmérséklet lekérdezése
         send_hom_TM1637(gelojel, gfok);
         send_mperc_shift(gmasperc);
        }
      else       //   idő kijelzés
        {
          if(ora_jar) 
            { send_ido_TM1637(gora, gperc, gmasperc, szamlal, 0); send_mperc_shift(gmasperc); }
          if(ora_set) send_ido_TM1637(gora, gperc, gmasperc, szamlal, 1);  // óra villogtatva
          if(perc_set) send_ido_TM1637(gora, gperc, gmasperc, szamlal, 2);  // perc  villogtatva
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

void send_ido_TM1637(char ora, char perc, char mperc, char utem, char vill) {
         // ha vill=1 -> ora villog, ha vill=2 -> perc villog,  vill=0 -> semmi sem villog
     char szam1,szam2,szam3,szam4;
     char villog,pont;
     static const unsigned char szeg7[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
    // static const unsigned char szeg7[]={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6};
     komm_start();
     send_byte(0xc0);   // address
     komm_ask();
       
     szam1=ora/10;
     szam2=ora%10;
     szam3=perc/10;
     szam4=perc%10;
     pont=(utem&2)<<6;  // fél másodpercig 1-es, fél másodpercig 0-as, (1Hz)
     villog=utem&1;  // negyed másodpercig 1-es, negyed  másodpercig 0-as, (2Hz)
       
       /*  óra  1.  és 2. számjegye */ 
     if(vill==1) send_byte(szeg7[szam1]*villog);        // ha ora_set -> villog
     else send_byte(szeg7[szam1]);                //    egyebkent nem villog
     komm_ask();
     if(vill==0) send_byte(szeg7[szam2]|pont);        // ha ora_jar -> nem villog  + pont
     else if(vill==1) send_byte(szeg7[szam2]*villog);        // ha ora_set -> villog
     else send_byte(szeg7[szam2]);                //    egyébként (ha perc_set) nem villog
     komm_ask();  
      /*  perc  1.  és 2. számjegye */ 
     if(vill==2) send_byte(szeg7[szam3]*villog);        // ha perc_set -> villog
     else send_byte(szeg7[szam3]);                //    egyebkent nem villog
     komm_ask(); 
     if(vill==2) send_byte(szeg7[szam4]*villog);        // ha perc_set -> villog
     else send_byte(szeg7[szam4]);                //    egyebkent nem villog
     komm_ask();  

 }
 
 
/********************************************************************************************************************/

void send_mperc_shift(char mperc) {
         // másodpercek jelzése LED-eken  --> annyi LED világít, ahanyas számjegy !!
         //  bal oldalon (HI) az 1. számjegy, jobb oldalon (LO) a 2.
         // mindkét shift regiszter (HI és LO) 8 bites ! --> a másodperc 0-tól 59-ig mehet ---> 
         // első számjegy 0-5 (HI), a második 0-9 (LO)!!! --> a 9-es számjegyhez a a másik 8-as LED
         // csoportból is kell egyet használni, a jobboldalit
     char szam1,szam2;
     
     static const unsigned char led1[]={0,128,192,224,240,248,252,254,255,255};
     static const unsigned char led2[]={0,1,3,7,15,31,63,127,255,255};
     
     szam1=mperc/10;   // első számjegy
     szam2=mperc%10;   // második számjegy
     
     lepteto_LO(led2[szam2]);
     if(szam2<9)  lepteto_HI(led1[szam1]);
     else lepteto_HI(led1[szam1]+1); // 9-es 2. számjegynél az 1. számjegyhez tartozó - amúgy nem használt - 
                                     // LED bekapcsolása szükséges ! (1-es helyiértéken van)
}


/********************************************************************************************************************/


void send_hom_TM1637(char elojel,int fok) {

        // elojel   0 - pozitív   1- negatív  !!

      static const unsigned char szeg7[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0,64};
        // static const unsigned char szeg7[]={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,0,2};
      byte pontkell=0;

      char szam1=0;                      // elsõ számjegy
      char szam2=0;                   // 2. számjegy
      char szam3=0;                 // 3. számjegy
      char szam4=0;             // tizedesjegy
      char i;

      komm_start();
      send_byte(0xc0);   // address
      komm_ask();
       

      if(!elojel) {         // pozitív hõmérséklet
            szam4=fok%10;          // tizedesjegy
            fok=fok/10;
            szam3=fok%10;
            fok=fok/10;
            szam2=fok%10;
            szam1=fok/10;
            if(szam1==0)  { szam1=10; if(szam2==0) szam2=10; } // üres !!
         }

       if(elojel) {
            szam4=fok%10;
            fok=fok/10;
            szam3=fok%10;
            szam2=fok/10;
            szam1=11;     //  '-'

         }

        /* hőmérséklet  1.  és 2. számjegye */      
       send_byte(szeg7[szam1]);               
       komm_ask();        
       send_byte(szeg7[szam2]);             
       komm_ask(); 
         /* hőmérséklet  3.  és 4. számjegye */  
       send_byte(szeg7[szam3]|128);                //  tizedespont
       komm_ask();                       
       send_byte(szeg7[szam4]);               
       komm_ask();  
       

 }


/********************************************************************************************************************/


void loop() {
  // put your main code here, to run repeatedly:

}




