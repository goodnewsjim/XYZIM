/*
 * appxyzim.cpp
 *
 * Definition of AppXyzim, the main application object.
Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/

#include "appxyzim.h"
#ifdef USE_CEL
#include <celtool/initapp.h>
#endif

#include "universals.h"
#include "iImageHelper.h"
//#include "TextHelper.h"
#include "iFileHelper.h"
#include "xyz_anim.h"
#include "xyz_hash.h"

my_mouse M;
iMeshWrapper *sg1=0;
iMeshWrapper *sg2=0;
//globals for undo:
int undo_pos=0;
int max_undo=0;
//globals for animation frames
int Aframe_time[1000];
csPixmap *pixmap;
iTextureManager* txtmgr=0;

//if this is 1, then screenshot going into a popup
int aquire_screen_shot=0;
//the popup the screen_shot is getting inserted into
popup *which_ss;

//BIGJUMP
XYZ_HASH *model=NULL;
ANIM_XYZ *XYZ=NULL;

//MORE UGLY ASS CODE:
//globals to save

int SA_max;

//meshobj name
char SA_c1[1000][200];
//corresponding vector
csVector3 SA_v[1000];
//corresponding matrix
csMatrix3 SA_m[1000];



//MORE UGLY GLOBALS- TO show all frames of animation(can crush weak systems)
int show_all_frames=-1;

//last 2 ugly globals.
int last_frame=9999;

int ugly_global_step;

char def_save_skel_dir[200];
char def_save_anim_dir[200];
char def_load_skel_dir[200];
char def_load_anim_dir[200];
char def_save_skel_file[200];
char def_save_anim_file[200];
char def_load_skel_file[200];
char def_load_anim_file[200];


// Convenience function to get a csVector3 from a config file.
static csVector3 GetConfigVector (csConfigAccess& config,
        const char* key, const char* def)
{
  csVector3 v;
  csScanStr (config->GetStr (key, def), "%f,%f,%f", &v.x, &v.y, &v.z);
  return v;
}

AppXyzim::AppXyzim() : csApplicationFramework()
{
  SetApplicationName("xyzim");
}

AppXyzim::~AppXyzim()
{
}

void AppXyzim::DoStuffOncePerFrame()
{

  csTicks elapsed_time = vc->GetElapsedTicks ();

  // Now rotate the camera according to keyboard state
  float speed = (elapsed_time / 1000.0) * (0.03 * 20);
  float elapsed_time2=elapsed_time;


  iCamera* c = view->GetCamera();
  
  int ok=1;
if (kbd->GetKeyState (CSKEY_F2))
{
//printf("hmmm\n");
delete(model);
delete(XYZ);
model=NULL;
XYZ=NULL;

}//end of esc


  if(model!=NULL)
  if(model->showing_animation>-1 || model->showing_animation==-2){ ok=0;}
//printf("12312\n");
  if(ok==1)
  {
check_input3();
return;
}



//if(model!=NULL)
//printf("not null\n");
//if(model!=NULL)
//printf("animation showing:%f\n",model->showing_animation);

  if(model!=NULL)
  if(model->showing_animation > -1.0)
  {
//printf("heeee\n");
//use ugly_global_step
if(DBG==1) printf("here1\n");

model->showing_animation=model->showing_animation+elapsed_time2 / 1000.0;
printf("here1a\n");

//temp time
float temp;
temp=model->showing_animation;
printf("here1b\n");

XYZ->update_hash_model(temp,model);
printf("here2\n");

int aaa=XYZ->get_frame_number(temp);

ANIM_FRAME *f=XYZ->get_frame(temp);
//printf("SA t %f,%f\n",showing_animation,temp);

//says this:
printf("here3\n");


if(f==NULL)
{
printf("reseting\n");
model->showing_animation=0;
model->cur_step=0;
//ANIM_FRAME *f=XYZ->get_frame(temp);
//f->do_frame(temp,model);
//aaa=0;
}
else
f->do_frame(temp,model);
printf("here4\n");


printf("aaa%d\n",aaa);
printf("last_frame %d\n",last_frame);

if(aaa>=last_frame || (aaa==-1 && last_frame< 1000) )
{
model->showing_animation=-1;
model->cur_step=0;
delete(model);
delete(XYZ);
model=NULL;
XYZ=NULL;
}

if (kbd->GetKeyState (CSKEY_ESC))
{
delete(model);
delete(XYZ);
model=NULL;
XYZ=NULL;

}//end of esc

printf("here5\n");


  }



  if(model!=NULL)
  if(model->showing_animation==-2)
  {


int j;
HASHNODE *H;
char *c0;
for(j=0;j<XYZ->AF[ugly_global_step]->steps;j++)
{
c0=XYZ->AF[ugly_global_step]->AS[j]->get_name();
H=model->find_node(c0);

if(XYZ->AF[ugly_global_step]->AS[j]->is_v==1)
{
H->MW->GetMovable()->SetPosition(XYZ->AF[ugly_global_step]->AS[j]->pos);
H->MW->GetMovable()->UpdateMove();
}

if(XYZ->AF[ugly_global_step]->AS[j]->is_m==1)
{
H->MW->GetMovable()->SetTransform(XYZ->AF[ugly_global_step]->AS[j]->rot);
H->MW->GetMovable()->UpdateMove();
}

}
int af=new_aframe();
Aframe_time[af]=XYZ->AF[ugly_global_step]->time;
char c1[200];
sprintf(c1,"%d",af);
save_skeleton2("animations/working",c1);

ugly_global_step++;
printf("max frames %d\n",XYZ->max_frames);
if(ugly_global_step>(XYZ->max_frames-1) )
{


printf("hmmm2\n");
delete(model);
delete(XYZ);
model=NULL;
XYZ=NULL;
show_all_frames=1;
}
//maybe do some deletions and model





  }

}


void AppXyzim::check_input3()
{
  static int firsttime = 1;
  //int x;
  static int om1 = 0, om2 = 0;
  bool mousebuttonup = false;
  bool mousebuttondown = false;
  static bool oldkey_leftbrace;
  static bool oldkey_rightbrace;
  iMeshWrapper *temp_wrap = 0;
  static int mlplace = 0;
  static int lastmx,lastmy;
  static int drag = 0;
  static int faked = 0;
  static csVector3 fake_position1, fake_position2;
  static int press=0,lastpress=0;

lastpress=press;
press=0;
  check_input();

  M.x = mos->GetLastX (0);
  M.y = mos->GetLastY (0);
  M.button1 = 0;
  M.button2 = 0;
  if (mos->GetLastButton (0,0))
    M.button1 = 1;
  if (mos->GetLastButton (0,1))
    M.button2=1;

  if((om1 == 0 && M.button1 == 1) || (om2 == 0 && M.button2 == 1))
  {
    mousebuttondown=true;
if(M.button1 == 0 && M.button2 == 0)
{
P.losefocus();

if(drag==1)
{
saveundo();
}
drag=0;
}
    //printf("buttondown\n");
  }
  if((om1 == 1 && M.button1 == 0) || (om2 == 1 && M.button2 == 0))
  {
    mousebuttonup=true;
if(M.button1 == 0 && M.button2 == 0)
{
P.losefocus();

if(drag==1)
{
saveundo();

}
drag=0;
}
    //printf("buttonup\n");
  }

P.check_popups();


if(P.focus!=NULL)
{
sg1=NULL;
check_expand_string("");

}


if(P.EV_window!=NULL)
{
handle_popup_events();
//printf("test33\n");
P.clear_events();
//printf("test44\n");

}
else
{


  if(mousebuttondown)
  {
    csVector3 v;
    //M.x is Mouse x, M.y is Mouse y
    csVector2 p (M.x, M.y);
    //view->GetCamera()->InvPerspective(p,1.0);
    temp_wrap=csEngineTools::FindScreenTarget(p,400,view->GetCamera(),0).mesh;

    if(temp_wrap!=NULL)
    {
      iObject *O1=temp_wrap->QueryObject();
      char c1[500];
      strcpy(c1,O1->GetName());
      printf(c1);
      printf("\n");
    }
  }
//  drag=1;

  //printf("X: %d\n",M.button2);
  if(kbd->GetKeyState ('['))
  {
    if(oldkey_leftbrace)
    {}
    else
    {
      int mlmax;
      iMeshList *ml;
      ml = engine->GetMeshes();
      mlmax = ml->GetCount();

      while(temp_wrap==NULL)
      {
        mlplace = mlplace - 1;
        if (mlplace < 0)
        mlplace = mlmax - 1;
        temp_wrap = ml->Get (mlplace);
        iObject *O1 = temp_wrap->QueryObject ();
        char c1[500];
        strcpy(c1,O1->GetName ());
        if (strcmp(c1,"walls") == 0)
          temp_wrap=NULL;
        printf(c1);
        printf("\n");
      }
    }
    oldkey_leftbrace=true;
  }
  else
    oldkey_leftbrace=false;

  if(kbd->GetKeyState (']'))
  {
    if(oldkey_rightbrace)
    {}
    else
    {
      int mlmax;
      iMeshList *ml;
      ml = engine->GetMeshes();
      mlmax = ml->GetCount();

      while(temp_wrap==NULL)
      {
        mlplace = mlplace + 1;
        if (mlplace == mlmax)
          mlplace = 0;
        temp_wrap = ml->Get (mlplace);
        iObject *O1 = temp_wrap->QueryObject ();
        char c1[500];
        strcpy(c1,O1->GetName ());
        if (strcmp(c1,"walls") == 0)
          temp_wrap=NULL;
        printf(c1);
        printf("\n");
      }
    }
    oldkey_rightbrace = true;
  }
  else
    oldkey_rightbrace=false;

  if (temp_wrap != NULL)
  {
    iObject *O1=temp_wrap->QueryObject();
    char c1[500];
    strcpy(c1,O1->GetName ());
    if (strcmp(c1,"walls") == 0)
    {
      temp_wrap = NULL;
      sg1 = NULL;
      paint_all_white();
    }
  }

  if (temp_wrap != NULL)
  {
drag=1;
    sg1 = temp_wrap;
    selected(sg1);
  }

  //printf("Mx %d, My %d\n",M.x,M.y);

  if (sg1 != NULL)
    if(M.button1==1)
    {
      if (M.button2 == 1)
      {
        int FrameHeight;
        iGraphics2D* g2d = g3d->GetDriver2D ();
        FrameHeight = g2d->GetHeight ();
        csVector3 startpos;
        csVector3 relatepos;
        csVector3 oldpos;

        oldpos = sg1->GetMovable ()->GetFullPosition ();
        startpos = oldpos;
        relatepos = sg1->GetMovable ()->GetPosition ();
        oldpos = view->GetCamera ()->GetTransform ().Other2This (oldpos);
        csVector3 newpos;
        // THIS CODE CHANGED FROM OLD REVISION
        //view->GetCamera ()->InvPerspective (csVector2 (M.x, FrameHeight-M.y), oldpos.z, newpos);

        newpos = view->GetCamera ()->InvPerspective (csVector2 (M.x, FrameHeight - M.y), oldpos.z);
        newpos = view->GetCamera ()->GetTransform ().This2Other (newpos);
        newpos = newpos - startpos;
        relatepos = relatepos + newpos;
        sg1->GetMovable ()->SetPosition (csVector3 (relatepos));
        sg1->GetMovable ()->UpdateMove ();

     //   drag = 1;
      }//end of M.button2=1

      if (M.button2 == 0)
      {
        csVector3 fake_math;
        int FrameWidth, FrameHeight;
        iGraphics2D* g2d = g3d->GetDriver2D ();
        FrameWidth = g2d->GetWidth ();
        FrameHeight = g2d->GetHeight ();
        double ax;
        double ay;
        double az;
        csOrthoTransform& camtrans = view->GetCamera ()->GetTransform ();
        const csVector3& v_o2t = camtrans.GetOrigin ();
        ax = v_o2t.x;
        ay = v_o2t.y;
        az = v_o2t.z;
        csVector3 startpos;
        csVector3 relatepos;
        csVector3 oldpos;
        oldpos = sg1->GetMovable ()->GetFullPosition ();
        startpos = oldpos;
        relatepos = sg1->GetMovable()->GetPosition();
        if (faked == 1)
        {
          oldpos = fake_position2;
          relatepos = fake_position1;
        }
        if (faked == 0)
        {
          fake_position2 = oldpos;
          fake_position1 = relatepos;
        }
        oldpos = view->GetCamera ()->GetTransform ().Other2This (oldpos);
        csVector3 newpos;
        newpos = view->GetCamera ()->InvPerspective (csVector2 (M.x, FrameHeight - M.y), oldpos.z);
        newpos = view->GetCamera ()->GetTransform ().This2Other (newpos);
        //printf("Old fake pos1/2 (%2.5f,%2.5f,%2.5f) (%2.5f,%2.5f,%2.5f) \n",fake_position1.x,fake_position1.y,fake_position1.z,fake_position2.x,fake_position2.y,fake_position2.z);
        fake_math = fake_position2 - newpos;
        fake_position2 = newpos;
        newpos = newpos - startpos;
        relatepos = relatepos + newpos;
        ax = fake_math.x;
        ay = fake_math.y;
        az = fake_math.z;
        //printf("AX,AY,AZ (%2.2f,%2.2f,%2.2f)\n",ax,ay,az);
        csVector3 v0 = csVector3 (ax, ay, az);
       if( v0.x == 0 && v0.y == 0 && v0.z == 0)
       {
       }
       else
       {
         if (!kbd->GetKeyState (CSKEY_CTRL))
         {
//1.1          if (v0.x != 0)
//1.1             sg1->GetMovable ()->GetTransform ().RotateOther (csVector3 (1, 0, 0), v0.x * 2);
//1.1           if (v0.z != 0)
//1.1             sg1->GetMovable ()->GetTransform ().RotateOther (CS_VEC_ROT_RIGHT, v0.z * 2);
//1.1           if (v0.y != 0)
//1.1             sg1->GetMovable ()->GetTransform ().RotateOther (csVector3 (0, 0, -1), v0.y * 2);
         }
         else
         {
 //1.1          v0 = csVector3 (ax, az, -ay);
 //1.1          sg1->GetMovable ()->GetTransform ().RotateThis (v0, 0.13f);
         }
       }
//1.1       sg1->GetMovable ()->UpdateMove ();
       fake_position1 = relatepos;
//       drag = 1;
       faked = 1;
    } 
    else
      faked=0;
//WHY 2 faked?
  }
  else
    faked=0;
}//end of popup checking

  if (sg1 != NULL)
    if (sg2 != NULL)
    {
      if (sg2 == sg1)
      {
        sg1 = NULL;
        sg2 = NULL;
        paint_all_white ();
      }
      else
      {
        saveundo();
        

        printf("save undo\n");
        connect_mesh2 ();
      }
    }

  // First get elapsed time from the virtual clock.
  csTicks elapsed_time = vc->GetElapsedTicks ();
  // Now rotate the camera according to keyboard state
  float speed = (elapsed_time / 1000.0) * (0.06 * 20);

  iCamera* c = view->GetCamera();

  if (kbd->GetKeyState (CSKEY_ALT))
  {
    // If the user is holding down alt, the arrow keys will cause
    // the camera to strafe up, down, left or right from it's
    // current position.
    if (kbd->GetKeyState (CSKEY_RIGHT))
      c->Move (CS_VEC_RIGHT * 8 * speed);
    if (kbd->GetKeyState (CSKEY_LEFT))
      c->Move (CS_VEC_LEFT * 8 * speed);
    if (kbd->GetKeyState (CSKEY_UP))
      c->Move (CS_VEC_UP * 8 * speed);
    if (kbd->GetKeyState (CSKEY_DOWN))
      c->Move (CS_VEC_DOWN * 8 * speed);
  }
  else
  {
    // left and right cause the camera to rotate on the global Y
    // axis; page up and page down cause the camera to rotate on the
    // _camera's_ X axis (more on this in a second) and up and down
    // arrows cause the camera to go forwards and backwards.
    if (kbd->GetKeyState (CSKEY_RIGHT))
      rotY += speed;
    if (kbd->GetKeyState (CSKEY_LEFT))
      rotY -= speed;
    if (kbd->GetKeyState (CSKEY_PGUP))
      rotX += speed;
    if (kbd->GetKeyState (CSKEY_PGDN))
      rotX -= speed;
    if (kbd->GetKeyState (CSKEY_UP))
      c->Move (CS_VEC_FORWARD * 4 * speed);
    if (kbd->GetKeyState (CSKEY_DOWN))
      c->Move (CS_VEC_BACKWARD * 4 * speed);
  }


if (kbd->GetKeyState (CSKEY_ENTER))
{
//SOmeone hit enter so they want to put data in top function
//P.gainfocus();
}

if (kbd->GetKeyState (CSKEY_F5))
{
popup *which;
char c1[200];

which=P.open(11);

if(which!=NULL)
{int max;
max=count_aframe();

sprintf(c1,"-%d Frames-",max);

which->G3[0]->set_text(c1);
P.insert_guts (which);
}


}
if (kbd->GetKeyState (CSKEY_F8))
{
int iii3;
iii3=count_aframe();
if(lastpress==0)
{

printf("dbg1\n");
if(iii3==0)
P.perror("No frames of animation to save.");
else
{
saveundo();

printf("before popup open\n");
popup *p=P.open(9);
printf("After popup open\n");
if(p!=NULL)
{
strcpy(p->G3[2]->e.c1,def_save_anim_dir);
strcpy(p->G3[3]->e.c1,def_save_anim_file);
P.insert_guts (p);
}
printf("After popup guts\n");

}
}
press=1;


}


if (kbd->GetKeyState (CSKEY_F7))
{
printf("dbg2\n");
if(lastpress==0)
{

popup *p=P.open(12);
if(p!=NULL)
{
strcpy(p->G3[2]->e.c1,def_load_anim_dir);
strcpy(p->G3[3]->e.c1,def_load_anim_file);
P.insert_guts (p);
}
}
printf("dbg3\n");
press=1;
}

if (kbd->GetKeyState (CSKEY_F4))
{
popup *p=P.open(3);
if(p!=NULL)
{
strcpy(p->G3[2]->e.c1,def_save_skel_dir);
strcpy(p->G3[3]->e.c1,def_save_skel_file);
P.insert_guts (p);
}

}
if (kbd->GetKeyState (CSKEY_F3))
{
popup *p=P.open(6);
if(p!=NULL)
{
strcpy(p->G3[2]->e.c1,def_load_skel_dir);
strcpy(p->G3[3]->e.c1,def_load_skel_file);
P.insert_guts (p);
}
}

if (kbd->GetKeyState (CSKEY_F1))
{
//printf("dddone\n");

play_animation(0,9999);

}

if (kbd->GetKeyState (CSKEY_F6))
{
if(lastpress==0)
{
int af;
char c1[20];
which_ss=P.open(7);
aquire_screen_shot=1;
af=new_aframe();
Aframe_time[af]=1000;

sprintf(c1,"%d",af);

strcpy(which_ss->G3[2]->e.c1,c1);
sprintf(c1,"%d",Aframe_time[af]);
strcpy(which_ss->G3[3]->e.c1,c1);
which_ss->mdata[0]=af;

sprintf(c1,"%d",af);
printf(c1);
printf("saving\n");
save_skeleton2("animations/working",c1);
save_animation("undo","undo");

}
press =1;

}









  // We now assign a new rotation transformation to the camera.  You
  // can think of the rotation this way: starting from the zero
  // position, you first rotate "rotY" radians on your Y axis to get
  // the first rotation.  From there you rotate "rotX" radians on the
  // your X axis to get the final rotation.  We multiply the
  // individual rotations on each axis together to get a single
  // rotation matrix.  The rotations are applied in right to left
  // order .
  if (firsttime == 1)
  {
    rotY = -1.57f;
    firsttime = 0;
  }
  //printf("%lf,%lf \n",rotX,rotY);

  csMatrix3 rot = csXRotMatrix3 (rotX) * csYRotMatrix3 (rotY);
  csOrthoTransform ot (rot, c->GetTransform().GetOrigin ());
  c->SetTransform (ot);

  //c->GetTransform().RotateThis(CS_VEC_ROT_LEFT,1.57);

  om1 = M.button1;
  om2 = M.button2;
  lastmx = M.x;
  lastmy = M.y;

  /* 
  g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_2DGRAPHICS);
  pixmap->DrawScaled (g3d, 0, 0, 100, 100);
  */


/*UNCOMMENT to show window
  static int suck=-1;
  popup *p;
  while (suck < 0)
  {
    p = P.open (1);
    p->rename ("TEST WIN");
    suck++;
  }
*/
  
  

  //g3d->FinishDraw();
  //g3d->Print(0);
//printf("test55\n");
} // DoStuffOncePerFrame()

//vknecht totally reformatted my code so bitmaps can be displayed.
void AppXyzim::Frame()
{
  /*
    Note: if you use CEL, you probably don't want to call DoStuffOncePerFrame()
          nor g3d->BeginDraw() and use the entity/propclass system.
          This part (similar to simpmap tutorial) is just there as a kickstart.
  */
  DoStuffOncePerFrame ();

  if (g3d->BeginDraw(engine->GetBeginDrawFlags() | CSDRAW_3DGRAPHICS))
  {
    // Draw frame.
#ifdef USE_CEL
    // When using CEL, the entity system takes care of drawing,
    // so there's no need to call iView::Draw() here.
    // That template doesn't setup any CEL entity, so we have to call it now.
    // Once you have your entities set, remove it.
    view->Draw ();
#else
    view->Draw ();
#endif
  }

  g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_2DGRAPHICS);
//  pixmap->DrawScaled (g3d, 0, 0, 100, 100);
if(aquire_screen_shot==0)
P.show_windows(); 


if(aquire_screen_shot==1)
{

csImageManipulate csim;
csRef<iImage> SS;
csRef<iImage> S2;
  iGraphics2D* g2d = g3d->GetDriver2D ();
which_ss->display=P.make_outline(which_ss);
SS=g2d->ScreenShot();
S2=I_nothing->GetMipmap(0);

S2=csim.Rescale(S2,540,380);
//S2->Rescale(540,380);
blit(SS,S2,100,100,0,0,630,470);
aquire_screen_shot=0;
S2=csim.Rescale(S2,500,380);
//S2->Rescale(400,200);
//blit(S2,which_ss->display,50,0,10,16,00,280);
blit(S2,which_ss->display,50,0,10,16,320,270);
P.insert_guts(which_ss);
SS=NULL;
S2=NULL;
}



}

bool AppXyzim::OnKeyboard(iEvent& ev)
{
  // We got a keyboard event.
  if (csKeyEventHelper::GetEventType(&ev) == csKeyEventTypeDown)
  {
    // The user pressed a key (as opposed to releasing it).
    utf32_char code = csKeyEventHelper::GetCookedCode(&ev);
    if (code == CSKEY_ESC)
    {
      // The user pressed escape, so terminate the application.  The proper way
      // to terminate a Crystal Space application is by broadcasting a
      // csevQuit event.  That will cause the main run loop to stop.  To do
      // so we retrieve the event queue from the object registry and then post
      // the event.
//      csRef<iEventQueue> q =
//        csQueryRegistry<iEventQueue> (GetObjectRegistry());
//      if (q.IsValid())
//        q->GetEventOutlet()->Broadcast(csevQuit(GetObjectRegistry)));
    }
  }
  return false;
}

bool AppXyzim::OnInitialize(int argc, char* argv[])
{
  iObjectRegistry* r = GetObjectRegistry();

  // Load application-specific configuration file.
  if (!csInitializer::SetupConfigManager(r,
      "/xyzim/AppXyzim.cfg", GetApplicationName()))
    return ReportError("Failed to initialize configuration manager!");

#ifdef USE_CEL
  celInitializer::SetupCelPluginDirs(r);
#endif

  // RequestPlugins() will load all plugins we specify.  In addition it will
  // also check if there are plugins that need to be loaded from the
  // configuration system (both the application configuration and CS or global
  // configurations).  It also supports specifying plugins on the command line
  // via the --plugin= option.
  if (!csInitializer::RequestPlugins(r,
	CS_REQUEST_VFS,
	CS_REQUEST_OPENGL3D,
	CS_REQUEST_ENGINE,
	CS_REQUEST_FONTSERVER,
	CS_REQUEST_IMAGELOADER,
	CS_REQUEST_LEVELLOADER,
	CS_REQUEST_REPORTER,
	CS_REQUEST_REPORTERLISTENER,
	CS_REQUEST_PLUGIN("crystalspace.collisiondetection.opcode",
		iCollideSystem),
	CS_REQUEST_END))
    return ReportError("Failed to initialize plugins!");

  // "Warm up" the event handler so it can interact with the world
  csBaseEventHandler::Initialize(GetObjectRegistry());
 
  // Set up an event handler for the application.  Crystal Space is fully
  // event-driven.  Everything (except for this initialization) happens in
  // response to an event.
  if (!RegisterQueue (r, csevAllEvents(GetObjectRegistry())))
    return ReportError("Failed to set up event handler!");

  return true;
}

void AppXyzim::OnExit()
{
  printer.Invalidate();
}

bool AppXyzim::Application()
{
  iObjectRegistry* r = GetObjectRegistry();

  // Open the main system. This will open all the previously loaded plugins
  // (i.e. all windows will be opened).
  if (!OpenApplication(r))
    return ReportError("Error opening system!");

  // Now get the pointer to various modules we need.  We fetch them from the
  // object registry.  The RequestPlugins() call we did earlier registered all
  // loaded plugins with the object registry.  It is also possible to load
  // plugins manually on-demand.
  g3d = csQueryRegistry<iGraphics3D> (r);
  if (!g3d)
    return ReportError("Failed to locate 3D renderer!");
gut_g3d=g3d;
  engine = csQueryRegistry<iEngine> (r);
  if (!engine)
    return ReportError("Failed to locate 3D engine!");
    
  printer.AttachNew(new FramePrinter(GetObjectRegistry()));

  vc = csQueryRegistry<iVirtualClock> (r);
  if (!vc)
    return ReportError ("Failed to locate Virtual Clock!");

  kbd = csQueryRegistry<iKeyboardDriver> (r);
  if (!kbd)
    return ReportError ("Failed to locate Keyboard Driver!");

  loader = csQueryRegistry<iLoader> (r);
  if (!loader)
    return ReportError("Failed to locate map loader plugin!");

  cdsys = csQueryRegistry<iCollideSystem> (r);
  if (!cdsys)
    return ReportError ("Failed to locate CD system!");

  cfgmgr = csQueryRegistry<iConfigManager> (r);
  if (!cfgmgr)
    return ReportError ("Failed to locate the configuration manager plugin!");

  mos = csQueryRegistry<iMouseDriver> (r);
  if (!mos)
    return ReportError ("Failed to locate Mouse' Driver!");

  //undo
  FS = csQueryRegistry<iVFS> (r);
  //JD =
  if (!FS)
    return ReportError("Failed to locate VFS!");

  // We need a View to the virtual world.
  view.AttachNew(new csView (engine, g3d));
  iGraphics2D* g2d = g3d->GetDriver2D ();
  // We use the full window to draw the world.
  view->SetRectangle (0, 0, g2d->GetWidth (), g2d->GetHeight ());

  // Set the window title.
  iNativeWindow* nw = g2d->GetNativeWindow ();
  if (nw)
    nw->SetTitle (cfgmgr->GetStr ("WindowTitle",
          "Please set WindowTitle in AppXyzim.cfg"));
  /*
  if (!LoadMap (cfgmgr->GetStr ("DefaultMap", "/appdata/defaultmap")))
    return ReportError("Failed to load map!");

  if (!PostLoadMap ())
    return ReportError ("Error during PostLoadMap()!");
  */

  // First disable the lighting cache. Our app is simple enough
  // not to need this.
  engine->SetLightingCacheMode (0);
  
  // Here we create our world.
  CreateRoom();
  
  // Let the engine prepare all lightmaps for use and also free all images 
  // that were loaded for the texture manager.
  engine->Prepare ();
  
  // these are used store the current orientation of the camera
  rotY = rotX = 0;
  
  // Now we need to position the camera in our world.
  view->GetCamera ()->SetSector (sector);
  view->GetCamera ()->GetTransform ().SetOrigin (csVector3 (8, 3, 12));
  
  iCamera *tempcam;
  csOrthoTransform temptrans;
  tempcam = view->GetCamera ();
  temptrans = tempcam->GetTransform();
  temptrans.RotateThis (CS_VEC_ROT_LEFT, 1.57);
  tempcam->SetTransform (temptrans);
  //printf("hello\n");
  view->GetCamera ()->GetTransform ().RotateThis (CS_VEC_ROT_LEFT,1.57);
  //tempcam->Move (CS_VEC_LEFT * 12);
  //  csMatrix3 rot = csXRotMatrix3 (0) * csYRotMatrix3 (-1.57);
  //  csOrthoTransform ot (rot, tempcam->GetTransform().GetOrigin ());

  //  csMatrix3 rot = csXRotMatrix3 (20.54) * csYRotMatrix3 (1);
  //  csOrthoTransform ot (rot, tempcam->GetTransform().GetOrigin ());
  //  tempcam->SetTransform (ot);
  view->SetCamera (tempcam);

  txtmgr = g3d->GetTextureManager ();

  csRef < iTextureHandle > txt2;

  txt2 = txtmgr->RegisterTexture (xbut, CS_TEXTURE_2D);
  pixmap = new csSimplePixmap (txt2);

  // We use some other "helper" event handlers to handle 
  // pushing our work into the 3D engine and rendering it
  // to the screen.
  /*
  drawer.AttachNew(new FrameBegin3DDraw (GetObjectRegistry (), view));
  printer.AttachNew(new FramePrinter (GetObjectRegistry ()));
  */


  // Start the default run/event loop.  This will return only when some code,
  // such as OnKeyboard(), has asked the run loop to terminate.
  Run();

  return true;
}

bool AppXyzim::LoadMap (const char* path)
{
  // Set VFS current directory to the level we want to load.
  csRef<iVFS> VFS (csQueryRegistry<iVFS> (GetObjectRegistry ()));
  VFS->PushDir (path);
  // Load the level file which is called 'world'.
  if (!loader->LoadMapFile ("world"))
  {
    VFS->PopDir ();
    return ReportError("Error couldn't load level file %s/world!", path);
  }
  VFS->PopDir ();

  return true;
}

bool AppXyzim::PostLoadMap ()
{
  // Initialize collision objects for all loaded objects.
  csColliderHelper::InitializeCollisionWrappers (cdsys, engine);

  engine->Prepare ();

  // Creates an accessor for configuration settings.
  csConfigAccess cfgAcc (GetObjectRegistry ());

  // Find the starting position in this level.
  csVector3 pos (0);
  int nbStartPositions = engine->GetCameraPositions ()->GetCount ();
  if (nbStartPositions > 0)
  {
    // There is a valid starting position defined in the level file.
    iCameraPosition* campos = engine->GetCameraPositions ()->Get (0);
    sector = engine->GetSectors ()->FindByName (campos->GetSector ());
    pos = campos->GetPosition ();
    csPrintf ("\nFound %d starting position%s, using the first one:\n"
          "name: %s, sector: %s\n",
          nbStartPositions, nbStartPositions > 1 ? "s" : "",
          campos->QueryObject ()->GetName (), campos->GetSector ());
  }
  else
  {
    // We didn't find a valid starting position,
    // so we default to going to the first sector at position (0,2,0).
    sector = engine->GetSectors ()->Get (0);
    pos = csVector3 (GetConfigVector (cfgAcc, "DefaultStartPos", "0,2,0"));
    ReportWarning (
          "Didn't find any starting position, using %s in sector '%s'",
          pos.Description ().GetData (), sector->QueryObject ()->GetName ());
  }
  if (!sector)
    return ReportError("Couldn't find a valid sector in PostLoadMap()!");

  // Now we need to position the camera in our world.
  view->GetCamera ()->SetSector (sector);
  view->GetCamera ()->GetTransform ().SetOrigin (pos);

  // Initialize our collider actor.
  collider_actor.SetCollideSystem (cdsys);
  collider_actor.SetEngine (engine);
  // Read the values from config files, and use default values if not set.
  csVector3 legs (GetConfigVector (cfgAcc, "Actor.Legs", "0.2,0.3,0.2"));
  csVector3 body (GetConfigVector (cfgAcc, "Actor.Body", "0.2,1.2,0.2"));
  csVector3 shift (GetConfigVector (cfgAcc, "Actor.Shift", "0.0,-1.0,0.0"));
  collider_actor.InitializeColliders (view->GetCamera (),
        legs, body, shift);

  // Load a mesh factory from a library, then instatiate a mesh from it
  LoadLibrary ("/appdata/defaultmap/libSimpleCube");
  iMeshFactoryWrapper* mfw = engine->FindMeshFactory ("genSimpleCube");
  if (mfw)
  {
    csRef<iMeshWrapper> cube = engine->CreateMeshWrapper (
          mfw, "aSimpleCube", sector, csVector3 (5,1,8));
    // Initialize the collider for this object, since it was created
    // after InitializeCollisionWrappers() call...
    csColliderHelper::InitializeCollisionWrapper (cdsys, cube);
  }
  else
    ReportWarning ("Couldn't find the genSimpleCube mesh factory!");

  return true;
}

bool AppXyzim::LoadLibrary (const char* path)
{
  csRef<iVFS> vfs (csQueryRegistry<iVFS> (GetObjectRegistry ()));
  if (vfs)
  {
    // Set current VFS dir to the level dir, helps with relative paths in maps
    vfs->PushDir (path);
    if (!loader->LoadLibraryFile (path))
    {
      vfs->PopDir ();
      return ReportError("Couldn't load library file %s!", path);
    }
    vfs->PopDir ();
  }
  else
    return ReportError ("Couldn't find VFS plugin!");

  return true;
}

void AppXyzim::CreateRoom ()
{
int i,j;

  // Load the texture from the standard library.  This is located in
  // CS/data/standard.zip and mounted as /lib/std using the Virtual
  // File System (VFS) plugin.
  if (!loader->LoadTexture ("stone", "/lib/std/stone4.gif"))
    ReportError("Error loading 'stone4' texture!");

I_nothing=loader->LoadImage("/this/data/AWT/NOTHING.TGA",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);





xbut=loader->LoadImage("/this/data/AWT/X.TGA",CS_IMGFMT_TRUECOLOR | 
CS_IMGFMT_ALPHA);

x2but=loader->LoadImage("/this/data/AWT/X1.TGA",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

hzbar=loader->LoadImage("/this/data/AWT/HZBAR.TGA",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

hzbar2=loader->LoadImage("/this/data/AWT/HZBAR2.TGA",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

vtbar=loader->LoadImage("/this/data/AWT/VTBAR.TGA",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

grbut=loader->LoadImage("/this/data/AWT/GRBUT.TGA",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

rdbut=loader->LoadImage("/this/data/AWT/RDBUT.TGA",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

cornul=loader->LoadImage("/this/data/AWT/CORNUL.TGA",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

cornll=loader->LoadImage("/this/data/AWT/CORNLL.TGA",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

cornur=loader->LoadImage("/this/data/AWT/CORNUR.TGA",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

cornlr=loader->LoadImage("/this/data/AWT/CORNLR.TGA",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

cornlr=loader->LoadImage("/this/data/AWT/CORNLR.TGA",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

TEXT2=loader->LoadImage("/this/data/AWT/text.tga",CS_IMGFMT_TRUECOLOR);



//LOAD UP the text images









for(i=0;i<95;i++)

I_I[i]=NULL;



I_I[16]=loader->LoadImage("/this/data/AWT/text/0.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[17]=loader->LoadImage("/this/data/AWT/text/1.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[18]=loader->LoadImage("/this/data/AWT/text/2.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[19]=loader->LoadImage("/this/data/AWT/text/3.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[20]=loader->LoadImage("/this/data/AWT/text/4.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[21]=loader->LoadImage("/this/data/AWT/text/5.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[22]=loader->LoadImage("/this/data/AWT/text/6.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[23]=loader->LoadImage("/this/data/AWT/text/7.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[24]=loader->LoadImage("/this/data/AWT/text/8.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[25]=loader->LoadImage("/this/data/AWT/text/9.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);





I_I[0]=loader->LoadImage("/this/data/AWT/text/space.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[1]=loader->LoadImage("/this/data/AWT/text/exclaim.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[2]=loader->LoadImage("/this/data/AWT/text/doublequote.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[3]=loader->LoadImage("/this/data/AWT/text/pound.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[4]=loader->LoadImage("/this/data/AWT/text/dollar.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[5]=loader->LoadImage("/this/data/AWT/text/percent.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[6]=loader->LoadImage("/this/data/AWT/text/and.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[7]=loader->LoadImage("/this/data/AWT/text/rightquote.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[8]=loader->LoadImage("/this/data/AWT/text/leftparen.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[9]=loader->LoadImage("/this/data/AWT/text/rightparen.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[10]=loader->LoadImage("/this/data/AWT/text/asterix.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[11]=loader->LoadImage("/this/data/AWT/text/plus.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[12]=loader->LoadImage("/this/data/AWT/text/comma.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[13]=loader->LoadImage("/this/data/AWT/text/dash.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[14]=loader->LoadImage("/this/data/AWT/text/period.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[15]=loader->LoadImage("/this/data/AWT/text/slash.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[26]=loader->LoadImage("/this/data/AWT/text/colon.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[27]=loader->LoadImage("/this/data/AWT/text/semicolon.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[28]=loader->LoadImage("/this/data/AWT/text/leftcarrot.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[29]=loader->LoadImage("/this/data/AWT/text/equal.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[30]=loader->LoadImage("/this/data/AWT/text/rightcarrot.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[31]=loader->LoadImage("/this/data/AWT/text/question.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[32]=loader->LoadImage("/this/data/AWT/text/at.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[33]=loader->LoadImage("/this/data/AWT/text/Ac.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[34]=loader->LoadImage("/this/data/AWT/text/Bc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[35]=loader->LoadImage("/this/data/AWT/text/Cc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[36]=loader->LoadImage("/this/data/AWT/text/Dc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[37]=loader->LoadImage("/this/data/AWT/text/Ec.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[38]=loader->LoadImage("/this/data/AWT/text/Fc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[39]=loader->LoadImage("/this/data/AWT/text/Gc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[40]=loader->LoadImage("/this/data/AWT/text/Hc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[41]=loader->LoadImage("/this/data/AWT/text/Ic.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[42]=loader->LoadImage("/this/data/AWT/text/Jc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[43]=loader->LoadImage("/this/data/AWT/text/Kc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[44]=loader->LoadImage("/this/data/AWT/text/Lc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[45]=loader->LoadImage("/this/data/AWT/text/Mc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[46]=loader->LoadImage("/this/data/AWT/text/Nc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[47]=loader->LoadImage("/this/data/AWT/text/Oc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[48]=loader->LoadImage("/this/data/AWT/text/Pc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[49]=loader->LoadImage("/this/data/AWT/text/Qc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[50]=loader->LoadImage("/this/data/AWT/text/Rc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[51]=loader->LoadImage("/this/data/AWT/text/Sc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[52]=loader->LoadImage("/this/data/AWT/text/Tc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[53]=loader->LoadImage("/this/data/AWT/text/Uc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[54]=loader->LoadImage("/this/data/AWT/text/Vc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[55]=loader->LoadImage("/this/data/AWT/text/Wc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[56]=loader->LoadImage("/this/data/AWT/text/Xc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[57]=loader->LoadImage("/this/data/AWT/text/Yc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[58]=loader->LoadImage("/this/data/AWT/text/Zc.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[59]=loader->LoadImage("/this/data/AWT/text/leftbracket.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[60]=loader->LoadImage("/this/data/AWT/text/switch.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[61]=loader->LoadImage("/this/data/AWT/text/rightbracket.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[62]=loader->LoadImage("/this/data/AWT/text/carrot.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[63]=loader->LoadImage("/this/data/AWT/text/underline.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[64]=loader->LoadImage("/this/data/AWT/text/leftquote.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[65]=loader->LoadImage("/this/data/AWT/text/a.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[66]=loader->LoadImage("/this/data/AWT/text/b.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[67]=loader->LoadImage("/this/data/AWT/text/c.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[68]=loader->LoadImage("/this/data/AWT/text/d.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[69]=loader->LoadImage("/this/data/AWT/text/e.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[70]=loader->LoadImage("/this/data/AWT/text/f.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[71]=loader->LoadImage("/this/data/AWT/text/g.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[72]=loader->LoadImage("/this/data/AWT/text/h.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[73]=loader->LoadImage("/this/data/AWT/text/i.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[74]=loader->LoadImage("/this/data/AWT/text/j.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[75]=loader->LoadImage("/this/data/AWT/text/k.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[76]=loader->LoadImage("/this/data/AWT/text/l.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[77]=loader->LoadImage("/this/data/AWT/text/m.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[78]=loader->LoadImage("/this/data/AWT/text/n.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[79]=loader->LoadImage("/this/data/AWT/text/o.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[80]=loader->LoadImage("/this/data/AWT/text/p.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[81]=loader->LoadImage("/this/data/AWT/text/q.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[82]=loader->LoadImage("/this/data/AWT/text/r.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[83]=loader->LoadImage("/this/data/AWT/text/s.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[84]=loader->LoadImage("/this/data/AWT/text/t.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[85]=loader->LoadImage("/this/data/AWT/text/u.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[86]=loader->LoadImage("/this/data/AWT/text/v.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[87]=loader->LoadImage("/this/data/AWT/text/w.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[88]=loader->LoadImage("/this/data/AWT/text/x.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[89]=loader->LoadImage("/this/data/AWT/text/y.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[90]=loader->LoadImage("/this/data/AWT/text/z.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[91]=loader->LoadImage("/this/data/AWT/text/leftsquiggly.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[92]=loader->LoadImage("/this/data/AWT/text/bar.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[93]=loader->LoadImage("/this/data/AWT/text/rightsquiggly.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

I_I[94]=loader->LoadImage("/this/data/AWT/text/tilda.tga",CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);

//All text must then manually be converted into having an alpha channel:


for(i=0;i<95;i++)

{

if(I_I[i]!=NULL)

{

TEXT=I_I[i]->GetMipmap(0);

blit_transparent(TEXT,I_I[i],0,0,0,0,0,1);

}

}



//Again since my gimpy skills can't get alpha loaded yet, do ghetto BLACK0,0,0 -> alpha on load.

//TEXT=cornll->Clone();

//blit_transparent(TEXT,cornll,0,0,0,0,0,1);











//background for testing bitmaps

for(i=0;i<TEXT2->GetWidth();i++)

{

for(j=0;j<TEXT2->GetHeight();j++)

{

setred(TEXT2,i,j,244);
setgreen(TEXT2,i,j,0);
setblue(TEXT2,i,j,0);



//Background with text formatting lines

//if((j+5)%22==0)

//setgreen(TEXT2,i,j,125);



}

}





/////////LOAD TEXT



iTextureWrapper* txt = loader->LoadTexture ("white_bone",

  	"/this/data/xyzimation/tools/skins/white_bone.png", CS_TEXTURE_3D, txtmgr, true);



txt = loader->LoadTexture ("blue_bone",

  	"/this/data/xyzimation/tools/skins/blue_bone.png", CS_TEXTURE_3D, txtmgr, true);



txt = loader->LoadTexture ("red_bone",

  	"/this/data/xyzimation/tools/skins/red_bone.png", CS_TEXTURE_3D, txtmgr, true);



txt = loader->LoadTexture ("green_bone",

  	"/this/data/xyzimation/tools/skins/green_bone.png", CS_TEXTURE_3D, txtmgr, true);



txt = loader->LoadTexture ("purple_bone",

  	"/this/data/xyzimation/tools/skins/purple_bone.png", CS_TEXTURE_3D, txtmgr, true);








  iMaterialWrapper* tm = engine->GetMaterialList ()->FindByName ("stone");

  // We create a new sector called "room".
  sector = engine->CreateSector ("room");

  // Creating the walls for our room.
  csRef<iMeshWrapper> walls (engine->CreateSectorWallsMesh (sector, "walls"));
  iMeshObject* walls_object = walls->GetMeshObject ();
  iMeshObjectFactory* walls_factory = walls_object->GetFactory();
  csRef<iThingFactoryState> walls_state = 
    scfQueryInterface<iThingFactoryState> (walls_factory);
  walls_state->AddInsideBox (csVector3 (-20, 2.25, -20), csVector3 (20, 80, 20));
  walls_state->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
  walls_state->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3);

  // Now we need light to see something.
/*
  csRef<iLight> light;
  iLightList* ll = sector->GetLights ();

  light = engine->CreateLight(0, csVector3(-3, 5, 0), 10, csColor(1, 0, 0));
  ll->Add (light);

  light = engine->CreateLight(0, csVector3(3, 5,  0), 10, csColor(0, 0, 1));
  ll->Add (light);

  light = engine->CreateLight(0, csVector3(0, 5, -3), 10, csColor(0, 1, 0));
  ll->Add (light);
*/

for(i=0;i<200;i++)
strcpy(S_mesh[i],"");


strcpy(def_save_skel_dir,"");
strcpy(def_save_anim_dir,"");
strcpy(def_load_skel_dir,"");
strcpy(def_load_anim_dir,"");
strcpy(def_save_skel_file,"");
strcpy(def_save_anim_file,"");
strcpy(def_load_skel_file,"");
strcpy(def_load_anim_file,"");


S_index=0;

loader->LoadMapFile ("/this/data/xyzimation/tools/meshobjects/lights", false);

loader->LoadMapFile ("/this/data/xyzimation/tools/meshobjects/robot_objects", false);


//loader->LoadMapFile ("/this/data/xyzimation/skeletons/undo/1", false);

Load_Plugins("/this/data/xyzimation/tools/meshobjects/robot_objects");


saveundo();


sg1=NULL;
sg2=NULL;
for(i=0;i<1000;i++)
Aframe_time[i]=-1;


}

/* ========================================================================= */

void AppXyzim::selected(iMeshWrapper *MW)
{
  //iObject *O;
  //iMeshList *M2;
  iMeshObject *MO;
  iMaterialList *MATL;
  iMaterialWrapper *MATW;

  iMeshWrapper *MW2;
  csRef <iSceneNode> SN;
  csRef <iSceneNode> SN2;
  csRef <iSceneNodeArray> SNA;

  paint_all_white ();

  MO = MW->GetMeshObject ();

  MATL = engine->GetMaterialList ();

  MATW = MATL->FindByName ("blue_bone");

  MO->SetMaterialWrapper (MATW);


  SN = MW->QuerySceneNode ();

  if (SN->GetParent() != NULL)
  {
    SN2 = SN->GetParent ();
    MW2 = SN2->QueryMesh ();
    MO = MW2->GetMeshObject ();
    MATL = engine->GetMaterialList ();
    MATW = MATL->FindByName ("red_bone");
    MO->SetMaterialWrapper(MATW);
  }

  SNA = SN->GetChildrenArray ();

  paint_green_and_children (SNA);
}

void AppXyzim::paint_green_and_children (iSceneNodeArray *SNA)
{
  int max;
  max = SNA->GetSize ();
  int i;

  iMeshWrapper *MW;
  iObject *O;
  //iMeshList *M2;

  iMeshObject *MO;

  csRef <iSceneNodeArray> SNB;
  iMaterialList *MATL;
  iMaterialWrapper *MATW;

  char c1[500];
  for (i = 0; i < max; i++)
  {
    MW = SNA->Get (i)->QueryMesh ();
    O = MW->QueryObject ();

    strcpy(c1,O->GetName ());

    if (strcmp(c1, "walls") != 0)
    {
      //physical mesh changing to white
      MO = MW->GetMeshObject ();
      MATL = engine->GetMaterialList ();
      MATW = MATL->FindByName ("green_bone");
      MO->SetMaterialWrapper (MATW);
      //M2=MW->GetChildren();
      SNB = MW->QuerySceneNode ()->GetChildrenArray ();

      if (SNB->GetSize () != 0)
        paint_green_and_children (SNB);
    }
  }
}

void AppXyzim::paint_all_white(void)
{
  int mlmax;
  iMeshWrapper *temp_wrap = NULL;
  iMeshList *ML = engine->GetMeshes ();

  int i;
  iMeshObject *MO;
  iMaterialList *MATL;
  iMaterialWrapper *MATW;
  mlmax = ML->GetCount ();

  for (i = 0; i < mlmax; i++)
  {
    temp_wrap = ML->Get (i);
    MO = temp_wrap->GetMeshObject ();

    MATL = engine->GetMaterialList ();
    MATW = MATL->FindByName ("white_bone");

    MO->SetMaterialWrapper (MATW);
  }

  //paint_white_and_children(ML);
}

//void AppXyzim::paint_white_and_children(iMeshList *ML)
void paint_white_and_children(iSceneNodeArray *SNA)
{
  /*
  int max;
  max=SNA->GetSize();
  int i;
  iMeshWrapper *MW;
  iObject *O;
  iMeshList *M2;
  iMeshObject *MO;
  iSceneNodeArray *SNB;
  iMaterialList *MATL;
  iMaterialWrapper *MATW;

  char c1[500];
  for (i = 0; i < max; i++)
  {
    MW = SNA->Get (i)->QueryMesh ();
    O = MW->QueryObject ();
    strcpy(c1, O->GetName ());
    if (strcmp(c1, "walls") != 0)
    {
      //physical mesh changing to white
      MO = MW->GetMeshObject ();
      MATL = engine->GetMaterialList ();
      MATW = MATL->FindByName ("white_bone");
      MO->SetMaterialWrapper (MATW);

      //M2 = MW->GetChildren ();
      SNB = MW->QuerySceneNode ()->GetChildrenArray ();

      if (SNB->GetSize () !=0)
        paint_white_and_children(SNB);
    }
  }
*/
}

void AppXyzim::check_input(void)
{
int i;
int ok;
char c2[200],c1[200];
//int j;

//this case is for when all frames need to be opened
if(show_all_frames>-1)
{


ok=0;
for(i=show_all_frames;i<1000;i++)
{
if(Aframe_time[i]!=-1)
{
//load i
sprintf(c2,"%d",i);
load_skeleton("animations/working",c2);
show_all_frames=i+1;
ok=1;

which_ss=P.open(7);
aquire_screen_shot=1;

sprintf(c1,"%d",i);

strcpy(which_ss->G3[2]->e.c1,c1);
sprintf(c1,"%d",Aframe_time[i]);
strcpy(which_ss->G3[3]->e.c1,c1);
which_ss->mdata[0]=i;

sprintf(c1,"%d",i);
i=999999;
}//end of Aframe_time



}//end of for loop

if (kbd->GetKeyState (CSKEY_ESC) || ok==0)
{
show_all_frames=-1;
loadundo();
}//end of esc

}//end of show all frames
else
  check_input2();


}

void AppXyzim::check_input2(void)
{
  static float acc=.0001;
  //so people dont load a ton by holding insert
//  static int ok_to_load=1;
  //so people dont redo  a ton by holding insert
  static int ok_to_redo=1;
  //so people dont redo  a ton by holding insert
  static int ok_to_undo=1;

  static int lastchange=0;
  int change;
  change=0;

  if (sg1 != NULL)
    if (kbd->GetKeyState (CSKEY_SHIFT))
      if (kbd->GetKeyState ('o'))
      {
        if (change == 0)
          change = 1;
        disconnect_mesh(sg1);
      }

  if (sg1 != NULL)
    if (!kbd->GetKeyState (CSKEY_SHIFT))
      if (kbd->GetKeyState ('o'))
      {
        if (change == 0)
          change = 1;
        connect_mesh(sg1);
      }

  if (kbd->GetKeyState ('`'))
  {
    sg1 = NULL;
    sg2 = NULL;
    paint_all_white();
    //P.close_all();
  }
  if (kbd->GetKeyState (CSKEY_CTRL))
    acc = 0.0003f;
  else
    acc = 0.005f;

  if (kbd->GetKeyState (CSKEY_SHIFT))
  {
    if (acc > 0)
      acc = acc * -1;
  }
  else
  {
    if (acc < 0)
      acc = acc * -1;
  }
  if (sg1 != NULL)
    if (kbd->GetKeyState (CSKEY_ALT))
    {
      if (kbd->GetKeyState ('f'))
      {
        if (change == 0)
          change = 1;
        sg1->GetMovable ()->MovePosition (csVector3 (0, -acc, 0));
        sg1->GetMovable ()->UpdateMove ();
      }

      if (kbd->GetKeyState ('r'))
      {
        if (change == 0)
          change = 1;
        sg1->GetMovable ()->MovePosition (csVector3 (0, acc, 0));
        sg1->GetMovable ()->UpdateMove ();
      }

      if (kbd->GetKeyState ('e') || kbd->GetKeyState ('d') || kbd->GetKeyState ('c'))
      {
        if (change == 0)
          change = 1;
        sg1->GetMovable ()->MovePosition (csVector3 (0, 0, acc));
        sg1->GetMovable ()->UpdateMove ();
      }

      if (kbd->GetKeyState ('q') || kbd->GetKeyState ('a') || kbd->GetKeyState ('z'))
      {
        if (change == 0)
          change = 1;
        sg1->GetMovable ()->MovePosition (csVector3 (0, 0, -acc));
        sg1->GetMovable ()->UpdateMove ();
      }

      if (kbd->GetKeyState ('w') || kbd->GetKeyState ('q') || kbd->GetKeyState ('e'))
      {
        if(change == 0)
          change = 1;
        sg1->GetMovable ()->MovePosition (csVector3 (-acc, 0, 0));
        sg1->GetMovable ()->UpdateMove ();
      }

      if (kbd->GetKeyState ('x') || kbd->GetKeyState ('s') || kbd->GetKeyState ('z') || kbd->GetKeyState ('c'))
      {
        if (change == 0)
          change =1;
        sg1->GetMovable ()->MovePosition (csVector3 (acc, 0, 0));
        sg1->GetMovable ()->UpdateMove ();
      }
    }
    ///THIS ELSE IS IF SHIFT IS NOT HELD IN
    else
    {
      if (kbd->GetKeyState ('f'))
      {
        if (change == 0)
        change = 1;
        sg1->GetMovable ()->GetTransform ().RotateThis (csVector3 (0, -1, 0), acc);
        sg1->GetMovable ()->UpdateMove ();
      }

      if (kbd->GetKeyState ('r'))
      {
        if (change == 0)
        change = 1;
        sg1->GetMovable ()->GetTransform ().RotateThis (csVector3 (0, 1, 0), acc);
        sg1->GetMovable ()->UpdateMove ();
      }

      if (kbd->GetKeyState ('w') || kbd->GetKeyState ('q') || kbd->GetKeyState ('e'))
      {
        if (change == 0)
          change = 1;
        sg1->GetMovable ()->GetTransform ().RotateThis (csVector3 (0, 0, 1), acc);
        sg1->GetMovable ()->UpdateMove ();
      }

      if (kbd->GetKeyState ('x') || kbd->GetKeyState ('s') || kbd->GetKeyState ('z') || kbd->GetKeyState ('c'))
      {
        if (change == 0)
          change =1;
        sg1->GetMovable ()->GetTransform ().RotateThis (csVector3 (0, 0, -1), acc);
        sg1->GetMovable ()->UpdateMove ();
      }

      if (kbd->GetKeyState ('q') || kbd->GetKeyState ('a') || kbd->GetKeyState ('z'))
      {
        if (change == 0)
          change = 1;
        sg1->GetMovable ()->GetTransform ().RotateThis (csVector3 (-1, 0, 0), acc);
        sg1->GetMovable ()->UpdateMove ();
      }

      if (kbd->GetKeyState ('e') || kbd->GetKeyState ('d') || kbd->GetKeyState ('c'))
      {
        if (change == 0)
          change =1;
        sg1->GetMovable ()->GetTransform ().RotateThis (csVector3 (1, 0, 0), acc);
        sg1->GetMovable ()->UpdateMove ();
      }
    }

    if (ok_to_undo == 1)
      if (kbd->GetKeyState (CSKEY_HOME))
      {
        printf("ok to loadundo\n");
        loadundo();
        ok_to_undo=0;
      }

    if (kbd->GetKeyState (CSKEY_HOME))
      ok_to_undo = 0;
    else
      ok_to_undo = 1;

    if (ok_to_redo == 1)
    if (kbd->GetKeyState (CSKEY_END))
    {
      loadredo ();
      ok_to_redo = 0;
    }

    if (kbd->GetKeyState (CSKEY_END))
      ok_to_redo = 0;
    else
      ok_to_redo = 1;

    if (lastchange == 0)
      if (change == 1)
{        saveundo ();
 
}
    if (lastchange == 1)
      if (change == 0)
{
        saveundo ();


}
    lastchange=change;
}


void AppXyzim::disconnect_mesh(iMeshWrapper *MW)
{
//  iMeshWrapper *MWP;
//  iMeshList *ML;
//  iMeshList *EML;
  //iMovable *MOV;
  csVector3 v;
  csRef <iSceneNode> SN;
  csRef <iSceneNode> SN2;
  csRef <iSceneNodeArray> SNA;

  SN = MW->QuerySceneNode ();

  if (SN->GetParent () == NULL)
  {
    printf("This has no parent to disconnect\n");
    return;
  }

  v = MW->GetMovable ()->GetFullPosition ();

  SN->SetParent (NULL);

  //SN2=SN->GetParent();
  //MWP=SN2->QueryMesh();
  //SNA=MWP->QuerySceneNode()->GetChildrenArray();

  //step 1: remove from hierarchy(watch csrefs?)
  //SCF_INC_REF(MW);
  //ML=MWP->GetChildren();
  //ML->Remove(MW);

  //step 2: add into main
  //EML=engine->GetMeshes();
  //EML->Add(MW);
  selected(MW);

  MW->GetMovable ()->SetPosition (view->GetCamera ()->GetSector (), v);

  //print_list(EML);
  //step3: adjust its position
}

void AppXyzim::connect_mesh(iMeshWrapper *MW)
{
  //iObject *O;
  //iMeshList *M2;
  iMeshObject *MO;
  iMaterialList *MATL;
  iMaterialWrapper *MATW;
  //iMeshWrapper *MW2,*MWP;
  //MeeshWrapper *MWP;
  csRef <iSceneNode> SN;
  csRef <iSceneNode> SN2;
  csRef <iSceneNodeArray> SNA;

  if (MW == NULL)
  {
    printf("Select a mesh to connect");
    return;
  }

  SN = MW->QuerySceneNode ();

  if (SN->GetParent () != NULL)
  {
    printf("Error, mesh already connected\n");
    return;
  }

  //step 1
  paint_all_white();

  //step 2: paint this one purple
  MO = MW->GetMeshObject ();
  MATL = engine->GetMaterialList();
  MATW = MATL->FindByName ("purple_bone");
  MO->SetMaterialWrapper (MATW);

  sg2 = MW;
  sg1 = NULL;
}

void AppXyzim::connect_mesh2(void)
{
  //make sg2 a child of sg1
  //iMeshWrapper *MWP,*MWC;
iMeshList *MLE;//,*MLC;//,*ML;
  //iMeshList *EML;
  //iMovable *MOV;
  //int max;
  int i;
  csVector3 v1,v2;
  csRef <iSceneNode> SN;
  csRef <iSceneNode> SN2;
  csRef <iSceneNodeArray> SNA;

  //step 0: make sure sg1 isn't a child of sg2
  i = is_b_childof_a (sg2, sg1);

  if (i == 1)
  {
    printf("Error: You can't connect it to its children\n");
    sg1 = NULL;
    sg2 = NULL;
    paint_all_white();
    return;
  }

  //Step 1: remove sg2 from main list.

  //Step 2: Add sg2 to sg1 childrenlist, while preserving location
  v1 = sg1->GetMovable ()->GetFullPosition ();
  v2 = sg2->GetMovable ()->GetFullPosition ();

  v1 = v2 - v1;
  MLE = engine->GetMeshes ();
  //SCF_INC_REF(sg2);
  //MLE->Remove(sg2);
  SN = sg1->QuerySceneNode ();
  SNA = SN->GetChildrenArray ();
  //MLC=sg1->GetChildren();

  sg2->GetMovable ()->SetPosition (v1);
  //printf("%d\n", MLC->GetCount ());

  SN2 = sg2->QuerySceneNode ();
  SN2->SetParent (SN);

  //MLC->Add (sg2);
  //printf("ok\n");
  paint_all_white();
  //printf("ok\n");
  //SCF_DEC_REF(sg2);
  sg2=NULL;
}

int AppXyzim::is_b_childof_a (iMeshWrapper *MWP, iMeshWrapper *MWC)
{
  int i;
  int max;
  int j;
//  iMeshList *ML;
  //,*ML2;
  iMeshWrapper *MWT;
  csRef <iSceneNode> SN;
  csRef <iSceneNode> SN2;
  csRef <iSceneNodeArray> SNA;

  SN = MWP->QuerySceneNode ();
  SNA = SN->GetChildrenArray ();
  //ML=MWP->GetChildren();

  max = SNA->GetSize ();
  //max=ML->GetCount();

  for (i = 0; i < max; i++)
  {
    MWT = SNA->Get (i)->QueryMesh ();
    //MWT = ML->Get (i);

    //if MWC = tempmeshwrapper(MWT), then return 1.
    if (MWT == MWC)
      return 1;

    //recurse this meshwrappers kids
    j = is_b_childof_a (MWT, MWC);
    if (j == 1)
      return 1;
  }

  return 0;
}

void AppXyzim::saveundo(void)
{
  char c1[15];
  undo_pos++;

  if (undo_pos > max_undo)
    max_undo = undo_pos;

  sprintf(c1, "%d", undo_pos);
//  printf(c1);
//  printf(" save \n");
  save_skeleton ("undo", c1);





}

void AppXyzim::save_skeleton(char *directory, char *filename)
{
  //iFile *F1;
  csRef<iStringArray> SV;
  int max;
  char c1[500];
  char c2[500];
  //bool b;
  int i;
  //int match=0;

  FS->Sync();
  strcpy(c1, FS->GetCwd ());

  //printf("%d",max);
  //// (vk) changed to use app's VFS path
  ////strcpy(c2,"/this/data/xyzimation/skeletons/");
  strcpy(c2, "/appdata/xyzimation/skeletons/");
  strcat(c2, directory);
  strcat(c2, "/");

  SV = FS->FindFiles (c2);
  max = SV->Length ();

  strcat(c2, filename);

  for (i = 0; i < max; i++)
  {
    strcpy(c1, SV->Get (i));

    if (strcmp(directory, "undo") != 0)
      if (strcmp(c1, c2) == 0)
      {
        //popup *which=P.open(5);
        //strcpy(which->G[2]->e.c1,directory);
        //strcpy(which->G[3]->e.c1,filename);
        FS->ChDir("/");
        return;
      }
  }

  save_skeleton2 (directory, filename);

  printf(directory);
  printf(filename);
  printf("\n");
}

//struggled through this one if you cant tell :P
void AppXyzim::save_skeleton2(char *directory, char *filename)
{
  csRef<iFile> F1;
  csRef<iFile> F2;
  csRef<iStringArray> SV;
  int max;
  char c1[5000];
  char c2[500];
  char c3[200];
  char c5[500];
  //bool b;
  int i;
  //int j;
  //int match=0;
  int a;

  //printf(directory);
  //printf("           ");
  //printf(filename);
  //printf("\n");

  //// (vk) changed to use app's VFS path
  ////strcpy(c2,"/this/data/xyzimation/skeletons/");
  strcpy(c2,"/appdata/xyzimation/skeletons/");
  strcat(c2,directory);
  strcat(c2,"/");
  //strcat(c2,filename);
  FS->ChDir(c2);
  //FOR NOW
  //FS->ChDir("/this/data/xyzimation/tools/meshobjects/");
  FS->WriteFile (filename, " ", 1);
  F1 = FS->Open (filename, VFS_FILE_WRITE);
  //For now
  //FS->WriteFile ("robot_objects", " ", 1);
  //F1=FS->Open ("robot_objects", VFS_FILE_WRITE);
  //F1->Write ("suck it \n", 9);
  //F1->Write ("suck it \n", 9);
  //F1->Write ("suck it \n", 9);
  //F1->Write ("suck it \n", 9);
  strcpy(c1, "<world>\n");
  F1->Write (c1, strlen(c1));
  strcpy(c1, "<textures>\n");
  F1->Write (c1, strlen(c1));

  strcpy(c1, "</textures>\n");

  F1->Write(c1, strlen(c1));
  strcpy(c1, "<materials>\n");
  F1->Write(c1, strlen(c1));
  strcpy(c1, "</materials>\n");
  F1->Write(c1, strlen(c1));

  csRef<iMeshFactoryList> MFL;
  csRef<iMeshFactoryWrapper> MFW;
  csRef<iObject> O;
  csRef<iString> S;

  MFL = engine->GetMeshFactories ();
  max = MFL->GetCount ();

  for (i = 0; i < max; i++)
  {
    MFW = MFL->Get (i);
    O = MFW->QueryObject ();
    strcpy(c1, "<meshfact name=\"");
    strcpy(c3, O->GetName ());
    strcat(c1, c3);
    strcat(c1, "\">\n");

    if (DBG == 1)
      printf(c1);
    if (DBG == 1)
      printf("\n");

    F1->Write (c1, strlen(c1));
    //printf(c1);
    a= get_plugin (c3);
    if (a == -1)
    {
      printf("THIS IS FOR SERIOUS, get_plugin returned -1 \n");
      printf("Trying c3:");
      printf(c3);
      printf("\n");
      print_plugins();
    }

    //strcpy(S_mesh[i],meshname);
    sprintf(c5, "/this/data/Tmemory/file%d",a);

    F2 = FS->Open (c5, VFS_FILE_READ);

    //printf("wwwwwwwwww\n");
    while(!F2->AtEOF())
    {
      strcpy(c1, ReadLine (F2));
      //printf(c1);
      //printf("\n");
      F1->Write (c1, strlen(c1));
    }

  }

  //done with mesh facts

  //strcpy(c1, "<sector name=\"room\">");
  //F1->Write (c1, strlen(c1));

  //start writing meshobjs

  csRef<iMeshList> ML;
  csRef<iMeshWrapper> MW;
  //int max;
  //int i;
  ML = engine->GetMeshes ();
  max = ML->GetCount();

  strcpy(c1, "<sector name=\"room\">\n");
  F1->Write (c1,strlen(c1));

  for (i = 0; i < max; i++)
  {
    MW = ML->Get (i);
//    if (strcmp(MW->QueryObject ()->GetName (), "walls") != 0)
    if (strcmp(MW->QueryObject ()->GetName (), "cubard") == 0 
     || strcmp(MW->QueryObject ()->GetName (), "002cubard") == 0)
    {
      char cname[100];
      strcpy(cname, MW->GetFactory ()->QueryObject ()->GetName ());
      if (DBG == 1)
        printf("NAME: ");
      if (DBG == 1) 
        printf(cname);
      if (DBG == 1)
        printf("\n ");

      save_skeleton_sub1(MW,F1);
    }
  }
  strcpy(c1,"</sector>\n");
  F1->Write (c1, strlen(c1));

  strcpy(c1, "</world>\n");
  F1->Write (c1, strlen(c1));
  FS->ChDir ("/");

  //needed?
  F1 = NULL;
  F2 = NULL;
  //SV=NULL;
  //ML=NULL;
  //MW=NULL;
}

void AppXyzim::save_skeleton_sub1(iMeshWrapper *MWP,iFile *F1)
{
  csRef<iMeshList> ML;
  csRef<iMeshFactoryWrapper> MFW;
  csRef<iFile> F2;
  csRef<iMovable> MOVE;
  csRef<iMeshWrapper> MW2;
  int i;
  int max;
  char c1[300];
  char c5[300];
  char cname[300];
  int a;

  strcpy(c1, "<meshobj name=\"");
  strcat(c1, MWP->QueryObject ()->GetName ());
  strcat(c1, "\">\n");
  F1->Write (c1,strlen(c1));
  if (DBG == 1)
    printf(c1);
  if (DBG == 1)
    printf("\n");

  MFW = MWP->GetFactory ();
  //printf("Its factory->");
  //printf(MFW->QueryObject ()->GetName ());
  //printf("\n");

  strcpy(cname, MFW->QueryObject ()->GetName ());
  a = get_plugin (cname);
  if(a == -1)
  {
    printf("THIS IS FOR SERIOUS, get_plugin returned -1 \n");
    printf("Trying cname:");
    printf(cname);
    printf("\n");
    print_plugins ();
  }

  //now load file, and extract plugin line
  sprintf(c5, "/this/data/Tmemory/file%d",a);

  F2 = FS->Open (c5, VFS_FILE_READ);
  strcpy(c5, "<plugin>");
  strcat(c5, Parse_plugin_factory (F2));
  strcat(c5, "</plugin>\n");

  F1->Write (c5, strlen(c5));
  //printf(c5);

  strcpy(c5, "<params>\n");
  F1->Write (c5, strlen (c5));
  //printf(c5);

  strcpy(c5, "<factory>");
  strcat(c5, MFW->QueryObject ()->GetName ());
  strcat(c5,"</factory>\n");
  F1->Write (c5, strlen(c5));
  //printf(c5);

  strcpy(c5,"</params>\n");
  //printf(c5);
  F1->Write(c5, strlen(c5));

  MOVE = MWP->GetMovable ();
  csVector3 v1;
  v1 = MOVE->GetPosition ();

  strcpy(c5, "<move>\n");
  F1->Write (c5,strlen(c5));
  //printf(c5);
  sprintf(c5, "<v x=\"%f\" y=\"%f\" z=\"%f\" />\n", v1.x, v1.y, v1.z);

  F1->Write(c5,strlen(c5));
  //printf(c5);
  //printf(c5);

  csMatrix3 m1 = MOVE->GetTransform ().GetT2O ();
  strcpy(c5, "<matrix>\n");
  F1->Write(c5, strlen(c5));
  //printf(c5);
  sprintf(c5, "<m11> %f </m11>\n", m1.m11);
  F1->Write (c5, strlen(c5));
  //printf(c5);
  sprintf(c5, "<m12> %f </m12>\n", m1.m12);
  F1->Write (c5, strlen(c5));
  //printf(c5);
  sprintf(c5, "<m13> %f </m13>\n", m1.m13);
  F1->Write (c5, strlen(c5));
  //printf(c5);
  sprintf(c5, "<m21> %f </m21>\n", m1.m21);
  F1->Write (c5, strlen(c5));
  //printf(c5);
  sprintf(c5, "<m22> %f </m22>\n", m1.m22);
  F1->Write (c5, strlen(c5));
  //printf(c5);
  sprintf(c5, "<m23> %f </m23>\n", m1.m23);
  F1->Write (c5, strlen(c5));
  //printf(c5);
  sprintf(c5, "<m31> %f </m31>\n", m1.m31);
  F1->Write (c5, strlen(c5));
  //printf(c5);
  sprintf(c5, "<m32> %f </m32>\n", m1.m32);
  F1->Write (c5, strlen(c5));
  //printf(c5);
  sprintf(c5, "<m33> %f </m33>\n", m1.m33);
  F1->Write (c5, strlen(c5));
  //printf(c5);
  strcpy(c5, "</matrix>\n");
  F1->Write (c5, strlen(c5));
  //printf(c5);
  strcpy(c5, "</move>\n");
  F1->Write (c5, strlen(c5));

  //ML=MWP->GetChildren();
  csRef <iSceneNode> SN;
  csRef <iSceneNodeArray> SNA;
  SN = MWP->QuerySceneNode ();
  SNA = SN->GetChildrenArray ();
  max = SNA->GetSize ();

  for (i = 0; i < max; i++)
  {
    strcpy(cname, SNA->Get (i)->QueryMesh ()->GetFactory ()->QueryObject ()->GetName ());
    //strcpy(cname,ML->Get(i)->GetFactory()->QueryObject()->GetName());
    if (DBG == 1)
      printf("SUB NAME: ");
    if (DBG == 1)
      printf(cname);
    if (DBG == 1)
      printf("\n ");
    save_skeleton_sub1 (SNA->Get (i)->QueryMesh (), F1);
  }

  strcpy(c1,"</meshobj>\n");
  F1->Write(c1, strlen(c1));
  //printf(c1);

  F2 = NULL;
  //ML=NULL;
  //MFW=NULL;
  //MOVE=NULL;
  //MW2=NULL;
}

int AppXyzim::get_plugin(char *meshname)
{
  int i;
  //  int ok=;
  //printf("jhhh\n");
  ///printf("%d\n",S_index);

  for(i = 0; i< S_index; i++)
  {
    if (strcmp(S_mesh[i], meshname) ==0)
      return(i);
  }
  return -1;
}

void AppXyzim::print_plugins(void)
{
  int i;
  printf("%d\n", S_index);
  for (i = 0; i < S_index; i++)
  {
    printf(S_mesh[i]);
    printf("\n");
  }
}

void AppXyzim::insert_plugin(char *meshname,char *pluginname,iFile *F1)
{
  int i;
  char c5[400];
  csRef<iFile> F2;

  for (i = 0; i < S_index; i++)
  {
    if (strcmp(S_mesh[i], meshname) == 0)
      return;
  }

  strcpy(S_mesh[i], meshname);
  //strcpy(c5,"/this/data/Tmemory/file%d",S_index);
  sprintf(c5, "/this/data/Tmemory/file%d", S_index);
  F2 = FS->Open (c5, VFS_FILE_WRITE);
  Write_Extra (F1, F2);
  S_index++;
  F2=NULL;
}

void AppXyzim::Load_Plugins(char *filename)
{
  csRef<iFile> F1;
  //int i;
  char c1[270];
  char c2[270];
  //int j;
//  printf("load plugins\n");
  F1 = FS->Open (filename, VFS_FILE_READ);

  while (!F1->AtEOF ())
  {
    //Pairs a name with an index
    strcpy(c1, Parse_meshfact (F1));
    insert_plugin (c1, c2, F1);
    //uses the index to write extra parts of the file past plugin, which later is read, and pasted
    //Write_Extra (F1);
    //OVER ALL THIS GHETTO FUNCTION re-reads in worldfiles, and saves it in a format I can save later.
  }
  //printf("\n");
  //printf("\n");
  //for (i = 0; i < 250; i++)
  //  printf("%d,",c1[i]);
  //printf("\n");
  F1=NULL;
}

int AppXyzim::new_aframe(void)
{
  int i;
  for (i = 1; i < 1000; i++)
    if(Aframe_time[i]==-1)
      return i;
  return -1;
}

int AppXyzim::count_aframe(void)
{
  int i;
  int max;
  max = 0;

  for (i = 1; i < 1000; i++)
    if (Aframe_time[i] != -1)
      max++;
  return max;
}

void AppXyzim::loadundo(void)
{
  char c1[15];
  if (undo_pos == 0)
    return;

  sprintf(c1, "%d", undo_pos);
//  printf(c1);
//  printf(" load undo \n");
  load_skeleton ("undo", c1);
  undo_pos--;
  sg1=NULL;
  sg2=NULL;
}

void AppXyzim::load_skeleton(char *directory, char *filename)
{
  char c1[300];
  char c2[300];
  int i;
  int max, max2;
  int a;
  csRef<iMeshList> ML = engine->GetMeshes ();
  csRef<iMeshFactoryList> MFL = engine->GetMeshFactories ();
  csRef<iMeshFactoryWrapper> MFW;
  csRef<iMeshWrapper> MW;
  csRef<iObject> O;
  bool b;

  FS->Sync();

  //printf(directory);
  //printf("\n");
  //printf(filename);
  //printf("\n");
  strcpy(c2, "/this/data/xyzimation/skeletons/");
  strcat(c2, directory);
  strcat(c2, "/");
  printf(c2);
  printf("\n");
  if(DBG==1)
    printf("does it exist\n");

  FS->ChDir(c2);
  b = FS->Exists (filename);
  FS->ChDir ("/");

  if (!b)
  {
    strcpy(c1, "File not found");
    strcat(c1, c2);
    strcat(c1, filename);
    //P.perror(c1);
    return;
  }

  strcat(c2,filename);
  if(DBG==1)
    printf("exists \n");

//  saveundo();

  max = ML->GetCount ();
  for (i = 0; i < max; i++)
  {
    MW = ML->Get (i);
    O = MW->QueryObject ();
    strcpy(c1, O->GetName ());
    if (strcmp(c1, "walls") != 0)
      ML->Remove (MW);

    a = ML->GetCount ();
    if (DBG == 1)
      printf("COUNT%d  VS I%d\n", a, i);
    //if(DBG==0)
    //  printf("NOCOUNT%d  VS I%d\n",a,i);

    max2 = ML->GetCount ();
    if (max2 < max)
    {
      max = max2;
      i = -1;
    }
  }

  //remove mesh factories from system too!
  max = MFL->GetCount ();
  for (i = 0; i < max; i++)
  {
    MFW = MFL->Get (i);
    strcpy(c1, MFW->QueryObject ()->GetName ());
    if (DBG == 1)
      printf(c1);
    if (DBG == 1)
      printf("\n");

    a = get_plugin (c1);
    if (DBG == 1)
      printf("A:%d\n", a);
    if (a > -1)
      MFL->Remove (MFW);

    a = MFL->GetCount ();
    if (DBG == 1)
      printf("COUNT%d  VS I%d\n", a, i);
    //if(DBG==0)
    //  printf("NOCOUNT%d  VS I%d\n", a, i);

    max2 = MFL->GetCount ();
    if (max2 < max)
    {
      max = max2;
      i = -1;
    }
  }

  for (i = 0; i < 200; i++)
    strcpy(S_mesh[i], "");

  //DONE REMOVING MESH FACTORIES
  S_index = 0;
  if (DBG == 1)
    printf("world cleared\n");
  if (DBG == 1)
    printf("Loading new map file\n");

  loader->LoadMapFile (c2, false);
  if (DBG == 1)
    printf("New map file loaded, now loading plugins\n");

  Load_Plugins (c2);
  if (DBG == 1)
    printf("Plugins loaded\n");
}

void AppXyzim::loadredo(void)
{
  char c1[15];
  //printf("33\n");
  undo_pos++;
  if (undo_pos > max_undo)
  {
    undo_pos = max_undo;
    return;
  }
  sprintf(c1, "%d", undo_pos);
//  printf(c1);
//  printf(" load redo \n");
  load_skeleton ("undo", c1);
  sg1 = NULL;
  sg2 = NULL;
}

void AppXyzim::handle_popup_events(void)
{
//int i;
//save_animation();



//type 11 is the Animation controller
if(P.EV_window->type == 11)
{
//
if(P.EV_gutn1==2)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
int af;
char c1[20];
which_ss=P.open(7);
aquire_screen_shot=1;
af=new_aframe();
Aframe_time[af]=1000;

sprintf(c1,"%d",af);

strcpy(which_ss->G3[2]->e.c1,c1);
sprintf(c1,"%d",Aframe_time[af]);
strcpy(which_ss->G3[3]->e.c1,c1);
which_ss->mdata[0]=af;

sprintf(c1,"%d",af);
//printf(c1);
//printf("saving\n");
save_skeleton2("animations/working",c1);
return;
}

//Show all Frames
if(P.EV_gutn1==3)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
//Steps, saveundo


saveundo();
show_all_frames=1;
return;
}

//Play animation
if(P.EV_gutn1==4)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
play_animation(0,9999);
return;
}

//save
if(P.EV_gutn1==5)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{


saveundo();
P.open(9);
return;
}
//load
if(P.EV_gutn1==6)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.open(12);
return;
}


}///end of type==11


//TYPE 10 is confirm animation overwrite
if(P.EV_window->type == 10)
{

if(P.EV_gutn1==0)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
//printf("cool2\n");

save_animation2(P.EV_window->G3[2]->e.c1,P.EV_window->G3[3]->e.c1);
P.close_window(P.EV_window);
return;
}

//close window
if(P.EV_gutn1==1)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.close_window(P.EV_window);
return;
}


}///end of type==10

//TYPE 9is save animation -OK or Return
if(P.EV_window->type == 9)
{

if(P.EV_gutn1==0)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.losefocus();
//P.focus=NULL;
if(strcmp(P.EV_window->G3[2]->e.c1,"")==0)
strcpy(P.EV_window->G3[2]->e.c1,"temp");

if(strcmp(P.EV_window->G3[3]->e.c1,"")==0)
strcpy(P.EV_window->G3[3]->e.c1,"temp");


strcpy(def_save_anim_dir,P.EV_window->G3[2]->e.c1);
strcpy(def_save_anim_file,P.EV_window->G3[3]->e.c1);
save_animation(P.EV_window->G3[2]->e.c1,P.EV_window->G3[3]->e.c1);
P.close_window(P.EV_window);
return;



}

if(P.EV_new_mb==4)
{
P.losefocus();
if(strcmp(P.EV_window->G3[2]->e.c1,"")==0)
strcpy(P.EV_window->G3[2]->e.c1,"temp");

if(strcmp(P.EV_window->G3[3]->e.c1,"")==0)
strcpy(P.EV_window->G3[3]->e.c1,"temp");

strcpy(def_save_anim_dir,P.EV_window->G3[2]->e.c1);
strcpy(def_save_anim_file,P.EV_window->G3[3]->e.c1);
save_animation(P.EV_window->G3[2]->e.c1,P.EV_window->G3[3]->e.c1);
printf("changed\n");
P.close_window(P.EV_window);
return;
}

//toggles text field - Tab
if(P.EV_new_mb==5 )
{

//P.focus=P.EV_window;
if(P.focus_gut==2)
P.setfocus(P.EV_window,3);
else
if(P.focus_gut==3)
P.setfocus(P.EV_window,2);

return;


}
//close window
if(P.EV_gutn1==1)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.close_window(P.EV_window);
return;
}


}///end of type==9


//animation frame window
if(P.EV_window->type == 7)
{
if(P.EV_gutn1==0)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
play_animation(0,P.EV_window->mdata[0]);


if(P.EV_gutn1==1)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
play_animation(0,9999);


if(P.EV_gutn1==6)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
play_animation(0,9999);


if(P.EV_gutn1==7)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
play_animation(0,9999);


// return or gut 9 is pressed
if(P.EV_new_mb==4 || (P.EV_gutn1==9 && P.EV_old_mb>0 && P.EV_new_mb==0))
{
//P.focus=NULL;
//printf("got here\n");
int r;
r=atoi(P.EV_window->G3[2]->e.c1);
//printf("%d\n",r);
//printf("%d\n",Aframe_time[r]);


//if hash table occupied

if(Aframe_time[r]!=-1)
{
//And not by self
if(P.EV_window->mdata[0]!=r)
P.perror("That frame # already used. Choose another.");
}
else
{
char c1t[200];
char c2t[200];
sprintf(c1t,"/this/data/xyzimation/skeletons/animations/working/%d",P.EV_window->mdata[0]);

//100% DEFINATELY NEEDS TO copy FILE
sprintf(c2t,"/this/data/xyzimation/skeletons/animations/working/");
FS->ChDir(c2t);
sprintf(c2t,"%d",r);
FS->DeleteFile(c2t);
FS->ChDir("/");
//printf("hey\n");

sprintf(c2t,"/this/data/xyzimation/skeletons/animations/working/%d",r);
copyit(c1t,c2t);

//printf("hey2\n");

//old=P.EV_window->mdata[0]
//new=r

Aframe_time[P.EV_window->mdata[0]]=-1;
P.EV_window->mdata[0]=r;

}

r=atoi(P.EV_window->G3[3]->e.c1);
if(r>0)
Aframe_time[P.EV_window->mdata[0]]=r;

P.close_window(P.EV_window);
return;
}

//toggles text field - Tab
if(P.EV_new_mb==5 )
{
//P.focus=P.EV_window;
if(P.focus_gut==2)
P.setfocus(P.EV_window,3);
//P.focus_gut=3;
else
if(P.focus_gut==3)
P.setfocus(P.EV_window,2);
//P.focus_gut=2;

return;


}

//close window
if(P.EV_gutn1==8)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
int r;
r=atoi(P.EV_window->G3[2]->e.c1);
printf("%d\n",r);
Aframe_time[r]=-1;

P.close_window(P.EV_window);
return;
}


}///end of type==7


//TYPE 6is load skeleton -OK or Return
if(P.EV_window->type == 12)
{

if(P.EV_gutn1==0)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.losefocus();
if(strcmp(P.EV_window->G3[2]->e.c1,"")==0)
strcpy(P.EV_window->G3[2]->e.c1,"temp");

if(strcmp(P.EV_window->G3[3]->e.c1,"")==0)
strcpy(P.EV_window->G3[3]->e.c1,"temp");


saveundo();
strcpy(def_load_anim_dir,P.EV_window->G3[2]->e.c1);
strcpy(def_load_anim_file,P.EV_window->G3[3]->e.c1);

load_animation(P.EV_window->G3[2]->e.c1,P.EV_window->G3[3]->e.c1);
printf("test1\n");
P.close_window(P.EV_window);
printf("test2\n");
return;
}

if(P.EV_new_mb==4)
{
P.losefocus();
if(strcmp(P.EV_window->G3[2]->e.c1,"")==0)
strcpy(P.EV_window->G3[2]->e.c1,"temp");

if(strcmp(P.EV_window->G3[3]->e.c1,"")==0)
strcpy(P.EV_window->G3[3]->e.c1,"temp");


saveundo();
strcpy(def_load_anim_dir,P.EV_window->G3[2]->e.c1);
strcpy(def_load_anim_file,P.EV_window->G3[3]->e.c1);

load_animation(P.EV_window->G3[2]->e.c1,P.EV_window->G3[3]->e.c1);
//printf("test11\n");
P.close_window(P.EV_window);
//printf("test22\n");

return;
}

//toggles text field - Tab
if(P.EV_new_mb==5 )
{
//P.focus=P.EV_window;
if(P.focus_gut==2)
P.setfocus(P.EV_window,3);
else
if(P.focus_gut==3)
P.setfocus(P.EV_window,2);


return;


}
//close window
if(P.EV_gutn1==1)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.close_window(P.EV_window);
return;
}


}


///end of type==12


//TYPE 6is load skeleton -OK or Return
if(P.EV_window->type == 6)
{

if(P.EV_gutn1==0)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.losefocus();
if(strcmp(P.EV_window->G3[2]->e.c1,"")==0)
strcpy(P.EV_window->G3[2]->e.c1,"temp");

if(strcmp(P.EV_window->G3[3]->e.c1,"")==0)
strcpy(P.EV_window->G3[3]->e.c1,"temp");


saveundo();
strcpy(def_load_skel_file,P.EV_window->G3[3]->e.c1);
strcpy(def_load_skel_dir,P.EV_window->G3[2]->e.c1);

load_skeleton(P.EV_window->G3[2]->e.c1,P.EV_window->G3[3]->e.c1);
P.close_window(P.EV_window);
return;
}

if(P.EV_new_mb==4)
{
P.losefocus();
if(strcmp(P.EV_window->G3[2]->e.c1,"")==0)
strcpy(P.EV_window->G3[2]->e.c1,"temp");

if(strcmp(P.EV_window->G3[3]->e.c1,"")==0)
strcpy(P.EV_window->G3[3]->e.c1,"temp");


saveundo();
strcpy(def_load_skel_file,P.EV_window->G3[3]->e.c1);
strcpy(def_load_skel_dir,P.EV_window->G3[2]->e.c1);
load_skeleton(P.EV_window->G3[2]->e.c1,P.EV_window->G3[3]->e.c1);
P.close_window(P.EV_window);
return;
}

//toggles text field - Tab
if(P.EV_new_mb==5 )
{
//P.focus=P.EV_window;
if(P.focus_gut==2)
P.setfocus(P.EV_window,3);
//P.focus_gut=3;
else
if(P.focus_gut==3)
P.setfocus(P.EV_window,2);
//P.focus_gut=2;

return;


}
//close window
if(P.EV_gutn1==1)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.close_window(P.EV_window);
return;
}


}///end of type==6



//confirm overwrite window
if(P.EV_window->type == 5)
{

if(P.EV_gutn1==0)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
save_skeleton2(P.EV_window->G3[2]->e.c1,P.EV_window->G3[3]->e.c1);
P.close_window(P.EV_window);
return;
}

//close window
if(P.EV_gutn1==1)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.close_window(P.EV_window);
return;
}


}///end of type==5



//TYPE 3is save skeleton -OK or Return
if(P.EV_window->type == 3)
{

if(P.EV_gutn1==0)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.losefocus();
if(strcmp(P.EV_window->G3[2]->e.c1,"")==0)
strcpy(P.EV_window->G3[2]->e.c1,"temp");

if(strcmp(P.EV_window->G3[3]->e.c1,"")==0)
strcpy(P.EV_window->G3[3]->e.c1,"temp");

strcpy(def_save_skel_dir,P.EV_window->G3[2]->e.c1);
strcpy(def_save_skel_file,P.EV_window->G3[3]->e.c1);

save_skeleton(P.EV_window->G3[2]->e.c1,P.EV_window->G3[3]->e.c1);
P.close_window(P.EV_window);
return;
}

if(P.EV_new_mb==4)
{
P.losefocus();
if(strcmp(P.EV_window->G3[2]->e.c1,"")==0)
strcpy(P.EV_window->G3[2]->e.c1,"temp");

if(strcmp(P.EV_window->G3[3]->e.c1,"")==0)
strcpy(P.EV_window->G3[3]->e.c1,"temp");

strcpy(def_save_skel_dir,P.EV_window->G3[2]->e.c1);
strcpy(def_save_skel_file,P.EV_window->G3[3]->e.c1);

save_skeleton(P.EV_window->G3[2]->e.c1,P.EV_window->G3[3]->e.c1);
P.close_window(P.EV_window);
return;
}

//toggles text field - Tab
if(P.EV_new_mb==5 )
{

if(P.focus_gut==2)
P.setfocus(P.EV_window,3);
//P.focus_gut=3;
else
if(P.focus_gut==3)
P.setfocus(P.EV_window,2);
//P.focus_gut=2;

return;


}
//close window
if(P.EV_gutn1==1)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.close_window(P.EV_window);
return;
}


}///end of type==3


//TYPE 4 is quit ok
/*
if(P.EV_window->type == 4)
{

if(P.EV_gutn1==0)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    if (q) q->GetEventOutlet()->Broadcast (cscmdQuit);

//return;
}



//close window
if(P.EV_gutn1==1)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.close_window(P.EV_window);
//printf("TEXT:");
//printf(P.EV_window->G[2]->e.c1);
//printf("\n");
return;
}

}///end of type==4
*/

//TYPE 2 is insert mesh
if(P.EV_window->type == 2)
{

if(P.EV_gutn1==0)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
load_mesh(P.EV_window->G3[2]->e.c1);
//printf("TEXT:");
//printf(P.EV_window->G[2]->e.c1);
//printf("\n");
return;
}
//someone pressed enter on text gut
if(P.EV_new_mb==4)
{
load_mesh(P.EV_window->G3[2]->e.c1);
//printf("TEXT:");
//printf(P.EV_window->G[2]->e.c1);
//printf("\n");
return;


}

//close window
if(P.EV_gutn1==1)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.close_window(P.EV_window);
//printf("TEXT:");
//printf(P.EV_window->G[2]->e.c1);
//printf("\n");
return;
}


}///end of type==2



//type 1 is debug
if(P.EV_window->type == 1)
{



if(P.EV_gutn1==0)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
printf("TEXT:");
printf(P.EV_window->G3[2]->e.c1);
printf("\n");
return;
}
//someone pressed enter on text gut
if(P.EV_new_mb==4)
{
printf("TEXT:");
printf(P.EV_window->G3[2]->e.c1);
printf("\n");
return;


}


if(P.EV_gutn1==1)
if(P.EV_old_mb>0)
if(P.EV_new_mb==0)
{
P.close_window(P.EV_window);
//printf("TEXT:");
//printf(P.EV_window->G[2]->e.c1);
//printf("\n");
return;
}

}
//end of type=1



}

// no start frame...
// just end frame... Even that isn't 100% exact since
// no functions apply direct point and matrix setting
void AppXyzim::play_animation(int start_frame,int end_frame)
{
int i;
csRef<iFile> F1;
char c2[200];

//char c1[100];

//strcpy(c1,"temp");
//strcpy(c2,"lastused");

if(DBG2==1) printf("$$$$$$$$$$$$$$$$$oook\n");
save_animation2("temp","lastused");
if(DBG2==1) printf("$$$$$$$$$$$$$$$$$$$$2oook\n");

//STEP1: Initialize HashTable for this model
//The hash table stores the hierarchical parts by name, and links them to an *iMeshWrapper
//This allows ANIM_XYZ class to act upon a model by moving parts by name linked through
//the model's data stored in the hash table.  Yes this means you can use animations
//not meant for a model by hashing their name what ANIM_XYZ calls... For most cases this
//will just result in gibberish spasms. :)
if(model!=NULL)
delete model;
model=new(XYZ_HASH);

//starts timer counter at 0
model->showing_animation=0;

//inits step to 0
model->cur_step=0;



// THEN SLAP IN THE HASH TABLE ALL EXISTANT MESHES!(mainly should be 1 hierarchy, but who knows)
//iMeshList *EML;
//iMovable *MOV;

int max;
iMeshList *ML;
iMeshWrapper *MW;

ML=engine->GetMeshes();
max=ML->GetCount();

//printf("Engine Meshes: %d\n");
if(DBG2==1) printf("2.4oook\n");

for(i=0;i<max;i++)
{
MW=ML->Get(i);
//if(strcmp(MW->QueryObject()->GetName(),"walls")!=0)
if(strcmp(MW->QueryObject()->GetName(),"cubard")==0
   || strcmp(MW->QueryObject()->GetName(),"002cubard") == 0)
model->hash_hierarchy(MW);
}
//model->print_all();

//STEP #2: Parse information into an animation struct

if(DBG2==1) printf("3oook\n");


strcpy(c2,"/this/data/xyzimation/skeletons/animations/");
strcat(c2,"temp");
strcat(c2,"/");
//printf(c2);
//printf("\n");
FS->ChDir(c2);
F1=FS->Open("lastused",VFS_FILE_READ);
FS->ChDir("/");
if(DBG2==1) printf("4oook\n");

if(XYZ!=NULL)
delete XYZ;

if(F1==NULL)
{
P.perror(c2);
P.perror("No animation file exists there");
return;
}


//printf("um\n");
XYZ=new ANIM_XYZ(F1);


if(DBG2==1) printf("5oook\n");

last_frame=end_frame;
F1=NULL;
}

void AppXyzim::copyit(char *read, char *write)
{
csRef<iFile> F1;
csRef<iFile> F2;
F1=FS->Open(read,VFS_FILE_READ);
F2=FS->Open(write,VFS_FILE_WRITE);
copy_textfile(F1,F2);
F1=NULL;
F2=NULL;
}

void AppXyzim::save_animation2(char *directory, char *filename)
{
csRef<iMeshList> ML;
csRef<iMeshWrapper> MW;
csRef<iFile> F1;
csRef<iFile> F2;
//csRef<iStrVector> SV;
csRef<iStringArray> SV;
int max;
char c1[5000];
char c2[500];
//char c3[200];
//char c5[500];
//bool b;
int i,j;
//int match=0;
//int a;

int orig=1;

i=count_aframe();
if(i==0)
{
if(strcmp(directory,"undo")!=0)
P.perror("No frames of animation to save.");
return;
}


int max2;

max=0;
for(i=1;i<1000;i++)
if(Aframe_time[i]!=-1)
max++;

if(DBG2==1) printf("inside 1\n");

if(max==0)
{
P.perror("NO FRAMES TO SAVE AN ANIMATION WITH");
P.perror("Maybe you just wanted to save a skeleton, use F4 then");

//ML=NULL;
//MW=NULL;
F1=NULL;
F2=NULL;
//SV=NULL;

return;
}

///printf(directory);
//printf("           ");
//printf(filename);
//printf("\n");

strcpy(c2,"/this/data/xyzimation/skeletons/animations/");
strcat(c2,directory);
strcat(c2,"/");

//printf(c2);
//printf("\n");


FS->ChDir(c2);
FS->WriteFile(filename," ",1);
F1=FS->Open(filename,VFS_FILE_WRITE);

if(DBG2==1) printf("inside 2\n");




//resets hash table.
SA_max=0;





strcpy(c1,"<frames>");
F1->Write(c1,strlen(c1));
sprintf(c1,"%d",max);
F1->Write(c1,strlen(c1));
strcpy(c1,"</frames>\n");
F1->Write(c1,strlen(c1));



for(j=1;j<1000;j++)
if(Aframe_time[j]!=-1)
{
sprintf(c2,"%d",j);

//printf(c2);
//printf("-THE j for looping AFRAMES\n");


//store
if(orig==0)
{
sprintf(c2,"%d",j);
load_skeleton("animations/working",c2);
ML=engine->GetMeshes();
max=ML->GetCount();

sprintf(c2,"%d",max);

//printf(c2);
//printf("-MAX GET COUNT\n");




if(DBG2==1) printf("inside 3\n");



max2=0;
for(i=0;i<max;i++)
{
MW=ML->Get(i);
//if(strcmp(MW->QueryObject()->GetName(),"walls")!=0)
if(strcmp(MW->QueryObject()->GetName(),"cubard")==0
   || strcmp(MW->QueryObject()->GetName(),"002cubard")==0 )
max2=max2+count_animation_sub1(MW);

if(DBG2==1) printf("inside 3.3\n");
}
//sprintf(c2,"%d",max2);

//printf(c2);
//printf("-MAX2 COUNT ANIMATION SUBS\n");

char c3[300];


strcpy(c1,"<moved>");
sprintf(c3,"%d",max2);
strcat(c1,c3);
strcat(c1,"</moved>\n");

if(DBG2==1) printf(c1);
if(DBG2==1) printf("\n");



F1->Write(c1,strlen(c1));
if(DBG2==1) printf("inside 3.4\n");

//printf(c1);
//printf("\n");

strcpy(c1,"<time>");
F1->Write(c1,strlen(c1));
sprintf(c1,"%d",Aframe_time[j]);
F1->Write(c1,strlen(c1));
strcpy(c1,"</time>\n");
F1->Write(c1,strlen(c1));

//printf(c1);
//printf("\n");
if(DBG2==1) printf("save kids org0:::\n");

for(i=0;i<max;i++)
{
MW=ML->Get(i);
//if(strcmp(MW->QueryObject()->GetName(),"walls")!=0)
////IF YOU DO A multimesh, all meshes need to be named here...


if(strcmp(MW->QueryObject()->GetName(),"cubard")==0
|| strcmp(MW->QueryObject()->GetName(),"002cubard")==0)
{
save_animation_sub1(MW,F1);
printf("=====\n");
}

}
if(DBG2==1) printf("end of kids org0:::\n");


}//end of orig=0


if(orig==1)
{
orig=0;
//Save original stance data:
//LOAD WORLD DATA
printf("-load---------\n");
sprintf(c2,"%d",j);
load_skeleton("animations/working",c2);
printf("-loaded------\n");

//read from internal data
//Save the crud: MeshObj name/pos/matrix

//int max;
//int i;
ML=engine->GetMeshes();
max=ML->GetCount();

max2=0;
for(i=0;i<max;i++)
{
MW=ML->Get(i);
//if(strcmp(MW->QueryObject()->GetName(),"walls")!=0)
if(strcmp(MW->QueryObject()->GetName(),"cubard")==0
|| strcmp(MW->QueryObject()->GetName(),"002cubard")==0)
max2=max2+count_children(MW);
}

printf("---\n");

strcpy(c1,"<moved>");
F1->Write(c1,strlen(c1));
sprintf(c1,"%d",max2);
F1->Write(c1,strlen(c1));
strcpy(c1,"</moved>\n");
F1->Write(c1,strlen(c1));


strcpy(c1,"<time>");
F1->Write(c1,strlen(c1));
sprintf(c1,"%d",Aframe_time[j]);
F1->Write(c1,strlen(c1));
strcpy(c1,"</time>\n");
F1->Write(c1,strlen(c1));


//THEN YOU NEED TO SAVE
//EVERY MESHFACT BY NAME, POS, MATRIX
//ALSO keep track of name, pos, matrix to see if they changed.
if(DBG2==1) printf("save kids org1:::\n");

for(i=0;i<max;i++)
{
MW=ML->Get(i);
//if(strcmp(MW->QueryObject()->GetName(),"walls")!=0)
////MULTI MESHES NEED TO START WITH CUBARD
if(strcmp(MW->QueryObject()->GetName(),"cubard")==0
|| strcmp(MW->QueryObject()->GetName(),"002cubard")==0)
save_animation_sub1(MW,F1);
}

if(DBG2==1) printf("Outside of kids, end of orig==1\n");

}//end of orig==1





//printf("----------\n");
//ML=NULL;
//printf("---------\n");
//MW=NULL;
//printf("--------\n");
//F1=NULL;
//printf("-------\n");
F2=NULL;
//printf("------\n");
//SV=NULL;



}



F1=NULL;
FS->ChDir("/");
}


int AppXyzim::count_animation_sub1(iMeshWrapper *MWP)
{
csRef<iMeshList> ML;
csRef<iMeshFactoryWrapper> MFW;
csRef<iFile> F2;
csRef<iMovable> MOVE;
csRef<iMeshWrapper> MW2;

  csRef <iSceneNode> SN;
  csRef <iSceneNodeArray> SNA;


  SN = MWP->QuerySceneNode ();
  SNA = SN->GetChildrenArray ();
  int max;
  max = SNA->GetSize ();




int i;
//char c1[300];
//char c5[300];
char cname[300];
//int a;
int hashed;
//int ok;
int ok1,ok2;

int counting;

counting=0;

ok1=0;
ok2=0;
//char c11[300];
//char c12[300];
//char c13[300];


strcpy(cname,MWP->QueryObject()->GetName());

hashed=is_hashed(cname);
printf("123123\n");
if(DBG2==1) printf("Cname:");
if(DBG2==1) printf(cname);
if(DBG2==1) printf("\n");


if(hashed==-1)
{
printf("ERROR, hashed=-1 when should be hashed from first run- possibly new mesh \n");
ok1=1;
ok2=1;
counting=1;
}


MOVE=MWP->GetMovable();
csVector3 v1;

v1=MOVE->GetPosition();

if(ok1==0)
{
if(SA_v[hashed].x!=v1.x)
ok1=1;
if(SA_v[hashed].y!=v1.y)
ok1=1;
if(SA_v[hashed].z!=v1.z)
ok1=1;
}

csMatrix3 m1=MOVE->GetTransform().GetT2O();

if(ok2==0)
{
if(SA_m[hashed].m11!=m1.m11)
ok2=1;
if(SA_m[hashed].m12!=m1.m12)
ok2=1;
if(SA_m[hashed].m13!=m1.m13)
ok2=1;
if(SA_m[hashed].m21!=m1.m21)
ok2=1;
if(SA_m[hashed].m22!=m1.m22)
ok2=1;
if(SA_m[hashed].m23!=m1.m23)
ok2=1;
if(SA_m[hashed].m31!=m1.m31)
ok2=1;
if(SA_m[hashed].m32!=m1.m32)
ok2=1;
if(SA_m[hashed].m33!=m1.m33)
ok2=1;
}

if(ok1==1 || ok2==1)
counting=1;

printf("--------------------\n");
//ML=MWP->GetChildren();
//max=ML->GetCount();

for(i=0;i<max;i++)
{

counting=counting+count_animation_sub1(SNA->Get (i)->QueryMesh ());
      iObject *O1=SNA->Get (i)->QueryMesh ()->QueryObject();
      char c1[500];
      strcpy(c1,O1->GetName());
      printf(c1);
      printf(" - counting %d\n",counting);



}
//ML=NULL;
//MFW=NULL;
F2=NULL;
//MOVE=NULL;
//MW2=NULL;
return counting;
}


void AppXyzim::save_animation_sub1(iMeshWrapper *MWP,iFile *F1)
{
csRef<iMeshList> ML;
csRef<iMeshFactoryWrapper> MFW;
csRef<iFile> F2;
csRef<iMovable> MOVE;
csRef<iMeshWrapper> MW2;


int i;
char c1[300];
char c5[300];
char cname[300];
//int a;
int hashed;
//int ok;
int ok1,ok2;
  csRef <iSceneNode> SN;
  csRef <iSceneNodeArray> SNA;


  SN = MWP->QuerySceneNode ();
  SNA = SN->GetChildrenArray ();
  int max;
  max = SNA->GetSize ();



ok1=0;
ok2=0;
char c11[300];
//char c12[300];
//char c13[300];


//printf("here\n");

strcpy(c1,"<meshobj name=\"");
strcpy(cname,MWP->QueryObject()->GetName());
strcat(c1,MWP->QueryObject()->GetName());
strcat(c1,"\">\n");

//only write name if changed(ok=1)
strcpy(c11,c1);

printf(cname);
printf(" - hashing\n");

hashed=is_hashed(cname);

if(hashed==-1)
{
ok1=1;
ok2=1;
}



MOVE=MWP->GetMovable();
csVector3 v1;

v1=MOVE->GetPosition();

if(ok1==0)
{
if(SA_v[hashed].x!=v1.x)
ok1=1;
if(SA_v[hashed].y!=v1.y)
ok1=1;
if(SA_v[hashed].z!=v1.z)
ok1=1;
}

csMatrix3 m1=MOVE->GetTransform().GetT2O();

if(ok2==0)
{
if(SA_m[hashed].m11!=m1.m11)
ok2=1;
if(SA_m[hashed].m12!=m1.m12)
ok2=1;
if(SA_m[hashed].m13!=m1.m13)
ok2=1;
if(SA_m[hashed].m21!=m1.m21)
ok2=1;
if(SA_m[hashed].m22!=m1.m22)
ok2=1;
if(SA_m[hashed].m23!=m1.m23)
ok2=1;
if(SA_m[hashed].m31!=m1.m31)
ok2=1;
if(SA_m[hashed].m32!=m1.m32)
ok2=1;
if(SA_m[hashed].m33!=m1.m33)
ok2=1;
}
////meshobj name
//char SA_c1[1000][200];
//corresponding vector
//csVector3 SA_v[1000];
//corresponding matrix
//csMatrix3 SA_m[1000];


if(ok1!=0 || ok2!=0)
{
///printf("pass\n");

F1->Write(c11,strlen(c1));
//printf(c11);
//printf("\n");


strcpy(c5,"<move>\n");
F1->Write(c5,strlen(c5));

if(ok1==1)
{
sprintf(c5,"<v x=\"%f\" y=\"%f\" z=\"%f\" />\n",v1.x,v1.y,v1.z);
F1->Write(c5,strlen(c5));
}

if(ok2==1){
strcpy(c5,"<matrix>\n");
F1->Write(c5,strlen(c5));
sprintf(c5,"<m11> %f </m11>\n",m1.m11);
F1->Write(c5,strlen(c5));
sprintf(c5,"<m12> %f </m12>\n",m1.m12);
F1->Write(c5,strlen(c5));
sprintf(c5,"<m13> %f </m13>\n",m1.m13);
F1->Write(c5,strlen(c5));
sprintf(c5,"<m21> %f </m21>\n",m1.m21);
F1->Write(c5,strlen(c5));
sprintf(c5,"<m22> %f </m22>\n",m1.m22);
F1->Write(c5,strlen(c5));
sprintf(c5,"<m23> %f </m23>\n",m1.m23);
F1->Write(c5,strlen(c5));
sprintf(c5,"<m31> %f </m31>\n",m1.m31);
F1->Write(c5,strlen(c5));
sprintf(c5,"<m32> %f </m32>\n",m1.m32);
F1->Write(c5,strlen(c5));
sprintf(c5,"<m33> %f </m33>\n",m1.m33);
F1->Write(c5,strlen(c5));
strcpy(c5,"</matrix>\n");
F1->Write(c5,strlen(c5));
}

strcpy(c5,"</move>\n");
F1->Write(c5,strlen(c5));
}



if(hashed==-1)
{
//printf("hashing:");
//printf(cname);
//printf("\n");
strcpy(SA_c1[SA_max],cname);
SA_v[SA_max]=v1;
SA_m[SA_max]=m1;
SA_max++;
}
else
{

//printf("Hashed-");
//printf(SA_c1[hashed]);
//printf("-Hashed\n");
strcpy(SA_c1[hashed],cname);
SA_v[hashed]=v1;
SA_m[hashed]=m1;
}


//ML=MWP->GetChildren();
//max=ML->GetCount();
for(i=0;i<max;i++)
save_animation_sub1(SNA->Get (i)->QueryMesh (),F1);


if(ok1!=0 || ok2!=0)
{
strcpy(c1,"</meshobj>\n");
F1->Write(c1,strlen(c1));
}


//ML=NULL;
//MFW=NULL;
F2=NULL;
//MW2=NULL;


}

//counts self!
int AppXyzim::count_children(iMeshWrapper *MWP)
{
csRef<iMeshList> ML;
int i;
int a;
  csRef <iSceneNode> SN;
  csRef <iSceneNodeArray> SNA;
  SN = MWP->QuerySceneNode ();
  SNA = SN->GetChildrenArray ();
  int max;
  max = SNA->GetSize ();

a=1;



for(i=0;i<max;i++)
a=a+count_children(SNA->Get (i)->QueryMesh ());

return a;


}

//returns index if it is.
//Otherwise returns -1
int AppXyzim::is_hashed(char *c1)
{
int i;
printf("max %d\n",SA_max);
for(i=0;i<SA_max;i++)
if(strcmp(c1,SA_c1[i])==0)
return i;

return -1;
}


char *AppXyzim::check_expand_string(char *c2)
{
//char c3[200];
static int last=0;
printf("Checking expand string\n");

if (kbd->GetKeyState ('`'))
{
//P.focus=NULL;
P.close_all();
return("");
}

//printf("heey\n");

//strcpy(c3,c2);
if (kbd->GetKeyState ('q'))
{
if(last!=1)
P.send_focus('q');
last=1;
return("");
}

if (kbd->GetKeyState ('w'))
{
if(last!=2)
P.send_focus('w');
last=2;
return("");
}
if (kbd->GetKeyState ('e'))
{
if(last!=3)
P.send_focus('e');
last=3;
return("");
}
printf("before r");
if (kbd->GetKeyState ('r'))
{
printf("sending r\n");
if(last!=4)
P.send_focus('r');
printf("After sent r\n");
last=4;
return("");
}
printf("after r");
if (kbd->GetKeyState ('t'))
{
if(last!=5)
P.send_focus('t');

last=5;
return("");
}
if (kbd->GetKeyState ('y'))
{
if(last!=6)
P.send_focus('y');
last=6;
return("");
}
if (kbd->GetKeyState ('u'))
{
if(last!=7)
P.send_focus('u');
last=7;
return("");}
if (kbd->GetKeyState ('i'))
{
if(last!=8)
P.send_focus('i');
last=8;
return("");}
if (kbd->GetKeyState ('o'))
{
if(last!=9)
P.send_focus('o');
last=9;
return("");}
if (kbd->GetKeyState ('p'))
{
printf("Pressed P\n");
if(last!=10)
P.send_focus('p');
printf("Pressed P\n");
last=10;
return("");}
if (kbd->GetKeyState ('1'))
{
if(last!=11)
P.send_focus('1');
last=11;
return("");}
if (kbd->GetKeyState ('2'))
{
if(last!=12)
P.send_focus('2');
last=12;
return("");}
if (kbd->GetKeyState ('3'))
{
if(last!=13)
P.send_focus('3');
last=13;
return("");}
if (kbd->GetKeyState ('4'))
{
if(last!=14)
P.send_focus('4');
last=14;
return("");}
if (kbd->GetKeyState ('5'))
{
if(last!=15)
P.send_focus('5');
last=15;
return("");}
if (kbd->GetKeyState ('6'))
{
if(last!=16)
P.send_focus('6');
last=16;
return("");}
if (kbd->GetKeyState ('7'))
{
if(last!=17)
P.send_focus('7');
last=17;
return("");}
if (kbd->GetKeyState ('8'))
{
if(last!=18)
P.send_focus('8');
last=18;
return("");}
if (kbd->GetKeyState ('9'))
{
if(last!=19)
P.send_focus('9');
last=19;
return("");}
if (kbd->GetKeyState ('0'))
{
if(last!=20)
P.send_focus('0');
last=20;
return("");}



if (kbd->GetKeyState ('a'))
{
if(last!=21)
P.send_focus('a');
last=21;
return("");}
if (kbd->GetKeyState ('s'))
{
if(last!=22)
P.send_focus('s');
last=22;
return("");}
if (kbd->GetKeyState ('d'))
{
if(last!=23)
P.send_focus('d');
last=23;
return("");}
if (kbd->GetKeyState ('f'))
{
if(last!=24)
P.send_focus('f');
last=24;
return("");}
if (kbd->GetKeyState ('g'))
{
if(last!=25)
P.send_focus('g');
last=25;
return("");}
if (kbd->GetKeyState ('h'))
{
if(last!=26)
P.send_focus('h');
last=26;
return("");}
if (kbd->GetKeyState ('j'))
{
if(last!=27)
P.send_focus('j');
last=27;
return("");}
if (kbd->GetKeyState ('k'))
{
if(last!=28)
P.send_focus('k');
last=28;
return("");}
if (kbd->GetKeyState ('l'))
{
if(last!=29)
P.send_focus('l');
last=29;
return("");}

if (kbd->GetKeyState ('z'))
{
if(last!=30)
P.send_focus('z');
last=30;
return("");}
if (kbd->GetKeyState ('x'))
{
if(last!=31)
P.send_focus('x');
last=31;
return("");}
if (kbd->GetKeyState ('c'))
{
if(last!=32)
P.send_focus('c');
last=32;
return("");}
if (kbd->GetKeyState ('v'))
{
if(last!=33)
P.send_focus('v');
last=33;
return("");}
if (kbd->GetKeyState ('b'))
{
if(last!=34)
P.send_focus('b');
last=34;
return("");}
if (kbd->GetKeyState ('n'))
{
if(last!=35)
P.send_focus('n');
last=35;
return("");}
if (kbd->GetKeyState ('m'))
{
if(last!=36)
P.send_focus('m');
last=36;
return("");}
if (kbd->GetKeyState (','))
{
if(last!=37)
P.send_focus(',');
last=37;
return("");}
if (kbd->GetKeyState ('.'))
{
if(last!=38)
P.send_focus('.');
last=38;
return("");}
if (kbd->GetKeyState ('/'))
{
if(last!=39)
P.send_focus('/');
last=39;
return("");}

if(kbd->GetKeyState (CSKEY_DEL) || kbd->GetKeyState (CSKEY_BACKSPACE) )
{

if(last!=40)
P.send_focus(1);
last=40;
return("");}

if(kbd->GetKeyState (CSKEY_ENTER))
{

if(last!=41)
P.send_focus(2);
last=41;
return("");}

if(kbd->GetKeyState (' '))
{

if(last!=42)
P.send_focus(' ');
last=42;
return("");}


if(kbd->GetKeyState (CSKEY_TAB))
{

if(last!=43)
P.send_focus(3);
last=43;
return("");}



last=0;
return("");
}

void AppXyzim::load_animation(char *directory, char *filename)
{
int i;
csRef<iFile> F1;
char c2[200];
bool b;
char c1[100];
//  csRef <iSceneNode> SN;
//  csRef <iSceneNodeArray> SNA;

FS->Sync();

//printf("looooooading\n");

//STEP1: Initialize HashTable for this model
//The hash table stores the hierarchical parts by name, and links them to an *iMeshWrapper
//This allows ANIM_XYZ class to act upon a model by moving parts by name linked through
//the model's data stored in the hash table.  Yes this means you can use animations
//not meant for a model by hashing their name what ANIM_XYZ calls... For most cases this
//will just result in gibberish spasms. :)
if(model!=NULL)
delete model;
model=new(XYZ_HASH);

//starts timer counter at 0
model->showing_animation=0;

//inits step to 0
model->cur_step=0;



// THEN SLAP IN THE HASH TABLE ALL EXISTANT MESHES!(mainly should be 1 hierarchy, but who knows)
//iMeshList *EML;
//iMovable *MOV;

int max;
iMeshList *ML;
iMeshWrapper *MW;

ML=engine->GetMeshes();
max=ML->GetCount();

for(i=0;i<max;i++)
{
MW=ML->Get(i);
//if(strcmp(MW->QueryObject()->GetName(),"walls")!=0)
if(strcmp(MW->QueryObject()->GetName(),"cubard")==0
|| strcmp(MW->QueryObject()->GetName(),"002cubard")==0)
model->hash_hierarchy(MW);
}
//model->print_all();


printf("ook\n");

//STEP #2: Parse information into an animation struct

strcpy(c2,"/this/data/xyzimation/skeletons/animations/");
strcat(c2,directory);
strcat(c2,"/");
//printf(c2);
//printf("\n");
FS->ChDir(c2);


printf(c2);
printf("\n");

FS->ChDir(c2);
b=FS->Exists(filename);
printf("bool %d\n",b);

if(b==0)
{
strcpy(c1,"File not found");
strcat(c2,filename);
strcat(c1,c2);
P.perror(c1);
FS->ChDir("/");
delete model;
model=NULL;
F1=NULL;
return;
}

printf("ook2\n");

F1=FS->Open(filename,VFS_FILE_READ);
FS->ChDir("/");


if(XYZ!=NULL)
delete XYZ;

if(F1==NULL)
{
P.perror(c2);
P.perror("No animation file exists there");
delete model;
model=NULL;
return;
}

printf("um\n");
XYZ=new ANIM_XYZ(F1);
printf("hmmmm\n");


//PROBABLY SHOULD BE SOME AUTO WAY OF INITIALIZING every vector/matrix of a hierarchy

//loading
ugly_global_step=0;
model->showing_animation=-2;
for(i=0;i<1000;i++)
Aframe_time[i]=-1;
printf("ook3\n");

last_frame=9999;
F1=NULL;

printf("ook4\n");

}
















































void AppXyzim::save_animation(char *directory, char *filename)
{
//iFile *F1;
//csRef<iStrVector> SV;
csRef<iStringArray> SV;
int max;
char c1[500];
char c2[500];
//bool b;
int i;
//int match=0;

i=count_aframe();
if(i==0)
{
if(strcmp(directory,"undo")!=0)
P.perror("No frames of animation to save.");
return;
}

FS->Sync();
strcpy(c1,FS->GetCwd());

//printf("%d",max);
strcpy(c2,"/this/data/xyzimation/skeletons/animations/");
strcat(c2,directory);
strcat(c2,"/");
SV=FS->FindFiles(c2);
max=SV->Length();
strcat(c2,filename);

for(i=0;i<max;i++)
{
strcpy(c1,SV->Get(i));
if(strcmp(directory,"undo")!=0)
if(strcmp(c1,c2)==0)
{
popup *which=P.open(10);
strcpy(which->G3[2]->e.c1,directory);
strcpy(which->G3[3]->e.c1,filename);


FS->ChDir("/");
//SV=NULL;
return;
}
//printf(c1);2
//printf("           ");
//printf(c2);
//printf("\n");
}
save_animation2(directory,filename);
//SV=NULL;
}






//Keep in mind your animations may produce particle effects, or may create or delete meshes
//At the moment, this only saves the translations of the initial model(frame1)
//Later as my game comes along, I can put extra work to get different sorts of animations
//Consult with me before you mod this file, and the .xyz standard James_Sager_PA@yahoo.com


//Basically it saves every mesh position and matrix, except when its unchanged.
//The first frame has all data, just like a skeleton.
//The next frames only do what changed
//Its quite important to save only what changes, as this animation is on the fly.
//You get position and matrix rotations ONLY when you need em.
//So while this code may seem sloppy as fuck, it produces streamlined results.
//The only thing I wonder is ROTX,ROTY,ROTZ format over a matrix rotation format

//Format: 1rst is <frames> How many Frames </frames>

///      FOR i =0 i < Frames
/////////////// <moved> How many meshes changed </moved>
/////////////// <time>Time to translate this over </time>
///////////////////////FOR j=0;j<moved
//////////////////////// <name> "Object Name" </name>
///////////////////////  <move> Stored in CS fashion </move>

//THe first frame has all the object names that will be modded...
//Normally I'd assume this only involves a hierarchical mesh




void AppXyzim::load_mesh(char *c1)
{
char c2[300];
//LOOK ITS THE CRAPTACULAR static int rotation of loading
//static int rotation=0;



//Would be better if prompted what piece you want loaded instead of cycling.
//But I'm not using this crap much.

//if(rotation==0)
//strcpy(c2,"/this/data/xyzimation/tools/meshfacts/RobotArm");
//if(rotation==1)
//strcpy(c2,"/this/data/xyzimation/tools/meshfacts/RobotBody");
//if(rotation==2)
//strcpy(c2,"/this/data/xyzimation/tools/meshfacts/RobotBut");
//if(rotation==3)
//strcpy(c2,"/this/data/xyzimation/tools/meshfacts/RobotFoot");
//if(rotation==4)
//strcpy(c2,"/this/data/xyzimation/tools/meshfacts/RobotHand");
//if(rotation==5)
//strcpy(c2,"/this/data/xyzimation/tools/meshfacts/RobotHead");
//if(rotation==6)






strcpy(c2,"/this/data/xyzimation/tools/meshfacts/");
strcat(c2,c1);
loader->LoadMapFile (c2, false);
Load_Plugins(c2);
//  csRef<iMeshFactoryWrapper> imeshfact;
//  imeshfact=loader->LoadMeshObjectFactory (c2);

//strcpy(c2,"/this/data/xyzimation/tools/my_meshfacts/");
//strcat(c2,c1);
//loader->LoadMapFile (c2, false);
//Load_Plugins(c2);


/*
  if (imeshfact == NULL)
{
strcpy(c2,"/this/data/xyzimation/tools/my_meshfacts/");
strcat(c2,c1);

//  imeshfact=loader->LoadMeshObjectFactory (c2);


  if (imeshfact == NULL)
{

    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.simple2",
    	"Error loading mesh object factory!");
    return;
}

  }
*/
  /*
  csRef<iMeshWrapper> sprite (engine->CreateMeshWrapper (
  	imeshfact, "MySprite", room,
	csVector3 (4, 2, 12)));


csMatrix3 m; m.Identity ();
  //extra sizens it :)
//  m *= 1;



sprite->GetMovable ()->SetTransform (m);
  sprite->GetMovable ()->UpdateMove ();
//csRef<iSprite3DState> spstate (
//	SCF_QUERY_INTERFACE (sprite->GetMeshObject (), iSprite3DState));
//spstate->SetAction ("default");
//spstate->SetMixMode (CS_FX_SETALPHA (.5));
  // The following two calls are not needed since CS_ZBUF_USE and
  // Object render priority are the default but they show how you
  // can do this.
sprite->SetZBufMode (CS_ZBUF_USE);
sprite->SetRenderPriority (engine->GetObjectRenderPriority ());
//sprite->DeferUpdateLighting (CS_NLIGHT_STATIC|CS_NLIGHT_DYNAMIC, 10);






paint_all_white();

sg1=sprite;
selected(sg1);
*/

}

