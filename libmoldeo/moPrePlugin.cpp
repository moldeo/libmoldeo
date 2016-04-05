/*******************************************************************************

                              moPrePlugin.cpp

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

#include "moPrePlugin.h"

#include "moArray.h"
moDefineDynamicArray( moPrePluginsArray )


moPreEffectFactory::~moPreEffectFactory() {
}


//===========================================
//
//                                   moPrePlugin
//
//===========================================

void moPrePlugin::Load(moText plugin_file)
{

    name = plugin_file;
    handle = moLoadPlugin(plugin_file);

    if(!handle) {
  #ifndef MO_WIN32
        moDebugManager::Error( "moPrePlugin::Load > Cannot open library: " + moText(dlerror()) );
	#else
		CHAR szBuf[80];
		DWORD dw = GetLastError();
		sprintf(szBuf, "%s failed: GetLastError returned %u\n",
			(char*)plugin_file, (int)dw);
    moDebugManager::Error( "moPrePlugin::Load > Cannot open library: " + moText(szBuf) );
	#endif
    return;
    }

    #ifdef MO_WIN32
	FARPROC farp;
	farp = GetProcAddress(handle, "DestroyPreEffectFactory");
    CreatePreEffectFactory = CreatePreEffectFactoryFunction(GetProcAddress(handle, "CreatePreEffectFactory"));
	DestroyPreEffectFactory = DestroyPreEffectFactoryFunction(GetProcAddress(handle, "DestroyPreEffectFactory"));
    #else
    CreatePreEffectFactory = CreatePreEffectFactoryFunction(dlsym(handle, "CreatePreEffectFactory"));
	DestroyPreEffectFactory = DestroyPreEffectFactoryFunction(dlsym(handle, "DestroyPreEffectFactory"));
    #endif

	if(this->CreatePreEffectFactory!=NULL)
		m_factory = this->CreatePreEffectFactory();

}

void moPrePlugin::Unload()
{
	this->DestroyPreEffectFactory();

    CreatePreEffectFactory = NULL;
	DestroyPreEffectFactory = NULL;

    moUnloadPlugin(handle);
}


moPreEffect* moPrePlugin::Create() {

	moPreEffect **narray;

	if(m_factory!=NULL) {
		moPreEffect *pfx = m_factory->Create();

		if(pfx==NULL) return NULL;

		if(n==0) {//primera vez
			array = new moPreEffect* [1];
		} else if(n>0) {//array dinamico
			narray = new moPreEffect* [n+1];//generamos el nuevo vacio
			for(int i=0;i<n;i++) narray[i] = array[i];//copiamos el array
			delete [] array;
			array = narray;
		}

		array[n] = pfx;
		n++;
		return pfx;
	}
	return NULL;
}

bool moPrePlugin::Destroy(moPreEffect *preeffect) {

	moPreEffect **narray;
	int i,j;

	if(m_factory!=NULL) {

		for(j=0;j<n;j++)
			if(array[j]==preeffect) {

                ///pointer encontrado borramos el objeto
			    m_factory->Destroy(preeffect);

                if(n==1) {//muere
                    delete [] array;
                } else if(n>1) {//array dinamico
                    narray = new moPreEffect* [n-1];//generamos el nuevo vacio
                    for(i=0;i<j;i++) narray[i] = array[i];//copiamos el array hasta el j(destruido)
                    for(i=j;i<(n-1);i++) narray[i] = array[i+1];//copiamos el array desde el j
                    delete [] array;
                    array = narray;
                }
                n--;

			    return true;
			}
	}
	return false;
}


LIBMOLDEO_API moPreEffect* moNewPreEffect(moText effect_name, moPrePluginsArray &plugins)
{
    // Creando el nombre complete del plugin(incluyendo ruta por defecto)
    // a partir del nombre del efecto.
    moText complete_name;

    if(!stricmp(effect_name, "nil")) return NULL;

    #if defined(_WIN32)
    complete_name = moText(moDataManager::GetModulesDir()+ "/preeffects/") + (moText)effect_name;
		#ifdef _DEBUG
		complete_name+=  moText("_d");
		#endif
    complete_name +=  moText(".dll");
    #else
    complete_name = moText(moDataManager::GetModulesDir()+ "/preeffects/libmoldeo_") + (moText)effect_name;
		#ifdef _DEBUG
		complete_name+=  moText("_d");
		#endif
    complete_name +=  moPluginExtension;
    #endif

    // Indice del plugin que se utilizara para crear a este efecto.
    int plg_index = -1;

    // First, revisa que el plugin ya no haya sido cargado antes.
    for(MOuint i = 0; i < plugins.Count(); i++)
        if(!stricmp(plugins[i]->GetName(), complete_name))
        {
            plg_index = i;
            break;
        }

    if(plg_index == -1)
    {
        // Es la primera vez que se intenta cargar este plugin. Se lo agrega al array.
        // Falta control de errores(que pasa si la libreria no carga, etc?) :-)
        plg_index = plugins.Count();
		moPrePlugin	*pplugin = new moPrePlugin(complete_name);
        plugins.Add( pplugin );
    }

    // El plugin crea al efecto!
	if(plugins[plg_index]->m_factory!=NULL)
	return plugins[plg_index]->Create();
	else return NULL;
}


LIBMOLDEO_API bool moDeletePreEffect(moPreEffect *preeffect, moPrePluginsArray &plugins)
{
    // Creando el nombre complete del plugin(incluyendo ruta por defecto)
    // a partir del nombre del efecto.
    moText complete_name;

    if(!stricmp(preeffect->GetName(), "")) return false;

    #if defined(_WIN32)
    complete_name = moText(moDataManager::GetModulesDir()+ "/preeffects/") + moText(preeffect->GetName());
		#ifdef _DEBUG
		complete_name+=  moText("_d");
		#endif
    complete_name +=  moText(".dll");
    #else
    complete_name = moText(moDataManager::GetModulesDir()+ "/preeffects/libmoldeo_") + moText(preeffect->GetName());
		#ifdef _DEBUG
		complete_name+=  moText("_d");
		#endif
    complete_name +=  moText(".so");
    #endif

    // Indice del plugin que se utilizara para crear a este efecto.
    int plg_index = -1;

    // First, revisa que el plugin ya no haya sido cargado antes.
    for(MOuint i = 0; i < plugins.Count(); i++)
        if(!stricmp(plugins[i]->GetName(), complete_name))
        {
            plg_index = i;
            break;
        }

    if(plg_index == -1)
    {
        return false;
    }

    bool res = plugins[plg_index]->Destroy(preeffect);

    ///unload plugin if all instances were delete
    if (res && plugins[plg_index]->n == 0) {
        plugins[plg_index]->Unload();
        plugins.Remove(plg_index);

    }

    return res;
}


