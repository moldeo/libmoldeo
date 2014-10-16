/*******************************************************************************

                                moGUIManager.h

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

#ifndef __MOGUIMANAGER_H
#define __MOGUIMANAGER_H

#include <mo3dModelManager.h>


/// Base abstracta de los objetos de interface de usuario (GUI).
/**
*	Los moWidget 's derivan de este objeto y todos aquellos que necesiten ser accedidos por el usuario.
*/
class LIBMOLDEO_API moGuiObject : public moAbstract {

	public:
		moGuiObject();
		virtual ~moGuiObject();
		virtual MOboolean Init( moResourceManager* pResourceManager );
		virtual MOboolean Finish();

	protected:
		moResourceManager*	m_pResourceManager;

};

moDeclareExportedDynamicArray( moGuiObject*, moGuiObjectArray);

/// Objeto de interface de usuario tipo ventana.
/**
*	Los moWidget 's  son los elementos básicos del GUI ( Graphical User Interface ) de Moldeo.
*/
class LIBMOLDEO_API moWidget : public moGuiObject {

	public:
		moWidget();
		virtual ~moWidget();

		virtual void Draw() = 0;
		virtual void Interaction() = 0;
		virtual void Update() = 0;

};

/// Ventana simple
/**
*	Dibuja una ventana especificando posición y tamaño
*
*	@see moWidget
*	@see moGUIObject
*	@see moGUIManager
*/
class LIBMOLDEO_API moWindow : public moWidget {

	public:
		moWindow();
		virtual ~moWindow();
		virtual MOboolean Init( moResourceManager* pResourceManager, MOfloat x, MOfloat y, MOfloat width, MOfloat height );

		void	SetInfo( moTextArray &pTexts );

		virtual void Draw();
		virtual void Interaction();
		virtual void Update();

		moTextArray	m_Texts;

		MOfloat m_X;
		MOfloat m_Y;
		MOfloat m_Width;
		MOfloat m_Height;

};

/// moWidget 3D - objeto tridimensional
/**
*	Con este tipo de objetos introducimos el concepto de objetos GUI interfaceables tridimensionalmente
*
*
*	@see moWidget
*	@see moGUIObject
*	@see moGUIManager
*/
class LIBMOLDEO_API mo3dWidget : public moWidget, public moSceneNode {

	public:
		mo3dWidget();
		virtual ~mo3dWidget();

		virtual void Draw();
		virtual void Interaction();
		virtual void Update();

};

/// Administrador de recursos GUI
/**
*	Aquí se guardan los moWidgets generados y mostrados para el usuario.
*
*	@see moWidget
*	@see moGUIObject
*/
class LIBMOLDEO_API moGUIManager : public moResource {

	public:

		moGUIManager();
		virtual ~moGUIManager();
		virtual MOboolean Init();
		virtual MOboolean Init( MO_HANDLE p_OpHandle, MO_DISPLAY p_Display );
		virtual MOboolean Finish();


		moWindow* NewWindow( MOfloat x, MOfloat y, MOfloat width, MOfloat height, moTextHeap &textheap );

		//moWidget*	GetWidget( MOint widgetid );

		void	DisplayInfoWindow( MOfloat x, MOfloat y, MOfloat width, MOfloat height, moTextArray &pTexts );
		//void	DisplayGauge( MOfloat x, MOfloat y, MOfloat width, MOfloat height, MOfloat percent );
		//void	DisplayText( MOfloat x, MOfloat y, moText ptext );

		//void	DisplayText( MOfloat x, MOfloat y, moText ptext );
		//void	DisplayText( MOfloat x, MOfloat y, moText ptext );
		//void	DisplayText( MOfloat x, MOfloat y, moText ptext );
		//void	DisplayText( MOfloat x, MOfloat y, moText ptext );

        void    SetDisplay( MO_DISPLAY p_Display );
        void    SetOpWindowHandle( MO_HANDLE p_OpHandle );
        void    SetVisWindowHandle( MO_HANDLE p_VisHandle );
        MO_DISPLAY GetDisplay();
        MO_HANDLE GetOpWindowHandle();
        MO_HANDLE GetVisWindowHandle();
/*
		HWND GetOpWindowHandle() { return hOpWnd; }
		HWND GetVisWindowHandle() { return hVisWnd; }
		*/
	private:
	/*
		HWND hOpWnd;
		HWND hVisWnd;*/
		MO_DISPLAY  m_Display;
		MO_HANDLE   m_OpHandle;
		MO_HANDLE   m_VisHandle;

		moGuiObjectArray	m_GuiObjects;
};


#endif /* __MO_GUIMANAGER_H__ */
