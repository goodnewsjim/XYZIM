	 /*
	 Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/


#include "iImageHelper.h"
#include "TextHelper.h"
#include "universals.h"
#include "crystalspace.h"


//Universals declared here:
csRef < iGraphics3D > gut_g3d;



/// END OF UNIVERSAL DECLARATIONS

//Way guts work, is that they get placed into your popup.
//Then they do different things depening on mouse over/clicked on.
//You put different functions in them, by changing the virtual functions :)





//This code loses the reference to a csRef pointer...
//In addition deallocates the memory, if no one else pointing to it.
//Which is what makes csRef sorta cool.




//Replace with csRef<>.invalidate()
void
SCF_DELETE (iImage * pointer)
{



  if (pointer != NULL)
    {
      pointer = NULL;
//CHANGE-Not sure how this code will work
//point
//csRef<>.invalidate();
//      SCF_DEC_REF (pointer);
    }


}



//Initialize GUT with nothing really.
void
GUT::debug_display (int pos, int px, int py)
{
}
void
GUT::kill (void)
{
}
void
GUT::draw (iImage * which)
{
}
void
GUT::draw (int pos, int px, int py, iImage * dest)
{
}
void
GUT::set_text (char *c1)
{
}

GUT::GUT (void)
{
  type2 = -1;
  page = 0;
}

BMAP1::BMAP1 ()
{
  page = 0;
  txt_reg = 0;
//one means inactive
  type = 1;
  type2 = -1;
  x_size = 0;
  y_size = 0;
  b = NULL;

}


BMAP1::BMAP1 (iImage * bm)
{
  page = 0;
  txt_reg = 0;
  x = 0;
  y = 0;
//one means inactive
  type = 1;
  type2 = -1;
  x_size = bm->GetWidth ();
  y_size = bm->GetHeight ();
  b = bm;
}

void
BMAP1::draw (iImage * dest)
{

  blit_alpha (b, dest, 0, 0, x, y, x_size, y_size, 1);
}


void
BMAP1::draw (int pos, int px, int py, iImage * dest)
{
  blit_alpha (b, dest, 0, 0, x + px, y + py, b->GetWidth (), b->GetHeight (),
	      1);
}


void
BMAP1::kill (void)
{

  /*
     if(b!=NULL)
     {
     SCF_DEC_REF(b);
     b=NULL;
     }
   */
  SCF_DELETE (b);
  /*
     if(b->w == 0  )
     if(b->h == 0 )
     return;
     destroy_bitmap (b);
   */
}


void
BMAP1::debug_display (int pos, int px, int py)
{

//if(txt_reg==0)
//{
//txt_reg=1;
//iGraphics2D* g2d = gut_g3d->GetDriver2D ();
  txtmgr = gut_g3d->GetTextureManager ();
  txt = txtmgr->RegisterTexture (b, CS_TEXTURE_2D);
//}

//CHANGE 
//  txt->Prepare ();
  csPixmap *pixmap;
  pixmap = new csSimplePixmap (txt);

// pixmap->DrawScaled (gut_g3d, px-500 , py-500,x_size*4,y_size*4);
// pixmap->DrawScaled (gut_g3d, px , py,x_size*4,y_size*4);

  pixmap->DrawScaled (gut_g3d, px, py, x_size * 1, y_size * 1);

// pixmap->Draw(gut_g3d, px , py);
  delete (pixmap);
}




BMAP3::BMAP3 (void)
{
  txt_reg = 0;
  page = 0;
  int i;
  for (i = 0; i < 3; i++)
    b[i] = NULL;
  type = 0;
  type2 = -1;
  x = 0;
  y = 0;

}




void
BMAP3::debug_display (int pos, int px, int py)
{
//csRef<iImage> i3;
//i3=b[pos]->Clone();
//iImage *i4;
//i4=i3;

//to stop txt mgr from exploding, save info
//if(txt_reg==0)
//{
//txt_reg=1;
//iGraphics2D* g2d = gut_g3d->GetDriver2D ();
  txtmgr = gut_g3d->GetTextureManager ();
  txt = txtmgr->RegisterTexture (b[pos], CS_TEXTURE_2D);
//}



//every time
  blit (xbut, b[pos], 0, 0, 0, 0, xbut->GetWidth (), xbut->GetHeight ());
//change
//  txt->Prepare ();

  csPixmap *pixmap;
  pixmap = new csSimplePixmap (txt);
  pixmap->Draw (gut_g3d, px, py);
  delete (pixmap);

//Registering Textures in txtmgr
//


}

void
BMAP3::kill (void)
{
  SCF_DELETE (b[0]);
  SCF_DELETE (b[1]);
  SCF_DELETE (b[2]);

//  int i;
//  for (i = 0; i < 3; i++)
//    destroy_bitmap (b[i]);
}

void
BMAP3::draw (iImage * dest)
{

  blit_alpha (b[0], dest, 0, 0, x, y, x_size, y_size, 1);

//  blit (xbut, dest, 0, 0, 40, 40, 16, 16);

}

void
BMAP3::draw (int pos, int px, int py, iImage * dest)
{
  blit_alpha (b[pos], dest, 0, 0, x + px, y + py, b[pos]->GetWidth (),
	      b[pos]->GetHeight (), 1);
}




TEXTBOX::TEXTBOX (void)
{
  page = 0;
//  int i;
  type = 2;
  type2 = -1;
  x = 0;
  y = 0;
  filter = 0;
  maxlen = 12;
}
TEXTBOX::TEXTBOX (int filt, int maxl)
{
  page = 0;
//  int i;
  type = 2;
  type2 = -1;
  x = 0;
  y = 0;

  filter = filt;
  maxlen = maxl;
}




void
TEXTBOX::kill (void)
{


}


void
TEXTBOX::set_text (char *c1)
{
  x_size = 12 * strlen (c1);
  if (x_size < 60)
    x_size = 60;

  strcpy (e.c1, c1);

}

void
TEXTBOX::draw (iImage * dest)
{

  if (strlen (e.c1) == 0)
    x_size = 48;

  rect (dest, x, y, x + x_size, y + 16, e.br[0], e.bg[0], e.bb[0]);

//  blit_alpha(b,dest, 0, 0, x, y, x_size, y_size,1);

  blit_text_background (dest, e, x, y);

}

void
TEXTBOX::draw (int pos, int px, int py, iImage * dest)
{
  if (strlen (e.c1) == 0)
    x_size = 48;
  rect (dest, x, y, x + x_size, y + 16, e.br[0], e.bg[0], e.bb[0]);
//if(e.ba[0]==0)
//blit_text(dest,e,x,y);
//else
  blit_text_background (dest, e, x, y);


//  blit_alpha(b, dest, 0, 0, x + px, y + py, b[pos]->GetWidth(), b[pos]->GetHeight(),1);
}




TEXTLINE::TEXTLINE (void)
{
  page = 0;
//  int i;
  type = 3;
  type2 = -1;
  x = 0;
  y = 0;
}


void
TEXTLINE::kill (void)
{


}

void
TEXTLINE::draw (iImage * dest)
{

//rect (dest, x, y,x+x_size,y+16,e.br[0],e.bg[0],e.bb[0]);

  if (e.ba[0] == 0)
    blit_text (dest, e, x, y);
  else
    blit_text_background (dest, e, x, y);

}

void
TEXTLINE::draw (int pos, int px, int py, iImage * dest)
{
//rect (dest, x, y,x+x_size,y+16,e.br[0],e.bg[0],e.bb[0]);
  if (e.ba[0] == 0)
    blit_text (dest, e, x, y);
  else
    blit_text_background (dest, e, x, y);

}

void
TEXTLINE::set_text (char *c1)
{
  x_size = 12 * strlen (c1);
  if (x_size < 60)
    x_size = 60;
  strcpy (e.c1, c1);

}




/*
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
