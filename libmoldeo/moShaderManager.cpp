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


*******************************************************************************/

#include <moShaderManager.h>
#include <moTextureFilterIndex.h>
#include <moDataManager.h>

moShaderManager::moShaderManager()
{
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_SHADER );

	SetName("shadermanager");
	SetLabelName("shadermanager");

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

  ///check moResourceInit
  moResource::Init();

  if (m_Config.IsConfigLoaded()) {
    m_pTextureFilterIndex->Init( &m_Config, 0, m_glmanager, m_fbmanager, this, m_pResourceManager->GetTextureMan(), m_pResourceManager->GetRenderMan());
  } else {
    m_pTextureFilterIndex->Init( m_glmanager, m_fbmanager, this, m_pResourceManager->GetTextureMan(), m_pResourceManager->GetRenderMan());
  }

  if (!m_RenderShader.Initialized()) {
    MODebug2->Message("moShaderManager::Init > Creating basic Render Shader...");

    m_RenderShader.Init();
    m_RenderShader.CreateShader(
                         moText("attribute vec4 position;")+moText("\n")
                        +moText("attribute vec3 color;")+moText("\n")
                        +moText("attribute vec2 tcoord;")+moText("\n")
                        +moText("uniform mat4 model;")+moText("\n")
                        +moText("varying lowp vec3 colorVarying;")+moText("\n")
                        +moText("void main() {")+moText("\n")
                        +moText("colorVarying = color;")+moText("\n")
                        +moText("gl_Position = position;")+moText("\n")
                        +moText("}"),

                         moText("varying lowp vec3 colorVarying;")+moText("\n")
                        +moText("void main() {")+moText("\n")
                        +moText("gl_FragColor = vec4(colorVarying, 1.0);")+moText("\n")
                        +moText("}")
        );

       m_RenderShader.PrintVertShaderLog();
       m_RenderShader.PrintFragShaderLog();

       m_RenderShaderPositionIndex = m_RenderShader.GetAttribID(moText("position"));
       m_RenderShaderColorIndex = m_RenderShader.GetAttribID(moText("color"));
       m_RenderShaderTextureIndex = m_RenderShader.GetAttribID(moText("tcoord"));
       m_RenderShaderProjectionMatrixIndex = m_RenderShader.GetUniformID("model");

       MODebug2->Message( moText("moShaderManager::Init > m_RenderShader Attrib IDs, position:")+IntToStr(m_RenderShaderPositionIndex)
                         +moText(" color:")+IntToStr(m_RenderShaderColorIndex) );
       MODebug2->Message( moText("moShaderManager::Init > m_RenderShader Uniform IDs, model:")+IntToStr(m_RenderShaderProjectionMatrixIndex)
                         +moText(" color:")+IntToStr(m_RenderShaderProjectionMatrixIndex) );
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
    else if (p_type == MO_SHADER_CG)
	{
#ifdef SHADER_CG
        moShaderCG* pshader_cg;
		pshader_cg = new moShaderCG();
		pshader_cg->Init();
		pshader_cg->SetName(p_name);
		pshader = (moShader*)pshader_cg;
		m_shaders_array.Add(pshader);
		return m_shaders_array.Count() - 1;
#endif
		return -1;
	}

	return -1;
}

MOint moShaderManager::AddShader(moText p_filename)
{

	moConfig config;

	moText complete_fn = m_pResourceManager->GetDataMan()->GetDataPath() + moText("/");
	complete_fn +=  p_filename;


	if (config.LoadConfig(complete_fn) != MO_CONFIG_OK) {
        moDebugManager::Error( moText("Couldn´t load shader config :") + complete_fn );
	    return -1;
	}

  moFile cfgFile( complete_fn );

	MOint type_idx = config.GetParamIndex("type");
	if (type_idx == MO_PARAM_NOT_FOUND) {
	    moDebugManager::Error( moText("In shader config :")
                                + complete_fn
                                + moText(" type parameter not founded.")
                                 );
	    return -1;
	}

	MOint vertex_idx = config.GetParamIndex("vertex");
	MOint fragment_idx = config.GetParamIndex("fragment");

	if ((vertex_idx == MO_PARAM_NOT_FOUND) && (fragment_idx == MO_PARAM_NOT_FOUND)) {
	    if ((fragment_idx == MO_PARAM_NOT_FOUND))
            moDebugManager::Error( moText("In shader config :")
                                + complete_fn
                                + moText(" fragment(pixel) shader parameter not founded.")
                                 );
	    if ((vertex_idx == MO_PARAM_NOT_FOUND))
            moDebugManager::Error( moText("In shader config :")
                                + complete_fn
                                + moText(" vertex shader parameter not founded.")
                                 );
	    return -1;
	}

  moFile vertexFile;
  moFile fragmentFile;

	moText vertex_fn;
	moText fragment_fn;

	if (vertex_idx == MO_PARAM_NOT_FOUND) vertex_fn = moText("");
	else
	{
		vertex_fn = cfgFile.GetPath() + moText("/");
		vertex_fn = vertex_fn + config.GetParam(vertex_idx).GetValue().GetSubValue().Text();
		vertexFile.SetCompletePath( vertex_fn );
    if (!vertexFile.Exists()) {
      vertex_fn = m_pResourceManager->GetDataMan()->GetDataPath() + moText("/");
      vertex_fn = vertex_fn + config.GetParam(vertex_idx).GetValue().GetSubValue().Text();
    }

	}

	if (fragment_idx == MO_PARAM_NOT_FOUND) fragment_fn = moText("");
	else
	{
		fragment_fn = cfgFile.GetPath() + moText("/");
		fragment_fn = fragment_fn + config.GetParam(fragment_idx).GetValue().GetSubValue().Text();
    fragmentFile.SetCompletePath(fragment_fn);
    if (!fragmentFile.Exists()) {
      fragment_fn = m_pResourceManager->GetDataMan()->GetDataPath() + moText("/");
      fragment_fn = fragment_fn + config.Text(fragment_idx);
    }
	}

	MOint grid_idx = config.GetParamIndex("grid");
	moTexturedGrid tex_grid;
	if (grid_idx == MO_PARAM_NOT_FOUND)	tex_grid.Set1QuadGrid();
	else tex_grid.Init(&config, grid_idx);

	return AddShader(config.GetParam(type_idx).GetValue().GetSubValue().Int(),
                  p_filename,
                  vertex_fn, fragment_fn, tex_grid);
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

