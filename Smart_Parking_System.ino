#include <Servo.h>
#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Define IR sensor pins for 4 parking slots
const int slotSensors[] = {6, 7, 8, 9};  // Adjust pins as per your connection
const int entrySensor = 10;              // IR sensor for car detection at entry
Servo gateServo;                         // Servo motor for gate control

const int totalSlots = 4;
int availableSlots = totalSlots;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Smart Parking");
  delay(2000);
  lcd.clear();

  // Initialize sensors
  for (int i = 0; i < totalSlots; i++) {
    pinMode(slotSensors[i], INPUT);
  }
  pinMode(entrySensor, INPUT);

  // Initialize servo motor
  gateServo.attach(13);  // Servo connected to pin 13
  gateServo.write(0);    // Initially closed

  updateSlots();
  updateLCD();
}

void loop() {
  updateSlots();
  updateLCD();

  if (digitalRead(entrySensor) == LOW && availableSlots > 0) {
    lcd.setCursor(0, 1);
    lcd.print("Car Detected!   ");
    openGate();
    delay(2000);  // Allow time for car to pass
    closeGate();
  }
  else if (availableSlots == 0) {
    lcd.setCursor(0, 1);
    lcd.print("No Slot Avail!  ");
  }

  delay(500);  // Short delay before next loop
}

// Function to update available slots
void updateSlots() {
  int occupied = 0;
  for (int i = 0; i < totalSlots; i++) {
    if (digitalRead(slotSensors[i]) == LOW) {  // LOW = Obstacle detected
      occupied++;
    }
  }
  availableSlots = totalSlots - occupied;
}

// Function to update LCD display
void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Available: ");
  lcd.print(availableSlots);
  lcd.print(" ");
}

// Function to open gate
void openGate() {
  gateServo.write(90);  // Rotate servo to open position
  lcd.setCursor(0, 1);
  lcd.print("Gate Opened     ");
  delay(2000);
}

// Function to close gate
void closeGate() {
  gateServo.write(0);   // Rotate servo back to close
  lcd.setCursor(0, 1);
  lcd.print("Gate Closed     ");
  delay(2000);
}
