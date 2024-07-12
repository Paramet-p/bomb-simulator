int num1, num2;
void random_wire(int WIRES_TO_CUT[], int size) {
  srand(time(NULL));
  int wire_count_one = 0;
  while (wire_count_one == 0) {
    int sum = 100;
    while (sum >= 64) {
      num1 = rand() % 32;
      num2 = rand() % 32;
      sum = num1 + num2;
      Serial.println(sum);
    }
    for (int i = TOTAL_WIRES - 1; i >= 0; i--) {
      WIRES_TO_CUT[i] = sum & 1;
      if (sum & 1)
        wire_count_one++;
      sum >>= 1;
    }
    if (wire_count_one > 3)
      wire_count_one = 0;
  }  
}

void mode2_cut_wire() {
  check_face();
  if ((minutes <= 0 && seconds <= 0) || trycount == 3) {
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print("*** BOOM ***");
    for (int i = 0; i < 5; i++) {
      lcd.noBacklight();
      digitalWrite(R_LED, LOW);
      tone(buzzer, 7000, 100);
      delay(100);
      lcd.backlight();
      digitalWrite(R_LED, HIGH);
      tone(buzzer, 7000, 100);
      delay(100);
      lcd.noBacklight();
      digitalWrite(Y_LED, LOW);
      tone(buzzer, 7000, 100);
      delay(100);
      lcd.backlight();
      digitalWrite(Y_LED, HIGH);
      tone(buzzer, 7000, 100);
      delay(100);
      lcd.noBacklight();
      digitalWrite(G_LED, LOW);
      tone(buzzer, 7000, 100);
      delay(100);
      lcd.backlight();
      digitalWrite(G_LED, HIGH);
      tone(buzzer, 7000, 100);
      delay(100);
    }
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("Press 'A'");
    lcd.setCursor(3, 2);
    lcd.print("to reset bomb");
    game_finish = true;
    trycount = 0;
    display_state = false;
    return;
  }

  timerAlarmWrite(My_timer, timerInterval * 1000, true);
  lcd.clear();
  show_time();
  lcd.setCursor(0, 0);
  lcd.print("What is ");
  lcd.print(num1);
  lcd.print(" + ");
  lcd.print(num2);

  // wireWithNewState = detectWireStateChange();
  if (wireWithNewState >= 0) {
    handleWireStateChange(wireWithNewState);
  }

  displayCurrentState();

  if (areAllCorrectWiresCut()) {
    timerInterval = 1000000;
    timerAlarmWrite(My_timer, timerInterval * 1000, true);
    lcd.clear();
    lcd.print("BOMB DEFUSED");
    lcd.setCursor(0, 1);
    lcd.print("Congrate!!!");
    tone(buzzer, 5200, 100);
    delay(50);
    tone(buzzer, 0, 100);
    delay(50);
    tone(buzzer, 5200, 100);
    delay(50);
    tone(buzzer, 0, 100);
    delay(50);
    tone(buzzer, 5200, 100);
    delay(50);
    tone(buzzer, 0, 100);
    delay(5000);
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("Press 'A'");
    lcd.setCursor(3, 2);
    lcd.print("to reset bomb");
    game_finish = true;
  }
}

void set_time_mode2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Timer: MM:SS");
  lcd.setCursor(0, 1);
  lcd.print("SET:   :");
  keycount = 5;

  while (keycount == 5) {
    char mintens = keypad.getKey();
    lcd.setCursor(5, 1);
    lcd.blink();
    if (mintens >= '0' && mintens <= '9') {
      mintenscode = mintens - '0';
      tone(buzzer, 5000, 100);
      lcd.print(mintens);
      keycount++;
    }
  }

  while (keycount == 6) {
    char minones = keypad.getKey();
    lcd.setCursor(6, 1);
    lcd.blink();
    if (minones >= '0' && minones <= '9') {
      minonescode = minones - '0';
      tone(buzzer, 5000, 100);
      lcd.print(minones);
      keycount++;
    }
  }

  while (keycount == 7) {
    char sectens = keypad.getKey();
    lcd.setCursor(8, 1);
    lcd.blink();
    if (sectens >= '0' && sectens <= '5') {
      sectenscode = sectens - '0';
      tone(buzzer, 5000, 100);
      lcd.print(sectens);
      keycount = 8;
    }
  }

  while (keycount == 8) {
    char secones = keypad.getKey();
    lcd.setCursor(9, 1);
    lcd.blink();
    if (secones >= '0' && secones <= '9') {
      seconescode = secones - '0';
      tone(buzzer, 5000, 100);
      lcd.print(secones);
      keycount = 9;
    }
  }

  if (keycount == 9) {
    minutes = (mintenscode * 10) + minonescode;
    seconds = (sectenscode * 10) + seconescode;
    delay(100);
    lcd.noBlink();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Timer set at: ");

    print2digits(minutes);
    lcd.print(":");
    print2digits(seconds);

    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press # to arm");
    delay(50);
    keycount = 10;
  }

  while (keycount == 10) {
    char armkey = keypad.getKey();
    if (armkey != NO_KEY)
      Serial.println(armkey);
    if (armkey == '#') {
      tone(buzzer, 5000, 100);
      delay(50);
      tone(buzzer, 0, 100);
      delay(50);
      tone(buzzer, 5000, 100);
      delay(50);
      tone(buzzer, 0, 100);
      delay(50);
      tone(buzzer, 5000, 100);
      delay(50);
      tone(buzzer, 0, 100);
      lcd.clear();
      lcd.print("Bomb Armed!");
      lcd.setCursor(0, 1);
      lcd.print("Countdown start");
      delay(3000);
      lcd.clear();
      keycount = 0;
      timerInterval = 1000;
      display_state = false;
      for (int i = 0; i < TOTAL_WIRES; i++) {
        wireStates[i] = LOW;  // wires are initially tied to GND
      }
      random_wire(WIRES_TO_CUT, 6);
    }
  }
}

int detectWireStateChange() {
  for (int i = 0; i < TOTAL_WIRES; i++) {
    int newValue = digitalRead(CUTTABLE_WIRES[i]);
    if (newValue != wireStates[i]) {
      wireStates[i] = newValue;
      return i;
    }
  }
  return -1;
}

void displayCurrentState() {
  lcd.setCursor(0, 1);
  for (int j = 0; j < 3 - trycount; j++) {
    lcd.write(byte(0));
  }
}

bool isIncorrectWriteCut() {
  for (int i = 0; i < TOTAL_WIRES; i++) {
    if (wireStates[i] == 1 && WIRES_TO_CUT[i] == 0) {
      return true;
    }
  }
  return false;
}

bool areAllCorrectWiresCut() {
  for (int i = 0; i < TOTAL_WIRES; i++) {
    if (wireStates[i] == 0 && WIRES_TO_CUT[i] == 1) {
      return false;
    }
  }
  return true;
}

void handleWireStateChange(int wireWithNewState) {
  if (display_state) {
    lcd.setCursor(0, 2);
    if (wireStates[wireWithNewState]) {
      if (WIRES_TO_CUT[wireWithNewState]) {
        lcd.setCursor(0, 3);
        lcd.print("Correct");
        displayCurrentState();
        delay(1000);
        lcd.clear();
      } else {
        lcd.setCursor(0, 3);
        lcd.print("INCORRECT");
        tone(buzzer, 300, 200);
        tone(buzzer, 300, 200);
        trycount++;
        switch (trycount) {
          case 1:
            timerInterval = 500;
            minutes /= 2;
            seconds = 0;
            break;
          case 2:
            timerInterval = 100;
            if (mode != '3') {
              minutes /= 3;
              seconds /= 3;
              if (minutes == 0 && seconds < 10) {
                seconds = 10;
              }
            }
            break;
          case 3:
            minutes = 0;
            seconds = 0;
            break;
        }
        delay(1000);
        lcd.clear();
      }
    }
    display_state = false;
  }
}