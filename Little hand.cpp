#include "BleMouse.h"

const int JOY_X = 27;          // consider moving to an ADC1 pin (e.g. 34)
const int JOY_Y = 35;
const int BTN_CLICK = 26;
const int BTN_SCROLL_UP = 14;
const int BTN_SCROLL_DOWN = 12;
const int LED = 2;

const int DEADZONE = 300;
const int MAX_SPEED = 25;            // pixels per update
const unsigned long MOVE_INTERVAL = 15;    // ms
const unsigned long SCROLL_INTERVAL = 120; // ms

int centerX = 2048, centerY = 2048;
unsigned long lastMove = 0, lastScroll = 0;

BleMouse mouse("Little Hand", "Kaushik", 100);

// Converts a raw ADC value to a signed speed with a deadzone
int axisToSpeed(int raw, int center) {
  int d = raw - center;
  if (abs(d) < DEADZONE) return 0;
  int range = (d > 0 ? 4095 - center : center) - DEADZONE;
  int speed = (abs(d) - DEADZONE) * MAX_SPEED / range;
  speed = constrain(speed, 1, MAX_SPEED);
  return d > 0 ? speed : -speed;
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_CLICK, INPUT_PULLUP);
  pinMode(BTN_SCROLL_UP, INPUT_PULLUP);
  pinMode(BTN_SCROLL_DOWN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  // Calibrate stick center (keep the stick untouched at power-on)
  long sx = 0, sy = 0;
  for (int i = 0; i < 32; i++) { sx += analogRead(JOY_X); sy += analogRead(JOY_Y); delay(5); }
  centerX = sx / 32;
  centerY = sy / 32;

  mouse.begin();
  Serial.println("BLE mouse started");
}

void loop() {
  unsigned long now = millis();

  if (!mouse.isConnected()) {
    digitalWrite(LED, (now / 300) % 2);   // non-blocking blink
    return;
  }

  bool active = false;

  // Click / drag: press while held, release when let go
  bool clickHeld = (digitalRead(BTN_CLICK) == LOW);
  if (clickHeld && !mouse.isPressed(MOUSE_LEFT)) mouse.press(MOUSE_LEFT);
  else if (!clickHeld && mouse.isPressed(MOUSE_LEFT)) mouse.release(MOUSE_LEFT);
  active |= clickHeld;

  // Cursor movement (diagonals work, speed scales with deflection)
  if (now - lastMove >= MOVE_INTERVAL) {
    lastMove = now;
    int dx = axisToSpeed(analogRead(JOY_X), centerX);
    int dy = axisToSpeed(analogRead(JOY_Y), centerY);
    if (dx || dy) {
      mouse.move(dx, dy);
      active = true;
    }
  }

  // Vertical scroll
  if (now - lastScroll >= SCROLL_INTERVAL) {
    if (digitalRead(BTN_SCROLL_UP) == LOW) {
      mouse.move(0, 0, 1);    // flip signs if direction feels backwards
      lastScroll = now; active = true;
    } else if (digitalRead(BTN_SCROLL_DOWN) == LOW) {
      mouse.move(0, 0, -1);
      lastScroll = now; active = true;
    }
  }

  digitalWrite(LED, active);
}