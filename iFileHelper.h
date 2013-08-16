	 /*
	 Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs
*/

#ifndef __iFileHelper__H__
#define __iFileHelper__H__

#include "crystalspace.h"



//for getting databack in move
struct M_V_MOVE
{
//1 means there is a vector
//-1 means no vector
  int is_v;
  csVector3 v;
//1 means there is a matrix
//-1 means no matrix
  int is_m;
  csMatrix3 m;

};


void report_hacker (char *error);

char *ReadLine (iFile * F1);
char *ReadLine2 (iFile * F1);


//returns the next meshfact and increments the filepos
char *Parse_meshfact (iFile * F1);
char *Parse_meshfact (char *c1);




//returns the next plugin and increments the filepos
//NOT USED AT ALL THOUGH! old functions
char *Parse_plugin (iFile * F1);
char *Parse_plugin (char *c1);

//removes factory from line
char *Parse_plugin_factory (iFile * f1);

//writes up til </meshfact> line
//of course this ghetto function only works if nothing is on the same lines
//as the <meshfact> </meshfact>
void Write_Extra (iFile * F1, iFile * F2);


void copy_textfile (iFile * read, iFile * write);


//These parses are for loading up ANIM_XYZ
//These dumb parse files assume crap is on the same line
//I suck with text files, so it works on stuff saved in the format...
//But if you have shit on different lines, crap will probably crash :(
// Better parsing could be coded *shrug*
int Parse_frames (iFile * F1);
int Parse_frames (char *c1);

//move and moved almost have the same name, which is sorta dumb of me.
int Parse_moved (iFile * F1);
int Parse_moved (char *c1);

int Parse_time (iFile * F1);
int Parse_time (char *c1);

char *Parse_meshobj (iFile * F1);
char *Parse_meshobj (const char *c1);

M_V_MOVE Parse_move (iFile * F1);
M_V_MOVE Parse_vector (char *c1);
//multi lined, needs to read from file
M_V_MOVE Parse_matrix (char *c1, iFile * F1);




//////////////////////////////////////////////
//////////////////////////////////////////////
///////////////////// KYUFU SPECIFICS
//////////////////////////////////////////////
//////////////////////////////////////////////
struct ee2
{
  char c1[200];
  int type;

/*
type
0-EOF
1-monster
2-object
3-special object
4-platform
5-specialplatform
6-vehicle
*/

};
void initialize_entities (char *filename);


ee2 Parse_Entity (iFile * F1);

char *Parse_monster (char *c1);
char *Parse_object (char *c1);
char *Parse_sobject (char *c1);
char *Parse_platform (char *c1);
char *Parse_splatform (char *c1);
char *Parse_vehicle (char *c1);


char *Parse_sector (iFile * F1);
char *Parse_sector (char *c1);

csVector3 Parse_pos (iFile * F1);
csVector3 Parse_pos (char *c1);




#endif
