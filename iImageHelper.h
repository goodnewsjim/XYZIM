	 /*
	 Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/

#ifndef __iImageHelper__H__
#define __iImageHelper__H__

#include "crystalspace.h"

//#include "popup_guts.h"
int getred (iImage * image, int x, int y);
int getgreen (iImage * image, int x, int y);
int getblue (iImage * image, int x, int y);
int getalpha (iImage * image, int x, int y);


void setgreen (iImage * image, int x, int y, int green);
void setblue (iImage * image, int x, int y, int blue);
void setred (iImage * image, int x, int y, int red);
void setalpha (iImage * image, int x, int y, int alpha);

//Copies full source into dest starting at point
void blit (iImage * source, iImage * dest, int destx, int desty);
//Copies a partial source(startx,y)-(endx,y), into dest at point destx,desty
//Copies alpha info, use merge to blend
void blit (iImage * source, iImage * dest, int source_startx,
	   int source_starty, int destx, int desty, int source_endx,
	   int source_endy);


//First, you need to load the image with SOME alpha defined, even if it means nothing.
//This function can then take a color:r,g,b and make it your alpha mask.
//You can specify a range of colors that are similar too.
void blit_transparent (iImage * source, iImage * dest, int destx, int desty,
		       int r2, int g2, int b2);
void blit_transparent (iImage * source, iImage * dest, int destx, int desty,
		       int r2, int g2, int b2, int range);
void blit_transparent (iImage * source, iImage * dest, int source_startx,
		       int source_starty, int source_endx, int source_endy,
		       int destx, int desty, int r0, int g0, int b0,
		       int range);

//Takes all of one color out of your image.
void remove_red (iImage * source);
void remove_green (iImage * source);
void remove_blue (iImage * source);

void blit_alpha (iImage * source, iImage * dest, int destx, int desty);
void blit_alpha (iImage * source, iImage * dest, int destx, int desty,
		 int range);
void blit_alpha (iImage * source, iImage * dest, int source_startx,
		 int source_starty, int destx, int desty, int source_endx,
		 int source_endy, int range);

//Blit_merge - splatter with partial alpha

//invert_brightness
void invert_brightness (iImage * source);

//So the text is mainly blue, with some traces of green for brightness.
//Swapping blue's intensity around to different values is the key way to change the text color.
//  Blue<->green changes text to green
//  Blue<->red changes text to red.
//  red<->green doesn't do anything to noticible
void swap_blue_to_green (iImage * source);
void swap_blue_to_red (iImage * source);
void swap_red_to_green (iImage * source);

//Future functions will take percentage of blue/red/green
// Use that as the brightness, and convert into a new ratio.
//
// I have like 20 parts blue, to 1 part green in text, so I'll probably do:
// 0,20,230
// for my original ratios of R G B for my blue biased text
//
// Now I can switch to red colors by doing:
//
// 230,0,0
// purple
// 230,0,230
//
// Darker red
// 120,0,0
//
// Dark purple
// 120,0,120
//
// Red biased purple:
// 230,0,120
//
// Green:
//
// 0,230,0
//
// and so on
//o_r,o_g,ob are original, for my blue biased text I pass:0,20,230

//230,230,0 is like gold
//240,0,240 purple

void alter_colors (iImage * source, int o_r, int o_g, int o_b, int r2, int g2,
		   int b2);
//don't alter a color unless its within range of originals
void alter_colors_except (iImage * source, int o_r, int o_g, int o_b, int r2,
			  int g2, int b2, int rng);


void alter_colors_ratio (iImage * source, int o_r, int o_g, int o_b, int r2,
			 int g2, int b2, int rng);

//draws a rectangle onto *dest
void rect (iImage * dest, int x, int y, int end_x, int end_y, int r, int g,
	   int b);
//draws an open rectangle
void rect_open (iImage * dest, int x, int y, int end_x, int end_y, int r,
		int g, int b);


void cbox (iImage * dest, int x, int y, int end_x, int end_y, int r, int g,
	   int b);
void dbox (iImage * dest, int x, int y, int end_x, int end_y, int r, int g,
	   int b);



//only works if you have the text loaded and initialized
//void blit_text(iImage *dest, char *c1, int posx,int posy, int r, int g, int b);
void clear_image (iImage * dest);
#endif
