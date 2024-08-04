#include "Mouse.h"

double Mouse::x = 0;
double Mouse::y = 0;

double Mouse::Lastx = 0;
double Mouse::Lasty = 0;

double Mouse::dx = 0;
double Mouse::dy = 0;

double Mouse::scrollDX = 0;
double Mouse::scrollDY = 0;

bool Mouse::firstMouse = true;

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = { 0 };

void Mouse::cursorPosCallback(GLFWwindow* window, double _x, double _y)
{
	x = _x;
	y = _y;

	if (firstMouse)
	{
		Lastx = x;
		Lasty = y;
	}

	dx = x - Lastx;
	dy = Lasty - y;
	Lastx = x;
	Lasty = y;
}

void Mouse::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action != GLFW_RELEASE)
	{
		if (!buttons[button])
		{
			buttons[button] = true;
		}
	}
	else
	{
		buttons[button] = false;
	}

	buttonsChanged[button] = action != GLFW_REPEAT;
}

void Mouse::mouseWheelCallback(GLFWwindow* window, double dx, double dy)
{
	scrollDX = dx;
	scrollDY = dy;
}

double Mouse::getMouseX()
{
	return x;
}

double Mouse::getMouseY()
{
	return y;
}

double Mouse::getDX()
{
	double _dx = dx;
	dx = 0;
	return _dx;
}

double Mouse::getDY()
{
	double _dy = dy;
	dy = 0;
	return _dy;
}

double Mouse::getScrollDX()
{
	double dx = scrollDX;
	scrollDX = 0;
	return dx;
}

double Mouse::getScrollDY()
{
	double dx = scrollDY;
	scrollDY = 0;
	return dx;
}

bool Mouse::button(int button)
{
	return buttons[button];
}

bool Mouse::buttonChanged(int button)
{
	bool ret = buttonsChanged[button];
	buttonsChanged[button] = false;
	return ret;
}

bool Mouse::buttonWentUp(int button)
{
	return !buttons[button] && buttonChanged(button);
}

bool Mouse::buttonWentDown(int button)
{
	return buttons[button] && buttonChanged(button);
}
