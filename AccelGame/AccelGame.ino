#include <Adafruit_CircuitPlayground.h>



#define  MINLVL 1
#define MAXLVL 10
#define STARTLVL 5

float X, Y, angle;
int currentPos;
int colorCode;
bool wasInSetup;
int currentLevel;


void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();

  wasInSetup = false;
  currentLevel=STARTLVL;
}

void loop() {

  //Setup Modus
  if (CircuitPlayground.slideSwitch())
  {
    if(!wasInSetup)
    {
      //It's first time now in Setup!  
      ShowCurrentLevel(currentLevel);
    }
    wasInSetup = true;
    SetupDifficulty();

  }
  //Game Mode
  else
  {
    if (wasInSetup)
    {
      //Daten speichern!
      Serial.println("Saving Data NOW!");
      ClearLights();
      wasInSetup = false;
    }

    //Values from the sensor
    X = CircuitPlayground.motionX();
    Y = CircuitPlayground.motionY();
    if (isValidTilt(X, Y))
    {
      angle = GetAngle(X, Y);
      currentPos = getPos(angle);
      colorCode = CalcColor(angle);
      DrawLights(currentPos, colorCode);
    }
  }



  delay(100);
}

//Menue to setup the difficulty
void SetupDifficulty()
{

  int tmplvl = currentLevel;
  if (CircuitPlayground.rightButton())
  {
    tmplvl += 1;
  }
  if (CircuitPlayground.leftButton())
  {
    tmplvl -= 1;
  }
  delay(100);

  int newLevel = constrain(tmplvl, MINLVL, MAXLVL);
  //show and play only if change is there
  if (currentLevel != newLevel)
  {
    playSound(newLevel);
    currentLevel = newLevel;

    ShowCurrentLevel(currentLevel);
  }

}

void playSound(int lvl)
{
  CircuitPlayground.playTone((lvl + 1) * 150, 100);
}

//Shows the current level on the wheel
void ShowCurrentLevel(int lvl)
{
  Serial.print("current Level is");
  Serial.println(lvl);

  CircuitPlayground.clearPixels();

  for (int i = 0; i <= lvl; i++)
  {
    int scaler = i * 25;
    //Level starts with 1, pixels start with 0
    CircuitPlayground.setPixelColor(i - 1, scaler, 255 - scaler, 0);
  }
}

bool isValidTilt(float x, float y)
{
  bool valid=false;
  float hyp = sqrt(pow(x, 2) + pow(y, 2));
  Serial.println(hyp);
  if (hyp < (MAXLVL-currentLevel))
  {
    valid=true;
  }
  else
  {
    valid=false; 
    CircuitPlayground.playTone(100,100);
  }
  CircuitPlayground.redLED(!valid);
  
  return valid;
   
}

float GetAngle(float x, float y)
{
  float w = (atan2(y, x) * 180) / PI;
  return w;
}


//Draws the lights
void DrawLights(int index, int cCode)
{
  ClearLights();
  CircuitPlayground.setPixelColor(index, cCode);

}

void ClearLights()
{
  CircuitPlayground.clearPixels();
}

//Calculates a color, dependant on the angle
int CalcColor(double angle)
{
  //make it positive
  float newAngle = angle + 180.0;
  uint8_t  colorSeed = (uint8_t)(newAngle * 0.7);
  return CircuitPlayground.colorWheel(colorSeed);
}

//Maps the angle to the position of the LEDs
int getPos(float angle)
{
  int pos;
  if (angle >= 0 && angle < 36) {
    pos = 8;
  } else if (angle >= 36 && angle < 72) {
    pos = 9;
  } else if (angle >= 72 && angle < 108) {
    pos = 0;
  } else if (angle >= 108 && angle < 144) {
    pos = 1;
  } else if (angle >= 144 && angle < 180) {
    pos = 2;
  } else if (angle >= -180 && angle < -144) {
    pos = 3;
  } else if (angle >= -144 && angle < -108) {
    pos = 4;
  } else if (angle >= -108 && angle < -72) {
    pos = 5;
  } else if (angle >= -72 && angle < -36) {
    pos = 6;
  } else if (angle >= -36 && angle < 0) {
    pos = 7;
  }
  return pos;
}
