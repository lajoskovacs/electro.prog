
_send_egybajt:

;lcd_ora.c,52 :: 		void send_egybajt(char rs,char adat)
;lcd_ora.c,54 :: 		char felso4=0;
;lcd_ora.c,55 :: 		char also4=0;
	CLRF       send_egybajt_also4_L0+0
;lcd_ora.c,56 :: 		also4=adat&0b00001111;              // alsó 4 bit
	MOVLW      15
	ANDWF      FARG_send_egybajt_adat+0, 0
	MOVWF      send_egybajt_also4_L0+0
;lcd_ora.c,57 :: 		felso4=(adat&0b11110000)>>4;         // felsõ 4 bit
	MOVLW      240
	ANDWF      FARG_send_egybajt_adat+0, 0
	MOVWF      R2+0
	MOVF       R2+0, 0
	MOVWF      R0+0
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
;lcd_ora.c,58 :: 		PORTD.B0=rs;        //RS láb beállítása (parancs vagy adat?)
	BTFSC      FARG_send_egybajt_rs+0, 0
	GOTO       L__send_egybajt38
	BCF        PORTD+0, 0
	GOTO       L__send_egybajt39
L__send_egybajt38:
	BSF        PORTD+0, 0
L__send_egybajt39:
;lcd_ora.c,59 :: 		PORTC=PORTC&0b11110000;
	MOVLW      240
	ANDWF      PORTC+0, 1
;lcd_ora.c,60 :: 		PORTC=PORTC|(felso4&0b00001111);      // felsõ fél-byte kiküldése
	MOVLW      15
	ANDWF      R0+0, 1
	MOVF       R0+0, 0
	IORWF      PORTC+0, 1
;lcd_ora.c,61 :: 		PORTD.B1=1;        // órajel (ENG) 1-be
	BSF        PORTD+0, 1
;lcd_ora.c,62 :: 		Delay_ms(1);
	MOVLW      6
	MOVWF      R12+0
	MOVLW      48
	MOVWF      R13+0
L_send_egybajt0:
	DECFSZ     R13+0, 1
	GOTO       L_send_egybajt0
	DECFSZ     R12+0, 1
	GOTO       L_send_egybajt0
	NOP
;lcd_ora.c,63 :: 		PORTD.B1=0;        // órajel (ENG) 0-ba
	BCF        PORTD+0, 1
;lcd_ora.c,64 :: 		Delay_ms(1);
	MOVLW      6
	MOVWF      R12+0
	MOVLW      48
	MOVWF      R13+0
L_send_egybajt1:
	DECFSZ     R13+0, 1
	GOTO       L_send_egybajt1
	DECFSZ     R12+0, 1
	GOTO       L_send_egybajt1
	NOP
;lcd_ora.c,65 :: 		PORTC=PORTC&0b11110000;
	MOVLW      240
	ANDWF      PORTC+0, 1
;lcd_ora.c,66 :: 		PORTC=PORTC|(also4&0b00001111);     // alsó fél-byte kiküldése
	MOVLW      15
	ANDWF      send_egybajt_also4_L0+0, 0
	MOVWF      R0+0
	MOVF       R0+0, 0
	IORWF      PORTC+0, 1
;lcd_ora.c,67 :: 		PORTD.B1=1;        // órajel (ENG) 1-be
	BSF        PORTD+0, 1
;lcd_ora.c,68 :: 		Delay_ms(1);
	MOVLW      6
	MOVWF      R12+0
	MOVLW      48
	MOVWF      R13+0
L_send_egybajt2:
	DECFSZ     R13+0, 1
	GOTO       L_send_egybajt2
	DECFSZ     R12+0, 1
	GOTO       L_send_egybajt2
	NOP
;lcd_ora.c,69 :: 		PORTD.B1=0;        // órajel (ENG) 0-ba
	BCF        PORTD+0, 1
;lcd_ora.c,70 :: 		Delay_ms(1);
	MOVLW      6
	MOVWF      R12+0
	MOVLW      48
	MOVWF      R13+0
L_send_egybajt3:
	DECFSZ     R13+0, 1
	GOTO       L_send_egybajt3
	DECFSZ     R12+0, 1
	GOTO       L_send_egybajt3
	NOP
;lcd_ora.c,71 :: 		}
L_end_send_egybajt:
	RETURN
; end of _send_egybajt

_send_felbajt:

;lcd_ora.c,73 :: 		void send_felbajt(char rs,char adat)
;lcd_ora.c,75 :: 		PORTC=PORTC&0b11110000;
	MOVLW      240
	ANDWF      PORTC+0, 1
;lcd_ora.c,76 :: 		PORTC=PORTC|(adat&0b00001111);          //  fél-byte kiküldése
	MOVLW      15
	ANDWF      FARG_send_felbajt_adat+0, 0
	MOVWF      R0+0
	MOVF       R0+0, 0
	IORWF      PORTC+0, 1
;lcd_ora.c,77 :: 		PORTD.B0=rs;        //RS láb beállítása (parancs vagy adat?)
	BTFSC      FARG_send_felbajt_rs+0, 0
	GOTO       L__send_felbajt41
	BCF        PORTD+0, 0
	GOTO       L__send_felbajt42
L__send_felbajt41:
	BSF        PORTD+0, 0
L__send_felbajt42:
;lcd_ora.c,78 :: 		PORTD.B1=1;        // órajel (ENG) 1-be
	BSF        PORTD+0, 1
;lcd_ora.c,79 :: 		Delay_ms(1);
	MOVLW      6
	MOVWF      R12+0
	MOVLW      48
	MOVWF      R13+0
L_send_felbajt4:
	DECFSZ     R13+0, 1
	GOTO       L_send_felbajt4
	DECFSZ     R12+0, 1
	GOTO       L_send_felbajt4
	NOP
;lcd_ora.c,80 :: 		PORTD.B1=0;        // órajel (ENG) 0-ba
	BCF        PORTD+0, 1
;lcd_ora.c,81 :: 		Delay_ms(1);
	MOVLW      6
	MOVWF      R12+0
	MOVLW      48
	MOVWF      R13+0
L_send_felbajt5:
	DECFSZ     R13+0, 1
	GOTO       L_send_felbajt5
	DECFSZ     R12+0, 1
	GOTO       L_send_felbajt5
	NOP
;lcd_ora.c,83 :: 		}
L_end_send_felbajt:
	RETURN
; end of _send_felbajt

_kezdo_beall:

;lcd_ora.c,87 :: 		void kezdo_beall(void)
;lcd_ora.c,90 :: 		OSCCON = 0x70;   // külsõ oszcillátor,  16MHZ
	MOVLW      112
	MOVWF      OSCCON+0
;lcd_ora.c,91 :: 		TRISA = 0xFF; // PORTA-n kapcsolók, RA0-fel, RA1-bal, RA2-jobb, RA3-le, RA5-stop
	MOVLW      255
	MOVWF      TRISA+0
;lcd_ora.c,92 :: 		ANSEL=0;    // PORTA, PORTE  digitális I/O
	CLRF       ANSEL+0
;lcd_ora.c,93 :: 		ANSELH=0b00101011;    // PORTB négy lába analóg bemenet !!(RB4-->AN11 RB3-->AN9   RB2-->AN8  RB5-->AN13, a többi láb digitális I/O
	MOVLW      43
	MOVWF      ANSELH+0
;lcd_ora.c,94 :: 		TRISC =0;         // LCD kijelzõ           RC0  RC1  RC2  RC3
	CLRF       TRISC+0
;lcd_ora.c,95 :: 		TRISD=0;             // LCD kijelzõ         RD0  RD1
	CLRF       TRISD+0
;lcd_ora.c,96 :: 		TRISB=0b00111100;
	MOVLW      60
	MOVWF      TRISB+0
;lcd_ora.c,97 :: 		PORTC=0;
	CLRF       PORTC+0
;lcd_ora.c,98 :: 		PORTD=0;
	CLRF       PORTD+0
;lcd_ora.c,99 :: 		PORTB=0;
	CLRF       PORTB+0
;lcd_ora.c,100 :: 		C1ON_bit = 0;               // Disable comparators
	BCF        C1ON_bit+0, BitPos(C1ON_bit+0)
;lcd_ora.c,101 :: 		C2ON_bit = 0;
	BCF        C2ON_bit+0, BitPos(C2ON_bit+0)
;lcd_ora.c,105 :: 		OPTION_REG= 0x06;   // TIMER0  elõosztó: 128 ---> Fosc/4 * 1/256      ---> ha Fosc=8Mhz ---> 15625Hz  (16MHz  ---> 31250)
	MOVLW      6
	MOVWF      OPTION_REG+0
;lcd_ora.c,106 :: 		TMR0=130;   // timer órajel 15625Hz, és 255-130= 125 számlálás után megszakítás  --->megszakítás frekv. 125Hz (16Mhz  ---> 250)
	MOVLW      130
	MOVWF      TMR0+0
;lcd_ora.c,107 :: 		INTCON=0xA0;    // TMR0 interrupt engedélyezve
	MOVLW      160
	MOVWF      INTCON+0
;lcd_ora.c,117 :: 		ADCON1= 0b10000000;  // A/D   ADFM bit --> 1     eredmény:  ADRESH alsó két bit + ADRESL
	MOVLW      128
	MOVWF      ADCON1+0
;lcd_ora.c,118 :: 		ADCON0= 0b11101100;  // AN11 lesz a bemeneti csatorna,
	MOVLW      236
	MOVWF      ADCON0+0
;lcd_ora.c,119 :: 		ADCON0=ADCON0|1;     //   A/D engedélyezés
	BSF        ADCON0+0, 0
;lcd_ora.c,124 :: 		Delay_ms(300);
	MOVLW      7
	MOVWF      R11+0
	MOVLW      23
	MOVWF      R12+0
	MOVLW      106
	MOVWF      R13+0
L_kezdo_beall6:
	DECFSZ     R13+0, 1
	GOTO       L_kezdo_beall6
	DECFSZ     R12+0, 1
	GOTO       L_kezdo_beall6
	DECFSZ     R11+0, 1
	GOTO       L_kezdo_beall6
	NOP
;lcd_ora.c,125 :: 		send_felbajt(0,0b0010);   //Set to 4 bit operation
	CLRF       FARG_send_felbajt_rs+0
	MOVLW      2
	MOVWF      FARG_send_felbajt_adat+0
	CALL       _send_felbajt+0
;lcd_ora.c,126 :: 		Delay_ms(5);
	MOVLW      26
	MOVWF      R12+0
	MOVLW      248
	MOVWF      R13+0
L_kezdo_beall7:
	DECFSZ     R13+0, 1
	GOTO       L_kezdo_beall7
	DECFSZ     R12+0, 1
	GOTO       L_kezdo_beall7
	NOP
;lcd_ora.c,127 :: 		send_egybajt(0,0b00101100);   //Function set, 4 bit mode,  2 rows, font 5x10
	CLRF       FARG_send_egybajt_rs+0
	MOVLW      44
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,128 :: 		send_egybajt(0,0b00001100);   //Display control, Display ON, Cursor Off
	CLRF       FARG_send_egybajt_rs+0
	MOVLW      12
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,129 :: 		send_egybajt(0,0b00000001);   //Clear Display
	CLRF       FARG_send_egybajt_rs+0
	MOVLW      1
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,130 :: 		send_egybajt(0,0b00000110);  //Entry Mode, Increment cursor position, No display shift
	CLRF       FARG_send_egybajt_rs+0
	MOVLW      6
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,132 :: 		}
L_end_kezdo_beall:
	RETURN
; end of _kezdo_beall

_interrupt:
	MOVWF      R15+0
	SWAPF      STATUS+0, 0
	CLRF       STATUS+0
	MOVWF      ___saveSTATUS+0
	MOVF       PCLATH+0, 0
	MOVWF      ___savePCLATH+0
	CLRF       PCLATH+0

;lcd_ora.c,138 :: 		void interrupt(){
;lcd_ora.c,141 :: 		szamlal++;
	INCF       _szamlal+0, 1
;lcd_ora.c,142 :: 		if(szamlal==250) { szamlal=0; gmasperc++;  }
	MOVF       _szamlal+0, 0
	XORLW      250
	BTFSS      STATUS+0, 2
	GOTO       L_interrupt8
	CLRF       _szamlal+0
	INCF       _gmasperc+0, 1
L_interrupt8:
;lcd_ora.c,143 :: 		if(gmasperc==60) { gmasperc=0; gperc++;  }
	MOVF       _gmasperc+0, 0
	XORLW      60
	BTFSS      STATUS+0, 2
	GOTO       L_interrupt9
	CLRF       _gmasperc+0
	INCF       _gperc+0, 1
L_interrupt9:
;lcd_ora.c,144 :: 		if(gperc==60) { gperc=0; gora++;  }
	MOVF       _gperc+0, 0
	XORLW      60
	BTFSS      STATUS+0, 2
	GOTO       L_interrupt10
	CLRF       _gperc+0
	INCF       _gora+0, 1
L_interrupt10:
;lcd_ora.c,145 :: 		if(gora==24) { gora=0;  }
	MOVF       _gora+0, 0
	XORLW      24
	BTFSS      STATUS+0, 2
	GOTO       L_interrupt11
	CLRF       _gora+0
L_interrupt11:
;lcd_ora.c,147 :: 		TMR0=130;
	MOVLW      130
	MOVWF      TMR0+0
;lcd_ora.c,148 :: 		INTCON=0xA0;    // TMR0 interrupt engedélyezve
	MOVLW      160
	MOVWF      INTCON+0
;lcd_ora.c,149 :: 		}
L_end_interrupt:
L__interrupt45:
	MOVF       ___savePCLATH+0, 0
	MOVWF      PCLATH+0
	SWAPF      ___saveSTATUS+0, 0
	MOVWF      STATUS+0
	SWAPF      R15+0, 1
	SWAPF      R15+0, 0
	RETFIE
; end of _interrupt

_irLCDre:

;lcd_ora.c,153 :: 		void irLCDre(int fok, char eloj, char ora, char perc, char masperc) {
;lcd_ora.c,154 :: 		char fokszj0=' ';        // elõjel
	MOVLW      32
	MOVWF      irLCDre_fokszj0_L0+0
	CLRF       irLCDre_fokszj1_L0+0
	CLRF       irLCDre_fokszj2_L0+0
	CLRF       irLCDre_fokszj3_L0+0
	CLRF       irLCDre_fokszj4_L0+0
	CLRF       irLCDre_oraszj1_L0+0
	CLRF       irLCDre_oraszj2_L0+0
	CLRF       irLCDre_percszj1_L0+0
	CLRF       irLCDre_percszj2_L0+0
	CLRF       irLCDre_mpercszj1_L0+0
	CLRF       irLCDre_mpercszj2_L0+0
;lcd_ora.c,166 :: 		fokszj0=eloj;
	MOVF       FARG_irLCDre_eloj+0, 0
	MOVWF      irLCDre_fokszj0_L0+0
;lcd_ora.c,167 :: 		fokszj4=fok%10;
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVF       FARG_irLCDre_fok+0, 0
	MOVWF      R0+0
	MOVF       FARG_irLCDre_fok+1, 0
	MOVWF      R0+1
	CALL       _Div_16x16_S+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVF       R8+1, 0
	MOVWF      R0+1
	MOVF       R0+0, 0
	MOVWF      irLCDre_fokszj4_L0+0
;lcd_ora.c,168 :: 		fok=fok/10;
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVF       FARG_irLCDre_fok+0, 0
	MOVWF      R0+0
	MOVF       FARG_irLCDre_fok+1, 0
	MOVWF      R0+1
	CALL       _Div_16x16_S+0
	MOVF       R0+0, 0
	MOVWF      FARG_irLCDre_fok+0
	MOVF       R0+1, 0
	MOVWF      FARG_irLCDre_fok+1
;lcd_ora.c,169 :: 		fokszj3=fok%10;
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	CALL       _Div_16x16_S+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVF       R8+1, 0
	MOVWF      R0+1
	MOVF       R0+0, 0
	MOVWF      irLCDre_fokszj3_L0+0
;lcd_ora.c,170 :: 		fok=fok/10;
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVF       FARG_irLCDre_fok+0, 0
	MOVWF      R0+0
	MOVF       FARG_irLCDre_fok+1, 0
	MOVWF      R0+1
	CALL       _Div_16x16_S+0
	MOVF       R0+0, 0
	MOVWF      FARG_irLCDre_fok+0
	MOVF       R0+1, 0
	MOVWF      FARG_irLCDre_fok+1
;lcd_ora.c,171 :: 		fokszj2=fok%10;
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	CALL       _Div_16x16_S+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVF       R8+1, 0
	MOVWF      R0+1
	MOVF       R0+0, 0
	MOVWF      irLCDre_fokszj2_L0+0
;lcd_ora.c,172 :: 		fokszj1=fok/10;
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVF       FARG_irLCDre_fok+0, 0
	MOVWF      R0+0
	MOVF       FARG_irLCDre_fok+1, 0
	MOVWF      R0+1
	CALL       _Div_16x16_S+0
	MOVF       R0+0, 0
	MOVWF      irLCDre_fokszj1_L0+0
;lcd_ora.c,174 :: 		oraszj1=ora/10;
	MOVLW      10
	MOVWF      R4+0
	MOVF       FARG_irLCDre_ora+0, 0
	MOVWF      R0+0
	CALL       _Div_8x8_U+0
	MOVF       R0+0, 0
	MOVWF      irLCDre_oraszj1_L0+0
;lcd_ora.c,175 :: 		oraszj2=ora-10*oraszj1;
	MOVLW      10
	MOVWF      R4+0
	CALL       _Mul_8x8_U+0
	MOVF       R0+0, 0
	SUBWF      FARG_irLCDre_ora+0, 0
	MOVWF      irLCDre_oraszj2_L0+0
;lcd_ora.c,176 :: 		percszj1=perc/10;
	MOVLW      10
	MOVWF      R4+0
	MOVF       FARG_irLCDre_perc+0, 0
	MOVWF      R0+0
	CALL       _Div_8x8_U+0
	MOVF       R0+0, 0
	MOVWF      irLCDre_percszj1_L0+0
;lcd_ora.c,177 :: 		percszj2=perc-10*percszj1;
	MOVLW      10
	MOVWF      R4+0
	CALL       _Mul_8x8_U+0
	MOVF       R0+0, 0
	SUBWF      FARG_irLCDre_perc+0, 0
	MOVWF      irLCDre_percszj2_L0+0
;lcd_ora.c,178 :: 		mpercszj1=masperc/10;
	MOVLW      10
	MOVWF      R4+0
	MOVF       FARG_irLCDre_masperc+0, 0
	MOVWF      R0+0
	CALL       _Div_8x8_U+0
	MOVF       R0+0, 0
	MOVWF      irLCDre_mpercszj1_L0+0
;lcd_ora.c,179 :: 		mpercszj2=masperc-10*mpercszj1;
	MOVLW      10
	MOVWF      R4+0
	CALL       _Mul_8x8_U+0
	MOVF       R0+0, 0
	SUBWF      FARG_irLCDre_masperc+0, 0
	MOVWF      irLCDre_mpercszj2_L0+0
;lcd_ora.c,180 :: 		send_egybajt(0,0b10000000);   //1. sor 1. pozíció
	CLRF       FARG_send_egybajt_rs+0
	MOVLW      128
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,181 :: 		send_egybajt(1,oraszj1+48);        // egy karakter kiírása  (a 0   ASCII kódja a 48 !!)
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      48
	ADDWF      irLCDre_oraszj1_L0+0, 0
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,182 :: 		send_egybajt(1,oraszj2+48);
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      48
	ADDWF      irLCDre_oraszj2_L0+0, 0
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,183 :: 		send_egybajt(1,':');
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      58
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,184 :: 		send_egybajt(1,percszj1+48);
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      48
	ADDWF      irLCDre_percszj1_L0+0, 0
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,185 :: 		send_egybajt(1,percszj2+48);
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      48
	ADDWF      irLCDre_percszj2_L0+0, 0
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,186 :: 		send_egybajt(1,':');
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      58
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,187 :: 		send_egybajt(1,mpercszj1+48);
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      48
	ADDWF      irLCDre_mpercszj1_L0+0, 0
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,188 :: 		send_egybajt(1,mpercszj2+48);
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      48
	ADDWF      irLCDre_mpercszj2_L0+0, 0
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,190 :: 		send_egybajt(0,0b11000000);   //2. sor 1. pozíció
	CLRF       FARG_send_egybajt_rs+0
	MOVLW      192
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,191 :: 		send_egybajt(1,fokszj0);   //szöveg kiírása
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVF       irLCDre_fokszj0_L0+0, 0
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,192 :: 		send_egybajt(1,fokszj1+48);        // egy karakter kiírása  (a 0   ASCII kódja a 48 !!)
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      48
	ADDWF      irLCDre_fokszj1_L0+0, 0
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,193 :: 		send_egybajt(1,fokszj2+48);
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      48
	ADDWF      irLCDre_fokszj2_L0+0, 0
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,194 :: 		send_egybajt(1,fokszj3+48);
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      48
	ADDWF      irLCDre_fokszj3_L0+0, 0
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,195 :: 		send_egybajt(1,'.');
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      46
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,196 :: 		send_egybajt(1,fokszj4+48);
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      48
	ADDWF      irLCDre_fokszj4_L0+0, 0
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,198 :: 		}
L_end_irLCDre:
	RETURN
; end of _irLCDre

_main:

;lcd_ora.c,201 :: 		void main() {
;lcd_ora.c,203 :: 		char i=1;
	MOVLW      1
	MOVWF      main_i_L0+0
	CLRF       main_fok2_L0+0
	CLRF       main_fok2_L0+1
	MOVLW      32
	MOVWF      main_eloj_L0+0
	CLRF       main_poti_L0+0
	CLRF       main_poti_L0+1
;lcd_ora.c,214 :: 		kezdo_beall();
	CALL       _kezdo_beall+0
;lcd_ora.c,215 :: 		Delay_ms(15);
	MOVLW      78
	MOVWF      R12+0
	MOVLW      235
	MOVWF      R13+0
L_main12:
	DECFSZ     R13+0, 1
	GOTO       L_main12
	DECFSZ     R12+0, 1
	GOTO       L_main12
;lcd_ora.c,217 :: 		send_egybajt(0,0b00000001);   //Clear Display
	CLRF       FARG_send_egybajt_rs+0
	MOVLW      1
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,218 :: 		send_egybajt(0,0b10000000);   //1. sor 1. pozíció
	CLRF       FARG_send_egybajt_rs+0
	MOVLW      128
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,219 :: 		send_egybajt(1,'H');   //szöveg kiírása
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      72
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,220 :: 		send_egybajt(1,'e');
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      101
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,221 :: 		send_egybajt(1,'l');
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      108
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,222 :: 		send_egybajt(1,'o');
	MOVLW      1
	MOVWF      FARG_send_egybajt_rs+0
	MOVLW      111
	MOVWF      FARG_send_egybajt_adat+0
	CALL       _send_egybajt+0
;lcd_ora.c,224 :: 		while(1){
L_main13:
;lcd_ora.c,229 :: 		ADCON0=ADCON0|2;  // GO bit -->1   A/D indítása
	BSF        ADCON0+0, 1
;lcd_ora.c,230 :: 		while((ADCON0&2)!=0){   // várakozás A/D eredményre  amikor DONE=0 lesz
L_main15:
	MOVLW      2
	ANDWF      ADCON0+0, 0
	MOVWF      R1+0
	MOVF       R1+0, 0
	XORLW      0
	BTFSC      STATUS+0, 2
	GOTO       L_main16
;lcd_ora.c,231 :: 		asm nop;
	NOP
;lcd_ora.c,232 :: 		}
	GOTO       L_main15
L_main16:
;lcd_ora.c,233 :: 		poti=ADRESH&3;
	MOVLW      3
	ANDWF      ADRESH+0, 0
	MOVWF      main_poti_L0+0
	CLRF       main_poti_L0+1
	MOVLW      0
	ANDWF      main_poti_L0+1, 1
	MOVLW      0
	MOVWF      main_poti_L0+1
;lcd_ora.c,234 :: 		poti=(poti<<8)+ADRESL;     // eredmény:  ADRESH alsó két bit + ADRESL
	MOVF       main_poti_L0+0, 0
	MOVWF      R0+1
	CLRF       R0+0
	MOVF       ADRESL+0, 0
	ADDWF      R0+0, 0
	MOVWF      R2+0
	MOVF       R0+1, 0
	BTFSC      STATUS+0, 0
	ADDLW      1
	MOVWF      R2+1
	MOVF       R2+0, 0
	MOVWF      main_poti_L0+0
	MOVF       R2+1, 0
	MOVWF      main_poti_L0+1
;lcd_ora.c,236 :: 		if(poti<0) poti=0;
	MOVLW      128
	XORWF      R2+1, 0
	MOVWF      R0+0
	MOVLW      128
	SUBWF      R0+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main48
	MOVLW      0
	SUBWF      R2+0, 0
L__main48:
	BTFSC      STATUS+0, 0
	GOTO       L_main17
	CLRF       main_poti_L0+0
	CLRF       main_poti_L0+1
L_main17:
;lcd_ora.c,237 :: 		if(poti>1023) poti=1023;
	MOVLW      128
	XORLW      3
	MOVWF      R0+0
	MOVLW      128
	XORWF      main_poti_L0+1, 0
	SUBWF      R0+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main49
	MOVF       main_poti_L0+0, 0
	SUBLW      255
L__main49:
	BTFSC      STATUS+0, 0
	GOTO       L_main18
	MOVLW      255
	MOVWF      main_poti_L0+0
	MOVLW      3
	MOVWF      main_poti_L0+1
L_main18:
;lcd_ora.c,240 :: 		if(PORTA.B2==0)  {perc_set=1; ora_jar=0; ora_set=0; }         //jobb ---> perc beállítása
	BTFSC      PORTA+0, 2
	GOTO       L_main19
L_main19:
;lcd_ora.c,241 :: 		if(PORTA.B1==0)  {ora_set=1; ora_jar=0; perc_set=0; }         //bal  -->  óra beállítása
	BTFSC      PORTA+0, 1
	GOTO       L_main20
L_main20:
;lcd_ora.c,242 :: 		if(PORTA.B5==0)  {ora_jar=1; ora_set=0; perc_set=0; }         //stop ---> normál üzemmód,  jár az óra
	BTFSC      PORTA+0, 5
	GOTO       L_main21
L_main21:
;lcd_ora.c,243 :: 		if(PORTA.B0==0) {   }                    // fel  ---> növelés
	BTFSC      PORTA+0, 0
	GOTO       L_main22
L_main22:
;lcd_ora.c,244 :: 		if(PORTA.B3==0)  {    }                  // le ---> csökkentés
	BTFSC      PORTA+0, 3
	GOTO       L_main23
L_main23:
;lcd_ora.c,246 :: 		i=0;
	CLRF       main_i_L0+0
;lcd_ora.c,247 :: 		while(poti<fesz[i]) i++;
L_main24:
	MOVF       main_i_L0+0, 0
	MOVWF      R0+0
	CLRF       R0+1
	RLF        R0+0, 1
	RLF        R0+1, 1
	BCF        R0+0, 0
	MOVLW      _fesz+0
	ADDWF      R0+0, 1
	MOVLW      hi_addr(_fesz+0)
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      R0+1, 1
	MOVF       R0+0, 0
	MOVWF      ___DoICPAddr+0
	MOVF       R0+1, 0
	MOVWF      ___DoICPAddr+1
	CALL       _____DoICP+0
	MOVWF      R2+0
	INCF       ___DoICPAddr+0, 1
	BTFSC      STATUS+0, 2
	INCF       ___DoICPAddr+1, 1
	CALL       _____DoICP+0
	MOVWF      R2+1
	MOVLW      128
	XORWF      main_poti_L0+1, 0
	MOVWF      R0+0
	MOVLW      128
	XORWF      R2+1, 0
	SUBWF      R0+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main50
	MOVF       R2+0, 0
	SUBWF      main_poti_L0+0, 0
L__main50:
	BTFSC      STATUS+0, 0
	GOTO       L_main25
	INCF       main_i_L0+0, 1
	GOTO       L_main24
L_main25:
;lcd_ora.c,248 :: 		fok=100*foktar[i];               //  -65  -----    +150
	MOVF       main_i_L0+0, 0
	MOVWF      R0+0
	CLRF       R0+1
	RLF        R0+0, 1
	RLF        R0+1, 1
	BCF        R0+0, 0
	MOVLW      _foktar+0
	ADDWF      R0+0, 1
	MOVLW      hi_addr(_foktar+0)
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      R0+1, 1
	MOVF       R0+0, 0
	MOVWF      ___DoICPAddr+0
	MOVF       R0+1, 0
	MOVWF      ___DoICPAddr+1
	CALL       _____DoICP+0
	MOVWF      R0+0
	INCF       ___DoICPAddr+0, 1
	BTFSC      STATUS+0, 2
	INCF       ___DoICPAddr+1, 1
	CALL       _____DoICP+0
	MOVWF      R0+1
	MOVLW      100
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	CALL       _Mul_16x16_U+0
	MOVF       R0+0, 0
	MOVWF      main_fok_L0+0
	MOVF       R0+1, 0
	MOVWF      main_fok_L0+1
	MOVLW      0
	BTFSC      main_fok_L0+1, 7
	MOVLW      255
	MOVWF      main_fok_L0+2
	MOVWF      main_fok_L0+3
;lcd_ora.c,249 :: 		if(i>1 && i<34) fok=fok-500*(poti-fesz[i])/(fesz[i-1]-fesz[i]);
	MOVF       main_i_L0+0, 0
	SUBLW      1
	BTFSC      STATUS+0, 0
	GOTO       L_main28
	MOVLW      34
	SUBWF      main_i_L0+0, 0
	BTFSC      STATUS+0, 0
	GOTO       L_main28
L__main36:
	MOVF       main_i_L0+0, 0
	MOVWF      R0+0
	CLRF       R0+1
	RLF        R0+0, 1
	RLF        R0+1, 1
	BCF        R0+0, 0
	MOVLW      _fesz+0
	ADDWF      R0+0, 1
	MOVLW      hi_addr(_fesz+0)
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      R0+1, 1
	MOVF       R0+0, 0
	MOVWF      ___DoICPAddr+0
	MOVF       R0+1, 0
	MOVWF      ___DoICPAddr+1
	CALL       _____DoICP+0
	MOVWF      FLOC__main+0
	INCF       ___DoICPAddr+0, 1
	BTFSC      STATUS+0, 2
	INCF       ___DoICPAddr+1, 1
	CALL       _____DoICP+0
	MOVWF      FLOC__main+1
	MOVF       FLOC__main+0, 0
	SUBWF      main_poti_L0+0, 0
	MOVWF      R0+0
	MOVF       FLOC__main+1, 0
	BTFSS      STATUS+0, 0
	ADDLW      1
	SUBWF      main_poti_L0+1, 0
	MOVWF      R0+1
	MOVLW      244
	MOVWF      R4+0
	MOVLW      1
	MOVWF      R4+1
	CALL       _Mul_16x16_U+0
	MOVLW      1
	SUBWF      main_i_L0+0, 0
	MOVWF      R5+0
	CLRF       R5+1
	BTFSS      STATUS+0, 0
	DECF       R5+1, 1
	MOVF       R5+0, 0
	MOVWF      R2+0
	MOVF       R5+1, 0
	MOVWF      R2+1
	RLF        R2+0, 1
	RLF        R2+1, 1
	BCF        R2+0, 0
	MOVLW      _fesz+0
	ADDWF      R2+0, 1
	MOVLW      hi_addr(_fesz+0)
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      R2+1, 1
	MOVF       R2+0, 0
	MOVWF      ___DoICPAddr+0
	MOVF       R2+1, 0
	MOVWF      ___DoICPAddr+1
	CALL       _____DoICP+0
	MOVWF      R2+0
	INCF       ___DoICPAddr+0, 1
	BTFSC      STATUS+0, 2
	INCF       ___DoICPAddr+1, 1
	CALL       _____DoICP+0
	MOVWF      R2+1
	MOVF       FLOC__main+0, 0
	SUBWF      R2+0, 0
	MOVWF      R4+0
	MOVF       FLOC__main+1, 0
	BTFSS      STATUS+0, 0
	ADDLW      1
	SUBWF      R2+1, 0
	MOVWF      R4+1
	CALL       _Div_16x16_S+0
	MOVLW      0
	BTFSC      R0+1, 7
	MOVLW      255
	MOVWF      R0+2
	MOVWF      R0+3
	MOVF       R0+0, 0
	SUBWF      main_fok_L0+0, 1
	MOVF       R0+1, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+1, 0
	SUBWF      main_fok_L0+1, 1
	MOVF       R0+2, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+2, 0
	SUBWF      main_fok_L0+2, 1
	MOVF       R0+3, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+3, 0
	SUBWF      main_fok_L0+3, 1
L_main28:
;lcd_ora.c,250 :: 		if(i==34) fok=fok-4000*(poti-fesz[i])/(fesz[i-1]-fesz[i]);
	MOVF       main_i_L0+0, 0
	XORLW      34
	BTFSS      STATUS+0, 2
	GOTO       L_main29
	MOVF       main_i_L0+0, 0
	MOVWF      R0+0
	CLRF       R0+1
	RLF        R0+0, 1
	RLF        R0+1, 1
	BCF        R0+0, 0
	MOVLW      _fesz+0
	ADDWF      R0+0, 1
	MOVLW      hi_addr(_fesz+0)
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      R0+1, 1
	MOVF       R0+0, 0
	MOVWF      ___DoICPAddr+0
	MOVF       R0+1, 0
	MOVWF      ___DoICPAddr+1
	CALL       _____DoICP+0
	MOVWF      FLOC__main+0
	INCF       ___DoICPAddr+0, 1
	BTFSC      STATUS+0, 2
	INCF       ___DoICPAddr+1, 1
	CALL       _____DoICP+0
	MOVWF      FLOC__main+1
	MOVF       FLOC__main+0, 0
	SUBWF      main_poti_L0+0, 0
	MOVWF      R0+0
	MOVF       FLOC__main+1, 0
	BTFSS      STATUS+0, 0
	ADDLW      1
	SUBWF      main_poti_L0+1, 0
	MOVWF      R0+1
	MOVLW      160
	MOVWF      R4+0
	MOVLW      15
	MOVWF      R4+1
	CALL       _Mul_16x16_U+0
	MOVLW      1
	SUBWF      main_i_L0+0, 0
	MOVWF      R5+0
	CLRF       R5+1
	BTFSS      STATUS+0, 0
	DECF       R5+1, 1
	MOVF       R5+0, 0
	MOVWF      R2+0
	MOVF       R5+1, 0
	MOVWF      R2+1
	RLF        R2+0, 1
	RLF        R2+1, 1
	BCF        R2+0, 0
	MOVLW      _fesz+0
	ADDWF      R2+0, 1
	MOVLW      hi_addr(_fesz+0)
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      R2+1, 1
	MOVF       R2+0, 0
	MOVWF      ___DoICPAddr+0
	MOVF       R2+1, 0
	MOVWF      ___DoICPAddr+1
	CALL       _____DoICP+0
	MOVWF      R2+0
	INCF       ___DoICPAddr+0, 1
	BTFSC      STATUS+0, 2
	INCF       ___DoICPAddr+1, 1
	CALL       _____DoICP+0
	MOVWF      R2+1
	MOVF       FLOC__main+0, 0
	SUBWF      R2+0, 0
	MOVWF      R4+0
	MOVF       FLOC__main+1, 0
	BTFSS      STATUS+0, 0
	ADDLW      1
	SUBWF      R2+1, 0
	MOVWF      R4+1
	CALL       _Div_16x16_S+0
	MOVLW      0
	BTFSC      R0+1, 7
	MOVLW      255
	MOVWF      R0+2
	MOVWF      R0+3
	MOVF       R0+0, 0
	SUBWF      main_fok_L0+0, 1
	MOVF       R0+1, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+1, 0
	SUBWF      main_fok_L0+1, 1
	MOVF       R0+2, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+2, 0
	SUBWF      main_fok_L0+2, 1
	MOVF       R0+3, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+3, 0
	SUBWF      main_fok_L0+3, 1
L_main29:
;lcd_ora.c,251 :: 		if(i==1) fok=fok-1500*(poti-fesz[i])/(fesz[i-1]-fesz[i]);
	MOVF       main_i_L0+0, 0
	XORLW      1
	BTFSS      STATUS+0, 2
	GOTO       L_main30
	MOVF       main_i_L0+0, 0
	MOVWF      R0+0
	CLRF       R0+1
	RLF        R0+0, 1
	RLF        R0+1, 1
	BCF        R0+0, 0
	MOVLW      _fesz+0
	ADDWF      R0+0, 1
	MOVLW      hi_addr(_fesz+0)
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      R0+1, 1
	MOVF       R0+0, 0
	MOVWF      ___DoICPAddr+0
	MOVF       R0+1, 0
	MOVWF      ___DoICPAddr+1
	CALL       _____DoICP+0
	MOVWF      FLOC__main+0
	INCF       ___DoICPAddr+0, 1
	BTFSC      STATUS+0, 2
	INCF       ___DoICPAddr+1, 1
	CALL       _____DoICP+0
	MOVWF      FLOC__main+1
	MOVF       FLOC__main+0, 0
	SUBWF      main_poti_L0+0, 0
	MOVWF      R0+0
	MOVF       FLOC__main+1, 0
	BTFSS      STATUS+0, 0
	ADDLW      1
	SUBWF      main_poti_L0+1, 0
	MOVWF      R0+1
	MOVLW      220
	MOVWF      R4+0
	MOVLW      5
	MOVWF      R4+1
	CALL       _Mul_16x16_U+0
	MOVLW      1
	SUBWF      main_i_L0+0, 0
	MOVWF      R5+0
	CLRF       R5+1
	BTFSS      STATUS+0, 0
	DECF       R5+1, 1
	MOVF       R5+0, 0
	MOVWF      R2+0
	MOVF       R5+1, 0
	MOVWF      R2+1
	RLF        R2+0, 1
	RLF        R2+1, 1
	BCF        R2+0, 0
	MOVLW      _fesz+0
	ADDWF      R2+0, 1
	MOVLW      hi_addr(_fesz+0)
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      R2+1, 1
	MOVF       R2+0, 0
	MOVWF      ___DoICPAddr+0
	MOVF       R2+1, 0
	MOVWF      ___DoICPAddr+1
	CALL       _____DoICP+0
	MOVWF      R2+0
	INCF       ___DoICPAddr+0, 1
	BTFSC      STATUS+0, 2
	INCF       ___DoICPAddr+1, 1
	CALL       _____DoICP+0
	MOVWF      R2+1
	MOVF       FLOC__main+0, 0
	SUBWF      R2+0, 0
	MOVWF      R4+0
	MOVF       FLOC__main+1, 0
	BTFSS      STATUS+0, 0
	ADDLW      1
	SUBWF      R2+1, 0
	MOVWF      R4+1
	CALL       _Div_16x16_S+0
	MOVLW      0
	BTFSC      R0+1, 7
	MOVLW      255
	MOVWF      R0+2
	MOVWF      R0+3
	MOVF       R0+0, 0
	SUBWF      main_fok_L0+0, 1
	MOVF       R0+1, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+1, 0
	SUBWF      main_fok_L0+1, 1
	MOVF       R0+2, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+2, 0
	SUBWF      main_fok_L0+2, 1
	MOVF       R0+3, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+3, 0
	SUBWF      main_fok_L0+3, 1
L_main30:
;lcd_ora.c,252 :: 		if(i==0) fok=-6500;
	MOVF       main_i_L0+0, 0
	XORLW      0
	BTFSS      STATUS+0, 2
	GOTO       L_main31
	MOVLW      156
	MOVWF      main_fok_L0+0
	MOVLW      230
	MOVWF      main_fok_L0+1
	MOVLW      255
	MOVWF      main_fok_L0+2
	MOVWF      main_fok_L0+3
L_main31:
;lcd_ora.c,254 :: 		if(fok<0) { eloj='-'; fok=-fok;  }     // elõjel beállítása
	MOVLW      128
	XORWF      main_fok_L0+3, 0
	MOVWF      R0+0
	MOVLW      128
	SUBWF      R0+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main51
	MOVLW      0
	SUBWF      main_fok_L0+2, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main51
	MOVLW      0
	SUBWF      main_fok_L0+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main51
	MOVLW      0
	SUBWF      main_fok_L0+0, 0
L__main51:
	BTFSC      STATUS+0, 0
	GOTO       L_main32
	MOVLW      45
	MOVWF      main_eloj_L0+0
	CLRF       R0+0
	CLRF       R0+1
	CLRF       R0+2
	CLRF       R0+3
	MOVF       main_fok_L0+0, 0
	SUBWF      R0+0, 1
	MOVF       main_fok_L0+1, 0
	BTFSS      STATUS+0, 0
	INCFSZ     main_fok_L0+1, 0
	SUBWF      R0+1, 1
	MOVF       main_fok_L0+2, 0
	BTFSS      STATUS+0, 0
	INCFSZ     main_fok_L0+2, 0
	SUBWF      R0+2, 1
	MOVF       main_fok_L0+3, 0
	BTFSS      STATUS+0, 0
	INCFSZ     main_fok_L0+3, 0
	SUBWF      R0+3, 1
	MOVF       R0+0, 0
	MOVWF      main_fok_L0+0
	MOVF       R0+1, 0
	MOVWF      main_fok_L0+1
	MOVF       R0+2, 0
	MOVWF      main_fok_L0+2
	MOVF       R0+3, 0
	MOVWF      main_fok_L0+3
L_main32:
;lcd_ora.c,255 :: 		if(fok%10>5)   fok2=fok/10+1;
	MOVLW      10
	MOVWF      R4+0
	CLRF       R4+1
	CLRF       R4+2
	CLRF       R4+3
	MOVF       main_fok_L0+0, 0
	MOVWF      R0+0
	MOVF       main_fok_L0+1, 0
	MOVWF      R0+1
	MOVF       main_fok_L0+2, 0
	MOVWF      R0+2
	MOVF       main_fok_L0+3, 0
	MOVWF      R0+3
	CALL       _Div_32x32_S+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVF       R8+1, 0
	MOVWF      R0+1
	MOVF       R8+2, 0
	MOVWF      R0+2
	MOVF       R8+3, 0
	MOVWF      R0+3
	MOVLW      128
	MOVWF      R4+0
	MOVLW      128
	XORWF      R0+3, 0
	SUBWF      R4+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main52
	MOVF       R0+2, 0
	SUBLW      0
	BTFSS      STATUS+0, 2
	GOTO       L__main52
	MOVF       R0+1, 0
	SUBLW      0
	BTFSS      STATUS+0, 2
	GOTO       L__main52
	MOVF       R0+0, 0
	SUBLW      5
L__main52:
	BTFSC      STATUS+0, 0
	GOTO       L_main33
	MOVLW      10
	MOVWF      R4+0
	CLRF       R4+1
	CLRF       R4+2
	CLRF       R4+3
	MOVF       main_fok_L0+0, 0
	MOVWF      R0+0
	MOVF       main_fok_L0+1, 0
	MOVWF      R0+1
	MOVF       main_fok_L0+2, 0
	MOVWF      R0+2
	MOVF       main_fok_L0+3, 0
	MOVWF      R0+3
	CALL       _Div_32x32_S+0
	MOVF       R0+0, 0
	ADDLW      1
	MOVWF      main_fok2_L0+0
	MOVLW      0
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      R0+1, 0
	MOVWF      main_fok2_L0+1
	GOTO       L_main34
L_main33:
;lcd_ora.c,256 :: 		else   fok2=fok/10;     // kerekítés egy tizedes jegyre
	MOVLW      10
	MOVWF      R4+0
	CLRF       R4+1
	CLRF       R4+2
	CLRF       R4+3
	MOVF       main_fok_L0+0, 0
	MOVWF      R0+0
	MOVF       main_fok_L0+1, 0
	MOVWF      R0+1
	MOVF       main_fok_L0+2, 0
	MOVWF      R0+2
	MOVF       main_fok_L0+3, 0
	MOVWF      R0+3
	CALL       _Div_32x32_S+0
	MOVF       R0+0, 0
	MOVWF      main_fok2_L0+0
	MOVF       R0+1, 0
	MOVWF      main_fok2_L0+1
L_main34:
;lcd_ora.c,258 :: 		irLCDre(fok2,eloj,gora,gperc,gmasperc);
	MOVF       main_fok2_L0+0, 0
	MOVWF      FARG_irLCDre_fok+0
	MOVF       main_fok2_L0+1, 0
	MOVWF      FARG_irLCDre_fok+1
	MOVF       main_eloj_L0+0, 0
	MOVWF      FARG_irLCDre_eloj+0
	MOVF       _gora+0, 0
	MOVWF      FARG_irLCDre_ora+0
	MOVF       _gperc+0, 0
	MOVWF      FARG_irLCDre_perc+0
	MOVF       _gmasperc+0, 0
	MOVWF      FARG_irLCDre_masperc+0
	CALL       _irLCDre+0
;lcd_ora.c,260 :: 		Delay_ms(200);
	MOVLW      5
	MOVWF      R11+0
	MOVLW      15
	MOVWF      R12+0
	MOVLW      241
	MOVWF      R13+0
L_main35:
	DECFSZ     R13+0, 1
	GOTO       L_main35
	DECFSZ     R12+0, 1
	GOTO       L_main35
	DECFSZ     R11+0, 1
	GOTO       L_main35
;lcd_ora.c,262 :: 		}
	GOTO       L_main13
;lcd_ora.c,264 :: 		}
L_end_main:
	GOTO       $+0
; end of _main
