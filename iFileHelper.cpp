

/*
Copyright(C) Written by James Sager III 2003-2013
Compiles under CrystalSpace 3d: http://www.crystalspace3d.org/main/Main_Page
Tutorial Demo: https://www.youtube.com/watch?v=SBbDRRGdKvs

#include "cssysdef.h"
#include "iutil/vfs.h"
#include "csutil/cscolor.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
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
#include "iengine/texture.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/material.h"

#include "igeom/polymesh.h"

#include "imesh/sprite3d.h"
#include "imesh/object.h"






#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"
#include "ivideo/txtmgr.h"
#include "ivideo/texture.h"
#include "ivideo/material.h"
#include "ivideo/fontserv.h"
#include "igraphic/imageio.h"
#include "imap/saver.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "ivaria/dynamics.h"
#include "csutil/cmdhelp.h"



#include "csgeom/transfrm.h"

//jim hmods
#include "ivaria/conin.h"
#include "ivaria/conout.h"


#include "iengine/campos.h"

//jim mod2s
//#include "csinput.h"

#include "cstool/cspixmap.h"

// Includes
#include "igraphic/image.h"

#include "string.h"
*/
#include "universals.h"
#include "crystalspace.h"
#include "iFileHelper.h"



//c#include "roam.h"
//c#include "monster.h"

//assume lines less than 1000 in length

  iSector *global_start_sector;
 csVector3 global_start_pos;


void
report_hacker (char *error)
{
  printf (error);
  printf ("\n");


  exit (0);
}

char *
ReadLine (iFile * F1)
{
  char c1[10];
static  char c2[1000];
  int i;
  strcpy (c1, "");
  strcpy (c2, "");


  for (i = 0; i < 1000; i++)
    {
//printf("%d\n",c1[0]);
      F1->Read (c1, 1);
      c2[i] = c1[0];
      if (c1[0] == 10 || F1->AtEOF ())
	{
	  c2[i + 1] = 0;
	  return c2;

	}
    }

  return c2;

}


char *
ReadLine2 (iFile * F1)
{
  static char c2[400];

  strcpy (c2, ReadLine (F1));
  c2[strlen (c2) - 1] = 0;
  return c2;


}



char *
Parse_meshfact (iFile * F1)
{
  char c1[1000];
  const char *c2;
//int i;
//int loc,loc2;

static  char c3[1000];

  while (!F1->AtEOF ())
    {
      strcpy (c1, ReadLine (F1));
      c2 = strstr (c1, "<meshfact name=\"");

//contains a meshfactname
      if (c2 != NULL)
	{
	  strcpy (c3, Parse_meshfact (c1));
	  if (strcmp (c3, "") != 0)
	    {
//printf(c3);
//printf("\n");
	      return c3;
	    }
	}

    }



  return ("");

}

//Assume, all factory files have .factory in plugin
//and all meshes are the same, except they're missing .factory
char *
Parse_plugin_factory (iFile * F1)
{
  int i, j;
  int len;
//int loc,loc2;
  char c1[1000];
  static char c2[1000];

  strcpy (c1, Parse_plugin (F1));

//remove .factory
  j = 0;
  len =(int)(strlen (c1));
  for (i = 0; i < len; i++)
    {
      if (c1[i] == '.')
	if (c1[i + 1] == 'f')
	  if (c1[i + 2] == 'a')
	    if (c1[i + 3] == 'c')
	      if (c1[i + 4] == 't')
		if (c1[i + 5] == 'o')
		  if (c1[i + 6] == 'r')
		    if (c1[i + 7] == 'y')
		      i = i + 8;

      c2[j] = c1[i];
      j++;
    }

  c2[j] = 0;

  return (c2);
}

char *
Parse_plugin (iFile * F1)
{
  char c1[1000];
  const char *c2;
//int i;
//int loc,loc2;

static  char c3[1000];

  while (!F1->AtEOF ())
    {
      strcpy (c1, ReadLine (F1));
      c2 = strstr (c1, "<plugin>");

//contains a meshfactname
      if (c2 != NULL)
	{
	  strcpy (c3, Parse_plugin (c1));
	  if (strcmp (c3, "") != 0)
	    return c3;

	}

    }

  return ("");

}


char *
Parse_plugin (char *c1)
{
  int i;
  int loc=0;
  int loc2;
static  char c3[1000];
  int len;

//-15 before?
  len = (int)(strlen (c1)) - 9;
  for (i = 0; i < len; i++)
    {
      if (c1[i] == '<')
	if (c1[i + 1] == 'p')
	  if (c1[i + 2] == 'l')
	    if (c1[i + 3] == 'u')
	      if (c1[i + 4] == 'g')
		if (c1[i + 5] == 'i')
		  if (c1[i + 6] == 'n')
		    if (c1[i + 7] == '>')
		      {
//printf("huh\n");

			loc = i + 8;
			i = 999999;
		      }


    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	{
//printf("huh2\n");
	  c3[i - loc] = c1[i];

	}

    }

//printf(c3);
//printf("\n");
  return c3;
}





//"=34
char *
Parse_meshfact (char *c1)
{
  int i;
  int loc=0; int loc2;
static  char c3[1000];
//-15 before
  for (i = 0; i < (int)(strlen (c1)) - 16; i++)
    {
      if (c1[i] == '<')
	if (c1[i + 1] == 'm')
	  if (c1[i + 2] == 'e')
	    if (c1[i + 3] == 's')
	      if (c1[i + 4] == 'h')
		if (c1[i + 5] == 'f')
		  if (c1[i + 6] == 'a')
		    if (c1[i + 7] == 'c')
		      if (c1[i + 8] == 't')
			if (c1[i + 9] == ' ')
			  if (c1[i + 10] == 'n')
			    if (c1[i + 11] == 'a')
			      if (c1[i + 12] == 'm')
				if (c1[i + 13] == 'e')
				  if (c1[i + 14] == '=')
				    if (c1[i + 15] == 34)
				      {

					loc = i + 16;
					i = 999999;
				      }


    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == 34)
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }

  return c3;
}


//writes until it gets the </meshfact> line

void
Write_Extra (iFile * F1, iFile * F2)
{
  char c1[1000];
  char *c2;

  while (!F1->AtEOF ())
    {
      strcpy (c1, ReadLine (F1));

      F2->Write (c1, (int)(strlen (c1)));

      c2 = strstr (c1, "</meshfact>");
      if (c2 != NULL)
	return;

    }

}

void
copy_textfile (iFile * read, iFile * write)
{
  char c1[1000];
//  char *c2;

  while (!read->AtEOF ())
    {
      strcpy (c1, ReadLine (read));
      write->Write (c1, (int)(strlen (c1)));
    }

}





int
Parse_frames (iFile * F1)
{
  char c1[1000];
  const char *c2;
  int a;
//int loc,loc2;
//char c3[1000];

  while (!F1->AtEOF ())
    {
      strcpy (c1, ReadLine (F1));
      c2 = strstr (c1, "<frames>");

//contains a meshfactname
      if (c2 != NULL)
	{
	  a = Parse_frames (c1);
	  return a;

	}

    }

  return (-1);

}


int
Parse_frames (char *c1)
{
  int i;
  int loc=0; int loc2;
  char c3[1000];
  int a;

//-15 before
  for (i = 0; i < (int)(strlen (c1)) - 9; i++)
    {
      if (c1[i] == '<')
	if (c1[i + 1] == 'f')
	  if (c1[i + 2] == 'r')
	    if (c1[i + 3] == 'a')
	      if (c1[i + 4] == 'm')
		if (c1[i + 5] == 'e')
		  if (c1[i + 6] == 's')
		    if (c1[i + 7] == '>')
		      {

			loc = i + 8;
			i = 999999;
		      }


    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	{
	  c3[i - loc] = c1[i];

	}

    }

  a = atoi (c3);
  return a;
}



int
Parse_moved (iFile * F1)
{
  char c1[1000];
  const char *c2;
  int a;
//int loc,loc2;
//char c3[1000];

  while (!F1->AtEOF ())
    {
      strcpy (c1, ReadLine (F1));
      c2 = strstr (c1, "<moved>");

//contains a meshfactname
      if (c2 != NULL)
	{
//printf("cool\n");
	  a = Parse_moved (c1);
	  return a;

	}

    }

  return (-1);

}


int
Parse_moved (char *c1)
{
  int i;
  int loc=0; int loc2;
  char c3[1000];
  int a;

//-15 before
  for (i = 0; i < (int)(strlen (c1)) - 7; i++)
    {
      if (c1[i] == '<')
	if (c1[i + 1] == 'm')
	  if (c1[i + 2] == 'o')
	    if (c1[i + 3] == 'v')
	      if (c1[i + 4] == 'e')
		if (c1[i + 5] == 'd')
		  {

		    loc = i + 7;
		    i = 999999;
		  }


    }
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	{

	  c3[i - loc] = c1[i];

	}

    }

  a = atoi (c3);
  return a;
}




int
Parse_time (iFile * F1)
{
  char c1[1000];
  const char *c2;
  int a;
//int loc,loc2;
//char c3[1000];

  while (!F1->AtEOF ())
    {
      strcpy (c1, ReadLine (F1));
      c2 = strstr (c1, "<time>");

//contains a meshfactname
      if (c2 != NULL)
	{
	  a = Parse_time (c1);
	  return a;

	}

    }

  return (-1);

}


int
Parse_time (char *c1)
{
  int i;
  int loc=0; int loc2;
  char c3[1000];
  int a;

//-15 before
  for (i = 0; i < (int)(strlen (c1)) - 6; i++)
    {
      if (c1[i] == '<')
	if (c1[i + 1] == 't')
	  if (c1[i + 2] == 'i')
	    if (c1[i + 3] == 'm')
	      if (c1[i + 4] == 'e')
		{

		  loc = i + 6;
		  i = 999999;
		}


    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	{

	  c3[i - loc] = c1[i];

	}

    }

  a = atoi (c3);
  return a;
}



char *
Parse_meshobj (iFile * F1)
{
  char c1[1000];
  const char *c2;
//int i;
//int loc,loc2;

static  char c3[1000];

  while (!F1->AtEOF ())
    {
      strcpy (c1, ReadLine (F1));
      c2 = strstr (c1, "<meshobj");

//contains a meshfactname
      if (c2 != NULL)
	{

	  strcpy (c3, Parse_meshobj (c1));
	  if (strcmp (c3, "") != 0)
	    return c3;
	}
    }
  return ("");
}

char *
Parse_meshobj (const char *c1)
{
  int i;
  int loc=0; int loc2;
static  char c3[1000];
  int len;


  len = (int)(strlen (c1)) - 7;
  for (i = 0; i < len; i++)
    {
      if (c1[i] == 'n')
	if (c1[i + 1] == 'a')
	  if (c1[i + 2] == 'm')
	    if (c1[i + 3] == 'e')
	      if (c1[i + 4] == '=')
		if (c1[i + 5] == '\"')
		  {
		    loc = i + 6;
		    i = 99999;
		  }


    }
  strcpy (c3, "");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
//printf("e");
      if (c1[i] == '\"')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];



    }

  return c3;
}


















M_V_MOVE
Parse_move (iFile * F1)
{
  char c1[1000];
  const char *c2;
//int i;
//int loc,loc2;

  M_V_MOVE MV;

  MV.is_v = -1;
  MV.is_m = -1;


  M_V_MOVE RET;


//char c3[1000];

  while (!F1->AtEOF ())
    {
      strcpy (c1, ReadLine (F1));
      c2 = strstr (c1, "<v x");

      if (c2 != NULL)
	{
	  RET = Parse_vector (c1);
	  if (RET.is_v != -1)
	    {
	      MV.is_v = 1;
	      MV.v = RET.v;
	    }
	}

      c2 = strstr (c1, "<matrix");

      if (c2 != NULL)
	{
	  RET = Parse_matrix (c1, F1);
	  if (RET.is_m != -1)
	    {
	      MV.is_m = 1;
	      MV.m = RET.m;
	    }

	}

      c2 = strstr (c1, "</move");

      if (c2 != NULL)
	return MV;

    }
//ERROR!
    return MV;
//return NULL;
//error
}











M_V_MOVE
Parse_vector (char *c1)
{
  int i;
  int loc=0; int loc2;
  char c3[1000];

  M_V_MOVE MV;
  MV.is_v = 1;
  MV.is_m = -1;

//-15 before
  for (i = 0; i < (int)(strlen (c1)) - 3; i++)
    {
      if (c1[i] == 'x')
	if (c1[i + 1] == '=')
	  if (c1[i + 2] == '\"')
	    {

	      loc = i + 3;
	      i = 999999;
	    }

//atof
    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '\"')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }



  MV.v.x = atof (c3);


  for (i = 0; i < (int)(strlen(c1)) - 15; i++)
    {
      if (c1[i] == 'y')
	if (c1[i + 1] == '=')
	  if (c1[i + 2] == '\"')
	    {

	      loc = i + 3;
	      i = 999999;
	    }

//atof
    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '\"')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }


  MV.v.y = atof (c3);


  for (i = 0; i < (int)(strlen(c1)) - 15; i++)
    {
      if (c1[i] == 'z')
	if (c1[i + 1] == '=')
	  if (c1[i + 2] == '\"')
	    {

	      loc = i + 3;
	      i = 999999;
	    }

//atof
    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '\"')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }


  MV.v.z = atof (c3);

  return MV;
}








M_V_MOVE
Parse_matrix (char *c1, iFile * F1)
{
  int i;
  int loc=0; int loc2;
  char c3[1000];

  M_V_MOVE MV;
  MV.is_v = -1;
  MV.is_m = 1;


  strcpy (c1, ReadLine (F1));

//-15 before
  for (i = 0; i < (int)(strlen(c1)) - 4; i++)
    {
      if (c1[i] == 'm')
	if (c1[i + 1] == '1')
	  if (c1[i + 2] == '1')
	    if (c1[i + 3] == '>')
	      {
		loc = i + 4;
		i = 999999;
	      }

//atof
    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }

  MV.m.m11 = atof (c3);





  strcpy (c1, ReadLine (F1));

  for (i = 0; i < (int)(strlen(c1)) - 5; i++)
    {
      if (c1[i] == 'm')
	if (c1[i + 1] == '1')
	  if (c1[i + 2] == '2')
	    if (c1[i + 3] == '>')
	      {
		loc = i + 4;
		i = 999999;
	      }

//atof
    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }

  MV.m.m12 = atof (c3);

  strcpy (c1, ReadLine (F1));

  for (i = 0; i < (int)(strlen(c1)) - 5; i++)
    {
      if (c1[i] == 'm')
	if (c1[i + 1] == '1')
	  if (c1[i + 2] == '3')
	    if (c1[i + 3] == '>')
	      {
		loc = i + 4;
		i = 999999;
	      }

//atof
    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }

  MV.m.m13 = atof (c3);

  strcpy (c1, ReadLine (F1));

  for (i = 0; i < (int)(strlen(c1)) - 5; i++)
    {
      if (c1[i] == 'm')
	if (c1[i + 1] == '2')
	  if (c1[i + 2] == '1')
	    if (c1[i + 3] == '>')
	      {
		loc = i + 4;
		i = 999999;
	      }

//atof
    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }

  MV.m.m21 = atof (c3);

  strcpy (c1, ReadLine (F1));

  for (i = 0; i < (int)(strlen(c1)) - 5; i++)
    {
      if (c1[i] == 'm')
	if (c1[i + 1] == '2')
	  if (c1[i + 2] == '2')
	    if (c1[i + 3] == '>')
	      {
		loc = i + 4;
		i = 999999;
	      }

//atof
    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }

  MV.m.m22 = atof (c3);




  strcpy (c1, ReadLine (F1));

  for (i = 0; i < (int)(strlen(c1)) - 5; i++)
    {
      if (c1[i] == 'm')
	if (c1[i + 1] == '2')
	  if (c1[i + 2] == '3')
	    if (c1[i + 3] == '>')
	      {
		loc = i + 4;
		i = 999999;
	      }

//atof
    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }

  MV.m.m23 = atof (c3);


  strcpy (c1, ReadLine (F1));

  for (i = 0; i < (int)(strlen(c1)) - 5; i++)
    {
      if (c1[i] == 'm')
	if (c1[i + 1] == '3')
	  if (c1[i + 2] == '1')
	    if (c1[i + 3] == '>')
	      {
		loc = i + 4;
		i = 999999;
	      }

//atof
    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }

  MV.m.m31 = atof (c3);



  strcpy (c1, ReadLine (F1));

  for (i = 0; i < (int)(strlen(c1)) - 5; i++)
    {
      if (c1[i] == 'm')
	if (c1[i + 1] == '3')
	  if (c1[i + 2] == '2')
	    if (c1[i + 3] == '>')
	      {
		loc = i + 4;
		i = 999999;
	      }

//atof
    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }

  MV.m.m32 = atof (c3);

  strcpy (c1, ReadLine (F1));

  for (i = 0; i < (int)(strlen(c1)) - 5; i++)
    {
      if (c1[i] == 'm')
	if (c1[i + 1] == '3')
	  if (c1[i + 2] == '3')
	    if (c1[i + 3] == '>')
	      {
		loc = i + 4;
		i = 999999;
	      }

//atof
    }
//("huh3\n");
  for (i = loc; i < (int)(strlen (c1)); i++)
    {
      if (c1[i] == '<')
	{
	  loc2 = i - 1;
	  c3[i - loc] = 0;
	  i = 99999999;
	}
      else
	c3[i - loc] = c1[i];

    }

  MV.m.m33 = atof (c3);


  return MV;
}




//Initializes entities for a map!









