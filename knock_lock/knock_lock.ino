/*
  Замок с сервоприводом и секретным стуком
  При старте системы крышка открывается, ожидается ввод секретной комбинации
  Если комбинация не вводится в течение 5 секунд, замок будет открываться по одному "стуку"
  После ввода комбинации (таймаут 5 секунд), пищалка играет комбинацию и закрывает замок
  Для открытия нужно просто простукать комбинацию
  Для повторного закрытия "ткнуть" один раз, либо нажать внутреннюю кнопку

  Система использует аппаратные прерывания, что позволяет находиться в режиме сна,
  а также очень чётко отрабатывать "стуки", практически без ошибок.
  Хотел допилить запись в EEPROM, но не сегодня =)
*/

//----- НАСТРОЙКИ -----
#define difficulty 100  // миллисекунд на реакцию (плюс минус)
#define max_knock 30    // число запоминаемых "стуков"
#define close_angle 85  // угол закрытия
#define open_angle 180  // угол открытия
#define debug 1         // режим отладки - вывод в порт информации о процессе игры
//----- НАСТРОЙКИ -----

#include <Servo.h>
Servo servo;
#define buzzPin 7       // пин пищалки
#define buzzGND 6       // земля пищалки
#define sensGND 5       // земля сенсора
#define sensVCC 4       // питание сенсора
#define servoVCC 8      // питание серво
#define servoPin 11     // серво 

#include <TimerOne.h>
#include <LowPower.h>      // библиотека сна
byte fade_count, knock;
volatile byte mode;
boolean cap_flag, write_start;
volatile boolean debonce_flag, threshold_flag;
volatile unsigned long debounce_time;
unsigned long last_fade, last_try, last_knock, knock_time, button_time;

byte count, try_count;
int wait_time[max_knock], min_wait[max_knock], max_wait[max_knock];

void setup() {
  Serial.begin(9600);
  if (debug) Serial.println("system start");
  delay(50);
  pinMode(2, INPUT);              // пин датчика вибрации или кнопка
  pinMode(3, INPUT_PULLUP);       // пин датчика вибрации или кнопка
  servo.attach(servoPin);

  // настраиваем пины питания как выходы
  pinMode(buzzPin, OUTPUT);
  pinMode(buzzGND, OUTPUT);
  pinMode(sensGND, OUTPUT);
  pinMode(sensVCC, OUTPUT);
  pinMode(servoVCC, OUTPUT);

  // подаём нужные сигналы
  digitalWrite(buzzPin, 0);
  digitalWrite(buzzGND, 0);
  digitalWrite(sensGND, 0);
  digitalWrite(sensVCC, 1); 

  digitalWrite(servoVCC, 1);
  servo.write(open_angle);
  delay(1000);
  digitalWrite(servoVCC, 0);
  
  attachInterrupt(0, threshold, RISING);     // прерывание датчика
  attachInterrupt(1, buttonPress, FALLING);  // прерывание кнопки выключить
  threshold_flag = 0;

  knockWrite();

  delay(50);
  close_cap();
  good_night();                              // сразу спать
}

void knockWrite() {                                 // режим записи стука
  if (debug) Serial.println("knock write mode");
  last_knock = millis();
  knock = 0;
  while (1) {                                     // ждём первого удара
    if (millis() - last_knock > 5000) {           // если 5 секунд не ударяли
      write_start = 0;                            // всё сбросить, выйти из режима
      break;
    }
    if (threshold_flag) {                         // если ударили
      write_start = 1;                            // разрешить запись
      tone(buzzPin, 400, 50);                 // пикнуть дрыгнуть
      last_knock = millis();
      threshold_flag = 0;
      if (debug) Serial.println("knock");
      break;
    }
  }

  if (write_start) {                                  // если запись пошла
    while (1) {
      if (threshold_flag) {
        knock_time = millis() - last_knock;           // расчёт времени между стуками
        wait_time[knock] = knock_time;                // записать
        min_wait[knock] = knock_time - difficulty;    // определить время с учётом времени реакции
        max_wait[knock] = knock_time + difficulty;    // определить время с учётом времени реакции
        knock++;                                      // перейти к следующему
        tone(buzzPin, 400, 50);                                  // пикнуть дрыгнуть
        last_knock = millis();
        threshold_flag = 0;
        if (debug) Serial.println("knock");
      }
      if (millis() - last_knock > 3000) {
        break;
      }
    }
    // показать комбинацию "раунда"
    tone(buzzPin, 400, 50);                       // пыхнуть светодиодом
    for (byte i = 0; i < knock; i++) {
      delay(wait_time[i]);               // ждать время шага одного хода
      tone(buzzPin, 400, 50);                       // пыхнуть светодиодом
      if (debug) Serial.println(wait_time[i]);
    }
    mode = 0;                            // перейти в режим игры
  }

}

void loop() {
  if (threshold_flag && mode == 0) {
    threshold_flag = 0;
    if (knock == 0) {
      mode = 3;
      goto openCap;
    }
    debounce_time = millis();
    last_try = millis();      // обнулить таймер
    tone(buzzPin, 400, 50);
    try_count = 0;
    threshold_flag = 0;
    while (1) {

      // если не нажал в установленное время (проигрыш)
      if (millis() - last_try > max_wait[try_count]) {
        // мигнуть красным два раза
        tone(buzzPin, 400, 50);
        delay(1000);
        mode = 0;             // перейти в начало! Это начало нового раунда
        if (debug) Serial.println("too slow");
        threshold_flag = 0;
        break;
      }
      if (threshold_flag) {

        // если нажатие попало во временной диапазон (правильное нажатие)
        if (millis() - last_try > min_wait[try_count] && millis() - last_try < max_wait[try_count]) {
          tone(buzzPin, 400, 50);               // мигнуть
          try_count++;               // увеличить счётчик правильных нажатий
          last_try = millis();       // ВОТ ТУТ СЧЁТЧИК СБРАСЫВАЕТСЯ, ЧТОБЫ УБРАТЬ ВЛИЯНИЕ ЗАДЕРЖЕК!
          threshold_flag = 0;        // сбросить флаг
          if (debug) Serial.println("good");

          // если нажал слишком рано (проигрыш)
        } else if (millis() - last_try < min_wait[try_count] && threshold_flag) {
          tone(buzzPin, 400, 50);
          delay(100);
          tone(buzzPin, 400, 50);        // мигнуть красным дважды
          delay(1000);
          mode = 0;            // перейти в начало! Это начало нового раунда
          if (debug) Serial.println("too fast");
          threshold_flag = 0;
          break;
        }

        // если число правильных нажатий совпало с нужным для раунда (выигрыш)
        if (try_count == knock) {
          // мигнуть 3 раза
          delay(200);
          tone(buzzPin, 400, 50);
          delay(200);
          tone(buzzPin, 400, 50);
          delay(200);
          tone(buzzPin, 400, 50);
          delay(200);
          mode = 3;   // перейти к действию при выигрыше
          if (debug) Serial.println("victory");
          break;
        }
      }
    }
  }


  if (mode == 3) {
openCap:
    mode = 4;
    delay(500);
    open_cap();
    good_night();
  }


  if ((threshold_flag && mode == 4) || mode == 5) {
    mode = 0;
    delay(500);
    close_cap();
    good_night();
  }

  if (millis() - debounce_time > 10000 || mode == 4) {
    good_night();
  }
}

void threshold() {
  if (millis() - debounce_time > 50) debonce_flag = 1;
  if (debonce_flag) {
    debounce_time = millis();
    threshold_flag = 1;
    debonce_flag = 0;
  }
}

void buttonPress() {
  if (mode == 4) {
    mode = 5;
  }
}

void good_night() {
  if (debug) {
    Serial.println("good night");
    delay(50);
  }
  delay(5);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);    // спать. mode POWER_OFF, АЦП выкл
}

void open_cap() {
  if (debug) Serial.println("open cap");
  digitalWrite(servoVCC, 1);
  for (int i = close_angle; i < open_angle; i++) {
    servo.write(i);
    Serial.println(i);
    delay(12);
  }
  delay(100);
  digitalWrite(servoVCC, 0);
}

void close_cap() {
  if (debug) Serial.println("close cap");
  digitalWrite(servoVCC, 1);
  for (int i = open_angle; i > close_angle; i--) {
    servo.write(i);
    Serial.println(i);
    delay(12);
  }
  delay(100);
  digitalWrite(servoVCC, 0);
}

