	 /*
	 Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/


#include "cssysdef.h"
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
#include "iutil/string.h"

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
#include "imap/saver.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "csutil/cmdhelp.h"



#include "csgeom/transfrm.h"

//jim hmods
#include "ivaria/conin.h"
#include "ivaria/conout.h"


#include "iengine/campos.h"
//#include "csengine/campos.h"

//jim mod2s
//#include "csinput.h"

#include "cstool/cspixmap.h"

// Includes
#include "igraphic/image.h"

#include "iFileHelper.h"


#include "string.h"
#include "xyz_hash.h"
#include "crystalspace.h"


//Really hashing is only necessary if you have a TOOOOOOOOOOOOOOOOOOOON of meshes in your
//skeleton, but I coded up for fuck's sake since its nice anyway.

//returns -1 error
//returns 1 ok
//returns 2 already there
int XYZ_HASH::add(const char *c1,iMeshWrapper *MW)
{
int len;
int hash_index;
len=strlen(c1);
if(len==0)
{
printf("Error: hashing with no string \n");
return -1;
}
hash_index=(c1[len-1]-97)%BINZ;
if(hash_index<0)
hash_index=hash_index*-1;
HASHNODE *H=insert(hash_index,c1);
if(H==NULL)
return 2;
H->MW=MW;
//important to store old position, so each step youcan translate right
H->v=MW->GetMovable()->GetPosition();
H->m=MW->GetMovable()->GetTransform().GetT2O();
return 1;
}//end of add





int XYZ_HASH::del(const char *c1)
{
int len;
int hash_index;
len=strlen(c1);

if(len==0)
{
printf("Error: DEL:  hashing with no string \n");
return -1;
}
hash_index=(c1[len-1]-97)%BINZ;
if(hash_index<0)
hash_index=hash_index*-1;

remove(hash_index,c1);
return 1;
}







HASHNODE *XYZ_HASH::remove(int hash_index,const char *c1)
{
HASHNODE *H;
HASHNODE *OH;
H=Bin[hash_index];

if(H==NULL)
return NULL;

while (H!=NULL)
{
if(strcmp(H->name,c1)==0)
{

if(H->prev!=NULL)
H->prev->next=H->next;
else
Bin[hash_index]=H->next;

if(H->next!=NULL)
H->next->prev=H->prev;

delete(H);
return NULL;
}

OH=H;
H=H->next;

}

return NULL;
}






//returns NULL if already present
HASHNODE *XYZ_HASH::insert(int hash_index,const char *c1)
{
HASHNODE *H;
HASHNODE *OH;
//printf("index%d\n",hash_index);

H=Bin[hash_index];

if(H==NULL)
{
//printf("H==NULL\n");
Bin[hash_index]=new(HASHNODE);
//Bin[hash_index]->name=new(char[strlen(c1)+1]);
//strcpy(Bin[hash_index]->name,c1);
Bin[hash_index]->name=strdup(c1);

Bin[hash_index]->next=NULL;
Bin[hash_index]->prev=NULL;
//printf("ok1\n");

return Bin[hash_index];

}


while (H!=NULL)
{
if(strcmp(H->name,c1)==0)
return NULL;

OH=H;
H=H->next;
}
HASHNODE *N=new(HASHNODE);
//N->name=new(char[strlen(c1)+1]);
//strcpy(N->name,c1);
N->name=strdup(c1);

N->next=NULL;
N->prev=OH;
OH->next=N;

return N;
}

HASHNODE *XYZ_HASH::find_node(const char *c1){
int len;
int hash_index;
len=strlen(c1);

if(len==0)
{
printf("Error:Find_node hashing with no string \n");
return NULL;
}

hash_index=(c1[len-1]-97)%BINZ;
if(hash_index<0)
hash_index=hash_index*-1;

HASHNODE *H=locate(hash_index,c1);
return(H);

}

iMeshWrapper *XYZ_HASH::find(const char *c1)
{
int len;
int hash_index;
len=strlen(c1);

if(len==0)
{
printf("Error:FIND: hashing with no string \n");
return NULL;
}

hash_index=(c1[len-1]-97)%BINZ;
if(hash_index<0)
hash_index=hash_index*-1;

HASHNODE *H=locate(hash_index,c1);
if(H!=NULL)
return(H->MW);
return NULL;
}



HASHNODE *XYZ_HASH::locate(int hash_index,const char *c1)
{
HASHNODE *H;
HASHNODE *OH;
H=Bin[hash_index];

if(H==NULL)
return NULL;

while (H!=NULL)
{
if(strcmp(H->name,c1)==0)
return H;

OH=H;
H=H->next;
}

return NULL;
}










XYZ_HASH::XYZ_HASH ()
{
int i;
for(i=0;i<BINZ;i++)
Bin[i]=NULL;


showing_animation=-1;
cur_step=-1;
}



void XYZ_HASH::print(int index){
HASHNODE *H;
H=Bin[index];

while(H!=NULL)
{
printf(H->name);
printf(" : ");
H=H->next;
}

}


void XYZ_HASH::print_all(void){
int i;
for(i=0;i<BINZ;i++)
{
printf("Index %d=",i);
print(i);
printf("\n");
}
}


//////////////////////////////////////////MWP= MESH_WRAPPER_PARRENT, MAD WRAPPER P in the hizzouse
void XYZ_HASH::hash_hierarchy(iMeshWrapper *MWP)
{
csRef<iMeshList> ML;
int i;
int max;
//int a;
//char *c;
//int len;
csRef <iSceneNodeArray> SNA;



//SWEET easy hashing :) With a little boot to the head action to force "const char*" into "char *"

printf(MWP->QueryObject ()->GetName ());
printf("\n");
  add (MWP->QueryObject ()->GetName (), MWP);

//SHOW EVERY NAME HASHED
 // printf (MWP->QueryObject ()->GetName ());
//  printf ("\n");
//END OF PRINT :)

//then do hierarchy below :)

//oldcode  ML = MWP->GetChildren ();
SNA=MWP->QuerySceneNode ()->GetChildrenArray();



//oldcode  max = ML->GetCount ();
max=SNA->GetSize();

//Nice to have some sweet recursion after all that bullshit coding I did on the maker.

  for (i = 0; i < max; i++)
     //  printf("%d\n",i);
//oldcode    hash_hierarchy (ML->Get (i));
hash_hierarchy(SNA->Get(i)->QueryMesh());


}


void XYZ_HASH::del_all(void)
{
int i;
HASHNODE *H,*OH;

for(i=0;i<BINZ;i++)
{
//printf("beh\n");
//printf("Index %d=",i);
//print(i);
H=Bin[i];
while(H!=NULL)
{
//printf(H->name);
//printf("\n");
//printf(" : ");
//while(H->next!=NULL)
//{
OH=H;
H=H->next;
delete OH;
//}//end while
//delete H;



}//end H!=NULL
}//reloop i


}//all deleted





