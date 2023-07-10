//  DC motor vezérlés,  L293B motormeghajtó IC-vel

// #include <arduino.h> 
// #include "motor34.h" 

   /*    ATmega328  lábkiosztas, Arduino Uno3 számozás
     
                              +--------------+
                     RESET   -|1        PC5 28|-  ADC5	Arduino A5
    Ard. D0             RXD  -|2 PD0    PC4 27|-  ADC4  Ard. A4
    Ard. D1             TXD  -|3 PD1    PC3 26|-  ADC3  Ard. A3
    Ard. D2                  -|4 PD2    PC2 25|-  ADC2  Ard. A2
	Ard. D3     	PWM (OC2B)   -|5 PD3    PC1 24|-  ADC1  Ard. A1
	Ard. D4 			          	 -|6 PD4    PC0 23|-  ADC0  Ard. A0
                       VCC   -|7 			22|- GND
                      GND    -|8 			21|- AREF
                     (xtal)  -|9 PB6        20|- AVCC
                     (xtal)  -|10 PB7   PB5 19|- ISP(SCK)      Ard. D13
    Ard. D5       PWM (OC0B) -|11 PD5   PB4 18|- ISP(MISO)     Ard. D12
    Ard. D6       PWM (OC0A) -|12 PD6   PB3 17|- ISP(MOSI),  PWM (OC2A)  Ard. D11
    Ard. D7                  -|13 PD7   PB2 16|-  PWM (OC1B)   Ard. D10
    Ard. D8                  -|14 PB0   PB1 15|-  PWM (OC1A)   Ard. D9
                               +--------------+
     

    */
    //---------------------------------------------------------------------
 
//  DC motor vezérlés,  L293B motormeghajtó IC-vel  
// Hobbi elektronika:  WILL-I - Robot, AVR mikrovezérlővel   alapján
    

/*
 * created by Rui Santos, https://randomnerdtutorials.com
 * 
 * Complete Guide for Ultrasonic Sensor HC-SR04
 *
    Ultrasonic sensor Pins:
        VCC: +5VDC
        Trig : Trigger (INPUT) - Pin11
        Echo: Echo (OUTPUT) - Pin 12
        GND: GND

distance = (traveltime/2) x speed of sound
The speed of sound is: 343m/s = 0.0343 cm/uS = 1/29.1 cm/uS
Or in inches: 13503.9in/s = 0.0135in/uS = 1/74in/uS
 */



 
   
#include <avr/io.h>
//#include <arduino.h>      

#define Mot1pwm		9		// motor1 (bal)  PWM  PB1
#define Mot2pwm		10		// motor2 (jobb)  PWM  PB2
#define Mot1A		5     // Motor1A = PD5
#define Mot1B		6     // Motor1B = PD6
#define Mot2A		7     // Motor2A = PD7
#define Mot2B		8     // Motor2B = PD8

#define VoJSZ A0     // vonalkövető, jobb szélső
#define VoJK A1     // vonalkövető, jobb közép
#define VoBK A2     // vonalkövető, bal közép
#define VoBSZ A3     // vonalkövető, bal szélső

#define  UH1_trig   11    // Trigger
#define  UH1_echo   12    // Echo

#define Start  4    // Start, PD4   0-át kapcsol !!

unsigned int sebes=255;   //sebesség  0-255
     
byte mozgas=0;  // áll



//*********************************************************************
     
void motor_init()         //Timer1 PWM beallitasa
{
     TCCR1A |= (1<<WGM10);     //8 bites fazishelyes PWM
     TCCR1B |= (1<<CS10);         //elooszto = FCPU/1
     pinMode(Mot1pwm, OUTPUT);	// 9-es digit. láb kimenet
     pinMode(Mot2pwm, OUTPUT);	// 10-es digit. láb kimenet

     pinMode(Mot1A, OUTPUT);  // 2-es digit. láb kimenet
     pinMode(Mot1B, OUTPUT);  // 4-es digit. láb kimenet
     pinMode(Mot2A, OUTPUT);  // 7-es digit. láb kimenet
     pinMode(Mot2B, OUTPUT);  // 8-as digit. láb kimenet
     
}
    

//*********************************************************************   

void szenzor_init()         //szenzorok beallitasa
{
    
      //Define inputs and outputs
    pinMode(UH1_trig, OUTPUT);
    pinMode(UH1_echo, INPUT);
    
    pinMode(VoJSZ, INPUT);
    pinMode(VoJK, INPUT);
    pinMode(VoBSZ, INPUT);
    pinMode(VoBK, INPUT);  
     
    pinMode(Start, INPUT); // 2-es digit. láb bemenet
     
}
    

     
//*********************************************************************     
    
     
void motor_stop()
{
  
   analogWrite(Mot1pwm,0);    //  PWM motor1  kikapcsol
   analogWrite(Mot2pwm,0);     //  PWM motor2  kikapcsol
   digitalWrite(Mot1A, LOW);
	 digitalWrite(Mot1B, LOW);
   digitalWrite(Mot2A, LOW);
	 digitalWrite(Mot2B, LOW);
}


//*********************************************************************     
     
void elore()     //mindket motor elore 
{

   analogWrite(Mot1pwm,sebes);    //  PWM motor1  max.
   analogWrite(Mot2pwm,sebes);     //  PWM motor2  max.
   digitalWrite(Mot1A, HIGH);
	 digitalWrite(Mot1B, LOW);
   digitalWrite(Mot2A, HIGH);
	 digitalWrite(Mot2B, LOW);
   //  Serial.println("előre");
}
     
//*********************************************************************     
     
void hatra()     //  mindket motor hatra
{ 
   analogWrite(Mot1pwm,sebes);    //  PWM motor1  max.
   analogWrite(Mot2pwm,sebes);     //  PWM motor2  max.
   digitalWrite(Mot1A, LOW);
	 digitalWrite(Mot1B, HIGH);
   digitalWrite(Mot2A, LOW);
	 digitalWrite(Mot2B, HIGH);
    // Serial.println("hátra");
}

    
//*********************************************************************
     
void jobbra100()     // jobbra fordulas nagyon kis ívben: motor1 elore, motor2 all 
{
   analogWrite(Mot1pwm,sebes);    //  PWM motor1  max.
   analogWrite(Mot2pwm,0);     //  PWM motor2  min
   digitalWrite(Mot1A, HIGH);
   digitalWrite(Mot1B, LOW);
   digitalWrite(Mot2A, LOW);
   digitalWrite(Mot2B, LOW);     
   // Serial.println("jobbra100  ");
}

//*********************************************************************    
     
void balra100()         // balra fordulas nagyon kis ívben: motor1 all, motor2 elore
{
   analogWrite(Mot1pwm,0);    //  PWM motor1  min.
   analogWrite(Mot2pwm,sebes);     //  PWM motor2  max.
   digitalWrite(Mot1A, LOW);
   digitalWrite(Mot1B, LOW);
   digitalWrite(Mot2A, HIGH);
   digitalWrite(Mot2B, LOW);     
    //  Serial.print("balra100  "); 
}



//*********************************************************************
     
void balraX(byte x)         // balra fordulas valamilyen (x) ívben   0-100
{
   unsigned int kitolt1;

   if(x>100) x=100;
   kitolt1=((100-x)*sebes)/100;
   analogWrite(Mot1pwm,kitolt1);    //  PWM motor1 (bal) lassúbb
   analogWrite(Mot2pwm,sebes);     //  PWM motor2  max.
   digitalWrite(Mot1A, HIGH);
   digitalWrite(Mot1B, LOW);
   digitalWrite(Mot2A, HIGH);
   digitalWrite(Mot2B, LOW);  
  //  Serial.print("balraX    "); Serial.println(x); 
 
     
}

//*********************************************************************    
     
void jobbraX(byte x)     // jobbra fordulas valamilyen (x) ívben   0-100
{
   unsigned int kitolt2;
   
   if(x>100) x=100;
   kitolt2=((100-x)*sebes)/100;
   analogWrite(Mot2pwm,kitolt2);    //  PWM motor2 (jobb) lassúbb
   analogWrite(Mot1pwm,sebes);     //  PWM motor1 (bal)  max.
   digitalWrite(Mot1A, HIGH);
   digitalWrite(Mot1B, LOW);
   digitalWrite(Mot2A, HIGH);
   digitalWrite(Mot2B, LOW);   
  //  Serial.print("jobbraX    ");  Serial.println(x); 
  
}

//*********************************************************************
     
void jobbraforog()     // jobbra fordulas helyben: motor1 elore, motor2 hatra 
{
   analogWrite(Mot1pwm,sebes);    //  PWM motor1  max.
   analogWrite(Mot2pwm,sebes);     //  PWM motor2  max.
   digitalWrite(Mot1A, HIGH);
   digitalWrite(Mot1B, LOW);
   digitalWrite(Mot2A, LOW);
   digitalWrite(Mot2B, HIGH);     
    //  Serial.println("jobbra forog  ");
}

//*********************************************************************    
     
void balraforog()         // balra fordulas helyben: motor1 hatra, motor2 elore
{
   analogWrite(Mot1pwm,sebes);    //  PWM motor1  max.
   analogWrite(Mot2pwm,sebes);     //  PWM motor2  max.
   digitalWrite(Mot1A, LOW);
   digitalWrite(Mot1B, HIGH);
   digitalWrite(Mot2A, HIGH);
   digitalWrite(Mot2B, LOW);     
    //  Serial.println("balra forog  ");
}


//*********************************************************************    
     
long uh_szenzor()   
{
   long duration, cm;
    
     // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
     // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    digitalWrite(UH1_trig, LOW);
    delayMicroseconds(5);
    digitalWrite(UH1_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(UH1_trig, LOW);

     // Read the signal from the sensor: a HIGH pulse whose
     // duration is the time (in microseconds) from the sending
     // of the ping to the reception of its echo off of an object.
     // pinMode(echoP, INPUT);
     duration = pulseIn(UH1_echo, HIGH);    
     cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
     
    delay(20);
    return cm; 
}


//*********************************************************************

void mozog1teszt() 
{
    unsigned int i=0;


      elore();
      delay(4000);
      motor_stop();   // mindkét motor állj
      delay(5000);
     
      jobbra100();
      delay(3000);
       motor_stop();
      delay(5000);
      
      balra100();
      delay(3000);
      motor_stop();
      delay(5000);

      hatra();
      delay(6000);
       motor_stop();  
  
      delay(8000);

  
}


//*********************************************************************

void mozog2teszt() 
{
    unsigned int i=0;

 
      jobbraX(80);
      delay(1200);
      motor_stop();
      delay(9000);
      
      jobbraX(70);
      delay(1200);
      motor_stop();
      delay(9000);    
      
      jobbraX(60);
      delay(1800);
      motor_stop();
      delay(9000);

      jobbraX(50);
      delay(2200);
      motor_stop();
      delay(9000);
                
      delay(8000);

  
}


//*********************************************************************
//   VoJSZ A0     // vonalkövető, jobb szélső
//   VoJK A1     // vonalkövető, jobb közép
//   VoBK A2     // vonalkövető, bal közép
//   VoBSZ A3     // vonalkövető, bal szélső

// világos felület ->  30-50     -->    <60
//  fekete határán -> 130-200   -->     >100
//  fekete  -> 330-600           -->    >250

void vonalteszt() 
{
   int jobb,jkoz,bkoz,bal;
   long ido;

  while(1) 
  {
      ido = millis();
      Serial.print("idő:    ");
      Serial.println(ido);

      jobb=analogRead(VoJSZ);
      jkoz=analogRead(VoJK);
      bkoz=analogRead(VoBK);
      bal=analogRead(VoBSZ);
      
      ido = millis();
      Serial.print("idő A/D után:    ");
      Serial.println(ido);
     
      
      Serial.print(bal);
      Serial.print("  ");
      Serial.print(bkoz);
      Serial.print("  ");
      Serial.print(jkoz);
      Serial.print("  ");
      Serial.print(jobb);
      Serial.print("  ");
      Serial.println();
      delay(1000);
  }
}


//*********************************************************************

void kerul(byte tav) 
{
   long kozel;
   // long ido;

  while(1) 
  {
      kozel = uh_szenzor();
	  if(kozel<tav)		// akadály
	  {
      motor_stop(); 
		  balraforog();		// kerülés balra
		  delay(200);
      motor_stop();
	  }
       
      elore();		// autó előre
     
      delay(20);
      
  }
}


//*********************************************************************
//   VoJSZ A0     // vonalkövető, jobb szélső
//   VoJK A1     // vonalkövető, jobb közép
//   VoBK A2     // vonalkövető, bal közép
//   VoBSZ A3     // vonalkövető, bal szélső

// világos felület ->  30-50     -->    <60
//  fekete határán -> 130-200   -->     >100
//  fekete  -> 300-600           -->    >300

//  mozgas:  0 - áll    1 - előre     2 - hátra   
//  10 - balra100   11- balra forog		
//  20 - jobbra100   21- jobbra forog	

// auto lerakása a vonal  baloldalára !!

void vonal1sz() 
{
    int szenzor;
    int kicsi=100;
    int nagy=200;   //  ezt az értéket keresi és próbálja tartani, kb. fekete csík széle
    byte mozgas_regi;
    long ido;

    sebes=255;
    mozgas=20;
    mozgas_regi=0;
    szenzor=analogRead(VoJK);     // szenzor lekérdezése
    while(szenzor<nagy)
    {
       mozgas=20;
        jobbra100();
     /*  Serial.println("  jobbra ");  
       Serial.println("  szenzor1 ");        
      Serial.print(szenzor);
      Serial.println();    */
       szenzor=analogRead(VoJK);     // szenzor lekérdezése
      
       delay(3);
       mozgas_regi=mozgas;
      
    }
    
    while(1)
    {

        szenzor=analogRead(VoJK);     // szenzorok lekérdezése
       // jkoz=analogRead(VoBK);
  
      /*  Serial.println("  szenzor2 ");        
        Serial.print(szenzor);
        Serial.println();     */
 
		    if(szenzor<nagy) mozgas=20;		// jobbra100
        else if(szenzor==nagy) mozgas=1;  //  előre
		    else mozgas=10;					// balra100
       
      /*   Serial.println("  mozgas ");        
        Serial.print(mozgas);
        Serial.println();     */
        
        if(mozgas!=mozgas_regi)      // ha változott a mozgás irány -> megfelelő függvény meghívása
         {
            if(mozgas==10)
               { balra100();     }
            else if(mozgas==20) 
               { jobbra100();     }
            else 
                {  elore();   }
         }
        delay(3);
        mozgas_regi=mozgas;
        
    }

}



//*********************************************************************
//   VoJSZ A0     // vonalkövető, jobb szélső
//   VoJK A1     // vonalkövető, jobb közép
//   VoBK A2     // vonalkövető, bal közép
//   VoBSZ A3     // vonalkövető, bal szélső

// világos felület ->  30-50     -->    <60
//  fekete határán -> 130-200   -->     >100
//  fekete  -> 300-600           -->    >300

//  mozgas:  0 - áll    1 - előre     2 - hátra   
//  10 - balra100   11- balra forog    
//  20 - jobbra100   21- jobbra forog  

// auto lerakása a vonal  baloldalára !!

void vonal3sz() 
{
    int szenzor;
    int kicsi=100;
    int nagy=200;   //  ezt az értéket keresi és próbálja tartani, kb. fekete csík széle
    byte mozgas_regi;
    long ido;
    int bal,bkoz,jkoz,jobb;  // szenzorok

    sebes=255;
    mozgas=20;
    mozgas_regi=0;
    bkoz=analogRead(VoBK);     // szenzor lekérdezése
    jobbra100();
    while(bkoz<nagy)    // vonalra állás
    {  
         jobb=analogRead(VoJSZ);   // szenzorok lekérdezése
        // jkoz=analogRead(VoJK);
         bkoz=analogRead(VoBK);
         bal=analogRead(VoBSZ);
         
         if(jobb>nagy) mozgas=20;   // jobbra100
         else if(bal>nagy) mozgas=10;  //  balra100
         else mozgas=20;         // jobbra100
         
         if(mozgas!=mozgas_regi)      // ha változott a mozgás irány -> megfelelő függvény meghívása
          {
            if(mozgas==10)
               { balra100();     }   
            else if(mozgas==20) 
               { jobbra100();     } 
            else 
                {  elore();   }
           }
          delay(1);
          mozgas_regi=mozgas;
      
    }
    
    while(1)
    {

         jobb=analogRead(VoJSZ);   // szenzorok lekérdezése
        // jkoz=analogRead(VoJK);
         bkoz=analogRead(VoBK);
         bal=analogRead(VoBSZ);
  
      /*  Serial.println("  szenzor2 ");        
        Serial.print(szenzor);
        Serial.println();     */
 
        if(bkoz<nagy) mozgas=20;   // jobbra100
        else if(bkoz==nagy) mozgas=1;  //  előre
        else mozgas=10;         // balra100
       
      /*   Serial.println("  mozgas ");        
        Serial.print(mozgas);
        Serial.println();     */
        
        if(mozgas!=mozgas_regi)      // ha változott a mozgás irány -> megfelelő függvény meghívása
         {
            if(mozgas==10)
               { balra100();     }   
            else if(mozgas==20) 
               { jobbra100();     } 
            else 
                {  elore();   }
         }
        delay(1);
        mozgas_regi=mozgas;
        
    }

}



//*********************************************************************

void setup() {
  // put your setup code here, to run once:

	
    unsigned int i=0;
    long tavol;
   
    
    motor_init();
    szenzor_init();
     
 
    
    Serial.begin(9600);
  

    // while (DigitalRead(Start));  
   
    Serial.println("indul");

    while(1)
    {

      
       // vonalteszt();
       // mozog1teszt();

       // mozog2teszt();

        // vonal1sz();

        // vonal3sz();
        
        kerul(30);

    }  

}


//*********************************************************************



void loop() {
  // put your main code here, to run repeatedly:

}
