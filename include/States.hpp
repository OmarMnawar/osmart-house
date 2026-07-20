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
		Number4
	};

    extern Buttons current_button;
}

namespace Lcd {
    enum class PowerState
    {
        Off,
        On,
    };

    enum class State
    {
        NoAction,
        ShowingWelcomeMessage,
        Idle,
        InMainMenu,
        InThermometerMenu,
        InMusicMenu,
        InLedsMenu,
        ShuttingOff
    };

    extern PowerState current_power_state;
    extern State current_state;
}

#endif