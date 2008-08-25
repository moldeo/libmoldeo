#include "moTextFilterParam.h"

void moTextFilterParam::setParamValues()
	{
        if (-1 < m_par_flt1) glUniform1fARB(m_par_flt1, par_flt1);
        if (-1 < m_par_flt2) glUniform1fARB(m_par_flt2, par_flt2);
        if (-1 < m_par_flt3) glUniform1fARB(m_par_flt3, par_flt3);

        if ((-1 < m_par_mat2) && (par_mat2 != NULL)) glUniformMatrix2fvARB(m_par_mat2, 1, false, par_mat2);
        if ((-1 < m_par_mat3) && (par_mat3 != NULL)) glUniformMatrix3fvARB(m_par_mat3, 1, false, par_mat3);
        if ((-1 < m_par_mat4) && (par_mat4 != NULL)) glUniformMatrix4fvARB(m_par_mat4, 1, false, par_mat4);
    }
