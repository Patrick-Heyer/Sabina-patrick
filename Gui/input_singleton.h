#ifndef INPUT_CONTENT_H
#define INPUT_CONTENT_H

#define  LEFT_BUTTON                   0x0000
#define  MIDDLE_BUTTON                 0x0001
#define  RIGHT_BUTTON                  0x0002
#define  DOWN                          0x0000
#define  UP                            0x0001

#define  ACTIVE_SHIFT                  0x0001
#define  ACTIVE_CTRL                   0x0002
#define  ACTIVE_ALT                    0x0004


#include <stdlib.h>

class InputSingleton
{
protected:
    // constructor/destructor
    InputSingleton( void ) {
        Initialize();
    }
    virtual ~InputSingleton( void ) { }

public:
    // singleton functions
    static InputSingleton	&getInstance( void );
    static void	 		FreeInstance( void );
    void	Initialize ( void );
	void 	ProcessMouse ( int button, int state, int x, int y, int modkey );
	void 	PassiveMotion (int x, int y);
	void 	ActiveMotion ( int x, int y );
	void 	KeyPressed ( unsigned char key, int x, int y, int modkey);
	void 	KeyReleased( unsigned char key, int x, int y );
	void 	SpecialKeyPressed ( int key, int x, int y, int modkey );
	void 	SpecialKeyReleased ( int key, int x, int y );
    
    bool keys[256];
    bool specialkeys[256];
    unsigned char key;
    unsigned char special_key;
    int keyboard_mod_key;
    int special_mod_key;
    int mouse_mod_key;
    int mouse_x;
    int mouse_y;
    int last_clicked_button;
	int left_click_x;
	int left_click_y;
	int right_click_x;
	int right_click_y;
	int middle_click_x;
	int middle_click_y;
	int left_active_x;
	int left_active_y;
	int right_active_x;
	int right_active_y;
	int middle_active_x;
	int middle_active_y;
	int last_released_button;
	int left_released_x;
	int left_released_y;
	int right_released_x;
	int right_released_y;
	int middle_released_x;
	int middle_released_y;
    bool mouse_active;
    bool left_button;
    bool middle_button;
    bool right_button;

private:

    static InputSingleton			*m_singleton;
};


#endif // INPUT_CONTENT_H
