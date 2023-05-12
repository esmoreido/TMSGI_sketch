// библиотека для работы с дисплеем
#include <TroykaTextLCD.h>
// создаем объект для работы с дисплеем
TroykaTextLCD lcd;

// библиотека для работы с GPS устройством
#include <TroykaGPS.h>
// serial-порт к которому подключён GPS-модуль
#define GPS_SERIAL Serial1
// создаём объект класса GPS и передаём в него объект Serial1
GPS gps(GPS_SERIAL);
// задаём размер массива для времени, даты, широты и долготы
#define MAX_SIZE_MASS 16
// массив для хранения текущего времени
char strTime[MAX_SIZE_MASS];
// массив для хранения текущей даты
char strDate[MAX_SIZE_MASS];
// массив для хранения широты в градусах, минутах и секундах
char latitudeBase60[MAX_SIZE_MASS];
// массив для хранения долготы в градусах, минутах и секундах
char longitudeBase60[MAX_SIZE_MASS];


#define BUTTON_PIN  13
#define LED_PIN A0
void setup()
{
  // открываем монитор Serial-порта
  Serial.begin(9600);
  // устанавливаем количество столбцов и строк экрана
  lcd.begin(16, 2);
  // устанавливаем контрастность в диапазоне от 0 до 63
  lcd.setContrast(27);
  // устанавливаем яркость в диапазоне от 0 до 255
  lcd.setBrightness(255);
  // настраиваем пин в режим входа
  pinMode(BUTTON_PIN, INPUT);
  // ждём, пока не откроется монитор последовательного порта
  // для того, чтобы отследить все события в программе
  while (!Serial) { }
  Serial.print("Serial init OK\r\n");
  // открываем Serial-соединение с GPS-модулем на скорости 115200 бод
  GPS_SERIAL.begin(115200);
  // печатаем строку
  Serial.println("GPS init is OK on speed 115200");
  // изменяем скорость обещение GPS-модуля с управляющей платой на 9600 бод
  // используем NMEA-команду «$PMTK251,9600*17\r\n»
  GPS_SERIAL.write("$PMTK251,9600*17\r\n");
  // закрываем Serial-соединение с GPS-модулем
  GPS_SERIAL.end();
  // открываем Serial-соединение с GPS-модулем на скорости 9600 бод
  GPS_SERIAL.begin(9600);
  // печатаем строку
  Serial.print("GPS init is OK on speed 9600");
    
}
 
void loop()
{
  // считываем состояние пина
  int buttonState = digitalRead(BUTTON_PIN);
  // выводим в Serial-порт
//  Serial.println(buttonState);
  // устанавливаем курсор в колонку 0, строку 0
  lcd.setCursor(0, 0);
  // печатаем первую строку
//  lcd.print(buttonState);
  delay(100);
  // если пришли данные с GPS-модуля
    if (gps.available()) {
        // считываем данные и парсим
        gps.readParsing();
        // проверяем состояние GPS-модуля
        switch (gps.getState()) {
        // всё OK
        case GPS_OK:
            Serial.println("GPS is OK");
            lcd.print("GPS is OK");
            lcd.setCursor(0, 8);
            lcd.print(gps.getSat());
            // выводим координаты широты и долготы
            // 1. в градусах, минутах и секундах
            // 2. градусах в виде десятичной дроби
            Serial.println("GPS Coordinates: ");
            gps.getLatitudeBase60(latitudeBase60, MAX_SIZE_MASS);
            gps.getLongitudeBase60(longitudeBase60, MAX_SIZE_MASS);
            lcd.setCursor(1, 0);
            Serial.print("Latitude\t");
            Serial.print(latitudeBase60);
            lcd.print(latitudeBase60);
            Serial.print("\t\t");
            Serial.println(gps.getLatitudeBase10(), 6);
            Serial.print("Longitude\t");
            Serial.print(longitudeBase60);
            lcd.setCursor(1, 8);
            lcd.print(longitudeBase60);
            Serial.print("\t\t");
            Serial.println(gps.getLongitudeBase10(), 6);
            // выводим количество видимых спутников
            Serial.print("Sat: ");
            Serial.println(gps.getSat());
            // выводим текущую скорость
            Serial.print("Speed: ");
            Serial.println(gps.getSpeedKm());
            // выводим высоту над уровнем моря
            Serial.print("Altitude: ");
            Serial.println(gps.getAltitude());
            // выводим текущее время
            Serial.print("Time: ");
            gps.getTime(strTime, MAX_SIZE_MASS);
            gps.getDate(strDate, MAX_SIZE_MASS);
            Serial.write(strTime);
            Serial.println();
            // выводим текущую дату
            Serial.print("Date: ");
            Serial.write(strDate);
            Serial.println("\r\n");
            // каждую переменную дату и времени можно выводить отдельно
            /*    Serial.print(gps.getHour());
                  Serial.print(gps.getMinute());
                  Serial.print(gps.getSecond());
                  Serial.print(gps.getDay());
                  Serial.print(gps.getMonth());
                  Serial.print(gps.getYear());
            */
            break;
        // ошибка данных
        case GPS_ERROR_DATA:
            Serial.println("GPS error data");
            lcd.print("GPS error data");
            break;
        // нет соединения со спутниками
        case GPS_ERROR_SAT:
            Serial.println("GPS is not connected to satellites!!!");
            lcd.print("GPS is not connected to satellites");
            break;
        }
    }
}
