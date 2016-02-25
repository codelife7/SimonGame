#include <LiquidCrystal.h>
#include <Mouse.h>
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
  //lcd.print("hello, world!");
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

void loop() {
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


  Vector<int> userpattern;
  int input = 0;
  lcd.setCursor(0, 0);
  delay(100);

  while (input < levelpattern.size() && life != 0)
  {
    delay(500);
    x = analogRead(A0);
    y = analogRead(A1);

    if (y < 100) {
      lcd.print("Left");
      userpattern.push_back(LEDy);
      if (levelpattern[input] == userpattern[input])
      {
        Serial.println("matching");
        input++;
        delay(100);
        lcd.clear();
        levelClear = true;
        continue;
      }
      else {
        input = 0;
        levelClear = false;
        life--;
        Serial.println("not matching");
      }
    }
    if (y > 900) {
      lcd.print("right");
      userpattern.push_back(LEDg);
      if (levelpattern[input] == userpattern[input])
      {
        Serial.println("matching");
        input++;
        delay(100);
        lcd.clear();
        levelClear = true;
        continue;
      }
      else {
        input = 0;
        levelClear = false;
        life--;
        Serial.println("not matching");
      }
    }
    if (x < 100) {
      lcd.print("down");
      userpattern.push_back(LEDw);
      if (levelpattern[input] == userpattern[input])
      {
        Serial.println("matching");
        input++;
        delay(100);
        lcd.clear();
        levelClear = true;
        continue;
      }
      else {
        input = 0;
        levelClear = false;
        life--;
        Serial.println("not matching");
      }

    }
    if (x > 900) {
      lcd.print("up");
      userpattern.push_back(LEDr);
      if (levelpattern[input] == userpattern[input])
      {
        Serial.println("matching");
        input++;
        delay(100);
        lcd.clear();
        levelClear = true;
        continue;
      }
      else {
        input = 0;
        levelClear = false;
        life--;
        Serial.println("not matching");
      }
    }
  }
  Serial.print("user pattern");
  for (int i = 0; i < userpattern.size(); i++) {
    Serial.print(userpattern[i]);
  }
  Serial.println();


  if (levelClear) {
    level++;
    if (level % 3 == 0)
      life++;
  }
  else if (life = 0 || levelClear == false) {
    endGame();
    Serial.println("game end");
  }
}

void endGame() {
  lcd.clear();
  lcd.print("Click reset button ");
  delay(1000000);
}
