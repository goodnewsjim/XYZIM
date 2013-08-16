	 /*
	 Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/


#include "crystalspace.h"
#include "universals.h"
#include "TextHelper.h"
#include "vector"
#include "iImageHelper.h"

int msgbox_type[6];
 char msgbox0[80],msgbox1[80],msgbox2[80],msgbox3[80],msgbox4[80],msgbox5[80];

//csPixmap *xbut,*x2but,*vtbar,*hzbar,*hzbar2,*grbut,*rdbut,*cornul,*cornll,*cornur,*cornlr;

//POP_UP_WINDOWS is the class that stores all the windowing, normally you'd only need 1.
POP_UP_WINDOWS P1;

POP_UP_WINDOWS::POP_UP_WINDOWS ()
{
  olb = -1;
  pskip = 0;
  priority = 0;
  top = NULL;
  bottom = NULL;
  show = 1;

//if dragging = something, user is dragging a window.
  dragging = NULL;
//NULL unless user has mouse clicked in over an X(close) button and hasn't released
  xclosin = NULL;


//Which type is sending data- IE in my example
//0-*TEST*
//1-Basic text prompt
//-1 means not over a window so nothing could happen at all anyway.
  EV_window = NULL;
//1-yes closed 0-still lopened
//EV_close=0;
//index of textfields -1 means no text fields
//EV_text_index=-1;
//array of strings for text fields
//for(i=0;i<EV_text_index;i++)
//printf(EV_text[i]);
// prints out all the text
//EV_text=NULL;

// gut number modified  -1 means no guts leaving
  EV_gutn0 = -1;
// gut number modified  -1 means no guts entering
  EV_gutn1 = -1;


  EV_old_mb = -1;
//new mouse button 0none, 1-left, 2-right, 3 both
  EV_new_mb = -1;


  EV_max_closed = 0;


}


//popups are the individual things the POP_UP_WINDOWS keep track of and display


void
POP_UP_WINDOWS::clear_events (void)
{
//int i;
//Which type is sending data- IE in my example
//0-*TEST*
//1-Basic text prompt
//-1 means not over a window so nothing could happen at all anyway.
  EV_window = NULL;
//1-yes closed 0-still opened
//EV_close=0;

//for(i=0;i<EV_text_index;i++)
//delete EV_text[i];


//normally just one event happens at a time, but a big mass closing may occur
  EV_max_closed = 0;
//index of textfields -1 means no text fields
///EV_text_index=-1;
//array of strings for text fields
//for(i=0;i<EV_text_index;i++)
//printf(EV_text[i]);
// prints out all the text
//EV_text=NULL;

// gut number modified  -1 means no guts
  EV_gutn0 = -1;
  EV_gutn1 = -1;

  EV_old_mb = -1;
//new mouse button 0none, 1-left, 2-right, 3 both
  EV_new_mb = -1;


}


//First thing you need to do to a popup is alloc its guts, for the number of things inside it:
//Say you had 2 buttons, cancel and ok, you'd allocate 2 guts :)
void
POP_UP_WINDOWS::alloc_guts (popup * which, int amt)
{
//static GUT **GGGGG;
  int i;
  which->guts = amt;
//  which->G3 = (GUT **) malloc (sizeof(GUT*) * amt);

//which->G3.SetCapacity(amt);
which->G3.SetSize(amt);
//std::vector<GUT*> G(amt);

  //this code is just the start of code, but why you'd want guts
  //without defining em is beyond me
///  if (amt == 0)
///{
///  which->G3 = NULL;
///}


  for (i = 0; i < amt; i++)
{
///   which->G3[i] = NULL;
which->G3.Put(i,NULL);
}


}


//next you need to generate it somehow, you can do custom generates
//or if you assign types, you can generate things on the fly.

//This function should be different for about every windowing system you create
//Inside it tells which function to go into to custom paint the GUTS inside.

//Only have this function called once after open, it allocates memory.

//TO repaint, call: make_outline then insert_guts
void
POP_UP_WINDOWS::generate (popup * which)
{
if(which==NULL)
printf("innn0\n");

//printf("innn1\n");

//old  SCF_DELETE (which->display);
//maybe memory leak?
if(which->display!=NULL)
delete(which->display);
//printf("innn2\n");

which->display.AttachNew(new csImageMemory(make_outline(which)));
//printf("innn3\n");


  switch (which->type)
    {
    case 1:
//debug/learning box
      generate_textbox (which);
      break;
    case 2:
      generate_insertmesh (which);
      break;
    case 3:
      generate_saveskeleton (which);
      break;
    case 4:
      generate_quit (which);
      break;
    case 5:
      generate_overwrite (which);
      break;
    case 6:
      generate_loadskeleton (which);
      break;
    case 7:
      //some is manually generate, but some is templated
//still need to open on the fly with frame manager
      generate_frame (which);
      break;
    case 9:
      //some is manually generate, but some is templated
      generate_saveanim (which);
      break;
    case 10:
      generate_overwrite_anim (which);
      break;
    case 11:
      generate_anim_controller (which);
      break;
    case 12:
      generate_loadanim (which);
      break;
//    case 13:
//      generate_healthbar (which);
//      break;
//    case 14:
///printf("innn4\n");
//      generate_messagebox (which);
//printf("innn5\n");
//printf("innn5\n");
//      break;


    }


  /* FOR MY OLD GAME, I used this case statement to generate on the fly


     switch (which->type)
     {
     case 0:
     gen_name (which);
     break;
     case 1:
     gen_attrib (which);
     break;
     case 2:
     gen_npc_help (which);
     break;
     case 3:
     gen_item_help (which);
     break;
     case 4:
     gen_wall_help (which);
     break;
     case 5:
     gen_floor_help (which);
     break;
     case 6:
     case 7:
     case 8:
     case 9:
     gen_action (which);
     break;
     case 15:
     gen_inventory(which);
     break;

     }
   */


}



popup *
POP_UP_WINDOWS::perror (char *c1)
{
  int len;
  int x;
  len = strlen (c1);
  if (len < 11)
    len = 11;
  x = len * 12 + 20;
  int y = 50;

//  int a;
  char c2[80];
  popup *p = new popup;
//  popup *temp;
  p->stay = 0;
  p->pages = 1;
  p->type = 8;

  push (p);



  p->x_size = x;
  p->y_size = y;
  p->stay = 0;

  p->x_pos = 60;
  p->y_pos = 60;

  p->bordercolor_r = 160;
  p->bordercolor_g = 20;
  p->bordercolor_b = 20;

  p->wincolor_r = 100;
  p->wincolor_g = 100;
  p->wincolor_b = 100;

  strcpy (p->name.c1, "*Error*");
  ash_text (p->name);
  make_background (p->name, 20, 20, 20);


  SCF_DELETE (p->display);

//old  p->display = make_outline (p)->Clone ();
p->display.AttachNew(new csImageMemory(make_outline(p)));
//e_text e0;

  alloc_guts (p, 1);
//printf("here\n");

  textlineize (p, 0, 230, 10, 10);
  p->G3[0]->x = 14;
  p->G3[0]->y = 20;
  strcpy (c2, c1);
  p->G3[0]->set_text (c2);

//boxize(p,0,10,10,40,180,40);
//p->G[0]->x=10;
//p->G[0]->y=10;

//printf("here4\n");
  insert_guts (p);


//printf("here\n");
  return p;
}




popup *
POP_UP_WINDOWS::open (int type)
{
  popup *p;
  p = open (type, 1);
  return p;
}


//good idea to classify your window types using this:
//Creates a popup, adds it to the POP_UP_WINDOWS, and if show =1, places it on top.
popup *
POP_UP_WINDOWS::open (int type, int show_y_n)
{
  int a;
//  char c1[40], c2[40];
  popup *p = new popup;
//  popup *temp;
  p->stay = 0;
  p->pages = 1;
  p->type = type;

//?DEBUG!


//In the cases, fill out the basic information about the popup you want to draw.
//Colors, text, and if only 1 should open or not

  switch (type)
    {
      //CASE 0: Is just the test popup, you can remove it if you want
    case 0:
      //name
//If single_copy(casen0), only allows 1 window of the type
//if you take this line of code out, you can open a ton of test popups
      //a = single_copy (0);
      a = 0;
      if (a == 1)
	{


//This code is left over, not sure what it did...
//I'm thinking it was like to check if you clicked on something other than a window
//        clicked_on_ground (ch.x, ch.y);
	  //cleanup
	  delete p;
	  return NULL;
	}

//places window on top

      push (p);

      alloc_guts (p, 1);

//adds a null gut
      call_defunct_gut (p, 0);

      p->x_size = 200;
      p->y_size = 200;
      p->stay = 0;

      p->x_pos = 60;
      p->y_pos = 200;

//dk purple border
      p->bordercolor_r = 180;
      p->bordercolor_g = 0;
      p->bordercolor_b = 180;

//light cyan insides for window
      p->wincolor_r = 0;
      p->wincolor_g = 220;
      p->wincolor_b = 220;

      strcpy (p->name.c1, "*TEST*");
//Add color to the text and     250 red, 250 blue = purple
      color_text (p->name, 230, 0, 230);
//Create a generic background for the text
      make_background (p->name);
      break;

    case 1:
      //name
//If single_copy(casen0), only allows 1 window of the type
//if you take this line of code out, you can open a ton of test popups
//a = single_copy (1);
      a = 0;
      if (a == 1)
	{


//This code is left over, not sure what it did...
//I'm thinking it was like to check if you clicked on something other than a window
//        clicked_on_ground (ch.x, ch.y);
	  //cleanup
	  delete p;
	  return NULL;
	}

//places window on top

      push (p);



      p->x_size = 200;
      p->y_size = 200;
      p->stay = 0;

      p->x_pos = 60;
      p->y_pos = 200;

      p->bordercolor_r = 40;
      p->bordercolor_g = 160;
      p->bordercolor_b = 160;

//light cyan insides for window
      p->wincolor_r = 0;
      p->wincolor_g = 220;
      p->wincolor_b = 220;

      strcpy (p->name.c1, "Prompt:");
//Add color to the text and     250 red, 250 blue = purple
      color_text (p->name, 80, 220, 220);
//Create a generic background for the text
      make_background (p->name, 100, 100, 100);
      break;

    case 2:
      //name
//If single_copy(caseN0), only allows 1 window of the type
//if you take this line of code out, you can open a ton of test popups
      a = single_copy (2);
      if (a == 1)
	{
	  delete p;
	  return NULL;
	}


      push (p);



      p->x_size = 220;
      p->y_size = 200;
      p->stay = 0;

      p->x_pos = 60;
      p->y_pos = 200;

      p->bordercolor_r = 40;
      p->bordercolor_g = 160;
      p->bordercolor_b = 160;

      p->wincolor_r = 0;
      p->wincolor_g = 220;
      p->wincolor_b = 220;

      strcpy (p->name.c1, "Insert Mesh");
      color_text (p->name, 100, 254, 254);
      make_background (p->name, 100, 100, 100);
      break;
    case 3:
      //name
//If single_copy(caseN0), only allows 1 window of the type
//if you take this line of code out, you can open a ton of test popups
      a = single_copy (3);
      if (a == 1)
	{
	  delete p;
	  return NULL;
	}


      push (p);



      p->x_size = 240;
      p->y_size = 200;
      p->stay = 0;

      p->x_pos = 60;
      p->y_pos = 200;

      p->bordercolor_r = 40;
      p->bordercolor_g = 160;
      p->bordercolor_b = 160;

      p->wincolor_r = 0;
      p->wincolor_g = 220;
      p->wincolor_b = 220;

      strcpy (p->name.c1, "Save Skeleton");
      color_text (p->name, 100, 254, 254);
      make_background (p->name, 100, 100, 100);
      break;


    case 4:
      //name
//If single_copy(caseN0), only allows 1 window of the type
//if you take this line of code out, you can open a ton of test popups
      a = single_copy (4);
      if (a == 1)
	{
	  delete p;
	  return NULL;
	}


      push (p);



      p->x_size = 200;
      p->y_size = 100;
      p->stay = 0;

      p->x_pos = 60;
      p->y_pos = 200;

      p->bordercolor_r = 40;
      p->bordercolor_g = 160;
      p->bordercolor_b = 160;

      p->wincolor_r = 0;
      p->wincolor_g = 220;
      p->wincolor_b = 220;

      strcpy (p->name.c1, "Exit XYZimation?");
      color_text (p->name, 100, 254, 254);
      make_background (p->name, 100, 100, 100);
      break;
    case 5:

      a = single_copy (5);
      if (a == 1)
	{
	  delete p;
	  return NULL;
	}


      push (p);



      p->x_size = 300;
      p->y_size = 100;
      p->stay = 0;

      p->x_pos = 60;
      p->y_pos = 200;

      p->bordercolor_r = 40;
      p->bordercolor_g = 160;
      p->bordercolor_b = 160;

      p->wincolor_r = 0;
      p->wincolor_g = 220;
      p->wincolor_b = 220;

      strcpy (p->name.c1, "Overwrite Skeleton?");
      color_text (p->name, 100, 254, 254);
      make_background (p->name, 100, 100, 100);
      break;
    case 6:
      //name
//If single_copy(caseN0), only allows 1 window of the type
//if you take this line of code out, you can open a ton of test popups
      a = single_copy (6);
      if (a == 1)
	{
	  delete p;
	  return NULL;
	}


      push (p);



      p->x_size = 240;
      p->y_size = 200;
      p->stay = 0;

      p->x_pos = 60;
      p->y_pos = 200;

      p->bordercolor_r = 40;
      p->bordercolor_g = 160;
      p->bordercolor_b = 160;

      p->wincolor_r = 0;
      p->wincolor_g = 220;
      p->wincolor_b = 220;

      strcpy (p->name.c1, "Load Skeleton");
      color_text (p->name, 100, 254, 254);
      make_background (p->name, 100, 100, 100);
      break;

      //case 7 is FRAME
    case 7:
      p->x_pos = 0;
      p->y_pos = 0;
      popup *p3;
      p3 = does_window_exist (7);
      if (p3)
	{
	  p->x_pos = p3->x_pos + 20;
	  p->y_pos = p3->y_pos + 20;


	}

      push (p);



      p->x_size = 400;
      p->y_size = 300;
      p->stay = 0;


      p->bordercolor_r = 160;
      p->bordercolor_g = 160;
      p->bordercolor_b = 40;

      p->wincolor_r = 100;
      p->wincolor_g = 100;
      p->wincolor_b = 100;

      strcpy (p->name.c1, "Animation Frame");
      color_text (p->name, 100, 254, 254);
      make_background (p->name, 100, 100, 100);
      break;



//CASE 8: error, not used

//case 9: save animation
    case 9:
      a = single_copy (9);
      if (a == 1)
	{
	  delete p;
	  return NULL;
	}


      push (p);



      p->x_size = 240;
      p->y_size = 200;
      p->stay = 0;

      p->x_pos = 60;
      p->y_pos = 200;

      p->bordercolor_r = 40;
      p->bordercolor_g = 160;
      p->bordercolor_b = 160;

      p->wincolor_r = 0;
      p->wincolor_g = 220;
      p->wincolor_b = 220;

      strcpy (p->name.c1, "Save Anim");
      color_text (p->name, 100, 254, 254);
      make_background (p->name, 100, 100, 100);
      break;
    case 10:

      a = single_copy (10);
      if (a == 1)
	{
	  delete p;
	  return NULL;
	}


      push (p);



      p->x_size = 300;
      p->y_size = 100;
      p->stay = 0;

      p->x_pos = 60;
      p->y_pos = 200;

      p->bordercolor_r = 40;
      p->bordercolor_g = 160;
      p->bordercolor_b = 160;

      p->wincolor_r = 0;
      p->wincolor_g = 220;
      p->wincolor_b = 220;

      strcpy (p->name.c1, "Overwrite Anim?");
      color_text (p->name, 100, 254, 254);
      make_background (p->name, 100, 100, 100);
      break;
    case 11:

      a = single_copy (11);
      if (a == 1)
	{
	  delete p;
	  return NULL;
	}


      push (p);



      p->x_size = 300;
      p->y_size = 330;
      p->stay = 0;

      p->x_pos = 60;
      p->y_pos = 150;

      p->bordercolor_r = 180;
      p->bordercolor_g = 130;
      p->bordercolor_b = 130;

      p->wincolor_r = 100;
      p->wincolor_g = 70;
      p->wincolor_b = 70;

      strcpy (p->name.c1, "Anim Controller");
      color_text (p->name, 250, 170, 120);
      make_background (p->name, 100, 100, 100);
      break;

    case 12:
      //name
//If single_copy(caseN0), only allows 1 window of the type
//if you take this line of code out, you can open a ton of test popups
      a = single_copy (12);
      if (a == 1)
	{
	  delete p;
	  return NULL;
	}


      push (p);



      p->x_size = 240;
      p->y_size = 200;
      p->stay = 0;

      p->x_pos = 60;
      p->y_pos = 200;

      p->bordercolor_r = 40;
      p->bordercolor_g = 160;
      p->bordercolor_b = 160;

      p->wincolor_r = 0;
      p->wincolor_g = 220;
      p->wincolor_b = 220;

      strcpy (p->name.c1, "Load Animation");
      color_text (p->name, 100, 254, 254);
      make_background (p->name, 100, 100, 100);
      break;

    case 13:
      //name
//If single_copy(caseN0), only allows 1 window of the type
//if you take this line of code out, you can open a ton of test popups
      a = single_copy (13);
      if (a == 1)
	{
	  delete p;
	  return NULL;
	}

      push (p);

      p->x_size = 80;
      p->y_size = 60;
      p->stay = 4;

      p->x_pos = -14;
      p->y_pos = -18;

      p->bordercolor_r = 0;
      p->bordercolor_g = 0;
      p->bordercolor_b = 0;

      p->wincolor_r = 0;
      p->wincolor_g = 0;
      p->wincolor_b = 0;

      strcpy (p->name.c1, "");
      color_text (p->name, 0, 0, 0);
//      make_background (p->name, 100, 100, 100);
      break;


    case 14:
      //name
//If single_copy(caseN0), only allows 1 window of the type
//if you take this line of code out, you can open a ton of test popups
printf("inn1\n");
      a = single_copy (14);
printf("inn2\n");

      if (a == 1)
	{
printf("inn3\n");
	  delete p;
	  return NULL;
	}
printf("inn4\n");

      push (p);
printf("inn5\n");

      p->x_size = 660;
      p->y_size = 92;
      p->stay = 4;

      p->x_pos = -14;
      p->y_pos = 392;

      p->bordercolor_r = 0;
      p->bordercolor_g =0;
      p->bordercolor_b =0;

      p->wincolor_r =0;
      p->wincolor_g =0;
      p->wincolor_b =0;

      strcpy (p->name.c1, "");
      color_text (p->name, 0, 0, 0);
      make_background (p->name, 0, 0, 0);
printf("inn6\n");

      break;
    }








  if (show_y_n == 1)
    {
//printf("inn7\n");

      generate (p);
    }
//printf("inn8\n");

  return (p);
}
























double drange;

/*
char *check_pref (int type);

char *
check_pref (int type)
{
  switch (type)
    {
    case 1:
      return ("Talk");
    case 2:
      return ("Attack");
    case 3:
      return ("Look");
    case 4:
      return ("Use");
    }

}
*/



popup::popup ()
{
  display = NULL;
  pages = 1;
  curpage = 0;
}

void
popup::rename (char *c1)
{
//printf(c1);
//printf("\n");
  strcpy (name.c1, c1);
  POP_UP_WINDOWS F;

  SCF_DELETE (this->display);

  display = F.make_outline (this);
  F.insert_guts (this);

//  blit (xbut, this->display, 0, 0, 40, 40, 16, 16);



/*

//slap on the x and button :)

  if(which->stay!=4)
  blit (xbut, temp, 0, 0, which->x_size - 10 - 16, 0, 16, 16);


//   next step is allowing the scale color to work with colors not relevant in range
  //should be relatively simple
  if (which->stay == 2)
    {
      t1=grbut->Clone();

alter_colors_ratio(t1,0,120,240,which->bordercolor_r,which->bordercolor_g,which->bordercolor_b,7);

      blit (t1, temp, 0, 0, 10, 0, grbut->GetWidth(), 16);
    }


  if (which->stay == 1)
    {
      t1=grbut->Clone();

alter_colors_ratio(t1,0,120,240,which->bordercolor_r,which->bordercolor_g,which->bordercolor_b,7);

      blit (t1, temp, 0, 0, 10, 0, grbut->GetWidth(), 16);
    }

  if (which->stay == 0)
    {
            t1=rdbut->Clone();

alter_colors_ratio(t1,0,120,240,which->bordercolor_r,which->bordercolor_g,which->bordercolor_b,7);

      blit (t1, temp, 0, 0, 10, 0, rdbut->GetWidth(), 16);
    }

t1=cornul->Clone();
alter_colors(t1,0,127,240,which->bordercolor_r,which->bordercolor_g,which->bordercolor_b);


  blit (t1, temp, 0, 0, 0, 0, 11, 16);
t1=cornur->Clone();
alter_colors(t1,0,127,240,which->bordercolor_r,which->bordercolor_g,which->bordercolor_b);


//slaps text on
  int ww;
  int nl;
  char c1[256];
  ww = which->x_size - 48;
  ww = ww / 12 - 1;
  nl = strlen (which->name.c1);
  if (ww <= nl)
    {
//truncate string nl to length ww.... ARGH.
      for (i = 0; i < ww; i++)
	c1[i] = which->name.c1[i];
      which->name.c1[i] = 0;
//then display to first posisition
//      xyprint (3*12, 0, which->name, temp);
    blit_text_background(temp,which->name,3*12,0);
    }
  else
blit_text_background(temp,which->name,12* (2 + (ww - nl) / 2),0);
*/

}





//
void
POP_UP_WINDOWS::call_defunct_gut (popup * which, int gutn0)
{
static  csRef < iImage > temp;
  BMAP1 *B1;
  //1,1 bitmap, old code created a new bitmap, but you can't
//old  temp = I_nothing->Clone ();
temp.AttachNew(new csImageMemory(I_nothing));
//  temp= hzbar->Clone();
  B1 = new BMAP1 (temp);
///  which->G3[gutn0] = B1;
///  which->G3[gutn0]->page = 0;
///  which->G3[gutn0]->x = 1;
///  which->G3[gutn0]->y = 1;
///  which->G3[gutn0]->x_size = 1;
///  which->G3[gutn0]->y_size = 1;

  which->G3[gutn0]=B1;
  which->G3[gutn0]->page = 0;
  which->G3[gutn0]->x = 1;
  which->G3[gutn0]->y = 1;
  which->G3[gutn0]->x_size = 1;
  which->G3[gutn0]->y_size = 1;



  //This makes sure temp stays around though its gone out of scope
//old  SCF_INC_REF (temp);
}


iImage *
POP_UP_WINDOWS::make_outline (popup * which)
{
  int i, j;

//x border - 10/10
//y border top 16, bottom
static  csRef < iImage > temp;
//OLD  temp = I_nothing->Clone ();
temp.AttachNew(new csImageMemory(I_nothing));
//OLD  SCF_INC_REF (temp);

//  temp = create_bitmap (which->x_size,
//old  temp->Rescale (which->x_size, which->y_size);
csImageManipulate *csim;
csim=new csImageManipulate;
temp=csim->Rescale(temp,which->x_size, which->y_size,1);
  for (i = 0; i < which->x_size; i++)
    for (j = 0; j < which->y_size; j++)
      {
	setred (temp, i, j, 0);
	setgreen (temp, i, j, 0);
	setblue (temp, i, j, 0);
	setalpha (temp, i, j, 255);
      }




//  BITMAP *t1, *t2;
  csRef < iImage > t1, t2;


//put corners in
  int x, y, xd, yd;


//Nice tan 0,127,240->230,127,0
//which->display.AttachNew(new csImageMemory(
  t1.AttachNew(new csImageMemory(hzbar));
  alter_colors (t1, 0, 127, 240, which->bordercolor_r, which->bordercolor_g,
		which->bordercolor_b);
  t2.AttachNew(new csImageMemory(hzbar2));
  alter_colors (t2, 0, 127, 240, which->bordercolor_r, which->bordercolor_g,
		which->bordercolor_b);


  for (x = 10; x < (which->x_size - 50); x = x + 40)
    {
      blit (t1, temp, 0, 0, x, 0, 40, 16);
      blit (t2, temp, 0, 0, x, which->y_size - 10, 40, 16);
    }



  xd = (which->x_size - 10) - x;
  blit (t1, temp, 0, 0, x, 0, xd, 16);
  blit (t2, temp, 0, 0, x, which->y_size - 10, xd, 16);


//  t1 =
//    scale_color (vtbar, c_silver_blue, which->border_color,
//               which->border_bright);

t1.AttachNew(new csImageMemory(vtbar));
//old  t1 = vtbar->Clone ();
  alter_colors (t1, 0, 127, 240, which->bordercolor_r, which->bordercolor_g,
		which->bordercolor_b);

  for (y = 10; y < which->y_size - 50; y = y + 40)
    {
      blit (t1, temp, 0, 0, 0, y, 10, 40);
      blit (t1, temp, 0, 0, which->x_size - 10, y, 10, 40);
    }

  yd = (which->y_size - 10) - y;
  blit (t1, temp, 0, 0, 0, y, 10, yd);
  blit (t1, temp, 0, 0, which->x_size - 10, y, 10, yd);

//slap on the x and button :)

if(which->stay!=4)
  blit (xbut, temp, 0, 0, which->x_size - 10 - 16, 0, 16, 16);


//  /* next step is allowing the scale color to work with colors not relevant in range
  //should be relatively simple
  if (which->stay == 2)
    {
//old      t1 = grbut->Clone ();
t1.AttachNew(new csImageMemory(grbut));
      alter_colors_ratio (t1, 0, 120, 240, which->bordercolor_r,
			  which->bordercolor_g, which->bordercolor_b, 7);

      blit (t1, temp, 0, 0, 10, 0, grbut->GetWidth (), 16);
    }


  if (which->stay == 1)
    {
     //old t1 = grbut->Clone ();
t1.AttachNew(new csImageMemory(grbut));
      alter_colors_ratio (t1, 0, 120, 240, which->bordercolor_r,
			  which->bordercolor_g, which->bordercolor_b, 7);

      blit (t1, temp, 0, 0, 10, 0, grbut->GetWidth (), 16);
    }

  if (which->stay == 0)
    {
t1.AttachNew(new csImageMemory(rdbut));
//      t1 = rdbut->Clone ();

      alter_colors_ratio (t1, 0, 120, 240, which->bordercolor_r,
			  which->bordercolor_g, which->bordercolor_b, 7);

      blit (t1, temp, 0, 0, 10, 0, rdbut->GetWidth (), 16);
    }

//  t1 = cornul->Clone ();
t1.AttachNew(new csImageMemory(cornul));  
alter_colors (t1, 0, 127, 240, which->bordercolor_r, which->bordercolor_g,
		which->bordercolor_b);


  blit (t1, temp, 0, 0, 0, 0, 11, 16);
t1.AttachNew(new csImageMemory(cornur));
//  t1 = cornur->Clone ();
  alter_colors (t1, 0, 127, 240, which->bordercolor_r, which->bordercolor_g,
		which->bordercolor_b);


  blit (t1, temp, 0, 0, which->x_size - 11, 0, 11, 16);
t1.AttachNew(new csImageMemory(cornll));
//  t1 = cornll->Clone ();
  alter_colors (t1, 0, 127, 240, which->bordercolor_r, which->bordercolor_g,
		which->bordercolor_b);


  blit (t1, temp, 0, 0, 0, which->y_size - 10, 10, 10);
  t1.AttachNew(new csImageMemory(cornlr));
//t1 = cornlr->Clone ();
  alter_colors (t1, 0, 127, 240, which->bordercolor_r, which->bordercolor_g,
		which->bordercolor_b);


  blit (t1, temp, 0, 0, which->x_size - 10, which->y_size - 10, 10, 10);

  int wr, wg, wb;

  wr = which->wincolor_r - 100;
  wg = which->wincolor_g - 100;
  wb = which->wincolor_b - 100;
  if (wr < 0)
    wr = 0;
  if (wg < 0)
    wg = 0;
  if (wb < 0)
    wb = 0;

  rect_open (temp, 10, 16, which->x_size - 11, which->y_size - 11,
	     wr, wg, wb);
  wr = which->wincolor_r - 75;
  wg = which->wincolor_g - 75;
  wb = which->wincolor_b - 75;
  if (wr < 0)
    wr = 0;
  if (wg < 0)
    wg = 0;
  if (wb < 0)
    wb = 0;

  rect_open (temp, 11, 17, which->x_size - 12, which->y_size - 12,
	     wr, wg, wb);
  wr = which->wincolor_r - 50;
  wg = which->wincolor_g - 50;
  wb = which->wincolor_b - 50;
  if (wr < 0)
    wr = 0;
  if (wg < 0)
    wg = 0;
  if (wb < 0)
    wb = 0;
  rect_open (temp, 12, 18, which->x_size - 13, which->y_size - 13,
	     wr, wg, wb);
  wr = which->wincolor_r - 25;
  wg = which->wincolor_g - 25;
  wb = which->wincolor_b - 25;
  if (wr < 0)
    wr = 0;
  if (wg < 0)
    wg = 0;
  if (wb < 0)
    wb = 0;
  rect_open (temp, 13, 19, which->x_size - 14, which->y_size - 14,
	     wr, wg, wb);
  rect (temp, 14, 20, which->x_size - 15, which->y_size - 15,
	which->wincolor_r, which->wincolor_g, which->wincolor_b);



//color_text(which->name,which->bordercolor_r+55,which->bordercolor_g+55,which->bordercolor_b+55);
//make_background(which->name);


  int ww;
  int nl;
  char c1[256];
  ww = which->x_size - 48;
  ww = ww / 12 - 1;
  nl = strlen (which->name.c1);
  if (ww <= nl)
    {
//truncate string nl to length ww.... ARGH.
      for (i = 0; i < ww; i++)
	c1[i] = which->name.c1[i];
      which->name.c1[i] = 0;
//then display to first posisition
//      xyprint (3*12, 0, which->name, temp);
      blit_text_background (temp, which->name, 3 * 12, 0);
    }
  else
    blit_text_background (temp, which->name, 12 * (2 + (ww - nl) / 2), 0);


//  blit (xbut, temp, 0, 0, 40, 40, 16, 16);

  return (iImage *) temp;
//show guts?
}




//You gotta alloc guts
//Paint up the window
//then insert the guts at the end
void
POP_UP_WINDOWS::generate_textbox (popup * which)
{
//  blit (xbut, which->display, 0, 0, 40, 40, 16, 16);
//printf("here\n");
  e_text e0;
//BMAP3 *TEMP;

  alloc_guts (which, 6);

  boxize (which, 0, 80, 40, 40, 180, 40);
///  which->G3[0]->x = 20;
///  which->G3[0]->y = 145;
  which->G3[0]->x = 20;
  which->G3[0]->y = 145;



//TEMP=(BMAP3 *)which->G3[0];
  green_text (e0);
  strcpy (e0.c1, "OK");
///  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 24, 12);
///  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 24, 12);
///  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 24, 12);


///which->

  boxize (which, 1, 80, 40, 180, 40, 40);
///  which->G3[1]->x = 100;
///  which->G3[1]->y = 145;
  which->G3[1]->x = 100;
  which->G3[1]->y = 145;

  red_text (e0);
  strcpy (e0.c1, "Cancel");
///  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 6, 12);
///  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 6, 12);
///  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 6, 12);


  textboxize (which, 2, 180, 40, 40);
///  which->G3[2]->x = 14;
///  which->G3[2]->y = 120;
  which->G3[2]->x = 14;
  which->G3[2]->y = 120;


//which->G3[2]->set_text("this is box2");

  textlineize (which, 3, 20, 50, 220);
///  which->G3[3]->x = 14;
///  which->G3[3]->y = 30;
  which->G3[3]->x = 14;
  which->G3[3]->y = 30;

  which->G3[3]->set_text ("Just a generic");

  textlineize (which, 4, 20, 50, 220);
  which->G3[4]->x = 14;
  which->G3[4]->y = 44;
  which->G3[4]->set_text ("window for debug");

  textlineize (which, 5, 20, 50, 220);
  which->G3[5]->x = 14;
  which->G3[5]->y = 58;
  which->G3[5]->set_text ("and learning.");



  insert_guts (which);
}


void
POP_UP_WINDOWS::generate_insertmesh (popup * which)
{
//  blit (xbut, which->display, 0, 0, 40, 40, 16, 16);
  e_text e0;
//printf("here\n");

  alloc_guts (which, 5);

  boxize (which, 0, 80, 40, 40, 180, 40);
  which->G3[0]->x = 20;
  which->G3[0]->y = 145;

  green_text (e0);
  strcpy (e0.c1, "OK");
  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 24, 12);



  boxize (which, 1, 80, 40, 180, 40, 40);
  which->G3[1]->x = 100;
  which->G3[1]->y = 145;



  red_text (e0);
  strcpy (e0.c1, "Cancel");
  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 6, 12);


  textboxize (which, 2, 180, 40, 40);
  which->G3[2]->x = 14;
  which->G3[2]->y = 120;

//which->G3[2]->set_text("this is box2");

  textlineize (which, 3, 50, 210, 250);
  which->G3[3]->x = 14;
  which->G3[3]->y = 30;

  which->G3[3]->set_text ("Load in a mesh.");

  textlineize (which, 4, 50, 210, 250);
  which->G3[4]->x = 14;
  which->G3[4]->y = 46;
  which->G3[4]->set_text ("Looks in meshfacts");

//textlineize(which,5,50,210,250);
//which->G3[5]->x=14;
//which->G3[5]->y=62;
//which->G3[5]->set_text("and my_meshfacts");

//finally give focus to the textbox for quick text entry
  focus = which;
  focus_gut = 2;

  insert_guts (which);
}

void
POP_UP_WINDOWS::generate_saveskeleton (popup * which)
{
  e_text e0;
//  blit (xbut, which->display, 0, 0, 40, 40, 16, 16);

//printf("here\n");

  alloc_guts (which, 6);

  boxize (which, 0, 80, 40, 40, 180, 40);
  which->G3[0]->x = 20;
  which->G3[0]->y = 145;

  green_text (e0);
  strcpy (e0.c1, "OK");
  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 24, 12);



  boxize (which, 1, 80, 40, 180, 40, 40);
  which->G3[1]->x = 100;
  which->G3[1]->y = 145;


  red_text (e0);
  strcpy (e0.c1, "Cancel");
  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 6, 12);


//leave some text in for quick entry.... later
  textboxize (which, 2, 180, 40, 40);
  which->G3[2]->x = 14;
  which->G3[2]->y = 48;

  textboxize (which, 3, 180, 40, 40);
  which->G3[3]->x = 14;
  which->G3[3]->y = 84;



  textlineize (which, 4, 50, 210, 250);
  which->G3[4]->x = 14;
  which->G3[4]->y = 30;
  which->G3[4]->set_text ("Skeleton is directory.");

  textlineize (which, 5, 50, 210, 250);
  which->G3[5]->x = 14;
  which->G3[5]->y = 66;
  which->G3[5]->set_text ("Pose is file.");

  focus = which;
  focus_gut = 2;

  insert_guts (which);
}



void
POP_UP_WINDOWS::generate_loadskeleton (popup * which)
{
  e_text e0;
//  blit (xbut, which->display, 0, 0, 40, 40, 16, 16);

//printf("here\n");

  alloc_guts (which, 6);

  boxize (which, 0, 80, 40, 40, 180, 40);
  which->G3[0]->x = 20;
  which->G3[0]->y = 145;

  green_text (e0);
  strcpy (e0.c1, "OK");
  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 24, 12);



  boxize (which, 1, 80, 40, 180, 40, 40);
  which->G3[1]->x = 100;
  which->G3[1]->y = 145;


  red_text (e0);
  strcpy (e0.c1, "Cancel");
  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 6, 12);


//leave some text in for quick entry.... later
  textboxize (which, 2, 180, 40, 40);
  which->G3[2]->x = 14;
  which->G3[2]->y = 48;

  textboxize (which, 3, 180, 40, 40);
  which->G3[3]->x = 14;
  which->G3[3]->y = 84;



  textlineize (which, 4, 50, 210, 250);
  which->G3[4]->x = 14;
  which->G3[4]->y = 30;
  which->G3[4]->set_text ("Skeleton is directory.");

  textlineize (which, 5, 50, 210, 250);
  which->G3[5]->x = 14;
  which->G3[5]->y = 66;
  which->G3[5]->set_text ("Pose is file.");

  focus = which;
  focus_gut = 2;

  insert_guts (which);
}

void
POP_UP_WINDOWS::generate_quit (popup * which)
{
  e_text e0;


  alloc_guts (which, 2);

  boxize (which, 0, 80, 40, 40, 180, 40);
  which->G3[0]->x = 20;
  which->G3[0]->y = 45;

  green_text (e0);
  strcpy (e0.c1, "OK-Exit");

//Someone says to: static_cast <BMAP3 *>
//or               dynamic_cast <BMAP3 *>
  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 6, 12);


  boxize (which, 1, 80, 40, 180, 40, 40);
  which->G3[1]->x = 100;
  which->G3[1]->y = 45;


  red_text (e0);
  strcpy (e0.c1, "Cancel");
  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 6, 12);



  insert_guts (which);
}



void
POP_UP_WINDOWS::generate_overwrite (popup * which)
{
  e_text e0;


  alloc_guts (which, 4);

  boxize (which, 0, 120, 40, 40, 180, 40);
  which->G3[0]->x = 20;
  which->G3[0]->y = 45;

  green_text (e0);
  strcpy (e0.c1, "OverWrite");

//Someone says to: static_cast <BMAP3 *>
//or               dynamic_cast <BMAP3 *>
  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 6, 12);


  boxize (which, 1, 120, 40, 180, 40, 40);
  which->G3[1]->x = 145;
  which->G3[1]->y = 45;


  red_text (e0);
  strcpy (e0.c1, "Cancel");
  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 26, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 26, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 26, 12);


///Could of forced text into above guts, but for readability and learning.
//I create 2 new guts for e_text storage
  call_defunct_gut (which, 2);
  call_defunct_gut (which, 3);
  insert_guts (which);

}


void
POP_UP_WINDOWS::generate_overwrite_anim (popup * which)
{
  e_text e0;


  alloc_guts (which, 4);

  boxize (which, 0, 120, 40, 40, 180, 40);
  which->G3[0]->x = 20;
  which->G3[0]->y = 45;

  green_text (e0);
  strcpy (e0.c1, "OverWrite");

//Someone says to: static_cast <BMAP3 *>
//or               dynamic_cast <BMAP3 *>
  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 6, 12);


  boxize (which, 1, 120, 40, 180, 40, 40);
  which->G3[1]->x = 145;
  which->G3[1]->y = 45;


  red_text (e0);
  strcpy (e0.c1, "Cancel");
  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 26, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 26, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 26, 12);


///Could of forced text into above guts, but for readability and learning.
//I create 2 new guts for e_text storage
  call_defunct_gut (which, 2);
  call_defunct_gut (which, 3);
  insert_guts (which);

}


void
POP_UP_WINDOWS::generate_frame (popup * which)
{
  e_text e0;
//  blit (xbut, which->display, 0, 0, 40, 40, 16, 16);

//printf("here\n");

  alloc_guts (which, 10);

  boxize (which, 0, 90, 40, 20, 20, 180);
  which->G3[0]->x = 20;
  which->G3[0]->y = 245;

  cyan_text (e0);
  strcpy (e0.c1, "Show");
  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 6, 6);
  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 6, 6);
  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 6, 6);
  strcpy (e0.c1, "Frame");
  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 6, 22);
  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 6, 22);
  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 6, 22);



  boxize (which, 1, 90, 40, 40, 200, 40);
  which->G3[1]->x = 110;
  which->G3[1]->y = 245;

  green_text (e0);
  strcpy (e0.c1, "Play");
  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 6, 6);
  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 6, 6);
  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 6, 6);
  strcpy (e0.c1, "ALL");
  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 6, 22);
  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 6, 22);
  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 6, 22);



  boxize (which, 6, 90, 40, 40, 200, 40);
  which->G3[6]->x = 200;
  which->G3[6]->y = 245;

  green_text (e0);
  strcpy (e0.c1, "Play");
  blit_text (((BMAP3 *) which->G3[6])->b[0], e0, 6, 6);
  blit_text (((BMAP3 *) which->G3[6])->b[1], e0, 6, 6);
  blit_text (((BMAP3 *) which->G3[6])->b[2], e0, 6, 6);
  strcpy (e0.c1, "Frame");
  blit_text (((BMAP3 *) which->G3[6])->b[0], e0, 6, 22);
  blit_text (((BMAP3 *) which->G3[6])->b[1], e0, 6, 22);
  blit_text (((BMAP3 *) which->G3[6])->b[2], e0, 6, 22);



  boxize (which, 7, 90, 40, 40, 200, 40);
  which->G3[7]->x = 290;
  which->G3[7]->y = 245;

  green_text (e0);
  strcpy (e0.c1, "Play");
  blit_text (((BMAP3 *) which->G3[7])->b[0], e0, 6, 6);
  blit_text (((BMAP3 *) which->G3[7])->b[1], e0, 6, 6);
  blit_text (((BMAP3 *) which->G3[7])->b[2], e0, 6, 6);
  strcpy (e0.c1, "After");
  blit_text (((BMAP3 *) which->G3[7])->b[0], e0, 6, 22);
  blit_text (((BMAP3 *) which->G3[7])->b[1], e0, 6, 22);
  blit_text (((BMAP3 *) which->G3[7])->b[2], e0, 6, 22);



  boxize (which, 8, 50, 100, 200, 20, 20);
  which->G3[8]->x = 18;
  which->G3[8]->y = 18;

  ash_text (e0);
  strcpy (e0.c1, "D");
  blit_text (((BMAP3 *) which->G3[8])->b[0], e0, 19, 4);
  blit_text (((BMAP3 *) which->G3[8])->b[1], e0, 19, 4);
  blit_text (((BMAP3 *) which->G3[8])->b[2], e0, 19, 4);
  strcpy (e0.c1, "E");
  blit_text (((BMAP3 *) which->G3[8])->b[0], e0, 19, 19);
  blit_text (((BMAP3 *) which->G3[8])->b[1], e0, 19, 19);
  blit_text (((BMAP3 *) which->G3[8])->b[2], e0, 19, 19);
  strcpy (e0.c1, "L");
  blit_text (((BMAP3 *) which->G3[8])->b[0], e0, 19, 34);
  blit_text (((BMAP3 *) which->G3[8])->b[1], e0, 19, 34);
  blit_text (((BMAP3 *) which->G3[8])->b[2], e0, 19, 34);
  strcpy (e0.c1, "E");
  blit_text (((BMAP3 *) which->G3[8])->b[0], e0, 19, 49);
  blit_text (((BMAP3 *) which->G3[8])->b[1], e0, 19, 49);
  blit_text (((BMAP3 *) which->G3[8])->b[2], e0, 19, 49);
  strcpy (e0.c1, "T");
  blit_text (((BMAP3 *) which->G3[8])->b[0], e0, 19, 64);
  blit_text (((BMAP3 *) which->G3[8])->b[1], e0, 19, 64);
  blit_text (((BMAP3 *) which->G3[8])->b[2], e0, 19, 64);
  strcpy (e0.c1, "E");
  blit_text (((BMAP3 *) which->G3[8])->b[0], e0, 19, 79);
  blit_text (((BMAP3 *) which->G3[8])->b[1], e0, 19, 79);
  blit_text (((BMAP3 *) which->G3[8])->b[2], e0, 19, 79);







//leave some text in for quick entry.... later
  textboxize (which, 2, 180, 40, 40);
  which->G3[2]->x = 324;
  which->G3[2]->y = 30;

  textboxize (which, 3, 180, 40, 40);
  which->G3[3]->x = 324;
  which->G3[3]->y = 50;



  textlineize (which, 4, 240, 0, 70);
  which->G3[4]->x = 186;
  which->G3[4]->y = 30;
  which->G3[4]->set_text ("Frame Number:");

  textlineize (which, 5, 240, 0, 70);
  which->G3[5]->x = 124;
  which->G3[5]->y = 50;
  which->G3[5]->set_text ("Transition delay(ms):");


  boxize (which, 9, 80, 22, 0, 80, 200);
  which->G3[9]->x = 300;
  which->G3[9]->y = 70;

  blue_text (e0);
  strcpy (e0.c1, "Confirm");
  blit_text (((BMAP3 *) which->G3[9])->b[0], e0, 6, 5);
  blit_text (((BMAP3 *) which->G3[9])->b[1], e0, 6, 5);
  blit_text (((BMAP3 *) which->G3[9])->b[2], e0, 6, 5);

//focus=which;
//focus_gut=3;

  insert_guts (which);
}



void
POP_UP_WINDOWS::generate_anim_controller (popup * which)
{
  e_text e0;
//  blit (xbut, which->display, 0, 0, 40, 40, 16, 16);

//printf("here\n");

  alloc_guts (which, 7);




  textlineize (which, 0, 240, 0, 70);
  which->G3[0]->x = 12;
  which->G3[0]->y = 30;
  which->G3[0]->set_text ("");

  textlineize (which, 1, 240, 0, 70);
  which->G3[1]->x = 12;
  which->G3[1]->y = 50;
  which->G3[1]->set_text ("To capture a Cel(Frame):");


  boxize (which, 2, 250, 40, 120, 60, 180);
  which->G3[2]->x = 20;
  which->G3[2]->y = 70;

  ash_text (e0);
  strcpy (e0.c1, "Take Snapshot");
  blit_text (((BMAP3 *) which->G3[2])->b[0], e0, 6, 6);
  blit_text (((BMAP3 *) which->G3[2])->b[1], e0, 6, 6);
  blit_text (((BMAP3 *) which->G3[2])->b[2], e0, 6, 6);
  strcpy (e0.c1, "Frame/Cel capture");
  blit_text (((BMAP3 *) which->G3[2])->b[0], e0, 6, 22);
  blit_text (((BMAP3 *) which->G3[2])->b[1], e0, 6, 22);
  blit_text (((BMAP3 *) which->G3[2])->b[2], e0, 6, 22);



  boxize (which, 3, 250, 40, 20, 20, 180);
  which->G3[3]->x = 20;
  which->G3[3]->y = 120;

  ash_text (e0);
  strcpy (e0.c1, "Show all Frames");
  blit_text (((BMAP3 *) which->G3[3])->b[0], e0, 6, 14);
  blit_text (((BMAP3 *) which->G3[3])->b[1], e0, 6, 14);
  blit_text (((BMAP3 *) which->G3[3])->b[2], e0, 6, 14);
//strcpy(e0.c1,"Frame");
//blit_text(((BMAP3 *)which->G3[3])->b[0],e0,6,22);
//blit_text(((BMAP3 *)which->G3[3])->b[1],e0,6,22);
//blit_text(((BMAP3 *)which->G3[3])->b[2],e0,6,22);


  boxize (which, 4, 250, 40, 20, 180, 50);
  which->G3[4]->x = 20;
  which->G3[4]->y = 170;

  ash_text (e0);
  strcpy (e0.c1, "Play the Animation");
  blit_text (((BMAP3 *) which->G3[4])->b[0], e0, 6, 14);
  blit_text (((BMAP3 *) which->G3[4])->b[1], e0, 6, 14);
  blit_text (((BMAP3 *) which->G3[4])->b[2], e0, 6, 14);



  boxize (which, 5, 250, 40, 100, 100, 100);
  which->G3[5]->x = 20;
  which->G3[5]->y = 220;

  color_text (e0, 200, 200, 10);
  strcpy (e0.c1, "Save the Animation");
  blit_text (((BMAP3 *) which->G3[5])->b[0], e0, 6, 14);
  blit_text (((BMAP3 *) which->G3[5])->b[1], e0, 6, 14);
  blit_text (((BMAP3 *) which->G3[5])->b[2], e0, 6, 14);

  boxize (which, 6, 250, 40, 100, 100, 100);
  which->G3[6]->x = 20;
  which->G3[6]->y = 270;

  color_text (e0, 200, 20, 10);
  strcpy (e0.c1, "Load animation");
  blit_text (((BMAP3 *) which->G3[6])->b[0], e0, 6, 6);
  blit_text (((BMAP3 *) which->G3[6])->b[1], e0, 6, 6);
  blit_text (((BMAP3 *) which->G3[6])->b[2], e0, 6, 6);
  strcpy (e0.c1, "& lose current");
  blit_text (((BMAP3 *) which->G3[6])->b[0], e0, 6, 22);
  blit_text (((BMAP3 *) which->G3[6])->b[1], e0, 6, 22);
  blit_text (((BMAP3 *) which->G3[6])->b[2], e0, 6, 22);



//strcpy(e0.c1,"Frame");
//blit_text(((BMAP3 *)which->G3[4])->b[0],e0,6,22);
//blit_text(((BMAP3 *)which->G3[4])->b[1],e0,6,22);
//blit_text(((BMAP3 *)which->G3[4])->b[2],e0,6,22);

//focus=which;
//focus_gut=3;

  insert_guts (which);
}

void
POP_UP_WINDOWS::generate_saveanim (popup * which)
{
  e_text e0;
//  blit (xbut, which->display, 0, 0, 40, 40, 16, 16);

//printf("here\n");

  alloc_guts (which, 6);

  boxize (which, 0, 80, 40, 40, 180, 40);
  which->G3[0]->x = 20;
  which->G3[0]->y = 145;

  green_text (e0);
  strcpy (e0.c1, "OK");
  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 24, 12);



  boxize (which, 1, 80, 40, 180, 40, 40);
  which->G3[1]->x = 100;
  which->G3[1]->y = 145;


  red_text (e0);
  strcpy (e0.c1, "Cancel");
  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 6, 12);


//leave some text in for quick entry.... later
  textboxize (which, 2, 180, 40, 40);
  which->G3[2]->x = 14;
  which->G3[2]->y = 48;

  textboxize (which, 3, 180, 40, 40);
  which->G3[3]->x = 14;
  which->G3[3]->y = 84;



  textlineize (which, 4, 50, 210, 250);
  which->G3[4]->x = 14;
  which->G3[4]->y = 30;
  which->G3[4]->set_text ("animations/DIRECTORY");

  textlineize (which, 5, 50, 210, 250);
  which->G3[5]->x = 14;
  which->G3[5]->y = 66;
  which->G3[5]->set_text ("Filename:");

  focus = which;
  focus_gut = 2;

  insert_guts (which);
}

void
POP_UP_WINDOWS::generate_loadanim (popup * which)
{
  e_text e0;
//  blit (xbut, which->display, 0, 0, 40, 40, 16, 16);

//printf("here\n");

  alloc_guts (which, 6);

  boxize (which, 0, 80, 40, 40, 180, 40);
  which->G3[0]->x = 20;
  which->G3[0]->y = 145;

  green_text (e0);
  strcpy (e0.c1, "OK");
  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 24, 12);
  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 24, 12);



  boxize (which, 1, 80, 40, 180, 40, 40);
  which->G3[1]->x = 100;
  which->G3[1]->y = 145;


  red_text (e0);
  strcpy (e0.c1, "Cancel");
  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 6, 12);


//leave some text in for quick entry.... later
  textboxize (which, 2, 180, 40, 40);
  which->G3[2]->x = 14;
  which->G3[2]->y = 48;

  textboxize (which, 3, 180, 40, 40);
  which->G3[3]->x = 14;
  which->G3[3]->y = 84;



  textlineize (which, 4, 50, 210, 250);
  which->G3[4]->x = 14;
  which->G3[4]->y = 30;
  which->G3[4]->set_text ("/animations/Dir:");

  textlineize (which, 5, 50, 210, 250);
  which->G3[5]->x = 14;
  which->G3[5]->y = 66;
  which->G3[5]->set_text ("Filename:");

  focus = which;
  focus_gut = 2;

  insert_guts (which);
}

/*
void
POP_UP_WINDOWS::generate_healthbar (popup * which)
{
  e_text e0;
//  blit (xbut, which->display, 0, 0, 40, 40, 16, 16);

//printf("here\n");

  alloc_guts (which, 2);

  /*
  boxize (which, 0, 80, 40, 40, 180, 40);
  which->G3[0]->x = 20;
  which->G3[0]->y = 145;

  green_text (e0);
  strcpy (e0.c1, "OK");
  blit_text (((BMAP3 *) which->G3[0])->b[0], e0, 24, 16);
  blit_text (((BMAP3 *) which->G3[0])->b[1], e0, 24, 16);
  blit_text (((BMAP3 *) which->G3[0])->b[2], e0, 24, 16);



  boxize (which, 1, 80, 40, 180, 40, 40);
  which->G3[1]->x = 100;
  which->G3[1]->y = 145;


  red_text (e0);
  strcpy (e0.c1, "Cancel");
  blit_text (((BMAP3 *) which->G3[1])->b[0], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[1], e0, 6, 12);
  blit_text (((BMAP3 *) which->G3[1])->b[2], e0, 6, 12);
//-

//leave some text in for quick entry.... later
/*  textboxize (which, 2, 180, 40, 40);
  which->G3[2]->x = 14;
  which->G3[2]->y = 48;

  textboxize (which, 3, 180, 40, 40);
  which->G3[3]->x = 14;
  which->G3[3]->y = 84;
//-


  textlineize (which, 0, 250, 0, 0);
  which->G3[0]->x = 14;
  which->G3[0]->y = 20;
  char c1[100];

//HERE
sprintf (c1, "%d", Sys->P->health);
//TEMP
//strcpy(c1,"100");
  which->G3[0]->set_text (c1);

  textlineize (which, 1, 0, 0, 250);
  which->G3[1]->x = 14;
  which->G3[1]->y = 40;
  sprintf (c1, "%d", Sys->P->stamina);
//TEMP
//strcpy(c1,"100");
  which->G3[1]->set_text (c1);

//  which->G3[1]->set_text ("220");

//  focus = which;
//  focus_gut = 2;

  insert_guts (which);
}



void
POP_UP_WINDOWS::generate_messagebox (popup * which)
{
  e_text e0;
//  blit (xbut, which->display, 0, 0, 40, 40, 16, 16);

printf("innnn1\n");

  alloc_guts (which, 6);
printf("innnn2\n");

  char c1[40];






  if(msgbox_type[0]==1)
  textlineize (which, 0, 250, 0, 0);
  if(msgbox_type[0]==2)
  textlineize (which, 0, 0, 250, 0);
  if(msgbox_type[0]==3)
  textlineize (which, 0, 250, 250,0);
  if(msgbox_type[0]==4)
  textlineize (which, 0, 0, 250,250);

strcpy(c1,msgbox0);
printf("innnn3\n");

  which->G3[0]->x = 14;
  which->G3[0]->y = 0;


// sprintf (c1, "%d", Sys->P->health);
  which->G3[0]->set_text (c1);

  if(msgbox_type[1]==1)
  textlineize (which, 1, 250, 0, 0);
  if(msgbox_type[1]==2)
  textlineize (which,1, 0, 250, 0);
  if(msgbox_type[1]==3)
  textlineize (which, 1, 250, 250,0);
  if(msgbox_type[1]==4)
  textlineize (which, 1, 0, 250,250);
strcpy(c1,msgbox1);
  which->G3[1]->x = 14;
  which->G3[1]->y = 15;
 //  sprintf (c1, "%d", Sys->P->stamina);
  which->G3[1]->set_text (c1);

printf("innnn4\n");

  if(msgbox_type[2]==1)
  textlineize (which, 2, 250, 0, 0);
  if(msgbox_type[2]==2)
  textlineize (which, 2, 0, 250, 0);
  if(msgbox_type[2]==3)
  textlineize (which, 2, 250, 250,0);
  if(msgbox_type[2]==4)
  textlineize (which, 2, 0, 250,250);
strcpy(c1,msgbox2);
  which->G3[2]->x = 14;
  which->G3[2]->y = 30;
//strcpy(c1,"100");
  which->G3[2]->set_text (c1);


  if(msgbox_type[3]==1)
  textlineize (which, 3, 250, 0, 0);
  if(msgbox_type[3]==2)
  textlineize (which, 3, 0, 250, 0);
  if(msgbox_type[3]==3)
  textlineize (which, 3, 250, 250,0);
  if(msgbox_type[3]==4)
  textlineize (which, 3, 0, 250,250);
strcpy(c1,msgbox3);
  which->G3[3]->x = 14;
  which->G3[3]->y = 45;
//strcpy(c1,"100");
  which->G3[3]->set_text (c1);

printf("innnn5\n");

  if(msgbox_type[4]==1)
  textlineize (which, 4, 250, 0, 0);
  if(msgbox_type[4]==2)
  textlineize (which, 4, 0, 250, 0);
  if(msgbox_type[4]==3)
  textlineize (which, 4, 250, 250,0);
  if(msgbox_type[4]==4)
  textlineize (which, 4, 0, 250,250);
strcpy(c1,msgbox4);
  which->G3[4]->x = 14;
  which->G3[4]->y = 60;
//strcpy(c1,"100");
  which->G3[4]->set_text (c1);


  if(msgbox_type[5]==1)
  textlineize (which, 5, 250, 0, 0);
  if(msgbox_type[5]==2)
  textlineize (which, 5, 0, 250, 0);
  if(msgbox_type[5]==3)
  textlineize (which, 5, 250, 250,0);
  if(msgbox_type[5]==4)
  textlineize (which, 5, 0, 250,250);
strcpy(c1,msgbox5);
  which->G3[5]->x = 14;
  which->G3[5]->y = 75;
//strcpy(c1,"100");

  which->G3[5]->set_text (c1);
//printf(c1);
printf(" innnn6\n");


  insert_guts (which);
printf("innnn7\n");

printf("innnn8\n");

}
*/

/*  HERE ARE  EXAMPLES:
 //1) HOW I HANDLED AN INVENTORY POPUP CREATION:


void
POP_UP_WINDOWS::generate_inventory (popup * which)
{
  int i;
  char *c1;
  d_text *d;
  int ppos;
  int ppi;
  int ppt;
  alloc_guts (which, 30);
  BMAP3 *B;


  destroy_bitmap (which->display);

//ppi=which->pos;
// Possibilities first!
//basically it will determine possibility
//then do stuff based if it can or can not be equipped.
////See if it can be taken at all





//      ch.inv[loc].itm = it;
//      ch.inv[loc].cfg = temp->mdata[(1+jk+temp->pos) * 2];

//function for all o em.

//10 X, 16 y                                           //x   y
call_function_to_make_inventory_selections(which, 2 , 12  ,18);
call_function_to_make_inventory_selections(which, 19, 56  ,18) ;
call_function_to_make_inventory_selections(which, 3 , 286  ,18) ;
call_function_to_make_inventory_selections(which, 25, 328  ,18)  ;

call_function_to_make_inventory_selections(which, 4 , 12  ,68)    ;
call_function_to_make_inventory_selections(which,27, 56  ,68)      ;
call_function_to_make_inventory_selections(which,26, 286  ,68)       ;
call_function_to_make_inventory_selections(which, 24, 328  ,68)     ;

//call_function_to_make_inventory_selections(which, 10, 12  ,118)
call_function_to_make_inventory_selections(which, 6, 56  ,118);
call_function_to_make_inventory_selections(which, 5 , 286  ,118);
//call_function_to_make_inventory_selections(which, 9, 328  ,118)

call_function_to_make_inventory_selections(which,10 , 12  ,168)  ;
call_function_to_make_inventory_selections(which, 8 , 56  ,168)   ;
call_function_to_make_inventory_selections(which, 7 , 286  ,168)   ;
call_function_to_make_inventory_selections(which, 9 , 328  ,168)    ;

call_function_to_make_inventory_selections(which, 12, 12  ,218)      ;
call_function_to_make_inventory_selections(which, 14, 56  ,218)       ;
call_function_to_make_inventory_selections(which, 13, 286  ,218)       ;
call_function_to_make_inventory_selections(which, 11, 328  ,218)        ;

call_function_to_make_inventory_selections(which, 15, 12  ,268);
call_function_to_make_inventory_selections(which, 30, 56  ,268) ;
call_function_to_make_inventory_selections(which, 31, 286  ,268) ;
call_function_to_make_inventory_selections(which, 16, 328  ,268)  ;

call_function_to_make_inventory_selections(which, 20, 12  ,318)    ;
call_function_to_make_inventory_selections(which, 22, 56  ,318)     ;
call_function_to_make_inventory_selections(which, 23, 286  ,318)     ;
call_function_to_make_inventory_selections(which, 21, 328  ,318)      ;

call_function_to_make_inventory_selections(which, 28, 12  ,368);
call_function_to_make_inventory_selections(which, 18, 56  ,368) ;
call_function_to_make_inventory_selections(which, 17, 286  ,368) ;
call_function_to_make_inventory_selections(which, 29, 328  ,368)  ;


  for (i = 0; i < which->G3uts; i++)
    which->G3[i]->page = which->curpage;
  insert_guts (which);

}



void POP_UP_WINDOWS::call_function_to_make_inventory_selections
(popup *which, int pos, int x,int y)
{
BMAP3 *B;
//pos represents both gutn0(-2), and the item

//in this is stored the information of the item had.
//if no item, null
//      ch.inv[loc].itm = it;
//in this is stored the style of equip. I think:
// 1 is attack
// 2 is armor
// 3 is both armor and weapon
// 0 itm
// I KNOW
// -1 means its not there
//      ch.inv[loc].cfg = temp->mdata[(1+jk+temp->pos) * 2];

//if position exists: make the black block
//Otherwise make a defunct gut
if(ch.inv[pos].cfg == -1)
{
call_defunct_gut(which,pos-2);
return;
}


      //then
//if item exists, plaster it in.
//if item does not exist, shade in another item


if(ch.inv[pos].itm == NULL)
{
      B = boxize (which, pos-2, 40, 48, c_grey, -5);
      which->G3[pos-2]->x = x;
      which->G3[pos-2]->y = y;
      return;
}

//If you're just holding an item, the box is grey
if(ch.inv[pos].cfg == 0)
{
      B = boxize (which, pos-2, 40, 48, c_grey, 0);
      which->G3[pos-2]->x = x;
      which->G3[pos-2]->y = y;
}

//Red boxes signify attacking weapon
if(ch.inv[pos].cfg == 1)
{
      B = boxize (which, pos-2, 40, 48, c_red, 0);
      which->G3[pos-2]->x = x;
      which->G3[pos-2]->y = y;
}
//Green boxes signify it helps as armor
if(ch.inv[pos].cfg == 2)
{
      B = boxize (which, pos-2, 40, 48, c_green, 0);
      which->G3[pos-2]->x = x;
      which->G3[pos-2]->y = y;
}
//Blue boxes means its both armor and weapon
if(ch.inv[pos].cfg == 3)
{
      B = boxize (which, pos-2, 40, 48, c_blue, 0);
      which->G3[pos-2]->x = x;
      which->G3[pos-2]->y = y;
}
//slap sprite on it
//    B->b[i] = create_bitmap (x, y);
draw_sprite (B->b[0], ch.inv[pos].itm->get_graphic (), 0,0);
draw_sprite (B->b[1], ch.inv[pos].itm->get_graphic (), 0,0);
draw_sprite (B->b[2], ch.inv[pos].itm->get_graphic (), 0,0);

//it->get_graphic ();



//ok so item exists, get its gfx


}


////////////////////#2) How I handled some item stuff
int
POP_UP_WINDOWS::item_thing1(int ppi,popup * which,int gi)
{
int ppos=-3;
  BMAP3 *B;
  d_text *d;
int i;
//3-Not doable because not lek or rat ( negative 3)
//so skip til get one.
while(ppos == -3)
{
ppos=get_possibility_equip_location(which,ppi);
ppi++;
}

//past max so make a non thing
if(ppos == -1)
{
//call defunct gut
//passes arguments: popup *, and gut number.
call_defunct_gut(which, gi);

return -1;

      }
//Something in slot so make darktext
if(ppos == -2)
{
  //bright or dark?
  B = boxize (which, gi, 240, 50, c_grey, -6);

  //Means that the equipment slot is taken
  which->G3[gi]->stor1 = -ppi ;
  which->G3[gi]->x = 10;
  which->G3[gi]->y = 16 +50*gi;

  d =
    get_equip_location (which->mdata[(ppi + which->pos) * 2 - 1], 38,
			which->mdata[(ppi + which->pos) * 2]+4);
  col_to_box (B, d, 2);
      return ppi;

}
  B = boxize (which, gi, 240, 50, c_blue);

//
  which->G3[gi]->stor1 = ppi  ;

  which->G3[gi]->x = 10;
  which->G3[gi]->y = 16 +50*gi;

  d =
    get_equip_location (which->mdata[(ppi + which->pos) * 2 - 1], 38,
			which->mdata[(ppi + which->pos) * 2]);
  col_to_box (B, d, 2);
      return ppi;

}








*/

int
POP_UP_WINDOWS::single_copy (int type)
{
  popup *temp;
  temp = does_window_exist (type);
  if (temp != NULL)
    {
      place_on_top_n_draw (temp);
      return 1;
    }
  return 0;
}

popup *
POP_UP_WINDOWS::does_window_exist (int type)
{
  popup *temp = top;

  while (temp != NULL)
    {
      if (temp->type == type)
	return temp;
      temp = temp->next;
    }
  return NULL;
}


void
POP_UP_WINDOWS::place_on_top_n_draw (popup * which)
{

  place_on_top (which);
}

void
POP_UP_WINDOWS::place_on_top (popup * which)
{
// 3 pos, begin, middle, end;

//already at top
  if (which->prev == NULL)
    return;
//what happens if the one, just before this is= to top...
  if (which->next == NULL)
    {
      which->prev->next = NULL;
      bottom = which->prev;

//if(top=which->prev)
//which->prev->prev=which;
      top->prev = which;

      which->next = top;
      top = which;
      which->prev = NULL;
      return;
    }
  which->prev->next = which->next;
  which->next->prev = which->prev;
  which->next = top;
  which->prev = NULL;
  top->prev = which;
  top = which;

}






/*

void
POP_UP_WINDOWS::show_all (void)
{
  int x;
  int y;
  int i = 0;

  if (pskip == 1)
    {
      show_mouse (screen);
      return;
    }
  show_mouse (NULL);
  if (top != NULL)
    draw_sprite (screen, layer2, 0, 0);
//blit(layer2,screen,0,0,0,0,640,480);
  show_mouse (screen);
}

*/

void
POP_UP_WINDOWS::push (popup * which)
{
//play swishing sound to say one is put here :)
  if (top == NULL)
    {
      top = which;
      which->prev = NULL;
      which->next = NULL;
      bottom = which;
      return;
    }
  which->next = top;
  top->prev = which;
  top = which;
  which->prev = NULL;
}




void
POP_UP_WINDOWS::place_on_bottom_n_draw (popup * which)
{
  place_on_bottom (which);
}

void
POP_UP_WINDOWS::place_on_bottom (popup * which)
{

  if (which->next == NULL)
    return;

  if (which->prev == NULL)
    {
      which->next->prev = NULL;
      top = which->next;


      bottom->next = which;

      which->prev = bottom;
      which->next = NULL;
      bottom = which;

      return;
    }

  which->prev->next = which->next;
  which->next->prev = which->prev;
  which->prev = bottom;
  which->next = NULL;
  bottom->next = which;
  bottom = which;
}



void
POP_UP_WINDOWS::hide_all (void)
{
  show = 0;
}

void
POP_UP_WINDOWS::close_window (popup * which)
{

//  remove_all ();

  if (which->prev == NULL)
    {
      top = which->next;
      if (which->next != NULL)
	which->next->prev = NULL;
      else
	bottom = NULL;
      goto l1;
    }

//at end and not at start.
  if (which->next == NULL)
    {
      which->prev->next = NULL;
      bottom = which->prev;
      goto l1;
    }

  which->prev->next = which->next;
  which->next->prev = which->prev;

l1:;


  destroy (which);

}

/*
void POP_UP_WINDOWS::place_on_top(popup *which)
{
  if (which->prev == NULL)
    {
      top = which->next;
      if (which->next != NULL)
	which->next->prev = NULL;
      else
	bottom = NULL;
      goto l1;
    }

//at end and not at start.
  if (which->next == NULL)
    {
      which->prev->next = NULL;
      bottom = which->prev;
      goto l1;
    }

  which->prev->next = which->next;
  which->next->prev = which->prev;

l1:;


push(which);
}
*/


void
POP_UP_WINDOWS::destroy (popup * which)
{
  //deallocates the GUT DATA
  int i;

  EV_closed[EV_max_closed] = which->type;
  EV_max_closed++;
  focus = NULL;

  for (i = 0; i < which->guts; i++)
    {
      if (which->G3[i] != NULL)
	{

	  which->G3[i]->kill ();
	}
    }

//deallocate pointers
  which->G3.DeleteAll();
//deallocate the popup itself
  delete (which);


}


void
POP_UP_WINDOWS::close_all (void)
{
  popup *temp = top;
  popup *temp2;
  while (temp != NULL)
    {
      temp2 = temp;
      destroy (temp2);
      temp = temp->next;

    }
  top = NULL;
  bottom = NULL;

}

void
POP_UP_WINDOWS::close_all_marked (void)
{
  popup *temp = top;
  popup *temp2;
  while (temp != NULL)
    {
      temp2 = temp;
      temp = temp->next;

      if (temp2->stay == 0)
	close_window (temp2);

    }
}







int
POP_UP_WINDOWS::check_popups (void)
{
//static int debug=0;
  int x = M.x;
  int y = M.y;
  int b = M.button1 + M.button2 * 2;
  static popup *which = NULL;
  static int gutn0 = 0;
  int g;
//  int type2;
//  int ok;
//  int a;
//  static int drag_gut=0;

  int ok2 = 0;
//int X_it;


  clear_events ();





//Multithread overide functions....
// These functions allow the POP_UP_WINDOWS class run multithreaded with your app.
//These functions used to go into while loops which locked control
///*
//for dragging window
  if (dragging != NULL)
    {
      EV_window = dragging;
      drag_window2 (dragging);
      return -1;
    }
// for close buttonc
  if (xclosin != NULL)
    {
//printf("12312\n");
      EV_window = xclosin;

      close_button2 (xclosin, x, y);
//return -1;
    }



//*/
//End of multithread override

  popup *temp = top;
  while (temp != NULL)
    {
      if (temp == which)
	{
	  ok2 = 1;
	  if (which != NULL)
	    EV_window = which;
	}

      temp = temp->next;
    }

  if (ok2 == 0)
    {
      which = NULL;
      gutn0 = 0;
    }




  if (which != NULL)
    {

      g = is_mouse_on_a_higher_window (which, x, y);
      if (g == 1)
	{
//	  printf ("NOT WIRED \n");
	  show_gut (which, gutn0, 0);
	  which = NULL;
	  goto lbl1;
	}

//printf("olb = %d\n",olb);
      EV_old_mb = olb;
      EV_new_mb = b;

      //Make sure still in the gut from last time
      if (x >= which->G3[gutn0]->x + which->x_pos
	  && x <= which->G3[gutn0]->x + which->x_pos + which->G3[gutn0]->x_size
	  && y >= which->G3[gutn0]->y + which->y_pos
	  && y <= which->G3[gutn0]->y + which->y_pos + which->G3[gutn0]->y_size)
	{
	  //TEXTLINE is type 3
	  //TEXTBOX is type 2
	  //BMAP3 is type 0 (not handled)
	  //BMAP1 is type 1 (sorta handled)
	  EV_gutn1 = gutn0;



	  if (which->G3[gutn0]->type == 0)
	    {
	      if (b > 0 && olb > 0)
		{
		  olb = b;
		  return 1;
		}
	      if (b == 0 && olb == 0)
		return 1;
//            type2 = which->G3[gutn0]->type2;
//            if (type2 > -1)
//              {
//                    clicked_on_something (which, gutn0);
//          printf("overgut3\n");
//                    return 1;
//
//              }
	      if (olb == 0)
		{
		  olb = b;
//          printf("Button Pressed\n");
		  show_gut (which, gutn0, 2);
//            drag_gut=1;
		  return 1;
		}

	      if (olb > 0)
		{
//          printf("Button Released\n");
		  olb = b;
//Button released
		  g = click (which, gutn0);
		  return g;
		}

	    }
//CHECK OTHER TYPES HERE///
//////
/////
////         //BMAP1
	  if (which->G3[gutn0]->type == 1)
	    {
	      if (b > 0 && olb > 0)
		{
		  olb = b;
		  return 1;
		}
	      if (b == 0 && olb == 0)
		return 1;


	      if (olb == 0)
		{
		  olb = b;
//          printf("Button Pressed\n");
//            drag_gut=1;
		  return 1;
		}

	      if (olb > 0)
		{
		  olb = b;
//Button released
		  return 1;
		}

	    }
/////////////
/////////////
////////////
///////////  TEXTBOX
	  if (which->G3[gutn0]->type == 2)
	    {
	      if (b > 0 && olb > 0)
		{
		  olb = b;
		  return 1;
		}
	      if (b == 0 && olb == 0)
		return 1;


	      if (olb == 0)
		{
		  olb = b;
//          printf("Button Pressed\n");
//            drag_gut=1;
		  return 1;
		}

	      if (olb > 0)
		{
		  olb = b;
//Button released
		  focus = which;
		  focus_gut = gutn0;
		  return 1;

		}

	    }
/////////////
/////////////
////////////
///////////  TEXTLINE
	  if (which->G3[gutn0]->type == 3)
	    {
	      if (b > 0 && olb > 0)
		{
		  olb = b;
		  return 1;
		}
	      if (b == 0 && olb == 0)
		return 1;


	      if (olb == 0)
		{
		  olb = b;
//          printf("Button Pressed\n");
//            drag_gut=1;
		  return 1;
		}

	      if (olb > 0)
		{
		  olb = b;
//Button released
		  return 1;
		}

	    }



	}

//OPEN {

//outside of gut. so change gut to non flashy display

//      printf("Mouse offgut\n");

      EV_gutn0 = gutn0;



      show_gut (which, gutn0, 0);
      which = NULL;

//      if(drag_gut==1)
//     printf("Attempted drag off gut\n");


//    drag_gut=0;
    }


  //THIS CODE ON TRIES TO FIND WHAT GUT NUMBER ITS OVER
lbl1:;


  temp = top;
  while (temp != NULL)
    {
      if (temp->x_pos <= x)
	if (temp->y_pos <= y)
	  if (temp->x_pos + temp->x_size >= x)
	    if (temp->y_pos + temp->y_size >= y)
	      {
		//works
		EV_window = temp;
		g = do_popup (temp, x - temp->x_pos, y - temp->y_pos);

		//check highlight/click if temp=which
		if (g > -1)
		  {
		    olb = 0;
		    which = temp;
		    gutn0 = g;
		    //show its over top code
		    EV_gutn1 = gutn0;


//                     blit (xbut, which->display, 0, 0, 40, 40, 16, 16);

		    show_gut (which, gutn0, 1);
		  }
		return 1;
	      }
      temp = temp->next;
    }








  return 0;
}



int
POP_UP_WINDOWS::is_mouse_on_a_higher_window (popup * which, int x, int y)
{
  popup *temp = top;

  while (temp != which)
    {
      if (x >= temp->x_pos && x <= temp->x_pos + temp->x_size
	  && y >= temp->y_pos && y <= temp->y_pos + temp->y_size)
	return 1;
      temp = temp->next;
    }

  return 0;
}







void
POP_UP_WINDOWS::show_gut (popup * which, int gutn0, int b)
{
//b
//0 is off
//1 is over
//2 is press

//special type of gut
  static int type2;
//  int oldt2 = type2;


  if (which->G3[gutn0] == NULL)
    return;

  type2 = which->G3[gutn0]->type2;

  //Not sure this is the right code for what display was
//  which->G3[gutn0]->draw (b, which->x_pos, which->y_pos,which->display);
  which->G3[gutn0]->draw (b, 0, 0, which->display);
//      which->G3[i]->draw (which->display);

//                    printf("overonce\n");
  if (type2 > -1)
    {
      switch (type2)
	{
	case 0:
//        type2 = ch.pref.npc;
	  break;
	case 1:
//        type2 = ch.pref.item;
	  break;
	case 2:
//        type2 = ch.pref.wall;
	  break;
	case 3:
//        type2 = ch.pref.floor;
	  break;
	}


      if (b == 1 || b == 2)
	{
	  curuse = type2;
	  switch (type2)
	    {
	    case 1:
//set mouse sprite changed what the mouse looked like when clicked over.
//            set_mouse_sprite (G_talk);
	      break;
	    case 2:
//            set_mouse_sprite (G_attack);
	      break;
	    case 3:
//            set_mouse_sprite (G_look);
	      break;
	    case 4:
//            set_mouse_sprite (G_use);
	      break;
	    }
	}
//      if (b == 0)
// Return mouse to normallity
//      set_mouse_sprite (moused);
    }



//  draw_sprite (screen, layer2, 0, 0);
//  show_mouse (screen);
}






//Once used for clicking on stuff, now I am more partial to events
void
POP_UP_WINDOWS::clicked_on_something (popup * which, int gutn0)
{
  int type2;
  int c;
//  char c1[40];
//  ITEM *temp;
//ok
//curuse++;
  switch (curuse)
    {
    case 1:
      c = 6;
      break;
    case 2:
      c = 7;
    case 3:
      c = 10;
    case 4:
      c = 8;
    }


  type2 = which->G3[gutn0]->type2;
  switch (type2)
    {
    case 0:
//npc
      break;
    case 1:

      //temp = which->I->look_up (which->pos);
//      item_react (temp, c);


      break;
    case 2:
//wall
      break;
    case 3:
//floor
      break;
    }


}



int
POP_UP_WINDOWS::click (popup * which, int gutn0)
{
//  popup *temp;
//play clicking sound.

  show_gut (which, gutn0, 0);


  return 1;
}





int
POP_UP_WINDOWS::do_popup (popup * which, int x, int y)
{

  int b = M.button1 + M.button2 * 2;

  int confirm = 0;
  int i;
//is over a gut?




  for (i = 0; i < which->guts; i++)
    {
//      if (which->G3[i]->type == 0)
//return for any gut type, not just BMAP3
      if (x >= which->G3[i]->x && x <= which->G3[i]->x + which->G3[i]->x_size
	  && y >= which->G3[i]->y && y <= which->G3[i]->y + which->G3[i]->y_size)
	{

	  return i;


	}
    }

  if (b == 0)
    {
      olb = 0;
      return -1;

    }
//popup is clicked


  if (b != 0)
    {
      if (olb == 0)
	confirm = stay_button (which, x, y);
      if (confirm == 1)
	{
	  olb = b;
	  return -1;
	}

      confirm = close_button1 (which, x, y);
      if (confirm == 1)
	{
	  olb = b;
	  return -1;
	}

    }

  if (b == 2)
    {
      place_on_bottom_n_draw (which);

    }
//allegro_exit();
//*/
  if (olb != 1)
    drag_window (which);
  return -1;
}



//TODO

//change stay_button on or off
int
POP_UP_WINDOWS::stay_button (popup * which, int x, int y)
{
//dominate


  if (which->stay == 1)
    return -1;



  if (x > 9)
    if (x < 36)
      if (y < 17)
	{
	  if (which->stay == 0)
	    {
//color change?

	      csRef < iImage > t1;
t1.AttachNew(new csImageMemory(grbut));
//	      t1 = grbut->Clone ();
	      alter_colors_ratio (t1, 0, 120, 240, which->bordercolor_r,
				  which->bordercolor_g, which->bordercolor_b,
				  7);
	      blit (t1, which->display, 0, 0, 10, 0, grbut->GetWidth (), 16);
// blit (t1, overlay, 0, 0, which->x_pos+10, which->y_pos, grbut->GetWidth(), 16);
	      which->stay = 2;
	    }
	  else if (which->stay == 2)
	    {

	      csRef < iImage > t1;
t1.AttachNew(new csImageMemory(rdbut));
//	      t1 = rdbut->Clone ();
	      alter_colors_ratio (t1, 0, 120, 240, which->bordercolor_r,
				  which->bordercolor_g, which->bordercolor_b,
				  7);
	      blit (t1, which->display, 0, 0, 10, 0, grbut->GetWidth (), 16);
// blit (t1, overlay, 0, 0, which->x_pos+10, which->y_pos, grbut->GetWidth(), 16);
	      which->stay = 0;

	    }
	  return 1;
	}
  return 0;

}






///* multithreaded code part 3: too slow
void
POP_UP_WINDOWS::drag_window (popup * which)
{
//  int ox = which->x_pos;
//  int oy = which->y_pos;

  dragging = which;
  original_mousex = M.x;
  original_mousey = M.y;
  place_on_top (which);
}


void
POP_UP_WINDOWS::drag_window2 (popup * which)
{
  int x = M.x;
  int y = M.y;
//  int ox = which->x_pos;
  //int oy = which->y_pos;


  which->x_pos = which->x_pos + x - original_mousex;
  which->y_pos = which->y_pos + y - original_mousey;

  original_mousex = x;
  original_mousey = y;

  if (M.button1 == 0 && M.button2 == 0)
    {
      olb = 0;
      dragging = NULL;
    }

}


//end of multithread pt 3*/





/*
void POP_UP_WINDOWS::drag_window2 (popup * which)
{
  int x = M.x;
  int y = M.y;
  int ox = which->x_pos;
  int oy = which->y_pos;


//original_mousex=M.x;
//original_mousey=M.y;


  //BITMAP *background;

//  if (mouse_b != 1)
//    return;
//  background = create_bitmap (which->x_size, which->y_size);
//  clear (background);
//  clear (layer2);
//  show_to_bottom (which);
//  remove_all ();
//  show_all ();
//  show_mouse (NULL);
//  blit (screen, layer2, 0, 0, 0, 0, 640, 480);

//  while (mouse_b == 1)
//    {
      which->x_pos = ox + x - original_mousex;
      which->y_pos = oy + y - original_mousey;
//      blit (layer2, background, which->x_pos, which->y_pos, 0, 0,
//	    background->w, background->h);
//      draw_sprite (layer2, which->display, which->x_pos, which->y_pos);
//      blit (layer2, screen, 0, 0, 0, 0, 640, 480);
//      draw_sprite (layer2, background, which->x_pos, which->y_pos);
//    }
//  make_layer2 ();
//  show_mouse (NULL);
//  draw_sprite (screen, which->display, which->x_pos, which->y_pos);
//  show_mouse (screen);
//  destroy_bitmap (background);

make_overlay();
if(M.button1==0 && M.button2==0)
dragging=NULL;
}







*/


void
POP_UP_WINDOWS::show_windows (void)
{
  popup *temp = bottom;
  csRef < iTextureManager > txtmgr;
  csRef < iTextureHandle > txt;


//iGraphics2D* g2d = gut_g3d->GetDriver2D ();
//printf("in1\n");
  txtmgr = gut_g3d->GetTextureManager ();
//printf("in2\n");

  while (temp != NULL)
    {
//printf("in3\n");


      txt = txtmgr->RegisterTexture (temp->display, CS_TEXTURE_2D);
//HERE
//OLD      txt->Prepare ();
      csPixmap *pixmap;
      pixmap = new csSimplePixmap (txt);

      pixmap->Draw (gut_g3d, temp->x_pos, temp->y_pos);

// pixmap->Draw(gut_g3d, px , py);
      delete (pixmap);


      temp = temp->prev;
    }


}






int
POP_UP_WINDOWS::close_button1 (popup * which, int x, int y)
{

  place_on_top (which);
//REFACTOR  
int b = M.button1 + M.button2 * 2;

  if (x < which->x_size - 10)
    if (x > which->x_size - 10 - 17)
      if (y < 16)
	{
	  if (x < which->x_size - 10
	      && x > which->x_size - 10 - 16 && y < 16 && y > 0)
	    {


	      blit (x2but, which->display, which->x_size - 10 - 16, 0);
	      xclosin = which;
	      close_int = b;
	      return 1;
	    }
	  else
	    return 0;

	}
  return 0;
}


int
POP_UP_WINDOWS::close_button2 (popup * which, int x, int y)
{
//4 cases

//1-Release button over close
//2-Release button not over close(once not over, no longer a problem)
//3-Hold button over X
//3-Hold button not over X(once not over, no longer a problem)
  int b = M.button1 + M.button2 * 2;

  //release button

  x = x - which->x_pos;
  y = y - which->y_pos;
  if (b == 0)
    {
//cases 1 & 2

      if (x < which->x_size - 10)
	if (x > which->x_size - 10 - 17)
	  if (y < 16)

	    if (x < which->x_size - 10
		&& x > which->x_size - 10 - 16 && y < 16 && y > 0)
	      {

//blit (xbut, which->display, which->x_size - 10 - 16, 0);
//blit (x2but, which->display, which->x_size -10-16,0);
		xclosin = NULL;
		olb = 0;
//close_int=b;
		blit (xbut, which->display, which->x_size - 10 - 16, 0);

		if (close_int == 1)
		  close_window (which);
		else
		  close_all_marked ();

		return 1;
	      }

      blit (xbut, which->display, which->x_size - 10 - 16, 0);
      xclosin = NULL;
      olb = 0;
      return 1;


    }
  else
    {
//cases 3 and 4, still held



      if (x < which->x_size - 10)
	if (x > which->x_size - 10 - 17)
	  if (y < 16)

	    if (x < which->x_size - 10
		&& x > which->x_size - 10 - 16 && y < 16 && y > 0)
	      {
		blit (x2but, which->display, which->x_size - 10 - 16, 0);

//blit (xbut, which->display, which->x_size - 10 - 16, 0);
//blit (x2but, which->display, which->x_size -10-16,0);
//xclosin=which;
//close_int=b;
//return 1;
	      }
      blit (xbut, which->display, which->x_size - 10 - 16, 0);

      xclosin = NULL;
      olb = b;
      return 1;





    }




}





	/*
	   while (b != 0)
	   {

	   if (mouse_x < which->x_pos + which->x_size - 10
	   && mouse_x > which->x_pos + which->x_size - 10 - 16
	   && mouse_y < which->y_pos + 16 && mouse_y > which->y_pos)
	   {
	   if (confirm == 0)
	   {
	   show_mouse (NULL);
	   blit (x2but, screen, 0, 0,
	   which->x_pos + which->x_size - 10 - 16,
	   which->y_pos, 16, 16);
	   show_mouse (screen);
	   confirm = b;
	   }
	   }
	   else if (confirm != 0)
	   {
	   show_mouse (NULL);
	   blit (xbut, screen, 0, 0,
	   which->x_pos + which->x_size - 10 - 16, which->y_pos,
	   16, 16);
	   show_mouse (screen);
	   confirm = 0;
	   }
	   b = mouse_b;
	   }
	   return confirm;
	   }
	 */

TEXTLINE *
POP_UP_WINDOWS::textlineize (popup * which, int gutn0, int r, int g, int b)
{
  TEXTLINE *B = new TEXTLINE;
  int i;

  B->x_size = 0;
  B->y_size = 16;

  for (i = 0; i < 110; i++)
    {
      B->e.r[i] = r;
      B->e.g[i] = g;
      B->e.b[i] = b;
      B->e.a[i] = 255;

//dont display
      B->e.ba[i] = 0;
      B->e.br[i] = 50;
      B->e.bg[i] = 50;
      B->e.bb[i] = 50;
    }

  which->G3[gutn0] = B;
  return B;

}





TEXTBOX *
POP_UP_WINDOWS::textboxize (popup * which, int gutn0, int r, int g, int b)
{
  TEXTBOX *B = new TEXTBOX;
  int i;

  B->x_size = 12 * B->maxlen;
  B->y_size = 16;

  for (i = 0; i < 110; i++)
    {
      B->e.r[i] = r;
      B->e.g[i] = g;
      B->e.b[i] = b;
      B->e.a[i] = 255;
      B->e.ba[i] = 255;
      B->e.br[i] = 50;
      B->e.bg[i] = 50;
      B->e.bb[i] = 50;
    }

  which->G3[gutn0] = B;
  return B;

}




BMAP3 *
POP_UP_WINDOWS::boxize (popup * which, int gutn0, int x, int y, int r, int g,
			int b)
{
  BMAP3 *B = new BMAP3;
  int i;
  for (i = 0; i < 3; i++)
    {
B->b[i].AttachNew(new csImageMemory(I_nothing));
//      B->b[i] = I_nothing->Clone ();
csImageManipulate *csim;
csim=new csImageManipulate;
B->b[i]=csim->Rescale(B->b[i],x,y,1);

//      B->b[i]->Rescale (x, y);

    }

  B->x_size = x;
  B->y_size = y;

  cbox (B->b[0], 0, 0, x - 1, y - 1, r, g, b);
  dbox (B->b[2], 0, 0, x - 1, y - 1, r, g, b);
  r = r - 75;
  g = g - 75;
  b = b - 75;
  if (r < 0)
    r = 0;
  if (g < 0)
    g = 0;
  if (b < 0)
    b = 0;
  cbox (B->b[1], 0, 0, x - 1, y - 1, r, g, b);
  which->G3[gutn0] = B;
  return B;

}









void
POP_UP_WINDOWS::insert_guts (popup * which)
{
//new gut->make classes
  int i;

//printf("here2222\n");
//printf("%d\n",which->guts);
//printf("%d\n",which->G3[0]->page);
//printf("%d\n",which->curpage);
//printf("%d\n",which->guts);
//  blit (xbut, which->display, 0, 0, 40, 40, 16, 16);

  for (i = 0; i < which->guts; i++)
//changed.
    if (which->G3[i]->page == which->curpage)
{
//printf("hey\n");
      which->G3[i]->draw (which->display);
}

}

void
POP_UP_WINDOWS::send_focus (char c)
{


//apply filters here!
//I don't feel like putting the filters in yet, too much effort
//textbox,decimalbox,filename filters

//c=1 means delete
//c=2 means return
//c=3 means tab
  char c1[60];
  char c2[200];
  int len;
//int i;


printf("Sent focus 0");

  if (c == 3)
    {
      EV_window = focus;
      EV_new_mb = 5;
      return;
    }

  if (c == 2)
    {
      EV_window = focus;
      EV_new_mb = 4;
      return;
    }

  if (c == 1)
    {
printf("Send_focus 1");

      strcpy (c2, focus->G3[focus_gut]->e.c1);
printf("Send_focus 2");

      len = strlen (c2);
printf("Send_focus 3");

if(len>0){
      c2[len - 1] = 32;
      focus->G3[focus_gut]->set_text (c2);
printf("Send_focus 4");

      insert_guts (focus);
      c2[len - 1] = 0;
}
printf("Send_focus 5");

//if(len<5)
//{
///for(i=len;i<6;i++)
//{
//c2[i]=0;
//}
//c2[i]=0;
//}

      focus->G3[focus_gut]->set_text (c2);
printf("Send_focus 6-end");




      return;
    }
printf("Send_focus B1");

  strcpy (c1, "a");
  c1[0] = c;

  strcpy (c2, focus->G3[focus_gut]->e.c1);
printf("Send_focus B2");

  strcat (c2, c1);
  focus->G3[focus_gut]->set_text (c2);
printf("Send_focus B3");

//SCF_DELETE(focus->display);

//focus->display=make_outline(focus);
  insert_guts (focus);
printf("Send_focus B4-end");


}


void POP_UP_WINDOWS::gainfocus (void)
{
int i;

for(i=0;i<top->guts;i++)
{
if(top->G3[i]->type==2)
{
//focus=top;
//focus_gut=i;
enter_down=1;
setfocus(top,i);
return;


}

}


}

void POP_UP_WINDOWS::setfocus(popup *which, int gutn0)
{
//int i;
losefocus();
focus=which;
focus_gut=gutn0;
show_gut (which, gutn0, 4);
}

void POP_UP_WINDOWS::losefocus(void)
{
if(focus!=NULL)
show_gut (focus,focus_gut,0);
focus=NULL;
}

