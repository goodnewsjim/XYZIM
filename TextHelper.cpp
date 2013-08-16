	 /*
	 Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/


#include"crystalspace.h"
#include "math.h"
#include "universals.h"
#include "iImageHelper.h"
//DEL
//csRef < iImage > I_I[95];
//END DEL

csRef<iImage> xbut,x2but,vtbar,hzbar,hzbar2,grbut,rdbut,cornul,cornll,cornur,cornlr;
 csRef<iImage> TEXT,TEXT2;
csRef<iImage> I_I[95];
csRef<iImage> I_nothing;
POP_UP_WINDOWS P;


e_text::e_text (void)
{
  int i;
  strcpy (c1, "");
  for (i = 0; i < 110; i++)
    {
      r[i] = 0;
      g[i] = 0;
      b[i] = 0;
      a[i] = 255;
      br[i] = 0;
      bg[i] = 0;
      bb[i] = 0;
      ba[i] = 255;
    }


}



e_text::e_text (char *c2)
{
  int i;
  strcpy (c1, c2);
  for (i = 0; i < 110; i++)
    {
      r[i] = 0;
      g[i] = 0;
      b[i] = 0;
      a[i] = 255;
      br[i] = 0;
      bg[i] = 0;
      bb[i] = 0;
      ba[i] = 255;
    }


}

int text_width (e_text & e)
{
  int x = 0;
  int max;
  int i;
  max = strlen (e.c1);
  for (i = 0; i < max; i++)
    x = x + I_I[e.c1[i] - 32]->GetWidth () + 2;

  return x;

}
void
blit_text (iImage * dest, e_text e, int posx, int posy)
{
  int max;
  int i;
  int x = 0, y = 0;
  csRef < iImage > temp;
  max = strlen (e.c1);
//max
  for (i = 0; i < max; i++)
    {
//csImageMemory *CSIM;
//CSIM=new(I_I[0]);
//temp.AttachNew(new csImageMemory (I_I[0]));
	temp.AttachNew(new csImageMemory (I_I[e.c1[i] - 32]));      
//xbut2.AttachNew(new csImageMemory(xbut));

//fixed	temp = I_I[e.c1[i] - 32]->Clone ();
      if (e.r[i] == 0 && e.g[i] == 0 && e.b[i] == 0)
	{
	}
      else
	alter_colors (temp, 0, 20, 230, e.r[i], e.g[i], e.b[i]);

      blit_alpha (temp, dest, posx + x, posy + y);
      x = x + temp->GetWidth () + 1;

    }






}



//draws the background instead of alpha
void
blit_text_background (iImage * dest, e_text e, int posx, int posy)
{
  int max;
  int i;
  int r, g, b;
  int text_size;


  r = e.br[0];
  g = e.bg[0];
  b = e.bb[0];




  int x = 0, y = 0;
  csRef < iImage > temp;
  max = strlen (e.c1);
//max
//standardize textlength-or make a length counter
  text_size = text_width (e);

  rect (dest, posx, posy, posx + text_size, posy + 16, r, g, b);

  for (i = 0; i < max; i++)
    {
//fixed      temp = I_I[e.c1[i] - 32]->Clone ();
	temp.AttachNew (new csImageMemory (I_I[e.c1[i] - 32]));      

      if (e.r[i] == 0 && e.g[i] == 0 && e.b[i] == 0)
	{
	}
      else
	alter_colors (temp, 0, 20, 230, e.r[i], e.g[i], e.b[i]);

      blit_alpha (temp, dest, posx + x, posy + y);
      x = x + temp->GetWidth () + 1;

    }






}


void
blue_text (e_text & e)
{
  int i;
  for (i = 0; i < 110; i++)
    {
      e.g[i] = 40;
      e.b[i] = 255;
      e.r[i] = 0;
    }
}

void
green_text (e_text & e)
{
  int i;
  for (i = 0; i < 110; i++)
    {
      e.g[i] = 255;
      e.b[i] = 0;
      e.r[i] = 0;
    }


}
void
red_text (e_text & e)
{
  int i;
  for (i = 0; i < 110; i++)
    {
      e.r[i] = 255;
      e.g[i] = 0;
      e.b[i] = 0;
    }


}

void
cyan_text (e_text & e)
{
  int i;
  for (i = 0; i < 110; i++)
    {
      e.r[i] = 0;
      e.g[i] = 250;
      e.b[i] = 250;
    }


}
void
DKcyan_text (e_text & e)
{
  int i;
  for (i = 0; i < 110; i++)
    {
      e.r[i] = 0;
      e.g[i] = 170;
      e.b[i] = 170;
    }


}
void
purple_text (e_text & e)
{
  int i;
  for (i = 0; i < 110; i++)
    {
      e.r[i] = 250;
      e.g[i] = 0;
      e.b[i] = 250;
    }


}
void
DKpurple_text (e_text & e)
{
  int i;
  for (i = 0; i < 110; i++)
    {
      e.r[i] = 170;
      e.g[i] = 0;
      e.b[i] = 170;
    }


}
void
zombie_text (e_text & e)
{
  int i;
  for (i = 0; i < 110; i++)
    {
      e.r[i] = 250;
      e.g[i] = 175;
      e.b[i] = 175;
    }


}
void
ash_text (e_text & e)
{
  int i;
  for (i = 0; i < 110; i++)
    {
      e.r[i] = 150;
      e.g[i] = 120;
      e.b[i] = 120;
    }


}
void
lavendar_text (e_text & e)
{
  int i;
  for (i = 0; i < 110; i++)
    {
      e.r[i] = 250;
      e.g[i] = 150;
      e.b[i] = 250;
    }


}
void
forestgreen_text (e_text & e)
{
  int i;
  for (i = 0; i < 110; i++)
    {
      e.r[i] = 120;
      e.g[i] = 180;
      e.b[i] = 120;
    }


}
void
spewg_text (e_text & e)
{
  int i;
  for (i = 0; i < 110; i++)
    {
      e.r[i] = 240;
      e.g[i] = 240;
      e.b[i] = 240;
    }


}

void
color_text (e_text & e, int r, int g, int b)
{
  int i;
  if (r > 255)
    r = 255;
  if (g > 255)
    g = 255;
  if (b > 255)
    b = 255;

  for (i = 0; i < 110; i++)
    {
      e.r[i] = r;
      e.g[i] = g;
      e.b[i] = b;
    }


}
void
color_text (e_text & e, int r, int g, int b, int alpha)
{
  int i;
  if (r > 255)
    r = 255;
  if (g > 255)
    g = 255;
  if (b > 255)
    b = 255;
  for (i = 0; i < 110; i++)
    {
      e.r[i] = r;
      e.g[i] = g;
      e.b[i] = b;
      e.a[i] = alpha;
    }


}


void
make_background (e_text & e)
{
  int i;
  int r;
  int g;
  int b;

  r = e.r[0] - 80;
  g = e.g[0] - 80;
  b = e.b[0] - 80;
  if (r < 0)
    r = 0;
  if (g < 0)
    g = 0;
  if (b < 0)
    b = 0;

  for (i = 0; i < 110; i++)
    {
      e.br[i] = r;
      e.bg[i] = g;
      e.bb[i] = b;
    }




}



void
make_background (e_text & e, int r, int g, int b)
{
  int i;


  for (i = 0; i < 110; i++)
    {
      e.br[i] = r;
      e.bg[i] = g;
      e.bb[i] = b;
    }




}



