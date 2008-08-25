/*******************************************************************************

                                moTextureFilter.cpp

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

#include "moTextureFilter.h"

#include "moArray.cpp"
moDefineDynamicArray(moTextureFilterArray)

moColorMatrix::moColorMatrix()
{
    ZeroMatrix();
}

moColorMatrix::~moColorMatrix()
{
	Finish();
}

void moColorMatrix::Init()
{
}

void moColorMatrix::Finish()
{
}

void moColorMatrix::Copy(moColorMatrix& p_mat)
{
	int i, j;
	for (j = 0; j < 4; j++)
		for (i = 0; i < 4; i++)
			m_Matrix[MatIdx(i, j)] = p_mat.m_Matrix[MatIdx(i, j)];
}

void moColorMatrix::Multiply(moColorMatrix& p_mat)
{
	int i, j;
	for (j = 0; j < 4; j++)
		for (i = 0; i < 4; i++)
			m_MatrixChg[MatIdx(i, j)] = p_mat.m_Matrix[MatIdx(i, j)];

    ApplyChgMatrix();
}

void moColorMatrix::ZeroMatrix()
{
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			m_Matrix[MatIdx(i, j)] = 0.0;
}

void moColorMatrix::IdentityMatrix()
{
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			m_Matrix[MatIdx(i, j)] = i == j;
}

void moColorMatrix::SetXRotateMatrix(float rs, float rc)
{
    m_MatrixChg[MatIdx(0, 0)] = 1.0;
    m_MatrixChg[MatIdx(0, 1)] = 0.0;
    m_MatrixChg[MatIdx(0, 2)] = 0.0;
    m_MatrixChg[MatIdx(0, 3)] = 0.0;

    m_MatrixChg[MatIdx(1, 0)] = 0.0;
    m_MatrixChg[MatIdx(1, 1)] = rc;
    m_MatrixChg[MatIdx(1, 2)] = rs;
    m_MatrixChg[MatIdx(1, 3)] = 0.0;

    m_MatrixChg[MatIdx(2, 0)] = 0.0;
    m_MatrixChg[MatIdx(2, 1)] = -rs;
    m_MatrixChg[MatIdx(2, 2)] = rc;
    m_MatrixChg[MatIdx(2, 3)] = 0.0;

    m_MatrixChg[MatIdx(3, 0)] = 0.0;
    m_MatrixChg[MatIdx(3, 1)] = 0.0;
    m_MatrixChg[MatIdx(3, 2)] = 0.0;
    m_MatrixChg[MatIdx(3, 3)] = 1.0;
}

void moColorMatrix::SetYRotateMatrix(float rs, float rc)
{
    m_MatrixChg[MatIdx(0, 0)] = rc;
    m_MatrixChg[MatIdx(0, 1)] = 0.0;
    m_MatrixChg[MatIdx(0, 2)] = -rs;
    m_MatrixChg[MatIdx(0, 3)] = 0.0;

    m_MatrixChg[MatIdx(1, 0)] = 0.0;
    m_MatrixChg[MatIdx(1, 1)] = 1.0;
    m_MatrixChg[MatIdx(1, 2)] = 0.0;
    m_MatrixChg[MatIdx(1, 3)] = 0.0;

    m_MatrixChg[MatIdx(2, 0)] = rs;
    m_MatrixChg[MatIdx(2, 1)] = 0.0;
    m_MatrixChg[MatIdx(2, 2)] = rc;
    m_MatrixChg[MatIdx(2, 3)] = 0.0;

    m_MatrixChg[MatIdx(3, 0)] = 0.0;
    m_MatrixChg[MatIdx(3, 1)] = 0.0;
    m_MatrixChg[MatIdx(3, 2)] = 0.0;
    m_MatrixChg[MatIdx(3, 3)] = 1.0;
}

void moColorMatrix::SetZRotateMatrix(float rs, float rc)
{
    m_MatrixChg[MatIdx(0, 0)] = rc;
    m_MatrixChg[MatIdx(0, 1)] = rs;
    m_MatrixChg[MatIdx(0, 2)] = 0.0;
    m_MatrixChg[MatIdx(0, 3)] = 0.0;

    m_MatrixChg[MatIdx(1, 0)] = -rs;
    m_MatrixChg[MatIdx(1, 1)] = rc;
    m_MatrixChg[MatIdx(1, 2)] = 0.0;
    m_MatrixChg[MatIdx(1, 3)] = 0.0;

    m_MatrixChg[MatIdx(2, 0)] = 0.0;
    m_MatrixChg[MatIdx(2, 1)] = 0.0;
    m_MatrixChg[MatIdx(2, 2)] = 1.0;
    m_MatrixChg[MatIdx(2, 3)] = 0.0;

    m_MatrixChg[MatIdx(3, 0)] = 0.0;
    m_MatrixChg[MatIdx(3, 1)] = 0.0;
    m_MatrixChg[MatIdx(3, 2)] = 0.0;
    m_MatrixChg[MatIdx(3, 3)] = 1.0;
}

void moColorMatrix::SetZShearMatrix(float dx, float dy)
{
    m_MatrixChg[MatIdx(0, 0)] = 1.0;
    m_MatrixChg[MatIdx(0, 1)] = 0.0;
    m_MatrixChg[MatIdx(0, 2)] = dx;
    m_MatrixChg[MatIdx(0, 3)] = 0.0;

    m_MatrixChg[MatIdx(1, 0)] = 0.0;
    m_MatrixChg[MatIdx(1, 1)] = 1.0;
    m_MatrixChg[MatIdx(1, 2)] = dy;
    m_MatrixChg[MatIdx(1, 3)] = 0.0;

    m_MatrixChg[MatIdx(2, 0)] = 0.0;
    m_MatrixChg[MatIdx(2, 1)] = 0.0;
    m_MatrixChg[MatIdx(2, 2)] = 1.0;
    m_MatrixChg[MatIdx(2, 3)] = 0.0;

    m_MatrixChg[MatIdx(3, 0)] = 0.0;
    m_MatrixChg[MatIdx(3, 1)] = 0.0;
    m_MatrixChg[MatIdx(3, 2)] = 0.0;
    m_MatrixChg[MatIdx(3, 3)] = 1.0;
}

void moColorMatrix::XFormPoint(float &tx, float &ty, float &tz)
{
    float x = RLUM;
	float y = GLUM;
	float z = BLUM;

    tx = x * m_Matrix[MatIdx(0, 0)] + y * m_Matrix[MatIdx(1, 0)] + z * m_Matrix[MatIdx(2, 0)] + m_Matrix[MatIdx(3, 0)];
    ty = x * m_Matrix[MatIdx(0, 1)] + y * m_Matrix[MatIdx(1, 1)] + z * m_Matrix[MatIdx(2, 1)] + m_Matrix[MatIdx(3, 1)];
    tz = x * m_Matrix[MatIdx(0, 2)] + y * m_Matrix[MatIdx(1, 2)] + z * m_Matrix[MatIdx(2, 2)] + m_Matrix[MatIdx(3, 2)];
}

void moColorMatrix::ApplyChgMatrix()
{
	int i, j, k;

	for (j = 0; j < 4; j++)
		for (i = 0; i < 4; i++)
		{
			m_MatrixTemp[MatIdx(i, j)] = 0.0;
			for (k = 0; k < 4; k++)
				m_MatrixTemp[MatIdx(i, j)] += m_Matrix[MatIdx(i, k)]* m_MatrixChg[MatIdx(k, j)];
		}

	for (j = 0; j < 4; j++)
		for (i = 0; i < 4; i++)
			m_Matrix[MatIdx(i, j)] = m_MatrixTemp[MatIdx(i, j)];
}

moHueSatIntMatrix::moHueSatIntMatrix() : moColorMatrix()
{

	m_PreserveLuminance = true;
}

moHueSatIntMatrix::~moHueSatIntMatrix()
{
	Finish();
}

void moHueSatIntMatrix::Init(MOboolean p_PreserveLuminance,
							 MOfloat p_min_int, MOfloat p_max_int,
							 MOfloat p_min_sat, MOfloat p_max_sat,
							 MOfloat p_min_hue, MOfloat p_max_hue)
{
	moColorMatrix::Init();

	m_PreserveLuminance = p_PreserveLuminance;

	m_min_int = p_min_int;
	m_max_int = p_max_int;

	m_min_sat = p_min_sat;
	m_max_sat = p_max_sat;

	m_min_hue = p_min_hue;
	m_max_hue = p_max_hue;
}

void moHueSatIntMatrix::Finish()
{
	moColorMatrix::Finish();
}

void moHueSatIntMatrix::Update(MOfloat p_int, MOfloat p_sat, MOfloat p_hue)
{
	m_int = m_min_int + p_int * (m_max_int - m_min_int);
	m_sat = m_min_sat + p_sat * (m_max_sat - m_min_sat);
	m_hue = m_min_hue + p_hue * (m_max_hue - m_min_hue);

	m_int = momax(1e-3, m_int);

	IdentityMatrix();
	IntensityMatrix();
	SaturateMatrix();
	if (m_PreserveLuminance) HueRotateMatrix();
	else SimpleHueRotateMatrix();
}

void moHueSatIntMatrix::IntensityMatrix()
{
    m_MatrixChg[MatIdx(0, 0)] = m_int;
    m_MatrixChg[MatIdx(0, 1)] = 0.0;
    m_MatrixChg[MatIdx(0, 2)] = 0.0;
    m_MatrixChg[MatIdx(0, 3)] = 0.0;

    m_MatrixChg[MatIdx(1, 0)] = 0.0;
    m_MatrixChg[MatIdx(1, 1)] = m_int;
    m_MatrixChg[MatIdx(1, 2)] = 0.0;
    m_MatrixChg[MatIdx(1, 3)] = 0.0;

    m_MatrixChg[MatIdx(2, 0)] = 0.0;
    m_MatrixChg[MatIdx(2, 1)] = 0.0;
    m_MatrixChg[MatIdx(2, 2)] = m_int;
    m_MatrixChg[MatIdx(2, 3)] = 0.0;

    m_MatrixChg[MatIdx(3, 0)] = 0.0;
    m_MatrixChg[MatIdx(3, 1)] = 0.0;
    m_MatrixChg[MatIdx(3, 2)] = 0.0;
    m_MatrixChg[MatIdx(3, 3)] = 1.0;

	ApplyChgMatrix();
}

void moHueSatIntMatrix::SaturateMatrix()
{
    MOfloat a, b, c, d, e, f, g, h, i;
	MOfloat rs = (1.0 - m_sat);

    a = (rs * RLUM + m_sat);
    b = (rs * RLUM);
    c = (rs * RLUM);
    d = (rs * GLUM);
    e = (rs * GLUM + m_sat);
    f = (rs * GLUM);
    g = (rs * BLUM);
    h = (rs * BLUM);
    i = (rs * BLUM + m_sat);

    m_MatrixChg[MatIdx(0, 0)] = a;
    m_MatrixChg[MatIdx(0, 1)] = b;
    m_MatrixChg[MatIdx(0, 2)] = c;
    m_MatrixChg[MatIdx(0, 3)] = 0.0;

    m_MatrixChg[MatIdx(1, 0)] = d;
    m_MatrixChg[MatIdx(1, 1)] = e;
    m_MatrixChg[MatIdx(1, 2)] = f;
    m_MatrixChg[MatIdx(1, 3)] = 0.0;

    m_MatrixChg[MatIdx(2, 0)] = g;
    m_MatrixChg[MatIdx(2, 1)] = h;
    m_MatrixChg[MatIdx(2, 2)] = i;
    m_MatrixChg[MatIdx(2, 3)] = 0.0;

    m_MatrixChg[MatIdx(3, 0)] = 0.0;
    m_MatrixChg[MatIdx(3, 1)] = 0.0;
    m_MatrixChg[MatIdx(3, 2)] = 0.0;
    m_MatrixChg[MatIdx(3, 3)] = 1.0;

    ApplyChgMatrix();
}

// Hue rotation that preserves luminance.
void moHueSatIntMatrix::HueRotateMatrix()
{
	float mag;
    float lx, ly, lz;
    float xrs, xrc;
    float yrs, yrc;
    float zrs, zrc;
    float zsx, zsy;

    // rotate the grey vector into positive Z.
    mag = sqrt(2.0);
    xrs = 1.0 / mag;
    xrc = 1.0 / mag;
    SetXRotateMatrix(xrs, xrc);
	ApplyChgMatrix();

    mag = sqrt(3.0);
    yrs = -1.0 / mag;
    yrc = sqrt(2.0) / mag;
    SetYRotateMatrix(yrs, yrc);
	ApplyChgMatrix();

    // shear the space to make the luminance plane horizontal.
    XFormPoint(lx, ly, lz);
    zsx = lx / lz;
    zsy = ly / lz;
	SetZShearMatrix(zsx, zsy);
	ApplyChgMatrix();

    // rotate the hue.
    zrs = sin(m_hue);
    zrc = cos(m_hue);
    SetZRotateMatrix(zrs, zrc);
	ApplyChgMatrix();

    // unshear the space to put the luminance plane back.
    SetZShearMatrix(-zsx, -zsy);
	ApplyChgMatrix();

    // rotate the grey vector back into place.
    SetYRotateMatrix(-yrs, yrc);
	ApplyChgMatrix();
    SetXRotateMatrix(-xrs,xrc);
	ApplyChgMatrix();
}

void moHueSatIntMatrix::SimpleHueRotateMatrix()
{
    MOfloat mag;
    MOfloat xrs, xrc;
    MOfloat yrs, yrc;
    MOfloat zrs, zrc;

    // rotate the grey vector into positive Z.
    mag = sqrt(2.0);
    xrs = 1.0/mag;
    xrc = 1.0/mag;
	SetXRotateMatrix(xrs, xrc);
	ApplyChgMatrix();

    mag = sqrt(3.0);
    yrs = -1.0/mag;
    yrc = sqrt(2.0)/mag;
    SetYRotateMatrix(yrs, yrc);
	ApplyChgMatrix();

    // rotate the hue
    zrs = sin(m_hue);
    zrc = cos(m_hue);
    SetZRotateMatrix(zrs, zrc);
	ApplyChgMatrix();

    // rotate the grey vector back into place.
    SetYRotateMatrix(-yrs, yrc);
	ApplyChgMatrix();
    SetXRotateMatrix(-xrs, xrc);
	ApplyChgMatrix();
}

moBrightContMatrix::moBrightContMatrix() : moColorMatrix()
{
}

moBrightContMatrix::~moBrightContMatrix()
{
	Finish();
}

void moBrightContMatrix::Init(MOfloat p_min_bright, MOfloat p_max_bright,
			                  MOfloat p_min_cont, MOfloat p_max_cont)
{
	moColorMatrix::Init();

	m_min_bright = p_min_bright;
	m_max_bright = p_max_bright;

	m_min_cont = p_min_cont;
	m_max_cont = p_max_cont;
}

void moBrightContMatrix::Finish()
{
	moColorMatrix::Finish();
}

void moBrightContMatrix::Update(MOfloat p_bright, MOfloat p_cont)
{
	m_bright = m_min_bright + p_bright * (m_max_bright - m_min_bright);
	m_cont = m_min_cont + p_cont * (m_max_cont - m_min_cont);

	IdentityMatrix();
	BrightnessMatrix();
	ContrastMatrix();
}

void moBrightContMatrix::BrightnessMatrix()
{
    m_MatrixChg[MatIdx(0, 0)] = 1.0;
    m_MatrixChg[MatIdx(0, 1)] = 0.0;
    m_MatrixChg[MatIdx(0, 2)] = 0.0;
    m_MatrixChg[MatIdx(0, 3)] = 0.0;

    m_MatrixChg[MatIdx(1, 0)] = 0.0;
    m_MatrixChg[MatIdx(1, 1)] = 1.0;
    m_MatrixChg[MatIdx(1, 2)] = 0.0;
    m_MatrixChg[MatIdx(1, 3)] = 0.0;

    m_MatrixChg[MatIdx(2, 0)] = 0.0;
    m_MatrixChg[MatIdx(2, 1)] = 0.0;
    m_MatrixChg[MatIdx(2, 2)] = 1.0;
    m_MatrixChg[MatIdx(2, 3)] = 0.0;

    m_MatrixChg[MatIdx(3, 0)] = m_bright;
    m_MatrixChg[MatIdx(3, 1)] = m_bright;
    m_MatrixChg[MatIdx(3, 2)] = m_bright;
    m_MatrixChg[MatIdx(3, 3)] = 1.0;

    ApplyChgMatrix();
}

void moBrightContMatrix::ContrastMatrix()
{
    m_MatrixChg[MatIdx(0, 0)] = m_cont;
    m_MatrixChg[MatIdx(0, 1)] = 0.0;
    m_MatrixChg[MatIdx(0, 2)] = 0.0;
    m_MatrixChg[MatIdx(0, 3)] = 0.0;

    m_MatrixChg[MatIdx(1, 0)] = 0.0;
    m_MatrixChg[MatIdx(1, 1)] = m_cont;
    m_MatrixChg[MatIdx(1, 2)] = 0.0;
    m_MatrixChg[MatIdx(1, 3)] = 0.0;

    m_MatrixChg[MatIdx(2, 0)] = 0.0;
    m_MatrixChg[MatIdx(2, 1)] = 0.0;
    m_MatrixChg[MatIdx(2, 2)] = m_cont;
    m_MatrixChg[MatIdx(2, 3)] = 0.0;

    m_MatrixChg[MatIdx(3, 0)] = (1.0 - m_cont) * 0.5;
    m_MatrixChg[MatIdx(3, 1)] = (1.0 - m_cont) * 0.5;
    m_MatrixChg[MatIdx(3, 2)] = (1.0 - m_cont) * 0.5;
    m_MatrixChg[MatIdx(3, 3)] = 1.0;

    ApplyChgMatrix();
}

//===========================================
//
//				moTextureFilter
//
//===========================================

MOboolean moTextureFilter::Init(moGLManager* p_glman, moRenderManager* p_renderman, moTextureArray &p_src_tex, moTextureArray &p_dest_tex, moShader *p_shader, moTextFilterParam *p_params)
{
	MOuint i;
	moText uname;

	m_glman = p_glman;
	m_renderman = p_renderman;

	m_use_screen_tex = false;
	for (i = 0; i < p_src_tex.Count(); i++)
	{
		m_src_tex.Add(p_src_tex[i]);
		if (p_src_tex[i]->GetName() == moText("screen_texture")) m_use_screen_tex = true;
	}
	for (i = 0; i < p_dest_tex.Count(); i++) {
	    m_dest_tex.Add(p_dest_tex[i]);
	}
	CheckDestTexAttachStatus();

	m_shader = p_shader;
	moShaderGLSL* pglsl = (moShaderGLSL*)m_shader;
	pglsl->PrintVertShaderLog();
	pglsl->PrintFragShaderLog();

	// Gettting uniform parameters.
	for (i = 0; i < m_src_tex.Count(); i++)
	{
		uname = moText("src_tex_unit") + (moText)IntToStr(i);
		m_src_tex_unit[i] = pglsl->GetUniformID(uname);
		uname = moText("src_tex_offset") + IntToStr(i);
		m_src_tex_offset[i] = pglsl->GetUniformID(uname);
	}

	uname = "tempo_angle";
	m_tempo_angle = pglsl->GetUniformID(uname);

	uname = "dest_tex_size";
	m_dest_tex_size = pglsl->GetUniformID(uname);

    uname = "fade_const";
    m_fade_const = pglsl->GetUniformID(uname);

    if (p_params == NULL) m_DefParams = new moTextFilterParam();
    else m_DefParams = p_params;

    m_DefParams->getParamIDs(pglsl);

	return true;
}

MOboolean moTextureFilter::Finish()
{
    if (m_DefParams != NULL)
    {
        delete m_DefParams;
        m_DefParams = NULL;
    }
	return true;
}

void moTextureFilter::Apply(MOuint p_i, MOfloat p_fade, moTextFilterParam *p_params)
{
	MOint w = m_dest_tex[0]->GetWidth();
	MOint h = m_dest_tex[0]->GetHeight();
	SetGLConf(w, h);
	moShaderGLSL* pglsl = (moShaderGLSL*)m_shader;

	if (m_use_screen_tex) m_renderman->SaveScreen();

	BindDestFBO();

	pglsl->StartShader();
	SetupShader(w, h, NULL, p_fade, p_params);

	BindSrcTex(p_i);
	pglsl->DrawGrid(w, h, m_src_tex.Count());
	UnbindSrcTex();

	pglsl->StopShader();

	UnbindDestFBO();

	RestoreGLConf();
}

void moTextureFilter::Apply(MOfloat p_cycle, MOfloat p_fade, moTextFilterParam *p_params)
{
	MOint w = m_dest_tex[0]->GetWidth();
	MOint h = m_dest_tex[0]->GetHeight();
	SetGLConf(w, h);
	moShaderGLSL* pglsl = (moShaderGLSL*)m_shader;

	if (m_use_screen_tex) m_renderman->SaveScreen();

	BindDestFBO();

	pglsl->StartShader();
	SetupShader(w, h, NULL, p_fade, p_params);

	BindSrcTex(p_cycle);
	pglsl->DrawGrid(w, h, m_src_tex.Count());
	UnbindSrcTex();

	pglsl->StopShader();

	UnbindDestFBO();

	RestoreGLConf();
}

void moTextureFilter::Apply(moTempo *p_tempo, MOfloat p_fade, moTextFilterParam *p_params)
{
	MOint w = m_dest_tex[0]->GetWidth();
	MOint h = m_dest_tex[0]->GetHeight();
	SetGLConf(w, h);
	moShaderGLSL* pglsl = (moShaderGLSL*)m_shader;

	if (m_use_screen_tex) m_renderman->SaveScreen();

	BindDestFBO();

	pglsl->StartShader();
	SetupShader(w, h, p_tempo, p_fade, p_params);

	BindSrcTex(p_tempo);
	pglsl->DrawGrid(w, h, m_src_tex.Count());
	UnbindSrcTex();

	pglsl->StopShader();

	UnbindDestFBO();

	RestoreGLConf();
}

void moTextureFilter::SetupShader(MOint w, MOint h, moTempo *p_tempo, MOfloat p_fade, moTextFilterParam *p_params)
{
	for (MOuint i = 0; i < m_src_tex.Count(); i++)
	{
		if (-1 < m_src_tex_unit[i])
			glUniform1iARB(m_src_tex_unit[i], i);

		if (-1 < m_src_tex_offset[i])
			if (m_glman->RectTexture(m_src_tex[i]->GetTexTarget())) glUniform2fARB(m_src_tex_offset[i], 1.0, 1.0);
			else glUniform2fARB(m_src_tex_offset[i], 1.0 / float(m_src_tex[i]->GetWidth()), 1.0 / float(m_src_tex[i]->GetHeight()));
	}

	if (-1 < m_tempo_angle)
		if (p_tempo != NULL)
		{
			float a = p_tempo->ang;
			float f = fmod(float(a), float(2.0 * moMathf::PI)) / (2.0 * moMathf::PI);
			glUniform2fARB(m_tempo_angle, a, f);
		}
		else glUniform2fARB(m_tempo_angle, 0.0, 0.0);

	if (-1 < m_dest_tex_size) glUniform2fARB(m_dest_tex_size, w, h);

	if (-1 < m_fade_const) glUniform1fARB(m_fade_const, p_fade);

    if (p_params != NULL)
    {
        m_DefParams->CopyDefParamIDs(p_params);
        p_params->setParamValues();
    }
    else m_DefParams->setParamValues();
}

void moTextureFilter::SetGLConf(MOint w, MOint h)
{
	m_blend_on = glIsEnabled(GL_BLEND);
	glGetIntegerv(GL_POLYGON_MODE, &m_poly_mode);

	glDisable(GL_BLEND);
    glPolygonMode(GL_FRONT, GL_FILL);

	m_glman->SaveView();
	m_glman->SetOrthographicView(w, h);
}

void moTextureFilter::RestoreGLConf()
{
	m_glman->RestoreView();

	if (m_blend_on) glEnable(GL_BLEND);
	glPolygonMode(GL_FRONT, m_poly_mode);
}

void moTextureFilter::BindDestFBO()
{
	moFBO* fbo = m_dest_tex[0]->GetFBO();
	if (fbo != NULL)
	{
		// Attach destination textures to attachements points of fbo (0, 1, 2, etc)...
		moTexture* ptex;

		fbo->Bind();

		if (m_reattach_dest_tex) fbo->ClearAttachements();

		GLsizei n = m_dest_tex.Count();
		int i, j;
		for (i = 0; i < n; i++)
		{
			ptex = m_dest_tex[i];
			if (m_reattach_dest_tex)
			{
				fbo->AttachTexture(ptex->GetWidth(), ptex->GetHeight(), ptex->GetTexParam(), ptex->GetGLId(), i);
				m_draw_buffers[i] = fbo->GetColorAttachPoint(i);
			}
			else
			{
				j = ptex->GetFBOAttachPoint();
				m_draw_buffers[i] = fbo->GetColorAttachPoint(j);
			}
		}

		glDrawBuffers(n, m_draw_buffers);
	}
}

void moTextureFilter::UnbindDestFBO()
{
	moFBO* fbo = m_dest_tex[0]->GetFBO();
	if (fbo != NULL) fbo->Unbind();
}

void moTextureFilter::BindSrcTex(MOuint p_i)
{
	for (MOuint i = 0; i < m_src_tex.Count(); i++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + i);
		glBindTexture(m_src_tex[i]->GetTexTarget(), m_src_tex.GetGLId(i, (GLint)p_i));
	}
}

void moTextureFilter::BindSrcTex(MOfloat p_cycle)
{
	for (MOuint i = 0; i < m_src_tex.Count(); i++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + i);
		glBindTexture(m_src_tex[i]->GetTexTarget(), m_src_tex.GetGLId(i, p_cycle));
	}
}

void moTextureFilter::BindSrcTex(moTempo *p_tempo)
{
	for (MOuint i = 0; i < m_src_tex.Count(); i++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + i);
		glBindTexture(m_src_tex[i]->GetTexTarget(), m_src_tex.GetGLId(i, p_tempo));
	}
}

void moTextureFilter::UnbindSrcTex()
{
	for (MOuint i = m_src_tex.Count(); 0 < i; i--)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + i - 1);
		glBindTexture(m_src_tex[i-1]->GetTexTarget(), 0);
	}
}

void moTextureFilter::CheckDestTexAttachStatus()
{
	m_reattach_dest_tex = false;
	for (MOuint i = 0; i < m_dest_tex.Count(); i++)
		if ((int)m_dest_tex[i]->GetFBOAttachPoint() == MO_UNDEFINED)
		{
			m_reattach_dest_tex = true;
			break;
		}
}
