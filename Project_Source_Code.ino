#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define Password_Length 6

Servo myservo;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); // Adjust pin numbers based on your LCD connections



const int buzzer = 8; // Adjust pin number based on your buzzer connection

char Data[Password_Length];
char Master[Password_Length] = "12A45";
byte data_count = 0, master_count = 0;

bool Pass_is_good;
bool door = false;
char customKey;


const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {0, 1, 2, 3}; // Adjust pin numbers based on your keypad connections
byte colPins[COLS] = {4, 5, 6, 7}; // Adjust pin numbers based on your keypad connections

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  pinMode(buzzer, OUTPUT);
  myservo.attach(9); // Adjust pin number based on your servo connection
  ServoClose();
  lcd.begin(20, 4); // Adjust column and row numbers based on your LCD size
  lcd.print("Protected Door");
  loading("Loading");
  lcd.clear();
}

void loop()
{
  if (door == true)
  {
    customKey = customKeypad.getKey();
    if (customKey == '#')
    {
      lcd.clear();
      ServoClose();
      lcd.print("Door is closed");
      delay(3000);
      door = false;
    }
  }
  else
    Open();
}

void loading (char msg[]) {
  lcd.setCursor(0, 1);
  lcd.print(msg);

  for (int i = 0; i < 9; i++) {
    delay(1000);
    lcd.print(".");
  }
}

void clearData()
{
  while (data_count != 0)
  {
    Data[data_count--] = 0;
  }
  return;
}

void ServoClose()
{
  for (int pos = 90; pos >= 0; pos -= 10) {
    myservo.write(pos);
  }
}

void ServoOpen()
{
  for (int pos = 0; pos <= 90; pos += 10) {
    myservo.write(pos);
  }
}

void Open()
{
  lcd.setCursor(0, 0);
  lcd.print("Enter code:");

  customKey = customKeypad.getKey();
  if (customKey)
  {
    // Print asterisk (*) on LCD instead of actual character
    lcd.setCursor(data_count, 1);
    lcd.print('*');

    Data[data_count] = customKey;
    data_count++;
  }

  if (data_count == Password_Length - 1)
  {
    if (!strcmp(Data, Master))
    {
      lcd.clear();
      ServoOpen();
      lcd.print(" Door is Open ");
      door = true;
      delay(5000);
      tone(buzzer, 1000); //  1KHz sound signal
      delay(1000);        // ...for 1 sec
      noTone(buzzer);     // Stop sound...
      delay(1000);
      loading("Waiting");
      lcd.clear();
      lcd.print(" Time is up! ");
      delay(1000);
      ServoClose();
      door = false;
    }
    else
    {
      lcd.clear();
      tone(buzzer, 1000);
      lcd.print(" Wrong Password ");
      delay(30000);
      noTone(buzzer);
      door = false;
    }
    delay(1000);
    lcd.clear();
    clearData();
  }
}
