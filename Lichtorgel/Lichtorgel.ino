/****************************************
Example Sound Level Sketch for the 
Adafruit Microphone Amplifier
****************************************/

unsigned int led = 3;                   // PWM Output Pin for LED

double amplify = 10.0;
double brightness = 0.0;
int peakToPeak = 0;            // peak-to-peak level
unsigned int i = 0;
unsigned int j = 0;

void setup() 
{  
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}


void loop() 
{
  const unsigned int sampleWindow = 50;   // level window width in mS (50 mS = 20Hz)
  
  const unsigned int mic = 0;             // Input pin of Microphone
  const unsigned int micMax = 675;
  const double pwm = 255.0;
  const double pwmMin = 50.0;
  
  const double amplifyMax = 10.0;          // Regulater
  const double amplifyMin = 1.0;
  const double difference = 1.0;
  const unsigned int limit = 9;

  const int sensitvity = 15;    // Filter
  int oldPTP = 0;

  unsigned long startMillis= millis();    // Start of sample window

  unsigned int signalMax = 0;
  unsigned int signalMin = micMax;


  // collect data for 50 mS
  oldPTP = peakToPeak;
  //Serial.println(oldPTP);
  while (millis() - startMillis < sampleWindow)
  {
     unsigned int level = analogRead(mic);
     if (level <= micMax)                // toss out spurious readings
     {
        if (level > signalMax)
        {
           signalMax = level;            // save just the max levels
        }
        else if (level < signalMin)
        {
           signalMin = level;            // save just the min levels
        }
     }
  }
  peakToPeak = signalMax - signalMin;    // max - min = peak-peak amplitude
  //Serial.println(peakToPeak);
  Serial.println(oldPTP-peakToPeak);

  // Filter
  if(sensitvity < oldPTP - peakToPeak || -sensitvity > oldPTP - peakToPeak)
  {
    const unsigned int fadeAmount = 10;
    const unsigned int regulateWindow = 20;
    double target = amplify * (peakToPeak * pwm) / micMax;  // convert to pwm
    j = 0;

    // Limiter
    if(target >= pwm)
    {
      target = pwm;
      i = i + 1;
      j = 0;
      Serial.print("i");
      Serial.println(i);
    }
    else if(target <= pwmMin)
    {
      target = pwmMin;
      j = j + 1;
      i = 0;
      Serial.print("j");
      Serial.println(j);
    }

    // Regulater
    if(i > limit && amplify > amplifyMin)
    {
      amplify = amplify - difference;
      i = 0;
      j = 0;
    }
    else if(j > limit && amplify < amplifyMax)
    {
      amplify = amplify + difference;
      j = 0;
      i = 0;
    }
      
    startMillis = millis();

    while(brightness < target-fadeAmount || brightness > target+fadeAmount || millis() - startMillis < regulateWindow)
    {
      if(brightness > target)
      {
        brightness -= fadeAmount;
      }
      else
      {
        brightness += fadeAmount;
      }
    
      analogWrite(led, (int)brightness);

      Serial.print("Brightness: ");
      Serial.println(brightness);
      Serial.print("Target: ");
      Serial.println(target);
      /*Serial.println(i);
      Serial.println(j);*/
      Serial.println(" ");
    }

    brightness = target;
    analogWrite(led, (int)brightness);
  }
  
  Serial.print("Amplify: ");
  Serial.println(amplify);


  /*// Limiter
  if(brightness >= pwm)
  {
    brightness = pwm;
    i = i + 1;
  }
  else if(brightness <= pwmMin)
  {
    brightness = pwmMin;
    j = j + 1;
  }

  analogWrite(led, (int)brightness);
  

  // Regulater
  if(i > limit && amplify > amplifyMin)
  {
    amplify = amplify - difference;
    i = 0;
    j = 0;
  }
  else if(j > limit && amplify < amplifyMax)
  {
    amplify = amplify + difference;
    j = 0;
    i = 0;
  }
  
  Serial.println(brightness);
  Serial.println(i);
  Serial.println(j);
  Serial.println(amplify);*/
}
