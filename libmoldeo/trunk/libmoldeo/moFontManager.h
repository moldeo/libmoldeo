/*******************************************************************************

                                moFontManager.h

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

#ifndef __MOFONTMANAGER_H
#define __MOFONTMANAGER_H

#include "moTypes.h"
#include "moResourceManager.h"


typedef void moFontFace;

/**
* Tipo de representación de tipografía
*/
typedef MOfloat moFontSize;

enum moFontType {
	MO_FONT_OUTLINE = 0,
	MO_FONT_TRANSLUCENT = 1,
	MO_FONT_TRANSLUCENTTEXTURE = 2,
	MO_FONT_GRAYSCALE = 3,
	MO_FONT_MONOCHROME = 4,
	MO_FONT_SOLID = 5,
	MO_FONT_FILLED = 6,
	MO_FONT_GLBUILD = 7,
	MO_FONT_TYPES = 8,
	MO_FONT_UNDEFINED=-1
};


/// clase base para una fuente 3d o 2d
/**
*   Implementada a través de la librería freetype y OGLFT, permite crear y manipular tipografías vectoriales.
*/
class LIBMOLDEO_API moFont : public moAbstract {

	public:
		moFont();
		virtual ~moFont();

		virtual MOboolean Init();
		virtual MOboolean Init( moFontType p_Type, moText p_fontname /*or font bitmap name*/, MOint p_size, MOuint glid = 0 );
		virtual MOboolean Finish();

		moText  GetName();

		void SetSize( MOfloat	size );
		void Draw( MOfloat x, MOfloat y, moText& text );
		void Draw( MOfloat x, MOfloat y, moText& text, moFontSize p_fontsize, MOint set=0, MOfloat sx=1.0, MOfloat sy=1.0, MOfloat rt=0.0 );

		void SetForegroundColor( MOfloat p_r, MOfloat p_g, MOfloat p_b);
		void SetHorizontalJustification( int p_horizontal_justification);
		void SetStringRotation( MOfloat p_string_rotation );

	protected:

		moText	m_Name;
		moFontSize m_FontSize;

		//Abstract instantiation of faces:
		moFontFace* m_pFace;

		//glPrint implementation
		void BuildFont();
		void glPrint( GLint x, GLint y, char *string, int set, float scx, float scy, float rt );
		void KillFont();
        GLuint m_GLBaseList; // Base Display List For The Font
        GLuint m_FontGLId;

};

moDeclareExportedDynamicArray(moFont*, moFonts);

/// administrator de fuentes
/**
*
*/
class LIBMOLDEO_API moFontManager : public moResource {

	public:
		moFontManager();
		virtual ~moFontManager();
		virtual MOboolean Init();
		virtual MOboolean Finish();

        moFontType  GetFontType( moText fonttype );

		moFont*     GetFont( moText p_fontname, bool create = false, moFontType p_fonttype = MO_FONT_OUTLINE, MOfloat p_fontsize = 12 );
		moFont*     GetFont( MOint p_id );
        moFont*     GetFont( moValue& p_value, bool create = true );

		moFonts*	GetFonts();
		moFont*     AddFont( moText p_fontname, moText p_fonttype="OUTLINE", MOfloat p_fontsize = 12 );
		moFont*     AddFont( moText p_fontname, moFontType p_fonttype = MO_FONT_OUTLINE, MOfloat p_fontsize = 12 );
		void        AddFont( moFont*    p_pFont );

	private:

        moFont* LoadFont( moText p_fontname_path, moFontType p_fonttype = MO_FONT_OUTLINE, MOfloat p_fontsize = 12 );

		moFonts		m_Fonts;
};


#endif
