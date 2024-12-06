#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pins for the RFID module
#define RST_PIN 9    // Reset pin
#define SS_PIN 10    // Slave Select pin

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Create LCD instance with the I2C address (commonly 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change the address if necessary

void setup() {
  Serial.begin(9600);       // Initialize serial communication
  SPI.begin();              // Start SPI bus
  mfrc522.PCD_Init();       // Initialize MFRC522 module
  lcd.init();               // Initialize the LCD with correct method
  lcd.backlight();          // Turn on the LCD backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting...");
}

void loop() {
  // Check if a new card is present and its UID can be read
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Construct the UID string
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) {
        uid += "0"; // Add leading zero for single-digit values
      }
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase(); // Convert to uppercase for consistency

    // Send UID to the serial monitor
    Serial.println(uid);

    // Halt communication with the card
    mfrc522.PICC_HaltA();

    // Display "Card Scanned" on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card Scanned");
    delay(2000); // Short delay before waiting for PC input
  }

  // Check for incoming data from the PC
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n'); // Read until newline character
    lcd.clear();
    lcd.setCursor(0, 0);

    // Split received data if it's too long for one line
    if (receivedData.length() > 16) {
      lcd.print(receivedData.substring(0, 16)); // Display the first part on the first row
      lcd.setCursor(0, 1);
      lcd.print(receivedData.substring(16));   // Display the rest on the second row
    } else {
      lcd.print(receivedData); // Display everything on the first row if it fits
    }
  }
}
