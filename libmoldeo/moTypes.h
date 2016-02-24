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
    #define NOGDI //comentar esta línea con MinGW64??
    //#undef _WINDOWS_
    #define MO_GSTREAMER
    #define MO_HANDLE HWND
    #define MO_DISPLAY void*

  #endif
#endif

#undef SHADER_CG
#define NDEBUG 1


#include <math.h>
#include <limits.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>
#include <string.h>
#include <vector>
#include <fstream>


using namespace std;


/*MOLDEO SETUP DEFINITIONS*/
#ifndef OPENGLESV2
#define USE_GL_EXTENSIONS
#else
#define  glFramebufferTexture2DEXT glFramebufferTexture2D
#define  glCheckFramebufferStatusEXT glCheckFramebufferStatus
#define  glGenFramebuffersEXT glGenFramebuffers
#define  glDeleteFramebuffersEXT glDeleteFramebuffers
typedef char GLcharARB;
typedef unsigned int GLhandleARB;
#define glColor4f moColor4f
#define glNormal3f moNormal3f
#define glColor3f moColor3f
#define GL_FRAMEBUFFER_EXT GL_FRAMEBUFFER
#define GL_FRAMEBUFFER_COMPLETE_EXT GL_FRAMEBUFFER_COMPLETE
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT GL_FRAMEBUFFER_UNSUPPORTED
#define GL_COLOR_ATTACHMENT0_EXT GL_COLOR_ATTACHMENT0
#define glBindFramebufferEXT glBindFramebuffer
#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#define GL_RGBA16F_ARB 0x84F5
#define GL_LUMINANCE16F_ARB 0x881E
#define GL_RGBA32F_ARB 0x8814
#define GL_LUMINANCE32F_ARB 0x8818
#define GLEW_EXT_framebuffer_object 1
#define GLEW_ARB_multitexture 1
#define GLEW_ARB_shader_objects 1
#define GLEW_ARB_vertex_shader 1
#define GLEW_ARB_fragment_shader 1
#define GLEW_ARB_shading_language_100 1
#define GLEW_ARB_texture_non_power_of_two 1
#define GL_TEXTURE0_ARB GL_TEXTURE0

#define GL_VERTEX_SHADER_ARB GL_VERTEX_SHADER
#define GL_FRAGMENT_SHADER_ARB GL_FRAGMENT_SHADER
#define GL_OBJECT_LINK_STATUS_ARB GL_LINK_STATUS
#define glCreateProgramObjectARB glCreateProgram
#define glDeleteObjectARB glDeleteShader
#define glCreateShaderObjectARB glCreateShader
#define glAttachObjectARB glAttachShader
#define glDetachObjectARB glDetachShader
#define glUseProgramObjectARB glUseProgram
#define glGetUniformLocationARB glGetUniformLocation
#define glGetAttribLocationARB glGetAttribLocation
#define glShaderSourceARB glShaderSource
#define glLinkProgramARB glLinkProgram
#define glGetObjectParameterivARB glGetShaderiv
#define GL_OBJECT_INFO_LOG_LENGTH_ARB GL_INFO_LOG_LENGTH
#define glGetInfoLogARB glGetShaderInfoLog
#define glCompileShaderARB glCompileShader
#define glUniform1fARB glUniform1f
#define glUniform2fARB glUniform2f
#define glUniform3fARB glUniform3f
#define glUniform4fARB glUniform4f
#define glUniform1iARB glUniform1i
#define glUniform2iARB glUniform2i
#define glUniform3iARB glUniform3i
#define glUniform4iARB glUniform4i
#define glUniformMatrix2fvARB glUniformMatrix2fv
#define glUniformMatrix3fvARB glUniformMatrix3fv
#define glUniformMatrix4fvARB glUniformMatrix4fv
#define glUniform1fvARB glUniform1fv
#define glUniform2fvARB glUniform2fv
#define glUniform3fvARB glUniform3fv
#define glUniform4fvARB glUniform4fv
#define glUniform1ivARB glUniform1iv
#define glUniform2ivARB glUniform2iv
#define glUniform3ivARB glUniform3iv
#define glUniform4ivARB glUniform4iv
#define glActiveTextureARB glActiveTexture
#endif

//#define USE_QUICKTIME
//#define USE_SDLIMAGE
#define USE_FREEIMAGE
#define USE_MOTEXT0

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

  //#include <X11/X.h>

  #define MO_HANDLE void*

  //revisar esto!!! para Mac OSX
  #define MO_DISPLAY void*
  #include "SDL.h"
  #include "SDL_thread.h"
  #include "SDL_mutex.h"
/*
  #include "SDL_net.h"
  #include "SDL_image.h"
  //#include "QuickTime.h"
*/
  #ifdef USE_GL_EXTENSIONS
    // GLEW provides gl.h, glu.h and glext.h
    #include <GL/glew.h>
  #else
    #include "OpenGL/gl.h"
    #include "OpenGL/glu.h"
    #include "OpenGL/glext.h"
  #endif

    #include "GLUT/glut.h"

	#define MO_GSTREAMER 1

    # ifndef __int64
    # define __int64 long long
    # endif

    # ifndef _int64
    # define _int64 __int64
    # endif

    #define moint64 __int64
    typedef __int16_t moWord;
	typedef __int32_t moDWord;

    #undef assert
    #define assert(ignore)((void) 0)


#endif

/*LINUX*/
#ifdef MO_LINUX

    #include "X11/X.h"

    #define MO_HANDLE Window
    #define MO_DISPLAY void*

	#ifdef USE_GL_EXTENSIONS
		// GLEW provides gl.h, glu.h and glext.h
		#include <GL/glew.h>
	#else
#ifdef OPENGLESV2
	//#include <GLES/gl.h>
	#include <GLES2/gl2.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glext.h>
	#include <GL/glut.h>
#endif
	#endif

	

	#define MO_GSTREAMER 1

    # ifndef __int64
    # define __int64 long long
    # endif

    # ifndef _int64
    # define _int64 __int64
    # endif

    #define moint64 __int64
    typedef __int16_t moWord;
    typedef __int32_t moDWord;

    #undef MO_RASPBIAN
    #define MO_RASPBIAN 1
#endif

/*WINDOWS*/
#ifdef MO_WIN32

  /*VISUAL C++ COMPILER*/
  #ifdef _MSC_VER

    #define MO_HANDLE HWND
    #define MO_DISPLAY void*

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

	//#define MO_DIRECTSHOW
	#define MO_GSTREAMER
	#undef MO_USING_ALFW
	//precompiled headers:
	//#include "StdAfx.h"
  #endif

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
/*
	#include <SDL.h>
	#include <SDL_thread.h>
	#include <SDL_mutex.h>
	#include <SDL_syswm.h>
*/
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

	//#define GLUT_DISABLE_ATEXIT_HACK
	//#include "GL/glut.h"

    //basetyps.h
    # ifndef _int64
    #  define _int64 __int64
    # endif

    #define moint64 __int64
    typedef __int16 moWord;
    typedef __int32 moDWord;

    //#define assert(X) {}
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

#define MO_MAX_DEBUG	4096
#define MO_MAX_TEXTURAS	512
#define MO_MAX_MODELOS	256

#define MOboolean       unsigned int
#define MOswitch        signed char
#define MOuint          unsigned int
#define MOint           int
#define MOint64         moint64
#define MOuint64        unsigned moint64
#define MOlong          long
#define MOulong         unsigned long
#define MOlonglong      moint64
#define MOulonglong     unsigned moint64
#define MOword          moWord
#define MOdword         moDWord
#define MOchar          char
#define MOuchar         unsigned char
#define MOubyte		unsigned char
#define MObyte		char
#define MOshort         short
#define MOushort        unsigned short
#define MOfloat         float
#define MOdouble        double
#define MOtexture       unsigned int
#define MOefectoid	unsigned int


#define MOpointer		void*

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

//3D STEREOSCOPIC RENDER
#define MO_LEFT_TEX 4
#define MO_RIGHT_TEX 5


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

/// Modos de combinación
/**
*   Modos de combinación predeterminados
*   Estos valores son referencias para los modos de combinación de colores que suelen usarse y están ya implementados
*   bajo OpenGL a través de la función glBlend()
*/
enum moBlendingModes {
   MO_BLENDING_TRANSPARENCY = 0, /// transparencia
   MO_BLENDING_ADDITIVEALPHA = 1, /// aditivo según transparencia
   MO_BLENDING_MIXING = 2, /// mezcla
   MO_BLENDING_MULTIPLY = 3, /// multipliación
   MO_BLENDING_EXCLUSION = 4, /// exclusión
   MO_BLENDING_ADDITIVE = 5, /// aditivo por color
   MO_BLENDING_OVERLAY = 6, /// sobrecarga
   MO_BLENDING_SUBSTRACTIVE = 7, /// sustracción
   MO_BLENDING_SATURATE = 8, /// saturación
   MO_BLENDINGS = 9 /// cantidad de modos
};

/// Modos de dibujado de polígonos
/**
*   Modos de dibujado de polígonos
*   Estos modos son aquellos predeterminados bajo OpenGL
*
*/
enum moPolygonModes {
   MO_POLYGONMODE_FILL = 0, /// relleno
   MO_POLYGONMODE_LINE = 1, /// sólo líneas
   MO_POLYGONMODE_POINT = 2, /// sólo puntos
   MO_POLYGONMODES = 3 /// cantidad de modos
};

/// Modo estereoscópico
/**
*   Modo estereoscópico
*   Para la estereoscopía se fijan estos modos
*/
enum moStereoSides {
  MO_STEREO_NONE = 0,
  MO_STEREO_LEFT = 1,
  MO_STEREO_RIGHT = 2
};

//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// Linux compatibility                             ///
//////////////////////////////////////////////////////

#ifdef MO_LINUX
//#    include "port/getc.h"
//#    include "port/itoa.h"
//#    include "port/ptypes.h"
#    include "moPort.h"
#endif

#ifdef MO_MACOSX
//#    include "port/getc.h"
//#    include "port/itoa.h"
//#    include "port/ptypes.h"
////#    include "port/stricmp.h"
#define stricmp strcasecmp
#endif

//////////////////////////////////////////////////////

LIBMOLDEO_API MOfloat morand();
LIBMOLDEO_API MOint moRand(MOint);
LIBMOLDEO_API void HSVtoRGB(short,short,short,float*,float*,float*);
LIBMOLDEO_API void moColor4f(float r, float g, float b, float a);
LIBMOLDEO_API void moNormal3f(float x, float y, float z);
LIBMOLDEO_API void moColor3f(float r, float g, float b);

/// Tipos de objetos en Moldeo
/**
 * Definición y enumeración de tipos de objetos en Moldeo
 *
 */
enum moMoldeoObjectType {
	MO_OBJECT_UNDEFINED = -1, /// Objeto indefinido
	MO_OBJECT_EFFECT = 0, /// Objeto dibujable, efecto ( efectos en el orden de dibujado )
	MO_OBJECT_PREEFFECT = 1,/// Objeto dibujable, pre-efecto ( primeros efectos en el orden de dibujado )
	MO_OBJECT_POSTEFFECT = 2,/// Objeto dibujable, post-efecto ( últímos efectos en el orden de dibujado )
	MO_OBJECT_MASTEREFFECT = 3,/// Objeto dibujable, efecto-maestro ( puede controlar otros efectos )
	MO_OBJECT_IODEVICE = 4,/// Dispositivo de entrada/salida, típicamente, interfaces humanas de IO y datos ( teclado, mouse, tableta, tcp, udp, serial )
	MO_OBJECT_RESOURCE = 5,/// Recursos de datos, objetos, imágenes, videos y funcionalidades múltiples
	MO_OBJECT_CONSOLE = 6,/// Objeto principal de administración y dibujado de objetos de Moldeo
	MO_OBJECT_TYPES = 7 /// referencia para la cantidad de tipos de objetos
};

/// Parámetros internos de una textura
/**
 * moTexParam
 *  estructura que representa la parametrización de una textura en OPENGL
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


/// Parámetros internos predeterminados de una textura
/**
* MODefTex2DParams
*  Parámetros internos predeterminados de una textura
*
*
*/

const moTexParam MODefTex2DParams =
{
    GL_TEXTURE_2D, /// textura 2d
    GL_RGBA, /// 32 bits, rojo, verde, azul, opacidad
    GL_LINEAR, /// (GL_NEAREST_MIPMAP_LINEAR) interpolación de filtro lineal para el achicamiento
    GL_LINEAR, /// (GL_NEAREST_MIPMAP_LINEAR) interpolación de filtro lineal para el agrandamiento
    GL_REPEAT, /// modo de repetición en el orden horizontal
    GL_REPEAT /// modo de repetición en el orden vertical
};


/// Parámetros internos predeterminados de una textura no identificados
/**
* MOUndefinedTex
*  Parámetros internos predeterminados de una textura no identificados
*
*
*/
const moTexParam MOUndefinedTex =
{
    0,
    MO_UNDEFINED,
	MO_UNDEFINED,
	MO_UNDEFINED,
	MO_UNDEFINED,
	MO_UNDEFINED
};


///Devuelve la versión de libmoldeo
/**
*   Función global que devuelve en formato texto la versión utilizada de libmoldeo, el ´núcleo de Moldeo.
*/
#include "moText.h"
LIBMOLDEO_API moText moGetVersionStr();

///macros para máximos y mínimos
/**
*   macros para máximos y mínimos
*/
#ifndef momax
#define momax(a,b) (((a) > (b)) ? (a) : (b))
#define momin(a,b) (((a) < (b)) ? (a) : (b))
#endif

#endif

