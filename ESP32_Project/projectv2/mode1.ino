int number1, number2, ans;
void random_number() {
  srand(time(NULL));
  number1 = rand() % 4999;
  number2 = rand() % 4999;
  ans = number1 + number2;
}

void mode1_enter_password() {
  char disarmcode = keypad.getKey();
  timerAlarmWrite(My_timer, timerInterval * 1000, true);

  if (minutes <= 0 && seconds <= 0) {
    lcd.noBlink();
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print("*** BOOM ***");
    game_finish = true;

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
  }

  if (game_finish == false) {
    while (keycount < 4) {
      lcd.setCursor(0, 1);
      lcd.print("Code: ");
      lcd.setCursor(0, 0);
      lcd.print("What is ");
      lcd.print(number1);
      lcd.print(" + ");
      lcd.print(number2);

      check_face();

      char disarmcode = keypad.getKey();
      lcd.setCursor(keycount + 6, 1);
      lcd.blink();
      delay(100);
      //lcd.noBlink();

      if (disarmcode == '#') {
        tone(buzzer, 5000, 100);
        keycount = 0;
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Code: ");
      } else if (disarmcode != NO_KEY) {
        lcd.setCursor(keycount + 6, 1);
        lcd.print(disarmcode);
        int d_code = atoi(&disarmcode);
        entered[keycount] = d_code;
        delay(100);
        tone(buzzer, 5000, 100);
        delay(100);
        lcd.setCursor(keycount + 6, 1);
        lcd.print("*");
        keycount++;
        lcd.setCursor(keycount + 6, 1);
      }
      show_time();
      lcd.setCursor(0, 3);
      for (int j = 0; j < 3 - trycount; j++) {
        lcd.write(byte(0));
      }
      if (minutes == 0 && seconds == 0)
        break;
    }
    if (keycount == 4) {
      if (entered[0] == password[0] && entered[1] == password[1] && entered[2] == password[2] && entered[3] == password[3]) {
        lcd.noBlink();
        lcd.clear();
        lcd.home();
        lcd.print("Bomb Defused!");
        lcd.setCursor(0, 1);
        lcd.print("Well Done!");
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
        keycount = 0;
        digitalWrite(G_LED, LOW);
        timerInterval = 1000000;
        timerAlarmWrite(My_timer, timerInterval * 1000, true);
        game_finish = true;

        delay(5000);
        lcd.clear();
        lcd.setCursor(5, 1);
        lcd.print("Press 'A'");
        lcd.setCursor(3, 2);
        lcd.print("to reset bomb");

      } else {
        lcd.noBlink();
        lcd.clear();
        lcd.home();
        lcd.print("Wrong Password!");
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
            minutes /= 3;
            seconds /= 3;
            if (minutes == 0 && seconds < 10) {
              seconds = 10;
            }
            break;
          case 3:
            minutes = 0;
            seconds = 0;
            break;
        }
        timerAlarmWrite(My_timer, timerInterval * 1000, true);
        delay(1000);
        keycount = 0;
      }
    }
  }
}

void set_time_mode1() {
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
      set_password();
      //last_time = millis();
    }
  }
}

void set_password() {

  random_number();

  for (int i = 3; i >= 0; i--) {
    password[i] = ans % 10;
    ans /= 10;
  }

  for (int j = 0; j < 4; j++) {
    Serial.print(password[j]);
    Serial.print(" ");
  }
}