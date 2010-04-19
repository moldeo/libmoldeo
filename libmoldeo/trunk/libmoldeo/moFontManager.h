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
  Andrés Colubri

*******************************************************************************/

#ifndef __MOFONTMANAGER_H
#define __MOFONTMANAGER_H

#include "moTypes.h"
#include "moAbstract.h"
#include "moArrayH.h"
#include "moResourceManager.h"
#include "moText.h"
#include "moParam.h"
#include "moOGLFT.h"

/**
* Tipo de representación de tipografía
*/

enum moFontType {
	MO_FONT_OUTLINE,
	MO_FONT_TRANSLUCENT,
	MO_FONT_TRANSLUCENTTEXTURE,
	MO_FONT_GRAYSCALE,
	MO_FONT_MONOCHROME,
	MO_FONT_SOLID,
	MO_FONT_FILLED
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
		virtual MOboolean Init( moFontType p_Type, moText p_fontname, MOint p_size );
		virtual MOboolean Finish();

		moText  GetName();

		void SetSize( MOfloat	size );
		void Draw( MOfloat x, MOfloat y, moText text );

		void SetForegroundColor( MOfloat p_r, MOfloat p_g, MOfloat p_b);
		void SetHorizontalJustification( OGLFT::Face::HorizontalJustification p_horizontal_justification);
		void SetStringRotation( MOfloat p_string_rotation );

	protected:

		moText	m_Name;

		//Abstract instantiation of faces:
		OGLFT::Face* m_pFace;


};

/*
template class LIBMOLDEO_API moDynamicArray<moFont*>;
typedef moDynamicArray<moFont*> moFonts;
*/
moDeclareExportedDynamicArray(moFont*, moFonts)

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

		moFont*     GetFont( moText p_fontname, bool create = false, moFontType p_fonttype = MO_FONT_OUTLINE, MOfloat p_fontsize = 12 );
		moFont*     GetFont( MOint p_id );
        moFont*     GetFont( moValue& p_value, bool create = true );

		moFonts*	GetFonts();
		moFont*     AddFont( moText p_fontname, moFontType p_fonttype = MO_FONT_OUTLINE, MOfloat p_fontsize = 12 );
		void        AddFont( moFont*    p_pFont );

	private:

        moFont* LoadFont( moText p_fontname, moFontType p_fonttype = MO_FONT_OUTLINE, MOfloat p_fontsize = 12 );

		moFonts		m_Fonts;
};


#endif
