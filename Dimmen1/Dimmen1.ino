
const unsigned int led = 3;     //wenn const davor steht, kann der Wert im Programm nicht mehr geändert werden, ist vorteilhaft, wenn Werte nicht versehentlich geändert werden sollen
unsigned int brightness = 0;    //bei unsigned gibt es keine negativen Werte
unsigned int fadeAmount = 5;    //Alle Variablen, die hier definiert werden sind !!!!global!!!!

void setup()                  //Das Setup wird nur einmal beim Start des Arduinos ausgeführt
{
   Serial.begin(9600);        //Wird für die Verwendung des seriellen Monitors benötigt. Dieser ist unter Werkzeuge zu finden. Die Baudrate muss auf 9600 eingestellt werden (unten rechts im Monitor)
   pinMode(led, OUTPUT);      //led = 3 --> der Pin D3 wird als Output festgelegt. Das ist unser PWM-Pin
}


void loop()                   //Der Loop ist eine Dauerschleife, die solange ausgeführt wird, bis der Arduino deaktiviert wird oder durch den Rese-Knopf neu gestartet wurde
{                             //Variablen, die hier defeniert werden, werden in jedem Loop neu definiert und sind nur hier vorhanden!!
  
  // set the brightness of pin 3:
  analogWrite(led, brightness);     //mit analogWrite kann ein Wert von 0 - 255 ausgegeben werden, an dem Pin led, in diesem fall Pin 3

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) 
  {
    fadeAmount = -fadeAmount;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);                        //delay ist eine schlechte Methode, da hier der Arduino pausiert, also sozusagen 30 ms lange hängt. Aber in diesem Fall ist das ausreichend

  Serial.print("PWM: ");            //Serial.print ist die Ausgabe im seriellen Monitor. Sachen in Anführungszeichen werden einfach so ausgegeben
  Serial.println(brightness);       //bei println wird nach der Ausgabe in eine neue Zeile gesprungen. In der Klammer steht der Wert, der ausgegeben werden soll. In diesem Fall unser aktuelles PWM-Signal
  Serial.println(" ");              //dient nur der Übersichtklichkeit (Leerzeile)
}
