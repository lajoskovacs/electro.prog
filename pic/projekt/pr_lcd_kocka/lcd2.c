

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
//   1aaaaaaa  set DDRAM address    karakter pozíciók címei
//              elsõ sor  00  01  02  03  ...... 0F
//              második s. 40  41  42   ....  4F
//
//

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
    OSCCON = 0x77;   // belsõ oszcillátor,  8MHZ
    // OSCCON = 0x79;   // külsõ oszcillátor,  8MHZ
    TRISA = 0xFF; // PORTA-n kapcsolók
    ANSEL=0x03;    // PORTA alsó két lába analóg bemenet !!! (RA0 --> AN0   RA1 ---> AN1)
    ANSELH=0;      // PORTB digitális I/O
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
      
       //Display ON, Cursor Off
    Lcd_Cmd(_LCD_TURN_ON);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_Cmd(0b00101100);     // funstion set:  4 bites mód, 2 sor, font 5x10
      //Clear Display
    Lcd_Cmd(_LCD_CLEAR);

    Lcd_Out(1,1,"Kandó");               //  szöveg kiírása  1. sor 1. pozíciótól
    Lcd_Out_Cp(" ");
    Lcd_Out_Cp("Kálmán");

        // egyéni  jelek feltöltése        CGRAM-ba
    Lcd_Cmd(0b01000000);     // set CGRAM addres (0.)
    for(i=0;i<8;i++) Lcd_Chr_CP(kockalent[i]);         // CGRAM-0.
    for(i=0;i<8;i++) Lcd_Chr_CP(kockafent[i]);          // CGRAM-1.
    for(i=0;i<8;i++) Lcd_Chr_CP(kockaures[i]);           // CGRAM-2.
    for(i=0;i<8;i++) Lcd_Chr_CP(parh[i]);                  // CGRAM-3.

             
    while(1){

       Lcd_Chr(2,1,szam+48);        // egy karrakter kiírása  (a 0   ASCII kódja a 48 !!)
       for(i=0;i<szam;i++) Lcd_Chr_Cp(' ');               //  space
       if(szam%2)
           Lcd_Chr_Cp(1);               //  kockafent
       else
           Lcd_Chr_Cp(0);               //  kockalent
       Lcd_Chr_Cp(' ');
       szam++;
       if (szam==10) {
          szam=0;
          Lcd_Out(2,2,"          ");
       }
       Delay_ms(1000);
    }

}