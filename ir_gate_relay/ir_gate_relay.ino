#include <IRremote.h>
#include  <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

const int IR_RECEIVE_PIN = A0;
int RECEIVED = 0;

int OPEN_RELAY = 11;
int CLOSE_RELAY = 12;

int OPEN_RELAY_STATE = 0;

bool IN_OP = false;
bool STOP_1 = false;
bool STOP_2 = false;


unsigned long OPEN_RELAY_LAST_BUTTON_TIME = millis();

const int TIME_OUT_DELAY_OPEN = 2000;

void setup() {
  Serial.begin(9600);
  Serial.println("IR Receive test");
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  pinMode(OPEN_RELAY, OUTPUT);
  pinMode(CLOSE_RELAY, OUTPUT);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Innovator Park");
  lcd.setCursor(0, 1);
  lcd.print("  Quadrant 1.0");
}

void loop() {
  if (IrReceiver.decode()) {
    RECEIVED = IrReceiver.decodedIRData.command, HEX;
    Serial.println(RECEIVED);
    if (RECEIVED == 77 && !IN_OP) {
      IN_OP = true;
      digitalWrite(OPEN_RELAY, HIGH);
      OPEN_RELAY_STATE = 1;
      OPEN_RELAY_LAST_BUTTON_TIME  = millis();
    }
    else if (RECEIVED == REPEAT &&  OPEN_RELAY_STATE == 1)
    {
      OPEN_RELAY_LAST_BUTTON_TIME = millis();
    }

    IrReceiver.resume();
  }

  if ((millis() -  OPEN_RELAY_LAST_BUTTON_TIME > TIME_OUT_DELAY_OPEN) &&  OPEN_RELAY_STATE == 1)
  {
    digitalWrite(OPEN_RELAY, LOW);
    OPEN_RELAY_STATE = 0;
    STOP_1 = true;
  }
  if (STOP_1) {
    delay(4000);
    STOP_2 = true;
    STOP_1 = false;
  }
  if (STOP_2) {
    digitalWrite(CLOSE_RELAY, HIGH);
    delay(2000);
    digitalWrite(CLOSE_RELAY, LOW);
    STOP_2 = false;
    IN_OP = false;
  }
}
