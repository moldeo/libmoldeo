/*******************************************************************************

                                moTextureFilter.h

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

#ifndef __MO_TEXTURE_FILTER_H
#define __MO_TEXTURE_FILTER_H

#include "moTypes.h"
#include "moAbstract.h"
#include "moConfig.h"
#include "moParam.h"
#include "moMoldeoObject.h"

#include "moShader.h"
#include "moShaderGLSL.h"

class moGLManager;
class moRenderManager;
class moShaderManager;

#define RLUM    ((float) (0.3086))
#define GLUM    ((float) (0.6094))
#define BLUM    ((float) (0.0820))

#include "moTextFilterParam.h"

// Base class to implement color transformation matrices.
class LIBMOLDEO_API moColorMatrix
{
public:
	moColorMatrix();
	virtual ~moColorMatrix();

	virtual void Init();
	virtual void Finish();

	void Copy(moColorMatrix& p_mat);
	void Multiply(moColorMatrix& p_mat);
	void SetZero() { ZeroMatrix(); }
	void SetIndentity() { IdentityMatrix(); }

	MOfloat* GetMatrixPointer() { return m_Matrix; }
protected:
	MOfloat m_Matrix[16];
	MOfloat m_MatrixChg[16];
	MOfloat m_MatrixTemp[16];

	int MatIdx(int i, int j) { return j * 4 + i; }

	void ZeroMatrix();
	void IdentityMatrix();

	void SetXRotateMatrix(float rs, float rc);
	void SetYRotateMatrix(float rs, float rc);
	void SetZRotateMatrix(float rs, float rc);
    void SetZShearMatrix(float dx, float dy);
    void XFormPoint(float &tx, float &ty, float &tz);

	void ApplyChgMatrix();
};

// Class that generates a Hue/Staturation/Intensity matrix
// Based on the Hue/Saturation/Intensity Filter for VirtualDub
// written by Donald A. Graft
// Some useful explanations on Matrix Operations for Image Processing
// can be found here:
// http://www.graficaobscura.com/matrix/index.html
class LIBMOLDEO_API moHueSatIntMatrix : public moColorMatrix
{
public:
	moHueSatIntMatrix();
	~moHueSatIntMatrix();

	void Init(MOboolean p_PreserveLuminance = true,
			MOfloat p_min_int = 0.0, MOfloat p_max_int = 2.0,
			MOfloat p_min_sat = 0.0, MOfloat p_max_sat = 2.0,
			MOfloat p_min_hue = 0.0, MOfloat p_max_hue = 2 * moMathf::PI);
	void Finish();

	// Updates the matrix using the provided intensity, saturation and
	// hue values (all in [0, 1]).
	void Update(MOfloat p_int, MOfloat p_sat, MOfloat p_hue);
protected:
	MOboolean m_PreserveLuminance;
	MOfloat m_int, m_sat, m_hue;
	MOfloat m_min_int, m_max_int;
	MOfloat m_min_sat, m_max_sat;
	MOfloat m_min_hue, m_max_hue;

	void IntensityMatrix();
	void SaturateMatrix();
	void HueRotateMatrix();
	void SimpleHueRotateMatrix();
};

// Class that generates a Brightness/Contrast matrix.
class LIBMOLDEO_API moBrightContMatrix : public moColorMatrix
{
public:
	moBrightContMatrix();
	~moBrightContMatrix();

	void Init(MOfloat p_min_bright = 0.0, MOfloat p_max_bright = 2.0,
			  MOfloat p_min_cont = 0.0, MOfloat p_max_cont = 2.0);
	void Finish();

	void Update(MOfloat p_bright, MOfloat p_cont);
protected:
	MOfloat m_bright, m_cont;
	MOfloat m_min_bright, m_max_bright;
	MOfloat m_min_cont, m_max_cont;

	void BrightnessMatrix();
	void ContrastMatrix();
};

class moTextureIndex;

#include "moVideoManager.h"
/**
 * Esta clase define un filtro 2D para aplicar en texturas. Un filtro es básicamente
 * un programa de shader (escrito en CG o GLSL) con un conjunto de predefinido de parámetros
 * uniformes y una grilla 2D donde las texturas fuente son mapeadas. Los puntos de la grilla 2D
 * pueden ser modificados en el estadio de vértices del filtro, permitiendo distorsiones arbitrarias
 * en la forma de la texturas.
 */
class LIBMOLDEO_API moTextureFilter : public moAbstract
{
public:
    /**
     * El constructor por defecto de la clase.
     */
	moTextureFilter()
	{
		m_glman = NULL;
		m_renderman = NULL;
		m_shader = NULL;
		m_DefParams = NULL;
        m_TextureFilterLabelName = moText("");
	}
    /**
     * El destructor por defecto de la clase.
     */
	virtual ~moTextureFilter() { Finish(); }

    /**
     * Método de inicialización de la grilla.
     * @param p_glman puntero al administrador de OpenGL.
     * @param p_renderman puntero al administrador de render.
     * @param p_src_tex referencia a la lista de texturas de orígen.
     * @param p_dest_tex referencia a la lista de texturas de destino.
     * @param p_shader puntero al programa de shader que define el filtro.
     * @param p_params puntero al objeto de parámetros a utilizar en este filtro.
     * @return true si la operación fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(moGLManager* p_glman, moRenderManager* p_renderman, moTextureArray &p_src_tex, moTextureArray &p_dest_tex, moShader *p_shader, moTextFilterParam *p_params = NULL);
    /**
     * Método de finalización.
     * @return true si la operación fue exitosa, false en caso contrario.
     */
	virtual MOboolean Finish();

    /**
     * Aplica el filtro sobre las texturas de orígen y escribiendo el resutlado en las texturas de destino.
     * @param p_i cuadro a utilizar en las texturas de orígen (en el caso de que sean animadas).
     * @param p_fade constante de "fading" utilizada para mezclar el resultado del filtro.
     * @param p_params parámetros del filtro.
     */
	void Apply(MOuint p_i, MOfloat p_fade = 1.0, moTextFilterParam *p_params = NULL);
    /**
     * Aplica el filtro sobre las texturas de orígen y escribiendo el resutlado en las texturas de destino.
     * @param p_cycle fracción del ciclo entre 0.0 y 1.0 a utilizar en las texturas de orígen (en el caso de que sean animadas).
     * @param p_fade constante de "fading" utilizada para mezclar el resultado del filtro.
     * @param p_params parámetros del filtro.
     */
	void Apply(MOfloat p_cycle, MOfloat p_fade = 1.0, moTextFilterParam *p_params = NULL);
    /**
     * Aplica el filtro sobre las texturas de orígen y escribiendo el resutlado en las texturas de destino.
     * @param p_tempo tempo a utilizar en las texturas de orígen (en el caso de que sean animadas).
     * @param p_fade constante de "fading" utilizada para mezclar el resultado del filtro.
     * @param p_params parámetros del filtro.
     */
	void Apply(moTempo *p_tempo, MOfloat p_fade = 1.0, moTextFilterParam *p_params = NULL);


  void Apply( moMoldeoObject *p_src_mob = NULL, MOfloat p_fade = 1.0, moTextFilterParam *p_params = NULL );

    /**
     * Devuelve el puntero a la lista de texturas de orígen.
     * @return puntero a la lista de texturas de orígen.
     */
	moTextureIndex* GetSrcTex() { return &m_src_tex; }
    /**
     * Devuelve el puntero a la lista de texturas de destino.
     * @return puntero a la lista de texturas de destino.
     */
	moTextureIndex* GetDestTex() { return &m_dest_tex; }

	moShader* GetShader() { return m_shader; }


    /**
     * Verifica que todas las texturas de destino estén asociadas a lo búfers de color del mismo FBO. En caso
     * contrario, las mismas serán re-asociadas (utilizando el FBO de la primera textura de destino) en el
     * momento de aplicar el filtro.
     */
	void CheckDestTexAttachStatus();


    void      SetTextureFilterLabelName( moText p_text ) {
        m_TextureFilterLabelName = p_text;
    }
    moText      GetTextureFilterLabelName() {
        return m_TextureFilterLabelName;
    }

protected:
	moShader* m_shader;
	moRenderManager* m_renderman;
	moGLManager* m_glman;

	moText          m_TextureFilterLabelName;//must be unique

	// Shader parameters.
	GLint m_src_tex_unit[MO_MAX_TEXTURE_UNITS];
	GLint m_src_tex_offset[MO_MAX_TEXTURE_UNITS];
	GLint m_tempo_angle;
	GLint m_fade_const;
	GLint m_dest_tex_size;

	moTextFilterParam *m_DefParams;

	// Texture indices.
	moTextureIndex m_src_tex;
	moTextureIndex m_dest_tex;

	GLboolean m_blend_on;
	GLint m_poly_mode;
	GLenum m_draw_buffers[MO_MAX_COLOR_ATTACHMENTS_EXT];

	MOboolean m_use_screen_tex;
	MOboolean m_reattach_dest_tex;

	void SetupShader(MOint w, MOint h, moTempo *p_tempo, MOfloat p_fade, moTextFilterParam *p_params, moMoldeoObject* p_src_object = NULL);
	void SetGLConf(MOint w, MOint h);
	void RestoreGLConf();
	void BindDestFBO();
	void UnbindDestFBO();

	void BindSrcTex(MOuint p_i);
	void BindSrcTex(MOfloat p_cycle);
	void BindSrcTex(moTempo *p_tempo);
	void BindSrcTex( moMoldeoObject* p_mob );
	void UnbindSrcTex();
	void RenderTexQuad(MOint w, MOint h);
};

moDeclareExportedDynamicArray( moTextureFilter*, moTextureFilterArray)

#include "moVideoManager.h"
#include "moShaderManager.h"
#include "moTextureManager.h"
#include "moGLManager.h"
#include "moRenderManager.h"

#endif

