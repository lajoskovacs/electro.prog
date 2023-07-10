 /*
     �ra �s h�m�r�
     PIC16F887 mikrovez�rl�re
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
                                                    // 0,1,2,3,4,5,6,7,8,9,�res,-,H,L       szegm.  h g f e d c b a
char tomb7[ ]={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,0,0b00000010,0b01101110,0b00011100,0};
                                                    // 0,1,2,3,4,5,6,7,8,9,�res,-,H,L        szegm.  a b c d e f g h
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
                           default: -1101000           (r) --> csak olvashat�
                           SCS  1 --->  bels� oszc.     0 ---> k�ls�
                           OSTS-1 --->  k�ls� oszc. fut,  0 ---> bels�
       OSCCON = %01100001 for 4mhz, or OSCCON = %01110001for 8mhz
        OSCCON = %01110001           'Int CLK 8MHz
        ANSEL= %11111111       '$0F = disable A/D converter
        option_reg = 7     'switch pull-ups ON
      END of oscillator selections  */

    //OSCCON = 0x71;   // bels� oszcill�tor,  8MHZ
    OSCCON = 0x70;   // k�ls� oszcill�tor,  16MHZ
    TRISA = 0xFF; // PORTA-n kapcsol�k, RA0-fel, RA1-bal, RA2-jobb, RA3-le, RA5-stop
    ANSEL=0;    // PORTA digit�lis I/O
    ANSELH=0;    // PORTB  digit�lis I/O
    TRISB=0;         // 7 szegmenses kijelz�  vez�rl�se        1 szj ->  RB0   2.szj. ->  RB1   3.szj. ->  RB2      4.szj. -> RB3
    TRISD=0;             // 7 szegmenses kijelz�,     a-h szegmensei ->       RD0 -- RD7
    PORTD=0;
    PORTB=0;
   // C1ON_bit = 0;               // Disable comparators
    // C2ON_bit = 0;

    /* timer, interrupt    */

     T1CON= 0b00110101;   // TIMER1  el�oszt�: 8 ---> Fosc/4 * 1/8      ---> ha Fosc=16Mhz ---> Ft1=500kHz
    PIR1=0;  // jelz�bit t�rl�se
    TMR1H=0b00001011;
    TMR1L=0b11011100;      // 3036 kezd� �rt�k ---> 62500 l�p�s  --->  500kHz/62500=8Hz ---> szamlal 1s alatt 8-et l�p !!
    INTCON=0b11000000;    //  interrupt enged�lyezve, glob�lis �s perif�ria
    PIE1=0b00000001;      // TMR1 interrupt enged�lyezve
      /*  INTCON reg.   --->   |GIE|PEIE|T0IE|INTE|RBIE|T0IF|INTF|RBIF|
          T1CON reg.   --->   |T1GINV|TMR1GE|T1CKPS1|T1CKPS2|T1OSCEN|T1SYNC|TMR1CS|TMR1ON|
                                T1CKPS1|T1CKPS2 el�oszt� (1/8  1/4  1/2  1/1)  TMR1CS -- clock source ( o --  bels�, Fosc/4)
                                       TMR1ON  --- enged�lyez�s (1)  */

    /*  ADC_Init    */
    ANSEL=0b11100000;    // PORTA digit�lis I/O,  PORTE  anal�g
    TRISE=0b00000111;      //  RE0, RE1, RE2  bemenet

      /*  ADCON1 reg.   --->   |ADFM|-|VCFG1|VCFG0|-|-|-|-|       ADFM bit --> 1     eredm�ny:  ADRESH als� k�t bit + ADRESL

               VCFG0  --  Vref+    VCFG1 -- Vref-      ha 0  --->   Vcc  �s  Vss ha 1 -->   AN3,  AN2  l�bak (k�ls� ref.)

         ADCON0 reg.   --->   |ADCS1|ADCS2|CHS3|CHS2|CHS1|CHS0|GO/DONE|ADON|

         ADCS1-ADCS2   clock    00 --> Fosc/2   01 --> Fosc/8  10 --> Fosc/32   11 --> RC

                ADON enged�lyez�s,   CHSx  csatorna kiv�laszt�sa       */

    ADCON1= 0b10000000;  // A/D   ADFM bit --> 1     eredm�ny:  ADRESH als� k�t bit + ADRESL

    ADCON0= 0b11010100;  // AN5 (RE0) lesz a bemeneti csatorna,

    ADCON0=ADCON0|1;     //   A/D enged�lyez�s

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
    PIR1=0;  // jelz�bit t�rl�se
    TMR1H=0b00001011;
    TMR1L=0b11011100;      // 3036 kezd� �rt�k ---> 62500 l�p�s  --->  500kHz/62500=8Hz ---> szamlal 1s alatt 8-et l�p !!
    INTCON=0b11000000;    //  interrupt enged�lyezve, glob�lis �s perif�ria
    PIE1=0b00000001;      // TMR1 interrupt enged�lyezve
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
       pont=(utem&0x04)>>2;  // f�l m�sodpercig 1-es, f�l m�sodpercig 0-as, (1Hz)
       villog=(utem&0x02)>>1;  // negyed m�sodpercig 1-es, negyed  m�sodpercig 0-as, (2Hz)
       
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsol�sa
       if(vill==1) szegmens7=tomb7[szam1]*villog;        // ha ora_set -> villog
       else szegmens7=tomb7[szam1];                //    egyebkent nem villog
       vez_szegm=0b00000001;                //  T1  tranz. bekapcs. � 1. kijelz� vil�g�t  ->  ora 1. szamjegy
       Delay_ms(4);
                       // while(PORTD.B0==1);
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsol�sa
       if(vill==0) szegmens7=tomb7[szam2]|pont;                        // ha ora_jar -> nem villog  + pont
       else if(vill==1) szegmens7=tomb7[szam2]*villog;  // ha ora_set -> villog
       else szegmens7=tomb7[szam2];                //   egy�bk�nt (ha perc_set) nem villog
       vez_szegm=0b00000010;                //  T2  tranz. bekapcs. � 2. kijelz� vil�g�t ->  ora 2. szamjegy
       Delay_ms(4);
                        // while(PORTD.B0==1);
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsol�sa
       if(vill==2) szegmens7=tomb7[szam3]*villog;   // ha perc_set -> villog
       else szegmens7=tomb7[szam3];                //  egyebkent nem villog    
       vez_szegm=0b00000100;                //  T3  tranz. bekapcs. � 3. kijelz� vil�g�t   -> perc 1. szamjegy
       Delay_ms(4);
                       // while(PORTD.B0==1);
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsol�sa
       if(vill==2) szegmens7=tomb7[szam4]*villog;    // ha perc_set -> villog
       else szegmens7=tomb7[szam4];                //      egyebkent nem villog
       vez_szegm=0b00001000;                //  T4  tranz. bekapcs. � 4. kijelz� vil�g�t   -> perc 2. szamjegy
       Delay_ms(4);


 }
 
/********************************************************************************************************************/



void hom7szegmre(char elojel,int fok) {

        // elojel   0 - pozit�v   1- negat�v  !!

      char szam1=0;                      // els� sz�mjegy
      char szam2=0;                   // 2. sz�mjegy
      char szam3=0;                 // 3. sz�mjegy
      char szam4=0;             // tizedesjegy
      char i;

      if(!elojel) {         // pozit�v h�m�rs�klet
            szam4=fok%10;          // tizedesjegy
            fok=fok/10;
            szam3=fok%10;
            fok=fok/10;
            szam2=fok%10;
            szam1=fok/10;
            if(szam1==0)  { szam1=10; if(szam2==0) szam2=10; } // �res !!
       }

       if(elojel) {
            szam4=fok%10;
            fok=fok/10;
            szam3=fok%10;
            szam2=fok/10;
            szam1=11;     //  '-'

       }

       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsol�sa
       szegmens7=tomb7[szam1];                //
       vez_szegm=0b00000001;                //  T1  tranz. bekapcs. -> 1. kijelz� vil�g�t
       Delay_ms(4);
                       // while(PORTD.B0==1);
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsol�sa
       szegmens7=tomb7[szam2];                //
       vez_szegm=0b00000010;                //  T2  tranz. bekapcs. -> 2. kijelz� vil�g�t
       Delay_ms(4);
                        // while(PORTD.B0==1);
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsol�sa
       szegmens7=tomb7[szam3]|0b00000001;                //  tizedespont
       vez_szegm=0b00000100;                //  T3  tranz. bekapcs. -> 3. kijelz� vil�g�t
       Delay_ms(4);
                       // while(PORTD.B0==1);
       vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsol�sa
       szegmens7=tomb7[szam4];                //
       vez_szegm=0b00001000;                //  T4  tranz. bekapcs. -> 4. kijelz� vil�g�t
       Delay_ms(4);

 }
 
 
/********************************************************************************************************************/

void ntc_lekerdez() {

    char i=0;
    char negativ=0;
    int poti=0;
    long fok;


         // 8-infra   9-poti  11-NTC (h��rz�kel�)   13-fotoell.

         ADCON0=ADCON0|2;  // GO bit -->1   A/D ind�t�sa
         while((ADCON0&2)!=0){   // v�rakoz�s A/D eredm�nyre  amikor DONE=0 lesz
             asm nop;
         }
         poti=ADRESH&3;
         poti=(poti<<8)+ADRESL;     // eredm�ny:  ADRESH als� k�t bit + ADRESL
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
         if(fok<0) { negativ=1; fok=-fok;  }     // el�jel be�ll�t�sa
         if(fok%10>5)   gfok=fok/10+1;
         else   gfok=fok/10;     // kerek�t�s egy tizedes jegyre
         gelojel=negativ;

}
 
/********************************************************************************************************************/

void main() {

    char i=0;
    char ciklus=0;

    kezdo_beall();
    Delay_ms(15);

    while(1){         //  f� ciklus !!

      if (!homero) {        //   �ra kijelz�s
         if(ora_jar) idoki7szegmre(gora, gperc, gmasperc, szamlal, 0);
         if(ora_set) idoki7szegmre(gora, gperc, gmasperc, szamlal, 1);  // �ra villogtatva
         if(perc_set) idoki7szegmre(gora, gperc, gmasperc, szamlal, 2);  // perc  villogtatva
      }
      if(homero) {            //  h�m�r� kijelz�s
         if(ciklus%15==0) ntc_lekerdez();               // h�m�rs�klet lek�rdez�se
         hom7szegmre(gelojel, gfok);
      }
       /*  nyom�gombok lek�rdez�se  -> �zemm�dok be�ll�t�sa  */
      if(kapcs_jobb==0)       //jobb ---> perc be�ll�t�sa
          { perc_set=1; ora_jar=0; ora_set=0; homero=0; }
      if(kapcs_bal==0)        //bal  -->  �ra be�ll�t�sa
          { ora_set=1; ora_jar=0; perc_set=0; homero=0; }
      if(!ora_jar&&(kapcs_stop==0))        //stop ---> norm�l �zemm�d,  j�r az �ra
          {INTCON=0b00000000; ora_jar=1; ora_set=0; perc_set=0; szamlal=0; gmasperc=0;
           TMR1H=0b00001011; TMR1L=0b11011100; INTCON=0b11000000; // TIMER1 megszakit�s �jra ind�t
          }
      if((ciklus%25==0)&&ora_set) {                               // �ra �rt�k be�ll�t�sa
             if(kapcs_fel==0) {gora++; if(gora>23) gora=0;  }        // fel  ---> n�vel�s
             if(kapcs_le==0)  {if(gora>1) gora--; else gora=23;    }    // le ---> cs�kkent�s
       }
      if((ciklus%25==0)&&perc_set) {                               // perc �rt�k be�ll�t�sa
             if(kapcs_fel==0) {gperc++; if(gperc>59) gperc=0;  }          // fel  ---> n�vel�s
             if(kapcs_le==0)  {if(gperc>1) gperc--; else gperc=59;    }    // le ---> cs�kkent�s
       }
      if(ora_jar&&(kapcs_le==0))  homero=1;      //  le ---> h�m�r� kijelz�s

      if(homero&&(kapcs_fel==0))  homero=0;      //  fel ---> �ra kijelz�s
          
      ciklus++;


    }

}