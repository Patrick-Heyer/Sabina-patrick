#include "gui.h"

#define PI 3.141592

gui::gui ( int x, int y )
{
    size[0]=x;
    size[1]=y;
    AddWindow(0,0,0,100,990,"Task");
    AddWindow(1,250,-TITLE_BAR_HEIGHT,940,20,"Terminal");
    this->FindWindow("Terminal")->hide=true;

}

void gui::AddWindow ( bool max, int x, int y, int w, int h, std::string label )
{
    window *temp;
    temp= new window (max, x, y, w, h, label, *this );
    temp->hide=false;
    this->AddWidget ( &temp->hide,"State_Button", label, "Task" );
    windows.push_back ( ( window* ) temp );
}

void gui::AddWidget(bool* checar, std::string type, std::string label, std::string par)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        if ( windows[i]->name==par )
        {
            windows[i]->AddWidget ( checar, type, label );
        }
    }

}

void gui::AddWidget ( float* checar, std::string type, std::string label, std::string par, float incre)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        if ( windows[i]->name==par )
        {
            windows[i]->AddWidget ( checar, type, label, incre );
        }
    }
}

void gui::AddWidget(std::string type, std::string label, std::string par)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        if ( windows[i]->name==par )
        {
            windows[i]->AddWidget ( type, label );
        }
        if ( type=="Locator" )
        {
            if (windows[i]->FindWidget(par)!=NULL)
            {
                windows[i]->AddWidget(&par,type,label);
            }

        }
    }

}


void gui::AddWidget(std::string *checar, std::string type, std::string label, std::string par)
{

    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        if ( windows[i]->name==par )
        {
            windows[i]->AddWidget (checar, type, label );
        }
    }



}

void gui::AddWidget(int x, int y, std::string type, std::string label, std::string par)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        if ( windows[i]->name==par )
        {
            windows[i]->AddWidget ( x, y, type,  label );
        }
    }
}


window* gui::FindWindow(std::string label)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        if (windows[i]->name==label) {
            return windows[i];

        }

    }
    return NULL;
}


widget* gui::FindWidget(std::string label)
{

    for ( unsigned int i=0; i<windows.size(); i++ )
    {

        if (windows[i]->FindWidget (label)!=NULL)
        {
            return windows[i]->FindWidget (label);
        }
    }

}

void gui::Mouse_Left_Click(int x, int y)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        windows[i]->Mouse_Left_Click (x,y);
    }
}

void gui::Mouse_Right_Click(int x, int y)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        windows[i]->Mouse_Right_Click (x,y);
    }
}

void gui::Mouse_Left_Released(int x, int y)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        windows[i]->Mouse_Left_Released (x,y);
    }
}

void gui::Mouse_Right_Released(int x, int y)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        windows[i]->Mouse_Right_Released (x,y);
    }
}

void gui::Mouse_Over_Active_L(int x, int y)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        windows[i]->Mouse_Over_Active_L(x,y);
    }
}

void gui::Mouse_Over_Active_R(int x, int y)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        windows[i]->Mouse_Over_Active_R(x,y);
    }
}

void gui::Mouse_Over_Pasive(int x, int y)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        windows[i]->Mouse_Over_Pasive (x,y);
    }
}

void gui::KillWindow(std::string label)
{

    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        if ( windows[i]->name==label )
        {
            windows[i]->Kill();
            windows.erase(windows.begin()+i);
        }
    }
    this->KillWidget(label);
}

void gui::KillWidget(std::string label)
{
    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        if (windows[i]->FindWidget(label)!=NULL)
        {
            windows[i]->KillWidget(label);
        }
    }
}


void gui::Draw()
{
    glLoadIdentity();
    glDisable ( GL_TEXTURE_2D );
    glCullFace ( GL_FRONT );

    setOrthographicProjection ( size[0], size[1] );

    for ( unsigned int i=0; i<windows.size(); i++ )
    {
        windows[i]->Draw();
    }
    resetPerspectiveProjection();
    glCullFace ( GL_BACK );
}

void gui::ReSize ( int x, int y )
{   if(x>0)  size[0]=x;
    if(y>0)  size[1]=y;
}

window::window (bool max, int x, int y, int w, int h, std::string label, gui &par )
{
    parent=&par;
    pos[0]=x;
    pos[1]=y;
    size[0]=w;
    size[1]=h;
    name=label;
    widpos=TITLE_BAR_HEIGHT+BUTTON_BORDER;
    state=max;
    color_normal[0]=0;
    color_normal[0]=0;
    color_normal[0]=.8;
    color_normal[0]=.5;
    color_mouse_pasive_over[0]=0;
    color_mouse_pasive_over[1]=0;
    color_mouse_pasive_over[2]=.8;
    color_mouse_pasive_over[3]=1;
    moving=false;
}



//BUTTON AND STATE BUTTON
void window::AddWidget(bool* checar, std::string type, std::string label)
{

    if ( type=="Button" )
    {

        button *temp;
        temp= new button (checar, pos[0]+BUTTON_BORDER, pos[1]+widpos, size[0]- ( BUTTON_BORDER*2 ), BUTTON_HEIGHT, label, *this );
        temp->widpos=widpos;
        temp->tipo="Button";
        temp->type=0;
        widgets.push_back ( ( widget* ) temp );
        widgets.push_back ( ( widget* ) temp );
        widpos+=BUTTON_HEIGHT+BUTTON_BORDER;
        if (size[1]<=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER)
            size[1]=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER;
    }

    if ( type=="State_Button" )
    {
        button *temp;
        temp= new button (checar, pos[0]+BUTTON_BORDER, pos[1]+widpos, size[0]- ( BUTTON_BORDER*2 ), BUTTON_HEIGHT, label, *this );
        temp->widpos=widpos;
        temp->tipo="State_Button";
        temp->type=1;
        widgets.push_back ( ( widget* ) temp );
        widpos+=BUTTON_HEIGHT+BUTTON_BORDER;
        if (size[1]<=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER)
            size[1]=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER;
    }

}

void window::AddWidget ( float* checar, std::string type, std::string label, float incre )
{
    if ( type=="Value_Button" )
    {
        button *temp;
        temp= new button (checar, pos[0]+BUTTON_BORDER, pos[1]+widpos, size[0]- ( BUTTON_BORDER*2 ), BUTTON_HEIGHT, label, *this, incre);
        temp->widpos=widpos;
        temp->tipo="Drop";
        temp->type=2;
        widgets.push_back ( ( widget* ) temp );
        widpos+=BUTTON_HEIGHT+BUTTON_BORDER;
        if (size[1]<=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER)
            size[1]=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER;
    }
}

void window::AddWidget(std::string type, std::string label)
{
    if ( type=="Text" )
    {
        textbox *temp;
        temp= new textbox ( pos[0]+BUTTON_BORDER, pos[1]+widpos, size[0]- ( BUTTON_BORDER*2 ), BUTTON_HEIGHT, label, *this);
        char c;
        temp->widpos=widpos;
        temp->tipo="Text";
        for ( c=0; c<label.length(); c++ )
        {
            if (label[c]=='\n')
            {
                widpos+=16;
            }
        }

        widgets.push_back ( ( widget* ) temp );
        if (size[1]<=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER)
            size[1]=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER;
    }
}

void window::AddWidget(std::string* checar, std::string type, std::string label)
{

    if ( type=="Drop" )
    {

        drop *temp;
        temp= new drop (checar,label, pos[0]+BUTTON_BORDER, pos[1]+widpos, size[0]- ( BUTTON_BORDER*2 ), BUTTON_HEIGHT,  *this );
        temp->widpos=widpos;
        temp->tipo="Drop";

        widgets.push_back ( ( widget* ) temp );
        widpos+=BUTTON_HEIGHT+BUTTON_BORDER;
        if (size[1]<=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER)
            size[1]=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER;
    }
    if ( type=="Text" )
    {

        textbox *temp;
        temp= new textbox (checar, pos[0]+BUTTON_BORDER, pos[1]+widpos, size[0]- ( BUTTON_BORDER*2 ), BUTTON_HEIGHT, label, *this);
        char c;
        temp->widpos=widpos;
        temp->tipo="Text";
        for ( c=0; c<label.length(); c++ )
        {
            if (label[c]=='\n')
            {
                widpos+=16;
            }
        }
        widgets.push_back ( ( widget* ) temp );
        widpos+=BUTTON_HEIGHT+BUTTON_BORDER;
        if (size[1]<=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER)
            size[1]=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER;
    }

    if ( type=="Locator" )
    {
        locator *temp;
        temp= new locator(label, checar, *this);
        temp->tipo="Locator";
        widgets.push_back ( ( widget* ) temp );
    }
}

void window::AddWidget(int x, int y,std::string type, std::string label)
{
    if (type=="Image")
    {
        image *temp;
        temp=new image( label, *this);
        temp->pos[0]=this->pos[0];
        temp->pos[1]=this->pos[1]+widpos;
        temp->size[0]=x;
        temp->size[1]=y;
        temp->widpos=widpos;
        temp->tipo="Image";
        if (x>this->size[0]+BUTTON_BORDER) ReSize(x+BUTTON_BORDER,this->size[1]);
        if (y+widpos>this->size[1]+BUTTON_BORDER) ReSize(this->size[0],y+widpos+BUTTON_BORDER);
        widgets.push_back( ( widget* ) temp );
        widpos+=BUTTON_BORDER+y;
        widgets.push_back ( ( widget* ) temp );
        widpos+=BUTTON_HEIGHT+BUTTON_BORDER;
        if (size[1]<=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER)
            size[1]=widpos+TITLE_BAR_HEIGHT+BUTTON_BORDER;
    }
}

widget* window::FindWidget(std::string label)
{

    for ( unsigned int i=0; i<widgets.size(); i++ )
    {
        if (widgets[i]->name==label) {
            return widgets[i];

        }

    }
    return NULL;
}

void window::KillWidget(std::string label)
{
    for ( unsigned int i=0; i<widgets.size(); i++ )
    {
        if ( widgets[i]->name==label )
        {
            widgets.erase(widgets.begin()+i);
        }
    }

}


void window::Mouse_Left_Click(int x, int y)
{
    if ( x>=pos[0]+size[0]-60-TITLE_BAR_HEIGHT && x<=pos[0]+size[0]-62 && y>=pos[1] && y<=pos[1]+TITLE_BAR_HEIGHT && name!="Task")
    {
        state ? state=MINIMIZED: state=MAXIMIZED;
    }

    else if ( x>=pos[0]+size[0]-40-TITLE_BAR_HEIGHT && x<=pos[0]+size[0]-42 && y>=pos[1] && y<=pos[1]+TITLE_BAR_HEIGHT && name!="Task")
    {

        this->parent->KillWindow(this->name);
    }

    else if ( x>=pos[0] && x<=pos[0]+size[0] && y>=pos[1] && y<=pos[1]+TITLE_BAR_HEIGHT && name=="Task")
    {
        state ? state=MINIMIZED: state=MAXIMIZED;
    }
    else if ( x>=pos[0] && x<=pos[0]+size[0] && y>=pos[1] && y<=pos[1]+TITLE_BAR_HEIGHT)
    {
        moving=true;
    }


    for ( unsigned int i=0; i<widgets.size(); i++ )
    {
        widgets[i]->Mouse_Left_Click(x,y);
    }
}

void window::Mouse_Right_Click(int x, int y)
{

    for ( unsigned int i=0; i<widgets.size(); i++ )
    {
        widgets[i]->Mouse_Right_Click(x,y);
    }
}




void window::Mouse_Left_Released(int x, int y)
{
    moving=false;
    for ( unsigned int i=0; i<widgets.size(); i++ )
    {
        widgets[i]->Mouse_Left_Released(x,y);
    }
}

void window::Mouse_Right_Released(int x, int y)
{
    for ( unsigned int i=0; i<widgets.size(); i++ )
    {
        widgets[i]->Mouse_Right_Released(x,y);
    }
}

void window::Mouse_Over_Active_L(int x, int y)
{
    if (moving)
    {
        Move(x-size[0]/2,y-TITLE_BAR_HEIGHT/2 );
    }
    for ( unsigned int i=0; i<widgets.size(); i++ )
    {
        widgets[i]->Mouse_Over_Active_L(x,y);
    }
}

void window::Mouse_Over_Active_R(int x, int y)
{
    if (moving)
    {
        Move(x-size[0]/2,y-TITLE_BAR_HEIGHT/2 );
    }
    for ( unsigned int i=0; i<widgets.size(); i++ )
    {
        widgets[i]->Mouse_Over_Active_R(x,y);
    }
}

void window::Mouse_Over_Pasive(int x, int y)
{
    for ( unsigned int i=0; i<widgets.size(); i++ )
    {
        widgets[i]->Mouse_Over_Pasive(x,y);
    }

}

void window::Kill()
{
    widgets.clear();;
}



void window::DrawTitle()
{
    state==MAXIMIZED ? uiSetRoundBox ( 12 ) : uiSetRoundBox ( 15 );

    glColor4fv (color_mouse_pasive_over);
    gl_round_box_shade ( GL_POLYGON,pos[0],pos[1],pos[0]+size[0],pos[1]+TITLE_BAR_HEIGHT,CORNER_RADIUS,.5,1 );
    DrawText ( pos[0]+20,pos[1]+TITLE_BAR_HEIGHT-3, name );

    if(name!="Task")
    {

        glColor4f (1,0,0,1);
        uiSetRoundBox ( 15 );
        gl_round_box( GL_POLYGON,pos[0]+size[0]-40-TITLE_BAR_HEIGHT,pos[1]+1,pos[0]+size[0]-42,pos[1]+TITLE_BAR_HEIGHT-2,6 );


        glColor4f (0,1,0,1);
        uiSetRoundBox ( 15 );
        gl_round_box( GL_POLYGON,pos[0]+size[0]-60-TITLE_BAR_HEIGHT,pos[1]+1,pos[0]+size[0]-62,pos[1]+TITLE_BAR_HEIGHT-2,6 );
        DrawText ( pos[0]+size[0]-40-(TITLE_BAR_HEIGHT-3),pos[1]+TITLE_BAR_HEIGHT-3, "X" );
        DrawText ( pos[0]+size[0]-60-(TITLE_BAR_HEIGHT-3),pos[1]+TITLE_BAR_HEIGHT-5, ">" );
    }
    glColor3f ( 0,0,0 );
    if ( state==MAXIMIZED )
    {
        glBegin ( GL_TRIANGLES );
        glVertex2i ( pos[0]+10,pos[1]+2 );
        glVertex2i ( pos[0]+15,pos[1]+5 );
        glVertex2i ( pos[0]+10,pos[1]+8 );
        glEnd();
    }
    else
    {
        glBegin ( GL_TRIANGLES );
        glVertex2i ( pos[0]+10,pos[1]+2 );
        glVertex2i ( pos[0]+15,pos[1]+2 );
        glVertex2i ( pos[0]+13,pos[1]+8 );
        glEnd();
    }

}

void window::DrawRest()
{
    uiSetRoundBox ( 3 );
    glColor4f(0,0,1,.5);
    uiRoundBox ( pos[0],pos[1]+TITLE_BAR_HEIGHT,pos[0]+size[0],pos[1]+size[1],CORNER_RADIUS );
}


void window::Draw()
{
    if (!hide)
    {
        glColor3f(1,1,1);
        DrawTitle();
        if (state)
        {
            DrawRest();
            for ( unsigned int i=0; i<widgets.size(); i++ )
            {
                widgets[i]->Draw();
            }
        }
    }
}

void window::Move ( int x, int y )
{
    pos[0]=x;
    pos[1]=y;
    for ( unsigned int i=0; i<widgets.size(); i++ )
    {
        widgets[i]->Move();
    }
}

void window::ReSize ( int x, int y )
{
    size[0]=x;
    size[1]=y;
}

void widget::Move(int x, int y)
{

}

void widget::Move()
{
    pos[0]=parent->pos[0]+BUTTON_BORDER;
    pos[1]=parent->pos[1]+widpos;
}

void widget::Draw()
{


}

button::button (bool* checar, int x, int y, int w, int h, std::string label, window& par )
{
    parent=&par;
    pos[0]=x;
    pos[1]=y;
    size[0]=w;
    size[1]=h;
    name=label;
    state=checar;
    clicked=false;
}

button::button(float* checar, int x, int y, int w, int h, std::string label, window& par, float incre)
{
    parent=&par;
    pos[0]=x;
    pos[1]=y;
    size[0]=w;
    size[1]=h;
    name=label;
    value=checar;
    incremento=incre;
    clicked=false;
}


void button::Mouse_Left_Click(int x, int y)
{
    if (type==0)
    {
        if ( x>=pos[0] && x<=pos[0]+size[0] && y>=pos[1] && y<=pos[1]+size[1])
        {
            *state=true;
            clicked=true;
        }
    }
    if (type==1)
    {
        if ( x>=pos[0] && x<=pos[0]+size[0] && y>=pos[1] && y<=pos[1]+size[1])
        {
            *state ? *state=false: *state=true;
            clicked ? clicked=false: clicked=true;
        }
    }

    if (type==2)
    {
        if ( x>=pos[0] && x<=pos[0]+size[0]/6 && y>=pos[1] && y<=pos[1]+size[1])
        {
            *value-=incremento;
            clicked=true;
        }
        if ( x>=pos[0]+(size[0]/6)*2 && x<=pos[0]+size[0] && y>=pos[1] && y<=pos[1]+size[1])
        {
            *value+=incremento;
            clicked=true;
        }

    }

}

void button::Mouse_Right_Click(int x, int y)
{

}

void button::Mouse_Left_Released(int x, int y)
{
    if (type==0)
    {
        *state=false;
        clicked=false;
    }
    if (type==2)
    {
        clicked=false;
    }
}

void button::Mouse_Right_Released(int x, int y)
{

}

void button::Mouse_Over_Active_L(int x, int y)
{

}

void button::Mouse_Over_Active_R(int x, int y)
{

}

void button::Mouse_Over_Pasive(int x, int y)
{

}

void button::Draw()
{
    if (type==1 || type==0)
    {
        if (clicked)  glColor4f(.3,1,.3,.8);
        else glColor4f(.3,.3,1,.8);


        uiSetRoundBox ( 15 );
        gl_round_box_shade ( GL_POLYGON,pos[0],pos[1],pos[0]+size[0],pos[1]+size[1],BUTTON_RADIUS,.1,.5 );
        DrawText ( pos[0]+20,pos[1]+BUTTON_HEIGHT-3, name );
    }
    if (type==2)
    {
        std::ostringstream stm;
        stm <<*value;


        uiSetRoundBox ( 15 );

        if (clicked)  glColor4f(.3,1,.3,.8);
        else glColor4f(.3,.3,1,.8);
        gl_round_box_shade ( GL_POLYGON,pos[0],pos[1],pos[0]+size[0],pos[1]+size[1],BUTTON_RADIUS,.1,.5 );

        glColor4f(.1,.1,1,1);
        uiSetRoundBox ( 9 );
        gl_round_box_shade ( GL_POLYGON,pos[0],pos[1],pos[0]+size[0]/6,pos[1]+size[1],BUTTON_RADIUS,.1,.5 );
        glColor4f(.1,.1,1,1);
        uiSetRoundBox ( 6 );
        gl_round_box_shade ( GL_POLYGON,pos[0]+size[0]-(size[0]/6),pos[1],pos[0]+size[0],pos[1]+size[1],BUTTON_RADIUS,.1,.5 );
        DrawText ( pos[0]+(size[0]/2)-name.length(),pos[1]+BUTTON_HEIGHT-3, name );
        DrawText ( pos[0]+5,pos[1]+BUTTON_HEIGHT-3, " - " );
        DrawText ( pos[0]+50,pos[1]+BUTTON_HEIGHT-3, stm.str() );
        DrawText ( pos[0]+(size[0]-15),pos[1]+BUTTON_HEIGHT-3, " + " );
    }


}

textbox::textbox(int x, int y, int w, int h, std::string label, window& par)
{
    parent=&par;
    pos[0]=x;
    pos[1]=y;
    size[0]=w;
    size[1]=h;
    text=label;
    inter=false;
    char c;
    for ( c=0; c<text.length(); c++ )
    {
        if (text[c]=='\n')
        {
            size[1]+=12;
            parent->size[1]+=12;
        }

    }

}

textbox::textbox(std::string *checar , int x, int y, int w, int h, std::string label, window& par)
{
    parent=&par;
    pos[0]=x;
    pos[1]=y;
    size[0]=w;
    size[1]=h;
    text=label;
    value=checar;
    inter=true;
    char c;
    for ( c=0; c<text.length(); c++ )
    {
        if (text[c]=='\n')
        {
            size[1]+=12;
            parent->size[1]+=12;
        }

    }
}



void textbox::Draw()
{
    glEnable(GL_BLEND);
    if (inter)
    {
        text=*value;

    }
    glBegin ( GL_QUADS );
    glVertex2f ( pos[0]+BUTTON_BORDER, pos[1]+size[1]-BUTTON_BORDER );
    glVertex2f ( pos[0]+BUTTON_BORDER, pos[1]+BUTTON_BORDER );
    glVertex2f ( pos[0]+size[0]-BUTTON_BORDER, pos[1]+BUTTON_BORDER );
    glVertex2f ( pos[0]+size[0]-BUTTON_BORDER, pos[1]+size[1]-BUTTON_BORDER );
    glEnd();
    glDisable(GL_BLEND);
    DrawText(pos[0]+5, pos[1]+TITLE_BAR_HEIGHT-3, text);
}

image::image(std::string label, window& par)
{
    parent=&par;

    name=label;
    texid=LoadTexture(label.c_str());
}

void image::Draw()
{
    glEnable (GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, texid);

    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(pos[0],pos[1]);
    glTexCoord2f(1,0);
    glVertex2f(pos[0]+size[0],pos[1]);
    glTexCoord2f(1,1);
    glVertex2f(pos[0]+size[0],pos[1]+size[1]);
    glTexCoord2f(0,1);
    glVertex2f(pos[0],pos[1]+size[1]);
    glEnd();

    glDisable (GL_TEXTURE_2D);

}

locator::locator(std::string label, std::string *ref, window& par)
{
    parent=&par;
    widget *temp;
    temp=parent->parent->FindWidget(*ref);
    pos[0]=temp->pos[0];
    pos[1]=temp->pos[1];
    size[0]=temp->size[0];
    size[1]=temp->size[1];
    name=label;
    x=0;
    y=0;
    rotation=0;
    brother=temp;

}

void locator::Draw()
{
    if (rotation<-179) rotation+=360;
    if (rotation>180) rotation-=360;
    if(this->x<=0) x=0;
    if(this->x>=size[0]) x=size[0];
    if(this->y<=0) y=0;
    if(this->y>=size[1]) y=size[1];
    glDisable (GL_TEXTURE_2D);
    glColor4f(1,0,0,.5);
    glBegin(GL_LINES);
    glVertex2f(pos[0],pos[1]+y);
    glVertex2f(pos[0]+size[0],pos[1]+y);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(pos[0]+x,pos[1]);
    glVertex2f(pos[0]+x,pos[1]+size[1]);
    glEnd();


    glColor4f(1,1,1,1);
    glBegin(GL_LINES);
    glVertex2f(pos[0]+x,pos[1]+y);
    glVertex2f(pos[0]+x+(cos(-1*rotation*PI/180)*100),pos[1]+y+(sin(-1*rotation*PI/180)*100));
    glEnd();


    glPointSize ( 50 );
}

void locator::Move()
{
    pos[0]=brother->pos[0];
    pos[1]=brother->pos[1];
}


void locator::Mouse_Left_Click(int x, int y)
{
    if ( x>=pos[0] && x<=pos[0]+size[0] && y>=pos[1] && y<=pos[1]+size[1])
    {
        this->x=x-pos[0];
        this->y=y-pos[1];
    }
}

void locator::Mouse_Right_Click(int x, int y)
{
    if ( x>=pos[0] && x<=pos[0]+size[0] && y>=pos[1] && y<=pos[1]+size[1])
    {
        float angle=atan2(pos[0]+this->x-x, pos[1]+this->y-y);

        rotation=angle* 180 / PI;
        rotation=rotation*-1;
        rotation=rotation-90;
        int temp=rotation;
        rotation=temp*-1;
    }

}


void locator::Mouse_Over_Active_L(int x, int y)
{
    if ( x>=pos[0] && x<=pos[0]+size[0] && y>=pos[1] && y<=pos[1]+size[1])
    {
        this->x=x-pos[0];
        this->y=y-pos[1];
    }
}

void locator::Mouse_Over_Active_R(int x, int y)
{
    if ( x>=pos[0] && x<=pos[0]+size[0] && y>=pos[1] && y<=pos[1]+size[1])
    {
        float angle=atan2(pos[0]+this->x-x, pos[1]+this->y-y);

        rotation=angle* 180 / PI;
        rotation=rotation*-1;
        rotation=rotation-90;
        int temp=rotation;
        rotation=temp*-1;
    }

}


drop::drop(std::string* checar, std::string label, int x, int y, int w, int h, window& par)
{
    parent=&par;
    pos[0]=x;
    pos[1]=y;
    size[0]=w;
    size[1]=h;
    clicked=false;
    value=checar;
    name="polo";

    std::string s(label);
    size_t pos;

    while ( (pos = s.find('(')) != std::string::npos || (pos = s.find(')')) != std::string::npos )
    {
        std::string tmp = s.substr(0,pos) + s.substr(pos+1);
        s = tmp;
    }
    std::istringstream iss(s);

    std::string sub;
    iss >> sub;
    name=sub;
    iss >> sub;
    int i=0;
    do
    {
        options[i]=sub;
        i++;
        iss >> sub;
    } while (iss);
    num=0;
    max=i-1;
    *checar=name+" "+options[0];
}

void drop::Draw()
{


    uiSetRoundBox ( 15 );

    if (clicked)  glColor4f(.3,1,.3,.8);
    else glColor4f(.3,.3,1,.8);
    gl_round_box_shade ( GL_POLYGON,pos[0],pos[1],pos[0]+size[0],pos[1]+size[1],BUTTON_RADIUS,.1,.5 );

    glColor4f(.1,.1,1,1);
    uiSetRoundBox ( 9 );
    gl_round_box_shade ( GL_POLYGON,pos[0],pos[1],pos[0]+size[0]/6,pos[1]+size[1],BUTTON_RADIUS,.1,.5 );
    glColor4f(.1,.1,1,1);
    uiSetRoundBox ( 6 );
    gl_round_box_shade ( GL_POLYGON,pos[0]+size[0]-(size[0]/6),pos[1],pos[0]+size[0],pos[1]+size[1],BUTTON_RADIUS,.1,.5 );
    DrawText ( pos[0]+5,pos[1]+BUTTON_HEIGHT-3, " - " );
    DrawText ( pos[0]+50,pos[1]+BUTTON_HEIGHT-3, name+"="+options[num]);
    DrawText ( pos[0]+(size[0]-15),pos[1]+BUTTON_HEIGHT-3, " + " );
}

void drop::Mouse_Left_Click(int x, int y)
{
    if ( x>=pos[0] && x<=pos[0]+50 && y>=pos[1] && y<=pos[1]+size[1])
    {
        if (num>0) num--;

        else num=max;

        clicked=true;
    }
    if ( x>=pos[0]+(size[0]-50) && x<=pos[0]+size[0] && y>=pos[1] && y<=pos[1]+size[1])
    {
        if (num<max) num++;

        else num=0;

        clicked=true;
    }
    *value=name+" "+options[num];

}

void drop::Mouse_Left_Released(int x, int y)
{
    clicked=false;
}


void drop::Mouse_Right_Click(int x, int y)
{
    std::string polo="";
    if ( x>=pos[0] && x<=pos[0]+size[0] && y>=pos[1] && y<=pos[1]+size[1])
    {
        parent->parent->AddWindow( 1, x, y, 100, 50, "opciones" );
        for (int i=0; i<=max; i++)
        {
            polo=polo+options[i]+"\n";
        }
        parent->parent->AddWidget("Text", polo, "opciones");
    }
}

void drop::Mouse_Right_Released(int x, int y)
{

    parent->parent->KillWindow("opciones");

}







