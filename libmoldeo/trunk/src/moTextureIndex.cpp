/*******************************************************************************

                              moTextureIndex.cpp

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
  Andrés Colubri#include "moTextureManager.h"

*******************************************************************************/


#include "moTexture.h"


MOint moTextureIndex::Add(moText p_name, moTextureManager* p_tex_man) // Returns the index of the newly added texture.
{
	MOint moid = p_tex_man->GetTextureMOId(p_name, true);
	if (-1 < moid) return Add(p_tex_man->GetTexture(moid));
	else return -1;
}

MOint moTextureIndex::Add(moTexture* p_tex)
{
	MOint idx;
	m_textures_array.Add(p_tex);
	idx = m_textures_array.Count() - 1;
	moText msg;
	msg = "Texture loaded: ";
	msg += m_textures_array[idx]->GetName();
	//MODebug->Push(msg);
	return idx;
}

MOuint moTextureIndex::GetGLId(MOuint p_idx) // Without synchro...
{
	if (ValidIndex(p_idx)) return m_textures_array[p_idx]->GetGLId();
	return MO_UNDEFINED;
}

MOuint moTextureIndex::GetGLId(MOuint p_idx, MOint p_cuad) // Without synchro, and arbitrary frame...
{
	if (ValidIndex(p_idx))
		if ((p_cuad >= 0) && ((m_textures_array[p_idx]->GetType() == MO_TYPE_TEXTURE_MULTIPLE) ||
								(m_textures_array[p_idx]->GetType() == MO_TYPE_MOVIE) ||
								(m_textures_array[p_idx]->GetType() == MO_TYPE_VIDEOBUFFER)))
		{
			moTextureAnimated* ptex_anim = (moTextureAnimated*)m_textures_array[p_idx];
			return ptex_anim->GetGLId((MOuint)p_cuad);
		}
		else return m_textures_array[p_idx]->GetGLId();
	else return 0;
}

MOuint moTextureIndex::GetGLId(MOuint p_idx, MOfloat p_cycle) // Without synchro, and arbitrary frame...
{
	if (ValidIndex(p_idx))
		if ((p_cycle >= 0.0) && ((m_textures_array[p_idx]->GetType() == MO_TYPE_TEXTURE_MULTIPLE) ||
								(m_textures_array[p_idx]->GetType() == MO_TYPE_MOVIE) ||
								(m_textures_array[p_idx]->GetType() == MO_TYPE_VIDEOBUFFER)))
		{
			moTextureAnimated* ptex_anim = (moTextureAnimated*)m_textures_array[p_idx];
			return ptex_anim ->GetGLId((MOfloat)p_cycle);
		}
		else return m_textures_array[p_idx]->GetGLId();
	else return 0;
}

MOuint moTextureIndex::GetGLId(MOuint p_idx, moTempo *p_tempo) // With synchro.
{
	if (ValidIndex(p_idx))
		if ((p_tempo != NULL) && ((m_textures_array[p_idx]->GetType() == MO_TYPE_TEXTURE_MULTIPLE) ||
								(m_textures_array[p_idx]->GetType() == MO_TYPE_MOVIE) ||
								(m_textures_array[p_idx]->GetType() == MO_TYPE_VIDEOBUFFER)))
		{
			moTextureAnimated* ptex_anim = (moTextureAnimated*)m_textures_array[p_idx];
			return ptex_anim->GetGLId(p_tempo);
		}
		else return m_textures_array[p_idx]->GetGLId();
	else return 0;
}

MOint moTextureIndex::GetType(MOuint p_idx)
{
	if (ValidIndex(p_idx)) return (m_textures_array[p_idx]->GetType());
	return -1;
}

moTexture* moTextureIndex::GetTexture(MOuint p_idx)
{
	if (ValidIndex(p_idx)) return m_textures_array[p_idx];
	return NULL;
}

MOboolean moTextureIndex::ValidIndex(MOuint p_idx)
{
	if (p_idx < m_textures_array.Count()) return true;
	else
	{
		moText text;
		text =  "Error(moTextureIndex): the index: ";
		text += IntToStr(p_idx);
		text +=  " does not exists.";
		//MODebug->Push(text);
		return false;
	}
}

MOuint moTextureIndex::LoadTextures(moConfig* p_cfg, MOuint p_param_idx, moTextureManager* p_tex_man)
{
	MOint moid;
	MOint idx = -1;
	MOuint nImages = p_cfg->GetParam(p_param_idx).GetValuesCount();
	moTexture* ptex;

	p_cfg->SetCurrentParamIndex(p_param_idx);
	p_cfg->FirstValue();
	for (MOuint i = 0; i < nImages; i++)
	{
		moid = p_tex_man->GetTextureMOId(&p_cfg->GetParam(), true);
		if (-1 < moid)
		{
			idx++;
			ptex = p_tex_man->GetTexture(moid);
			m_textures_array.Add(ptex);
			moText msg;
			msg = "Texture loaded: ";
			msg += ptex->GetName();
			//MODebug->Push(msg);
		} else {
			m_textures_array.Add(NULL);
		}
		p_cfg->NextValue();
	}
	return m_textures_array.Count();
}
