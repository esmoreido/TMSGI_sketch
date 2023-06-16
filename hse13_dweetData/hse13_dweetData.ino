#include <ESP8266WiFi.h>                                     // Подключаем библиотеку ESP8266WiFi
#include <TroykaThermometer.h> // Подключаем библиотеку для аналогового термометра

char ssid[]     = "";               // Укажите здесь в кавычках название вашей точки доступа
char password[] = "";            // Укажите здесь в кавычках пароль вашей точки доступа

const char* host     = "dweet.io";                              // Адрес сервера на который будем отправлять данные

TroykaThermometer thermometer(A7);                                      // Создаем объект датчика и указываем соответствующий порт
// адрес двита
String thing = "";          // В кавычках указываем любое название для стрима, например MyStreamTempSensor2023, тогда url для просмотра будет выглядеть как https://dweet.io/follow/MyStreamTempSensor2023
String content = ""; // Название для переменной, которое будет отображаться в стриме
void setup() {
  Serial.begin(9600);                                         // Инициализируем последовательный порт
  delay(10);

  // Подключаемся к точке доступа WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // ждем соединения с сетью
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // считывание данных с датчика
  thermometer.read();
  float temp = thermometer.getTemperatureC();                              // Считываем данные температуры
  Serial.print("Temperature = ");
  Serial.println(temp);
  
  // создаем объект для передачи запроса
  WiFiClient client;
  if (!client.connect("dweet.io", 80)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }
  
  Serial.println("sending data to server");
  if (client.connected()) {
    // формируем GET-запрос для отправки
  String url = "/dweet/for/";
  url += thing;
  url += "?";
  url += content;
  url += "=";
  url += temp;

  Serial.print("Requesting URL: ");
  Serial.println(url);
  // отправляем запрос 
     client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  } else {
    Serial.println("Connection failed!");
  }
  delay(1000);
  
}
