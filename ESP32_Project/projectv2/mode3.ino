
// //Line Token: s8SkTlewn3ckqNAp7pqogJcCBc0XjAAA3oqNEB7dmT4
char m_code[14] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D' };

char mose[14][10] = { "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", ".-", "-...", "-.-.", "-.." };

char password_mode3[4];
char entered_mode3[4];

bool game_finish1 = false, game_finish2 = false, sent_to_line = false;
size_t totalLength = 0;
int count_finish = 0;
char* mose_arr[10];
char* mose_wire[10];
char r_alpha[15];
char r_wire[15];

void mode3_wifi_mix() {
  timerAlarmWrite(My_timer, timerInterval * 1000, true);

  if (minutes <= 0 && seconds <= 0 || trycount == 3) {
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
    return;
  }

  if (game_finish == false) {
    if (game_finish1 == false) {
      while (keycount < 4) {

        check_face();

        lcd.setCursor(0, 0);
        lcd.print("Code: ");

        char disarmcode = keypad.getKey();
        lcd.setCursor(keycount + 6, 0);
        lcd.blink();
        delay(100);

        if (disarmcode == '#') {
          tone(buzzer, 5000, 100);
          keycount = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Code: ");
        } else if (disarmcode != NO_KEY) {
          lcd.setCursor(keycount + 6, 0);
          lcd.print(disarmcode);
          //int d_code = atoi(&disarmcode);
          entered_mode3[keycount] = disarmcode;
          delay(100);
          tone(buzzer, 5000, 100);
          delay(100);
          lcd.setCursor(keycount + 6, 0);
          lcd.print("*");
          keycount++;
          lcd.setCursor(keycount + 6, 0);
        }

        show_time();
        if (game_finish2 == false) {
          check_face();
          if (wireWithNewState >= 0) {
            handleWireStateChange(wireWithNewState);
          }
          if (areAllCorrectWiresCut()) {
            // lcd.setCursor(0, 3);
            // lcd.write(byte(1));
            count_finish++;
            // Serial.print("Count: ");
            // Serial.println(count_finish);
            game_finish2 = true;
          }
        }
        if (minutes == 0 && seconds == 0)
          break;

        lcd.setCursor(0, 1);
        for (int j = 0; j < 3 - trycount; j++) {
          lcd.write(byte(0));
        }
        lcd.setCursor(0, 3);
        for (int j = 0; j < count_finish; j++) {
          lcd.write(byte(1));
        }
      }
    }

    if (keycount == 4) {
      if (entered_mode3[0] == password_mode3[0] && entered_mode3[1] == password_mode3[1] && entered_mode3[2] == password_mode3[2] && entered_mode3[3] == password_mode3[3]) {
        keycount = 0;
        digitalWrite(R_LED, LOW);
        lcd.setCursor(0, 3);
        lcd.write(byte(1));
        count_finish++;
        Serial.print("Count: ");
        Serial.println(count_finish);
        game_finish1 = true;

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
            break;
          case 3:
            minutes = 0;
            seconds = 0;
            break;
        }
        timerAlarmWrite(My_timer, timerInterval * 1000, true);
        delay(1000);
        keycount = 0;
        lcd.clear();
      }
    }

    if (game_finish1 == true && game_finish2 == false) {
      show_time();
      lcd.setCursor(0,0);
      lcd.print("Code: ****");

      check_face();

      if (wireWithNewState >= 0) {
        handleWireStateChange(wireWithNewState);
      }

      lcd.setCursor(0, 1);
      for (int j = 0; j < 3 - trycount; j++) {
        lcd.write(byte(0));
      }
      lcd.setCursor(0, 3);
      for (int j = 0; j < count_finish; j++) {
        lcd.write(byte(1));
      }
      if (areAllCorrectWiresCut()) {
        count_finish++;
        game_finish2 = true;
      }
    }

    if (game_finish1 == true && game_finish2 == true) {
      timerInterval = 1000000;
      timerAlarmWrite(My_timer, timerInterval * 1000, true);
      lcd.clear();
      lcd.noBlink();
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
}

void sent_question_to_line() {
  if (!sent_to_line) {
    lcd.print("Sent Quiz To Line...");
    LINE.notify("------------------------------");
    LINE.notify("โหมดที่ 3 แก้ปริศนาโดยใช้รหัสมอส");
    LINE.notify("คุณมีเวลา 5 นาทีในการแก้ปริศนาโดยใช้ตารางรหัสมอส ข้างต้น");
    LINE.notifyPicture("https://scoutlife.org/wp-content/uploads/2007/02/morsecode-1.jpg?w=700");


    char* result = concatenateStrings(mose_arr, 4);
    if (result) {
      LINE.notify("ข้อที่ 1 กรุณาใส่รหัส");
      LINE.notify(result);
    }
    char* wire_result = concatenateStrings(mose_wire, 3);
    if (wire_result) {
      LINE.notify("ข้อที่ 2: กรุณาตัดสายไฟในตำแหน่งที่ระบุ");
      LINE.notify(wire_result);
    }

    sent_to_line = true;
    lcd.clear();
  }
}

void set_time_mode3() {
  minutes = 5;
  seconds = 0;
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
      game_finish1 = false;
      game_finish2 = false;
      sent_to_line = false;
      count_finish = 0;

      for (int i=0; i < TOTAL_WIRES ; i++) {
        WIRES_TO_CUT[i] = 0;
        wireStates[i] = LOW;
      }
      set_password_mode3();
      random_wire_mode3(WIRES_TO_CUT);
      sent_question_to_line();
    }
  }
}

void set_password_mode3() {
  srand(time(NULL));
  for (int i = 0; i < 4; i++) {
    int num_alpha = random_alpha();
    password_mode3[i] = m_code[num_alpha];
    mose_arr[i] = strdup(mose[num_alpha]);
  }

  Serial.print("Q1 Result: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(password_mode3[i]);
  }
  Serial.println();
}

void random_wire_mode3(int WIRES_TO_CUT[]) {
  srand(time(NULL));
  int set_wire = 0;
  while(set_wire < 3) {
    int num_wire = random_wire_posi();
    if (WIRES_TO_CUT[5-num_wire] != 1) {
      WIRES_TO_CUT[5-num_wire] = 1;
      r_wire[set_wire] = m_code[num_wire];
      mose_wire[set_wire] = strdup(mose[num_wire]);
      set_wire++;
    }    
  }
  Serial.print("Q2 Result: ");
  for (int i = 0; i < 3; i++)
    Serial.print(r_wire[i]);
  Serial.println();
  Serial.print("Q2 Result: ");
  for (int i = 0; i < TOTAL_WIRES; i++)
    Serial.print(WIRES_TO_CUT[i]);
  Serial.println();
}

int random_alpha() {
  int randomNumber = rand() % 14;
  return randomNumber;
}

int random_wire_posi() {
  int randomNumber = rand() % 6;
  return randomNumber;
}

char* concatenateStrings(char* strings[], int numStrings) {
  // Calculate the total length of the concatenated string
  int totalLength = 0;
  for (int i = 0; i < numStrings; i++) {
    totalLength += strlen(strings[i]);
  }

  // Allocate memory for the concatenated string
  char* result = (char*)malloc(totalLength + 1);  // +1 for the null terminator

  if (result) {
    // Initialize the result string
    result[0] = '\0';

    // Concatenate each string in the array
    for (int i = 0; i < numStrings; i++) {
      strcat(result, strings[i]);
      if (i < numStrings - 1) {
        strcat(result, " ");  // Add a space unless it's the last string
      }
    }
  }

  return result;
}