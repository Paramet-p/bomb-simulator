#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <time.h>
#include <TridentTD_LineNotify.h>
#include <HardwareSerial.h>

#define I2CADDR 0x20

#define RX2 16
#define TX2 17

#define buzzer 13
#define R_LED 14
#define Y_LED 27
#define G_LED 26

#define TOTAL_WIRES 6

#define SSID "OPPO F11 Pros"                                      //ใส่ ชื่อ Wifi ที่จะเชื่อมต่อ
#define PASSWORD "0944861348"                                     //ใส่ รหัส Wifi
#define LINE_TOKEN "s8SkTlewn3ckqNAp7pqogJcCBc0XjAAA3oqNEB7dmT4"  //ใส่ รหัส TOKEN ที่ได้มาจากข้างบน

HardwareSerial SerialPort(2);  // use UART2

int mintenscode, minonescode, sectenscode, seconescode;
int minutes, seconds, trycount, keycount = 0, i = 0;

long secMillis = 0, last_time = 0;

const int CUTTABLE_WIRES[TOTAL_WIRES] = { 15, 4, 5, 18, 19, 23 };
int WIRES_TO_CUT[TOTAL_WIRES];
int wireStates[TOTAL_WIRES];

int password[4];
int entered[4];

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' },
};

byte rowPins[ROWS] = { 7, 6, 5, 4 };  // R1, R2, R3, R4
byte colPins[COLS] = { 3, 2, 1, 0 };  // C1, C2, C3, C4

LiquidCrystal_I2C lcd(0x27, 20, 4);  //Change the HEX address
Keypad_I2C keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR);

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

byte correct[8] = {
  0b00000,
  0b00001,
  0b00011,
  0b10110,
  0b11100,
  0b01000,
  0b00000,
  0b00000
};

hw_timer_t *My_timer = NULL;

int face_time_count = 15, num_of_face = 0;
long timerInterval = 1000;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
bool game_finish = false, display_state = false;
char mode;

void IRAM_ATTR count_timer() {
  portENTER_CRITICAL_ISR(&timerMux);
  if (minutes <= 0 && seconds <= 0) {
    timerInterval = 1000000;
  } else {
    seconds--;
    if (seconds < 0) {
      seconds = 59;
      minutes--;
    }
    tone(buzzer, 5000, 50);
    digitalWrite(R_LED, !(digitalRead(R_LED)));
  }
  timerAlarmWrite(My_timer, timerInterval * 1000, true);
  portEXIT_CRITICAL_ISR(&timerMux);
}

int wireWithNewState = -1;
void IRAM_ATTR cut_wire_interrupt() {
  wireWithNewState = detectWireStateChange();
  display_state = true;
}

void setup() {
  //srand(time(NULL));
  pinMode(R_LED, OUTPUT);
  pinMode(Y_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);

  for (int i = 0; i < TOTAL_WIRES; i++) {
    pinMode(CUTTABLE_WIRES[i], INPUT_PULLUP);
    wireStates[i] = LOW;  // wires are initially tied to GND
    attachInterrupt(digitalPinToInterrupt(CUTTABLE_WIRES[i]), cut_wire_interrupt, CHANGE);
  }

  digitalWrite(R_LED, HIGH);
  digitalWrite(Y_LED, HIGH);
  digitalWrite(G_LED, HIGH);

  Serial.begin(9600);
  SerialPort.begin(115200, SERIAL_8N1, RX2, TX2);

  Wire.begin();
  keypad.begin();

  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n", SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
  LINE.setToken(LINE_TOKEN);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.createChar(0, heart);
  lcd.createChar(1, correct);

  select_mode();

  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &count_timer, true);
  timerAlarmWrite(My_timer, timerInterval * 1000, true);
  timerAlarmEnable(My_timer);  //Just Enable
}

void loop() {

  if (game_finish == true) {
    char play_again = keypad.getKey();
    keycount = 0;
    if (play_again == 'A') {
      tone(buzzer, 5000, 100);
      lcd.clear();
      select_mode();
      trycount = 0;
      game_finish = false;
    }
  } else {
    switch (mode) {
      case '1': mode1_enter_password(); break;
      case '2': mode2_cut_wire(); break;
      case '3': mode3_wifi_mix(); break;
    }
  }
}

void select_mode() {

  lcd.setCursor(0, 0);
  lcd.print("Enter mode:");
  lcd.setCursor(0, 1);
  lcd.print("1:Enter Password");
  lcd.setCursor(0, 2);
  lcd.print("2:Cut Wires");
  lcd.setCursor(0, 3);
  lcd.print("3:Mixs");
  while (keycount < 1) {
    mode = keypad.getKey();
    lcd.setCursor(12, 0);
    lcd.blink();
    if (mode == '1' || mode == '2' || mode == '3') {
      tone(buzzer, 5000, 100);
      keycount++;
    }
  }

  keycount = 0;
  switch (mode) {
    case '1': set_time_mode1(); break;
    case '2': set_time_mode2(); break;
    case '3': set_time_mode3(); break;
  }
  last_time = millis();
}

void show_time() {
  int time_position = 2;
  lcd.setCursor(0, time_position);
  lcd.print("Timer:");
  lcd.setCursor(7, time_position);
  print2digits(minutes);
  lcd.print(":");
  print2digits(seconds);
}

void detect_face() {
  if (SerialPort.available()) {
    // Read the incoming data from SoftwareSerial
    char incomingChar = SerialPort.read();

    // Print the received character to the Serial Monitor
    Serial.print("Received character from Odroid: ");
    Serial.println(incomingChar);
    num_of_face = atoi(&incomingChar);
    if (num_of_face >= 1)
      last_time = millis();
  }
}

void check_face() {
    detect_face();
    if (num_of_face == 0 && millis() - last_time > 5000) {
      face_time_count--;
      lcd.clear();
      show_time();
      lcd.setCursor(0,0);
      lcd.print("Time Left ");
      lcd.print(face_time_count);
      if (face_time_count == 0) {
        minutes = 0;
        seconds = 0;
      }
      delay(1000);
    }
    else {
      face_time_count = 15;
    }
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {  // ถ้า 0 <= number < 10 ให้เพิ่ม 0 ด้านหน้า
    lcd.print('0');
  }
  lcd.print(number);  // แสดงตัวเลข
}
