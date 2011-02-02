/*******************************************************************************

                              moGUIManager.cpp

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

#include "moGUIManager.h"
#include <moFontManager.h>

#include "moArray.h"
moDefineDynamicArray(moGuiObjectArray)
//===========================================
//
//				moGUIManager
//
//===========================================


moGUIManager::moGUIManager() {

	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_GUI );

	SetName("guimanager");
	SetLabelName("guimanager");
/*
	hOpWnd = NULL;
	hVisWnd = NULL;
	*/
	m_OpHandle = NULL;
	m_VisHandle = NULL;
}


moGUIManager::~moGUIManager() {

	Finish();

}

MOboolean
moGUIManager::Init(/*HWND p_hOpWnd, HWND p_hVisWnd*/) {
/*
	if (p_hOpWnd != NULL)
	{
		hOpWnd = p_hOpWnd;
	}
	else
	{
		SDL_SysWMinfo wmInfo;
		if (SDL_GetWMInfo(&wmInfo)) hOpWnd = wmInfo.window;
	}

	if (p_hVisWnd != NULL)
	{
		hVisWnd = p_hVisWnd;
	}
	else
	{
	}

	MODebug2->Push("Handle of Operation Window = " + IntToStr((int)hOpWnd));
	MODebug2->Push("Handle of Visualization Window = " + IntToStr((int)hVisWnd));
*/
	return true;
}

MOboolean
moGUIManager::Init( MO_HANDLE p_OpWindowHandle, MO_DISPLAY p_Display ) {
    SetDisplay( p_Display );
    SetOpWindowHandle( p_OpWindowHandle );
    return true;
}

MOboolean
moGUIManager::Finish() {
	return true;
}

moWindow*
moGUIManager::NewWindow( MOfloat x, MOfloat y, MOfloat width, MOfloat height, moTextHeap &textheap ) {


	return NULL;
}

void
moGUIManager::DisplayInfoWindow( MOfloat x, MOfloat y, MOfloat width, MOfloat height, moTextArray &pTexts ) {

	moWindow*	pWindow = NULL;
	pWindow = new moWindow();
	if (pWindow) {
		if (pWindow->Init( m_pResourceManager, x, y , width, height)) {
			pWindow->SetInfo( pTexts );
			pWindow->Draw();
		}
	}

	delete pWindow;
}

void
moGUIManager::SetDisplay( MO_DISPLAY p_Display )  {

    m_Display = p_Display;

}

void
moGUIManager::SetOpWindowHandle( MO_HANDLE p_OpHandle )  {

    m_OpHandle = p_OpHandle;

}

void
moGUIManager::SetVisWindowHandle( MO_HANDLE p_VisHandle )  {

    m_VisHandle = p_VisHandle;

}

MO_DISPLAY
moGUIManager::GetDisplay()  {

    return m_Display;

}


MO_HANDLE
moGUIManager::GetOpWindowHandle()  {

    return m_OpHandle;

}

MO_HANDLE
moGUIManager::GetVisWindowHandle()  {

    return m_VisHandle;

}

//===========================================
//
//				moGUIObject
//
//===========================================

moGuiObject::moGuiObject() {
	m_pResourceManager = NULL;
}

moGuiObject::~moGuiObject() {
	Finish();
}

MOboolean
moGuiObject::Init( moResourceManager* pResourceManager ) {
	m_pResourceManager = pResourceManager;
	if (m_pResourceManager)
		m_bInitialized = true;
	return moAbstract::Init();
}

MOboolean
moGuiObject::Finish() {
	return false;
}


//===========================================
//
//				moWidget
//
//===========================================

moWidget::moWidget() {

}

moWidget::~moWidget() {
	Finish();
}


//===========================================
//
//				moWindow
//
//===========================================

moWindow::moWindow() {

}

moWindow::~moWindow() {
	Finish();
}

MOboolean
moWindow::Init( moResourceManager* pResourceManager, MOfloat x, MOfloat y, MOfloat width, MOfloat height ) {

	m_bInitialized = moWidget::Init(pResourceManager);

	m_X = x;
	m_Y = y;
	m_Width = width;
	m_Height = height;

	return moAbstract::Init();

}

void
moWindow::SetInfo( moTextArray &pTexts ) {

	MOuint i;

	m_Texts.Empty();

	for(i=0; i<pTexts.Count(); i++) {
		m_Texts.Add(pTexts[i]);
	}

}

void
moWindow::Draw() {
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glBindTexture( GL_TEXTURE_2D, 0 );
	glColor4f( 0.3, 0.3, 0.3, 0.75 );
	glBegin(GL_QUADS);
		glTexCoord2f( 0, 0);
		glVertex2f( m_X, m_Y);

		glTexCoord2f( 0, 1);
		glVertex2f(  m_X+m_Width, m_Y);

		glTexCoord2f( 1, 1);
		glVertex2f(  m_X+m_Width,  m_Y+m_Height);

		glTexCoord2f( 1, 0);
		glVertex2f( m_X,  m_Y+m_Height);
	glEnd();

	moFont* pFont = NULL;
	pFont = m_pResourceManager->GetFontMan()->GetFonts()->Get(0);

	if (pFont) {
		pFont->SetForegroundColor( 0.0, 1.0, 0.0 );
		pFont->SetSize( 13.0 );
		for( MOuint i=0; i<m_Texts.Count(); i++ ) {
			pFont->Draw( m_X , m_Y + m_Height - (13.0+5.0)*(i+1)  ,  m_Texts[i]);
		}
	}

}

void
moWindow::Interaction() {

}

void
moWindow::Update() {


}

//===========================================
//
//				mo3dWidget
//
//===========================================

mo3dWidget::mo3dWidget() {

}

mo3dWidget::~mo3dWidget() {

}

void
mo3dWidget::Draw() {

}

void
mo3dWidget::Interaction() {

}

void
mo3dWidget::Update() {


}
