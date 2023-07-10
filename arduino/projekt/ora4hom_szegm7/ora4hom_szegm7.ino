
 /*
     Óra és hőmérő
      ATMEGA328  mikrovezérlõre
     2016.03.24.   --> 2017.08.06            KL
   */

//  # define F_CPU 16000000UL     // 16Mhz kvarc

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

# define kapcs_fel 8   // "fel" PC3 lábon
# define kapcs_bal 4    // "bal" PC2 lábon
# define kapcs_jobb 1   // "jobb" PC0 lábon
# define kapcs_le 2   // "le"  PC1 lábon
// # define kapcs_start 16

# define szegmens7 PORTD
# define vez_szegm PORTB
# define tr1  8   // 1. kijelző PB3 lábon
# define tr2  4   // 2. kijelző PB2 lábon
# define tr3  2   // 3. kijelző PB1 lábon
# define tr4  1   // 4. kijelző PB0 lábon

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
int gfok=0;
char gelojel=0;
char homero=0;   // hőmérséklet kijelzése ( ha értéke 1)

/* char tomb7[ ]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0,0b01000000,0b01110110,0b00111000,0};    */
                                                    // 0,1,2,3,4,5,6,7,8,9,üres,-,H,L       szegm.  h g f e d c b a
byte tomb7[ ]={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,0,0b00000010,0b01101110,0b00011100,0};
                                                    // 0,1,2,3,4,5,6,7,8,9,üres,-,H,L        szegm.  a b c d e f g h

const int foktar[35]={-65,-50,-45,-40,-35,-30,-25,-20,-15,-10,-5,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,150};
const int fesz[35]={0,30,40,52,66,84,106,132,161,195,233,275,320,366,415,464,512,559,604,647,687,723,757,786,813,837,859,878,894,909,922,933,943,952,1023};


/********************************************************************************************************************/


   

void setup() {
  // put your setup code here, to run once:

    char i=0;
    char jobble=0;

    char ciklus=0;

    cli();   // megszakítás tiltás
    kezdo_beall();
    kesleltet(15);
    sei();          // globális megszakítás engedélyezés

    while(1)
    {

      if(homero)             //  hõmérõ kijelzés
        {
         if(ciklus%15==0) ntc_lekerdez();               // hõmérséklet lekérdezése
         hom7szegmre(gelojel, gfok);
        }
      else       //   idő kijelzés
        {
          if(ora_jar) idoki7szegmre(gora, gperc, gmasperc, szamlal, 0);
          if(ora_set) idoki7szegmre(gora, gperc, gmasperc, szamlal, 1);  // óra villogtatva
          if(perc_set) idoki7szegmre(gora, gperc, gmasperc, szamlal, 2);  // perc  villogtatva
        }

       /*  nyomógombok lekérdezése  */
      if(!jobble && !(PINC&kapcs_jobb))   //jobb kapcsoló  ---> felfutó él detektálás
        {
          jobble=1; 
          homero=0;
          if(ora_jar || perc_set) {ora_set=1; ora_jar=0; perc_set=0;}    // óra beállítása
          else {ora_set=0; ora_jar=0; perc_set=1;}         //  perc beállítása
        }
      if(PINC&kapcs_jobb)  jobble=0;   //jobb kapcsoló elengedése
      if(!ora_jar && !(PINC&kapcs_bal))      // bal kapcsoló---> normál üzemmód,  jár az óra
          {
            cli();   // megszakítás tiltás
            ora_jar=1; ora_set=0; perc_set=0;
            homero=0;
            szamlal=0; gmasperc=0;
            TCNT1=0; // számlálás előlről !
            sei();          // globális megszakítás engedélyezés
          }     
      if((ciklus%20==0)&&ora_set) 
        {                               // óra érték beállítása
             if((PINC&kapcs_fel)==0) {gora++; if(gora>23) gora=0;  }        // fel  ---> növelés
             if((PINC&kapcs_le)==0)  {if(gora>1) gora--; else gora=23;    }    // le ---> csökkentés
        }
      if((ciklus%20==0)&&perc_set) 
        {                               // perc érték beállítása
             if((PINC&kapcs_fel)==0) {gperc++; if(gperc>59) gperc=0;  }          // fel  ---> növelés
             if((PINC&kapcs_le)==0) {if(gperc>1) gperc--; else gperc=59;    }    // le ---> csökkentés
        }

      if(ora_jar&&((PINC&kapcs_le)==0))  homero=1;      //  le ---> hõmérõ kijelzés

      if(homero&&((PINC&kapcs_fel)==0))  homero=0;      //  fel ---> óra kijelzés
  
      ciklus++;

    }

  

}


/********************************************************************************************************************/


void kezdo_beall(void)
{
      
    CLKPR=0x84;       // clock=oszc/16
    DDRC = 0x00; // PORTC-n kapcsolók és NTC (hőmérő), PC0-fel, PC1-bal, PC2-jobb, PC3-le, PC4-NTC (analóg bemenet !)
    // ANSEL=0;    // PORTA digitális I/O
    //  ANSELH=0;    // PORTB  digitális I/O
    DDRB= 0xFF;         // 7 szegmenses kijelzõ  vezérlése        1 szj -> PB0   2.szj. ->  PB1   3.szj. ->  PB2      4.szj. -> PB3
    DDRD= 0xFF;              // 7 szegmenses kijelzõ,     a-h szegmensei ->       PD0 -- PD7
    PORTD= 0; 
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

    ADMUX= 0b01000100;  // Uref: AVCC + AREF-kondi,  ADLAR=0,    ADC4  lesz a bemeneti csatorna,

    ADCSRA= 0b10000011;  //  ADEN=1  engedélyezés   ADPSn -- 011 --> Fosc/8

    

    // 8-infra    5-NTC  13-fotoell.

   
    kesleltet(300);


}

 /********************************************************************************************************************/



ISR(TIMER1_COMPA_vect)     //  ISR(SIG_OUTPUT_COMPARE0A)
 //TMR1 interrupt
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

void idoki7szegmre(char ora, char perc, char mperc, char utem, char vill) {
         // ha vill=1 -> ora villog, ha vill=2 -> perc villog,  vill=0 -> semmi sem villog
     char szam1,szam2,szam3,szam4;
     char villog,pont;
      
       szam1=ora/10;
       szam2=ora%10;
       szam3=perc/10;
       szam4=perc%10;
       pont=(utem&2)>>1;  // fél másodpercig 1-es, fél másodpercig 0-as, (1Hz)
       villog=utem&1;  // negyed másodpercig 1-es, negyed  másodpercig 0-as, (2Hz)
       
       vez_szegm=0;                //  mindegyik tranzisztor kikapcsolása
       if(vill==1) szegmens7=tomb7[szam1]*villog;        // ha ora_set -> villog
       else szegmens7=tomb7[szam1];                //    egyebkent nem villog
       vez_szegm=tr1;                //  T1  tranz. bekapcs.  1. kijelzõ világít  ->  ora 1. szamjegy
       kesleltet(4);
       
       vez_szegm=0;                //  mindegyik tranzisztor kikapcsolása
       if(vill==0) szegmens7=tomb7[szam2]|pont;                        // ha ora_jar -> nem villog  + pont
       else if(vill==1) szegmens7=tomb7[szam2]*villog;  // ha ora_set -> villog
       else szegmens7=tomb7[szam2];                //   egyébként (ha perc_set) nem villog
       vez_szegm=tr2;                //  T2  tranz. bekapcs.  2. kijelzõ világít ->  ora 2. szamjegy
       kesleltet(4);
                        
       vez_szegm=0;                //  mindegyik tranzisztor kikapcsolása
       if(vill==2) szegmens7=tomb7[szam3]*villog;   // ha perc_set -> villog
       else szegmens7=tomb7[szam3];                //  egyebkent nem villog    
       vez_szegm=tr3;                //  T3  tranz. bekapcs.  3. kijelzõ világít   -> perc 1. szamjegy
       kesleltet(4);
                       
       vez_szegm=0;                //  mindegyik tranzisztor kikapcsolása
       if(vill==2) szegmens7=tomb7[szam4]*villog;    // ha perc_set -> villog
       else szegmens7=tomb7[szam4];                //      egyebkent nem villog
       vez_szegm=tr4;                //  T4  tranz. bekapcs.  4. kijelzõ világít   -> perc 2. szamjegy
       kesleltet(4);


 }
/********************************************************************************************************************/


void hom7szegmre(char elojel,int fok) {

        // elojel   0 - pozitív   1- negatív  !!

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
            if(szam1==0)  { szam1=10; if(szam2==0) szam2=10; } // üres !!
       }

       if(elojel) {
            szam4=fok%10;
            fok=fok/10;
            szam3=fok%10;
            szam2=fok/10;
            szam1=11;     //  '-'

       }

       vez_szegm=0;                //  mindegyik tranzisztor kikapcsolása
       szegmens7=tomb7[szam1];                //
       vez_szegm=tr1;                //  T1  tranz. bekapcs. -> 1. kijelzõ világít
       kesleltet(4);
                    
       vez_szegm=0;                //  mindegyik tranzisztor kikapcsolása
       szegmens7=tomb7[szam2];                //
       vez_szegm=tr2;                //  T2  tranz. bekapcs. -> 2. kijelzõ világít
       kesleltet(4);
                       
       vez_szegm=0;                //  mindegyik tranzisztor kikapcsolása
       szegmens7=tomb7[szam3]|0b00000001;                //  tizedespont
       vez_szegm=tr3;                //  T3  tranz. bekapcs. -> 3. kijelzõ világít
       kesleltet(4);
                     
       vez_szegm=0;                //  mindegyik tranzisztor kikapcsolása
       szegmens7=tomb7[szam4];                //
       vez_szegm=tr4;                //  T4  tranz. bekapcs. -> 4. kijelzõ világít
       kesleltet(4);

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


void loop() {
  // put your main code here, to run repeatedly:

}




