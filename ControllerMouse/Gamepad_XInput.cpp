#include "Gamepad_XInput.h"
#include <cmath>

bool Gamepad_XInput::buttonPressed[56] = {};
bool Gamepad_XInput::buttonDown[56] = {};
bool Gamepad_XInput::buttonUp[56] = {};

float Gamepad_XInput::axisValue[24] = {};

bool Gamepad_XInput::gamepadConnected[4] = {};

int Gamepad_XInput::DEAD_ZONE = 512;

void Gamepad_XInput::init()
{
	XInputEnable(true);
}

void Gamepad_XInput::setButtonDown(int id, int button)
{
	int offset = id*Gamepad_XInput::BUTTONS_PER_CONTROLLER;

	if (buttonDown[button + offset] == false)
		buttonPressed[button + offset] = true;
	else
		buttonPressed[button + offset] = false;

	buttonDown[button + offset] = true;
	buttonUp[button + offset] = false;
}

void Gamepad_XInput::setButtonUp(int id, int button)
{
	int offset = id*Gamepad_XInput::BUTTONS_PER_CONTROLLER;

	if (buttonDown[button + offset] == true)
		buttonUp[button + offset] = true;
	else
		buttonUp[button + offset] = false;

	buttonDown[button + offset] = false;
	buttonPressed[button + offset] = false;
}

void Gamepad_XInput::setAxis(int id, int axis, float value)
{
	int offset = id*Gamepad_XInput::AXIES_PER_CONTROLLER;
	axisValue[axis + offset] = value;
}

void Gamepad_XInput::update()
{
	XINPUT_STATE controllerInfo;
	DWORD state;
	int offset = 0;
	bool value = false;
	float convertedValue = 0;

	for (int i = 0; i < 4; i++)
	{
		ZeroMemory(&state, sizeof(DWORD));
		ZeroMemory(&controllerInfo, sizeof(XINPUT_STATE));

		state = XInputGetState(i, &controllerInfo);

		if (state == ERROR_SUCCESS)
		{
			//gamepad is connected so store that for later use.
			//determine if the values have changed (DO LATER)
			gamepadConnected[i] = true;
			
			//for every button, determine if the button is held down then set some values

			//A BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_A)!=0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::A_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::A_BUTTON);

			//B BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::B_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::B_BUTTON);

			//X BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::X_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::X_BUTTON);

			//Y BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::Y_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::Y_BUTTON);

			//DPAD UP BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::DPAD_UP_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::DPAD_UP_BUTTON);

			//DPAD DOWN BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::DPAD_DOWN_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::DPAD_DOWN_BUTTON);

			//DPAD RIGHT BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::DPAD_RIGHT_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::DPAD_RIGHT_BUTTON);

			//DPAD LEFT BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::DPAD_LEFT_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::DPAD_LEFT_BUTTON);

			//LEFT STICK BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::LEFT_STICK_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::LEFT_STICK_BUTTON);

			//RIGHT STICK BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::RIGHT_STICK_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::RIGHT_STICK_BUTTON);

			//LEFT SHOULDER BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::LB_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::LB_BUTTON);

			//RIGHT SHOULDER BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::RB_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::RB_BUTTON);

			//START BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::START_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::START_BUTTON);

			//BACK BUTTON
			value = ((controllerInfo.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0);
			if (value == true)
				setButtonDown(i, Gamepad_XInput::BACK_BUTTON);
			else
				setButtonUp(i, Gamepad_XInput::BACK_BUTTON);


			//Now time for the axis values
			//convert the stick values to be between -1.0 and 1.0
			//convert the trigger values to be between 0.0 and 1.0
			
			if (abs(controllerInfo.Gamepad.sThumbLX) > DEAD_ZONE)
			{
				if (controllerInfo.Gamepad.sThumbLX > 0.0f)
				{
					convertedValue = (float)(controllerInfo.Gamepad.sThumbLX-DEAD_ZONE) / (AXIS_DIVIDE_VALUE - DEAD_ZONE);
					convertedValue = min(convertedValue, 1.0f);
				}
				else
				{
					convertedValue = (float)(controllerInfo.Gamepad.sThumbLX + DEAD_ZONE) / (AXIS_DIVIDE_VALUE - DEAD_ZONE);
					convertedValue = max(convertedValue, -1.0f);
				}

				//std::cout << convertedValue << std::endl;
				setAxis(i, LEFT_X_AXIS, convertedValue);
			}
			else
			{
				setAxis(i, LEFT_X_AXIS, 0.0f);
			}

			if (abs(controllerInfo.Gamepad.sThumbLY) > DEAD_ZONE)
			{
				if (controllerInfo.Gamepad.sThumbLY > 0.0f)
				{
					convertedValue = (float)(controllerInfo.Gamepad.sThumbLY - DEAD_ZONE) / (AXIS_DIVIDE_VALUE - DEAD_ZONE);
					convertedValue = min(convertedValue, 1.0f);
				}
				else
				{
					convertedValue = (float)(controllerInfo.Gamepad.sThumbLY + DEAD_ZONE) / (AXIS_DIVIDE_VALUE - DEAD_ZONE);
					convertedValue = max(convertedValue, -1.0f);
				}

				setAxis(i, LEFT_Y_AXIS, convertedValue);
			}
			else
			{
				setAxis(i, LEFT_Y_AXIS, 0.0f);
			}

			if (abs(controllerInfo.Gamepad.sThumbRX) > DEAD_ZONE)
			{
				if (controllerInfo.Gamepad.sThumbRX > 0.0f)
				{
					convertedValue = (float)(controllerInfo.Gamepad.sThumbRX - DEAD_ZONE) / (AXIS_DIVIDE_VALUE - DEAD_ZONE);
					convertedValue = min(convertedValue, 1.0f);
				}
				else
				{
					convertedValue = (float)(controllerInfo.Gamepad.sThumbRX + DEAD_ZONE) / (AXIS_DIVIDE_VALUE - DEAD_ZONE);
					convertedValue = max(convertedValue, -1.0f);
				}

				setAxis(i, RIGHT_X_AXIS, convertedValue);
			}
			else
			{
				setAxis(i, RIGHT_X_AXIS, 0.0f);
			}

			if (abs(controllerInfo.Gamepad.sThumbRY) > DEAD_ZONE)
			{
				if (controllerInfo.Gamepad.sThumbRY > 0.0f)
				{
					convertedValue = (float)(controllerInfo.Gamepad.sThumbRY - DEAD_ZONE) / (AXIS_DIVIDE_VALUE - DEAD_ZONE);
					convertedValue = min(convertedValue, 1.0f);
				}
				else
				{
					convertedValue = (float)(controllerInfo.Gamepad.sThumbRY + DEAD_ZONE) / (AXIS_DIVIDE_VALUE - DEAD_ZONE);
					convertedValue = max(convertedValue, -1.0f);
				}

				setAxis(i, RIGHT_Y_AXIS, convertedValue);
			}
			else
			{
				setAxis(i, RIGHT_Y_AXIS, 0.0f);
			}

			convertedValue = (float)controllerInfo.Gamepad.bLeftTrigger / TRIGGER_DIVIDE_VALUE;
			setAxis(i, LEFT_TRIGGER, convertedValue);

			convertedValue = (float)controllerInfo.Gamepad.bRightTrigger / TRIGGER_DIVIDE_VALUE;
			setAxis(i, RIGHT_TRIGGER, convertedValue);
		}
		else
		{
			gamepadConnected[i] = false;
		}
	}
}

bool Gamepad_XInput::isConnected(int id)
{
	return Gamepad_XInput::gamepadConnected[id];
}

bool Gamepad_XInput::getButtonPressed(int id, int button)
{
	int offset = id*Gamepad_XInput::BUTTONS_PER_CONTROLLER;

	return Gamepad_XInput::buttonPressed[button + offset];
}

bool Gamepad_XInput::getButtonDown(int id, int button)
{
	int offset = id*Gamepad_XInput::BUTTONS_PER_CONTROLLER;

	return Gamepad_XInput::buttonDown[button + offset];
}

bool Gamepad_XInput::getButtonUp(int id, int button)
{
	int offset = id*Gamepad_XInput::BUTTONS_PER_CONTROLLER;

	return Gamepad_XInput::buttonUp[button + offset];
}

float Gamepad_XInput::getAxisValue(int id, int axis)
{
	int offset = id*Gamepad_XInput::AXIES_PER_CONTROLLER;

	return Gamepad_XInput::axisValue[axis + offset];
}

void Gamepad_XInput::dispose()
{

}
