#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int motorButonu = 22;
int kemerDurumu = 23;
int kapiDurumu = 24;
int sicaklikSensoru = A0;
int isikSensoru = A1;
int yakitSensoru = A2;

int kirmiziLED = 25;
int maviLED = 26;
int sariLED = 27;
int rgbKirmizi = 8;
int rgbYesil = 9;
int rgbMavi = 10;
int buzzer = 30;
int motorCikisi = 31;
int fan = 32;

bool motorCalisiyor = false;

bool farKontrol = false;

void setup() {
  lcd.begin(16, 2);

  pinMode(motorButonu, INPUT);
  pinMode(kemerDurumu, INPUT);
  pinMode(kapiDurumu, INPUT);

  pinMode(kirmiziLED, OUTPUT);
  pinMode(maviLED, OUTPUT);
  pinMode(sariLED, OUTPUT);
  pinMode(rgbKirmizi, OUTPUT);
  pinMode(rgbYesil, OUTPUT);
  pinMode(rgbMavi, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(motorCikisi, OUTPUT);
  pinMode(fan, OUTPUT);

  herSeyiKapat();
}

void loop() {
  int motorButonOku = digitalRead(motorButonu);
  int kemerOku = digitalRead(kemerDurumu);
  int kapiAcikMi = digitalRead(kapiDurumu);
  float sicaklik = analogRead(sicaklikSensoru) * (5.0 / 1023.0) * 100;
  int isikSeviyesi = analogRead(isikSensoru);
  int yakitSeviyesi = map(analogRead(yakitSensoru), 0, 1023, 0, 100);

  if (motorButonOku == LOW) {
    motorCalisiyor = false;
    herSeyiKapat();
  }

  if (motorButonOku == HIGH) {
    if (kapiAcikMi == LOW) {
      lcd.setCursor(0, 0);
      lcd.print("Kapi Acik");
      delay(500);
      analogWrite(rgbKirmizi, 255);
      analogWrite(rgbYesil, 0);
      analogWrite(rgbMavi, 127);
      motorCalisiyor = false;
    } else if (kemerOku == LOW) {
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("Emniyet Kemeri! ");
      delay(500);
      digitalWrite(kirmiziLED, HIGH);
      digitalWrite(buzzer, HIGH);
      motorCalisiyor = false;
    } else {
      motorCalisiyor = true;
      digitalWrite(kirmiziLED, LOW);
      digitalWrite(buzzer, LOW);
      analogWrite(rgbKirmizi, 0);
      analogWrite(rgbYesil, 0);
      analogWrite(rgbMavi, 0);
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
  }

  digitalWrite(motorCikisi, motorCalisiyor ? HIGH : LOW);

  if (motorCalisiyor) {
    if (yakitSeviyesi < 10 && yakitSeviyesi > 5) {
      digitalWrite(sariLED, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("Uyari: ");
      lcd.setCursor(0, 1);
      lcd.print("Yakit Dusuk - %");
      lcd.print(yakitSeviyesi);
      delay(500);
    } else if (yakitSeviyesi < 5 && yakitSeviyesi > 0) {
      digitalWrite(sariLED, millis() / 500 % 2);
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("KRITIK: %");
      lcd.print(yakitSeviyesi);
      lcd.print("             ");
      delay(500);
    } else if (yakitSeviyesi == 0) {
      motorCalisiyor = false;
      digitalWrite(motorCikisi, LOW);
      herSeyiKapat();
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("Yakit Bitti     ");
      lcd.setCursor(0, 1);
      lcd.print("Motor Durdu     ");
      delay(500);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      digitalWrite(sariLED, LOW);
    }
  }

  if (motorCalisiyor) {
    if (sicaklik > 25.9) {
      digitalWrite(fan, HIGH);
      lcd.setCursor(0, 1);
      lcd.print((int)sicaklik);
      lcd.print("C-Klima Acik");
      delay(500);
    } else {
      digitalWrite(fan, LOW);
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }

    if (isikSeviyesi <= 250) {
      digitalWrite(maviLED, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Farlar Acik     ");
      delay(500);
      lcd.setCursor(0, 1);
      lcd.print("                ");
      farKontrol = true;
    } else {
      if (farKontrol) {
        digitalWrite(maviLED, LOW);
        lcd.setCursor(0, 1);
        lcd.print("Farlar Kapandi  ");
        delay(500);
        lcd.setCursor(0, 1);
        lcd.print("                ");
        farKontrol = false;
      } else {
        digitalWrite(maviLED, LOW);
        lcd.setCursor(0, 1);
        lcd.print("                ");
      }
    }
  }

  delay(200);
}

void herSeyiKapat() {
  digitalWrite(kirmiziLED, LOW);
  digitalWrite(maviLED, LOW);
  digitalWrite(sariLED, LOW);
  analogWrite(rgbKirmizi, 0);
  analogWrite(rgbYesil, 0);
  analogWrite(rgbMavi, 0);
  digitalWrite(buzzer, LOW);
  digitalWrite(motorCikisi, LOW);
  digitalWrite(fan, LOW);
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}
