#ifndef _TEXTFILTERPARAM_H
#define _TEXTFILTERPARAM_H

#include "moShader.h"
#include "moShaderGLSL.h"

/**
 * Esta clase almacena los parámetros de un filtro. Hay 6 parámetros pre-definidos:
 * tres números flotsntes, y tres matrices (2x2, 3x3 and 4x4). Estos parámetros
 * deben corresponder a los parámetros uniformes definidos en el código del shader.
 */
class moTextFilterParam
{
public:
    /**
     * Constructor por defecto.
     */
    moTextFilterParam()
    {
        par_flt1 = par_flt2 = par_flt3 = 0.0;
        par_mat2 = new MOfloat[2 * 2];
        par_mat3 = new MOfloat[3 * 3];
        par_mat4 = new MOfloat[4 * 4];

        m_par_flt1 = m_par_flt2 = m_par_flt3 = -1;
        m_par_mat2 = m_par_mat3 = m_par_mat4 = -1;
    }

    /**
     * Destructor por defecto.
     */
    ~moTextFilterParam()
    {
        if (par_mat2 != NULL)
        {
            delete[] par_mat2;
            par_mat2 = NULL;
        }
        if (par_mat3 != NULL)
        {
            delete[] par_mat3;
            par_mat3 = NULL;
        }
        if (par_mat4 != NULL)
        {
            delete[] par_mat4;
            par_mat4 = NULL;
        }
    }

    /**
     * Obtiene los OpenGL IDs de los parámetros uniformes en el shader que corresponden a los parámetros
     * almacenados en esta clase.
     * @param p_glsl puntero al shader.
     */
	void getParamIDs(moShaderGLSL* p_glsl)
    {
        moText uname;

        uname = moText("par_flt1");
        m_par_flt1 = p_glsl->GetUniformID(uname);
        uname = moText("par_flt2");
        m_par_flt2 = p_glsl->GetUniformID(uname);
        uname = moText("par_flt3");
        m_par_flt3 = p_glsl->GetUniformID(uname);
        uname = moText("par_mat2");
        m_par_mat2 = p_glsl->GetUniformID(uname);
        uname = moText("par_mat3");
        m_par_mat3 = p_glsl->GetUniformID(uname);
        uname = moText("par_mat4");
        m_par_mat4 = p_glsl->GetUniformID(uname);
    }

    /**
     * Determina los valores de los parámetros uniformes.
     */
	void setParamValues();

    /**
     * Copia los IDs de los parámetros uniformes a params.
     * @param p_params puntero a los parámetros donde se copiarán los IDs.
     */
    void CopyDefParamIDs(moTextFilterParam *p_params)
    {
        if (p_params != NULL)
        {
            p_params->m_par_flt1 = m_par_flt1;
            p_params->m_par_flt2 = m_par_flt2;
            p_params->m_par_flt3 = m_par_flt3;

            p_params->par_mat2 = par_mat2;
            p_params->par_mat3 = par_mat3;
            p_params->par_mat4 = par_mat4;
        }
    }

	MOfloat par_flt1;
	MOfloat par_flt2;
	MOfloat par_flt3;

	MOfloat* par_mat2;
	MOfloat* par_mat3;
	MOfloat* par_mat4;
protected:
	GLint m_par_flt1, m_par_flt2, m_par_flt3;
	GLint m_par_mat2, m_par_mat3, m_par_mat4;
};

#endif
