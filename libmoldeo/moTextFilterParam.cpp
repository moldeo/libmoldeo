#include "moTextFilterParam.h"
#include "moShader.h"
#include "moShaderCG.h"
#include "moShaderGLSL.h"

void moTextFilterParam::getParamIDs(void* p_shader)
{
    moShaderGLSL* p_glsl = (moShaderGLSL*)p_shader;
    moText uname;
    if (p_glsl) {
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
}


void moTextFilterParam::setParamValues()
	{
        if (-1 < m_par_flt1) glUniform1fARB(m_par_flt1, par_flt1);
        if (-1 < m_par_flt2) glUniform1fARB(m_par_flt2, par_flt2);
        if (-1 < m_par_flt3) glUniform1fARB(m_par_flt3, par_flt3);

        if ((-1 < m_par_mat2) ) glUniformMatrix2fvARB(m_par_mat2, 1, false, &par_mat2[0]);
        if ((-1 < m_par_mat3) ) glUniformMatrix3fvARB(m_par_mat3, 1, false, &par_mat3[0]);
        if ((-1 < m_par_mat4) ) glUniformMatrix4fvARB(m_par_mat4, 1, false, &par_mat4[0]);
    }

