	 /*
	 Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/

#ifndef __TextHelper__H__
#define __TextHelper__H__

#include "crystalspace.h"
#include "universals.h"

void blit_text (iImage * dest, e_text e, int posx, int posy);
void blit_text_background (iImage * dest, e_text e, int posx, int posy);


void color_text (e_text & e, int r, int g, int b);
void color_text (e_text & e, int r, int g, int b, int alpha);


//Need ability to change brightness too

void blue_text (e_text & e);
void green_text (e_text & e);
void red_text (e_text & e);
void cyan_text (e_text & e);
void DKcyan_text (e_text & e);
void purple_text (e_text & e);
void DKpurple_text (e_text & e);
void zombie_text (e_text & e);
void ash_text (e_text & e);
void lavendar_text (e_text & e);
void forestgreen_text (e_text & e);
void spewg_text (e_text & e);


void make_background (e_text & e);
void make_background (e_text & e, int r, int g, int b);

int text_width (e_text & e);


#endif
