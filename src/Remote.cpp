#include "Remote.hpp"
#include <Arduino.h>
#include "Inputs.hpp"
#include "Print.h"
#include "States.hpp"
#include "Timer.hpp"
#include <IRremote.hpp>

namespace Remote {
    Buttons current_button = Buttons::None;

    uint32_t button_hex_code = 0;

    void init() {
        Serial.begin(9600);
        IrReceiver.begin(Pins::REMOTE_PIN);
    }

    void update_buttons() {

        if(IrReceiver.decode())
	    {
		    button_hex_code = IrReceiver.decodedIRData.command;

            // OM: The codes value was taken by printing it to the terminal.
		    switch(button_hex_code)
		    {
			    case 0xA2:
			    current_button = Buttons::OnOrOff;
			    break;
			
			    case 0xE2:
			    current_button = Buttons::Menu;
			    break;			
			
			    case 0x2:
			    current_button = Buttons::Plus;
			    break;
			
			    case 0xC2:
			    current_button = Buttons::Back;
			    break;
			
			
			    case 0xA8:
			    current_button = Buttons::Confirm;
			    break;
			
			    case 0x98:
			    current_button = Buttons::Minus;
			    break; 
			
			    case 0x68:
			    current_button = Buttons::Number0;
			    break;

			    case 0x30:
			    current_button = Buttons::Number1;
			    break;
                
			    case 0x18:
			    current_button = Buttons::Number2;
			    break;

			    case 0x7A:
			    current_button = Buttons::Number3;
			    break;
      			
                case 0x10:
			    current_button = Buttons::Number4;
			    break;

				case 0x38:
			    current_button = Buttons::Number5;
			    break;

				case 0x5A:
			    current_button = Buttons::Number6;
			    break;

				case 0x42:
			    current_button = Buttons::Number7;
			    break;

				case 0x4A:
			    current_button = Buttons::Number8;
			    break;

				case 0x52:
			    current_button = Buttons::Number9;
			    break;

		    }
		    IrReceiver.resume();
            
            Timer::start();
	    }


        if (current_button != Buttons::None && Timer::is_time_reached(10))
        {
            current_button = Buttons::None;
        }

        Timer::stop();
    }
}