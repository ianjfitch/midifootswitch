#include <Control_Surface.h> // Include the Control Surface library
 
// Instantiate a MIDI over USB interface
//USBMIDI_Interface midi;
HardwareSerialMIDI_Interface midi {Serial1, MIDI_BAUD};

/*
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

*/ 

// Instantiate an array of latched push buttons that send MIDI CC messages
// when pressed.
CCButtonLatched buttons[] {
 // { 0, 0x10 },
  //  │   │    └──── Base MIDI CC controller number
  //  │   └───────── Button pin number
  //  └───────────── Bank that changes the controller number
  { 5, 0x30 },
  { 6, 0x31 },
  { 7, 0x36 },
  { 8, 0x37 },
  { 9, 0x38 },
};



// Setup for an Analog pot for an EXP pedal on cc #11


// The array of pin numbers for LEDs that display the states of the buttons.
const pin_t ledPins[] = { A2, A1, A0, 15, 10};

// Get the length of an array
template <class T, size_t N> constexpr size_t length(T (&)[N]) { return N; }

static_assert(length(buttons) == length(ledPins),
              "Error: requires same number of buttons as LEDs");

void setup() {
  Control_Surface.begin();  // Initialize the Control Surface
  for (auto pin : ledPins)  // Set the pinMode to output for all LEDs
      pinMode(pin, OUTPUT);
}

void loop() {
  Control_Surface.loop();  // Update the Control Surface

  // Loop over all buttons and LEDs
  for (size_t i = 0; i < length(buttons); ++i)
      // Update the LED states to reflect the toggled switch states
      digitalWrite(ledPins[i], buttons[i].getState() ? HIGH : LOW);
}