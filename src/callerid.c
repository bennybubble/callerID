#include <Arduino.h>

#include "esphome.h"

// Define the pins connected to the MT8870
#define MT8870_D0 7 
#define MT8870_D1 8
#define MT8870_D2 6
#define MT8870_D3 1
#define MT8870_STQ 2

class DTMFDecoder : public PollingComponent {
public:
    DTMFDecoder() : PollingComponent(100) {}

    void setup() override {
        // Set up the data pins as inputs
        pinMode(MT8870_D0, INPUT);
        pinMode(MT8870_D1, INPUT);
        pinMode(MT8870_D2, INPUT);
        pinMode(MT8870_D3, INPUT);

        // Set up the StD pin as input
        pinMode(MT8870_STQ, INPUT);
    }

    void update() override {
        static String dtmfString = "";

        // Check if a DTMF tone has been detected
        if (digitalRead(MT8870_STQ) == HIGH) {
            // Read the 4-bit DTMF code
            int dtmfCode = (digitalRead(MT8870_D3) << 3) |
                           (digitalRead(MT8870_D2) << 2) |
                           (digitalRead(MT8870_D1) << 1) |
                           digitalRead(MT8870_D0);

            // Decode the DTMF code to the corresponding digit
            char dtmfDigit;
            switch (dtmfCode) {
                case 0x1: dtmfDigit = '1'; break;
                case 0x2: dtmfDigit = '2'; break;
                case 0x3: dtmfDigit = '3'; break;
                case 0x4: dtmfDigit = '4'; break;
                case 0x5: dtmfDigit = '5'; break;
                case 0x6: dtmfDigit = '6'; break;
                case 0x7: dtmfDigit = '7'; break;
                case 0x8: dtmfDigit = '8'; break;
                case 0x9: dtmfDigit = '9'; break;
                case 0xA: dtmfDigit = '0'; break;
                case 0xB: dtmfDigit = '*'; break;
                case 0xC: dtmfDigit = '#'; break;
                case 0xD: dtmfDigit = 'A'; break;
                case 0xE: dtmfDigit = 'B'; break;
                case 0xF: dtmfDigit = 'C'; break;
                case 0x0: dtmfDigit = 'D'; break;
                default: dtmfDigit = '?'; break;
            }

            // Append the detected DTMF digit to the string
            dtmfString += dtmfDigit;

            // Log the detected DTMF digit
            ESP_LOGD("dtmf", "Detected DTMF digit: %c", dtmfDigit);

            // Wait for the tone to end
            while (digitalRead(MT8870_STQ) == HIGH) {
                delay(10);
            }
        }

        // Log the complete DTMF string if it ends with '#'
        if (dtmfString.endsWith("C")) {
            ESP_LOGD("dtmf", "Complete DTMF string: %s", dtmfString.c_str());
            dtmfString = ""; // Reset the string for the next input
        }
    }
};
    }
}