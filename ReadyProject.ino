#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Servo.h> // подключаем библиотеку для работы с сервоприводом

#define SDA_PIN A4
#define SCL_PIN A5

// Устанавливаем соединение для DFPlayer mini
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
Servo servo1; // объявляем переменную servo типа "servo1"
Servo servo2;
Servo servo3;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  servo1.attach(5);
  servo2.attach(6);
  servo3.attach(7);
  Serial.begin(9600);
  pinMode(13, OUTPUT); // объявляем пин 13 как выход
  tcs.begin();
  
  if (!tcs.begin()) {
    Serial.println("Не удалось найти TCS3472. Проверьте подключение!");
    while (1);
  }
  
  tcs.setInterrupt(false);
  
  Serial.println("Датчик цвета TCS3472 готов к использованию!");

  // Устанавливаем скорость соединения
  mySoftwareSerial.begin(9600);
 // Serial.begin(115200);

  // Инициализируем DFPlayer mini
  if (!myDFPlayer.begin(mySoftwareSerial)) 
  {
    Serial.println(F("Не удалось инициализировать DFPlayer mini!"));
    

    while(1);
  }

   servo1.write(90); // ставим угол поворота под 0
   servo2.write(90); // ставим угол поворота под 0
   servo3.write(90);
}

void loop() {
  uint16_t clear, red, green, blue;
  
  tcs.getRawData(&red, &green, &blue, &clear);

  float red_f, green_f, blue_f;
  red_f = red;
  green_f = green;
  blue_f = blue;

  float sum = clear;
  
  // Приводим значения к процентам
  red_f /= sum;
  green_f /= sum;
  blue_f /= sum;

  // Определяем цвет в текстовом формате
  String color;
  if (red_f > 0.25 && green_f > 0.25 && blue_f > 0.25) {
    color = "Белый";
  } else if (red_f > green_f && red_f > blue_f) {
    color = "Красный";
    myDFPlayer.play(1); // Проигрываем первую аудиозапись
     servo1.write(0); // ставим угол поворота под 0
  delay(15000); // Ждем 5 секунд
   servo1.write(90); // ставим угол поворота под 0
  } else if (green_f > red_f && green_f > blue_f) {
    color = "Зеленый";
    servo3.write(0);
      myDFPlayer.play(2); // Проигрываем вторую аудиозапись
  delay(15000); // Ждем 5 секунд
   servo3.write(90);
  } else if (blue_f > red_f && blue_f > green_f) {
    color = "Синий";
     servo2.write(0); // ставим угол поворота под 0
     myDFPlayer.play(3); // Проигрываем третью аудиозапись
  delay(15000); // Ждем 5 секунд
   servo2.write(90); // ставим угол поворота под 0
  } else if (red_f > 0.25 && green_f > 0.25) {
    color = "Желтый";
    myDFPlayer.play(4); // Проигрываем четвертую аудиозапись
  } else if (red_f > 0.25 && blue_f > 0.25) {
    color = "Фиолетовый";
  } else if (green_f > 0.25 && blue_f > 0.25) {
    color = "Голубой";
  } else {
    color = "Неизвестно";
  }
  
  // Выводим результаты на серийный порт
  Serial.print("Определенный цвет: ");
  Serial.println(color);

  // Пауза 1 секунда
  delay(1000);
}
