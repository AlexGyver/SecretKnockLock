[![AlexGyver YouTube](http://alexgyver.ru/git_banner.jpg)](https://www.youtube.com/channel/UCgtAOyEQdAyjvm9ATCi_Aig?sub_confirmation=1)
# Замок с "секретным стуком" на Arduino
* [Описание проекта](#chapter-0)
* [Папки проекта](#chapter-1)
* [Схемы подключения](#chapter-2)
* [Материалы и компоненты](#chapter-3)
* [Настройка и использование](#chapter-4)
* [FAQ](#chapter-5)
* [Полезная информация](#chapter-6)
* [Мой сайт](http://alexgyver.ru/)
* [Основной YouTube канал](https://www.youtube.com/channel/UCgtAOyEQdAyjvm9ATCi_Aig?sub_confirmation=1)
* [YouTube канал про Arduino](https://www.youtube.com/channel/UC4axiS76D784-ofoTdo5zOA?sub_confirmation=1)

<a id="chapter-0"></a>
## Описание проекта
Замок с сервоприводом и датчиком звука/вибрации/касания
- "Запоминает" секретный стук, а именно - время между "ударами"
- Умеет распознавать слишком быстрое нажатие, слишком медленное, ну и само собой "правильное" нажатие
- Работает от аппаратного прерывания, что даёт очень стабильную и чёткую отработку "стуков"
- Использована библиотека сна, потребление в режиме ожидания около 0.1 мА
- Подробности в видео: https://youtu.be/DwI65tBNN1I

<a id="chapter-1"></a>
## Папки
- **libraries** - библиотеки, установить в
`C:\Program Files (x86)\Arduino\libraries\` (Windows x64)  
`C:\Program Files\Arduino\libraries\` (Windows x86)
- **knock_lock** - прошивка для Arduino, файлы в папках открыть в Arduino IDE (читай [FAQ](#chapter-5))

<a id="chapter-2"></a>
## Схема
![СХЕМА](https://github.com/AlexGyver/SecretKnockLock/blob/master/scheme.jpg)

<a id="chapter-3"></a>
## Материалы и компоненты
* Arduino NANO http://ali.ski/rWrni
* Серво http://ali.ski/KbBJY
* Батарейный отсек http://ali.ski/wwtAiC
* Сенсорная кнопка http://ali.ski/s05R9

* Датчик звука http://ali.ski/oDlkMQ
* Датчик удара http://ali.ski/Jl8QpL
* Датчик вибрации точный http://ali.ski/bFgK0g
* Датчик вибрации не очень точный http://ali.ski/xhcEmh

РАССЫПУХА
* МОСФЕТ 
  + IRF3704ZPBF
  + IRLB8743PBF
  + IRL2203NPBF
  + IRLB8748PBF
  + IRL8113PBF
  + IRL3803PBF
  + IRLB3813PBF
  + IRL3502PBF
  + IRL2505PBF
  + IRF3711PBF
  + IRL3713PBF
  + IRF3709ZPBF
  + AUIRL3705N
  + IRLB3034PBF
  + IRF3711ZPBF
* Резистор 100 Ом
* Резистор 10 кОм
* Пищалка из компьютера

## Вам скорее всего пригодится
* [Всё для пайки (паяльники и примочки)](http://alexgyver.ru/all-for-soldering/)
* [Недорогие инструменты](http://alexgyver.ru/my_instruments/)
* [Все существующие модули и сенсоры Arduino](http://alexgyver.ru/arduino_shop/)
* [Электронные компоненты](http://alexgyver.ru/electronics/)
* [Аккумуляторы и зарядные модули](http://alexgyver.ru/18650/)

<a id="chapter-4"></a>
## Настройка и использование
* [Загрузка прошивки](http://alexgyver.ru/arduino-first/) - ультра подробная статья по началу работы с Ардуино
* Установить библиотеки в
`C:\Program Files (x86)\Arduino\libraries\` (Windows x64)  
`C:\Program Files\Arduino\libraries\` (Windows x86)
* Подключить Ардуино к компьютеру
* Запустить файл прошивки
* Настроить (COM порт, модель Arduino NANO 328)
* Нажать загрузить
* Пользоваться

## Настройки в коде
    difficulty 100  // миллисекунд на реакцию (плюс минус)
    max_knock 30    // число запоминаемых "стуков"
    close_angle 85  // угол закрытия
    open_angle 180  // угол открытия
    debug 1         // режим отладки - вывод в порт информации о процессе игры

<a id="chapter-5"></a>
## FAQ
### Основные вопросы
В: Как скачать с этого грёбаного сайта?  
О: На главной странице проекта (где ты читаешь этот текст) вверху справа зелёная кнопка **Clone or download**, вот её жми, там будет **Download ZIP**

В: Скачался какой то файл .zip, куда его теперь?  
О: Это архив. Можно открыть стандартными средствами Windows, но думаю у всех на компьютере установлен WinRAR, архив нужно правой кнопкой и извлечь.

В: Я совсем новичок! Что мне делать с Ардуиной, где взять все программы?  
О: Читай и смотри видос http://alexgyver.ru/arduino-first/

В: Компьютер никак не реагирует на подключение Ардуины!  
О: Возможно у тебя зарядный USB кабель, а нужен именно data-кабель, по которому можно данные передавать

В: Ошибка! Скетч не компилируется!  
О: Путь к скетчу не должен содержать кириллицу. Положи его в корень диска.

В: Сколько стоит?  
О: Ничего не продаю.

### Вопросы по этому проекту

<a id="chapter-6"></a>
## Полезная информация
* [Мой сайт](http://alexgyver.ru/)
* [Основной YouTube канал](https://www.youtube.com/channel/UCgtAOyEQdAyjvm9ATCi_Aig?sub_confirmation=1)
* [YouTube канал про Arduino](https://www.youtube.com/channel/UC4axiS76D784-ofoTdo5zOA?sub_confirmation=1)
* [Мои видеоуроки по пайке](https://www.youtube.com/playlist?list=PLOT_HeyBraBuMIwfSYu7kCKXxQGsUKcqR)
* [Мои видеоуроки по Arduino](http://alexgyver.ru/arduino_lessons/)