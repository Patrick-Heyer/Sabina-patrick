#include "input_singleton.h"

// initialize the singleton
Input_Singleton	*Input_Singleton::m_singleton = 0;

// ----------------------------------------------
// GetInstance() - get a pointer to the unique
// Input_Singleton instance (the singleton)
// ----------------------------------------------
Input_Singleton &Input_Singleton::GetInstance( void )
{
    if ( m_singleton == 0 )
        m_singleton = new Input_Singleton;

    return ((Input_Singleton &)m_singleton);
}

// ----------------------------------------------
// FreeInstance() - delete the unique Input_Singleton
// instance.
// ----------------------------------------------
void Input_Singleton::FreeInstance( void )
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
void Input_Singleton::Initialize( void )
{
  for (int i=0; i<256; i++) keys[i]=false;
  for (int i=0; i<256; i++) specialkeys[i]=false;
  mouse_active=false;
  left_button=false;
  middle_button=false;
  right_button=false;
}

void Input_Singleton::ProcessMouse ( int button, int state, int x, int y, int modkey )
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

void Input_Singleton::PassiveMotion ( int x, int y )
{
	mouse_x=x;
	mouse_y=y;
}

void Input_Singleton::ActiveMotion ( int x, int y )
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

void Input_Singleton::KeyPressed ( unsigned char key, int x, int y, int modkey )
{
	keys[key]=true;
	key=key;
	keyboard_mod_key=modkey;
}

void Input_Singleton::KeyReleased ( unsigned char key, int x, int y )
{
	keys[key]=false;
	key=NULL;
	keyboard_mod_key=NULL;
}

void Input_Singleton::SpecialKeyPressed ( int key, int x, int y, int modkey )
{
	specialkeys[key]=true;
	special_mod_key=modkey;
}

void Input_Singleton::SpecialKeyReleased ( int key, int x, int y )
{
	specialkeys[key]=false;
	keyboard_mod_key=NULL;
}




