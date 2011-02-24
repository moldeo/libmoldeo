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


*******************************************************************************/

#include "moFontManager.h"
#include <moDataManager.h>
#include <moFileManager.h>
#include <moTextureManager.h>

#include "FTGL/ftgl.h"

#include "moArray.h"
moDefineDynamicArray(moFonts)

//===========================================
//
//				moFontManager
//
//===========================================

moFontManager::moFontManager() {
	SetType(MO_OBJECT_RESOURCE);
	SetResourceType(MO_RESOURCETYPE_FONT);

	SetName("fontmanager");
	SetLabelName("fontmanager");
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

		//completepath = m_pResourceManager->GetDataMan()->GetDataPath() + moText("/");
		completepath = moText(DATADIR "/fonts/Tuffy.ttf");

		pFont->Init( MO_FONT_OUTLINE, completepath, 16 );


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
moFontManager::AddFont( moText p_fontname, moText p_fonttype, MOfloat p_fontsize ) {
    return AddFont( p_fontname, GetFontType(p_fonttype), p_fontsize );
}

moFont*
moFontManager::AddFont( moText p_fontname, moFontType p_fonttype, MOfloat p_fontsize ) {

    moFont* pFont = NULL;
    if (! (p_fontname.Trim() == moText(""))) {
        pFont = GetFont( p_fontname, true, p_fonttype, p_fontsize );
    }
    return pFont;
}

moFont*
moFontManager::GetFont( moText p_fontname, bool create, moFontType p_fonttype, MOfloat p_fontsize) {

    moFont* pFont;
    pFont = NULL;

    if (! (p_fontname.Trim().Length() == 0)) {

        if (p_fontname.Trim()==moText("Default") || p_fontname.Trim()==moText("default")) {
            return m_Fonts.Get(0);
        }

        for(int i=0; i< (int)m_Fonts.Count(); i++) {

                pFont = m_Fonts.Get(i);
                if (pFont) {
                    if (pFont->GetName() == (moText)p_fontname ) {
                            return pFont;
                    }
                }

        }

        if (create) {

            moText	completepath;

            completepath = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash;
            completepath+= moText(p_fontname);

            moFile fname( completepath );
            if (!fname.Exists()) {
                    MODebug2->Error(moText("moFontManager: filename does't exists: ") + (moText)completepath );
                    completepath = moText("../../art/fonts/") + moText(p_fontname);
            }

            pFont = LoadFont( completepath, p_fonttype, p_fontsize );
        }
    } else MODebug2->Error(moText("FontManager:: empty string"));
    return pFont;
}

moFont*
moFontManager::GetFont( MOint p_fontid ) {

    moFont* pFont = NULL;

    if( 0<=p_fontid && p_fontid<(int)m_Fonts.Count())
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
moFontManager::LoadFont( moText p_fontname_path, moFontType p_fonttype, MOfloat p_fontsize ) {

    moFont *pFont = NULL;
    moTexture* p_Texture = NULL;
    int idx = -1;

	pFont = new moFont();

	if (pFont) {

        if ( p_fonttype == MO_FONT_GLBUILD ) {
            idx = m_pResourceManager->GetTextureMan()->GetTextureMOId( p_fontname_path, true );
            if (idx>-1) p_Texture = (moTexture*) m_pResourceManager->GetTextureMan()->GetTexture(idx);
            if (p_Texture)
              pFont->Init( p_fonttype, p_fontname_path, p_fontsize, p_Texture->GetGLId() );
            MODebug2->Push( moText("Loaded Bitmap Font: ") + (moText)p_fontname_path );
        } else if ( pFont->Init( p_fonttype, p_fontname_path, p_fontsize) ) {
            MODebug2->Push( moText("Loaded FreeType Font: ") + (moText)p_fontname_path );
        } else {
            MODebug2->Push( moText("Error: font: ") + (moText)p_fontname_path );
            return NULL;
        }

        AddFont(pFont);

	}

	return pFont;
}

moFontType
moFontManager::GetFontType( moText fonttype ) {

    if ( fonttype==moText("TRANSLUCENT") ) {
        return MO_FONT_TRANSLUCENT;
    } else if ( fonttype==moText("FILLED") ) {
        return MO_FONT_FILLED;
    } else if ( fonttype==moText("GRAYSCALE") ) {
        return MO_FONT_GRAYSCALE;
    } else if ( fonttype==moText("MONOCHROME") ) {
        return MO_FONT_MONOCHROME;
    } else if ( fonttype==moText("OUTLINE") ) {
        return MO_FONT_OUTLINE;
    } else if ( fonttype==moText("SOLID") ) {
        return MO_FONT_SOLID;
    } else if ( fonttype==moText("TRANSLUCENTTEXTURE") ) {
        return MO_FONT_TRANSLUCENTTEXTURE;
    } else if ( fonttype==moText("UNDEFINED") ) {
        return MO_FONT_UNDEFINED;
    } else if ( fonttype==moText("GLBUILD") ) {
        return MO_FONT_GLBUILD;
    }
    return MO_FONT_UNDEFINED;
}

//===========================================
//
//				moFont
//
//===========================================

moFont::moFont() {

	m_pFace = NULL;
	m_FontGLId = -1;
	m_FontSize = 12;

}

moFont::~moFont() {
}


MOboolean
moFont::Init() {
	return true;
}

MOboolean
moFont::Init( moFontType p_Type, moText p_fontname, MOint p_size, MOuint glid ) {

  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

  moFile file(p_fontname);

  if (!file.Exists()) {
    MODebug2->Error("moFont:Init filename "+p_fontname+" not found.");
    return false;
  }

	switch( (int)p_Type ) {
		case MO_FONT_OUTLINE://3d
			m_pFace = (FTFont*)new FTGLOutlineFont( p_fontname );
			break;
		case MO_FONT_TRANSLUCENT://2d
            m_pFace = (FTFont*)new FTGLBitmapFont( p_fontname );
			break;
		case MO_FONT_TRANSLUCENTTEXTURE://3d
            m_pFace = (FTFont*)new FTGLTextureFont( p_fontname );
			break;
		case MO_FONT_GRAYSCALE://2d
            m_pFace = (FTFont*)new FTGLPixmapFont( p_fontname );
			break;
		case MO_FONT_MONOCHROME://2d
            m_pFace = (FTFont*)new FTGLPixmapFont( p_fontname );
			break;
		case MO_FONT_SOLID://3d extruded (depth)
            m_pFace = (FTFont*)new FTGLExtrdFont( p_fontname );
			break;
		case MO_FONT_FILLED://3d
            m_pFace = (FTFont*)new FTGLPolygonFont( p_fontname );
			break;
    case MO_FONT_GLBUILD:
            m_FontGLId = glid;
            BuildFont();
			break;
    case MO_FONT_UNDEFINED:
            MODebug2->Error(moText(" FontManager:: UNDEFINED font type"));
            m_pFace = NULL;
            break;

	}


  FTFont* FF = (FTFont*) m_pFace;
  FT_Error FontError;
  if (FF)
    FontError = FF->Error();

	if ( ( p_Type!=MO_FONT_GLBUILD && ( FF == NULL || FontError!=0 ) ) ||
         ( p_Type==MO_FONT_UNDEFINED )  || (p_Type==MO_FONT_GLBUILD && (int)m_FontGLId==-1)) {
        MODebug2->Error(moText("FontManager: Could not construct face from ")+(moText)p_fontname);
        return false;
	} else {
		m_Name = p_fontname;
    if (FF) {
      SetSize(p_size);
      FF->Depth(20);
      //FF->CharMap(ft_encoding_unicode);
    }
		return true;
	}

	return false;
}

MOboolean
moFont::Finish() {

    FTFont* FF = (FTFont*) m_pFace;
    if (FF) {
        delete FF;
        FF = NULL;
    }

	return true;

}

void
moFont::SetSize( MOfloat size ) {

    m_FontSize = size;

    FTFont* FF = (FTFont*) m_pFace;
    if (FF) FF->FaceSize(m_FontSize);

}

void
moFont::SetForegroundColor( MOfloat p_r, MOfloat p_g, MOfloat p_b) {
  glColor3f( p_r, p_g, p_b );
}

void
moFont::SetHorizontalJustification( int p_horizontal_justification) {

    //FTFont* FF = (FTFont*) m_pFace;
    //if (FF) FF->

}

void
moFont::SetStringRotation( MOfloat p_string_rotation ) {

    //FTFont* FF = (FTFont*) m_pFace;
    //if (FF) FF->

}

void
moFont::Draw( MOfloat x, MOfloat y, moText& text) {
	Draw( x, y, text, m_FontSize );
}

void
moFont::Draw( MOfloat x, MOfloat y, moText& text, moFontSize p_fontsize, MOint set, MOfloat sx, MOfloat sy, MOfloat rt ) {

    FTFont* FF = (FTFont*) m_pFace;
    if (FF) {
        SetSize(p_fontsize);
        FF->Render( text, text.Length(), FTPoint(x,y) );
    }

    else {
      if (m_FontGLId>=0) {
          this->glPrint( (int)x, (int)y, text, set, sx, sy, rt );
      }
    }
}

moText
moFont::GetName() {

    return m_Name;

}

void
moFont::BuildFont()								// Build Our Font Display List
{
	float	cx;											// Holds Our X Character Coord
	float	cy;											// Holds Our Y Character Coord
    GLint loop;
	m_GLBaseList = glGenLists(256);								// Creating 256 Display Lists
    glBindTexture( GL_TEXTURE_2D, m_FontGLId );
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	for(loop=0; loop<256; loop++)						// Loop Through All 256 Lists
	{
		cx=float((int)(loop%16))/16.0f;						// X Position Of Current Character
		cy=float((int)(loop/16))/16.0f;						// Y Position Of Current Character

		glNewList(m_GLBaseList+loop,GL_COMPILE);				// Start Building A List
			glBegin(GL_QUADS);							// Use A Quad For Each Character
				glTexCoord2f(cx,1.0f-cy-0.0625f);			// Texture Coord(Bottom Left)
				glVertex2i(0,0);						// Vertex Coord(Bottom Left)
				glTexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);	// Texture Coord(Bottom Right)
				glVertex2i(16,0);						// Vertex Coord(Bottom Right)
				glTexCoord2f(cx+0.0625f,1.0f-cy);			// Texture Coord(Top Right)
				glVertex2i(16,16);						// Vertex Coord(Top Right)
				glTexCoord2f(cx,1.0f-cy);					// Texture Coord(Top Left)
				glVertex2i(0,16);						// Vertex Coord(Top Left)
			glEnd();									// Done Building Our Quad(Character)
			glTranslated(10,0,0);						// Move To The Right Of The Character
		glEndList();									// Done Building The Display List
	}													// Loop Until All 256 Are Built
}


void
moFont::glPrint( GLint x, GLint y, char *string, int set, float scx, float scy, float rt )	// Where The Printing Happens
{
	if(set>1)
	{
		set=1;
	}
    glBindTexture( GL_TEXTURE_2D, m_FontGLId );
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0,800,0,600,-1,1);							// Set Up An Ortho Screen

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	glRotated(rt,0.0,0.0,1.0);
	glTranslated(x,y,0);								// Position The Text(0,0 - Bottom Left)
	glScalef(scx,scy,1.0f);
	glListBase( m_GLBaseList -32+(128*set));						// Choose The Font Set(0 or 1)
	glCallLists(strlen(string),GL_BYTE,string);			// Write The Text To The Screen
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	//glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
}

void
moFont::KillFont()									// Delete The Font From Memory
{
	glDeleteLists( m_GLBaseList, 256);							// Delete All 256 Display Lists

}


