#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin Definitions
constexpr int LED_PIN = 13;
constexpr int SENSOR_PIN = 4;
constexpr int VCC_PIN = 2;
constexpr int GND_PIN = 3;

// Constants
constexpr int RAIN_THRESHOLD = 500;
constexpr int OLED_RESET = -1; // OLED reset pin
constexpr int FRAME_DELAY = 42; // Animation speed
constexpr int WARNING_BLINK_DELAY = 700; // Blinking speed for the warning message
constexpr int FRAME_WIDTH = 32;
constexpr int FRAME_HEIGHT = 32;

// Display
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

// Messages
const char* WARNING_MESSAGE = "SLOW DOWN!";
const char* REGULAR_MESSAGE = "NICE DAY!!";

// Global Variables
const uint8_t PROGMEM Rainframes[][128] = {
    /* Frames data as before */
};
size_t currentFrame = 0;
unsigned long lastFrameTime = 0;
unsigned long lastBlinkTime = 0;
bool blinkState = false;

// Function Declarations
void setupDisplay();
void updateRainAnimation();
void showWarningMessage();
void showRegularMessage();
void checkRainSensor();

void setup() {
    // Initialize pins
    pinMode(LED_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT);
    pinMode(VCC_PIN, OUTPUT);
    pinMode(GND_PIN, OUTPUT);
    digitalWrite(VCC_PIN, HIGH);
    digitalWrite(GND_PIN, LOW);

    // Initialize display
    setupDisplay();
}

void loop() {
    checkRainSensor();
    updateRainAnimation();
}

void setupDisplay() {
    if (!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
        while (true); // Hang if the display doesn't initialize
    }
    display.clearDisplay();
    display.display();
}

void updateRainAnimation() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastFrameTime >= FRAME_DELAY) {
        lastFrameTime = currentMillis;
        display.clearDisplay();
        display.drawBitmap(48, 16, Rainframes[currentFrame], FRAME_WIDTH, FRAME_HEIGHT, SSD1306_WHITE);
        display.display();
        currentFrame = (currentFrame + 1) % (sizeof(Rainframes) / 128);
    }
}

void checkRainSensor() {
    int sensorValue = analogRead(SENSOR_PIN);
    if (sensorValue > RAIN_THRESHOLD) {
        digitalWrite(LED_PIN, HIGH);
        showWarningMessage();
    } else {
        digitalWrite(LED_PIN, LOW);
        showRegularMessage();
    }
}

void showWarningMessage() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= WARNING_BLINK_DELAY) {
        lastBlinkTime = currentMillis;
        blinkState = !blinkState;
    }
    display.clearDisplay();
    if (blinkState) {
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println(WARNING_MESSAGE);
    }
    display.display();
}

void showRegularMessage() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(REGULAR_MESSAGE);
    display.display();
}
