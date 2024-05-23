
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#define LED_PIN    4
#define LED_COUNT 26
const int sampleWindow = 5;                             
#define SENSOR_PIN A0


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

unsigned long pixelPrevious = 0;        // Previous Pixel Millis
unsigned long patternPrevious = 0;      // Previous Pattern Millis
int           patternCurrent = 0;       // Current Pattern Number
int           patternInterval = 5000;   // Pattern Interval (ms)
int           pixelInterval = 10;       // Pixel Interval (ms)
int           pixelQueue = 0;           // Pattern Pixel Queue
int           pixelCycle = 0;           // Pattern Pixel Cycle
uint16_t      pixelCurrent = 0;         // Pattern Current Pixel Number
uint16_t      pixelNumber = LED_COUNT;  // Total Number of Pixels

// setup() function -- runs once at startup --------------------------------


const int dc_offset = 512;          // DC Offset to be removed from incoming signal 
const int numberOfSamples = 128;    // Number of samples to read at a time
const float aref_voltage = 3.3;     // Reference voltage of the Arduino ADC
const float alpha = 0.95;           // smoothing value

int sample;           // individual reading
long signal;          
long averageReading;
long RMS;             // root mean squared calculation

float dB;
long smoothedValue = 0;


void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  Serial.begin(115200);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(100); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {

  long sumOfSquares = 0;

  for (int i=0; i<numberOfSamples; i++){
    sample = analogRead(0);         // take a sample
    signal = (sample - dc_offset);  // subtract the dc offset to center the signal at 0
    signal *= signal;               // square the value to remove negative values
    sumOfSquares += signal;         // sum the values
  }
  
  RMS = sqrt(sumOfSquares / numberOfSamples);     
  smoothedValue = (alpha * smoothedValue) + ((1-alpha) * RMS);
  
  // convert the RMS value back into a voltage and convert to db  
  dB = 20 * log10(smoothedValue * (aref_voltage / 1024.0)); 

  int valrms = 512 - RMS;
  
    int db = map(valrms,0,37,0,LED_COUNT);
  
   if (db > 0) {
  //   Serial.print("Signal Level dB inside"); 
       Serial.println(db);
       for(int i=0; i < db ; i+=1) {
            if (i < 4  && i >= 0 ) { 
            strip.setPixelColor(i, strip.Color(0, 153, 0)); //  Update delay time  
            strip.show();
            }
            if (i < 8  && i >= 4 ) { 
            strip.setPixelColor(i, strip.Color(0, 255, 0)); //  Update delay time  
            strip.show();
            }
            if (i < 16  && i >= 8 ) { 
            strip.setPixelColor(i, strip.Color(255, 255, 0)); //  Update delay time  
            strip.show();
            }
            if (i < 20  && i >= 16 ) { 
            strip.setPixelColor(i, strip.Color(255, 127, 0)); //  Update delay time  
            strip.show();
            }
            if ( i < 24 && i >= 20) { 
            strip.setPixelColor(i, strip.Color(255, 0, 0)); //  Update delay time  
            strip.show();
            }
            if (i >= 24 ) { 
            strip.setPixelColor(i, strip.Color(255, 0, 0)); //  Update delay time  
            strip.show();
            }
       }    
    
   } else {
           for(int i=0; i< LED_COUNT; i+=1) {
              strip.setPixelColor(i, strip.Color(0, 0, 0)); //  Update delay time  
              strip.show();
           }
   }

}
