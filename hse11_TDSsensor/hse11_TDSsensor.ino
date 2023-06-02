// любой GPIO пин с поддержкой АЦП
constexpr auto pinSensor = A0;
 
void setup() {
  // открываем Serial-порт
  Serial.begin(9600);
}
 
void loop() {
  // считываем данные с датчика влажности почвы
  int valueSensor = analogRead(pinSensor);
  // переводим данные с датчика в напряжение
  float voltageSensor = valueSensor * 5 / 1024.0;
  // конвертируем напряжение в концентрацию
  float tdsSensor = (133.42 * pow(voltageSensor, 3) - 255.86 * pow(voltageSensor, 2) + 857.39 * voltageSensor) * 0.5;
  // выводим данные в Serial-порт
  Serial.print("TDS Value = "); 
  Serial.print(tdsSensor);
  Serial.println(" ppm");
  // ждём 100 мс
  delay(1000);
}
