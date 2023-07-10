/********************************************************************************************************************/
//  HD44780 LCD display parancsai:
//  00000001  clear display
//  0000001x  cursor return home
//  000001MS  entry mode set
//              M-cursor move, 1--> right, 0--> left
//              S - display shift, 1--> yes, 0--> no
//  00001DCB  display control
//               D - display, 1--> on, 0--> off
//               C - cursor, 1--> on, 0--> off
//               B - cursor blink, 1--> on, 0--> off
//  0001SDxx  cursor/display shift
//               S-shift-move, 1--> display shift, 0--> cursor move
//               D-direction, 1--> right, 0--> left
//  001BRFxx  funtion set
//               B - bit mode, 1--> 8bit, 0--> 4bit
//               R - row, 1--> 2 rows, 0--> 1 row
//               F - font, 1--> font 5x10, 0--> font 5x8
//  01aaaaaa  set CGRAM address (user character --->  5x8)
//         user character (max. 8):
//                1.row B4 B3 B2 B1 B0                     pl.  - - x - -       0b00100
//                2.row B4 B3 B2 B1 B0                          - x x x -       0b01110
//                3.row B4 B3 B2 B1 B0                          x - x - x       0b10101
//                4.row B4 B3 B2 B1 B0                          - - x - -       0b00100
//                5.row B4 B3 B2 B1 B0                          - - x - -       0b00100
//                6.row B4 B3 B2 B1 B0                          - - x - -       0b00100
//                7.row B4 B3 B2 B1 B0                          - - x - -       0b00100
//                8.row B4 B3 B2 B1 B0                          - - x - -       0b00100
//
//   1aaaaaaa  set DDRAM address    karakter poz�ci�k c�mei
//              els� sor  00  01  02  03  ...... 0F
//              m�sodik s. 40  41  42   ....  4F
//
//
/********************************************************************************************************************/

// LCD_RS   RD0_bit;        ha RS=1   ---> adat      ha RS=0  --->  parancs
//  LCD_EN  RD1_bit;
// LCD_D4  RC0_bit;
// LCD_D5  RC1_bit;
// LCD_D6  RC2_bit;
// LCD_D7  RC3_bit;

char szamlal=0;
char gora=0;
char gperc=0;
char gmasperc=0;
const int foktar[35]={-65,-50,-45,-40,-35,-30,-25,-20,-15,-10,-5,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,150};
const int fesz[35]={1023,994,984,972,958,940,918,892,863,829,791,749,705,658,609,560,512,465,420,377,337,301,268,238,211,187,165,146,130,115,102,91,81,72,0};

 /********************************************************************************************************************/
 void send_egybajt(char rs,char adat)
  {
        char felso4=0;
        char also4=0;
        also4=adat&0b00001111;              // als� 4 bit
        felso4=(adat&0b11110000)>>4;         // fels� 4 bit
        PORTD.B0=rs;        //RS l�b be�ll�t�sa (parancs vagy adat?)
        PORTC=PORTC&0b11110000;
        PORTC=PORTC|(felso4&0b00001111);      // fels� f�l-byte kik�ld�se
        PORTD.B1=1;        // �rajel (ENG) 1-be
        Delay_ms(1);
        PORTD.B1=0;        // �rajel (ENG) 0-ba
        Delay_ms(1);
        PORTC=PORTC&0b11110000;
        PORTC=PORTC|(also4&0b00001111);     // als� f�l-byte kik�ld�se
        PORTD.B1=1;        // �rajel (ENG) 1-be
        Delay_ms(1);
        PORTD.B1=0;        // �rajel (ENG) 0-ba
        Delay_ms(1);
  }
 /********************************************************************************************************************/
  void send_felbajt(char rs,char adat)
  {
        PORTC=PORTC&0b11110000;
        PORTC=PORTC|(adat&0b00001111);          //  f�l-byte kik�ld�se
        PORTD.B0=rs;        //RS l�b be�ll�t�sa (parancs vagy adat?)
        PORTD.B1=1;        // �rajel (ENG) 1-be
        Delay_ms(1);
        PORTD.B1=0;        // �rajel (ENG) 0-ba
        Delay_ms(1);

  }

/********************************************************************************************************************/

void kezdo_beall(void)
{
    //OSCCON = 0x71;   // bels� oszcill�tor,  8MHZ
     OSCCON = 0x70;   // k�ls� oszcill�tor,  16MHZ
    TRISA = 0xFF; // PORTA-n kapcsol�k, RA0-fel, RA1-bal, RA2-jobb, RA3-le, RA5-stop
    ANSEL=0;    // PORTA, PORTE  digit�lis I/O
    ANSELH=0b00101011;    // PORTB n�gy l�ba anal�g bemenet !!(RB4-->AN11 RB3-->AN9   RB2-->AN8  RB5-->AN13, a t�bbi l�b digit�lis I/O
    TRISC =0;         // LCD kijelz�           RC0  RC1  RC2  RC3
    TRISD=0;             // LCD kijelz�         RD0  RD1
    TRISB=0b00111100;
    PORTC=0;
    PORTD=0;
    PORTB=0;
    C1ON_bit = 0;               // Disable comparators
    C2ON_bit = 0;

    /* timer, interrupt    */

    OPTION_REG= 0x06;   // TIMER0  el�oszt�: 128 ---> Fosc/4 * 1/256      ---> ha Fosc=8Mhz ---> 15625Hz  (16MHz  ---> 31250)
    TMR0=130;   // timer �rajel 15625Hz, �s 255-130= 125 sz�ml�l�s ut�n megszak�t�s  --->megszak�t�s frekv. 125Hz (16Mhz  ---> 250)
    INTCON=0xA0;    // TMR0 interrupt enged�lyezve
      /*  INTCON reg.   --->   |GIE|PEIE|T0IE|INTE|RBIE|T0IF|INTF|RBIF|    */
      
    /*  ADC_Init    */
      /*  ADCON1 reg.   --->   |ADFM|-|VCFG1|VCFG0|-|-|-|-|       ADFM bit --> 1     eredm�ny:  ADRESH als� k�t bit + ADRESL
               VCFG0  --  Vref+    VCFG1 -- Vref-      ha 0  --->   Vcc  �s  Vss ha 1 -->   AN3,  AN2  l�bak (k�ls� ref.)
         ADCON0 reg.   --->   |ADCS1|ADCS2|CHS3|CHS2|CHS1|CHS0|GO/DONE|ADON|
         ADCS1-ADCS2   clock    00 --> Fosc/2   01 --> Fosc/8  10 --> Fosc/32   11 --> RC
                ADON enged�lyez�s,   CHSx  csatorna kiv�laszt�sa       */

    ADCON1= 0b10000000;  // A/D   ADFM bit --> 1     eredm�ny:  ADRESH als� k�t bit + ADRESL
    ADCON0= 0b11101100;  // AN11 lesz a bemeneti csatorna,
    ADCON0=ADCON0|1;     //   A/D enged�lyez�s
    // 8-infra   9-poti  11-NTC  13-fotoell.


    /*    Lcd_Init   */
    Delay_ms(300);
    send_felbajt(0,0b0010);   //Set to 4 bit operation
    Delay_ms(5);
    send_egybajt(0,0b00101100);   //Function set, 4 bit mode,  2 rows, font 5x10
    send_egybajt(0,0b00001100);   //Display control, Display ON, Cursor Off
    send_egybajt(0,0b00000001);   //Clear Display
    send_egybajt(0,0b00000110);  //Entry Mode, Increment cursor position, No display shift

}

 /********************************************************************************************************************/



void interrupt(){
 //TMR0 interrupt

    szamlal++;
    if(szamlal==250) { szamlal=0; gmasperc++;  }
    if(gmasperc==60) { gmasperc=0; gperc++;  }
    if(gperc==60) { gperc=0; gora++;  }
    if(gora==24) { gora=0;  }

    TMR0=130;
    INTCON=0xA0;    // TMR0 interrupt enged�lyezve
}

/********************************************************************************************************************/

void irLCDre(int fok, char eloj, char ora, char perc, char masperc) {
       char fokszj0=' ';        // el�jel
       char fokszj1=0;           // els� sz�mjegy
       char fokszj2=0;            // 2. sz�mjegy
       char fokszj3=0;            // 3. sz�mjegy
       char fokszj4=0;         // tizedesjegy
       char oraszj1=0;
       char oraszj2=0;
       char percszj1=0;
       char percszj2=0;
       char mpercszj1=0;
       char mpercszj2=0;
       
       fokszj0=eloj;
       fokszj4=fok%10;
       fok=fok/10;
       fokszj3=fok%10;
       fok=fok/10;
       fokszj2=fok%10;
       fokszj1=fok/10;

       oraszj1=ora/10;
       oraszj2=ora-10*oraszj1;
       percszj1=perc/10;
       percszj2=perc-10*percszj1;
       mpercszj1=masperc/10;
       mpercszj2=masperc-10*mpercszj1;
       send_egybajt(0,0b10000000);   //1. sor 1. poz�ci�
       send_egybajt(1,oraszj1+48);        // egy karakter ki�r�sa  (a 0   ASCII k�dja a 48 !!)
       send_egybajt(1,oraszj2+48);
       send_egybajt(1,':');
       send_egybajt(1,percszj1+48);
       send_egybajt(1,percszj2+48);
       send_egybajt(1,':');
       send_egybajt(1,mpercszj1+48);
       send_egybajt(1,mpercszj2+48);
       //send_egybajt(0,0b00000001);   //Clear Display
       send_egybajt(0,0b11000000);   //2. sor 1. poz�ci�
       send_egybajt(1,fokszj0);   //sz�veg ki�r�sa
       send_egybajt(1,fokszj1+48);        // egy karakter ki�r�sa  (a 0   ASCII k�dja a 48 !!)
       send_egybajt(1,fokszj2+48);
       send_egybajt(1,fokszj3+48);
       send_egybajt(1,'.');
       send_egybajt(1,fokszj4+48);

 }
/********************************************************************************************************************/

void main() {

    char i=1;
    long fok;
    int fok2=0;
    char txt[10];
    char ora_jar=1;
    char ora_set=0;
    char perc_set=0;
    char eloj=' ';        // el�jel
    int poti=0;

    
    kezdo_beall();
    Delay_ms(15);

    send_egybajt(0,0b00000001);   //Clear Display
    send_egybajt(0,0b10000000);   //1. sor 1. poz�ci�
    send_egybajt(1,'H');   //sz�veg ki�r�sa
    send_egybajt(1,'e');
    send_egybajt(1,'l');
    send_egybajt(1,'o');

    while(1){
    
        /*  A/D  lek�rdez�se  --> h�m�rs�klet  */
         // 8-infra   9-poti  11-NTC (h��rz�kel�s)   13-fotoell.

       ADCON0=ADCON0|2;  // GO bit -->1   A/D ind�t�sa
       while((ADCON0&2)!=0){   // v�rakoz�s A/D eredm�nyre  amikor DONE=0 lesz
                  asm nop;
            }
       poti=ADRESH&3;
       poti=(poti<<8)+ADRESL;     // eredm�ny:  ADRESH als� k�t bit + ADRESL
      // poti=640;
       if(poti<0) poti=0;
       if(poti>1023) poti=1023;
       
       /*  nyom�gombok lek�rdez�se  */
       if(PORTA.B2==0)  {perc_set=1; ora_jar=0; ora_set=0; }         //jobb ---> perc be�ll�t�sa
       if(PORTA.B1==0)  {ora_set=1; ora_jar=0; perc_set=0; }         //bal  -->  �ra be�ll�t�sa
       if(PORTA.B5==0)  {ora_jar=1; ora_set=0; perc_set=0; }         //stop ---> norm�l �zemm�d,  j�r az �ra
       if(PORTA.B0==0) {   }                    // fel  ---> n�vel�s
       if(PORTA.B3==0)  {    }                  // le ---> cs�kkent�s
       
       i=0;
       while(poti<fesz[i]) i++;
       fok=100*foktar[i];               //  -65  -----    +150
       if(i>1 && i<34) fok=fok-500*(poti-fesz[i])/(fesz[i-1]-fesz[i]);
       if(i==34) fok=fok-4000*(poti-fesz[i])/(fesz[i-1]-fesz[i]);
       if(i==1) fok=fok-1500*(poti-fesz[i])/(fesz[i-1]-fesz[i]);
       if(i==0) fok=-6500;

       if(fok<0) { eloj='-'; fok=-fok;  }     // el�jel be�ll�t�sa
       if(fok%10>5)   fok2=fok/10+1;
       else   fok2=fok/10;     // kerek�t�s egy tizedes jegyre

       irLCDre(fok2,eloj,gora,gperc,gmasperc);
       
       Delay_ms(200);

    }

}