
_io_init:

;lcd2.c,53 :: 		void io_init (void)
;lcd2.c,55 :: 		OSCCON = 0x77;   // belsõ oszcillátor,  8MHZ
	MOVLW      119
	MOVWF      OSCCON+0
;lcd2.c,57 :: 		TRISA = 0xFF; // PORTA-n kapcsolók
	MOVLW      255
	MOVWF      TRISA+0
;lcd2.c,58 :: 		ANSEL=0x03;    // PORTA alsó két lába analóg bemenet !!! (RA0 --> AN0   RA1 ---> AN1)
	MOVLW      3
	MOVWF      ANSEL+0
;lcd2.c,59 :: 		ANSELH=0;      // PORTB digitális I/O
	CLRF       ANSELH+0
;lcd2.c,60 :: 		TRISB =0;
	CLRF       TRISB+0
;lcd2.c,61 :: 		Lcd_Init();
	CALL       _Lcd_Init+0
;lcd2.c,62 :: 		}
L_end_io_init:
	RETURN
; end of _io_init

_main:

;lcd2.c,65 :: 		void main() {
;lcd2.c,69 :: 		char kockalent[]={0,0,0,0,14,14,14,0};
	CLRF       main_kockalent_L0+0
	CLRF       main_kockalent_L0+1
	CLRF       main_kockalent_L0+2
	CLRF       main_kockalent_L0+3
	MOVLW      14
	MOVWF      main_kockalent_L0+4
	MOVLW      14
	MOVWF      main_kockalent_L0+5
	MOVLW      14
	MOVWF      main_kockalent_L0+6
	CLRF       main_kockalent_L0+7
	CLRF       main_kockafent_L0+0
	MOVLW      14
	MOVWF      main_kockafent_L0+1
	MOVLW      14
	MOVWF      main_kockafent_L0+2
	MOVLW      14
	MOVWF      main_kockafent_L0+3
	CLRF       main_kockafent_L0+4
	CLRF       main_kockafent_L0+5
	CLRF       main_kockafent_L0+6
	CLRF       main_kockafent_L0+7
	CLRF       main_kockaures_L0+0
	CLRF       main_kockaures_L0+1
	MOVLW      4
	MOVWF      main_kockaures_L0+2
	MOVLW      10
	MOVWF      main_kockaures_L0+3
	MOVLW      17
	MOVWF      main_kockaures_L0+4
	MOVLW      10
	MOVWF      main_kockaures_L0+5
	MOVLW      4
	MOVWF      main_kockaures_L0+6
	CLRF       main_kockaures_L0+7
	CLRF       main_parh_L0+0
	MOVLW      10
	MOVWF      main_parh_L0+1
	MOVLW      10
	MOVWF      main_parh_L0+2
	MOVLW      10
	MOVWF      main_parh_L0+3
	MOVLW      10
	MOVWF      main_parh_L0+4
	MOVLW      10
	MOVWF      main_parh_L0+5
	MOVLW      10
	MOVWF      main_parh_L0+6
	CLRF       main_parh_L0+7
;lcd2.c,75 :: 		szam=0;
	CLRF       main_szam_L0+0
;lcd2.c,76 :: 		io_init();
	CALL       _io_init+0
;lcd2.c,77 :: 		Delay_ms(15);
	MOVLW      39
	MOVWF      R12+0
	MOVLW      245
	MOVWF      R13+0
L_main0:
	DECFSZ     R13+0, 1
	GOTO       L_main0
	DECFSZ     R12+0, 1
	GOTO       L_main0
;lcd2.c,80 :: 		Lcd_Cmd(_LCD_TURN_ON);
	MOVLW      12
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;lcd2.c,81 :: 		Lcd_Cmd(_LCD_CURSOR_OFF);
	MOVLW      12
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;lcd2.c,82 :: 		Lcd_Cmd(0b00101100);     // funstion set:  4 bites mód, 2 sor, font 5x10
	MOVLW      44
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;lcd2.c,84 :: 		Lcd_Cmd(_LCD_CLEAR);
	MOVLW      1
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;lcd2.c,86 :: 		Lcd_Out(1,1,"Kandó");               //  szöveg kiírása  1. sor 1. pozíciótól
	MOVLW      1
	MOVWF      FARG_Lcd_Out_row+0
	MOVLW      1
	MOVWF      FARG_Lcd_Out_column+0
	MOVLW      ?lstr1_lcd2+0
	MOVWF      FARG_Lcd_Out_text+0
	CALL       _Lcd_Out+0
;lcd2.c,87 :: 		Lcd_Out_Cp(" ");
	MOVLW      ?lstr2_lcd2+0
	MOVWF      FARG_Lcd_Out_CP_text+0
	CALL       _Lcd_Out_CP+0
;lcd2.c,88 :: 		Lcd_Out_Cp("Kálmán");
	MOVLW      ?lstr3_lcd2+0
	MOVWF      FARG_Lcd_Out_CP_text+0
	CALL       _Lcd_Out_CP+0
;lcd2.c,91 :: 		Lcd_Cmd(0b01000000);     // set CGRAM addres (0.)
	MOVLW      64
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;lcd2.c,92 :: 		for(i=0;i<8;i++) Lcd_Chr_CP(kockalent[i]);         // CGRAM-0.
	CLRF       main_i_L0+0
L_main1:
	MOVLW      8
	SUBWF      main_i_L0+0, 0
	BTFSC      STATUS+0, 0
	GOTO       L_main2
	MOVF       main_i_L0+0, 0
	ADDLW      main_kockalent_L0+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      FARG_Lcd_Chr_CP_out_char+0
	CALL       _Lcd_Chr_CP+0
	INCF       main_i_L0+0, 1
	GOTO       L_main1
L_main2:
;lcd2.c,93 :: 		for(i=0;i<8;i++) Lcd_Chr_CP(kockafent[i]);          // CGRAM-1.
	CLRF       main_i_L0+0
L_main4:
	MOVLW      8
	SUBWF      main_i_L0+0, 0
	BTFSC      STATUS+0, 0
	GOTO       L_main5
	MOVF       main_i_L0+0, 0
	ADDLW      main_kockafent_L0+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      FARG_Lcd_Chr_CP_out_char+0
	CALL       _Lcd_Chr_CP+0
	INCF       main_i_L0+0, 1
	GOTO       L_main4
L_main5:
;lcd2.c,94 :: 		for(i=0;i<8;i++) Lcd_Chr_CP(kockaures[i]);           // CGRAM-2.
	CLRF       main_i_L0+0
L_main7:
	MOVLW      8
	SUBWF      main_i_L0+0, 0
	BTFSC      STATUS+0, 0
	GOTO       L_main8
	MOVF       main_i_L0+0, 0
	ADDLW      main_kockaures_L0+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      FARG_Lcd_Chr_CP_out_char+0
	CALL       _Lcd_Chr_CP+0
	INCF       main_i_L0+0, 1
	GOTO       L_main7
L_main8:
;lcd2.c,95 :: 		for(i=0;i<8;i++) Lcd_Chr_CP(parh[i]);                  // CGRAM-3.
	CLRF       main_i_L0+0
L_main10:
	MOVLW      8
	SUBWF      main_i_L0+0, 0
	BTFSC      STATUS+0, 0
	GOTO       L_main11
	MOVF       main_i_L0+0, 0
	ADDLW      main_parh_L0+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      FARG_Lcd_Chr_CP_out_char+0
	CALL       _Lcd_Chr_CP+0
	INCF       main_i_L0+0, 1
	GOTO       L_main10
L_main11:
;lcd2.c,98 :: 		while(1){
L_main13:
;lcd2.c,100 :: 		Lcd_Chr(2,1,szam+48);        // egy karrakter kiírása  (a 0   ASCII kódja a 48 !!)
	MOVLW      2
	MOVWF      FARG_Lcd_Chr_row+0
	MOVLW      1
	MOVWF      FARG_Lcd_Chr_column+0
	MOVLW      48
	ADDWF      main_szam_L0+0, 0
	MOVWF      FARG_Lcd_Chr_out_char+0
	CALL       _Lcd_Chr+0
;lcd2.c,101 :: 		for(i=0;i<szam;i++) Lcd_Chr_Cp(' ');               //  space
	CLRF       main_i_L0+0
L_main15:
	MOVF       main_szam_L0+0, 0
	SUBWF      main_i_L0+0, 0
	BTFSC      STATUS+0, 0
	GOTO       L_main16
	MOVLW      32
	MOVWF      FARG_Lcd_Chr_CP_out_char+0
	CALL       _Lcd_Chr_CP+0
	INCF       main_i_L0+0, 1
	GOTO       L_main15
L_main16:
;lcd2.c,102 :: 		if(szam%2)
	BTFSS      main_szam_L0+0, 0
	GOTO       L_main18
;lcd2.c,103 :: 		Lcd_Chr_Cp(1);               //  kockafent
	MOVLW      1
	MOVWF      FARG_Lcd_Chr_CP_out_char+0
	CALL       _Lcd_Chr_CP+0
	GOTO       L_main19
L_main18:
;lcd2.c,105 :: 		Lcd_Chr_Cp(0);               //  kockalent
	CLRF       FARG_Lcd_Chr_CP_out_char+0
	CALL       _Lcd_Chr_CP+0
L_main19:
;lcd2.c,106 :: 		Lcd_Chr_Cp(' ');
	MOVLW      32
	MOVWF      FARG_Lcd_Chr_CP_out_char+0
	CALL       _Lcd_Chr_CP+0
;lcd2.c,107 :: 		szam++;
	INCF       main_szam_L0+0, 1
;lcd2.c,108 :: 		if (szam==10) {
	MOVF       main_szam_L0+0, 0
	XORLW      10
	BTFSS      STATUS+0, 2
	GOTO       L_main20
;lcd2.c,109 :: 		szam=0;
	CLRF       main_szam_L0+0
;lcd2.c,110 :: 		Lcd_Out(2,2,"          ");
	MOVLW      2
	MOVWF      FARG_Lcd_Out_row+0
	MOVLW      2
	MOVWF      FARG_Lcd_Out_column+0
	MOVLW      ?lstr4_lcd2+0
	MOVWF      FARG_Lcd_Out_text+0
	CALL       _Lcd_Out+0
;lcd2.c,111 :: 		}
L_main20:
;lcd2.c,112 :: 		Delay_ms(1000);
	MOVLW      11
	MOVWF      R11+0
	MOVLW      38
	MOVWF      R12+0
	MOVLW      93
	MOVWF      R13+0
L_main21:
	DECFSZ     R13+0, 1
	GOTO       L_main21
	DECFSZ     R12+0, 1
	GOTO       L_main21
	DECFSZ     R11+0, 1
	GOTO       L_main21
	NOP
	NOP
;lcd2.c,113 :: 		}
	GOTO       L_main13
;lcd2.c,115 :: 		}
L_end_main:
	GOTO       $+0
; end of _main
