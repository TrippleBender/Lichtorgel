/****************************************
Example Sound Level Sketch for the 
Adafruit Microphone Amplifier
****************************************/

unsigned int led = 3;                   // PWM Output Pin for LED

double amplify = 5.0;
double brightness = 0.0;
int peakToPeak = 0;
unsigned int i = 0;
unsigned int j = 0;

unsigned long lagMillis = 0;
unsigned long outTimeMillis = 0;
unsigned int amplifyOld = 0;
unsigned int k = 0;

void setup() 
{  
  pinMode(led, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}


void loop() 
{
  const unsigned int sampleWindow = 50;     // level window width in mS (50 mS = 20Hz)
  
  const int mic = 0;                        // Input pin of Microphone
  const int micMax = 675;
  const double pwm = 255.0;
  const double pwmMin = 50.0;
  
  const double amplifyMax = 10.0;           // Regulater
  const double amplifyMin = 1.0;
  const double difference = 1.0;
  const int limit = 5;                      // Wie oft in die Begrenzung gefahren werden darf

  const int sensitvity = 15;                // Filter
  int oldPTP = 0;

  unsigned int lag = 250;                   // For the Builtin_led to show the Amplify
  unsigned int outTime = 1000;


  unsigned long startMillis = millis();     // Start of sample window

  unsigned int signalMax = 0;
  unsigned int signalMin = micMax;


  // collect data for 50 mS
  oldPTP = peakToPeak;
  
  while (millis() - startMillis < sampleWindow)
  {
     unsigned int level = analogRead(mic);
     if (level <= micMax)                   // toss out spurious readings
     {
        if (level > signalMax)
        {
           signalMax = level;               // save just the max levels
        }
        else if (level < signalMin)
        {
           signalMin = level;               // save just the min levels
        }
     }
  }
  peakToPeak = signalMax - signalMin;       // max - min = peak-peak amplitude

  // Filter
  if(sensitvity < oldPTP - peakToPeak || -sensitvity > oldPTP - peakToPeak)
  {
    const int fadeAmount = 5;
    const int regulateWindow = 200;
    const int gap = 10;
    double target = amplify * (peakToPeak * pwm) / micMax;  // convert to pwm
    amplifyOld = (int) amplify;


    // Limiter
    if(target >= pwm)
    {
      target = pwm;                       // set maximal brightness
      i = i + 1;
      j = 0;
    }
    else if(target <= pwmMin)             // set minimal brightness
    {
      target = pwmMin;
      j = j + 1;
      i = 0;
    }

    // Regulater
    if(i > limit && amplify > amplifyMin)         // reduces the Amplify if overload
    {
      amplify -= difference;
      i = 0;
      j = 0;
    }
    else if(j > limit && amplify < amplifyMax)    // raise the Amplify if always minimal brightness is used 
    {
      amplify += difference;
      j = 0;
      i = 0;
    }
      
    startMillis = millis();

    while((brightness < target-fadeAmount || brightness > target+fadeAmount) && millis() - startMillis < regulateWindow)          // fading the brightness
    {
      if(brightness > target)
      {
        brightness -= fadeAmount;
      }
      else
      {
        brightness += fadeAmount;
      }

      delay(gap);
      /*Serial.print("Brightness: ");
      Serial.println(brightness);
      Serial.print("Target: ");
      Serial.println(target);*/
      
      analogWrite(led, (int)brightness);
    }

    brightness = target;
    analogWrite(led, (int)brightness);
  }

  if(k <= amplify && millis() - outTimeMillis >= outTime)                               // Ausgabe der Verstärkung als Blinksignal
  {
    
    if(millis() - lagMillis >= lag && millis() - lagMillis <= 2 * lag)
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if(millis() - lagMillis >= 2 * lag)
    {
      lagMillis = millis();
      digitalWrite(LED_BUILTIN, LOW);
      k += 1;
    }
    
  }
  else if(k > amplify)
  {
    k = 0;
    outTimeMillis = millis();
  }

  //Serial.print("Brightness: ");
  //Serial.println(brightness);
  //Serial.println(peakToPeak);
  //Serial.println(oldPTP);
  //Serial.println(oldPTP-peakToPeak);
  //Serial.print("Amplify: ");
  //Serial.println(amplify);
  //Serial.print("j: ");
  //Serial.println(j);
  //Serial.print("i: ");
  //Serial.println(i);
  Serial.println(" ");
  
}
