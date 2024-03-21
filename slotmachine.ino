#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

hd44780_I2Cexp lcd;
byte cirese[8] = {
  0b11100,
  0b01110,
  0b00001,
  0b00010,
  0b00110,
  0b01010,
  0b11011,
  0b11011,
  };

 byte emptyHeart[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b10101,
  0b10001,
  0b01010,
  0b00100,
  0b00000,
  };

 byte heart[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  };
// Masca pentru al doilea caracter
byte spades[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b11111,
  0b10101,
  0b01110,
  0b00000,
  };

  byte septar[8] = {
  0b00000,
  0b11111,
  0b10001,
  0b00001,
  0b00010,
  0b00100,
  0b00100,
  0b00000,
  };

  byte bell[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b01110,
  0b01110,
  0b11111,
  0b00100,
  0b00000,
  };

  byte dolar[8] = {
  0b00000,
  0b00100,
  0b01111,
  0b10100,
  0b01110,
  0b00101,
  0b11110,
  0b00100,
  };

  const int pinButtonR = 2;
  const int pinButtonN = 3;
  const int pinButtonRoll = 4;
  const int pinButtonBet = 5;
  int bet = 1;
  float balance = 50;
  float winning = 0;
  int symbols[3] = {random(4, 6), random(4, 6),random(4, 6)};
  bool matrixIsInitiated = false;
  bool modDublaj = false;

void setup() {
  lcd.begin(20, 4);
  lcd.backlight(); // activate the backlight
  lcd.createChar(0, cirese);
  lcd.createChar(1, bell);
  lcd.createChar(2, septar);
  lcd.createChar(3, dolar);
  lcd.createChar(4, heart);
  lcd.createChar(5, emptyHeart);

  pinMode(pinButtonR, INPUT);
  pinMode(pinButtonN, INPUT);
  pinMode(pinButtonRoll, INPUT);
  pinMode(pinButtonBet, INPUT);
  digitalWrite(pinButtonR, HIGH);
  digitalWrite(pinButtonN, HIGH);
  digitalWrite(pinButtonRoll, HIGH);
  digitalWrite(pinButtonBet, HIGH);

  lcd.setCursor(0,0);
  lcd.print(balance);
  lcd.setCursor(0,2);
  lcd.print(bet);
}

void loop()
{
  lcd.setCursor(0,0);

  int arr[3][5];
  if (digitalRead(pinButtonRoll) == LOW) {
    balance += winning;
    modDublaj = false;
    if(balance - bet >= 0){
      balance -= bet;
      lcd.clear();
      generateRandomMatrix(arr);
      printRandomMatrix(arr);
      winning = checkPaylines(arr);
      lcd.setCursor(0,0);
      lcd.print(balance);
      lcd.setCursor(0,1);
      lcd.print(winning);
      lcd.setCursor(0,2);
      lcd.print(bet);    
    }
    delay(500);
  }
  if (digitalRead(pinButtonBet) == LOW) {
    increaseBet();
    lcd.clear();
    if (matrixIsInitiated == true){
      printRandomMatrix(arr);
      winning = checkPaylines(arr);
    }
    lcd.setCursor(0,0);
    lcd.print(balance);
    lcd.setCursor(0,1);
    lcd.print(winning);
    lcd.setCursor(0,2);
    lcd.print(bet);
    delay(500);
  }

  if ((modDublaj == false) && (winning > 0) && (digitalRead(pinButtonN) == LOW || digitalRead(pinButtonR) == LOW)) {
    modDublaj = true;
    doubleWin();
    delay(500);
  }
  if ((modDublaj == true) && (digitalRead(pinButtonR) == LOW)){
    int newChoice = 5;
    for(int i = 2; i > 0; i--)
      symbols[i] = symbols[i - 1];
    symbols[0] = random(4, 6);

    if( newChoice == symbols[0] ) {
      winning = winning * 2;
    }
    else{
      winning = 0;
      modDublaj = false;  
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(balance);
    lcd.setCursor(0,1);
    lcd.print(winning);
    lcd.setCursor(0,2);
    lcd.print(bet);
    lcd.setCursor(12,0);
    lcd.write(symbols[0]);
    lcd.write(symbols[1]);
    lcd.write(symbols[2]);
    lcd.setCursor(13,1);
    lcd.write(5);
    
    delay(500);
  }

  if ((modDublaj == true) && (digitalRead(pinButtonN) == LOW)){
    int newChoice = 4;
    for(int i = 2; i > 0; i--)
      symbols[i] = symbols[i - 1];
    symbols[0] = random(4, 6);
    lcd.setCursor(13,1);
    lcd.write(4);

    if( newChoice == symbols[0] ) {
      winning = winning * 2;
    }
    else{
      winning = 0;
      modDublaj = false;  
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(balance);
    lcd.setCursor(0,1);
    lcd.print(winning);
    lcd.setCursor(0,2);
    lcd.print(bet);
    lcd.setCursor(12,0);
    lcd.write(symbols[0]);
    lcd.write(symbols[1]);
    lcd.write(symbols[2]); 
    lcd.setCursor(13,1);
    lcd.write(4);
    
    delay(500);
  }
}

void doubleWin(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(balance);
  lcd.setCursor(0,1);
  lcd.print(winning);
  lcd.setCursor(0,2);
  lcd.print(bet);

  lcd.setCursor(12,0);
  lcd.write(symbols[0]);
  lcd.write(symbols[1]);
  lcd.write(symbols[2]);
}

void increaseBet(){
  if ( bet == 1)
    bet = 2;
  else if ( bet == 2)
    bet = 5;
  else if ( bet == 5)
    bet = 10;
  else if ( bet == 10)
    bet = 1;
}

void generateRandomMatrix(int arr[3][5]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 5; j++) {
      arr[i][j] = random(0, 5);
    }
  }
  matrixIsInitiated = true;
}

void printRandomMatrix(int arr[3][5]){

  lcd.setCursor(15, 0);
  lcd.write(arr[0][0]);
  lcd.write(arr[0][1]);
  lcd.write(arr[0][2]);
  lcd.write(arr[0][3]);
  lcd.write(arr[0][4]);
  
  lcd.setCursor(15, 1);
  lcd.write(arr[1][0]);
  lcd.write(arr[1][1]);
  lcd.write(arr[1][2]);
  lcd.write(arr[1][3]);
  lcd.write(arr[1][4]);
  
  lcd.setCursor(15, 2);
  lcd.write(arr[2][0]);
  lcd.write(arr[2][1]);
  lcd.write(arr[2][2]);
  lcd.write(arr[2][3]);
  lcd.write(arr[2][4]);
}


float checkPaylines(int arr[3][5]) {
  float totalWinnings = 0;

  // Check horizontal paylines
  for (int i = 0; i < 3; i++) {
    int count = 0;
    for (int j = 0; j < 5; j++) {
      if (arr[i][0] == arr[i][j]) {
        count++;
      } else {
        break;
      }
    }
      int value = 0;
      if (arr[i][0] == 0)
        value = 1;
      else if(arr[i][0] == 2)
        value = 5;
      else 
        value = 3;
      if (count == 3) {
        totalWinnings += 1.25 * value;
      } else if (count == 4) {
        totalWinnings += 1.75 * value;
      } else if (count == 5) {
        totalWinnings += 2.50 * value;
      }
  }

  int countDiag = 0;
  if (arr[0][0] == arr[1][1] && arr[1][1] == arr[2][2]) {
    countDiag = 3;
  }

  if (arr[0][0] == arr[1][1] && arr[1][1] == arr[2][2] && arr[2][2] == arr[1][3]) {
    countDiag = 4;
  }

  if (arr[0][0] == arr[1][1] && arr[1][1] == arr[2][2] && arr[2][2] == arr[1][3] && arr[1][3] == arr[0][4]) {
    countDiag = 5;
  }

  int value = 0;
  if (arr[0][0] == 0)
    value = 1;
  else if(arr[0][0] == 2)
    value = 5;
  else 
    value = 3;
  if (countDiag == 3) {
    totalWinnings += 1.25 * value;
  }
  else if (countDiag == 4) {
    totalWinnings += 1.75 * value;
  }
  else if (countDiag == 5) {
    totalWinnings += 2.50 * value;
  }

  countDiag = 0;
  if (arr[2][0] == arr[1][1] && arr[1][1] == arr[0][2]) {
    countDiag = 3;
  }

  if (arr[2][0] == arr[1][1] && arr[1][1] == arr[0][2] && arr[0][2] == arr[1][3]) {
    countDiag = 4;
  }

  if (arr[2][0] == arr[1][1] && arr[1][1] == arr[0][2] && arr[0][2] == arr[1][3] && arr[1][3] == arr[2][4]) {
    countDiag = 5;
  }

  value = 0;
  if (arr[2][0] == 0)
    value = 1;
  else if(arr[2][0] == 2)
    value = 5;
  else 
    value = 3;
    
  if (countDiag == 3) {
    totalWinnings += 1.25 * value;
  }
  else if (countDiag == 4) {
    totalWinnings += 1.75 * value;
  }
  else if (countDiag == 5) {
    totalWinnings += 2.50 * value;
  }

return totalWinnings * bet;
}
