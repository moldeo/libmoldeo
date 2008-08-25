/*******************************************************************************

                              moFontManager.cpp

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

#include "moFontManager.h"

#include "moArray.cpp"
moDefineDynamicArray(moFonts)

//===========================================
//
//				moFontManager
//
//===========================================

moFontManager::moFontManager() {
	SetType(MO_OBJECT_RESOURCE);
	SetResourceType(MO_RESOURCETYPE_FONT);

	SetName("Font Manager");
}

moFontManager::~moFontManager() {

}

MOboolean
moFontManager::Init() {

    m_Fonts.Init(0, NULL);



	moFont *pFont = NULL;

	pFont = new moFont();

	if (pFont) {

		moText	completepath;

		completepath = m_pResourceManager->GetDataMan()->GetDataPath() + moText("/");
		completepath+= moText("fonts/arial.ttf");

		pFont->Init( MO_FONT_TRANSLUCENT, completepath, 16 );

		//pFont->Init( MO_FONT_SOLID, completepath, 16 );

	}

	AddFont(pFont);

	return true;

}

MOboolean
moFontManager::Finish() {
	m_Fonts.Empty();
	return true;
}

moFonts*
moFontManager::GetFonts() {
	return &m_Fonts;
}

void
moFontManager::AddFont( moFont*	p_pFont ) {
	m_Fonts.Add( p_pFont );
}

moFont*
moFontManager::AddFont( moText p_fontname, moFontType p_fonttype, MOfloat p_fontsize ) {

    moFont* pFont = NULL;

    pFont = GetFont( p_fontname, true, p_fonttype, p_fontsize );

    return pFont;
}

moFont*
moFontManager::GetFont( moText p_fontname, bool create, moFontType p_fonttype, MOfloat p_fontsize) {

    moFont* pFont = NULL;

    for(int i=0; i< m_Fonts.Count(); i++) {

            pFont = m_Fonts.Get(i);
            if (pFont) {
                if (pFont->GetName() == (moText)p_fontname ) {
                        return pFont;
                }
            }

    }

    if (create)
        pFont = LoadFont( p_fontname, p_fonttype, p_fontsize );

    return pFont;
}

moFont*
moFontManager::GetFont( MOint p_fontid ) {

    moFont* pFont = NULL;

    if( 0<=p_fontid && p_fontid<m_Fonts.Count())
        return m_Fonts[p_fontid];

    return pFont;
}

moFont*
moFontManager::GetFont( moValue& p_value, bool create ) {

    moText fontname;
    moFontType fonttype;
    MOint   fontsize;

    switch(p_value.GetSubValueCount()) {

        case 0:
            return NULL;
            break;
        case 1:
            fontname = p_value.GetSubValue(0).Text();
            return GetFont( fontname, create );
            break;
        case 2:
            fontname = p_value.GetSubValue(0).Text();
            fonttype = (moFontType)p_value.GetSubValue(1).Int();
            return GetFont( fontname, create, fonttype );
            break;
        case 3:
            fontname = p_value.GetSubValue(0).Text();
            fonttype = (moFontType)p_value.GetSubValue(1).Int();
            fontsize = p_value.GetSubValue(2).Int();
            return GetFont( fontname, create, fonttype, fontsize );
            break;
    }

    return NULL;

}

moFont*
moFontManager::LoadFont( moText p_fontname, moFontType p_fonttype, MOfloat p_fontsize ) {

    moFont *pFont = NULL;

	pFont = new moFont();

	if (pFont) {

		moText	completepath;

		completepath = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash;
		completepath+= moText(p_fontname);

		if (pFont->Init( p_fonttype, completepath, p_fontsize )) {
            MODebug->Push( moText("Loaded font: ") + (moText)completepath );
            AddFont(pFont);
        } else {
            MODebug->Push( moText("Error: font: ") + (moText)completepath );
            return NULL;
        }

	}

	return pFont;
}

//===========================================
//
//				moFont
//
//===========================================

moFont::moFont() {

	m_pFace = NULL;

}

moFont::~moFont() {
}


MOboolean
moFont::Init() {
	return true;
}

MOboolean
moFont::Init( moFontType p_Type, moText p_fontname, MOint p_size ) {

	switch( p_Type ) {
		case MO_FONT_OUTLINE:
			m_pFace = new OGLFT::Outline( p_fontname, p_size );
			break;
		case MO_FONT_TRANSLUCENT:
			m_pFace = new OGLFT::Translucent( p_fontname, p_size );
			break;
		case MO_FONT_TRANSLUCENTTEXTURE:
			m_pFace = new OGLFT::TranslucentTexture( p_fontname, p_size );
			break;
		case MO_FONT_FILLED:
			m_pFace = new OGLFT::Filled( p_fontname, p_size );
			break;
		case MO_FONT_GRAYSCALE:
			m_pFace = new OGLFT::Grayscale( p_fontname, p_size );
			break;
		case MO_FONT_MONOCHROME:
			m_pFace = new OGLFT::Monochrome( p_fontname, p_size );
			break;
		case MO_FONT_SOLID:
			m_pFace = new OGLFT::Solid( p_fontname, p_size );
			break;

	}

	if ( m_pFace == 0 || !m_pFace->isValid() ) {
	 cerr << "Could not construct face from " << p_fontname << endl;
	 return false;
	} else {
		m_Name = p_fontname;
		return true;
	}

	return false;
}

MOboolean
moFont::Finish() {

	if (m_pFace!=NULL) {
		delete m_pFace;
		m_pFace = NULL;
		m_Name = moText("");
	}

	return true;

}

void
moFont::SetSize( MOfloat size ) {

	if (m_pFace)
		m_pFace->setPointSize( size );

}

void
moFont::SetForegroundColor( MOfloat p_r, MOfloat p_g, MOfloat p_b) {

	if (m_pFace)
		m_pFace->setForegroundColor( p_r, p_g, p_b );
}

void
moFont::SetHorizontalJustification( OGLFT::Face::HorizontalJustification p_horizontal_justification) {
	if (m_pFace)
		m_pFace->setHorizontalJustification( p_horizontal_justification );
}

void
moFont::SetStringRotation( MOfloat p_string_rotation ) {
	if (m_pFace)
		m_pFace->setStringRotation( p_string_rotation );
}

void
moFont::Draw( MOfloat x, MOfloat y, moText text ) {

	if (m_pFace)
		m_pFace->draw( x, y, text );

}


moText
moFont::GetName() {

    return m_Name;

}

  // Always check to make sure the face was properly constructed


