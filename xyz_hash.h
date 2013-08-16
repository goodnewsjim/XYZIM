	 /*
	 Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/


#ifndef __XYZ_HASH__H__
#define __XYZ_HASH__H__

//nclude "cssysdef.h"
//#include "cssys/sysfunc.h"
#include "iutil/vfs.h"
#include "csutil/cscolor.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
//#include "simple2.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/csinput.h"
#include "iutil/virtclk.h"
#include "iengine/sector.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/light.h"
//#include "iengine/statlght.h"
#include "iengine/texture.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/material.h"

//#include "imesh/thing/polygon.h"
//#include "imesh/thing/thing.h"
#include "imesh/sprite3d.h"
#include "imesh/object.h"






#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"
#include "ivideo/txtmgr.h"
#include "ivideo/texture.h"
#include "ivideo/material.h"
#include "ivideo/fontserv.h"
#include "igraphic/imageio.h"
//#include "imap/parser.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "csutil/cmdhelp.h"



#include "csgeom/transfrm.h"

#include "ivaria/conin.h"
#include "ivaria/conout.h"


#include "iengine/campos.h"
//#include "csengine/campos.h"


#include "igraphic/image.h"
//#include "xyz_anim.h"

//This hashtable hashes off the last letter of the name
//so you got 26 bins
#define BINZ 26
//less is better for meshes with less meshes probably! Hashing is like alchemy sometimes.


//Not to mention I only use the last letter to hash, there are WAAAY better schemes to use :)
//See whats best for your ap if you care.  I can give odds you'll find something more
//effective.

class HASHNODE
{
public:
char *name;
csRef<iMeshWrapper> MW;


//Here's the only serious weak spot of my code...
//You need to store the original (position and rotational matrix) of each meshwrapper...
//You need to store this original position so a proper increment can occur during the animation...
//For some reason I decided to put this data here.
//
// Position and rotational matrix should get reset each time an animation hits the next step.
csMatrix3 m;
csVector3 v;



HASHNODE *next;
HASHNODE *prev;


//destructor needed

~HASHNODE(){/*printf(name);printf("before\n")*/if(MW!=NULL) MW=NULL;if(name!=NULL) delete name;/*printf("after\n");*/}
HASHNODE(){name=NULL; MW=NULL;}

};


class XYZ_HASH
{

private:
HASHNODE *remove(int hash_index,const char *c1);
HASHNODE *insert(int hash_index,const char *c1);
HASHNODE *locate(int hash_index,const char *c1);

HASHNODE *Bin[BINZ];



public:

//-1 during no animations 0+ the time elapsed
float showing_animation;
//which step the animation is on.
int cur_step;

//ANIM_XYZ *test;



int add(const char *c1,iMeshWrapper *MW);
int del(const char *c1);
iMeshWrapper *find(const char *c1);

HASHNODE *find_node(const char *c1);
void print(int index);
//new func, but simple
void print_all(void);

void hash_hierarchy(iMeshWrapper *top);

void del_all(void);
XYZ_HASH();
//DELETE ALL NODES(code later)
~XYZ_HASH(){del_all();}

};



#endif
