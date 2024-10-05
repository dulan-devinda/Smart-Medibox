#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

#define Buzzer 5
#define LED_1 15
#define PB_Cancel 34
#define PB_Ok 32
#define PB_Down 35
#define PB_Up 33
#define DHTPin 12
#define LED_2 16
#define LED_3 17

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET_DST  0
//Remove hard-coded UTC offsets
int userUtcOffsetHours = 0;
int userUtcOffsetMinutes = 0;


//global variables of time
int seconds = 0;
int minutes = 0;
int hours = 0;
int days = 0;
unsigned long timeNow = 0;
unsigned long timeLast = 0;

bool alarm_enabled = true;
int n_alarms = 3;
//choose initial values that do not correspond to a valid alarm time as the initial value
int alarm_hours[] = {-1, -1, -1};
int alarm_minutes[] = {-1, -1, -1};
bool alarm_triggered[] = {false, false};

//for the buzzer
int note_count = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;
int note_array[] = {C, D, E, F, G, A, B, C_H};

int current_mode = 0;
int max_modes = 5;
String modes[] = {"1 - Set Time Zone", "2 - Set Alarm 1", "3 - Set Alarm 2", "4 - Set Alarm 3", "5 - Disable All Alarms"};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;

void setup() {
  pinMode(Buzzer, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(PB_Cancel, INPUT);
  pinMode(PB_Down, INPUT);
  pinMode(PB_Ok, INPUT);
  pinMode(PB_Up, INPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);

  dhtSensor.setup(DHTPin, DHTesp::DHT22);
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // is used to update the OLED display with any changes that have made to its content
  display.display();
  delay(2000);

  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    display.clearDisplay();
    print_line("Connecting to WiFi", 0, 0, 2);
  }

  display.clearDisplay();
  print_line("Connected to WiFi", 0, 0, 2);

  configTime((userUtcOffsetHours * 3600) + (userUtcOffsetMinutes * 60), UTC_OFFSET_DST, NTP_SERVER);

  //is called to clear the display
  display.clearDisplay();

  print_line("Welcome to Medibox!", 10, 20, 2);
  delay(2000);
  display.clearDisplay();
}

void loop() {
  // Nothing to do here
  update_time_check_alarm();
  if (digitalRead(PB_Ok) == LOW) {
    delay(200);
    go_to_menu();
  }
  check_temp();
}

void print_line(String text, int column, int row, int textsize) {
  display.setTextSize(textsize);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column, row);
  display.println(text);
  display.display();

}
void print_time_now() {
  // Print the time in the format 0:0:0:0
  display.clearDisplay();
  print_line(String(days), 0, 0, 2);
  print_line(":", 20, 0, 2);
  print_line(String(hours), 32, 0, 2);
  print_line(":", 55, 0, 2);
  print_line(String(minutes), 65, 0, 2);
  print_line(":", 87, 0, 2);
  print_line(String(seconds), 100, 0, 2);
}

void update_time() {
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char timeHour[3];
  strftime(timeHour, 3, "%H", &timeinfo);
  hours = atoi(timeHour);

  char timeMinute[3];
  strftime (timeMinute, 3, "%M", &timeinfo);
  minutes = atoi (timeMinute);

  char timeSecond[3];
  strftime(timeSecond, 3, "%S", &timeinfo);
  seconds = atoi(timeSecond);

  char timeDay[3];
  strftime(timeDay, 3, "%d", &timeinfo);
  days = atoi(timeDay);
}

void ring_alarm() {
  // ring the buzzer
  display.clearDisplay();
  print_line("MEDICINE TIME", 0, 0, 2);

  bool break_happened = false;
  digitalWrite(LED_1, HIGH);

  //ring the buzzer
  while (break_happened == false && digitalRead(PB_Cancel) == HIGH) {
    for (int i = 0; i < note_count; i++ ) {
      if (digitalRead(PB_Cancel) == LOW) {
        delay(200);
        break_happened = true;
        break;
      }
      tone(Buzzer, note_array[i]);
      delay(500);
      noTone(Buzzer);
      delay(2);
    }
  }

  digitalWrite(LED_1, LOW);
  display.clearDisplay();

}

void update_time_check_alarm(void) {
  update_time();
  print_time_now();

  // Check each alarm
  if (alarm_enabled == true  ) {
    for (int i = 0; i < n_alarms; i++) {
      if (hours == alarm_hours[i] && minutes == alarm_minutes[i]  && !alarm_triggered[i]) {
        // Trigger alarm
        ring_alarm();
        alarm_triggered[i] = true; // Mark the alarm as triggered to avoid repeated triggering


      }
    }
  }
}

int wait_for_button_press() {
  while (true) {
    if (digitalRead(PB_Up) == LOW) {
      delay(200);
      return PB_Up;
    }
    else if (digitalRead(PB_Down) == LOW) {
      delay(200);
      return PB_Down;
    }
    else if (digitalRead(PB_Ok) == LOW) {
      delay(200);
      return PB_Ok;
    }
    else if (digitalRead(PB_Cancel) == LOW) {
      delay(200);
      return PB_Cancel;
    }
    update_time();
  }
}

void go_to_menu() {

  while (digitalRead(PB_Cancel) == HIGH) {
    display.clearDisplay();
    print_line(modes[current_mode], 0, 0, 2);
    int pressed = wait_for_button_press();
    if (pressed == PB_Up) {
      delay(200);
      current_mode += 1;
      current_mode = current_mode % max_modes;
    }
    else if (pressed == PB_Down) {
      delay(200);
      current_mode -= 1;
      if (current_mode < 0) {
        current_mode = max_modes - 1;
      }
    }
    else if (pressed == PB_Ok) {
      delay(200);
      run_mode(current_mode);
    }
    if (pressed == PB_Cancel) {
      delay(200);
      break;
    }
  }
}
void set_time_zone() {
  int Offset_Hour = userUtcOffsetHours;

  while (true) {
    display.clearDisplay();
    print_line("Enter UTC Offset hour: " + String(Offset_Hour), 0, 0, 2);

    int pressed = wait_for_button_press();
    if (pressed == PB_Up) {
      delay(200);
      Offset_Hour += 1;
      if (Offset_Hour > 14) {
        Offset_Hour = -12;
      }
    }
    else if (pressed == PB_Down) {
      delay(200);
      Offset_Hour -= 1;
      if (Offset_Hour < -12) {
        Offset_Hour = 14;
      }
    }
    else if (pressed == PB_Ok) {
      delay(200);
      userUtcOffsetHours = Offset_Hour;
      break;
    }
    if (pressed == PB_Cancel) {
      delay(200);
      break;
    }
  }
  int Offset_Minute = userUtcOffsetMinutes;

  while (true) {
    display.clearDisplay();
    print_line("Enter UTC Offset minute: " + String(Offset_Minute), 0, 0, 2);

    int pressed = wait_for_button_press();
    if (pressed == PB_Up) {
      delay(200);
      Offset_Minute += 1;
      Offset_Minute = Offset_Minute % 60;
    }
    else if (pressed == PB_Down) {
      delay(200);
      Offset_Minute -= 1;
      if (Offset_Minute < -59) {
        Offset_Minute = 59;
      }
    }
    else if (pressed == PB_Ok) {
      delay(200);
      userUtcOffsetMinutes = Offset_Minute;
      break;
    }
    if (pressed == PB_Cancel) {
      delay(200);
      break;
    }
  }
  configTime((userUtcOffsetHours * 3600) + (userUtcOffsetMinutes * 60), UTC_OFFSET_DST, NTP_SERVER);
  display.clearDisplay();
  print_line("Time Zone is set", 0, 0, 2);
  delay(1000);
}

void set_alarm(int alarm) {

  int temp_hour = alarm_hours[alarm];

  while (true) {
    display.clearDisplay();
    print_line("Enter hour: " + String(temp_hour), 0, 0, 2);

    int pressed = wait_for_button_press();
    if (pressed == PB_Up) {
      delay(200);
      temp_hour += 1;
      temp_hour = temp_hour % 24;
    }
    else if (pressed == PB_Down) {
      delay(200);
      temp_hour -= 1;
      if (temp_hour < 0) {
        temp_hour = 23;
      }
    }
    else if (pressed == PB_Ok) {
      delay(200);
      alarm_hours[alarm] = temp_hour;
      break;
    }
    if (pressed == PB_Cancel) {
      delay(200);
      break;
    }
  }
  int temp_minutes = alarm_minutes[alarm];

  while (true) {
    display.clearDisplay();
    print_line("Enter minutes: " + String(temp_minutes), 0, 0, 2);

    int pressed = wait_for_button_press();
    if (pressed == PB_Up) {
      delay(200);
      temp_minutes += 1;
      temp_minutes = temp_minutes % 60;
    }
    else if (pressed == PB_Down) {
      delay(200);
      temp_minutes -= 1;
      if (temp_minutes < 0) {
        temp_minutes = 59;
      }
    }
    else if (pressed == PB_Ok) {
      delay(200);
      alarm_minutes[alarm] = temp_minutes;
      break;
    }
    if (pressed == PB_Cancel) {
      delay(200);
      break;
    }
  }
  display.clearDisplay();
  print_line("Alarm is set", 0, 0, 2);
  delay(1000);
}

void run_mode(int mode) {
  if (mode == 0) {
    set_time_zone();
  }
  else if (mode == 1 || mode == 2 || mode == 3) {
    set_alarm(mode - 1);
  }
  else if (mode == 4) {
    alarm_enabled = false;
    // Disable all alarms and set them to their initial values
    for (int i = 0; i < n_alarms; i++) {
      alarm_hours[i] = -1;
      alarm_minutes[i] = -1;
      alarm_triggered[i] = false;
    }
    display.clearDisplay();
    print_line("All Alarms Disabled", 0, 0, 2);
    delay(500);
  }
}

void check_temp() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  if (data.temperature > 32) {
    print_line("Temp high", 0, 40, 1);
    delay(1000);
    digitalWrite(LED_2, HIGH);
  }
  else if (data.temperature < 26) {
    print_line("Temp low", 0, 40, 1);
    delay(500);
    digitalWrite(LED_2, HIGH);
  }
  else {
    digitalWrite(LED_2, LOW);
  }
  if (data.humidity > 80) {
    print_line("Humidity high", 0, 50, 1);
    delay(500);
    digitalWrite(LED_3, HIGH);
  }
  else if (data.humidity < 60) {
    print_line("Humidity low", 0, 50, 1);
    delay(500);
    digitalWrite(LED_3, HIGH);
  }
  else {
    digitalWrite(LED_3, LOW);
  }
}