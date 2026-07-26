#include "remote.hpp"
#include <Arduino.h>
#include "inputs.hpp"

#define IR_USE_AVR_TIMER1
#include <IRremote.hpp>

namespace remote {
    buttons current_button = buttons::none;

    uint32_t button_hex_code = 0;
	uint32_t last_press_time = 0;

    void init() {
        Serial.begin(9600);
        IrReceiver.begin(pins::REMOTE);
    }

    void update_buttons() {

        if(IrReceiver.decode())
	    {
		    button_hex_code = IrReceiver.decodedIRData.command;

            // OM: The codes value was taken by printing it to the terminal.
		    switch(button_hex_code)
		    {
			    case 0xA2:
			    current_button = buttons::on_or_off;
			    break;
			
			    case 0xE2:
			    current_button = buttons::menu;
			    break;			
			
			    case 0x2:
			    current_button = buttons::plus;
			    break;
			
			    case 0xC2:
			    current_button = buttons::back;
			    break;
			
			
			    case 0xA8:
			    current_button = buttons::confirm;
			    break;
			
			    case 0x98:
			    current_button = buttons::minus;
			    break;

				case 0xB0:
				current_button = buttons::clear;
				break;
			
			    case 0x68:
			    current_button = buttons::number_0;
			    break;

			    case 0x30:
			    current_button = buttons::number_1;
			    break;
                
			    case 0x18:
			    current_button = buttons::number_2;
			    break;

			    case 0x7A:
			    current_button = buttons::number_3;
			    break;
      			
                case 0x10:
			    current_button = buttons::number_4;
			    break;

				case 0x38:
			    current_button = buttons::number_5;
			    break;

				case 0x5A:
			    current_button = buttons::number_6;
			    break;

				case 0x42:
			    current_button = buttons::number_7;
			    break;

				case 0x4A:
			    current_button = buttons::number_8;
			    break;

				case 0x52:
			    current_button = buttons::number_9;
			    break;

		    }
		    IrReceiver.resume();
            last_press_time = millis();
	    }


        if (current_button != buttons::none && millis() - last_press_time >= 10)
        {
            current_button = buttons::none;
        }

		last_press_time = 0;
    }
}