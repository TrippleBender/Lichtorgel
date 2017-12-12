/****************************************
Example Sound Level Sketch for the 
Adafruit Microphone Amplifier
****************************************/

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
int led = 3;

void setup() 
{
   Serial.begin(9600);
   pinMode(led, OUTPUT);
}


void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level

   unsigned int signalMax = 0;
   unsigned int signalMin = 675;

   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

   // 255 ist maximalte PWM, 675 ist das maximale Signal, dass vom Mikrophon kommt, also maximaler Wert für peakToPeak
   double volts = (peakToPeak * 255) / 675;  // convert to pwm
   
   analogWrite(led, volts);
   
   Serial.print("Signal: ");
   Serial.println(peakToPeak);

   Serial.print("PWM: ");
   Serial.println(volts);
   Serial.println(" ");
}
