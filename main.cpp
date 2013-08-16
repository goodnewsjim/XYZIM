	 /*
	 Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/

/*
 * main.cpp
 *
 * Main entry point for xyzim.
 */

#include "xyzim.h"
#include "appxyzim.h"
#include <csutil/sysfunc.h> // Provides csPrintf()

CS_IMPLEMENT_APPLICATION

int main(int argc, char** argv)
{
  csPrintf ("xyzim version 1.0 by Jim Sager.\n");
  csPrintf ("If you want to do 3d code: www.crystalspace3d.org \n");
  csPrintf ("Use:\n");
  csPrintf ("Click on any bone.  If you hold the left mouse down, you can spin around the bone and its children.  If you hold both buttons down, you can move the bone.  Since the mouse movement is strange there are keys too.\n");
  csPrintf ("\n");
  csPrintf ("Keyboard keys are(after you select a bone):\n");
  csPrintf ("f,r,e,d,c,q,a,z,w,x\n");
  csPrintf ("If you hold shift, the key press moves opposite.\n");
  csPrintf ("\n");
  csPrintf ("If you hold ctrl, the keys move slower.\n");
  csPrintf ("\n");
  csPrintf ("Home = undo\n");
  csPrintf ("End = redo\n");
  csPrintf ("\n");
  csPrintf ("Function keys\n");
  csPrintf ("f6-Takes a snapshot of the skeleton.  To understand this, take a shot of the skeleton, then move a bone, then take another shot of the skeleton. Close both windows\n");
  csPrintf ("f1- Plays the animation.  If you have two frames taken, you can play the animation.\n");
  csPrintf ("f5- Menu\n");
  csPrintf ("f8- Save animation\n");
  csPrintf ("f7- Load animation\n");
  csPrintf ("f4- Load skeleton\n");
  csPrintf ("f3- Save akeleton\n");
  csPrintf ("\n");
  csPrintf ("Extra information:\n");
  csPrintf ("With the window you get from pressing f6, you can change the speed at which the animation plays by changing the 1000 to a different number of ms.\n");

  /* Runs the application.  
   *
   * csApplicationRunner<> cares about creating an application instance
   * which will perform initialization and event handling for the entire game.
   *
   * The underlying csApplicationFramework also performs some core
   * initialization.  It will set up the configuration manager, event queue,
   * object registry, and much more.  The object registry is very important,
   * and it is stored in your main application class (again, by
   * csApplicationFramework).
   */
  return csApplicationRunner<AppXyzim>::Run (argc, argv);
}
