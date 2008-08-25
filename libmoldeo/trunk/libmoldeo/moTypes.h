/*******************************************************************************

                                moTypes.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                    *
  *                                                                          *
  *   This code is distributed in the hope that it will be useful, but       *
  *   WITHOUT ANY WARRANTY; without even the implied warranty of             *
  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
  *   General Public License for more details.                               *
  *                                                                          *
  *   A copy of the GNU General Public License is available on the World     *
  *   Wide Web at <http://www.gnu.org/copyleft/gpl.html>. You can also       *
  *   obtain it by writing to the Free Software Foundation,                  *
  *   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.         *
  *                                                                          *
  ****************************************************************************

  Copyright(C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa
  Andres Colubri

*******************************************************************************/
//types definidos
#ifndef __MOTYPES_H
#define __MOTYPES_H

//para evitar warnings sobre funciones obsoletas
#pragma once
#define _CRT_SECURE_NO_DEPRECATE	1
#define _CRT_NONSTDC_NO_DEPRECATE	1

/*OS SELECT*/

#if defined(__APPLE__)
#undef MO_MACOSX
#define MO_MACOSX	1
#elif defined(macintosh)
#undef MO_MACOS
#define MO_MACOS	1
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
#undef MO_LINUX
#define MO_LINUX	1
#endif

#if defined(WIN32) || defined(_WIN32)
#undef MO_WIN32
#define MO_WIN32	1
#endif

/*STANDARD LIBRARIES: define here or die*/
#ifdef MO_WIN32
  #ifndef _MSC_VER
    //atencion, esto se agregó para evitar el conflicto con freeimage en mingw
    #define NOGDI
    #define MO_GSTREAMER
    #define MO_HANDLE HWND
  #endif
#endif

#ifdef MO_WIN32
    #include "muParserDLL.h"
#endif

#ifdef MO_LINUX
    #include <muParser/muParser.h>
#endif


#include <math.h>
#include <limits.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>
#include <string>
#include <vector>
#include <fstream>


using namespace std;


/*MOLDEO SETUP DEFINITIONS*/

#define USE_GL_EXTENSIONS

//#define USE_QUICKTIME
//#define USE_SDLIMAGE
#define USE_FREEIMAGE


/*OS DEFINITIONS*/

#ifdef MO_WIN32
    #ifdef LIBMOLDEO_EXPORTS
    #define LIBMOLDEO_API __declspec(dllexport)
    #else
    #define LIBMOLDEO_API __declspec(dllimport)
    #endif
#else
	#define LIBMOLDEO_API
#endif


/*OSX*/
#ifdef MO_MACOSX

  #include "SDL.h"
  #include "SDL_thread.h"
  #include "SDL_mutex.h"
  #include "SDL_net.h"
  #include "SDL_image.h"
  #include "QuickTime.h"
  #include "GLUT/glut.h"

  #ifdef USE_GL_EXTENSIONS
    // GLEW provides gl.h, glu.h and glext.h
    #include <GL/glew.h>
  #else
    #include "OpenGL/gl.h"
    #include "OpenGL/glu.h"
    #include "OpenGL/glext.h"
  #endif

#endif

/*LINUX*/
#ifdef MO_LINUX

    #define MO_HANDLE void*

	#include <SDL/SDL.h>
	#include <SDL/SDL_thread.h>
	#include <SDL/SDL_mutex.h>
	#include <SDL/SDL_net.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_syswm.h>

	#ifdef USE_GL_EXTENSIONS
		// GLEW provides gl.h, glu.h and glext.h
		#include <GL/glew.h>
	#else
		#include <GL/gl.h>
		#include <GL/glu.h>
		#include <GL/glext.h>
	#endif

	#include <GL/glut.h>

	#define MO_GSTREAMER 1

    # ifndef __int64
    # define __int64 long long
    # endif

    # ifndef _int64
    # define _int64 __int64
    # endif

    #define moint64 __int64


#endif

/*WINDOWS*/
#ifdef MO_WIN32

  /*VISUAL C++ COMPILER*/
  #ifdef _MSC_VER

    #define MO_HANDLE HWND

    #define MO_USING_VC
    // Microsoft Visual C++ specific pragmas.  MSVC6 is version 12.00, MSVC7.0 is
    // version 13.00, and MSVC7.1 is version 13.10.  MSVC8.0 is version 14.00.
    #if _MSC_VER < 1300
    #define MO_USING_VC6
    #elif _MSC_VER < 1310
    #define MO_USING_VC70
    #elif _MSC_VER < 1400
    #define MO_USING_VC71
    #else
    #define MO_USING_VC80
    #endif

    // Disable the warning "non dll-interface class FOO used as base for
    // dll-interface class BAR.".
    //#pragma warning( disable : 4275 )

    // Disable the warning about truncating the debug names to 255 characters.
    // This warning shows up often with STL code in MSVC6, but not MSVC7.
    #pragma warning( disable : 4786 )

    // This warning is disabled because MSVC6 warns about not finding
    // implementations for the pure virtual functions that occur in the template
    // classes 'template <class Real>' when explicitly instantiating the classes.
    // NOTE:  If you create your own template classes that will be explicitly
    // instantiated, you should re-enable the warning to make sure that in fact
    // all your member data and functions have been defined and implemented.
    #pragma warning( disable : 4661 )

    #define MO_DIRECTSHOW
  #endif

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

	#include <SDL.h>
	#include <SDL_thread.h>
	#include <SDL_mutex.h>
	#include <SDL_net.h>
	#include <SDL_image.h>
	#include <SDL_syswm.h>

	#ifdef USE_QUICKTIME
	//#define TARGET_API_MAC_CARBON 0
	//#define TARGET_OS_WIN32 1
	//#define TARGET_CPU_X86 1
	//#define FUNCTION_DECLSPEC 1
	//#define FUNCTION_WIN32CC 1
	//#define PHYSICAL_ADDRESS LARGE_INTEGER
	#endif

	#ifdef USE_GL_EXTENSIONS
		// GLEW provides gl.h, glu.h and glext.h
		#include <GL/glew.h>
	#else
		#include "GL/gl.h"
		#include "GL/glu.h"
		#include "GL/glext.h"
	#endif

	#include "GL/glut.h"

    //basetyps.h
    # ifndef _int64
    #  define _int64 __int64
    # endif

    #define moint64 __int64
#endif

/*CONSTANTS DEFINITIONS*/


#define MO_PI           3.1415926535897932384626433832795f
#define MO_ON			1
#define MO_OFF			-1
#define MO_ACTIVATED    1
#define MO_DEACTIVATED -1
#define MO_FALSE        0
#define MO_TRUE         1
#define MO_PRESSED      1
#define MO_PULSED		1
#define MO_RELEASED     0

#define MO_SUCCESS		0
#define MO_NOERROR		0
#define MO_ERROR		65535
#define MO_FAIL			65535
#define MO_UNDEFINED    -1

#define MO_MAX_DEBUG	65535
#define MO_MAX_TEXTURAS	512
#define MO_MAX_MODELOS	256

#define MOboolean       GLuint
#define MOswitch        signed char
#define MOuint          GLuint
#define MOint           GLint
#define MOlong          long
#define MOulong         unsigned long
#define MOlonglong      moint64
#define MOulonglong     unsigned moint64
#define MOchar          char
#define MOuchar         unsigned char
#define MOubyte			unsigned char
#define MObyte			char
#define MOshort         short
#define MOushort        unsigned short
#define MOfloat         GLfloat
#define MOdouble        GLdouble
#define MOtexture       GLuint
#define MOefectoid		GLuint

#define MOpointer		MOubyte*

#define MOaccion int
#define MOdevcode int

#define MO_DEF_SCREEN_WIDTH  800
#define MO_DEF_SCREEN_HEIGHT 600
#define MO_DEF_RENDER_WIDTH  800
#define MO_DEF_RENDER_HEIGHT 600
#define MO_DEF_SCREEN_DEPTH 32

#define MO_RENDER_TEX 0
#define MO_SCREEN_TEX 1
#define MO_EFFECTS_TEX 2
#define MO_FINAL_TEX 3

#define MO_MAX_PRESETS 9
#define MO_DEBUG

#define OLD_CONFIG	0


//////////////////////////////////////////////////////
// Parametros mas comunes usados en varios Effects ///
//////////////////////////////////////////////////////

#ifndef MO_RED
#define MO_RED      0
#define MO_GREEN    1
#define MO_BLUE     2
#define MO_ALPHA    3
#endif

enum moBlendingModes {
   MO_BLENDING_TRANSPARENCY = 0,
   MO_BLENDING_ADDITIVEALPHA = 1,
   MO_BLENDING_MIXING = 2,
   MO_BLENDING_MULTIPLY = 3,
   MO_BLENDING_EXCLUSION = 4,
   MO_BLENDING_ADDITIVE = 5,
   MO_BLENDING_OVERLAY = 6,
   MO_BLENDING_SUBSTRACTIVE = 7,
   MO_BLENDING_SATURATE = 8,
   MO_BLENDINGS = 9
};

enum moPolygonModes {
   MO_POLYGONMODE_FILL = 0,
   MO_POLYGONMODE_LINE = 1,
   MO_POLYGONMODE_POINT = 2,
   MO_POLYGONMODES = 3
};

//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// Linux compatibility                             ///
//////////////////////////////////////////////////////

#ifdef MO_LINUX
//#    include "port/getc.h"
//#    include "port/itoa.h"
//#    include "port/ptypes.h"
#    include "port/stricmp.h"
#endif

//////////////////////////////////////////////////////

class LIBMOLDEO_API ajCoord {
public:
   MOfloat X, Y, Z;     //coord
   MOfloat XN, YN, ZN;  //coord news
   MOfloat INCX, INCY, INCZ;  //coord news
   MOfloat NX,NY,NZ;    //normal
   MOfloat U,V;         //text
   MOint I,J;           //raster
   MOtexture Texture;   //texture
   MOfloat Scalef;
};

class LIBMOLDEO_API ajQuad {
public:
	ajCoord A;
	ajCoord B;
	ajCoord C;
	ajCoord D;
};

LIBMOLDEO_API MOfloat morand();
LIBMOLDEO_API MOint moRand(MOint);
LIBMOLDEO_API void HSVtoRGB(short,short,short,float*,float*,float*);

enum moMoldeoObjectType {
	MO_OBJECT_UNDEFINED = -1,
	MO_OBJECT_EFFECT = 0,
	MO_OBJECT_PREEFFECT = 1,
	MO_OBJECT_POSTEFFECT = 2,
	MO_OBJECT_MASTEREFFECT = 3,
	MO_OBJECT_IODEVICE = 4,
	MO_OBJECT_RESOURCE = 5,
	MO_OBJECT_TYPES = 6
};

/**
 * moTexParam
 *
 */

struct LIBMOLDEO_API moTexParam {
    public:
        GLenum target;
        GLint internal_format;
		GLint min_filter;
		GLint mag_filter;
		GLint wrap_s;
		GLint wrap_t;
};


/**
 * MODefTex2DParams
 *
 */

const moTexParam MODefTex2DParams =
{
    GL_TEXTURE_2D,
    GL_RGBA,
	GL_LINEAR,
	GL_LINEAR,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_EDGE
};


/**
 * MOUndefinedTex
 *
 */

const moTexParam MOUndefinedTex =
{
    MO_UNDEFINED,
    MO_UNDEFINED,
	MO_UNDEFINED,
	MO_UNDEFINED,
	MO_UNDEFINED,
	MO_UNDEFINED
};


LIBMOLDEO_API MOulong moGetTicks();

#ifndef momax
#define momax(a,b) (((a) > (b)) ? (a) : (b))
#define momin(a,b) (((a) < (b)) ? (a) : (b))
#endif

#include "moTempo.h"

#ifdef USE_SDLIMAGE
//#include "SDL_image.h"
#endif

#ifdef USE_FREEIMAGE
//#include "FreeImage.h"
#endif

#endif
