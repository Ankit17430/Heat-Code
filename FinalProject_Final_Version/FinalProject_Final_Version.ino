#include <Wire.h>

#include <LiquidCrystal_I2C.h>

#include <Keypad.h>

const byte ROWS = 4;

const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'D','C','B','A'  },
  {'#','9','6','3'  },
  {'0','8','5','2'  },
  {'*','7','4','1'  },
};

byte rowPins[ROWS] = {23, 25, 27, 29};

byte colPins[COLS] = {31, 33, 35, 37};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //

float tempvalue;
float val = 0;

void setup() {
  pinMode (3, OUTPUT);
  pinMode (7, OUTPUT);
  lcd.begin(16, 2);
  digitalWrite(3, LOW);
}


int TempC(float input){
  
   if (tempvalue < input - 0.5) {
      analogWrite(3, 255);

    } else if (tempvalue < input) {
      analogWrite(3, 100);
      tone(7, 500, 250);
      
    } else if (tempvalue > input) {
      analogWrite(3, 0);
    }
}

int TempF(float input){
  
    if (tempvalue < input - 0.7) {
      analogWrite(3, 255);
      
    } else if (tempvalue < input) {
      analogWrite(3, 100);
      tone(7, 500, 250);
      
    } else if (tempvalue > input) {
      analogWrite(3, 0);
    } 
}

void loop() {
  start: 
  lcd.setCursor(0, 0);
  lcd.print("1 for Celsius");
  
  lcd.setCursor(0, 1);
  lcd.print("2 for Fahrenheit");
  
  char input = keypad.getKey();

  while (input == '1') {
    lcd.clear();
    
    while(keypad.getKey() != '#'){
      val = analogRead(A0); 
      tempvalue = val * 500/1024;
      lcd.setCursor(0, 0);
      lcd.print("Temp in C: ");
      lcd.print(tempvalue);
      analogWrite(3, 0);
    }
    
    lcd.setCursor(0, 1);
    int inputC = 0;
    inputC = input_Value();
    
    int timeStart = millis() / 1000;
    lcd.noCursor();
    
    while (keypad.getKey() != '#'){
      val = analogRead(A0); 
      tempvalue = val * 500/1024;

      lcd.setCursor(0, 0);
      lcd.print("Temp in C: ");
      lcd.print(tempvalue);

      TempC(inputC);
      int timeCurrent = millis() / 1000;
      int timeGone = timeCurrent - timeStart;
      lcd.setCursor(6, 1);
      lcd.print(timeGone);
      lcd.print('s');
      delay(500);
    }
    lcd.clear();
    goto start;
    
    }

  while (input == '2') {
    lcd.clear();

    while(keypad.getKey() != '#'){
      val = analogRead(A0); 
      tempvalue = val * 500/1024 * 1.8 + 32.0;
      lcd.setCursor(0, 0);
      lcd.print("Temp in F: ");
      lcd.print(tempvalue);
      analogWrite(3, 0);
    }
    
    lcd.setCursor(0, 1);
    int inputF = 0;
    inputF = input_Value();
    
    int timeStart = millis() / 1000;
    lcd.noCursor();
    
    while (keypad.getKey() != '#'){
      val = analogRead(A0); 
      tempvalue = val * 500/1024 * 1.8 + 32.0;

      lcd.setCursor(0, 0);
      lcd.print("Temp in F: ");
      lcd.print(tempvalue);

      TempF(inputF);
      int timeCurrent = millis() / 1000;
      int timeGone = timeCurrent - timeStart;
      lcd.setCursor(6, 1);
      lcd.print(timeGone);
      lcd.print('s');
      delay(500);
    }
    
    lcd.clear();
    goto start;
    
  }
}

int input_Value()
{
  unsigned int n=0; //holding no. of chars inputted
  unsigned char tmp=0;
  char input_temp[10];
  int input_len=0, i;

  tmp = keypad.getKey();

  lcd.cursor(); //show cursor
  lcd.blink();  //blink cursor
  while(tmp != '#')
  {
    if(tmp == '*')
    {
      lcd.rightToLeft();
      
      for(i=0; i<n; i++)
        lcd.print(" ");

      lcd.leftToRight();
      lcd.print(" ");
      
      lcd.rightToLeft();
      lcd.print(" ");
      lcd.leftToRight();
      n = 0;
    }
    else if(tmp != NO_KEY && tmp != '*')
    {
      input_temp[n++] = tmp;
      lcd.print(input_temp[n-1]);
    }

    tmp = keypad.getKey();
  }

  lcd.noCursor(); //no cursor

  return convert_chars_to_int(input_temp, n);
}

int convert_chars_to_int(char *buf, int len)
{
  int i, value=0, j=len;

  for(i=0; i<j; i++)
    value += (pow_int(10, --len) * (buf[i] - '0'));

  return value;
}

int pow_int(int base, int expo)
{
  int value =1, i;

  for(i=0; i<expo; i++)
    value *= 10;

  return value;
}
