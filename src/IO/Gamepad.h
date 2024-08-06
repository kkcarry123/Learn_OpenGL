#pragma once

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

// analog input	button values					//		PS		|		XBOX
#define GLFW_Gamepad_BTN_LEFT 0				//	Square		|	X
#define GLFW_Gamepad_BTN_DOWN 1				//	X			|	A
#define GLFW_Gamepad_BTN_RIGHT 2				//	Circle		|	B
#define GLFW_Gamepad_BTN_UP 3					//	Triangle	|	Y	
#define GLFW_Gamepad_SHOULDER_LEFT 4			//	L1			|	LB
#define GLFW_Gamepad_SHOULDER_RIGHT 5			//	R1			|	RB
#define GLFW_Gamepad_TRIGGER_LEFT 6			//	L2			|	LT
#define GLFW_Gamepad_TRIGGER_RIGHT 7			//	R2			|	RT
#define GLFW_Gamepad_SELECT 8					//	Share		|	Address
#define GLFW_Gamepad_START 9					//	Options		|	Menu
#define GLFW_Gamepad_LEFT_STICK 10				//	L3			|	LS
#define GLFW_Gamepad_RIGHT_STICK 11			//	R3			|	RS
#define GLFW_Gamepad_HOME 12					//	Home		|	Home
#define GLFW_Gamepad_CLICK 13					//	Touch pad	|	n/a
#define GLFW_Gamepad_DPAD_UP 14				//	Dpad up		|	Dpad up
#define GLFW_Gamepad_DPAD_RIGHT 15				//	Dpad right	|	Dpad right
#define GLFW_Gamepad_DPAD_DOWN 16				//	Dpad down	|	Dpad down
#define GLFW_Gamepad_DPAD_LEFT 17				//	Dpad left	|	Dpad left

// axes
#define GLFW_Gamepad_AXES_LEFT_STICK_X 0
#define GLFW_Gamepad_AXES_LEFT_STICK_Y 1
#define GLFW_Gamepad_AXES_RIGHT_STICK_X 2
#define GLFW_Gamepad_AXES_LEFT_TRIGGER 3
#define GLFW_Gamepad_AXES_RIGHT_TRIGGER 4
#define GLFW_Gamepad_AXES_RIGHT_STICK_Y 5


class Gamepad
{
public:
    Gamepad(int joystickID)
        : joystickID(joystickID), name(glfwGetJoystickName(joystickID))
    {
        if (glfwJoystickIsGamepad(joystickID))
        {
            std::cout << "Gamepad connected: " << name << std::endl;
        }
        else
        {
            std::cout << "Joystick " << joystickID << " is not a gamepad." << std::endl;
        }
    }

    void update()
    {
        if (glfwJoystickIsGamepad(joystickID))
        {
            GLFWgamepadstate state;
            if (glfwGetGamepadState(joystickID, &state))
            {
                buttons.assign(state.buttons, state.buttons + GLFW_GAMEPAD_BUTTON_LAST + 1);
                axes.assign(state.axes, state.axes + GLFW_GAMEPAD_AXIS_LAST + 1);
            }
        }
    }

    bool isButtonPressed(int button) const
    {
        if (button >= 0 && button < buttons.size())
        {
            return buttons[button] == GLFW_PRESS;
        }
        return false;
    }

    float getAxisValue(int axis) const
    {
        if (axis >= 0 && axis < axes.size())
        {
            return axes[axis];
        }
        return 0.0f;
    }

    std::string getName() const
    {
        return name;
    }

private:
    int joystickID;
    std::string name;
    std::vector<unsigned char> buttons;
    std::vector<float> axes;
};

