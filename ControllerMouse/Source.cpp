#include<iostream>
#include<Windows.h>
#include "Gamepad_XInput.h"
int x = 0;
int y = 0;

void setMousePosition()
{
	SetCursorPos(x, y);
}

void mouseWheelUp()
{
	INPUT in;
	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_WHEEL;
	in.mi.mouseData = WHEEL_DELTA;

	SendInput(1, &in, sizeof(INPUT));
}

void mouseWheelDown()
{
	INPUT in;
	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_WHEEL;
	in.mi.mouseData = -WHEEL_DELTA;

	SendInput(1, &in, sizeof(INPUT));
}

void mouseWheelLeft()
{
	INPUT in;
	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_HWHEEL;
	in.mi.mouseData = WHEEL_DELTA;

	SendInput(1, &in, sizeof(INPUT));
}

void mouseWheelRight()
{
	INPUT in;
	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_HWHEEL;
	in.mi.mouseData = -WHEEL_DELTA;

	SendInput(1, &in, sizeof(INPUT));
}
void buttonDown(int but)
{
	INPUT in;
	in.type = INPUT_MOUSE;
	switch (but)
	{
	case 0:
		in.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		break;
	case 1:
		in.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		break;
	case 2:
		in.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
		break;
	default:
		break;
	}

	SendInput(1, &in, sizeof(INPUT));
}

void buttonUp(int but)
{
	INPUT in;
	in.type = INPUT_MOUSE;
	switch (but)
	{
	case 0:
		in.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		break;
	case 1:
		in.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		break;
	case 2:
		in.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
		break;
	default:
		break;
	}

	SendInput(1, &in, sizeof(INPUT));
}

void run()
{
	while (true)
	{
		POINT p;

		GetCursorPos(&p);

		x = p.x;
		y = p.y;

		Gamepad_XInput::update();

		if (Gamepad_XInput::isConnected(0))
		{
			if (Gamepad_XInput::getButtonPressed(0, Gamepad_XInput::BACK_BUTTON))
			{
				break;
			}

			if (Gamepad_XInput::getButtonPressed(0, Gamepad_XInput::A_BUTTON))
			{
				//Left Mouse Button
				buttonDown(0);
			}
			if (Gamepad_XInput::getButtonUp(0, Gamepad_XInput::A_BUTTON))
			{
				//Left Mouse Button
				buttonUp(0);
			}

			if (Gamepad_XInput::getButtonPressed(0, Gamepad_XInput::B_BUTTON))
			{
				//Right Mouse Button
				buttonDown(1);
			}
			if (Gamepad_XInput::getButtonUp(0, Gamepad_XInput::B_BUTTON))
			{
				//Right Mouse Button
				buttonUp(1);
			}

			if (Gamepad_XInput::getButtonPressed(0, Gamepad_XInput::X_BUTTON))
			{
				//Middle Mouse Button
				buttonDown(2);
			}
			if (Gamepad_XInput::getButtonUp(0, Gamepad_XInput::X_BUTTON))
			{
				//Middle Mouse Button
				buttonUp(2);
			}

			if (Gamepad_XInput::getAxisValue(0, Gamepad_XInput::RIGHT_Y_AXIS) > 0.25f)
			{
				mouseWheelUp();
			}
			else if (Gamepad_XInput::getAxisValue(0, Gamepad_XInput::RIGHT_Y_AXIS) < -0.25f)
			{
				mouseWheelDown();
			}


			if (Gamepad_XInput::getAxisValue(0, Gamepad_XInput::RIGHT_X_AXIS) > 0.25f)
			{
				mouseWheelLeft();
			}
			else if (Gamepad_XInput::getAxisValue(0, Gamepad_XInput::RIGHT_X_AXIS) < -0.25f)
			{
				mouseWheelRight();
			}

			x += Gamepad_XInput::getAxisValue(0, Gamepad_XInput::LEFT_X_AXIS) * 16;
			y -= Gamepad_XInput::getAxisValue(0, Gamepad_XInput::LEFT_Y_AXIS) * 16;

			setMousePosition();

			Sleep(16);
		}
		else
		{
			break;
		}
	}
}

void main()
{
	Gamepad_XInput::init();

	run();

	Gamepad_XInput::dispose();
}