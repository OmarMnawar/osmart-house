#ifndef REMOTE_H
#define REMOTE_H


namespace remote {

	enum class buttons
	{
		none,
		on_or_off,
		menu,
		test,
		plus,
		back,
		rewind,
		confirm,
		fastForward,
		minus,
		clear,
		number_0,
		number_1,
		number_2,
		number_3,
		number_4,
        number_5,
        number_6,
        number_7,
        number_8,
        number_9,
	};

	void init();
	void update_buttons();

	extern buttons current_button;
}

#endif