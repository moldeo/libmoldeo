/*******************************************************************************

                              moTextureManager.cpp

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

#include "moTextureManager.h"

//===========================================
//
//				moTextureManager
//
//===========================================

moTextureManager::moTextureManager()
{
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_TEXTURE );
	SetName("Texture Manager");

	m_glmanager = NULL;
	m_fbmanager = NULL;
}

moTextureManager::~moTextureManager()
{
	Finish();
}

MOboolean moTextureManager::Init()
{
	if (m_pResourceManager){
		m_glmanager = m_pResourceManager->GetGLMan();
		m_fbmanager = m_pResourceManager->GetFBMan();
	} else return false;

	m_textures_array.Init(0, NULL);
	return (m_glmanager && m_fbmanager);
}

MOboolean moTextureManager::Finish()
{
	m_textures_array.Finish();

	m_glmanager = NULL;
	m_fbmanager = NULL;

	return true;
}

MOint moTextureManager::GetTextureMOId(moParam *param, MOboolean p_create_tex)
{
	MOint moid = GetTextureMOId(param->GetValue().GetSubValue(0).Text(), false);
	if (-1 < moid) return moid;
	else
		if (p_create_tex) return AddTexture(param);
		else return -1;
}

MOint moTextureManager::GetTextureMOId(moValue *value, MOboolean p_create_tex)
{
	MOint moid = GetTextureMOId( value->GetSubValue(0).Text(), false);
	if (-1 < moid) return moid;
	else
		if (p_create_tex) return AddTexture(value);
		else return -1;
}

MOint moTextureManager::GetTextureMOId(moText p_name, MOboolean p_create_tex)
{
	moTexture* ptex;
	for (MOuint i = 0; i < m_textures_array.Count(); i++)
	{
		ptex = m_textures_array[i];
		if ((ptex != NULL) && (!stricmp(ptex->GetName(), p_name))) return i;
	}
	if (p_create_tex) return AddTexture(p_name);
	else return -1;
}

MOint moTextureManager::GetTextureMOId(MOuint p_glid)
{
	moTexture* ptex;
	for (MOuint i = 0; i < m_textures_array.Count(); i++)
	{
		ptex = m_textures_array[i];
		if ((ptex != NULL) && (ptex->GetGLId() == p_glid)) return i;
	}
	return -1;
}

MOint moTextureManager::AddTexture(MOuint p_type, moText p_name, moTexParam p_tex_param)
{
	moTexture* ptex = CreateTexture(p_type, p_name, p_tex_param);
	if (ptex != NULL)
	{
		m_textures_array.Add(ptex);
		ptex->SetMOId(m_textures_array.Count() - 1);
		return ptex->GetMOId();
	}
	else return -1;
}

MOint moTextureManager::AddTexture(moParam* p_param)
{
    moValue* pValue = &p_param->GetValue();
	return AddTexture( pValue );
}

MOint moTextureManager::AddTexture(moValue* p_value)
{
	moText name = p_value->GetSubValue(0).Text();
	MOuint type = GetTypeForFile(name);
	moTextureMultiple* ptex_mult;
	moMovie* ptex_mov;
	moVideoBuffer* ptex_videobuffer;
	moCircularVideoBuffer* ptex_circularvideobuffer;

	moTexture* ptex = CreateTexture(type, name);
	if (ptex != NULL)
	{
		MOboolean res = false;
		if (type == MO_TYPE_TEXTURE) res = ptex->Load( p_value );
		else if (type == MO_TYPE_TEXTURE_MULTIPLE)
		{
			ptex_mult = (moTextureMultiple*)ptex;
			res = CreateMultipleTex(ptex_mult, p_value);
		}
		else if (type == MO_TYPE_MOVIE)
		{
			ptex_mov = (moMovie*)ptex;
			res = ptex_mov->Load(p_value);
		}
		else if (type == MO_TYPE_VIDEOBUFFER)
		{
			ptex_videobuffer = (moVideoBuffer*)ptex;
			res = ptex_videobuffer->Load(p_value);
		}
		else if (type == MO_TYPE_CIRCULARVIDEOBUFFER)
		{
			ptex_circularvideobuffer = (moCircularVideoBuffer*)ptex;
			res = ptex_circularvideobuffer->Load(p_value);
		}
		if (res)
		{
			m_textures_array.Add(ptex);
			ptex->SetMOId(m_textures_array.Count() - 1);
			return ptex->GetMOId();
		}
		else
		{
			delete ptex;
			return -1;
		}
	}
	else return -1;
}

MOint moTextureManager::AddTexture(moText p_name, MOuint p_width, MOuint p_height, moTexParam p_tex_param)
{
	moTexture* ptex = CreateTexture(MO_TYPE_TEXTURE, p_name, p_tex_param);
	if (ptex != NULL)
	{
		MOboolean res = ptex->BuildEmpty(p_width, p_height);
		if (res)
		{
			m_textures_array.Add(ptex);
			ptex->SetMOId(m_textures_array.Count() - 1);
			return ptex->GetMOId();
		}
		else
		{
			delete ptex;
			return -1;
		}
	}
	else return -1;
}

MOint moTextureManager::AddTexture(moText p_name, MOuint p_width, MOuint p_height, GLvoid* p_buffer, GLenum p_pixel_format, GLenum p_pixel_type, moTexParam p_tex_param)
{
	moTexture* ptex = CreateTexture(MO_TYPE_TEXTURE, p_name, p_tex_param);
	if (ptex != NULL)
	{
		MOboolean res = ptex->BuildFromBuffer(p_width, p_height, p_buffer, p_pixel_format, p_pixel_type);
		if (res)
		{
			m_textures_array.Add(ptex);
			ptex->SetMOId(m_textures_array.Count() - 1);
			return ptex->GetMOId();
		}
		else
		{
			delete ptex;
			return -1;
		}
	}
	else return -1;
}

MOint moTextureManager::AddTexture(moText p_filename)
{
	moText name = p_filename;
	MOuint type = GetTypeForFile(name);
	moMovie* ptex_mov;

	moTexture* ptex = CreateTexture(type, name);
	p_filename = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)p_filename;
	if (ptex != NULL)
	{
		MOboolean res = false;
		if (type == MO_TYPE_TEXTURE) res = ptex->BuildFromFile(p_filename);
		else if (type == MO_TYPE_MOVIE)
		{
			ptex_mov = (moMovie*)ptex;
			res = ptex_mov->LoadMovieFile(p_filename);
		}
		if (res)
		{
			m_textures_array.Add(ptex);
			ptex->SetMOId(m_textures_array.Count() - 1);
			return ptex->GetMOId();
		}
		else
		{
			delete ptex;
			return -1;
		}
	}
	else return -1;
}

MOboolean moTextureManager::DeleteTexture(MOuint p_moid)
{
	if (ValidTexture(p_moid))
	{
		moTexture* ptex = m_textures_array[p_moid];
		if (ptex)
			delete ptex;
		m_textures_array.Remove(p_moid);
		for (MOuint i = p_moid; i < m_textures_array.Count(); i++) m_textures_array[i]->SetMOId(i);
		return true;
	}
	else return false;
}

MOuint moTextureManager::GetTypeForFile(moText p_filename)
{
	moText extension = p_filename;
	moText begin = p_filename;
	begin.Left(7);
	extension.Right(3);

	if (!stricmp(extension,"tga") || !stricmp(extension,"jpg") || !stricmp(extension,"png"))
		return MO_TYPE_TEXTURE;
	if (!stricmp(begin,"multiple"))
		return MO_TYPE_TEXTURE_MULTIPLE;
	if (!stricmp(extension,"avi") || !stricmp(extension,"mov") || !stricmp(extension,"mpg") || !stricmp(extension,"vob"))
		return MO_TYPE_MOVIE;

	return MO_TYPE_TEXTURE;
}

moTexture* moTextureManager::CreateTexture(MOuint p_type, moText p_name, moTexParam p_param)
{
	moTexture* ptex;
	moTextureMultiple* ptex_mult;
	moMovie* ptex_movie;
	moVideoBuffer* ptex_videobuffer;
	moCircularVideoBuffer* ptex_circularvideobuffer;

	MOuint new_moid = m_textures_array.Count();

	if (p_type == MO_TYPE_TEXTURE)
	{
		ptex = new moTexture;
	}
	else if (p_type == MO_TYPE_TEXTURE_MULTIPLE)
	{
		ptex_mult = new moTextureMultiple;
		ptex = (moTexture*)ptex_mult;
	}
	else if (p_type == MO_TYPE_MOVIE)
	{
		ptex_movie = new moMovie;
		ptex = (moTexture*)ptex_movie;
	}
	else if (p_type == MO_TYPE_VIDEOBUFFER)
	{
		ptex_videobuffer = new moVideoBuffer;
		ptex = (moTexture*)ptex_videobuffer;
	}
	else if (p_type == MO_TYPE_CIRCULARVIDEOBUFFER)
	{
		ptex_circularvideobuffer = new moCircularVideoBuffer;
		ptex = (moTexture*)ptex_circularvideobuffer;
	}
	else return NULL;

	ptex->Init(p_name, new_moid, m_pResourceManager, p_param);
	return ptex;
}

MOboolean moTextureManager::CreateMultipleTex(moTextureMultiple* p_tex_mult, moParam* p_param)
{
    moValue* pValue = &p_param->GetValue();
	return CreateMultipleTex(p_tex_mult, pValue);
}

MOboolean moTextureManager::CreateMultipleTex(moTextureMultiple* p_tex_mult, moValue* p_value)
{
	moText texfn;
	MOint moid;
	MOuint nfiles = p_value->GetSubValueCount() - 1;

	for (MOuint i = 0; i < nfiles; i++)
	{
		texfn = p_value->GetSubValue(i + 1).Text();
		moid = GetTextureMOId(texfn, true);
		if (-1 < moid) p_tex_mult->AddFrame(GetTexture(moid));
	}

	return 0 < p_tex_mult->GetTextureCount();
}

MOint moTextureManager::GetType(MOuint p_moid) {
    return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetType() : MO_UNDEFINED;

}

MOuint moTextureManager::GetGLId(MOuint p_moid) {
    return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetGLId() : 0;
}
