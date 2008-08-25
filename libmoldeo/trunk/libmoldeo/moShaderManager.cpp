/*******************************************************************************

                                moShaderManager.cpp

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                     *
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

#include "moShaderManager.h"
#include "moTextureFilterIndex.h"

moShaderManager::moShaderManager()
{
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_SHADER );
	SetName("Shader Manager");

	m_glmanager = NULL;
	m_fbmanager = NULL;
	m_pTextureFilterIndex = NULL;
}

MOboolean moShaderManager::Init()
{
	if (m_pResourceManager){
		m_glmanager = m_pResourceManager->GetGLMan();
		m_fbmanager = m_pResourceManager->GetFBMan();
	} else return false;

    m_pTextureFilterIndex = new moTextureFilterIndex();

	m_shaders_array.Init(0, NULL);

	moText confignamecompleto = m_pResourceManager->GetDataMan()->GetDataPath();
	confignamecompleto +=  moText("/") + (const moText)GetConfigName();
    confignamecompleto +=  moText(".cfg");

	if(m_Config.LoadConfig(confignamecompleto) == MO_CONFIG_OK ) {
        //carga los filtros que se ejecutaran sistematicamente
        m_pTextureFilterIndex->Init( &m_Config, 0, m_glmanager, m_fbmanager, this, m_pResourceManager->GetTextureMan(), m_pResourceManager->GetRenderMan());
    } else {
        m_pTextureFilterIndex->Init( m_glmanager, m_fbmanager, this, m_pResourceManager->GetTextureMan(), m_pResourceManager->GetRenderMan());
    }

	return (m_glmanager && m_fbmanager);
}

MOboolean moShaderManager::Finish()
{
    if (m_pTextureFilterIndex) {
        delete m_pTextureFilterIndex;
        m_pTextureFilterIndex = NULL;
    }

	m_shaders_array.Finish();

	m_glmanager = NULL;
	m_fbmanager = NULL;

	return true;
}

MOint moShaderManager::GetShaderIndex(moText p_name, MOboolean p_create_shader)
{
	moShader* pshader;
	for (MOuint i = 0; i < m_shaders_array.Count(); i++)
	{
		pshader = m_shaders_array[i];
		if ((pshader != NULL) && (!stricmp(pshader->GetName(), p_name))) return i;
	}
	if (p_create_shader) return AddShader(p_name);
	else return -1;
}

MOint moShaderManager::AddShader(MOuint p_type, moText p_name)
{
	moShader* pshader;
	moShaderGLSL* pshader_glsl;
	if (p_type == MO_SHADER_GLSL)
	{
		pshader_glsl = new moShaderGLSL();
		pshader_glsl->Init();
		pshader_glsl->SetName(p_name);
		pshader = (moShader*)pshader_glsl;
		m_shaders_array.Add(pshader);
		return m_shaders_array.Count() - 1;
	}
	else return -1;
}

MOint moShaderManager::AddShader(moText p_filename)
{
	moConfig config;
	moText complete_fn = m_pResourceManager->GetDataMan()->GetDataPath() + moText("/");
	complete_fn +=  p_filename;

	if (config.LoadConfig(complete_fn) != MO_CONFIG_OK) return -1;

	MOint type_idx = config.GetParamIndex("type");
	if (type_idx == MO_PARAM_NOT_FOUND) return -1;

	MOint vertex_idx = config.GetParamIndex("vertex");
	MOint fragment_idx = config.GetParamIndex("fragment");

	if ((vertex_idx == MO_PARAM_NOT_FOUND) && (fragment_idx == MO_PARAM_NOT_FOUND)) return -1;

	moText vertex_fn;
	moText fragment_fn;

	if (vertex_idx == MO_PARAM_NOT_FOUND) vertex_fn = moText("");
	else
	{
		vertex_fn = m_pResourceManager->GetDataMan()->GetDataPath() + moText("/");
		vertex_fn = vertex_fn + config.GetParam(vertex_idx).GetValue().GetSubValue().Text();
	}

	if (fragment_idx == MO_PARAM_NOT_FOUND) fragment_fn = moText("");
	else
	{
		fragment_fn = m_pResourceManager->GetDataMan()->GetDataPath() + moText("/");
		fragment_fn = fragment_fn + config.GetParam(fragment_idx).GetValue().GetSubValue().Text();
	}

	MOint grid_idx = config.GetParamIndex("grid");
	moTexturedGrid tex_grid;
	if (grid_idx == MO_PARAM_NOT_FOUND)	tex_grid.Set1QuadGrid();
	else tex_grid.Init(&config, grid_idx);

	return AddShader(config.GetParam(type_idx).GetValue().GetSubValue().Int(), p_filename, vertex_fn, fragment_fn, tex_grid);
}

MOint moShaderManager::AddShader(MOuint p_type, moText p_name, moText p_vert_fn, moText p_frag_fn, moTexturedGrid p_tex_grid)
{
	MOint idx = AddShader(p_type, p_name);
	if (-1 < idx)
	{
		if (p_vert_fn == moText("")) GetShader(idx)->LoadFragShader(p_frag_fn);
		else if (p_frag_fn == moText("")) GetShader(idx)->LoadVertShader(p_vert_fn);
		else GetShader(idx)->LoadShader(p_vert_fn, p_frag_fn);
		GetShader(idx)->SetGrid(p_tex_grid);
		return idx;
	}
	else return -1;
}

MOboolean moShaderManager::DeleteShader(MOint p_idx)
{
	if (p_idx < (MOint)m_shaders_array.Count())
	{
		m_shaders_array.Remove(p_idx);
		return true;
	}
	else return false;
}
