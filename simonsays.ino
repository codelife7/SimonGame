#include <LiquidCrystal.h>
#include <Mouse.h>
#include <MelodyUtils.h>
#include "pitches.h"

//Sonar Sensor Pins
const int trigPin = 13;
const int echoPin = A2;

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int melody1[] = {
  NOTE_B3, NOTE_C4
};

int noteDurations1[] = {
  4, 8
};

//Led Pins
const int LEDy = 9; //white
const int LEDw = 8; //yellow
const int LEDg = 7; //green
const int LEDr = 6; //red

//Joystick pins
const int pX = A0;  //x-axis
const int pY = A1;  //y-axis
int x;
int y;

//piezo pin
const int pSpeaker = 10;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


//Game variables
int level = 1;  //level increases length of round increases
int timer = 1000;  //timer decreases as level increases
int life = 2; // user gains 1 life every 3 levels
bool levelClear = false; // ture if level is successfully completed

// Minimal class to replace std::vector
template<typename Data>
class Vector {
    size_t d_size; // Stores no. of actually stored objects
    size_t d_capacity; // Stores allocated capacity
    Data *d_data; // Stores data
  public:
    Vector() : d_size(0), d_capacity(0), d_data(0) {}; // Default constructor
    Vector(Vector const &other) : d_size(other.d_size), d_capacity(other.d_capacity), d_data(0) {
      d_data = (Data *)malloc(d_capacity * sizeof(Data));
      memcpy(d_data, other.d_data, d_size * sizeof(Data));
    }; // Copy constuctor
    ~Vector() {
      free(d_data);
    }; // Destructor
    Vector &operator=(Vector const &other) {
      free(d_data);
      d_size = other.d_size;
      d_capacity = other.d_capacity;
      d_data = (Data *)malloc(d_capacity * sizeof(Data));
      memcpy(d_data, other.d_data, d_size * sizeof(Data));
      return *this;
    }; // Needed for memory management
    void push_back(Data const &x) {
      if (d_capacity == d_size) resize();
      d_data[d_size++] = x;
    }; // Adds new value. If needed, allocates more space
    size_t size() const {
      return d_size;
    }; // Size getter
    Data const &operator[](size_t idx) const {
      return d_data[idx];
    }; // Const getter
    Data &operator[](size_t idx) {
      return d_data[idx];
    }; // Changeable getter
  private:
    void resize() {
      d_capacity = d_capacity ? d_capacity * 2 : 1;
      Data *newdata = (Data *)malloc(d_capacity * sizeof(Data));
      memcpy(newdata, d_data, d_size * sizeof(Data));
      free(d_data);
      d_data = newdata;
    };// Allocates double the old space
};

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pSpeaker, OUTPUT);
  pinMode(LEDy, OUTPUT);
  pinMode(LEDw, OUTPUT);
  pinMode(LEDg, OUTPUT);
  pinMode(LEDr, OUTPUT);
  pinMode(pX, INPUT);
  pinMode(pY, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Simon Says!");
}

void PinTone(int i) {
  if (i == LEDy)
  { tone(pSpeaker, 2000, 200);
  }
  if (i == LEDw)
  { tone(pSpeaker, 2100, 200);
  }
  if (i == LEDg)
  { tone(pSpeaker, 1900, 200);
  }
  if (i == LEDr)
  { tone(pSpeaker, 1800, 200);
  }
}

void Levelup_tone() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(pSpeaker, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(10);
  }
}

void Error_tone() {
  for (int thisNote = 0; thisNote < 2; thisNote++) {
    int noteDuration = 1000 / noteDurations1[thisNote];
    tone(pSpeaker, melody1[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(10);
  }
}

void Endgame_tone() {
  MelodyUtils mel(pSpeaker);
  mel.Glis(NOTE_C3, NOTE_C4, 5);
  delay(1000);
  mel.Trem(NOTE_C3, 1000, 30);
}

bool PersonNear() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  if (distance < 80) {
    Serial.println("true");
    return true;
  }
  else {
    Serial.println("false");
    return false;
  }
  if (distance >= 200 || distance <= 0) {
    Serial.println("Out of range");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
  }
  delay(500);
}

void loop() {
  if (PersonNear())
  {
    int i = 0;
    int lastLed = 0;
    Vector<int> levelpattern;   // create a vector to hold "size" int's
    // all initialized to zero
    //myvector[0] = 1234;
    while (i <= level)
    {
      int LedToBlink = random(LEDr, LEDy + 1);
      if (LedToBlink == lastLed)
      {
        continue;
      }
      else {
        tone(pSpeaker, LedToBlink * 100, 50);
        lastLed = LedToBlink;
        levelpattern.push_back( LedToBlink);
        digitalWrite(LedToBlink, HIGH);
        //PinTone(8);
        delay(timer);
        digitalWrite(LedToBlink, LOW);
        i++;
      }
    }
    Serial.print("level pattern");
    for (int i = 0; i < levelpattern.size(); i++) {
      Serial.print(levelpattern[i]);
    }
    Serial.println();

A:
    Vector<int> userpattern;
    int input = 0;
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Enter Sequence!");
    delay(100);
    lcd.setCursor(0, 1);
    lcd.print("lives : ");
    //lcd.setCursor(0,9);
    lcd.print(life);

    while (input < levelpattern.size() && life != 0)
    {
      delay(500);
      x = analogRead(A0);
      y = analogRead(A1);

      if (y < 100) {
        tone(pSpeaker, LEDy * 100, 50);
        //      lcd.print("Left");
        userpattern.push_back(LEDy);
        if (levelpattern[input] == userpattern[input])
        {
          Serial.println("matching");
          input++;
          delay(100);
          levelClear = true;
          continue;
        }
        else {
          Error_tone();
          input = 0;
          levelClear = false;
          life--;
          Serial.println("not matching");
          goto A;
        }
      }
      if (y > 900) {
        tone(pSpeaker, LEDg * 100, 50);
        //      lcd.print("right");
        userpattern.push_back(LEDg);
        if (levelpattern[input] == userpattern[input])
        {
          Serial.println("matching");
          input++;
          delay(100);
          levelClear = true;
          continue;
        }
        else {
          Error_tone();
          input = 0;
          levelClear = false;
          life--;
          Serial.println("not matching");
          goto A;
        }
      }
      if (x < 100) {
        tone(pSpeaker, LEDw * 100, 50);
        //      lcd.print("down");
        userpattern.push_back(LEDw);
        if (levelpattern[input] == userpattern[input])
        {
          Serial.println("matching");
          input++;
          delay(100);
          levelClear = true;
          continue;
        }
        else {
          Error_tone();
          input = 0;
          levelClear = false;
          life--;
          Serial.println("not matching");
          goto A;
        }

      }
      if (x > 900) {
        tone(pSpeaker, LEDr * 100, 50);
        //      lcd.print("up");
        userpattern.push_back(LEDr);
        if (levelpattern[input] == userpattern[input])
        {
          Serial.println("matching");
          input++;
          delay(100);
          levelClear = true;
          continue;
        }
        else {
          Error_tone();
          input = 0;
          levelClear = false;
          life--;
          Serial.println("not matching");
          goto A;
        }
      }
    }
    Serial.print("user pattern");
    for (int i = 0; i < userpattern.size(); i++) {
      Serial.print(userpattern[i]);
    }
    Serial.println();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Good Job!");
    lcd.setCursor(0, 1);
    lcd.print("Level: ");
    lcd.print(level + 1);

    if (levelClear) {
      delay(800);
      Levelup_tone();
      level++;
      if (timer > 150)
        timer -= 150;
      if (level % 3 == 0)
        life++;
    }
    if (life == 1 || levelClear == false) {
      endGame();
      Serial.println("game end");
    }
  }
}

void endGame() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("You Lost!");
  lcd.setCursor(0, 1);
  lcd.print("Click Reset!!");
  Endgame_tone();
  delay(10000000);
}
