#line 1 "D:/prog/avr_pic/pic16f887/lcd_ora_ok/lcd_ora.c"
#line 44 "D:/prog/avr_pic/pic16f887/lcd_ora_ok/lcd_ora.c"
char szamlal=0;
char gora=0;
char gperc=0;
char gmasperc=0;
const int foktar[35]={-65,-50,-45,-40,-35,-30,-25,-20,-15,-10,-5,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,150};
const int fesz[35]={1023,994,984,972,958,940,918,892,863,829,791,749,705,658,609,560,512,465,420,377,337,301,268,238,211,187,165,146,130,115,102,91,81,72,0};


 void send_egybajt(char rs,char adat)
 {
 char felso4=0;
 char also4=0;
 also4=adat&0b00001111;
 felso4=(adat&0b11110000)>>4;
 PORTD.B0=rs;
 PORTC=PORTC&0b11110000;
 PORTC=PORTC|(felso4&0b00001111);
 PORTD.B1=1;
 Delay_ms(1);
 PORTD.B1=0;
 Delay_ms(1);
 PORTC=PORTC&0b11110000;
 PORTC=PORTC|(also4&0b00001111);
 PORTD.B1=1;
 Delay_ms(1);
 PORTD.B1=0;
 Delay_ms(1);
 }

 void send_felbajt(char rs,char adat)
 {
 PORTC=PORTC&0b11110000;
 PORTC=PORTC|(adat&0b00001111);
 PORTD.B0=rs;
 PORTD.B1=1;
 Delay_ms(1);
 PORTD.B1=0;
 Delay_ms(1);

 }



void kezdo_beall(void)
{

 OSCCON = 0x70;
 TRISA = 0xFF;
 ANSEL=0;
 ANSELH=0b00101011;
 TRISC =0;
 TRISD=0;
 TRISB=0b00111100;
 PORTC=0;
 PORTD=0;
 PORTB=0;
 C1ON_bit = 0;
 C2ON_bit = 0;



 OPTION_REG= 0x06;
 TMR0=130;
 INTCON=0xA0;
#line 117 "D:/prog/avr_pic/pic16f887/lcd_ora_ok/lcd_ora.c"
 ADCON1= 0b10000000;
 ADCON0= 0b11101100;
 ADCON0=ADCON0|1;




 Delay_ms(300);
 send_felbajt(0,0b0010);
 Delay_ms(5);
 send_egybajt(0,0b00101100);
 send_egybajt(0,0b00001100);
 send_egybajt(0,0b00000001);
 send_egybajt(0,0b00000110);

}





void interrupt(){


 szamlal++;
 if(szamlal==250) { szamlal=0; gmasperc++; }
 if(gmasperc==60) { gmasperc=0; gperc++; }
 if(gperc==60) { gperc=0; gora++; }
 if(gora==24) { gora=0; }

 TMR0=130;
 INTCON=0xA0;
}



void irLCDre(int fok, char eloj, char ora, char perc, char masperc) {
 char fokszj0=' ';
 char fokszj1=0;
 char fokszj2=0;
 char fokszj3=0;
 char fokszj4=0;
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
 send_egybajt(0,0b10000000);
 send_egybajt(1,oraszj1+48);
 send_egybajt(1,oraszj2+48);
 send_egybajt(1,':');
 send_egybajt(1,percszj1+48);
 send_egybajt(1,percszj2+48);
 send_egybajt(1,':');
 send_egybajt(1,mpercszj1+48);
 send_egybajt(1,mpercszj2+48);

 send_egybajt(0,0b11000000);
 send_egybajt(1,fokszj0);
 send_egybajt(1,fokszj1+48);
 send_egybajt(1,fokszj2+48);
 send_egybajt(1,fokszj3+48);
 send_egybajt(1,'.');
 send_egybajt(1,fokszj4+48);

 }


void main() {

 char i=1;
 long fok;
 int fok2=0;
 char txt[10];
 char ora_jar=1;
 char ora_set=0;
 char perc_set=0;
 char eloj=' ';
 int poti=0;


 kezdo_beall();
 Delay_ms(15);

 send_egybajt(0,0b00000001);
 send_egybajt(0,0b10000000);
 send_egybajt(1,'H');
 send_egybajt(1,'e');
 send_egybajt(1,'l');
 send_egybajt(1,'o');

 while(1){




 ADCON0=ADCON0|2;
 while((ADCON0&2)!=0){
 asm nop;
 }
 poti=ADRESH&3;
 poti=(poti<<8)+ADRESL;

 if(poti<0) poti=0;
 if(poti>1023) poti=1023;


 if(PORTA.B2==0) {perc_set=1; ora_jar=0; ora_set=0; }
 if(PORTA.B1==0) {ora_set=1; ora_jar=0; perc_set=0; }
 if(PORTA.B5==0) {ora_jar=1; ora_set=0; perc_set=0; }
 if(PORTA.B0==0) { }
 if(PORTA.B3==0) { }

 i=0;
 while(poti<fesz[i]) i++;
 fok=100*foktar[i];
 if(i>1 && i<34) fok=fok-500*(poti-fesz[i])/(fesz[i-1]-fesz[i]);
 if(i==34) fok=fok-4000*(poti-fesz[i])/(fesz[i-1]-fesz[i]);
 if(i==1) fok=fok-1500*(poti-fesz[i])/(fesz[i-1]-fesz[i]);
 if(i==0) fok=-6500;

 if(fok<0) { eloj='-'; fok=-fok; }
 if(fok%10>5) fok2=fok/10+1;
 else fok2=fok/10;

 irLCDre(fok2,eloj,gora,gperc,gmasperc);

 Delay_ms(200);

 }

}
