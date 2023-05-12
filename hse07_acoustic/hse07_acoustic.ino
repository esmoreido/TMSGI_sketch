#define SOUND_PIN  A5
#define NOISE_PIN  A4
 
void setup() 
{
  // открываем монитор Serial-порта
  Serial.begin(9600);
}
 
void loop()
{
  // считываем показания микрофона
  int soundValue = analogRead(SOUND_PIN);
  // считываем показания уровня шума
  int noiseValue = analogRead(NOISE_PIN);
  Serial.print(soundValue);
  Serial.print("\t\t");
  Serial.println(noiseValue);
}
