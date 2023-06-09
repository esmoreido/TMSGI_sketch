#include <ESP8266WiFi.h>                                     // Подключаем библиотеку ESP8266WiFi
#include <TroykaThermometer.h>

char ssid[]     = "";               // Укажите здесь в кавычках название вашей точки доступа
char password[] = "";            // Укажите здесь в кавычках пароль вашей точки доступа

const char* host     = "dweet.io";                              // Адрес сервера на который будем отправлять данные

TroykaThermometer thermometer(A7);                                      // Создаем объект датчика
// адрес двита
String thing = "";
String content = "";
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
  
     client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  } else {
    Serial.println("Connection failed!");
  }
  delay(1000);
  
}
