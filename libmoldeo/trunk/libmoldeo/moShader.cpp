/*******************************************************************************

                               moShader.cpp

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *   (at your option) any later version.                                    *
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

  Copyright (C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa
  Andres Colubri

  Description:
  Base class for GLSL and Cg shaders.

*******************************************************************************/

#include "moShader.h"

#include "moArray.cpp"
moDefineDynamicArray(moShaderArray)

//===========================================
//
//				moTexturedGrid
//
//===========================================

moTexturedGrid::moTexturedGrid()
{
	m_num_layers = 0;
	m_size_x = 0;
	m_size_y = 0;
}

moTexturedGrid::~moTexturedGrid()
{
	Finish();
}

MOboolean moTexturedGrid::Init(MOint p_size_x, MOint p_size_y, MOint p_num_layers, const MOfloat p_grid_dx[], const MOfloat p_grid_dy[])
{
	m_size_x = p_size_x;
	m_size_y = p_size_y;

	m_num_layers = p_num_layers;
	for (int i = 0; i < m_num_layers; i++)
	{
		m_grid_dx[i] = p_grid_dx[i];
		m_grid_dy[i] = p_grid_dy[i];
	}
	return true;
}

// The format of the grid parameters is the following:
// nx ny
// dx0 dy0
// dx1 dy1
// ...
// dxn dyn
// there nx and ny are the number of grid quads along x and y, and dxi, dyi
// are the grid spacings on each layer (the layer 0 represents the grid itself,
// the rest of the layers represent the texture coordinates).
MOboolean moTexturedGrid::Init(moConfig* p_cfg, MOuint p_param_idx)
{
	moParam* param;
	MOuint count = p_cfg->GetParam(p_param_idx).GetValue().GetSubValueCount();

	p_cfg->SetCurrentParamIndex(p_param_idx);
	p_cfg->FirstValue();
	for (MOuint i = 0; i < count; i++)
	{
		param = &p_cfg->GetParam();
		if (i == 0)
		{
			m_size_x = param->GetValue().GetSubValue(0).Int() + 1;
			m_size_y = param->GetValue().GetSubValue(1).Int() + 1;
		}
		else
		{
			// Reading layers.
			m_grid_dx[i - 1] = param->GetValue().GetSubValue(0).Float();
			m_grid_dy[i - 1] = param->GetValue().GetSubValue(1).Float();

		}
		p_cfg->NextValue();
	}

	// if there is only one parameter line, it should contain the
	// grid dimensions. The 0th layer is default to the grid spacing
	// corresponding to 1/(number of grid quads)
	if (count == 1)
	{
		m_num_layers = 1;
		m_grid_dx[0] = 1.0 / float(m_size_x - 1);
		m_grid_dy[0] = 1.0 / float(m_size_y - 1);
	}
	else m_num_layers = count - 1;
	return true;
}

void moTexturedGrid::Set1QuadGrid()
{
	m_num_layers = 1;
	m_size_x = 2;
	m_size_y = 2;
	m_grid_dx[0] = 1.0;
	m_grid_dy[0] = 1.0;
}

MOboolean moTexturedGrid::Finish() {
	return true;
}

void moTexturedGrid::GetPoint(MOint layer, MOint i, MOint j, MOfloat &x, MOfloat &y)
{
	if (layer < m_num_layers)
	{
		x = i * m_grid_dx[layer];
		y = j * m_grid_dy[layer];
	}
	else
	{
		// If there are not layers defined below the 0th, then the texture coordinates
		// are set identical to the grid coordinates.
		x = i * m_grid_dx[0];
		y = j * m_grid_dy[0];
	}
}

void moTexturedGrid::Draw(MOint w, MOint h, MOint l)
{
	float x0, y0, x1, y1;

	for (int j = 0; j < m_size_y - 1; j++)
	{
		glBegin(GL_QUAD_STRIP);
			for (int i = 0; i < m_size_x; i++)
			{
				GetPoint(0, i, j, x0, y0);
				GetPoint(0, i, j + 1, x1, y1);

				x0 *= w; y0 *= h;
				x1 *= w; y1 *= h;

				SetTexCoord(i, j, l);
				glVertex2f(x0, y0);

				SetTexCoord(i, j + 1, l);
				glVertex2f(x1, y1);
			}
		glEnd();
	}
}

void moTexturedGrid::SetTexCoord(MOint i, MOint j, MOint l)
{
	float s, t;
	for (int k = 1; k <= l; k++)
	{
		GetPoint(k, i, j, s, t);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB + k - 1, s, t);
	}
}

moTexturedGrid &moTexturedGrid::operator = (const moTexturedGrid &p_src_grid)
{
	Finish();
	Init(p_src_grid.m_size_x, p_src_grid.m_size_y, p_src_grid.m_num_layers, p_src_grid.m_grid_dx, p_src_grid.m_grid_dy);
	return *this;
}

//===========================================
//
//				moShader
//
//===========================================

moShader::moShader()
{
    m_Active = 0;
    m_VertErrorCode = 0;
    m_FragErrorCode = 0;

	m_name = moText("");
}

moShader::~moShader()
{
    Finish();
}

MOboolean moShader::Init()
{
	return true;
}

MOboolean moShader::Finish()
{
    m_VertErrorCode = 0;
    m_FragErrorCode = 0;

    if (ShaderActive()) StopShader();
	return true;
}

void moShader::StartShader()
{
    m_Active = 1;
}

void moShader::StopShader()
{
    m_Active = 0;
}

MOboolean moShader::ShaderActive()
{
    return m_Active;
}

MOint moShader::VertErrorCode()
{
    return m_VertErrorCode;
}

MOint moShader::FragErrorCode()
{
    return m_FragErrorCode;
}

moText moShader::LoadShaderSource(moText p_fn)
{
	FILE *src_file = fopen(p_fn, "rt");
	moText src_text;

	if (src_file)
	{
		char *content = NULL;
		int count = 0;

		fseek(src_file, 0, SEEK_END);
		count = ftell(src_file);
		fseek(src_file, 0, SEEK_SET);
        if (count > 0)
		{
			content = (char *)malloc(sizeof(char) * (count+1));
			count = fread(content,sizeof(char), count, src_file);
			content[count] = '\0';
		}
		fclose(src_file);

		src_text = moText(content);
	}
	else
	{
		moText errstr = "Shader source file ";
		errstr += p_fn + moText(" not found") ;
		if (MODebug != NULL) MODebug->Push(errstr);
		src_text = moText("");
	}

	return src_text;
}
