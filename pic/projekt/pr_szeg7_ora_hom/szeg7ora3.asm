
_kezdo_beall:

;szeg7ora3.c,36 :: 		void kezdo_beall(void)
;szeg7ora3.c,50 :: 		OSCCON = 0x70;   // külsõ oszcillátor,  16MHZ
	MOVLW      112
	MOVWF      OSCCON+0
;szeg7ora3.c,51 :: 		TRISA = 0xFF; // PORTA-n kapcsolók, RA0-fel, RA1-bal, RA2-jobb, RA3-le, RA5-stop
	MOVLW      255
	MOVWF      TRISA+0
;szeg7ora3.c,52 :: 		ANSEL=0;    // PORTA digitális I/O
	CLRF       ANSEL+0
;szeg7ora3.c,53 :: 		ANSELH=0;    // PORTB  digitális I/O
	CLRF       ANSELH+0
;szeg7ora3.c,54 :: 		TRISB=0;         // 7 szegmenses kijelzõ  vezérlése        1 szj ->  RB0   2.szj. ->  RB1   3.szj. ->  RB2      4.szj. -> RB3
	CLRF       TRISB+0
;szeg7ora3.c,55 :: 		TRISD=0;             // 7 szegmenses kijelzõ,     a-h szegmensei ->       RD0 -- RD7
	CLRF       TRISD+0
;szeg7ora3.c,56 :: 		PORTD=0;
	CLRF       PORTD+0
;szeg7ora3.c,57 :: 		PORTB=0;
	CLRF       PORTB+0
;szeg7ora3.c,63 :: 		T1CON= 0b00110101;   // TIMER1  elõosztó: 8 ---> Fosc/4 * 1/8      ---> ha Fosc=16Mhz ---> Ft1=500kHz
	MOVLW      53
	MOVWF      T1CON+0
;szeg7ora3.c,64 :: 		PIR1=0;  // jelzõbit törlése
	CLRF       PIR1+0
;szeg7ora3.c,65 :: 		TMR1H=0b00001011;
	MOVLW      11
	MOVWF      TMR1H+0
;szeg7ora3.c,66 :: 		TMR1L=0b11011100;      // 3036 kezdõ érték ---> 62500 lépés  --->  500kHz/62500=8Hz ---> szamlal 1s alatt 8-et lép !!
	MOVLW      220
	MOVWF      TMR1L+0
;szeg7ora3.c,67 :: 		INTCON=0b11000000;    //  interrupt engedélyezve, globális és periféria
	MOVLW      192
	MOVWF      INTCON+0
;szeg7ora3.c,68 :: 		PIE1=0b00000001;      // TMR1 interrupt engedélyezve
	MOVLW      1
	MOVWF      PIE1+0
;szeg7ora3.c,75 :: 		ANSEL=0b11100000;    // PORTA digitális I/O,  PORTE  analóg
	MOVLW      224
	MOVWF      ANSEL+0
;szeg7ora3.c,76 :: 		TRISE=0b00000111;      //  RE0, RE1, RE2  bemenet
	MOVLW      7
	MOVWF      TRISE+0
;szeg7ora3.c,88 :: 		ADCON1= 0b10000000;  // A/D   ADFM bit --> 1     eredmény:  ADRESH alsó két bit + ADRESL
	MOVLW      128
	MOVWF      ADCON1+0
;szeg7ora3.c,90 :: 		ADCON0= 0b11010100;  // AN5 (RE0) lesz a bemeneti csatorna,
	MOVLW      212
	MOVWF      ADCON0+0
;szeg7ora3.c,92 :: 		ADCON0=ADCON0|1;     //   A/D engedélyezés
	BSF        ADCON0+0, 0
;szeg7ora3.c,96 :: 		Delay_ms(300);
	MOVLW      7
	MOVWF      R11+0
	MOVLW      23
	MOVWF      R12+0
	MOVLW      106
	MOVWF      R13+0
L_kezdo_beall0:
	DECFSZ     R13+0, 1
	GOTO       L_kezdo_beall0
	DECFSZ     R12+0, 1
	GOTO       L_kezdo_beall0
	DECFSZ     R11+0, 1
	GOTO       L_kezdo_beall0
	NOP
;szeg7ora3.c,98 :: 		}
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

;szeg7ora3.c,103 :: 		void interrupt(){
;szeg7ora3.c,106 :: 		szamlal++;
	INCF       _szamlal+0, 1
;szeg7ora3.c,107 :: 		if(szamlal==8) { szamlal=0; gmasperc++;  }
	MOVF       _szamlal+0, 0
	XORLW      8
	BTFSS      STATUS+0, 2
	GOTO       L_interrupt1
	CLRF       _szamlal+0
	INCF       _gmasperc+0, 1
L_interrupt1:
;szeg7ora3.c,108 :: 		if(ora_jar)
	MOVF       _ora_jar+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_interrupt2
;szeg7ora3.c,110 :: 		if(gmasperc==60) { gmasperc=0; gperc++;  }
	MOVF       _gmasperc+0, 0
	XORLW      60
	BTFSS      STATUS+0, 2
	GOTO       L_interrupt3
	CLRF       _gmasperc+0
	INCF       _gperc+0, 1
L_interrupt3:
;szeg7ora3.c,111 :: 		if(gperc==60) { gperc=0; gora++;  }
	MOVF       _gperc+0, 0
	XORLW      60
	BTFSS      STATUS+0, 2
	GOTO       L_interrupt4
	CLRF       _gperc+0
	INCF       _gora+0, 1
L_interrupt4:
;szeg7ora3.c,112 :: 		if(gora==24) { gora=0;  }
	MOVF       _gora+0, 0
	XORLW      24
	BTFSS      STATUS+0, 2
	GOTO       L_interrupt5
	CLRF       _gora+0
L_interrupt5:
;szeg7ora3.c,113 :: 		}
L_interrupt2:
;szeg7ora3.c,114 :: 		PIR1=0;  // jelzõbit törlése
	CLRF       PIR1+0
;szeg7ora3.c,115 :: 		TMR1H=0b00001011;
	MOVLW      11
	MOVWF      TMR1H+0
;szeg7ora3.c,116 :: 		TMR1L=0b11011100;      // 3036 kezdõ érték ---> 62500 lépés  --->  500kHz/62500=8Hz ---> szamlal 1s alatt 8-et lép !!
	MOVLW      220
	MOVWF      TMR1L+0
;szeg7ora3.c,117 :: 		INTCON=0b11000000;    //  interrupt engedélyezve, globális és periféria
	MOVLW      192
	MOVWF      INTCON+0
;szeg7ora3.c,118 :: 		PIE1=0b00000001;      // TMR1 interrupt engedélyezve
	MOVLW      1
	MOVWF      PIE1+0
;szeg7ora3.c,119 :: 		}
L_end_interrupt:
L__interrupt87:
	MOVF       ___savePCLATH+0, 0
	MOVWF      PCLATH+0
	SWAPF      ___saveSTATUS+0, 0
	MOVWF      STATUS+0
	SWAPF      R15+0, 1
	SWAPF      R15+0, 0
	RETFIE
; end of _interrupt

_idoki7szegmre:

;szeg7ora3.c,124 :: 		void idoki7szegmre(char ora, char perc, char mperc, char utem, char vill) {
;szeg7ora3.c,129 :: 		szam1=ora/10;
	MOVLW      10
	MOVWF      R4+0
	MOVF       FARG_idoki7szegmre_ora+0, 0
	MOVWF      R0+0
	CALL       _Div_8x8_U+0
	MOVF       R0+0, 0
	MOVWF      idoki7szegmre_szam1_L0+0
;szeg7ora3.c,130 :: 		szam2=ora%10;
	MOVLW      10
	MOVWF      R4+0
	MOVF       FARG_idoki7szegmre_ora+0, 0
	MOVWF      R0+0
	CALL       _Div_8x8_U+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVF       R0+0, 0
	MOVWF      idoki7szegmre_szam2_L0+0
;szeg7ora3.c,131 :: 		szam3=perc/10;
	MOVLW      10
	MOVWF      R4+0
	MOVF       FARG_idoki7szegmre_perc+0, 0
	MOVWF      R0+0
	CALL       _Div_8x8_U+0
	MOVF       R0+0, 0
	MOVWF      idoki7szegmre_szam3_L0+0
;szeg7ora3.c,132 :: 		szam4=perc%10;
	MOVLW      10
	MOVWF      R4+0
	MOVF       FARG_idoki7szegmre_perc+0, 0
	MOVWF      R0+0
	CALL       _Div_8x8_U+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVF       R0+0, 0
	MOVWF      idoki7szegmre_szam4_L0+0
;szeg7ora3.c,133 :: 		pont=(utem&0x04)>>2;  // fél másodpercig 1-es, fél másodpercig 0-as, (1Hz)
	MOVLW      0
	BTFSC      FARG_idoki7szegmre_utem+0, 2
	MOVLW      1
	MOVWF      idoki7szegmre_pont_L0+0
;szeg7ora3.c,134 :: 		villog=(utem&0x02)>>1;  // negyed másodpercig 1-es, negyed  másodpercig 0-as, (2Hz)
	MOVLW      0
	BTFSC      FARG_idoki7szegmre_utem+0, 1
	MOVLW      1
	MOVWF      idoki7szegmre_villog_L0+0
;szeg7ora3.c,136 :: 		vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
	CLRF       PORTB+0
;szeg7ora3.c,137 :: 		if(vill==1) szegmens7=tomb7[szam1]*villog;        // ha ora_set -> villog
	MOVF       FARG_idoki7szegmre_vill+0, 0
	XORLW      1
	BTFSS      STATUS+0, 2
	GOTO       L_idoki7szegmre6
	MOVF       idoki7szegmre_szam1_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      R0+0
	MOVF       idoki7szegmre_villog_L0+0, 0
	MOVWF      R4+0
	CALL       _Mul_8x8_U+0
	MOVF       R0+0, 0
	MOVWF      PORTD+0
	GOTO       L_idoki7szegmre7
L_idoki7szegmre6:
;szeg7ora3.c,138 :: 		else szegmens7=tomb7[szam1];                //    egyebkent nem villog
	MOVF       idoki7szegmre_szam1_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      PORTD+0
L_idoki7szegmre7:
;szeg7ora3.c,139 :: 		vez_szegm=0b00000001;                //  T1  tranz. bekapcs. › 1. kijelzõ világít  ->  ora 1. szamjegy
	MOVLW      1
	MOVWF      PORTB+0
;szeg7ora3.c,140 :: 		Delay_ms(4);
	MOVLW      21
	MOVWF      R12+0
	MOVLW      198
	MOVWF      R13+0
L_idoki7szegmre8:
	DECFSZ     R13+0, 1
	GOTO       L_idoki7szegmre8
	DECFSZ     R12+0, 1
	GOTO       L_idoki7szegmre8
	NOP
;szeg7ora3.c,142 :: 		vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
	CLRF       PORTB+0
;szeg7ora3.c,143 :: 		if(vill==0) szegmens7=tomb7[szam2]|pont;                        // ha ora_jar -> nem villog  + pont
	MOVF       FARG_idoki7szegmre_vill+0, 0
	XORLW      0
	BTFSS      STATUS+0, 2
	GOTO       L_idoki7szegmre9
	MOVF       idoki7szegmre_szam2_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVF       idoki7szegmre_pont_L0+0, 0
	IORWF      INDF+0, 0
	MOVWF      PORTD+0
	GOTO       L_idoki7szegmre10
L_idoki7szegmre9:
;szeg7ora3.c,144 :: 		else if(vill==1) szegmens7=tomb7[szam2]*villog;  // ha ora_set -> villog
	MOVF       FARG_idoki7szegmre_vill+0, 0
	XORLW      1
	BTFSS      STATUS+0, 2
	GOTO       L_idoki7szegmre11
	MOVF       idoki7szegmre_szam2_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      R0+0
	MOVF       idoki7szegmre_villog_L0+0, 0
	MOVWF      R4+0
	CALL       _Mul_8x8_U+0
	MOVF       R0+0, 0
	MOVWF      PORTD+0
	GOTO       L_idoki7szegmre12
L_idoki7szegmre11:
;szeg7ora3.c,145 :: 		else szegmens7=tomb7[szam2];                //   egyébként (ha perc_set) nem villog
	MOVF       idoki7szegmre_szam2_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      PORTD+0
L_idoki7szegmre12:
L_idoki7szegmre10:
;szeg7ora3.c,146 :: 		vez_szegm=0b00000010;                //  T2  tranz. bekapcs. › 2. kijelzõ világít ->  ora 2. szamjegy
	MOVLW      2
	MOVWF      PORTB+0
;szeg7ora3.c,147 :: 		Delay_ms(4);
	MOVLW      21
	MOVWF      R12+0
	MOVLW      198
	MOVWF      R13+0
L_idoki7szegmre13:
	DECFSZ     R13+0, 1
	GOTO       L_idoki7szegmre13
	DECFSZ     R12+0, 1
	GOTO       L_idoki7szegmre13
	NOP
;szeg7ora3.c,149 :: 		vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
	CLRF       PORTB+0
;szeg7ora3.c,150 :: 		if(vill==2) szegmens7=tomb7[szam3]*villog;   // ha perc_set -> villog
	MOVF       FARG_idoki7szegmre_vill+0, 0
	XORLW      2
	BTFSS      STATUS+0, 2
	GOTO       L_idoki7szegmre14
	MOVF       idoki7szegmre_szam3_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      R0+0
	MOVF       idoki7szegmre_villog_L0+0, 0
	MOVWF      R4+0
	CALL       _Mul_8x8_U+0
	MOVF       R0+0, 0
	MOVWF      PORTD+0
	GOTO       L_idoki7szegmre15
L_idoki7szegmre14:
;szeg7ora3.c,151 :: 		else szegmens7=tomb7[szam3];                //  egyebkent nem villog
	MOVF       idoki7szegmre_szam3_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      PORTD+0
L_idoki7szegmre15:
;szeg7ora3.c,152 :: 		vez_szegm=0b00000100;                //  T3  tranz. bekapcs. › 3. kijelzõ világít   -> perc 1. szamjegy
	MOVLW      4
	MOVWF      PORTB+0
;szeg7ora3.c,153 :: 		Delay_ms(4);
	MOVLW      21
	MOVWF      R12+0
	MOVLW      198
	MOVWF      R13+0
L_idoki7szegmre16:
	DECFSZ     R13+0, 1
	GOTO       L_idoki7szegmre16
	DECFSZ     R12+0, 1
	GOTO       L_idoki7szegmre16
	NOP
;szeg7ora3.c,155 :: 		vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
	CLRF       PORTB+0
;szeg7ora3.c,156 :: 		if(vill==2) szegmens7=tomb7[szam4]*villog;    // ha perc_set -> villog
	MOVF       FARG_idoki7szegmre_vill+0, 0
	XORLW      2
	BTFSS      STATUS+0, 2
	GOTO       L_idoki7szegmre17
	MOVF       idoki7szegmre_szam4_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      R0+0
	MOVF       idoki7szegmre_villog_L0+0, 0
	MOVWF      R4+0
	CALL       _Mul_8x8_U+0
	MOVF       R0+0, 0
	MOVWF      PORTD+0
	GOTO       L_idoki7szegmre18
L_idoki7szegmre17:
;szeg7ora3.c,157 :: 		else szegmens7=tomb7[szam4];                //      egyebkent nem villog
	MOVF       idoki7szegmre_szam4_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      PORTD+0
L_idoki7szegmre18:
;szeg7ora3.c,158 :: 		vez_szegm=0b00001000;                //  T4  tranz. bekapcs. › 4. kijelzõ világít   -> perc 2. szamjegy
	MOVLW      8
	MOVWF      PORTB+0
;szeg7ora3.c,159 :: 		Delay_ms(4);
	MOVLW      21
	MOVWF      R12+0
	MOVLW      198
	MOVWF      R13+0
L_idoki7szegmre19:
	DECFSZ     R13+0, 1
	GOTO       L_idoki7szegmre19
	DECFSZ     R12+0, 1
	GOTO       L_idoki7szegmre19
	NOP
;szeg7ora3.c,162 :: 		}
L_end_idoki7szegmre:
	RETURN
; end of _idoki7szegmre

_hom7szegmre:

;szeg7ora3.c,168 :: 		void hom7szegmre(char elojel,int fok) {
;szeg7ora3.c,172 :: 		char szam1=0;                      // elsõ számjegy
	CLRF       hom7szegmre_szam1_L0+0
	CLRF       hom7szegmre_szam2_L0+0
	CLRF       hom7szegmre_szam3_L0+0
	CLRF       hom7szegmre_szam4_L0+0
;szeg7ora3.c,178 :: 		if(!elojel) {         // pozitív hõmérséklet
	MOVF       FARG_hom7szegmre_elojel+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L_hom7szegmre20
;szeg7ora3.c,179 :: 		szam4=fok%10;          // tizedesjegy
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVF       FARG_hom7szegmre_fok+0, 0
	MOVWF      R0+0
	MOVF       FARG_hom7szegmre_fok+1, 0
	MOVWF      R0+1
	CALL       _Div_16x16_S+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVF       R8+1, 0
	MOVWF      R0+1
	MOVF       R0+0, 0
	MOVWF      hom7szegmre_szam4_L0+0
;szeg7ora3.c,180 :: 		fok=fok/10;
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVF       FARG_hom7szegmre_fok+0, 0
	MOVWF      R0+0
	MOVF       FARG_hom7szegmre_fok+1, 0
	MOVWF      R0+1
	CALL       _Div_16x16_S+0
	MOVF       R0+0, 0
	MOVWF      FARG_hom7szegmre_fok+0
	MOVF       R0+1, 0
	MOVWF      FARG_hom7szegmre_fok+1
;szeg7ora3.c,181 :: 		szam3=fok%10;
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
	MOVWF      hom7szegmre_szam3_L0+0
;szeg7ora3.c,182 :: 		fok=fok/10;
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVF       FARG_hom7szegmre_fok+0, 0
	MOVWF      R0+0
	MOVF       FARG_hom7szegmre_fok+1, 0
	MOVWF      R0+1
	CALL       _Div_16x16_S+0
	MOVF       R0+0, 0
	MOVWF      FARG_hom7szegmre_fok+0
	MOVF       R0+1, 0
	MOVWF      FARG_hom7szegmre_fok+1
;szeg7ora3.c,183 :: 		szam2=fok%10;
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
	MOVWF      hom7szegmre_szam2_L0+0
;szeg7ora3.c,184 :: 		szam1=fok/10;
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVF       FARG_hom7szegmre_fok+0, 0
	MOVWF      R0+0
	MOVF       FARG_hom7szegmre_fok+1, 0
	MOVWF      R0+1
	CALL       _Div_16x16_S+0
	MOVF       R0+0, 0
	MOVWF      hom7szegmre_szam1_L0+0
;szeg7ora3.c,185 :: 		if(szam1==0)  { szam1=10; if(szam2==0) szam2=10; } // üres !!
	MOVF       R0+0, 0
	XORLW      0
	BTFSS      STATUS+0, 2
	GOTO       L_hom7szegmre21
	MOVLW      10
	MOVWF      hom7szegmre_szam1_L0+0
	MOVF       hom7szegmre_szam2_L0+0, 0
	XORLW      0
	BTFSS      STATUS+0, 2
	GOTO       L_hom7szegmre22
	MOVLW      10
	MOVWF      hom7szegmre_szam2_L0+0
L_hom7szegmre22:
L_hom7szegmre21:
;szeg7ora3.c,186 :: 		}
L_hom7szegmre20:
;szeg7ora3.c,188 :: 		if(elojel) {
	MOVF       FARG_hom7szegmre_elojel+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_hom7szegmre23
;szeg7ora3.c,189 :: 		szam4=fok%10;
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVF       FARG_hom7szegmre_fok+0, 0
	MOVWF      R0+0
	MOVF       FARG_hom7szegmre_fok+1, 0
	MOVWF      R0+1
	CALL       _Div_16x16_S+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVF       R8+1, 0
	MOVWF      R0+1
	MOVF       R0+0, 0
	MOVWF      hom7szegmre_szam4_L0+0
;szeg7ora3.c,190 :: 		fok=fok/10;
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVF       FARG_hom7szegmre_fok+0, 0
	MOVWF      R0+0
	MOVF       FARG_hom7szegmre_fok+1, 0
	MOVWF      R0+1
	CALL       _Div_16x16_S+0
	MOVF       R0+0, 0
	MOVWF      FARG_hom7szegmre_fok+0
	MOVF       R0+1, 0
	MOVWF      FARG_hom7szegmre_fok+1
;szeg7ora3.c,191 :: 		szam3=fok%10;
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
	MOVWF      hom7szegmre_szam3_L0+0
;szeg7ora3.c,192 :: 		szam2=fok/10;
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVF       FARG_hom7szegmre_fok+0, 0
	MOVWF      R0+0
	MOVF       FARG_hom7szegmre_fok+1, 0
	MOVWF      R0+1
	CALL       _Div_16x16_S+0
	MOVF       R0+0, 0
	MOVWF      hom7szegmre_szam2_L0+0
;szeg7ora3.c,193 :: 		szam1=11;     //  '-'
	MOVLW      11
	MOVWF      hom7szegmre_szam1_L0+0
;szeg7ora3.c,195 :: 		}
L_hom7szegmre23:
;szeg7ora3.c,197 :: 		vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
	CLRF       PORTB+0
;szeg7ora3.c,198 :: 		szegmens7=tomb7[szam1];                //
	MOVF       hom7szegmre_szam1_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      PORTD+0
;szeg7ora3.c,199 :: 		vez_szegm=0b00000001;                //  T1  tranz. bekapcs. -> 1. kijelzõ világít
	MOVLW      1
	MOVWF      PORTB+0
;szeg7ora3.c,200 :: 		Delay_ms(4);
	MOVLW      21
	MOVWF      R12+0
	MOVLW      198
	MOVWF      R13+0
L_hom7szegmre24:
	DECFSZ     R13+0, 1
	GOTO       L_hom7szegmre24
	DECFSZ     R12+0, 1
	GOTO       L_hom7szegmre24
	NOP
;szeg7ora3.c,202 :: 		vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
	CLRF       PORTB+0
;szeg7ora3.c,203 :: 		szegmens7=tomb7[szam2];                //
	MOVF       hom7szegmre_szam2_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      PORTD+0
;szeg7ora3.c,204 :: 		vez_szegm=0b00000010;                //  T2  tranz. bekapcs. -> 2. kijelzõ világít
	MOVLW      2
	MOVWF      PORTB+0
;szeg7ora3.c,205 :: 		Delay_ms(4);
	MOVLW      21
	MOVWF      R12+0
	MOVLW      198
	MOVWF      R13+0
L_hom7szegmre25:
	DECFSZ     R13+0, 1
	GOTO       L_hom7szegmre25
	DECFSZ     R12+0, 1
	GOTO       L_hom7szegmre25
	NOP
;szeg7ora3.c,207 :: 		vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
	CLRF       PORTB+0
;szeg7ora3.c,208 :: 		szegmens7=tomb7[szam3]|0b00000001;                //  tizedespont
	MOVF       hom7szegmre_szam3_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVLW      1
	IORWF      INDF+0, 0
	MOVWF      PORTD+0
;szeg7ora3.c,209 :: 		vez_szegm=0b00000100;                //  T3  tranz. bekapcs. -> 3. kijelzõ világít
	MOVLW      4
	MOVWF      PORTB+0
;szeg7ora3.c,210 :: 		Delay_ms(4);
	MOVLW      21
	MOVWF      R12+0
	MOVLW      198
	MOVWF      R13+0
L_hom7szegmre26:
	DECFSZ     R13+0, 1
	GOTO       L_hom7szegmre26
	DECFSZ     R12+0, 1
	GOTO       L_hom7szegmre26
	NOP
;szeg7ora3.c,212 :: 		vez_szegm=0b00000000;                //  mindegyik tranzisztor kikapcsolása
	CLRF       PORTB+0
;szeg7ora3.c,213 :: 		szegmens7=tomb7[szam4];                //
	MOVF       hom7szegmre_szam4_L0+0, 0
	ADDLW      _tomb7+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      PORTD+0
;szeg7ora3.c,214 :: 		vez_szegm=0b00001000;                //  T4  tranz. bekapcs. -> 4. kijelzõ világít
	MOVLW      8
	MOVWF      PORTB+0
;szeg7ora3.c,215 :: 		Delay_ms(4);
	MOVLW      21
	MOVWF      R12+0
	MOVLW      198
	MOVWF      R13+0
L_hom7szegmre27:
	DECFSZ     R13+0, 1
	GOTO       L_hom7szegmre27
	DECFSZ     R12+0, 1
	GOTO       L_hom7szegmre27
	NOP
;szeg7ora3.c,217 :: 		}
L_end_hom7szegmre:
	RETURN
; end of _hom7szegmre

_ntc_lekerdez:

;szeg7ora3.c,222 :: 		void ntc_lekerdez() {
;szeg7ora3.c,224 :: 		char i=0;
	CLRF       ntc_lekerdez_i_L0+0
	CLRF       ntc_lekerdez_negativ_L0+0
	CLRF       ntc_lekerdez_poti_L0+0
	CLRF       ntc_lekerdez_poti_L0+1
;szeg7ora3.c,232 :: 		ADCON0=ADCON0|2;  // GO bit -->1   A/D indítása
	BSF        ADCON0+0, 1
;szeg7ora3.c,233 :: 		while((ADCON0&2)!=0){   // várakozás A/D eredményre  amikor DONE=0 lesz
L_ntc_lekerdez28:
	MOVLW      2
	ANDWF      ADCON0+0, 0
	MOVWF      R1+0
	MOVF       R1+0, 0
	XORLW      0
	BTFSC      STATUS+0, 2
	GOTO       L_ntc_lekerdez29
;szeg7ora3.c,234 :: 		asm nop;
	NOP
;szeg7ora3.c,235 :: 		}
	GOTO       L_ntc_lekerdez28
L_ntc_lekerdez29:
;szeg7ora3.c,236 :: 		poti=ADRESH&3;
	MOVLW      3
	ANDWF      ADRESH+0, 0
	MOVWF      ntc_lekerdez_poti_L0+0
	CLRF       ntc_lekerdez_poti_L0+1
	MOVLW      0
	ANDWF      ntc_lekerdez_poti_L0+1, 1
	MOVLW      0
	MOVWF      ntc_lekerdez_poti_L0+1
;szeg7ora3.c,237 :: 		poti=(poti<<8)+ADRESL;     // eredmény:  ADRESH alsó két bit + ADRESL
	MOVF       ntc_lekerdez_poti_L0+0, 0
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
	MOVWF      ntc_lekerdez_poti_L0+0
	MOVF       R2+1, 0
	MOVWF      ntc_lekerdez_poti_L0+1
;szeg7ora3.c,239 :: 		if(poti<0) poti=0;
	MOVLW      128
	XORWF      R2+1, 0
	MOVWF      R0+0
	MOVLW      128
	SUBWF      R0+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L__ntc_lekerdez91
	MOVLW      0
	SUBWF      R2+0, 0
L__ntc_lekerdez91:
	BTFSC      STATUS+0, 0
	GOTO       L_ntc_lekerdez30
	CLRF       ntc_lekerdez_poti_L0+0
	CLRF       ntc_lekerdez_poti_L0+1
L_ntc_lekerdez30:
;szeg7ora3.c,240 :: 		if(poti>1023) poti=1023;
	MOVLW      128
	XORLW      3
	MOVWF      R0+0
	MOVLW      128
	XORWF      ntc_lekerdez_poti_L0+1, 0
	SUBWF      R0+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L__ntc_lekerdez92
	MOVF       ntc_lekerdez_poti_L0+0, 0
	SUBLW      255
L__ntc_lekerdez92:
	BTFSC      STATUS+0, 0
	GOTO       L_ntc_lekerdez31
	MOVLW      255
	MOVWF      ntc_lekerdez_poti_L0+0
	MOVLW      3
	MOVWF      ntc_lekerdez_poti_L0+1
L_ntc_lekerdez31:
;szeg7ora3.c,241 :: 		i=0;
	CLRF       ntc_lekerdez_i_L0+0
;szeg7ora3.c,242 :: 		while(poti>fesz[i]) i++;
L_ntc_lekerdez32:
	MOVF       ntc_lekerdez_i_L0+0, 0
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
	XORWF      R2+1, 0
	MOVWF      R0+0
	MOVLW      128
	XORWF      ntc_lekerdez_poti_L0+1, 0
	SUBWF      R0+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L__ntc_lekerdez93
	MOVF       ntc_lekerdez_poti_L0+0, 0
	SUBWF      R2+0, 0
L__ntc_lekerdez93:
	BTFSC      STATUS+0, 0
	GOTO       L_ntc_lekerdez33
	INCF       ntc_lekerdez_i_L0+0, 1
	GOTO       L_ntc_lekerdez32
L_ntc_lekerdez33:
;szeg7ora3.c,243 :: 		fok=100*foktar[i];               //  -65  -----    +150
	MOVF       ntc_lekerdez_i_L0+0, 0
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
	MOVWF      ntc_lekerdez_fok_L0+0
	MOVF       R0+1, 0
	MOVWF      ntc_lekerdez_fok_L0+1
	MOVLW      0
	BTFSC      ntc_lekerdez_fok_L0+1, 7
	MOVLW      255
	MOVWF      ntc_lekerdez_fok_L0+2
	MOVWF      ntc_lekerdez_fok_L0+3
;szeg7ora3.c,244 :: 		if(i>1 && i<34) fok=fok-500*(fesz[i]-poti)/(fesz[i]-fesz[i-1]);
	MOVF       ntc_lekerdez_i_L0+0, 0
	SUBLW      1
	BTFSC      STATUS+0, 0
	GOTO       L_ntc_lekerdez36
	MOVLW      34
	SUBWF      ntc_lekerdez_i_L0+0, 0
	BTFSC      STATUS+0, 0
	GOTO       L_ntc_lekerdez36
L__ntc_lekerdez79:
	MOVF       ntc_lekerdez_i_L0+0, 0
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
	MOVWF      FLOC__ntc_lekerdez+0
	INCF       ___DoICPAddr+0, 1
	BTFSC      STATUS+0, 2
	INCF       ___DoICPAddr+1, 1
	CALL       _____DoICP+0
	MOVWF      FLOC__ntc_lekerdez+1
	MOVF       ntc_lekerdez_poti_L0+0, 0
	SUBWF      FLOC__ntc_lekerdez+0, 0
	MOVWF      R0+0
	MOVF       ntc_lekerdez_poti_L0+1, 0
	BTFSS      STATUS+0, 0
	ADDLW      1
	SUBWF      FLOC__ntc_lekerdez+1, 0
	MOVWF      R0+1
	MOVLW      244
	MOVWF      R4+0
	MOVLW      1
	MOVWF      R4+1
	CALL       _Mul_16x16_U+0
	MOVLW      1
	SUBWF      ntc_lekerdez_i_L0+0, 0
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
	MOVF       R2+0, 0
	SUBWF      FLOC__ntc_lekerdez+0, 0
	MOVWF      R4+0
	MOVF       R2+1, 0
	BTFSS      STATUS+0, 0
	ADDLW      1
	SUBWF      FLOC__ntc_lekerdez+1, 0
	MOVWF      R4+1
	CALL       _Div_16x16_S+0
	MOVLW      0
	BTFSC      R0+1, 7
	MOVLW      255
	MOVWF      R0+2
	MOVWF      R0+3
	MOVF       R0+0, 0
	SUBWF      ntc_lekerdez_fok_L0+0, 1
	MOVF       R0+1, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+1, 0
	SUBWF      ntc_lekerdez_fok_L0+1, 1
	MOVF       R0+2, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+2, 0
	SUBWF      ntc_lekerdez_fok_L0+2, 1
	MOVF       R0+3, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+3, 0
	SUBWF      ntc_lekerdez_fok_L0+3, 1
L_ntc_lekerdez36:
;szeg7ora3.c,245 :: 		if(i==34) fok=fok-4000*(fesz[i]-poti)/(fesz[i]-fesz[i-1]);
	MOVF       ntc_lekerdez_i_L0+0, 0
	XORLW      34
	BTFSS      STATUS+0, 2
	GOTO       L_ntc_lekerdez37
	MOVF       ntc_lekerdez_i_L0+0, 0
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
	MOVWF      FLOC__ntc_lekerdez+0
	INCF       ___DoICPAddr+0, 1
	BTFSC      STATUS+0, 2
	INCF       ___DoICPAddr+1, 1
	CALL       _____DoICP+0
	MOVWF      FLOC__ntc_lekerdez+1
	MOVF       ntc_lekerdez_poti_L0+0, 0
	SUBWF      FLOC__ntc_lekerdez+0, 0
	MOVWF      R0+0
	MOVF       ntc_lekerdez_poti_L0+1, 0
	BTFSS      STATUS+0, 0
	ADDLW      1
	SUBWF      FLOC__ntc_lekerdez+1, 0
	MOVWF      R0+1
	MOVLW      160
	MOVWF      R4+0
	MOVLW      15
	MOVWF      R4+1
	CALL       _Mul_16x16_U+0
	MOVLW      1
	SUBWF      ntc_lekerdez_i_L0+0, 0
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
	MOVF       R2+0, 0
	SUBWF      FLOC__ntc_lekerdez+0, 0
	MOVWF      R4+0
	MOVF       R2+1, 0
	BTFSS      STATUS+0, 0
	ADDLW      1
	SUBWF      FLOC__ntc_lekerdez+1, 0
	MOVWF      R4+1
	CALL       _Div_16x16_S+0
	MOVLW      0
	BTFSC      R0+1, 7
	MOVLW      255
	MOVWF      R0+2
	MOVWF      R0+3
	MOVF       R0+0, 0
	SUBWF      ntc_lekerdez_fok_L0+0, 1
	MOVF       R0+1, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+1, 0
	SUBWF      ntc_lekerdez_fok_L0+1, 1
	MOVF       R0+2, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+2, 0
	SUBWF      ntc_lekerdez_fok_L0+2, 1
	MOVF       R0+3, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+3, 0
	SUBWF      ntc_lekerdez_fok_L0+3, 1
L_ntc_lekerdez37:
;szeg7ora3.c,246 :: 		if(i==1) fok=fok-1500*(fesz[i]-poti)/(fesz[i]-fesz[i-1]);
	MOVF       ntc_lekerdez_i_L0+0, 0
	XORLW      1
	BTFSS      STATUS+0, 2
	GOTO       L_ntc_lekerdez38
	MOVF       ntc_lekerdez_i_L0+0, 0
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
	MOVWF      FLOC__ntc_lekerdez+0
	INCF       ___DoICPAddr+0, 1
	BTFSC      STATUS+0, 2
	INCF       ___DoICPAddr+1, 1
	CALL       _____DoICP+0
	MOVWF      FLOC__ntc_lekerdez+1
	MOVF       ntc_lekerdez_poti_L0+0, 0
	SUBWF      FLOC__ntc_lekerdez+0, 0
	MOVWF      R0+0
	MOVF       ntc_lekerdez_poti_L0+1, 0
	BTFSS      STATUS+0, 0
	ADDLW      1
	SUBWF      FLOC__ntc_lekerdez+1, 0
	MOVWF      R0+1
	MOVLW      220
	MOVWF      R4+0
	MOVLW      5
	MOVWF      R4+1
	CALL       _Mul_16x16_U+0
	MOVLW      1
	SUBWF      ntc_lekerdez_i_L0+0, 0
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
	MOVF       R2+0, 0
	SUBWF      FLOC__ntc_lekerdez+0, 0
	MOVWF      R4+0
	MOVF       R2+1, 0
	BTFSS      STATUS+0, 0
	ADDLW      1
	SUBWF      FLOC__ntc_lekerdez+1, 0
	MOVWF      R4+1
	CALL       _Div_16x16_S+0
	MOVLW      0
	BTFSC      R0+1, 7
	MOVLW      255
	MOVWF      R0+2
	MOVWF      R0+3
	MOVF       R0+0, 0
	SUBWF      ntc_lekerdez_fok_L0+0, 1
	MOVF       R0+1, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+1, 0
	SUBWF      ntc_lekerdez_fok_L0+1, 1
	MOVF       R0+2, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+2, 0
	SUBWF      ntc_lekerdez_fok_L0+2, 1
	MOVF       R0+3, 0
	BTFSS      STATUS+0, 0
	INCFSZ     R0+3, 0
	SUBWF      ntc_lekerdez_fok_L0+3, 1
L_ntc_lekerdez38:
;szeg7ora3.c,247 :: 		if(i==0) fok=-6500;
	MOVF       ntc_lekerdez_i_L0+0, 0
	XORLW      0
	BTFSS      STATUS+0, 2
	GOTO       L_ntc_lekerdez39
	MOVLW      156
	MOVWF      ntc_lekerdez_fok_L0+0
	MOVLW      230
	MOVWF      ntc_lekerdez_fok_L0+1
	MOVLW      255
	MOVWF      ntc_lekerdez_fok_L0+2
	MOVWF      ntc_lekerdez_fok_L0+3
L_ntc_lekerdez39:
;szeg7ora3.c,248 :: 		if(fok<0) { negativ=1; fok=-fok;  }     // elõjel beállítása
	MOVLW      128
	XORWF      ntc_lekerdez_fok_L0+3, 0
	MOVWF      R0+0
	MOVLW      128
	SUBWF      R0+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L__ntc_lekerdez94
	MOVLW      0
	SUBWF      ntc_lekerdez_fok_L0+2, 0
	BTFSS      STATUS+0, 2
	GOTO       L__ntc_lekerdez94
	MOVLW      0
	SUBWF      ntc_lekerdez_fok_L0+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__ntc_lekerdez94
	MOVLW      0
	SUBWF      ntc_lekerdez_fok_L0+0, 0
L__ntc_lekerdez94:
	BTFSC      STATUS+0, 0
	GOTO       L_ntc_lekerdez40
	MOVLW      1
	MOVWF      ntc_lekerdez_negativ_L0+0
	CLRF       R0+0
	CLRF       R0+1
	CLRF       R0+2
	CLRF       R0+3
	MOVF       ntc_lekerdez_fok_L0+0, 0
	SUBWF      R0+0, 1
	MOVF       ntc_lekerdez_fok_L0+1, 0
	BTFSS      STATUS+0, 0
	INCFSZ     ntc_lekerdez_fok_L0+1, 0
	SUBWF      R0+1, 1
	MOVF       ntc_lekerdez_fok_L0+2, 0
	BTFSS      STATUS+0, 0
	INCFSZ     ntc_lekerdez_fok_L0+2, 0
	SUBWF      R0+2, 1
	MOVF       ntc_lekerdez_fok_L0+3, 0
	BTFSS      STATUS+0, 0
	INCFSZ     ntc_lekerdez_fok_L0+3, 0
	SUBWF      R0+3, 1
	MOVF       R0+0, 0
	MOVWF      ntc_lekerdez_fok_L0+0
	MOVF       R0+1, 0
	MOVWF      ntc_lekerdez_fok_L0+1
	MOVF       R0+2, 0
	MOVWF      ntc_lekerdez_fok_L0+2
	MOVF       R0+3, 0
	MOVWF      ntc_lekerdez_fok_L0+3
L_ntc_lekerdez40:
;szeg7ora3.c,249 :: 		if(fok%10>5)   gfok=fok/10+1;
	MOVLW      10
	MOVWF      R4+0
	CLRF       R4+1
	CLRF       R4+2
	CLRF       R4+3
	MOVF       ntc_lekerdez_fok_L0+0, 0
	MOVWF      R0+0
	MOVF       ntc_lekerdez_fok_L0+1, 0
	MOVWF      R0+1
	MOVF       ntc_lekerdez_fok_L0+2, 0
	MOVWF      R0+2
	MOVF       ntc_lekerdez_fok_L0+3, 0
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
	GOTO       L__ntc_lekerdez95
	MOVF       R0+2, 0
	SUBLW      0
	BTFSS      STATUS+0, 2
	GOTO       L__ntc_lekerdez95
	MOVF       R0+1, 0
	SUBLW      0
	BTFSS      STATUS+0, 2
	GOTO       L__ntc_lekerdez95
	MOVF       R0+0, 0
	SUBLW      5
L__ntc_lekerdez95:
	BTFSC      STATUS+0, 0
	GOTO       L_ntc_lekerdez41
	MOVLW      10
	MOVWF      R4+0
	CLRF       R4+1
	CLRF       R4+2
	CLRF       R4+3
	MOVF       ntc_lekerdez_fok_L0+0, 0
	MOVWF      R0+0
	MOVF       ntc_lekerdez_fok_L0+1, 0
	MOVWF      R0+1
	MOVF       ntc_lekerdez_fok_L0+2, 0
	MOVWF      R0+2
	MOVF       ntc_lekerdez_fok_L0+3, 0
	MOVWF      R0+3
	CALL       _Div_32x32_S+0
	MOVF       R0+0, 0
	ADDLW      1
	MOVWF      _gfok+0
	MOVLW      0
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      R0+1, 0
	MOVWF      _gfok+1
	GOTO       L_ntc_lekerdez42
L_ntc_lekerdez41:
;szeg7ora3.c,250 :: 		else   gfok=fok/10;     // kerekítés egy tizedes jegyre
	MOVLW      10
	MOVWF      R4+0
	CLRF       R4+1
	CLRF       R4+2
	CLRF       R4+3
	MOVF       ntc_lekerdez_fok_L0+0, 0
	MOVWF      R0+0
	MOVF       ntc_lekerdez_fok_L0+1, 0
	MOVWF      R0+1
	MOVF       ntc_lekerdez_fok_L0+2, 0
	MOVWF      R0+2
	MOVF       ntc_lekerdez_fok_L0+3, 0
	MOVWF      R0+3
	CALL       _Div_32x32_S+0
	MOVF       R0+0, 0
	MOVWF      _gfok+0
	MOVF       R0+1, 0
	MOVWF      _gfok+1
L_ntc_lekerdez42:
;szeg7ora3.c,251 :: 		gelojel=negativ;
	MOVF       ntc_lekerdez_negativ_L0+0, 0
	MOVWF      _gelojel+0
;szeg7ora3.c,253 :: 		}
L_end_ntc_lekerdez:
	RETURN
; end of _ntc_lekerdez

_main:

;szeg7ora3.c,257 :: 		void main() {
;szeg7ora3.c,259 :: 		char i=0;
;szeg7ora3.c,260 :: 		char ciklus=0;
	CLRF       main_ciklus_L0+0
;szeg7ora3.c,262 :: 		kezdo_beall();
	CALL       _kezdo_beall+0
;szeg7ora3.c,263 :: 		Delay_ms(15);
	MOVLW      78
	MOVWF      R12+0
	MOVLW      235
	MOVWF      R13+0
L_main43:
	DECFSZ     R13+0, 1
	GOTO       L_main43
	DECFSZ     R12+0, 1
	GOTO       L_main43
;szeg7ora3.c,265 :: 		while(1){         //  fõ ciklus !!
L_main44:
;szeg7ora3.c,267 :: 		if (!homero) {        //   óra kijelzés
	MOVF       _homero+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L_main46
;szeg7ora3.c,268 :: 		if(ora_jar) idoki7szegmre(gora, gperc, gmasperc, szamlal, 0);
	MOVF       _ora_jar+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_main47
	MOVF       _gora+0, 0
	MOVWF      FARG_idoki7szegmre_ora+0
	MOVF       _gperc+0, 0
	MOVWF      FARG_idoki7szegmre_perc+0
	MOVF       _gmasperc+0, 0
	MOVWF      FARG_idoki7szegmre_mperc+0
	MOVF       _szamlal+0, 0
	MOVWF      FARG_idoki7szegmre_utem+0
	CLRF       FARG_idoki7szegmre_vill+0
	CALL       _idoki7szegmre+0
L_main47:
;szeg7ora3.c,269 :: 		if(ora_set) idoki7szegmre(gora, gperc, gmasperc, szamlal, 1);  // óra villogtatva
	MOVF       _ora_set+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_main48
	MOVF       _gora+0, 0
	MOVWF      FARG_idoki7szegmre_ora+0
	MOVF       _gperc+0, 0
	MOVWF      FARG_idoki7szegmre_perc+0
	MOVF       _gmasperc+0, 0
	MOVWF      FARG_idoki7szegmre_mperc+0
	MOVF       _szamlal+0, 0
	MOVWF      FARG_idoki7szegmre_utem+0
	MOVLW      1
	MOVWF      FARG_idoki7szegmre_vill+0
	CALL       _idoki7szegmre+0
L_main48:
;szeg7ora3.c,270 :: 		if(perc_set) idoki7szegmre(gora, gperc, gmasperc, szamlal, 2);  // perc  villogtatva
	MOVF       _perc_set+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_main49
	MOVF       _gora+0, 0
	MOVWF      FARG_idoki7szegmre_ora+0
	MOVF       _gperc+0, 0
	MOVWF      FARG_idoki7szegmre_perc+0
	MOVF       _gmasperc+0, 0
	MOVWF      FARG_idoki7szegmre_mperc+0
	MOVF       _szamlal+0, 0
	MOVWF      FARG_idoki7szegmre_utem+0
	MOVLW      2
	MOVWF      FARG_idoki7szegmre_vill+0
	CALL       _idoki7szegmre+0
L_main49:
;szeg7ora3.c,271 :: 		}
L_main46:
;szeg7ora3.c,272 :: 		if(homero) {            //  hõmérõ kijelzés
	MOVF       _homero+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_main50
;szeg7ora3.c,273 :: 		if(ciklus%15==0) ntc_lekerdez();               // hõmérséklet lekérdezése
	MOVLW      15
	MOVWF      R4+0
	MOVF       main_ciklus_L0+0, 0
	MOVWF      R0+0
	CALL       _Div_8x8_U+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVF       R0+0, 0
	XORLW      0
	BTFSS      STATUS+0, 2
	GOTO       L_main51
	CALL       _ntc_lekerdez+0
L_main51:
;szeg7ora3.c,274 :: 		hom7szegmre(gelojel, gfok);
	MOVF       _gelojel+0, 0
	MOVWF      FARG_hom7szegmre_elojel+0
	MOVF       _gfok+0, 0
	MOVWF      FARG_hom7szegmre_fok+0
	MOVF       _gfok+1, 0
	MOVWF      FARG_hom7szegmre_fok+1
	CALL       _hom7szegmre+0
;szeg7ora3.c,275 :: 		}
L_main50:
;szeg7ora3.c,277 :: 		if(kapcs_jobb==0)       //jobb ---> perc beállítása
	BTFSC      PORTA+0, 2
	GOTO       L_main52
;szeg7ora3.c,278 :: 		{ perc_set=1; ora_jar=0; ora_set=0; homero=0; }
	MOVLW      1
	MOVWF      _perc_set+0
	CLRF       _ora_jar+0
	CLRF       _ora_set+0
	CLRF       _homero+0
L_main52:
;szeg7ora3.c,279 :: 		if(kapcs_bal==0)        //bal  -->  óra beállítása
	BTFSC      PORTA+0, 1
	GOTO       L_main53
;szeg7ora3.c,280 :: 		{ ora_set=1; ora_jar=0; perc_set=0; homero=0; }
	MOVLW      1
	MOVWF      _ora_set+0
	CLRF       _ora_jar+0
	CLRF       _perc_set+0
	CLRF       _homero+0
L_main53:
;szeg7ora3.c,281 :: 		if(!ora_jar&&(kapcs_stop==0))        //stop ---> normál üzemmód,  jár az óra
	MOVF       _ora_jar+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L_main56
	BTFSC      PORTA+0, 5
	GOTO       L_main56
L__main84:
;szeg7ora3.c,282 :: 		{INTCON=0b00000000; ora_jar=1; ora_set=0; perc_set=0; szamlal=0; gmasperc=0;
	CLRF       INTCON+0
	MOVLW      1
	MOVWF      _ora_jar+0
	CLRF       _ora_set+0
	CLRF       _perc_set+0
	CLRF       _szamlal+0
	CLRF       _gmasperc+0
;szeg7ora3.c,283 :: 		TMR1H=0b00001011; TMR1L=0b11011100; INTCON=0b11000000; // TIMER1 megszakitás újra indít
	MOVLW      11
	MOVWF      TMR1H+0
	MOVLW      220
	MOVWF      TMR1L+0
	MOVLW      192
	MOVWF      INTCON+0
;szeg7ora3.c,284 :: 		}
L_main56:
;szeg7ora3.c,285 :: 		if((ciklus%25==0)&&ora_set) {                               // óra érték beállítása
	MOVLW      25
	MOVWF      R4+0
	MOVF       main_ciklus_L0+0, 0
	MOVWF      R0+0
	CALL       _Div_8x8_U+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVF       R0+0, 0
	XORLW      0
	BTFSS      STATUS+0, 2
	GOTO       L_main59
	MOVF       _ora_set+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_main59
L__main83:
;szeg7ora3.c,286 :: 		if(kapcs_fel==0) {gora++; if(gora>23) gora=0;  }        // fel  ---> növelés
	BTFSC      PORTA+0, 0
	GOTO       L_main60
	INCF       _gora+0, 1
	MOVF       _gora+0, 0
	SUBLW      23
	BTFSC      STATUS+0, 0
	GOTO       L_main61
	CLRF       _gora+0
L_main61:
L_main60:
;szeg7ora3.c,287 :: 		if(kapcs_le==0)  {if(gora>1) gora--; else gora=23;    }    // le ---> csökkentés
	BTFSC      PORTA+0, 3
	GOTO       L_main62
	MOVF       _gora+0, 0
	SUBLW      1
	BTFSC      STATUS+0, 0
	GOTO       L_main63
	DECF       _gora+0, 1
	GOTO       L_main64
L_main63:
	MOVLW      23
	MOVWF      _gora+0
L_main64:
L_main62:
;szeg7ora3.c,288 :: 		}
L_main59:
;szeg7ora3.c,289 :: 		if((ciklus%25==0)&&perc_set) {                               // perc érték beállítása
	MOVLW      25
	MOVWF      R4+0
	MOVF       main_ciklus_L0+0, 0
	MOVWF      R0+0
	CALL       _Div_8x8_U+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVF       R0+0, 0
	XORLW      0
	BTFSS      STATUS+0, 2
	GOTO       L_main67
	MOVF       _perc_set+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_main67
L__main82:
;szeg7ora3.c,290 :: 		if(kapcs_fel==0) {gperc++; if(gperc>59) gperc=0;  }          // fel  ---> növelés
	BTFSC      PORTA+0, 0
	GOTO       L_main68
	INCF       _gperc+0, 1
	MOVF       _gperc+0, 0
	SUBLW      59
	BTFSC      STATUS+0, 0
	GOTO       L_main69
	CLRF       _gperc+0
L_main69:
L_main68:
;szeg7ora3.c,291 :: 		if(kapcs_le==0)  {if(gperc>1) gperc--; else gperc=59;    }    // le ---> csökkentés
	BTFSC      PORTA+0, 3
	GOTO       L_main70
	MOVF       _gperc+0, 0
	SUBLW      1
	BTFSC      STATUS+0, 0
	GOTO       L_main71
	DECF       _gperc+0, 1
	GOTO       L_main72
L_main71:
	MOVLW      59
	MOVWF      _gperc+0
L_main72:
L_main70:
;szeg7ora3.c,292 :: 		}
L_main67:
;szeg7ora3.c,293 :: 		if(ora_jar&&(kapcs_le==0))  homero=1;      //  le ---> hõmérõ kijelzés
	MOVF       _ora_jar+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_main75
	BTFSC      PORTA+0, 3
	GOTO       L_main75
L__main81:
	MOVLW      1
	MOVWF      _homero+0
L_main75:
;szeg7ora3.c,295 :: 		if(homero&&(kapcs_fel==0))  homero=0;      //  fel ---> óra kijelzés
	MOVF       _homero+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_main78
	BTFSC      PORTA+0, 0
	GOTO       L_main78
L__main80:
	CLRF       _homero+0
L_main78:
;szeg7ora3.c,297 :: 		ciklus++;
	INCF       main_ciklus_L0+0, 1
;szeg7ora3.c,300 :: 		}
	GOTO       L_main44
;szeg7ora3.c,302 :: 		}
L_end_main:
	GOTO       $+0
; end of _main
