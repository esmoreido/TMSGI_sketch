// библиотека для работы с SPI
#include <SPI.h>
// библиотека для работы с SD-картами
#include <SD.h>
// библиотека для работы с датчиками DHT11, DHT21, DHT22
#include <TroykaDHT.h>
// создаём объект класса DHT
// передаём номер пина к которому подключён датчик и тип датчика
// типы сенсоров: DHT11, DHT21, DHT22
DHT dht(4, DHT11);
// даём разумное имя для CS пина microSD-карты
#define SD_CS_PIN  8

// создаем пустую строку для данных
String dataString = "";

void setup() {
  // открываем последовательный порт для мониторинга действий в программе
  Serial.begin(9600);
  dht.begin();
  // выводим сообщение в Serial-порт о поиске карты памяти
  Serial.println("Initializing SD card...");
  // если microSD-карта не была обнаружена
  if (!SD.begin(SD_CS_PIN)) {
    // выводим сообщение об ошибке
    Serial.println("Card failed, or not present");
    // возвращаемся к выполнению кода
    return;
  } else {
    Serial.println("Card initialized.");
  }
}

void loop() {
  // переменная состояния датчика
  int check;
  // переменная показания температуры
  double temperature;
  // переменная показания влажности
  double humidity;
  dataString = "";
  // мониторинг ошибок
  // считывание данных с датчика DHT11
  check = dht.read();
  // ветвление поведения в зависимости от результата проверки состояния  
  switch (check) {
    // всё OK
    case DHT_OK:
      // считываем показания влажности и температуры
      temperature = dht.getTemperatureC();
      humidity = dht.getHumidity();
      // формируем строку с результатами показаний датчика
      dataString += "Temperature = ";
      dataString += String(temperature);
      dataString += "C \t";
      dataString += "Humidity = ";
      dataString += String(humidity);
      dataString += "%\r\n";
      // выводим результаты в serial-порт
      Serial.print(dataString);
      // сохраняем на microSD
      saveSD();
      // выходим из ветвления      
      break;
    // ошибка контрольной суммы
    case DHT_ERROR_CHECKSUM:
      Serial.println("Checksum error");
      break;
    // превышение времени ожидания
    case DHT_ERROR_TIMEOUT:
      Serial.println("Time out error");
      break;
    // неизвестная ошибка
    default:
      Serial.println("Unknown error");
      break;
  }
  // ждём 1 секунду
  delay(1000);
}
 
void saveSD()
{
  // создаём файл для записи
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // если файл доступен для записи
  if (dataFile) {
    // сохраняем данные
    dataFile.print(dataString);
    // закрываем файл
    dataFile.close();
    // выводим сообщение об удачной записи
    Serial.println("Save OK");
  } else {
    // если файл не доступен
    Serial.println("Error opening datalog.txt");
  }

}
