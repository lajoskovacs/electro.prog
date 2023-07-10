#line 1 "D:/prog/avr_pic/pic16f887/lcd2/lcd2.c"
#line 37 "D:/prog/avr_pic/pic16f887/lcd2/lcd2.c"
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;



void io_init (void)
{
 OSCCON = 0x77;

 TRISA = 0xFF;
 ANSEL=0x03;
 ANSELH=0;
 TRISB =0;
 Lcd_Init();
}


void main() {

 char szam;
 char i,j;
 char kockalent[]={0,0,0,0,14,14,14,0};
 char kockafent[]={0,14,14,14,0,0,0,0};
 char kockaures[]={0,0,4,10,17,10,4,0};
 char parh[]={0,10,10,10,10,10,10,0};


 szam=0;
 io_init();
 Delay_ms(15);


 Lcd_Cmd(_LCD_TURN_ON);
 Lcd_Cmd(_LCD_CURSOR_OFF);
 Lcd_Cmd(0b00101100);

 Lcd_Cmd(_LCD_CLEAR);

 Lcd_Out(1,1,"Kandó");
 Lcd_Out_Cp(" ");
 Lcd_Out_Cp("Kálmán");


 Lcd_Cmd(0b01000000);
 for(i=0;i<8;i++) Lcd_Chr_CP(kockalent[i]);
 for(i=0;i<8;i++) Lcd_Chr_CP(kockafent[i]);
 for(i=0;i<8;i++) Lcd_Chr_CP(kockaures[i]);
 for(i=0;i<8;i++) Lcd_Chr_CP(parh[i]);


 while(1){

 Lcd_Chr(2,1,szam+48);
 for(i=0;i<szam;i++) Lcd_Chr_Cp(' ');
 if(szam%2)
 Lcd_Chr_Cp(1);
 else
 Lcd_Chr_Cp(0);
 Lcd_Chr_Cp(' ');
 szam++;
 if (szam==10) {
 szam=0;
 Lcd_Out(2,2,"          ");
 }
 Delay_ms(1000);
 }

}
