#include <MIDI.h>
#include <USB-MIDI.h>
#include <AceButton.h>
using namespace ace_button;

//MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
USBMIDI_CREATE_DEFAULT_INSTANCE();
// Set Midi Channel
const int midiChannel = 1;

// LED states. Some microcontrollers wire their built-in LED the reverse.
const int LED_ON = HIGH;
const int LED_OFF = LOW;

// Number of buttons and LEDs.
const uint8_t NUM_LEDS = 5;

// Helper struct that keeps track of the buttons and leds. An alternative is to
// use multiple arrays (e.g. BUTTON_PINS, LED_PINS, LED_STATES) which has the
// potential of reduing memory using PROGMEM, but for small to medium number of
// buttons, using a struct makes the program easier to read.
struct Info {
  const uint8_t buttonPin;
  const uint8_t ledPin;
  bool ledState;
  int midiControl;
};

// An array of button pins, led pins, and the led states. Cannot be const
// because Info.ledState is mutable.
Info INFOS[NUM_LEDS] = {
  //Blue
  {5, A2, LED_OFF, 48}, 
  //Purple
  {6, A1, LED_OFF, 49},
  //Yellow
  {7, A0, LED_OFF, 54},
  //Orange
  {8, 15, LED_OFF, 55},
  //Green
  {9, 10, LED_OFF, 56},
};

// Define the buttons in an array using the default constructor.
AceButton buttons[NUM_LEDS];

// Forward reference to prevent Arduino compiler becoming confused.
void handleEvent(AceButton*, uint8_t, uint8_t);

void setup() {
  delay(1000); // some microcontrollers reboot twice
  //Serial1.begin(31250);
  //Serial.begin(31250);
  MIDI.begin (MIDI_CHANNEL_OMNI);
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    // initialize built-in LED as an output
    pinMode(INFOS[i].ledPin, OUTPUT);
    // Button uses the built-in pull up register.
    pinMode(INFOS[i].buttonPin, INPUT_PULLUP);
    // initialize the corresponding AceButton
    buttons[i].init(INFOS[i].buttonPin, HIGH, i);
  }

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
}

void loop() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    buttons[i].check();
  }
}

// The event handler for the button.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  // Get the LED pin
  uint8_t id = button->getId();
  uint8_t ledPin = INFOS[id].ledPin;

    if(eventType==1){      
          if (INFOS[id].ledState == LED_OFF)
          {   
            for (int i=0; i<20; ++i) {
                MIDI.sendControlChange (INFOS[id].midiControl,127,midiChannel); 
              }
                digitalWrite(ledPin, LED_ON);
                INFOS[id].ledState = LED_ON;
               // Serial.println("ON");
          }
          else if (INFOS[id].ledState == LED_ON)
          {
            for (int i=0; i<20; ++i) {
                MIDI.sendControlChange (INFOS[id].midiControl,0,midiChannel); 
              }
                digitalWrite(ledPin, LED_OFF);
                INFOS[id].ledState = LED_OFF;
              //  Serial.println("OFF");
          }
    }
         

}