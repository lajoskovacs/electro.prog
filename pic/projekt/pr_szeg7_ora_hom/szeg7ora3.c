 /*
     Óra és hõmérõ
     PIC16F887 mikrovezérlõre
     2016-2017           KL
   */
   
char szamlal=0;
char ora_jar=1;
char ora_set=0;
char perc_set=0;
char homero=0;
char gora=0;
char gperc=0;
char gmasperc=0;
char kijelzo7=0;
int gfok=0;
char gelojel=0;

/* char tomb7[ ]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0,0b01000000,0b01110110,0b00111000,0};    */
                                                    // 0,1,2,3,4,5,6,7,8,9,üres,-,H,L       szegm.  h g f e d c b a
char tomb7[ ]={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,0,0b00000010,0b01101110,0b00011100,0};
                                                    // 0,1,2,3,4,5,6,7,8,9,üres,-,H,L        szegm.  a b c d e f g h
const int foktar[35]={-65,-50,-45,-40,-35,-30,-25,-20,-15,-10,-5,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,150};
const int fesz[35]={0,30,40,52,66,84,106,132,161,195,233,275,320,366,415,464,512,559,604,647,687,723,757,786,813,837,859,878,894,909,922,933,943,952,1023};

sbit kapcs_fel at PORTA.B0;
sbit kapcs_bal at PORTA.B1;
sbit kapcs_jobb at PORTA.B2;
sbit kapcs_le at PORTA.B3;
sbit kapcs_stop at PORTA.B5;
char szegmens7 at PORTD;
char vez_szegm at PORTB;

/********************************************************************************************************************/

void kezdo_beall(void)
{
      /*Ocsillator selections here
       OSCCON reg.   --->   |-|IRCF2|IRCF1|IRCF0|OSTS(r)|HTS(r)|LTS(r)|SCS|
                           default: -1101000           (r) --> csak olvasható
                           SCS  1 --->  belsõ oszc.     0 ---> külsõ
                           OSTS-1 --->  külsõ oszc. fut,  0 ---> belsõ
       OSCCON = %01100001 for 4mhz, or OSCCON = %01110001for 8mhz
        OSCCON = %01110001           'Int CLK 8MHz
        ANSEL= %11111111       '$0F = disable A/D converter
        option_reg = 7     'switch pull-ups ON
      END of oscillator selections  */

    //OSCCON = 0x71;   // belsõ oszcillátor,  8MHZ
    OSCCON = 0x70;   // külsõ oszcillátor,  16MHZ
    TRISA = 0xFF; // PORTA-n kapcsolók, RA0-fel, RA1-bal, RA2-jobb, RA3-le, RA5-stop
    ANSEL=0;    // PORTA digitális I/O
    ANSELH=0;    // PORTB  digitális I/O
    TRISB=0;         // 7 szegmenses kijelzõ  vezérlése        1 szj ->  RB0   2.szj. ->  RB1   3.szj. ->  RB2      4.szj. -> RB3
    TRISD=0;             // 7 szegmenses kijelzõ,     a-h szegmensei ->       RD0 -- RD7
    PORTD=0;
    PORTB=0;
   // C1ON_bit = 0;               // Disable comparators
    // C2ON_bit = 0;

    /* timer, interrupt    */

     T1CON= 0b00110101;   // TIMER1  elõosztó: 8 ---> Fosc/4 * 1/8      ---> ha Fosc=16Mhz ---> Ft1=500kHz
    PIR1=0;  // jelzõbit törlése
    TMR1H=0b00001011;
    TMR1L=0b11011100;      // 3036 kezdõ érték ---> 62500 lépés  --->  500kHz/62500=8Hz ---> szamlal 1s alatt 8-et lép !!
    INTCON=0b11000000;    //  interrupt engedélyezve, globális és periféria
    PIE1=0b00000001;      // TMR1 interrupt engedélyezve
      /*  INTCON reg.   --->   |GIE|PEIE|T0IE|INTE|RBIE|T0IF|INTF|RBIF|
          T1CON reg.   --->   |T1GINV|TMR1GE|T1CKPS1|T1CKPS2|T1OSCEN|T1SYNC|TMR1CS|TMR1ON|
                                T1CKPS1|T1CKPS2 elõosztó (1/8  1/4  1/2  1/1)  TMR1CS -- clock source ( o --  belsõ, Fosc/4)
                                       TMR1ON  --- engedélyezés (1)  */

    /*  ADC_Init    */
    ANSEL=0b11100000;    // PORTA digitális I/O,  PORTE  analóg
    TRISE=0b00000111;      //  RE0, RE1, RE2  bemenet

      /*  ADCON1 reg.   --->   |ADFM|-|VCFG1|VCFG0|-|-|-|-|       ADFM bit --> 1     eredmény:  ADRESH alsó két bit + ADRESL

               VCFG0  --  Vref+    VCFG1 -- Vref-      ha 0  --->   Vcc  és  Vss ha 1 -->   AN3,  AN2  lábak (külsõ ref.)

         ADCON0 reg.   --->   |ADCS1|ADCS2|CHS3|CHS2|CHS1|CHS0|GO/DONE|ADON|

         ADCS1-ADCS2   clock    00 --> Fosc/2   01 --> Fosc/8  10 --> Fosc/32   11 --> RC

                ADON engedélyezés,   CHSx  csatorna kiválasztása       */

    ADCON1= 0b10000000;  // A/D   ADFM bit --> 1     eredmény:  ADRESH alsó két bit + ADRESL

    ADCON0= 0b11010100;  // AN5 (RE0) lesz a bemeneti csatorna,

    ADCON0=ADCON0|1;     //   A/D engedélyezés

    // 8-infra    5-NTC  13-fotoell.
    
    Delay_ms(300);

}

 /********************************************************************************************************************/


void interrupt(){
 //TMR1 interrupt

    szamlal++;
    if(szamlal==8) { szamlal=0; gmasperc++;  }
    if(ora_jar)
    {
       if(gmasperc==60) { gmasperc=0; gperc++;  }
       if(gperc==60) { gperc=0; gora++;  }
       if(gora==24) { gora=0;  }
    }
    PIR1=0;  // jelzõbit törlése
    TMR1H=0b00001011;
    TMR1L=0b11011100;      // 3036 kezdõ érték ---> 62500 lépés  --->  500kHz/62500=8Hz ---> szamlal 1s alatt 8-et lép !!
    INTCON=0b11000000;    //  interrupt engedélyezve, globális és periféria
    PIE1=0b00000001;      // TMR1 interrupt engedélyezve
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
       pont=(utem&0x04)>>2;  // fél másodpercig 1-es, fél másodpercig 0-as, (1Hz)
       villog=(utem&0x02)>>1;  // negyed másodpercig 1-es, negyed  másodpercig 0-as, (2Hz)
       
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
       if(vill==1) szegmens7=tomb7[szam1]*villog;        // ha ora_set -> villog
       else szegmens7=tomb7[szam1];                //    egyebkent nem villog
       vez_szegm=0b00000001;                //  T1  tranz. bekapcs. › 1. kijelzõ világít  ->  ora 1. szamjegy
       Delay_ms(4);
                       // while(PORTD.B0==1);
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
       if(vill==0) szegmens7=tomb7[szam2]|pont;                        // ha ora_jar -> nem villog  + pont
       else if(vill==1) szegmens7=tomb7[szam2]*villog;  // ha ora_set -> villog
       else szegmens7=tomb7[szam2];                //   egyébként (ha perc_set) nem villog
       vez_szegm=0b00000010;                //  T2  tranz. bekapcs. › 2. kijelzõ világít ->  ora 2. szamjegy
       Delay_ms(4);
                        // while(PORTD.B0==1);
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
       if(vill==2) szegmens7=tomb7[szam3]*villog;   // ha perc_set -> villog
       else szegmens7=tomb7[szam3];                //  egyebkent nem villog    
       vez_szegm=0b00000100;                //  T3  tranz. bekapcs. › 3. kijelzõ világít   -> perc 1. szamjegy
       Delay_ms(4);
                       // while(PORTD.B0==1);
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
       if(vill==2) szegmens7=tomb7[szam4]*villog;    // ha perc_set -> villog
       else szegmens7=tomb7[szam4];                //      egyebkent nem villog
       vez_szegm=0b00001000;                //  T4  tranz. bekapcs. › 4. kijelzõ világít   -> perc 2. szamjegy
       Delay_ms(4);


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

       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
       szegmens7=tomb7[szam1];                //
       vez_szegm=0b00000001;                //  T1  tranz. bekapcs. -> 1. kijelzõ világít
       Delay_ms(4);
                       // while(PORTD.B0==1);
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
       szegmens7=tomb7[szam2];                //
       vez_szegm=0b00000010;                //  T2  tranz. bekapcs. -> 2. kijelzõ világít
       Delay_ms(4);
                        // while(PORTD.B0==1);
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
       szegmens7=tomb7[szam3]|0b00000001;                //  tizedespont
       vez_szegm=0b00000100;                //  T3  tranz. bekapcs. -> 3. kijelzõ világít
       Delay_ms(4);
                       // while(PORTD.B0==1);
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
       szegmens7=tomb7[szam4];                //
       vez_szegm=0b00001000;                //  T4  tranz. bekapcs. -> 4. kijelzõ világít
       Delay_ms(4);

 }
 
 
/********************************************************************************************************************/

void ntc_lekerdez() {

    char i=0;
    char negativ=0;
    int poti=0;
    long fok;


         // 8-infra   9-poti  11-NTC (hõérzékelõ)   13-fotoell.

         ADCON0=ADCON0|2;  // GO bit -->1   A/D indítása
         while((ADCON0&2)!=0){   // várakozás A/D eredményre  amikor DONE=0 lesz
             asm nop;
         }
         poti=ADRESH&3;
         poti=(poti<<8)+ADRESL;     // eredmény:  ADRESH alsó két bit + ADRESL
                          // poti=640;
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

void main() {

    char i=0;
    char ciklus=0;

    kezdo_beall();
    Delay_ms(15);

    while(1){         //  fõ ciklus !!

      if (!homero) {        //   óra kijelzés
         if(ora_jar) idoki7szegmre(gora, gperc, gmasperc, szamlal, 0);
         if(ora_set) idoki7szegmre(gora, gperc, gmasperc, szamlal, 1);  // óra villogtatva
         if(perc_set) idoki7szegmre(gora, gperc, gmasperc, szamlal, 2);  // perc  villogtatva
      }
      if(homero) {            //  hõmérõ kijelzés
         if(ciklus%15==0) ntc_lekerdez();               // hõmérséklet lekérdezése
         hom7szegmre(gelojel, gfok);
      }
       /*  nyomógombok lekérdezése  -> üzemmódok beállítása  */
      if(kapcs_jobb==0)       //jobb ---> perc beállítása
          { perc_set=1; ora_jar=0; ora_set=0; homero=0; }
      if(kapcs_bal==0)        //bal  -->  óra beállítása
          { ora_set=1; ora_jar=0; perc_set=0; homero=0; }
      if(!ora_jar&&(kapcs_stop==0))        //stop ---> normál üzemmód,  jár az óra
          {INTCON=0b00000000; ora_jar=1; ora_set=0; perc_set=0; szamlal=0; gmasperc=0;
           TMR1H=0b00001011; TMR1L=0b11011100; INTCON=0b11000000; // TIMER1 megszakitás újra indít
          }
      if((ciklus%25==0)&&ora_set) {                               // óra érték beállítása
             if(kapcs_fel==0) {gora++; if(gora>23) gora=0;  }        // fel  ---> növelés
             if(kapcs_le==0)  {if(gora>1) gora--; else gora=23;    }    // le ---> csökkentés
       }
      if((ciklus%25==0)&&perc_set) {                               // perc érték beállítása
             if(kapcs_fel==0) {gperc++; if(gperc>59) gperc=0;  }          // fel  ---> növelés
             if(kapcs_le==0)  {if(gperc>1) gperc--; else gperc=59;    }    // le ---> csökkentés
       }
      if(ora_jar&&(kapcs_le==0))  homero=1;      //  le ---> hõmérõ kijelzés

      if(homero&&(kapcs_fel==0))  homero=0;      //  fel ---> óra kijelzés
          
      ciklus++;


    }

}