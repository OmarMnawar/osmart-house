#ifndef STATES_HPP
#define STATES_HPP

namespace Sensor {

    enum class States {
        MovementDetected,
        NoMovementDetected
    };

    extern States current_state;
}

namespace Remote {
    
    enum class Buttons
	{
		None,
		OnOrOff,
		Menu,
		Test,
		Plus,
		Back,
		Rewind,
		Confirm,
		FastForward,
		Minus,
		DELETE,
		Number0,
		Number1,
		Number2,
		Number3,
		Number4,
        Number5,
        Number6,
        Number7,
        Number8,
        Number9,
	};

    extern Buttons current_button;
}

#endif