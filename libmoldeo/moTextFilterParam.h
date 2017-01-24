#ifndef _TEXTFILTERPARAM_H
#define _TEXTFILTERPARAM_H

#include "moTypes.h"


/**
 * Esta clase almacena los par�metros de un filtro. Hay 6 par�metros pre-definidos:
 * tres n�meros flotsntes, y tres matrices (2x2, 3x3 and 4x4). Estos par�metros
 * deben corresponder a los par�metros uniformes definidos en el c�digo del shader.
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
        for(int i=0; i<4; i++) par_mat2[i]=0.0;
        for(int i=0; i<9; i++) par_mat3[i]=0.0;
        for(int i=0; i<16; i++) par_mat4[i]=0.0;

        m_par_flt1 = m_par_flt2 = m_par_flt3 = -1;
        m_par_mat2 = m_par_mat3 = m_par_mat4 = -1;
    }

    /**
     * Destructor por defecto.
     */
    virtual ~moTextFilterParam()
    {
    }
    
    moTextFilterParam(const moTextFilterParam& p_src){
        (*this) = p_src;
    }
    
    const moTextFilterParam& operator=(const moTextFilterParam& p_src ) {
        m_par_flt1 = p_src.m_par_flt1;
        m_par_flt2 = p_src.m_par_flt2;
        m_par_flt3 = p_src.m_par_flt3;
        m_par_mat2 = p_src.m_par_mat2;
        m_par_mat3 = p_src.m_par_mat3;
        m_par_mat4 = p_src.m_par_mat4;
        
        par_flt1 = p_src.par_flt1;
        par_flt2 = p_src.par_flt2;
        par_flt3 = p_src.par_flt3;        
        for(int i=0; i<4; i++) par_mat2[i] = p_src.par_mat2[i];
        for(int i=0; i<9; i++) par_mat3[i] = p_src.par_mat3[i];
        for(int i=0; i<16; i++) par_mat4[i] = p_src.par_mat4[i];
        
        return (*this);
    }

    /**
     * Obtiene los OpenGL IDs de los par�metros uniformes en el shader que corresponden a los par�metros
     * almacenados en esta clase.
     * @param p_glsl puntero al shader.
     */
	void getParamIDs(void* p_shader);
    /**
     * Determina los valores de los par�metros uniformes.
     */
	void setParamValues();

    /**
     * Copia los IDs de los par�metros uniformes a params.
     * @param p_params puntero a los par�metros donde se copiar�n los IDs.
     */
    void CopyDefParamIDs(moTextFilterParam *p_params)
    {
        if (p_params != NULL)
        {
            p_params->m_par_flt1 = m_par_flt1;
            p_params->m_par_flt2 = m_par_flt2;
            p_params->m_par_flt3 = m_par_flt3;
            p_params->m_par_mat2 = m_par_mat2;
            p_params->m_par_mat3 = m_par_mat3;
            p_params->m_par_mat4 = m_par_mat4;


        }
    }

	MOfloat par_flt1;
	MOfloat par_flt2;
	MOfloat par_flt3;

	MOfloat par_mat2[4];
	MOfloat par_mat3[9];
	MOfloat par_mat4[16];

	GLint m_par_flt1, m_par_flt2, m_par_flt3;
	GLint m_par_mat2, m_par_mat3, m_par_mat4;
};

#endif

