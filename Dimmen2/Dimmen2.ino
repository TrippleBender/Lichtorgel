
const unsigned int led = 3; 
unsigned int brightness = 0;    
unsigned int fadeAmount = 5;    

unsigned long verzoegerungMillis = 0;      //Diese Variable wird als Vergleichswert dienen

void setup()                  
{
   Serial.begin(9600);
   pinMode(led, OUTPUT);
}


void loop()                   
{
  const unsigned int verzoegerung = 30;             //stellt die Verzögerung ein, selbe Bedeutung wie die 30 beim delay

  if(millis()-verzoegerungMillis >= verzoegerung)   //diese Methode ist besser als delay(). millis() gibt die aktuelle Laufzeit des Arduinos an. Weiter unten wird verzoegerungMillis auf die aktuelle Zeit gesetzt und gespeichert. Nach weitern 30 ms ist das aktuelle millis() - dem alten Wert verzoegerungMillis >= 30 ms, das bedeutet, dass das Programm alle 30 ms in die if-Schleife springt, aber der Arduino in der zwischenzeit anderer Sachen machen kann.
  {
    analogWrite(led, brightness);                   //das folgende ist bekannt

    brightness = brightness + fadeAmount;

    if (brightness <= 0 || brightness >= 255) 
    {
      fadeAmount = -fadeAmount;
    }

    verzoegerungMillis = millis();                  //hier wird die neue Vergleichszeit gespeichert.
  }
                      
  Serial.print("PWM: ");          
  Serial.println(brightness);   
  Serial.println(" ");        
}
