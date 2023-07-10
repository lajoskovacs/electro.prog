#line 1 "Z:/home/lala/Dokumentumok/prog/avr_pic/pic16f887/szeg7ora4hom/szeg7ora3.c"
#line 7 "Z:/home/lala/Dokumentumok/prog/avr_pic/pic16f887/szeg7ora4hom/szeg7ora3.c"
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



char tomb7[ ]={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,0,0b00000010,0b01101110,0b00011100,0};

const int foktar[35]={-65,-50,-45,-40,-35,-30,-25,-20,-15,-10,-5,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,150};
const int fesz[35]={0,30,40,52,66,84,106,132,161,195,233,275,320,366,415,464,512,559,604,647,687,723,757,786,813,837,859,878,894,909,922,933,943,952,1023};

sbit kapcs_fel at PORTA.B0;
sbit kapcs_bal at PORTA.B1;
sbit kapcs_jobb at PORTA.B2;
sbit kapcs_le at PORTA.B3;
sbit kapcs_stop at PORTA.B5;
char szegmens7 at PORTD;
char vez_szegm at PORTB;



void kezdo_beall(void)
{
#line 50 "Z:/home/lala/Dokumentumok/prog/avr_pic/pic16f887/szeg7ora4hom/szeg7ora3.c"
 OSCCON = 0x70;
 TRISA = 0xFF;
 ANSEL=0;
 ANSELH=0;
 TRISB=0;
 TRISD=0;
 PORTD=0;
 PORTB=0;





 T1CON= 0b00110101;
 PIR1=0;
 TMR1H=0b00001011;
 TMR1L=0b11011100;
 INTCON=0b11000000;
 PIE1=0b00000001;
#line 75 "Z:/home/lala/Dokumentumok/prog/avr_pic/pic16f887/szeg7ora4hom/szeg7ora3.c"
 ANSEL=0b11100000;
 TRISE=0b00000111;
#line 88 "Z:/home/lala/Dokumentumok/prog/avr_pic/pic16f887/szeg7ora4hom/szeg7ora3.c"
 ADCON1= 0b10000000;

 ADCON0= 0b11010100;

 ADCON0=ADCON0|1;



 Delay_ms(300);

}




void interrupt(){


 szamlal++;
 if(szamlal==8) { szamlal=0; gmasperc++; }
 if(ora_jar)
 {
 if(gmasperc==60) { gmasperc=0; gperc++; }
 if(gperc==60) { gperc=0; gora++; }
 if(gora==24) { gora=0; }
 }
 PIR1=0;
 TMR1H=0b00001011;
 TMR1L=0b11011100;
 INTCON=0b11000000;
 PIE1=0b00000001;
}




void idoki7szegmre(char ora, char perc, char mperc, char utem, char vill) {

 char szam1,szam2,szam3,szam4;
 char villog,pont;

 szam1=ora/10;
 szam2=ora%10;
 szam3=perc/10;
 szam4=perc%10;
 pont=(utem&0x04)>>2;
 villog=(utem&0x02)>>1;

 vez_szegm=0b00000000;
 if(vill==1) szegmens7=tomb7[szam1]*villog;
 else szegmens7=tomb7[szam1];
 vez_szegm=0b00000001;
 Delay_ms(4);

 vez_szegm=0b00000000;
 if(vill==0) szegmens7=tomb7[szam2]|pont;
 else if(vill==1) szegmens7=tomb7[szam2]*villog;
 else szegmens7=tomb7[szam2];
 vez_szegm=0b00000010;
 Delay_ms(4);

 vez_szegm=0b00000000;
 if(vill==2) szegmens7=tomb7[szam3]*villog;
 else szegmens7=tomb7[szam3];
 vez_szegm=0b00000100;
 Delay_ms(4);

 vez_szegm=0b00000000;
 if(vill==2) szegmens7=tomb7[szam4]*villog;
 else szegmens7=tomb7[szam4];
 vez_szegm=0b00001000;
 Delay_ms(4);


 }





void hom7szegmre(char elojel,int fok) {



 char szam1=0;
 char szam2=0;
 char szam3=0;
 char szam4=0;
 char i;

 if(!elojel) {
 szam4=fok%10;
 fok=fok/10;
 szam3=fok%10;
 fok=fok/10;
 szam2=fok%10;
 szam1=fok/10;
 if(szam1==0) { szam1=10; if(szam2==0) szam2=10; }
 }

 if(elojel) {
 szam4=fok%10;
 fok=fok/10;
 szam3=fok%10;
 szam2=fok/10;
 szam1=11;

 }

 vez_szegm=0b00000000;
 szegmens7=tomb7[szam1];
 vez_szegm=0b00000001;
 Delay_ms(4);

 vez_szegm=0b00000000;
 szegmens7=tomb7[szam2];
 vez_szegm=0b00000010;
 Delay_ms(4);

 vez_szegm=0b00000000;
 szegmens7=tomb7[szam3]|0b00000001;
 vez_szegm=0b00000100;
 Delay_ms(4);

 vez_szegm=0b00000000;
 szegmens7=tomb7[szam4];
 vez_szegm=0b00001000;
 Delay_ms(4);

 }




void ntc_lekerdez() {

 char i=0;
 char negativ=0;
 int poti=0;
 long fok;




 ADCON0=ADCON0|2;
 while((ADCON0&2)!=0){
 asm nop;
 }
 poti=ADRESH&3;
 poti=(poti<<8)+ADRESL;

 if(poti<0) poti=0;
 if(poti>1023) poti=1023;
 i=0;
 while(poti>fesz[i]) i++;
 fok=100*foktar[i];
 if(i>1 && i<34) fok=fok-500*(fesz[i]-poti)/(fesz[i]-fesz[i-1]);
 if(i==34) fok=fok-4000*(fesz[i]-poti)/(fesz[i]-fesz[i-1]);
 if(i==1) fok=fok-1500*(fesz[i]-poti)/(fesz[i]-fesz[i-1]);
 if(i==0) fok=-6500;
 if(fok<0) { negativ=1; fok=-fok; }
 if(fok%10>5) gfok=fok/10+1;
 else gfok=fok/10;
 gelojel=negativ;

}



void main() {

 char i=0;
 char ciklus=0;

 kezdo_beall();
 Delay_ms(15);

 while(1){

 if (!homero) {
 if(ora_jar) idoki7szegmre(gora, gperc, gmasperc, szamlal, 0);
 if(ora_set) idoki7szegmre(gora, gperc, gmasperc, szamlal, 1);
 if(perc_set) idoki7szegmre(gora, gperc, gmasperc, szamlal, 2);
 }
 if(homero) {
 if(ciklus%15==0) ntc_lekerdez();
 hom7szegmre(gelojel, gfok);
 }

 if(kapcs_jobb==0)
 { perc_set=1; ora_jar=0; ora_set=0; homero=0; }
 if(kapcs_bal==0)
 { ora_set=1; ora_jar=0; perc_set=0; homero=0; }
 if(!ora_jar&&(kapcs_stop==0))
 {INTCON=0b00000000; ora_jar=1; ora_set=0; perc_set=0; szamlal=0; gmasperc=0;
 TMR1H=0b00001011; TMR1L=0b11011100; INTCON=0b11000000;
 }
 if((ciklus%25==0)&&ora_set) {
 if(kapcs_fel==0) {gora++; if(gora>23) gora=0; }
 if(kapcs_le==0) {if(gora>1) gora--; else gora=23; }
 }
 if((ciklus%25==0)&&perc_set) {
 if(kapcs_fel==0) {gperc++; if(gperc>59) gperc=0; }
 if(kapcs_le==0) {if(gperc>1) gperc--; else gperc=59; }
 }
 if(ora_jar&&(kapcs_le==0)) homero=1;

 if(homero&&(kapcs_fel==0)) homero=0;

 ciklus++;


 }

}
