#define VIBRO_PIN             A5
#define VIBRO_INTEGRATED_PIN  A4
 
void setup() 
{
  // открываем монитор Serial-порта
  Serial.begin(9600);
}
 
void loop()
{
  // считываем показания с пьезодиска
  int vibroValue = analogRead(VIBRO_PIN);
  // считываем показания уровня вибрации
  int integratedVibroValue = analogRead(VIBRO_INTEGRATED_PIN);
  Serial.print("Immediate");
  Serial.print("\t");
  Serial.print(vibroValue);
  Serial.print("\t");
  Serial.print("Integrated");
  Serial.print("\t\t");
  Serial.print(integratedVibroValue);
  Serial.print(",275,315");
  Serial.println();
  delay(100);
}
