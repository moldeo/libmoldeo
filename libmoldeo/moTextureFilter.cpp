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


*******************************************************************************/

#include "moTextureFilter.h"
#include "moEffect.h"
#include "moArray.h"
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


moTextureFilter::moTextureFilter()	{
		m_glman = NULL;
		m_renderman = NULL;
		m_shader = NULL;
		m_DefParams = NULL;
    m_TextureFilterLabelName = "";
    m_uniform_idx = -1;
}


MOboolean moTextureFilter::Init(moGLManager* p_glman, moRenderManager* p_renderman, moTextureArray &p_src_tex, moTextureArray &p_dest_tex, moShader *p_shader, moTextFilterParam *p_params)
{
	MOuint i;
	moText uname;

	m_glman = p_glman;
	m_renderman = p_renderman;

  if (p_shader) {

  }

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
	moShaderGLSL* pglsl = NULL;

#ifdef SHADER_CG
    moShaderCG* pcg = NULL;
#endif

	if (m_shader->GetType()==MO_SHADER_CG) {
#ifdef SHADER_CG
        pcg = (moShaderCG*)m_shader;
#endif
	} else {
        pglsl = (moShaderGLSL*)m_shader;
	}


    if (pglsl) {
        pglsl->PrintVertShaderLog();
        pglsl->PrintFragShaderLog();
    }

	// Gettting uniform parameters.
	for (i = 0; i < m_src_tex.Count(); i++)
	{
		uname = moText("src_tex_unit") + IntToStr(i);

		if (pglsl) m_src_tex_unit[i] = pglsl->GetUniformID(uname);
		//MODebug2->Message("moTextureFilter::Init >"+uname+" val:"+IntToStr( m_src_tex_unit[i] ) );
#ifdef SHADER_CG
		if (pcg) {
            CGparameter pres =  pcg->GetFragParameter(uname);
            if (pres==NULL) m_src_tex_unit[i]  = -1;
            else m_src_tex_unit[i]  = 0/*(int)pres*/;
            MODebug2->Push("pcg - m_src_tex_unit[i]:" + IntToStr( m_src_tex_unit[i] ) );
		}
		//if (pcg) m_src_tex_unit[i] = -1;
#endif

		uname = moText("src_tex_offset") + IntToStr(i);

		if (pglsl) m_src_tex_offset[i] = pglsl->GetUniformID(uname);
		//MODebug2->Message("moTextureFilter::Init >"+uname+" val:"+IntToStr( m_src_tex_offset[i] ) );
#ifdef SHADER_CG
		if (pcg) {
		    CGparameter pres = pcg->GetFragParameter(uname);
            if (pres==NULL) m_src_tex_offset[i]  = -1;
            else m_src_tex_offset[i] = 0/*(int)pres*/;
            MODebug2->Push("pcg - m_src_tex_offset[i]:" + IntToStr( m_src_tex_offset[i] ) );
		}
#endif
	}

	uname = moText("tempo_angle");
	if (pglsl) m_tempo_angle = pglsl->GetUniformID(uname);

	uname = moText("tempo_dt");
	if (pglsl) m_tempo_dt = pglsl->GetUniformID(uname);

	uname = moText("tempo_delta");
	if (pglsl) m_tempo_delta = pglsl->GetUniformID(uname);

	uname = moText("tempo_syncro");
	if (pglsl) m_tempo_syncro = pglsl->GetUniformID(uname);

	uname = moText("tempo_ticks");
	if (pglsl) m_tempo_ticks = pglsl->GetUniformID(uname);
	;

	//MODebug2->Message( "moTextureFilter::Init >"+uname+" val:"+IntToStr( m_tempo_angle ) );
#ifdef SHADER_CG
    //if (pcg) m_tempo_angle = (int)pcg->GetFragParameter(uname);
    if (pcg) {
        CGparameter pres = pcg->GetFragParameter(uname);
        if (pres==NULL) m_tempo_angle  = -1;
        else m_tempo_angle = 0/*(int)pres*/;
    }
#endif

	uname = moText("dest_tex_size");
	if (pglsl) m_dest_tex_size = pglsl->GetUniformID(uname);
	//MODebug2->Message( "moTextureFilter::Init >"+uname+" val:"+IntToStr( m_dest_tex_size ) );

#ifdef SHADER_CG
    //if (pcg) m_dest_tex_size = (int)pcg->GetFragParameter(uname);
    if (pcg) {
        CGparameter pres = pcg->GetFragParameter(uname);
        if (pres==NULL) m_dest_tex_size  = -1;
        else m_dest_tex_size = 0/*(int)pres*/;
        MODebug2->Push("pcg - m_dest_tex_size:" + IntToStr( m_dest_tex_size ) );
    }
#endif

    uname = moText("fade_const");
	if (pglsl) m_fade_const  = pglsl->GetUniformID(uname);
	//MODebug2->Message("moTextureFilter::Init >"+uname+" val:"+IntToStr(m_fade_const));
#ifdef SHADER_CG
    if (pcg) {
        CGparameter pres = pcg->GetFragParameter(uname);
        if (pres==NULL) m_fade_const  = -1;
        else m_fade_const = 0/*(int)pres*/;
        MODebug2->Push("pcg - m_fade_const:" + IntToStr( m_fade_const ) );
    }
    //if (pcg)
    //pcg->GetFragParameter(uname)==NULL )
#endif

    uname = moText("random_uniform");
    if (pglsl) m_random_uniform = pglsl->GetUniformID(uname);
    //MODebug2->Message("moTextureFilter::Init >"+uname+" idx:"+IntToStr(m_random_uniform));

  m_uniform_idx = -1;
  for(int u=0; u<MAX_UNIFORM_VARS; u++ ) {
    m_uniform_variables_idx[u] = -1;
  }

  if (m_shader) {
    //do something
    //p_src_object = NULL;
    if (m_uniform_idx==-1) {
      m_uniform_idx = m_shader->m_Config.GetParamIndex( "uniform" );
      //MODebug2->Message("moTextureFilter::Init > founded uniform param in: m_uniform_idx:"+IntToStr(m_uniform_idx));
    }

    if (m_uniform_idx>-1) {
        for( int u=0; u<m_shader->m_Config.GetParam(m_uniform_idx).GetValuesCount(); u++ ) {
          moValue vb = m_shader->m_Config.GetParam(m_uniform_idx).GetValues().Get(u);
          moText uniform_var_name = vb.GetSubValue(0).GetData()->Text();
          moText uniform_var_type = vb.GetSubValue(1).GetData()->Text();
          uname = uniform_var_name;
          if (m_uniform_variables_idx[u]==-1 && pglsl) {
            m_uniform_variables_idx[u] = pglsl->GetUniformID(uname);
            //MODebug2->Message("moTextureFilter::Init > setting uniform id for ("+uname+","+uniform_var_type+") in:"+IntToStr(m_uniform_variables_idx[u]));
          }
        }
    }
  }



    if (p_params == NULL) m_DefParams = new moTextFilterParam();
    else m_DefParams = p_params;

    if (pglsl) {
        m_DefParams->getParamIDs(pglsl);
    }
#ifdef SHADER_CG
    else if (pcg) {
        m_DefParams->getParamIDs(pcg);
    }
#endif



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

void moTextureFilter::Apply( MOuint p_i, MOfloat p_fade, moTextFilterParam *p_params)
{
	MOint w = m_dest_tex[0]->GetWidth();
	MOint h = m_dest_tex[0]->GetHeight();
	SetGLConf(w, h);
//	moShaderGLSL* pglsl = (moShaderGLSL*)m_shader;

	if (m_use_screen_tex) m_renderman->SaveScreen();

	BindDestFBO();

	m_shader->StartShader();
	SetupShader(w, h, NULL, p_fade, p_params);

	BindSrcTex(p_i);
	m_shader->DrawGrid(w, h, m_src_tex.Count());
	UnbindSrcTex();

	m_shader->StopShader();

	UnbindDestFBO();

	RestoreGLConf();
}

void moTextureFilter::Apply(MOfloat p_cycle, MOfloat p_fade, moTextFilterParam *p_params)
{
	MOint w = m_dest_tex[0]->GetWidth();
	MOint h = m_dest_tex[0]->GetHeight();
	SetGLConf(w, h);
//	moShaderGLSL* pglsl = (moShaderGLSL*)m_shader;


	if (m_use_screen_tex) m_renderman->SaveScreen();

	BindDestFBO();

	m_shader->StartShader();
	SetupShader(w, h, NULL, p_fade, p_params);

	BindSrcTex(p_cycle);
	m_shader->DrawGrid(w, h, m_src_tex.Count());
	UnbindSrcTex();

	m_shader->StopShader();

	UnbindDestFBO();

	RestoreGLConf();
}

void moTextureFilter::Apply(moTempo *p_tempo, MOfloat p_fade, moTextFilterParam *p_params)
{
	MOint w = m_dest_tex[0]->GetWidth();
	MOint h = m_dest_tex[0]->GetHeight();
	SetGLConf(w, h);
//	moShaderGLSL* pglsl = (moShaderGLSL*)m_shader;

	if (m_use_screen_tex) m_renderman->SaveScreen();

	BindDestFBO();

	m_shader->StartShader();
	SetupShader(w, h, p_tempo, p_fade, p_params);

	BindSrcTex(p_tempo);
	m_shader->DrawGrid(w, h, m_src_tex.Count());
	UnbindSrcTex();

	m_shader->StopShader();

	UnbindDestFBO();

	RestoreGLConf();
}

void moTextureFilter::Apply( moMoldeoObject *p_src_mob, moTempo* p_tempo, MOfloat p_fade, moTextFilterParam *p_params ) {

  if (p_src_mob==NULL) return;
  if (m_shader==NULL) return;

  MOint w = m_dest_tex[0]->GetWidth();
	MOint h = m_dest_tex[0]->GetHeight();
	SetGLConf(w, h);

  if (m_use_screen_tex) m_renderman->SaveScreen();

	BindDestFBO();

	m_shader->StartShader();
	SetupShader(w, h, p_tempo, p_fade, p_params, p_src_mob);

	BindSrcTex( p_src_mob );
	m_shader->DrawGrid(w, h, m_src_tex.Count());
	UnbindSrcTex();

	m_shader->StopShader();

	UnbindDestFBO();

	RestoreGLConf();

}


void moTextureFilter::SetupShader(MOint w, MOint h, moTempo *p_tempo, MOfloat p_fade, moTextFilterParam *p_params, moMoldeoObject* p_src_object)
{
	for (MOuint i = 0; i < m_src_tex.Count(); i++)
	{
		if (-1 < m_src_tex_unit[i]) {
		    if ( m_shader && m_shader->GetType() == (MOuint)MO_SHADER_GLSL ) {
                glUniform1iARB(m_src_tex_unit[i], i);
		    } else if ( m_shader && m_shader->GetType() == (MOuint)MO_SHADER_CG ) {
                //cgGLSetTextureParameter( (CGparameter)m_src_tex_unit[i], i );
                //cgGLSetTextureParameter( (CGparameter)m_src_tex_unit[i], m_src_tex.GetGLId(i, (GLint)i) );
                //cgGLEnableTextureParameter( (CGparameter)m_src_tex_unit[i] );
		    }
		}
		if (-1 < m_src_tex_offset[i]) {
			if (m_glman->RectTexture(m_src_tex[i]->GetTexTarget())) glUniform2fARB(m_src_tex_offset[i], 1.0, 1.0);
			else glUniform2fARB(m_src_tex_offset[i], 1.0 / float(m_src_tex[i]->GetWidth()), 1.0 / float(m_src_tex[i]->GetHeight()));
		}
	}


  if (p_tempo != NULL)
  {

   if (-1 < m_tempo_angle) {
    float a = p_tempo->ang;
    float f = fmod(float(a), float(2.0 * moMathf::PI)) / (2.0 * moMathf::PI);

    glUniform2fARB(m_tempo_angle, a, f);
   }

   if (-1<m_tempo_dt) {
    glUniform1fARB(m_tempo_dt, p_tempo->dt );
   }
   if (-1<m_tempo_syncro) {
    glUniform1fARB(m_tempo_syncro, p_tempo->syncro );
   }
   if (-1<m_tempo_delta) {
    glUniform1fARB(m_tempo_delta, p_tempo->delta );
   }
   if (-1<m_tempo_ticks) {
    glUniform1fARB(m_tempo_ticks, (float)p_tempo->ticks );
   }
    //moDebugManager::Push("m_tempo_angle:" + FloatToStr(a));
    //(m_shader->GetType() == (MOuint)MO_SHADER_GLSL) ? glUniform2fARB(m_tempo_angle, a, f) : m_tempo_angle=0;/*cgGLSetParameter2f( (CGparameter)m_tempo_angle, a, f)*/
  }
  else {
      glUniform2fARB(m_tempo_angle, 0.0, 0.0);
      glUniform1fARB(m_tempo_dt, 0.0);
      //(m_shader->GetType() == (MOuint)MO_SHADER_GLSL) ? glUniform2fARB(m_tempo_angle, 0.0, 0.0) : m_tempo_angle=0;/*cgGLSetParameter2f( (CGparameter)m_tempo_angle, 0.0, 0.0 )*/
  }


	if (-1 < m_dest_tex_size) {
	    glUniform2fARB(m_dest_tex_size, w, h);
	   //(m_shader->GetType() == (MOuint)MO_SHADER_GLSL) ? glUniform2fARB(m_dest_tex_size, w, h) : m_dest_tex_size=0;/*cgGLSetParameter2f( (CGparameter)m_dest_tex_size, w, h )*/
	}

	if (-1 < m_fade_const) {
        glUniform1fARB(m_fade_const, p_fade);
	    //(m_shader->GetType() == (MOuint)MO_SHADER_GLSL) ? glUniform1fARB(m_fade_const, p_fade) : m_fade_const=0;/*cgGLSetParameter1f( (CGparameter)m_fade_const, p_fade )*/
	}

	if (-1 < m_random_uniform) {
			float seed = 0.0f;
			float r = moMathf::UnitRandom(seed);

			glUniform2fARB(m_random_uniform, r, seed);
	}


	///TODO: load and evaluate each parameter of m_shader->config using also p_src_object as parameters!!!
  if (p_src_object && m_shader) {

    moShaderGLSL* pglsl = (moShaderGLSL*)m_shader;
    //do something
    //p_src_object = NULL;
    if (m_uniform_idx>-1) {
        for( int u=0; u<m_shader->m_Config.GetParam(m_uniform_idx).GetValuesCount(); u++ ) {
          moValue vb = m_shader->m_Config.GetParam(m_uniform_idx).GetValues().Get(u);
          moText uniform_var_name = vb.GetSubValue(0).GetData()->Text();
          moText uniform_var_type = vb.GetSubValue(1).GetData()->Text();

          if (m_uniform_variables_idx[u]>-1) {
            int paramidx = p_src_object->GetConfig()->GetParamIndex(uniform_var_name);
            if (uniform_var_type=="FLOAT") {
              if (paramidx>-1) {
                float p_float = p_src_object->GetConfig()->Eval( uniform_var_name  );
                glUniform1fARB( m_uniform_variables_idx[u], p_float );
              } else {
                int inletidx = p_src_object->GetInletIndex(uniform_var_name);
                if (inletidx>-1) {
                  float p_float = p_src_object->GetInlets()->Get(inletidx)->GetData()->Float();
                  glUniform1fARB( m_uniform_variables_idx[u], p_float );
                }
              }
            } else
            if (uniform_var_type=="INT") {
              if (paramidx>-1) {
                int p_int = p_src_object->GetConfig()->Int( uniform_var_name );
                glUniform1iARB( m_uniform_variables_idx[u], p_int );
              } else {
                int inletidx = p_src_object->GetInletIndex(uniform_var_name);
                if (inletidx>-1) {
                  int p_int = p_src_object->GetInlets()->Get(inletidx)->GetData()->Int();
                  glUniform1iARB( m_uniform_variables_idx[u], p_int );
                }
              }
            }
          /**
            if (uniform_var_type=="FLOAT") {
              float p_float = 0.0;
              int paramidx = p_src_object->GetConfig()->GetParamIndex(uniform_var_name);
              if (paramidx>-1) {
                p_float = p_src_object->GetConfig()->Eval( moParamReference(paramidx)  );
              } else {
                int inletidx = p_src_object->GetInletIndex(uniform_var_name);
                if (inletidx>-1) {
                  p_float = p_src_object->GetInlets()->Get(inletidx)->GetData()->Float();
                }
              }
              glUniform1fARB( m_uniform_variables_idx[u], p_float );
              //MODebug2->Message("Assigning FLOAT:" + FloatToStr(p_float));
            } else
            if (uniform_var_type=="INT") {
              float p_int = 0;
              int paramidx = p_src_object->GetConfig()->GetParamIndex(uniform_var_name);
              if (paramidx>-1) {
                p_int = p_src_object->GetConfig()->Int( moParamReference(paramidx)  );
              } else {
                int inletidx = p_src_object->GetInletIndex(uniform_var_name);
                if (inletidx>-1) {
                  p_int = p_src_object->GetInlets()->Get(inletidx)->GetData()->Int();
                }
              }
              glUniform1iARB( m_uniform_variables_idx[u], p_int );
              //MODebug2->Message("Assigning FLOAT:" + FloatToStr(p_float));
            }
            */
          }
        }



    }
  }



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
#ifndef OPENGLESV2
	glGetIntegerv(GL_POLYGON_MODE, &m_poly_mode);
#endif
	glDisable(GL_BLEND);
#ifndef OPENGLESV2
    glPolygonMode(GL_FRONT, GL_FILL);
#endif
	m_glman->SaveView();
	m_glman->SetOrthographicView(w, h, 0.0, w, 0.0, h);
}

void moTextureFilter::RestoreGLConf()
{
	m_glman->RestoreView();

	if (m_blend_on) glEnable(GL_BLEND);
#ifndef OPENGLESV2
	glPolygonMode(GL_FRONT, m_poly_mode);
#endif
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
#ifndef OPENGLESV2
		glDrawBuffers(n, m_draw_buffers);
#endif
	}
}

void moTextureFilter::UnbindDestFBO()
{
	moFBO* fbo = m_dest_tex[0]->GetFBO();
	if (fbo != NULL) fbo->Unbind();
}

void moTextureFilter::BindSrcTex(MOuint p_i)
{
    //if (m_shader->GetType()==MO_SHADER_GLSL)
	for (MOuint i = 0; i < m_src_tex.Count(); i++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + i);
		glBindTexture(m_src_tex[i]->GetTexTarget(), m_src_tex.GetGLId(i, (GLint)p_i));
	}
}

void moTextureFilter::BindSrcTex(MOfloat p_cycle)
{
    //if (m_shader->GetType()==MO_SHADER_GLSL)
	for (MOuint i = 0; i < m_src_tex.Count(); i++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + i);
		glBindTexture(m_src_tex[i]->GetTexTarget(), m_src_tex.GetGLId(i, p_cycle));
	}
}

void moTextureFilter::BindSrcTex(moTempo *p_tempo)
{
	//if (m_shader->GetType()==MO_SHADER_GLSL)
	for (MOuint i = 0; i < m_src_tex.Count(); i++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + i);
		glBindTexture(m_src_tex[i]->GetTexTarget(), m_src_tex.GetGLId(i, p_tempo));
	}
}

void moTextureFilter::BindSrcTex( moMoldeoObject* p_mob ) {

  if (p_mob==NULL) return;

  moTempo* src_tempo  = NULL;
  moEffectState fs;

  if (  p_mob->GetType()==MO_OBJECT_EFFECT
      || p_mob->GetType()==MO_OBJECT_PREEFFECT
      || p_mob->GetType()==MO_OBJECT_POSTEFFECT
      || p_mob->GetType()==MO_OBJECT_MASTEREFFECT ) {

    moEffect* pFx = (moEffect*) p_mob;

    if (pFx) {
      fs = pFx->GetEffectState();
      src_tempo = &fs.tempo;
    }

  }


  for (MOuint i = 0; i < m_src_tex.Count(); i++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + i);
		//glBindTexture(m_src_tex[i]->GetTexTarget(), m_src_tex.GetGLId(i, p_mob));
		glBindTexture(m_src_tex[i]->GetTexTarget(), m_src_tex.GetGLId(i, src_tempo));
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

