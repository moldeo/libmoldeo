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








/**
#version 150

uniform mat4 model;
uniform vec3 cameraPosition;

// material settings
uniform sampler2D materialTex;
uniform float materialShininess;
uniform vec3 materialSpecularColor;

uniform struct Light {
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
   float attenuation;
   float ambientCoefficient;
} light;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 finalColor;

void main() {
    vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
    vec3 surfacePos = vec3(model * vec4(fragVert, 1));
    vec4 surfaceColor = texture(materialTex, fragTexCoord);
    vec3 surfaceToLight = normalize(light.position - surfacePos);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

    //ambient
    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;

    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
    vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;

    //attenuation
    float distanceToLight = length(light.position - surfacePos);
    float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

    //linear color (color before gamma correction)
    vec3 linearColor = ambient + attenuation*(diffuse + specular);

    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}
*/


MOboolean moShaderManager::Init()
{
	if (m_pResourceManager){
		m_glmanager = m_pResourceManager->GetGLMan();
		m_fbmanager = m_pResourceManager->GetFBMan();
	} else return false;

	if (!m_glmanager || !m_fbmanager) return false;

	if (m_pTextureFilterIndex==NULL)
    		m_pTextureFilterIndex = new moTextureFilterIndex();

	m_shaders_array.Init(0, NULL);

  ///check moResourceInit
  moResource::Init();

  if (m_Config.IsConfigLoaded()) {
    m_pTextureFilterIndex->Init( &m_Config, 0, m_glmanager, m_fbmanager, this, m_pResourceManager->GetTextureMan(), m_pResourceManager->GetRenderMan());
  } else {
    m_pTextureFilterIndex->Init( m_glmanager, m_fbmanager, this, m_pResourceManager->GetTextureMan(), m_pResourceManager->GetRenderMan());
  }
#ifndef OPENGLESV2
  if ( m_glmanager->GetGLMajorVersion() >=2 ) {
#endif
    if (!m_RenderShader.Initialized()) {
      MODebug2->Message("moShaderManager::Init > Creating basic Render Shader...");
      m_RenderShader.Init();
      m_RenderShader.CreateShader(
            moText(
            "attribute vec4 position;\n"
            "attribute vec3 colors;\n"
            "attribute vec2 t_coordedge;\n"
            "attribute vec2 t_coord;\n"
            "attribute vec3 normal;\n"
            "uniform mat4 projmatrix;\n"
            "varying vec3 colorVarying;\n"
            "varying vec3 v_normal;\n"
            "varying vec2 v_texcoord;\n"
            "varying vec2 v_texcoordedge;\n"
            "\n"
            "void main() {\n"
            "	colorVarying = colors;\n"
            "   v_normal = normal;\n"
            "	v_texcoord = t_coord;\n"
            "	v_texcoordedge = t_coordedge;\n"
            "	gl_Position = projmatrix*position;\n"
            "}\n"
            ),
            moText(
          #ifdef OPENGLESV2
            "precision mediump float;"
          #endif
            "varying vec3 colorVarying;\n"
            "uniform sampler2D t_image;\n"
            "varying vec3 v_normal;\n"
            "varying vec2 v_texcoord;\n"
            "varying vec2 v_texcoordedge;\n"
            "uniform float wseg;\n"
            "uniform float hseg;\n"
            "uniform float wireframe_width;"
            "uniform vec3 a_light;\n"
            "uniform vec3 color;\n"
            "uniform float opacity;\n"
            "\n"
            "void main() {\n"
            "	vec4 texcolor = texture2D( t_image, v_texcoord );\n"
            //"	vec4 texcolor = vec4( abs(v_normal.x), abs(v_normal.y), abs(v_normal.z), 1.0);\n"///NORMALES COLORIDAS
            //"	vec4 texcolor = vec4( 1.0, 1.0, 1.0, 1.0);\n"
            "   vec2 vector_to_borderC = vec2( 1.0, 1.0 ) - v_texcoord;\n"
            "   vec2 vector_to_borderB = vec2( 1.0, 0.0 ) - v_texcoord;\n"
            "   vec2 vector_to_borderA = vec2( 0.0, 0.0 ) - v_texcoord;\n"
            "   vec2 vector_to_borderD = vec2( 0.0, 1.0 ) - v_texcoord;\n"
            "   float distance_to_borderX = v_texcoord.x;\n"
            "   float intensity = 0.6+0.4*abs(max( 0.0, dot( v_normal, -1.0*a_light )) );\n"
            //"   float intensity = abs(a_light.y);\n"
            "   float dX = floor( v_texcoord.x * wseg );\n"
            "   float dY = floor( v_texcoord.y * hseg );\n"
            "   float distance_to_borderXd = abs( v_texcoord.x - dX / wseg );\n"
            "   float distance_to_borderY = v_texcoord.y;\n"
            "   float distance_to_borderYd = abs( v_texcoord.y - dY / hseg );\n"
            "   float distance_to_borderA = length(vector_to_borderA);\n"
            "   float distance_to_borderB = length(vector_to_borderB);\n"
            "   float distance_to_borderC = length(vector_to_borderC);\n"
            "   float distance_to_borderD = length(vector_to_borderD);\n"
            "	vec4 wirecolor = vec4( 0.0, 1.0, 0.0, 1.0);\n"
            "	vec4 wirecolorA = vec4( 1.0, 0.0, 0.0, 1.0);\n"
            "	vec4 wirecolorB = vec4( 0.0, 1.0, 0.0, 1.0);\n"
            "	vec4 wirecolorC = vec4( 0.0, 0.0, 1.0, 1.0);\n"
            "	vec4 wirecolorD = vec4( 1.0, 1.0, 1.0, 1.0);\n"
            "   if (distance_to_borderA<(wireframe_width*10.0)) texcolor = wirecolorA;\n"
            "   if (distance_to_borderB<(wireframe_width*10.0)) texcolor = wirecolorB;\n"
            "   if (distance_to_borderC<(wireframe_width*10.0)) texcolor = wirecolorC;\n"
            "   if (distance_to_borderD<(wireframe_width*10.0)) texcolor = wirecolorD;\n"
            "   if (distance_to_borderXd<wireframe_width) texcolor = wirecolor;\n"
            "   if (distance_to_borderYd<wireframe_width) texcolor = wirecolor;\n"
            "   if (distance_to_borderXd>(-wireframe_width+1.0/wseg)) texcolor = wirecolor;\n"
            "   if (distance_to_borderYd>(-wireframe_width+1.0/hseg)) texcolor = wirecolor;\n"
            //"	vec4 mulcolor = vec4( colorVarying, 1.0 );\n"
            "	vec4 mulcolor = intensity*vec4( 1.0*color.r, 1.0*color.g, 1.0*color.b, 1.0*opacity );\n"
            "	gl_FragColor = vec4( mulcolor.x*texcolor.x, mulcolor.y*texcolor.y, mulcolor.z*texcolor.z, mulcolor.w*texcolor.w );\n"
            "}\n"
            )
          );

         m_RenderShader.PrintVertShaderLog();
         m_RenderShader.PrintFragShaderLog();

         m_RenderShaderPositionIndex = m_RenderShader.GetAttribID(moText("position"));
         m_RenderShaderColorsIndex = m_RenderShader.GetAttribID(moText("colors"));
         m_RenderShaderTexCoordIndex = m_RenderShader.GetAttribID(moText("t_coord"));
         m_RenderShaderTexCoordEdgeIndex = m_RenderShader.GetAttribID(moText("t_coordedge"));
         m_RenderShaderNormalIndex = m_RenderShader.GetAttribID(moText("normal"));

         m_RenderShaderColorIndex = m_RenderShader.GetUniformID(moText("color"));
         m_RenderShaderOpacityIndex = m_RenderShader.GetUniformID(moText("opacity"));
         m_RenderShaderTextureIndex = m_RenderShader.GetUniformID(moText("t_image"));
         m_RenderShaderProjectionMatrixIndex = m_RenderShader.GetUniformID("projmatrix");
         m_RenderShaderWireframeWidthIndex = m_RenderShader.GetUniformID(moText("wireframe_width"));
         m_RenderShaderTexWSegmentsIndex = m_RenderShader.GetUniformID(moText("wseg"));
         m_RenderShaderTexHSegmentsIndex = m_RenderShader.GetUniformID(moText("hseg"));
         m_RenderShaderLightIndex = m_RenderShader.GetUniformID(moText("a_light"));

         MODebug2->Message(moText(

                            "moShaderManager::Init > m_RenderShader Attrib IDs,"
                            " position:"+IntToStr(m_RenderShaderPositionIndex)+""
                            " normal:"+IntToStr(m_RenderShaderNormalIndex)+""
                            " color:"+IntToStr(m_RenderShaderColorIndex)+""
                            " opacity:"+IntToStr(m_RenderShaderOpacityIndex)+""
                            " t_coord:"+IntToStr(m_RenderShaderTexCoordIndex)+""
                            " t_coordedge:"+IntToStr(m_RenderShaderTexCoordEdgeIndex)

                            ));

         MODebug2->Message( moText("moShaderManager::Init > m_RenderShader Uniform IDs,")
                            +moText(" projmatrix:")+IntToStr(m_RenderShaderProjectionMatrixIndex)
                            +moText(" wireframe_width:")+IntToStr(m_RenderShaderWireframeWidthIndex)
                            +moText(" wseg:")+IntToStr(m_RenderShaderTexWSegmentsIndex)
                            +moText(" hseg:")+IntToStr(m_RenderShaderTexHSegmentsIndex)
                            +moText(" a_light:")+IntToStr(m_RenderShaderLightIndex)
                            +moText(" t_image:")+IntToStr(m_RenderShaderTextureIndex) );
                        }
#ifndef OPENGLESV2
  }
#endif

  return (m_glmanager && m_fbmanager);
}

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
		pshader_glsl->SetName(p_name);
		pshader_glsl->Init();

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
        moDebugManager::Error( moText("Couldn't load shader config :") + complete_fn );
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
	    if (fragment_idx == MO_PARAM_NOT_FOUND)
            moDebugManager::Error( moText("In shader config :")
                                + complete_fn
                                + moText(" fragment(pixel) shader parameter not founded.")
                                 );
	    if (vertex_idx == MO_PARAM_NOT_FOUND)
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

	int result = AddShader(config.GetParam(type_idx).GetValue().GetSubValue().Int(),
                  p_filename,
                  vertex_fn, fragment_fn, tex_grid);

  if ( result > -1 ) {
    moShader* pshader = GetShader(result);
    if (pshader) {
      MODebug2->Message("Added shader, loading config: " + p_filename );
      pshader->m_Config.LoadConfig( p_filename );
    }
  }

  return result;
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

