#ifndef __UNIVERSALS__H__
#define __UNIVERSALS__H__

	 /*
	 Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/



#include "crystalspace.h"


#define DEF_ECLOSE 10
//Ideally, each .cpp file should have its own .h, but the compiler
//kept confusing which ones to compile first, so I put everything in one linear header.

//1 gives alot of print info
#define DBG 0

//1 gives alot of print info
#define DBG2 0


//stores colored text
class e_text
{
public:
char c1[110];
int r[110];
int g[110];
int b[110];
int a[110];
int br[110];
int bg[110];
int bb[110];
int ba[110];


e_text();
e_text(char *c1);

//background

};



struct my_mouse
{
int x;
int y;
int button1;
int button2;
};


class GUT
{
public:
///Storage
int stor1;

int x,y;
int x_size,y_size;
//until multiple popup pages is enabled, this should always be 0
int page;
//0-BMAP3,1-BMAP1,2-TEXTBOX,3-TEXTLINE
int type;
//style of gut, item, attribute, helpbutton
//TYPE2 is not used anymore really, but left for hecks sake
//init type2 of guts to 0 I guess or remove it
int type2;


//ONLY HERE FOR TEXT CRUD
e_text e;


//TEXT storage





GUT();
//Draw over top another pixmap:
virtual void debug_display(int pos,int px,int py);
virtual void kill(void);
virtual void draw(iImage *which);
virtual void draw(int pos,int px,int py, iImage *dest);

//Ba
virtual void set_text(char *c1);

};

//has three bitmaps when mouse is around.
//Normally this is to show:
//b[0] - idling, no mouse over
//b[1] - brighter, mouse over
//b[2] - Darker, mouse down

class BMAP3 : public GUT
{
private:
//debug_display variables:
int txt_reg;
csRef<iTextureManager> txtmgr;
csRef<iTextureHandle> txt;
//end of debug_display variables

public:
//0 nothin over, 1 over, 2 mouse down
csRef<iImage> b[3];
//Draw over another pixmap



//Pos is which iImage to display, 0,1 or 2
virtual void debug_display(int pos,int px,int py);
virtual void kill(void);
virtual void draw(iImage *dest);
virtual void draw(int pos,int px,int py, iImage *dest);
// Old version of draw, took which graphics page to print to, but this version is dumbed down
//It just prints to the main screen.  Also I can't concieve of a video page in Crystalspace.
//virtual void draw(VIDEO_PAGE_TO_DISPLAY_ON *VIDEOPAGE);


BMAP3();

virtual ~BMAP3(){}
};

//only 1 bitmap when mouse is around.
class BMAP1 : public GUT
{
private:
//debug_display variables:
int txt_reg;
csRef<iTextureManager> txtmgr;
csRef<iTextureHandle> txt;
//end of debug_display variables
public:
csRef<iImage> b;
virtual void kill(void);
virtual void draw(iImage *dest);
virtual void draw(int pos,int px,int py, iImage *dest);
virtual void debug_display(int pos,int px,int py);
BMAP1();
BMAP1(iImage *);

virtual ~BMAP1(){}
};

class TEXTBOX : public GUT
{

public:
//how many letters
int maxlen;
//0-Any Text
//1-Decimal Box, numbers and .
//2-Filename -maxlen should be 8, filter anything not numbers/letters
int filter;
//0 nothin over, 1 over, 2 mouse down
//csRef<iImage> b;
//Draw over another pixmap

virtual void kill(void);
virtual void draw(iImage *dest);
virtual void draw(int pos,int px,int py, iImage *dest);

virtual void set_text(char *c1);

TEXTBOX();
TEXTBOX(int filt,int maxl);
};

class TEXTLINE : public GUT
{

public:

virtual void set_text(char *c1);
virtual void kill(void);
virtual void draw(iImage *dest);
virtual void draw(int pos,int px,int py, iImage *dest);
TEXTLINE();
};

class popup
{
public:
//d_text name;
popup *next;
popup *prev;
int type;
int stay;
csRef<iImage> display;
//iImage *display;
int xb,yb;
int x_pos,y_pos;
int x_size,y_size;


e_text name;
//
int bordercolor_r;
int bordercolor_g;
int bordercolor_b;
int wincolor_r;
int wincolor_g;
int wincolor_b;




int guts;
popup();
int pages;
int curpage;
//GUT **G;
 csArray<GUT*> G3 ;

//delete what it points to
//clicked x,y;
int cx,cy;
//-------------memory part---------------
int active;
int mx,my,mz;
int pos;
int max;
int mdata[30];
//memory for items
//ITEM_list *DI;
//ITEM_list *I;
//used for equip/dragging
//ITEM *it;


//Bunch of crap used for the editor
int *gx;
int *gy;
int *gpage;
int *gtype;
iImage **b1;
iImage **b2;
iImage **b3;
int data[20];
//End pile o' crap

void rename(char *c1);




};

struct action
{
int type;
int num;

};

class POP_UP_WINDOWS
{
private:
popup *bottom;

//UNCOMMENTED
action act;


//1=ok to draw 0=ok to remove
//int draw;


//old mouse button
int olb;
int priority;
int curuse;



//Multithreaded variables for dragging window
//Old code had a seperated while loop inside drag_window

///* multi thread part1, abandoned because too slow, pts 2,3 in popup_awt

//For the window dragging
popup *dragging;
int original_mousex;
int original_mousey;

//For the window closing
popup *xclosin;
//1- means left click, 2 is right click
//left click means single window close
//right click means close all windows that aren't saved
int close_int;



//*/


public:
popup *top;
//UNCOMMENTED
POP_UP_WINDOWS();
int pskip;

int show;

//EVENTS RETURNED!
//Look into these to gather information about closed windows, ok buttons, and text fields.
//You could totally code in behavoir based on virtual functions in guts, but I chose
//the easy way out for my app.  You can do whatever you want in your app, but Events
//returned simply gives data about what happened.

//I used to handle using the click function, but events work too, whatever you feel like using.
//Events are prolly better, since you don't recode right into the class you're supposed to be building on.

popup *EV_window;
//
int EV_max_closed;
//Just to save myself some headache, only 10 windows max can be detected as closed at once
//You can mod this code if you want
int EV_closed[DEF_ECLOSE];
//index of textfields -1 means no text fields
//int EV_text_index;
//char **EV_text;
//array of strings for text fields
//for(i=0;i<EV_text_index;i++)
//printf(EV_text[i]);
// prints out all the text

// gut number exited  -1 means no guts left
int EV_gutn0;
// gut number entered or on  -1 means no guts over
int EV_gutn1;

//-1 means who knows
//old mouse button 0none, 1-left, 2-right, 3 both
//4 means someone pressed enter on a textgut
int EV_old_mb;
//new mouse button 0none, 1-left, 2-right, 3 both
int EV_new_mb;





////////////////////////
///End of event crap
/////////////////////////





////////////////////////
///START OF TEXTBOX FOCUS STUFF
////////////////////////
//focus is used to route keyboard commands to a GUT, namely TEXTBOX
//if focus is present, keyboard commands should go there instead of normally used as a hotkey
popup *focus;
int focus_gut;

//lets say someone types enter(or another key), but no focus, this gets a focus
void gainfocus(void);
int enter_down;
void setfocus(popup *which, int gutn0);
void losefocus(void);

////////////////////////
///END OF TEXTBOX FOCUS STUFF
////////////////////////







//if show =1 display, if show=0, turn off overlay display
//csRef<iImage> overlay; not needed with double buffer


/////Main function, you need to customize it.
//Place in rules for individual popup types you want:
popup *open(int type, int show_y_n);
popup *open(int type);

/////higher level allocation functions:
//creates a popup based on its type
void generate(popup *which);
//draws the popup using the iImages csRef<iImage> xbut,x2but,vtbar,hzbar,hzbar2,grbut,rdbut,cornul,cornll,cornur,cornlr;
iImage *make_outline(popup *which);
//This just arrays out the memory so the popup can create buttons and stuff.
void alloc_guts(popup *which,int amt);


//// Important functions to add or delete popups from the windowing system
void push(popup *which);

//draws the windowing system
void show_windows(void);
//Big function, called when mouse is clicked
//right or left click activates
int check_popups(void);



////Important functions designed to shift,navigate,move and rearrange the popups
void place_on_top(popup *which);
void place_on_top_n_draw(popup *which);
void place_on_bottom(popup *which);
void place_on_bottom_n_draw(popup *which);


////Some helper functions used by functions:
//Enforces a single copy of a window int type
//if already existing, returns a 1, otherwise 0
//Good if you don't want to recreate a window or int type that already is shown :)
//If window exists, it brings it to the top
int single_copy(int type);
// single copy uses this function
popup *does_window_exist(int type);




///Just a bunch of random functions:
//void make_overlay(void);
//void add_to_top_n_draw(popup *which);
//void add_to_top(popup *which);



//More random functions
void call_defunct_gut(popup *which,int gutn0);
void hide_all(void);

void destroy(popup *which);
//left click x, closes 1 window
void close_window(popup *which);
//special case closes all
void close_all(void);
//right clicked window closes all windows, except ones with green light marked
void close_all_marked(void);



int close_button1(popup *which, int x,int y);
int close_button2(popup *which, int x,int y);


int is_mouse_on_a_higher_window(popup *which,int x,int y);
void show_gut(popup *which,int gutn0,int bitmap_one_two_or_three);
void clicked_on_something(popup *which,int gutn0);

int click(popup *which,int gutn0);

int stay_button(popup *which,int x,int y);
int do_popup(popup *which,int x,int y);


void drag_window(popup *which);
void drag_window2(popup *which);


//adding:
//int CountTextFields(popup *which);
//char *GetTextField(int i);
//void SetTextField(int i, char *c1);
//


void clear_events(void);

BMAP3 *boxize(popup *which,int gutn0,int wx,int hy,int r,int g,int b);
//for making a button with a line of text in it

TEXTBOX *textboxize(popup *which,int gutn0,int r,int g,int b);
TEXTLINE *textlineize(popup *which,int gutn0,int r,int g,int b);


void insert_guts(popup *which);


void send_focus(char c);





//sends line of text and says:error
popup *perror (char *c1);


//CUSTOM GENERATES
//THESE SHOULD BE DIFFERENT FOR EVERY APPLICATION YOU MAKE!
//1 for every unique window.
//If you're super slick, you can make general popups.
//You can also load information from a file to generate them.
//But in general, the quick way to make an ap, is to
//copy a case section from ::open, mod it to your liking
//Make a new generate entry in ::generate
//Then make your function
void generate_textbox(popup *which);
void generate_insertmesh(popup *which);
void generate_saveskeleton(popup *which);
void generate_loadskeleton(popup *which);
void generate_quit(popup *which);
//overwritesave
void generate_overwrite(popup *which);
void generate_frame(popup *which);
void generate_saveanim(popup *which);
void generate_overwrite_anim(popup *which);
void generate_anim_controller(popup *which);
void generate_loadanim(popup *which);
/*
//void generate_name(popup *which);
//display stuff
//redraw all
//void redraw(popup *which);
//for popup
//void redraw(int type);
//end for popup

//NULL=no

//void set_action(int action_n0,int ref);

//simply arranges it in buffer
//arranges in data, assembles display, and blits it. Dirty like.



BMAP3 *boxize(popup *which,int gutn0,int wx,int hy,int color,int bright);
BMAP3 *boxize(popup *which,int gutn0,int wx,int hy,int color);
int check_scroll_bar(popup *which,int x,int y);
void display_gut(popup *which,int gutn0,int bitmap_one_two_or_three);
//starts in middle works way back up....

void do_popup(int popn0);


void dtext_to_box(BMAP3 *B,d_text d,int offset_x,int offset_y);
void text_to_box(BMAP3 *B,char *text,int text_color);
void text_to_box(BMAP3 *B,char *text,int text_color,int bright);
//
//FOR USE WITH POP UP crap
void text_to_box(popup *which,char *text,int boxn0, int text_color);
void text_to_box(popup *which,char *text,int boxn0, int text_color,int bright);
//END OF CRAP

void dtext_to_box(BMAP3 *B,d_text d);
//                                                            12 = 1x        16 = 1y
void col_to_box(BMAP3 *B,d_text *d,int leny);

//


void generate_attrib(popup *which);
//new stuff:

void generate_npc_help(popup *which);
void generate_item_help(popup *which);
void generate_wall_help(popup *which);
void generate_floor_help(popup *which);
void generate_action(popup *which);
void generate_block_wall(popup *which);
void generate_equip(popup *which);


void clicked_on_block_wall(popup *which,int gutn0);
void clicked_on_block(popup *which,int gutn0);
void generate_enviro(popup *which,int dit);
void generate_barrier(popup *which,int dit);
void generate_item(popup *which);
void clicked_on_item(popup *which,int gutn0);
void clicked_on_ditem(popup *which,int gutn0);
void clicked_on_block_and_ditem(popup *which,int gutn0);



int item_move_popup_check(void);



//more junk
int item_thing1(int ppi,popup * which,int gi);
int clicked_on_take(popup * which, int gutn0);



*/

//OLD FUNCTIONS FOR MY OLD GAME:
//void generate_inventory(popup *which);
//void call_function_to_make_inventory_selections(popup *which, int pos, int x,int y);

};

void SCF_DELETE(iImage *pointer);

extern csRef<iGraphics3D> gut_g3d;

extern csRef<iImage> xbut,x2but,vtbar,hzbar,hzbar2,grbut,rdbut,cornul,cornll,cornur,cornlr;
extern csRef<iImage> TEXT,TEXT2;
extern csRef<iImage> I_I[95];



extern csRef<iImage> I_nothing;


extern my_mouse M;
extern POP_UP_WINDOWS P;

#endif
