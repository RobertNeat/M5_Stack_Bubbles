#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 128

const int buttonPin = 39;

Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int maxCircles = 20;  // Maximum number of circles
int circleCount = 1;        // Start with one circle

struct Circle {
  int x;
  int y;
  int radius;
  int xSpeed;
  int ySpeed;
};

Circle circles[maxCircles];

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  Wire.begin(26, 32);
  
  if(!display.begin(0x3C, true)) {
    Serial.println(F("SH1107 allocation failed"));
    for(;;);
  }
  
  display.display();
  delay(2000);  // Pause for 2 seconds

  display.clearDisplay();
  display.setRotation(1);

  // Initialize the first circle
  circles[0] = {SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, 5, 2, 2};
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && circleCount < maxCircles) {
    Serial.println("Button pressed");

    // Attempt to place the new circle without overlapping existing circles
    bool validPosition = false;
    int newX, newY;
    while (!validPosition) {
      newX = random(10, SCREEN_HEIGHT - 10);
      newY = random(10, SCREEN_WIDTH - 10);
      validPosition = true;

      // Check for overlap with existing circles
      for (int i = 0; i < circleCount; i++) {
        int dx = circles[i].x - newX;
        int dy = circles[i].y - newY;
        int distance = sqrt(dx * dx + dy * dy);
        if (distance < circles[i].radius * 2) {
          validPosition = false;
          break;
        }
      }
    }

    circles[circleCount] = {newX, newY, 5, random(-2, 3), random(-2, 3)};
    circleCount++;
    delay(200); // Debounce delay
  }

  // Clear the display buffer
  display.clearDisplay();

  // Update and draw each circle
  for (int i = 0; i < circleCount; i++) {
    circles[i].x += circles[i].xSpeed;
    circles[i].y += circles[i].ySpeed;

    // Check for collisions with the edges of the screen
    if (circles[i].x - circles[i].radius < 0 || circles[i].x + circles[i].radius > SCREEN_HEIGHT) {
      circles[i].xSpeed = -circles[i].xSpeed;
    }
    if (circles[i].y - circles[i].radius < 0 || circles[i].y + circles[i].radius > SCREEN_WIDTH) {
      circles[i].ySpeed = -circles[i].ySpeed;
    }

    // Check for collisions with other circles
    for (int j = i + 1; j < circleCount; j++) {
      int dx = circles[j].x - circles[i].x;
      int dy = circles[j].y - circles[i].y;
      int distance = sqrt(dx * dx + dy * dy);
      if (distance < circles[i].radius + circles[j].radius) {
        // Simple elastic collision: swap speeds
        int tempXSpeed = circles[i].xSpeed;
        int tempYSpeed = circles[i].ySpeed;
        circles[i].xSpeed = circles[j].xSpeed;
        circles[i].ySpeed = circles[j].ySpeed;
        circles[j].xSpeed = tempXSpeed;
        circles[j].ySpeed = tempYSpeed;
      }
    }

    // Draw the circle
    display.fillCircle(circles[i].x, circles[i].y, circles[i].radius, SH110X_WHITE);
  }

  // Display the updated buffer
  display.display();

  // Small delay to control the speed of the circles' movement
  delay(50);
}


/*
//--Without collision between bubbles
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 128

const int buttonPin = 39;

Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int maxCircles = 20;  // Maximum number of circles
int circleCount = 1;        // Start with one circle

struct Circle {
  int x;
  int y;
  int radius;
  int xSpeed;
  int ySpeed;
};

Circle circles[maxCircles];

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  Wire.begin(26, 32);
  
  if(!display.begin(0x3C, true)) {
    Serial.println(F("SH1107 allocation failed"));
    for(;;);
  }
  
  display.display();
  delay(2000);  // Pause for 2 seconds

  display.clearDisplay();
  display.setRotation(1);

  // Initialize the first circle
  circles[0] = {SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, 5, 2, 2};
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && circleCount < maxCircles) {
    Serial.println("Button pressed");
    circles[circleCount] = {random(10, SCREEN_HEIGHT - 10), random(10, SCREEN_WIDTH - 10), 5, random(-2, 3), random(-2, 3)};
    circleCount++;
    delay(200); // Debounce delay
  }

  // Clear the display buffer
  display.clearDisplay();

  // Update and draw each circle
  for (int i = 0; i < circleCount; i++) {
    circles[i].x += circles[i].xSpeed;
    circles[i].y += circles[i].ySpeed;

    // Check for collisions with the edges of the screen
    if (circles[i].x - circles[i].radius < 0 || circles[i].x + circles[i].radius > SCREEN_HEIGHT) {
      circles[i].xSpeed = -circles[i].xSpeed;
    }
    if (circles[i].y - circles[i].radius < 0 || circles[i].y + circles[i].radius > SCREEN_WIDTH) {
      circles[i].ySpeed = -circles[i].ySpeed;
    }

    // Draw the circle
    display.fillCircle(circles[i].x, circles[i].y, circles[i].radius, SH110X_WHITE);
  }

  // Display the updated buffer
  display.display();

  // Small delay to control the speed of the circles' movement
  delay(50);
}
*/