// BP Code for Arduino
#include <LiquidCrystal.h>
//  Variables
int pulsePin = A3;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 12;                // pin to blink led at each beat
int fadePin = 13;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

LiquidCrystal lcd(3, 4, 5, 6, 7, 8);
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
 float z11=0;
 int z12=0;
 int z13=120;
float zd=0;
float zs=0;
float z4=0;
float z5=0;
float z6=0;
// Pass our oneWire reference to Dallas Temperature.

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = true;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse 

volatile int rate[10];                      // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;           // used to find IBI
volatile int P = 512;                      // used to find peak in pulse wave, seeded
volatile int T = 512;                     // used to find trough in pulse wave, seeded
volatile int thresh = 525;                // used to find instant moment of heart beat, seeded
volatile int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = false;      // used to seed rate array so we startup with reasonable BPM

void setup()
{
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
  Serial.begin(9600);             // we agree to talk fast!
  lcd.begin(16,2);
 
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
                                    // IF YOU ARE POWERING The Pulse Sensor AT VOLTAGE LESS THAN THE BOARD VOLTAGE, 
                                    // UN-COMMENT THE NEXT LINE AND APPLY THAT VOLTAGE TO THE A-REF PIN
                                    //   analogReference(EXTERNAL);   
}


//  Where the Magic Happens
void loop(){
  //lcd.clear();
  // serialOutput();  
   delay(500);
   lcd.clear();
   lcd.setCursor(0, 0);  // set the cursor of LCD at 0 column of first row 
  lcd.print("T=");  // print constant character 
  float z22=analogRead(A1);
  if(z22>300)
  {
   z4=1;
  }
  if(z4==1)
  {
    z5=z5+1;
  }
  if(z5>28)
  {
    z4=0;
    z5=0;
    zd=z22*1.22;
    zs=z22*1.61;
   /* Serial.print(" DIAood mmHg = ");
    Serial.println(z22*1.2);
    Serial.print("SYS mmHg = ");
    Serial.println(z22*1.8);*/
  }
  
    lcd.setCursor(3, 0); // set cursor at column 3 and row 1 
//   lcd.print(z33);
  
  // lcd.setCursor(10, 0); // set cursor at column 3 and row 1 
  // lcd.print("Deg C");
      lcd.setCursor(9, 0);  // set the cursor of LCD at 0 column of first row 
  lcd.print("HB=");  // print constant character 
  lcd.setCursor(13, 0); // set cursor at column 3 and row 1 
   lcd.print(BPM);
   
   //Serial.print(",");
  // Serial.print("BPM: ");
   Serial.print(BPM); 
   Serial.print(",");
     Serial.print(zd);
     Serial.print(",");
     Serial.println(zs);
  delay(500);
 
  if (QS == true) // A Heartbeat Was Found
    {     
      // BPM and IBI have been Determined
      // Quantified Self "QS" true when arduino finds a heartbeat
      fadeRate = 255; // Makes the LED Fade Effect Happen, Set 'fadeRate' Variable to 255 to fade LED with pulse
      serialOutputWhenBeatHappens(); // A Beat Happened, Output that to serial.     
      QS = false; // reset the Quantified Self flag for next time    
    }
     
  ledFadeToBeat(); // Makes the LED Fade Effect Happen 
  delay(20); //  take a break
}

void ledFadeToBeat()
{
   fadeRate -= 15;                         //  set LED fade value
   fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
   analogWrite(fadePin,fadeRate);          //  fade LED
}

void interruptSetup()
{     
  // Initializes Timer2 to throw an interrupt every 2mS.
  TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
  TCCR2B = 0x06;     // DON'T FORCE COMPARE, 256 PRESCALER 
  OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
  TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED      
} 

void serialOutput()
{   // Decide How To Output Serial. 
 if (serialVisual == true)
  {  
     arduinoSerialMonitorVisual('-', Signal);   // goes to function that makes Serial Monitor Visualizer
  } 
 else
  {
      sendDataToSerial('S', Signal);     // goes to sendDataToSerial function
   }        
}

void serialOutputWhenBeatHappens()
{    
 if (serialVisual == true) //  Code to Make the Serial Monitor Visualizer Work
   {            
    // Serial.print("*** Heart-Beat Happened *** ");  //ASCII Art Madness
    /* Serial.print(",");
     Serial.print("BPM: ");
     Serial.println(BPM);*/
     //
   if(BPM<150)
   {
  /* lcd.setCursor(0, 1);  // set the cursor of LCD at 0 column of first row 
  lcd.print("HB=");  // print constant character 
   lcd.setCursor(5, 1); // set cursor at column 3 and row 1 
   lcd.print(BPM);
    lcd.setCursor(10, 1);
     lcd.print("BPM");*/
   }
    if(BPM>150)
   {
   lcd.setCursor(0, 2);  // set the cursor of LCD at 0 column of first row 
  lcd.print("HB=");  // print constant character 
   lcd.setCursor(5, 2); // set cursor at column 3 and row 1 
   lcd.print("00");
    lcd.setCursor(10, 2);
     lcd.print("BPM: ");
   }
   
   }
 else
   {
     sendDataToSerial('B',BPM);   // send heart rate with a 'B' prefix
     sendDataToSerial('Q',IBI);   // send time between beats with a 'Q' prefix
   }   
}

void arduinoSerialMonitorVisual(char symbol, int data )
{    
  const int sensorMin = 0;      // sensor minimum, discovered through experiment
  const int sensorMax = 1024;    // sensor maximum, discovered through experiment
  int sensorReading = data; // map the sensor range to a range of 12 options:
  int range = map(sensorReading, sensorMin, sensorMax, 0, 11);
  // do something different depending on the 
  // range value:
  switch (range) 
  {
    case 0:     
    //  Serial.println("");     /////ASCII Art Madness
      break;
    case 1:   
    //  Serial.println("---");
      break;
    case 2:    
    //  Serial.println("------");
      break;
    case 3:    
     // Serial.println("---------");
      break;
    case 4:   
     // Serial.println("------------");
      break;
    case 5:   
    //  Serial.println("--------------|-");
      break;
    case 6:   
     // Serial.println("--------------|---");
      break;
    case 7:   
      //Serial.println("--------------|-------");
      break;
    case 8:  
      //Serial.println("--------------|----------");
      break;
    case 9:    
      //Serial.println("--------------|----------------");
      break;
    case 10:   
      //Serial.println("--------------|-------------------");
      break;
    case 11:   
     // Serial.println("--------------|-----------------------");
      break;
  } 
}


void sendDataToSerial(char symbol, int data )
{
   Serial.print(symbol);
   Serial.println(data);                
}

ISR(TIMER2_COMPA_vect) //triggered when Timer2 counts to 124
{  
  cli();                                      // disable interrupts while we do this
  Signal = analogRead(pulsePin);              // read the Pulse Sensor 
  sampleCounter += 2;                         // keep track of the time in mS with this variable
  int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise
                                              //  find the peak and trough of the pulse wave
  if(Signal < thresh && N > (IBI/5)*3) // avoid dichrotic noise by waiting 3/5 of last IBI
    {      
      if (Signal < T) // T is the trough
      {                        
        T = Signal; // keep track of lowest point in pulse wave 
      }
    }

  if(Signal > thresh && Signal > P)
    {          // thresh condition helps avoid noise
      P = Signal;                             // P is the peak
    }                                        // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (N > 250)
  {                                   // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) )
      {        
        Pulse = true;                               // set the Pulse flag when we think there is a pulse
        digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
        IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
        lastBeatTime = sampleCounter;               // keep track of time for next pulse
  
        if(secondBeat)
        {                        // if this is the second beat, if secondBeat == TRUE
          secondBeat = false;                  // clear secondBeat flag
          for(int i=0; i<=9; i++) // seed the running total to get a realisitic BPM at startup
          {             
            rate[i] = IBI;                      
          }
        }
  
        if(firstBeat) // if it's the first time we found a beat, if firstBeat == TRUE
        {                         
          firstBeat = false;                   // clear firstBeat flag
          secondBeat = true;                   // set the second beat flag
          sei();                               // enable interrupts again
          return;                              // IBI value is unreliable so discard it
        }   
      // keep a running total of the last 10 IBI values
      word runningTotal = 0;                  // clear the runningTotal variable    

      for(int i=0; i<=8; i++)
        {                // shift data in the rate array
          rate[i] = rate[i+1];                  // and drop the oldest IBI value 
          runningTotal += rate[i];              // add up the 9 oldest IBI values
        }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values 
      BPM = 25000/runningTotal;               // how many beats can fit into a minute? that's BPM!
      QS = true;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

  if (Signal < thresh && Pulse == true)
    {   // when the values are going down, the beat is over
      digitalWrite(blinkPin,LOW);            // turn off pin 13 LED
      Pulse = false;                         // reset the Pulse flag so we can do it again
      amp = P - T;                           // get amplitude of the pulse wave
      thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
      P = thresh;                            // reset these for next time
      T = thresh;
    }

  if (N > 2500)
    {                           // if 2.5 seconds go by without a beat
      thresh = 512;                          // set thresh default
      P = 512;                               // set P default
      T = 512;                               // set T default
      lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
      firstBeat = true;                      // set these to avoid noise
      secondBeat = false;                    // when we get the heartbeat back
    }

  sei();                                   // enable interrupts when youre done!
}// end isr
