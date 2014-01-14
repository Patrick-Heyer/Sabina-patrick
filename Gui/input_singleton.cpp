#include "input_singleton.h"

// initialize the singleton
InputSingleton	*InputSingleton::m_singleton = 0;

// ----------------------------------------------
// getInstance() - get a pointer to the unique
// Input_Singleton instance (the singleton)
// ----------------------------------------------
InputSingleton &InputSingleton::getInstance( void )
{
    if ( m_singleton == 0 )
        m_singleton = new InputSingleton;

    return ((InputSingleton &)m_singleton);
}

// ----------------------------------------------
// FreeInstance() - delete the unique Input_Singleton
// instance.
// ----------------------------------------------
void InputSingleton::FreeInstance( void )
{
    if ( m_singleton != 0 )
    {
        delete m_singleton;
        m_singleton = 0;
    }
}

// ----------------------------------------------
// Initialize() - Set default state
// .
// ----------------------------------------------
void InputSingleton::Initialize( void )
{
  for (int i=0; i<256; i++) keys[i]=false;
  for (int i=0; i<256; i++) specialkeys[i]=false;
  mouse_active=false;
  left_button=false;
  middle_button=false;
  right_button=false;
}

void InputSingleton::ProcessMouse ( int button, int state, int x, int y, int modkey )
{
	mouse_mod_key=modkey;
	if ( state == DOWN )
	{
		mouse_active=true;
		last_clicked_button=button;
		if ( button== LEFT_BUTTON )
		{
			left_button=true;
			left_click_x=x;
			left_click_y=y;
		}
		if ( button== MIDDLE_BUTTON )
		{
			middle_button=true;
			middle_click_x=x;
			middle_click_y=y;
		}
		if ( button== RIGHT_BUTTON )
		{
			right_button=true;
			right_click_x=x;
			right_click_y=y;
		}	
	}
	if ( state == UP )
	{
		last_released_button=button;
		mouse_active=false;
		if ( button== LEFT_BUTTON )
		{
			left_button=false;
			left_released_x=x;
			left_released_y=y;
		}
		if ( button== MIDDLE_BUTTON )
		{
			middle_button=false;
			middle_released_x=x;
			middle_released_y=y;
		}
		if ( button== RIGHT_BUTTON )
		{
			right_button=false;
			right_released_x=x;
			right_released_y=y;
		}
	}

}

void InputSingleton::PassiveMotion ( int x, int y )
{
	mouse_x=x;
	mouse_y=y;
}

void InputSingleton::ActiveMotion ( int x, int y )
{
	mouse_x=x;
	mouse_y=y;
	if (left_button) 
	{
		left_active_x=x;
		left_active_y=y;
	}
	if (middle_button) 
	{
		middle_active_x=x;
		middle_active_y=y;
	}
	if (right_button) 
	{
		right_active_x=x;
		right_active_y=y;
	}
}

void InputSingleton::KeyPressed ( unsigned char key, int x, int y, int modkey )
{
    if (key>32 && key<=127 )
    {
	InputSingleton::getInstance().keys[key]=true;
    InputSingleton::getInstance().key=key;
    InputSingleton::getInstance().keyboard_mod_key=modkey;
    }
    else
    {
        InputSingleton::getInstance().key=key;
    }
}

void InputSingleton::KeyReleased ( unsigned char key, int x, int y )
{
    if (key>32 && key<=127 )
    {
    InputSingleton::getInstance().keys[key]=false;
    InputSingleton::getInstance().key=NULL;
    InputSingleton::getInstance().keyboard_mod_key=NULL;
    }
    else
    {
        InputSingleton::getInstance().key=NULL;
    }
}

void InputSingleton::SpecialKeyPressed ( int key, int x, int y, int modkey )
{
    InputSingleton::getInstance().special_key=key;
    InputSingleton::getInstance().specialkeys[key]=true;
    InputSingleton::getInstance().special_mod_key=modkey;
}

void InputSingleton::SpecialKeyReleased ( int key, int x, int y )
{
    InputSingleton::getInstance().special_key=NULL;
    InputSingleton::getInstance().specialkeys[key]=false;
    InputSingleton::getInstance().keyboard_mod_key=NULL;
}




