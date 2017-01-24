/*******************************************************************************

                             moTextureFilterIndex.cpp

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

#include "moTextureFilterIndex.h"

moTextureFilterIndex::moTextureFilterIndex()
{
	m_filters_array.Init(0, NULL);

}
moTextureFilterIndex::~moTextureFilterIndex()
{
	Finish();
}

MOboolean moTextureFilterIndex::Init( moGLManager* p_glman, moFBManager* p_fbman, moShaderManager* p_shaman, moTextureManager* p_texman, moRenderManager* p_renderman)
{
	m_glman = p_glman;
	m_fbman = p_fbman;
	m_shaman = p_shaman;
	m_texman = p_texman;
	m_renderman = p_renderman;

	return true;
}

MOboolean moTextureFilterIndex::Init( moParam* p_param, moGLManager* p_glman, moFBManager* p_fbman, moShaderManager* p_shaman, moTextureManager* p_texman, moRenderManager* p_renderman)
{
	m_glman = p_glman;
	m_fbman = p_fbman;
	m_shaman = p_shaman;
	m_texman = p_texman;
	m_renderman = p_renderman;

	MOuint res = LoadFilters(p_param);

	return 0 < res;
}

MOboolean moTextureFilterIndex::Init(moConfig* p_cfg, MOuint p_param_idx, moGLManager* p_glman, moFBManager* p_fbman, moShaderManager* p_shaman, moTextureManager* p_texman, moRenderManager* p_renderman)
{
	m_glman = p_glman;
	m_fbman = p_fbman;
	m_shaman = p_shaman;
	m_texman = p_texman;
	m_renderman = p_renderman;

	MOuint res = LoadFilters(p_cfg, p_param_idx);

	return 0 < res;
}

MOboolean moTextureFilterIndex::Init(moTextArray* p_filters_str, moGLManager* p_glman, moFBManager* p_fbman, moShaderManager* p_shaman, moTextureManager* p_texman, moRenderManager* p_renderman)
{
	m_glman = p_glman;
	m_fbman = p_fbman;
	m_shaman = p_shaman;
	m_texman = p_texman;
	m_renderman = p_renderman;

	MOuint res = LoadFilters(p_filters_str);

	return 0 < res;
}

MOboolean moTextureFilterIndex::Finish()
{
	m_filters_array.Finish();
	return true;
}

MOint moTextureFilterIndex::Add(moTextureFilter* p_filter)
{
	MOint idx;
	m_filters_array.Add(p_filter);
	idx = m_filters_array.Count() - 1;
	return idx;
}

void moTextureFilterIndex::Apply(MOuint p_idx, MOuint p_i, MOfloat p_fade, const moTextFilterParam& p_params )
{
	if (ValidIndex(p_idx)) m_filters_array[p_idx]->Apply(p_i, p_fade, p_params);
}

void moTextureFilterIndex::Apply(MOuint p_idx, MOfloat p_cycle, MOfloat p_fade, const moTextFilterParam& p_params)
{
	if (ValidIndex(p_idx)) m_filters_array[p_idx]->Apply(p_cycle, p_fade, p_params);
}

void moTextureFilterIndex::Apply(MOuint p_idx, moTempo *p_tempo, MOfloat p_fade, const moTextFilterParam& p_params)
{
	if (ValidIndex(p_idx)) m_filters_array[p_idx]->Apply(p_tempo, p_fade, p_params);
}

void moTextureFilterIndex::Apply(moTempo *p_tempo, MOfloat p_fade, const moTextFilterParam& p_params)
{
	for (MOuint i = 0; i < m_filters_array.Count(); i++) m_filters_array[i]->Apply(p_tempo, p_fade, p_params);
}

MOboolean moTextureFilterIndex::ValidIndex(MOuint p_idx)
{
	if (p_idx < m_filters_array.Count()) return true;
	else
	{
		moText text;
		text =  moText("Error(moTextureIndex): the index: ");
		text += IntToStr(p_idx);
		text +=  moText(" does not exists.");
		MODebug2->Error(text);
		return false;
	}
}

moText
moTextureFilterIndex::MakeTextureFilterLabelName( moValue* p_value ) {

    moText  TextureFilterLabelName = "";
    moText sep = "";
    for(MOuint i=0; i < p_value->GetSubValueCount(); i++) {
        TextureFilterLabelName+= (moText)sep + (moText)p_value->GetSubValue(i).Text();
        sep = moText("::");
    }
    return TextureFilterLabelName;
}


MOint moTextureFilterIndex::TextureFilterExists( moValue* p_value ) {

    moText  TextureFilterLabelName = MakeTextureFilterLabelName( p_value );

    moTextureFilter*    pTextureFilter;

    for( MOuint f=0; f < m_filters_array.Count(); f++ ) {

        pTextureFilter = m_filters_array[f];
        if ( pTextureFilter->GetTextureFilterLabelName() == TextureFilterLabelName ) {
            return f;
        }
    }
    return -1;
}

MOuint moTextureFilterIndex::LoadFilter(moValue* p_value) {

	MOuint j;
	MOint error_code;
	MOuint nFilterParts;

	MOboolean reading_src_tex;

	moTextureFilter *pfilter;
	moTextureArray src_tex, dest_tex;
	moShader *pshader;

	MOuint dest_width, dest_height;
	moText name, extension, left;

    MOint idx = TextureFilterExists(p_value);

    if ( idx != -1)
        return (idx+1);

    nFilterParts = p_value->GetSubValueCount();
    reading_src_tex = true;

    error_code = 0;
    src_tex.Empty();
    dest_tex.Empty();



    for (j = 0; j < nFilterParts; j++)
    {
        moValueBase& vbase( p_value->GetSubValue(j) );

        if (  vbase.GetType() == MO_VALUE_TXT &&
              ( vbase.Type()==MO_DATA_TEXT ||
                vbase.Type()==MO_DATA_IMAGESAMPLE ||
                vbase.Type()==MO_DATA_IMAGESAMPLE_FILTERED )
              ) {

            name = p_value->GetSubValue(j).Text();

            extension = name;
            extension.Right(3);

            if (extension == moText("cfg"))
            {
                reading_src_tex = false;
                error_code = LoadShader(name, &pshader);
            }
            else if (reading_src_tex)
            {
                // Loading source textures...
                error_code = LoadSourceTexture(name, src_tex, j == 0, dest_width, dest_height);
            }
            else
            {
                // Creating destination textures...
                left = name;
                left.Left(3);
                if (left == moText("res:")) error_code = LoadDestTexResolution(name, dest_width, dest_height);
                else error_code = LoadDestTexture(name, dest_tex, dest_width, dest_height);
            }
        }
    }

    if (error_code == 0)
    {
        pfilter = new moTextureFilter();
        moTextFilterParam DefParam;
        pfilter->Init(m_glman, m_renderman, src_tex, dest_tex, pshader, DefParam);
        pfilter->SetTextureFilterLabelName( MakeTextureFilterLabelName( p_value ) );
        Add(pfilter);
    }
    else
    {
        // Print some error message...
        if (error_code == 1)
            MODebug2->Error("Error in creating filter: cannot load shader");
        else if (error_code == 2)
            MODebug2->Error("Error in creating filter: cannot load source texture");
        else if (error_code == 3)
            MODebug2->Error("Error in creating filter: cannot read resolution of destination texture");
        else if (error_code == 4)
            MODebug2->Error("Error in creating filter: cannot create destination texture");
        else
            MODebug2->Error("Unknown error in creating filter.");
    }

	SetupDestTexFBO();

	return m_filters_array.Count();

}

MOuint moTextureFilterIndex::LoadFilters(moParam* p_param)
{

	MOuint i, j;
	MOint error_code;
	MOuint nFilters, nFilterParts;

	MOboolean reading_src_tex;
	moParam* param = p_param;

	moTextureFilter *pfilter = NULL;
	moTextureArray src_tex, dest_tex;
	moShader *pshader = NULL;

	MOuint dest_width, dest_height;
	moText name, extension, left;

	nFilters = p_param->GetValuesCount();

	p_param->FirstValue();
	for (i = 0; i < nFilters; i++)
	{
		param = p_param;
		nFilterParts = param->GetValue().GetSubValueCount();
		reading_src_tex = true;

		error_code = 0;
		src_tex.Empty();
		dest_tex.Empty();
		for (j = 0; j < nFilterParts; j++)
		{
			name = param->GetValue().GetSubValue(j).Text();

			extension = name;
			extension.Right(3);

			if (extension == moText("cfg"))
			{
				reading_src_tex = false;
				error_code = LoadShader(name, &pshader);
			}
			else if (reading_src_tex)
			{
				// Loading source textures...
				error_code = LoadSourceTexture(name, src_tex, j == 0, dest_width, dest_height);
			}
			else
			{
				// Creating destination textures...
				left = name;
				left.Left(3);
				if (left == moText("res:")) error_code = LoadDestTexResolution(name, dest_width, dest_height);
				else error_code = LoadDestTexture(name, dest_tex, dest_width, dest_height);
			}
		}

		if (pshader==NULL) error_code = 1;

		if (error_code == 0)
		{
			pfilter = new moTextureFilter();
            moTextFilterParam DefParam;
			pfilter->Init(m_glman, m_renderman, src_tex, dest_tex, pshader, DefParam);
      pfilter->SetTextureFilterLabelName( MakeTextureFilterLabelName( &param->GetValue() ) );
			Add(pfilter);
		}
		else
		{
			// Print some error message...
			if (error_code == 1)
				MODebug2->Error("Error in creating filter: cannot load shader");
			else if (error_code == 2)
				MODebug2->Error("Error in creating filter: cannot load source texture");
			else if (error_code == 3)
				MODebug2->Error("Error in creating filter: cannot read resolution of destination texture");
			else if (error_code == 4)
				MODebug2->Error("Error in creating filter: cannot create destination texture");
			else
				MODebug2->Error("Unknown error in creating filter.");
		}

		p_param->NextValue();
	}

	SetupDestTexFBO();

	return m_filters_array.Count();

}

MOuint moTextureFilterIndex::LoadFilters(moConfig* p_cfg, MOuint p_param_idx)
{
	MOuint i, j;
	MOint error_code;
	MOuint nFilters, nFilterParts;

	MOboolean reading_src_tex;
	moParam* param;

	moTextureFilter *pfilter = NULL;
	moTextureArray src_tex, dest_tex;
	moShader *pshader = NULL;

	MOuint dest_width, dest_height;
	moText name, extension, left;

	nFilters = p_cfg->GetParam(p_param_idx).GetValuesCount();

	p_cfg->SetCurrentParamIndex(p_param_idx);
	p_cfg->FirstValue();
	for (i = 0; i < nFilters; i++)
	{
		param = &p_cfg->GetParam();
		nFilterParts = param->GetValue().GetSubValueCount();
		reading_src_tex = true;

		error_code = 0;
		src_tex.Empty();
		dest_tex.Empty();
		for (j = 0; j < nFilterParts; j++)
		{
			name = param->GetValue().GetSubValue(j).Text();

			extension = name;
			extension.Right(3);

			if (extension == moText("cfg"))
			{
				reading_src_tex = false;
				error_code = LoadShader(name, &pshader);
			}
			else if (reading_src_tex)
			{
				// Loading source textures...
				error_code = LoadSourceTexture(name, src_tex, j == 0, dest_width, dest_height);
			}
			else
			{
				// Creating destination textures...
				left = name;
				left.Left(3);
				if (left == moText("res:")) error_code = LoadDestTexResolution(name, dest_width, dest_height);
				else error_code = LoadDestTexture(name, dest_tex, dest_width, dest_height);
			}
		}

		if (pshader==NULL) error_code = 1;

		if (error_code == 0)
		{
			pfilter = new moTextureFilter();
            moTextFilterParam DefParam;
			pfilter->Init(m_glman, m_renderman, src_tex, dest_tex, pshader, DefParam);
      pfilter->SetTextureFilterLabelName( MakeTextureFilterLabelName( &param->GetValue() ) );
			Add(pfilter);
		}
		else
		{
			// Print some error message...
			if (error_code == 1)
				MODebug2->Error("Error in creating filter: cannot load shader");
			else if (error_code == 2)
				MODebug2->Error("Error in creating filter: cannot load source texture");
			else if (error_code == 3)
				MODebug2->Error("Error in creating filter: cannot read resolution of destination texture");
			else if (error_code == 4)
				MODebug2->Error("Error in creating filter: cannot create destination texture");
			else
				MODebug2->Error("Unknown error in creating filter.");
		}

		p_cfg->NextValue();
	}

	SetupDestTexFBO();

	return m_filters_array.Count();
}

MOuint moTextureFilterIndex::LoadFilters(moTextArray* p_filters_str)
{
	MOuint i, j;
	MOint error_code;
	MOuint nFilters;

	MOboolean reading_src_tex;

	moTextureFilter *pfilter = NULL;
	moTextureArray src_tex, dest_tex;
	moShader *pshader = NULL;

	MOuint dest_width, dest_height;
	moText name, extension, left, tmp;

	nFilters = p_filters_str->Count();
	for (i = 0; i < nFilters; i++)
	{
		reading_src_tex = true;
    moValue pValue;
		error_code = 0;
		src_tex.Empty();
		dest_tex.Empty();
		tmp = p_filters_str->Get(i);
		j = 0;
		while (tmp != moText(""))
		{
			name = tmp.Scan(moText(" "));
			name = name.Trim();

      if (name!="") pValue.AddSubValue( name, "TXT" );

			extension = name;
			extension.Right(3);

			if (extension == moText("cfg"))
			{
				reading_src_tex = false;
				error_code = LoadShader(name, &pshader);
			}
			else if (reading_src_tex)
			{
				// Loading source textures...
				error_code = LoadSourceTexture(name, src_tex, j == 0, dest_width, dest_height);
			}
			else
			{
				// Creating destination textures...
				left = name;
				left.Left(3);
				if (left == moText("res:")) error_code = LoadDestTexResolution(name, dest_width, dest_height);
				else error_code = LoadDestTexture(name, dest_tex, dest_width, dest_height);
			}
		}

		if (pshader==NULL) error_code = 1;

		if (error_code == 0)
		{
		  moText filterlabelname = MakeTextureFilterLabelName( &pValue );
		  int idx = TextureFilterExists(&pValue);
		  if (idx>0) {

		  } else {
        pfilter = new moTextureFilter();
             moTextFilterParam DefParam;
        pfilter->Init(m_glman, m_renderman, src_tex, dest_tex, pshader, DefParam);
        pfilter->SetTextureFilterLabelName( filterlabelname );
        Add(pfilter);
		  }
		}
		else
		{
			// Print some error message...
			if (error_code == 1)
				MODebug2->Error("Error in creating filter: cannot load shader");
			else if (error_code == 2)
				MODebug2->Error("Error in creating filter: cannot load source texture");
			else if (error_code == 3)
				MODebug2->Error("Error in creating filter: cannot read resolution of destination texture");
			else if (error_code == 4)
				MODebug2->Error("Error in creating filter: cannot create destination texture");
			else
				MODebug2->Error("Unknown error in creating filter.");
		}

		j++;
	}

	SetupDestTexFBO();

	return m_filters_array.Count();
}

MOint moTextureFilterIndex::LoadShader(moText& name, moShader **pshader)
{
	MOint idx = m_shaman->GetShaderIndex(name, true);
	if (-1 < idx)
	{
		*pshader = m_shaman->GetShader(idx);
		return 0;
	}
	return 1;
}

MOint moTextureFilterIndex::LoadSourceTexture(moText& name, moTextureArray& src_tex, MOboolean first_tex, MOuint& dest_width, MOuint& dest_height)
{
	MOint idx = m_texman->GetTextureMOId(name, true);
	if (-1 < idx)
	{
		moTexture* ptex = m_texman->GetTexture(idx);
		src_tex.Add(ptex);
		if (first_tex)
		{
			// By default, the resolution of the destination textures is the same
			// as the source textures.
			dest_width = ptex->GetWidth();
			dest_height = ptex->GetHeight();
		}
		return 0;
	}
	return 2;
}

MOint moTextureFilterIndex::LoadDestTexResolution( const moText& name, MOuint& dest_width, MOuint& dest_height)
{
	moText tmp_width, tmp_height;
	MOint l;

	tmp_height = name;
	tmp_height.Scan(moText("x"));
	l = tmp_height.Length();
	tmp_height.Mid(1, l - 1);

	tmp_width = name;
	tmp_width.Mid(4, tmp_width.Length() - l - 4);

	dest_width = atoi(tmp_width);
	dest_height = atoi(tmp_height);

	return 0;
}

MOint moTextureFilterIndex::LoadDestTexture( const moText& name, moTextureArray& dest_tex, MOuint dest_width, MOuint dest_height)
{
	moTexture* ptex;
	MOint idx = m_texman->GetTextureMOId(name, false);
	if (-1 < idx)
	{
		// The destination texture already exists...
		ptex = m_texman->GetTexture(idx);
		dest_tex.Add(ptex);
		return 0;
	}
	else
	{
		idx = m_texman->AddTexture(name, dest_width, dest_height);
		if (-1 < idx)
		{
			ptex = m_texman->GetTexture(idx);
			dest_tex.Add(ptex);
			return 0;
		}
		else return 4;
	}
}

// Checks if all the destination textures can be assigned to the same fbo, to avoid context changes.
// This implementation is very simple, it not all the textures cannot be assigned to a single fbo,
// then the textures of each filter have their own fbo.
void moTextureFilterIndex::SetupDestTexFBO()
{
	MOuint i, j;
	MOint idx, total_tex_count;
	moTextureIndex* dest_tex;
	moTexture* ptex;
    GLenum target0;
    GLint internal_format0;
	MOuint width0, height0;
	MOboolean first_tex, test, compatible_tex;
	moFBO* pfbo;
	MOuint attach_pt;

	// First, checks if all the destination textures would be compatible with each other to be in the same
	// fbo.
	total_tex_count = 0;
	first_tex = true;
	compatible_tex = true;
	for (i = 0; i < m_filters_array.Count(); i++)
	{
		dest_tex = m_filters_array[i]->GetDestTex();

		total_tex_count += dest_tex->Count();
		for (j = 0; j < dest_tex->Count(); j++)
		{
			ptex = dest_tex->GetTexture(j);

			if (first_tex)
			{
				// Getting parameters of first texture.
				target0 = ptex->GetTexParam().target;
				internal_format0 = ptex->GetTexParam().internal_format;
			    width0 = ptex->GetWidth();
				height0 = ptex->GetHeight();
			}
			else
			{
				// Comparing current texture with the parameters of the first one.
				test = (target0 == ptex->GetTexParam().target) &&
						(internal_format0 == ptex->GetTexParam().internal_format) &&
						(width0 == ptex->GetWidth()) && (height0 == ptex->GetHeight());
				if (!test)
				{
					compatible_tex = false;
					break;
				}
			}

			first_tex = false;
		}
	}

	if (compatible_tex)
	{
		// All the textures can be assigned to the same fbo.
		idx = m_fbman->CreateFBO();
		pfbo = m_fbman->GetFBO(idx);

		if (pfbo == NULL) return;

		if (total_tex_count <= MO_MAX_COLOR_ATTACHMENTS_EXT)
		{
			// All the destination textures can be assigned to the available attachement points of the fbo.
			// This is the optimal scenario.
			for (i = 0; i < m_filters_array.Count(); i++)
			{
				dest_tex = m_filters_array[i]->GetDestTex();
				for (j = 0; j < dest_tex->Count(); j++)
				{
					ptex = dest_tex->GetTexture(j);
					ptex->SetFBO(pfbo);
					pfbo->AddTexture(ptex->GetWidth(), ptex->GetHeight(),
								     ptex->GetTexParam(),
								     ptex->GetGLId(), attach_pt);
					ptex->SetFBOAttachPoint(attach_pt);
				}
			}
		}
		else
		{
			// There are not enough attachement points in the fbo for all the textures. So, all the textures
			// will be assigned to the same attachment point (the first one), and they will be reattached as
			// needed.
			for (i = 0; i < m_filters_array.Count(); i++)
			{
				dest_tex = m_filters_array[i]->GetDestTex();
				for (j = 0; j < dest_tex->Count(); j++)
				{
					ptex = dest_tex->GetTexture(j);
					ptex->SetFBO(pfbo);
				}
			}
		}
	}
	else
	{
		// Creating a different fbo for each filter.
		for (i = 0; i < m_filters_array.Count(); i++)
		{
			dest_tex = m_filters_array[i]->GetDestTex();

			idx = -1;
			pfbo = NULL;
			for (j = 0; j < dest_tex->Count(); j++)
			{
				ptex = dest_tex->GetTexture(j);

				if (j == 0)	idx = m_fbman->CreateFBO();
				pfbo = m_fbman->GetFBO(idx);

				if (pfbo != NULL)
				{
					ptex->SetFBO(pfbo);
					pfbo->AddTexture(ptex->GetWidth(), ptex->GetHeight(),
								     ptex->GetTexParam(),
								     ptex->GetGLId(), attach_pt);
					ptex->SetFBOAttachPoint(attach_pt);
				}
			}
		}
	}

	for (i = 0; i < m_filters_array.Count(); i++)
		m_filters_array[i]->CheckDestTexAttachStatus();
}

