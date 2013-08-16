	 /*
	 Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/

#ifndef __XYZ_ANIM_H__
#define __XYZ_ANIM_H__

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

#include "xyz_hash.h"
//Top class -ANIMATION (one animation)
//             uses x ANIM_FRAMEs (each frame of animation)
//                  which uses y ANIM_STEPs (each part movement)



class ANIM_STEP
{
//meshwrapper name-links through ANIM_HASH to retrieve iMeshWrapper * to the hierarchy :)
//
//If you creatively build your ANIM_HASH with names that match an animation
//But you link the iMeshWrapper* to a different hierarchy, you can apply animations
//to meshes you didn't design the mesh to work with...  This is just scratching
//the surface, trippy effects you could do.
char *name;

public:
int is_v;  //-1 no vector, 1 yes position vector
csVector3 pos; //Position
int is_m; //-1 no matrix, 1 yes matrix rotation
csMatrix3 rot; //Rotation
~ANIM_STEP(){if(name!=NULL) delete(name);}
//constructor not really called since
ANIM_STEP(){name=NULL;}

void set_name(char *c1);
char *get_name(void);



//dostep

};

class ANIM_FRAME
{
private:
//array of pointers to animsteps

public:
int time;
int steps;
ANIM_STEP **AS;

//initializes ANIM_STEPS
ANIM_FRAME(int time, int steps);
//void set(int step_number,ANIM_STEP *AS);
~ANIM_FRAME(){int i; for(i=0;i<steps;i++) if(AS[i]!=NULL) delete AS[i]; delete AS;}


//time is how much into this frame it is
void do_frame(float time0,XYZ_HASH *H);

};


class ANIM_XYZ
{
//not really that useful, but you can use it to find
//which hierarchical mesh in the engine this anim can be applied to
//This is the top mesh, parent of all.
public:

char *main_mesh_name;

//This is what the animation is named, maybe helpful with indexing.
char *animation_name;

//total number of frames.
int max_frames;

ANIM_FRAME **AF;

//initializes ANIM_STEPS
ANIM_XYZ(int frames);
//void set(int step_number,ANIM_STEP *AS);
~ANIM_XYZ(){int i; for(i=0;i<max_frames;i++) if(AF[i]!=NULL) delete AF[i]; delete AF;}

//loads up an animation and puts data in, most common method :)
ANIM_XYZ(iFile *F1);

//You supply the total time of this animation.
//Returns (#1) a pointer to an Animation frame
//And (#2) also mods your time, so you get how far into this frame you are.
ANIM_FRAME *get_frame(float &time);

int get_frame_number(float time);




void update_hash_model(float time, XYZ_HASH *H0);



};


/* CODE I USED TO TEST :)
ANIM_XYZ  XYZ(1);
ANIM_FRAME *FRAME=new ANIM_FRAME(1,2);
ANIM_STEP *STEP1,*STEP2;

STEP1=new(ANIM_STEP);
STEP2=new(ANIM_STEP);

STEP1->set_name("hey");
STEP2->set_name("hey2");

FRAME->AS[0]=STEP1;
FRAME->AS[1]=STEP2;

XYZ.AF[0]=FRAME;


printf(XYZ.AF[0]->AS[0]->get_name());
printf("\n");
printf(XYZ.AF[0]->AS[1]->get_name());
printf("\n");
*/

#endif
