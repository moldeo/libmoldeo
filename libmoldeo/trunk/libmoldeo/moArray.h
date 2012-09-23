/*******************************************************************************

                              moArray.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                    *
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

// *****************************************************************************
// * Purpose: implements methods of "template" class declared in               *
// *          DECLARE_OBJARRAY macro and which couldn't be implemented inline  *
// *          (because they need the full definition of type T in scope)       *
// *                                                                           *
// * Usage:   1) #include dynarray.h                                           *
// *          2) WX_DECLARE_OBJARRAY                                           *
// *          3) #include arrimpl.cpp                                          *
// *          4) WX_DEFINE_OBJARRAY                                            *
// *****************************************************************************

#ifndef __MO_ARRAY_H__

#include "moArrayH.h"
#include <stdarg.h>

// macro implements remaining (not inline) methods of template list
// (it's private to this file)
#undef  _moDefineDynamicArray
#define _moDefineDynamicArray(T, name)                                             \
\
name::name() {\
  array = NULL;\
  n = 0;\
}\
\
name::name(int N) {\
	\
	if (N>0) {\
		array = new T [N];\
		n = N;\
	} else {\
		n=0;\
		array = NULL;\
	}\
	\
}\
\
name::name(const name& src) {\
	\
	Copy( src);\
	\
}\
\
name& name::operator=(const name& src) {\
\
	\
	Copy( src);\
	return (*this);\
}\
\
name::~name() {\
	if ( array != NULL ) {\
		delete[] array;\
		array = NULL;\
		n = 0;\
	}\
}\
\
\
MOboolean name::Init( int N, T initvalue ) {\
		m_NULL = initvalue;\
\
		if (n>0) Empty();\
\
		if (N>0) {\
			array = new T [N];\
			n = N;\
			for( MOuint i=0; i<n; i++) array[i] = initvalue;\
		} else {\
			n=0;\
			array = NULL;\
		}\
		return (array!=NULL);\
}\
\
MOboolean name::Finish() {\
	Empty();\
	return true;\
}\
\
void name::Empty() {\
\
	n = 0;\
	if (array!=NULL) {\
		delete[] array;\
		array = NULL;\
	}\
\
}\
\
void name::Set(int x, const T &value) {\
\
	if ( 0<=x && x<(MOint)n && array!=NULL) array[x]=value;\
\
}\
\
void name::Insert(int x, const T &value) {\
\
	if ( 0<=x && x<(MOint)n && array!=NULL) {\
\
        MOuint i,j;\
        T* arrayaux;\
\
        arrayaux = new T [n+1];\
\
        for( i=0,j=0 ; i < (n+1) ; i++,j++ ) {		\
            ((int)i==x) ? arrayaux[j--] = value : arrayaux[i] = array[j];\
        }\
        arrayaux[n] = value;\
        n++;\
\
        if (array!=NULL) delete[] array;\
        array = arrayaux;\
	}\
\
}\
\
const T& name::Get(int x) const {\
\
    if ( 0<=x && x<(MOint)n && array!=NULL) {\
\
        return array[x];\
\
    } else return m_NULL;\
}\
\
T& name::GetRef(int x) {\
\
    if ( 0<=x && x<(MOint)n && array!=NULL) {\
\
        return array[x];\
\
    } else return m_NULL;\
}\
\
T& name::Item(int x) {\
\
    if ( 0<=x && x<(MOint)n && array!=NULL) {\
\
        return array[x];\
\
    } else return m_NULL;\
}\
\
T& name::operator [] (int x) {\
    if ( 0<=x && x<(MOint)n && array!=NULL) {\
\
        return array[x];\
\
    } else return m_NULL;\
}\
\
MOuint name::Count() const {\
	return n;\
}\
\
void name::Add( const T& value ) {\
\
	MOuint i;\
	T* arrayaux;\
\
	arrayaux = new T [n+1];\
\
    if (array!=NULL)\
        for( i=0 ; i < n ; i++ ) {		\
            arrayaux[i] = array[i];\
        }\
	arrayaux[n] = value;\
	n++;\
\
	if (array!=NULL) delete[] array;\
	array = arrayaux;\
}\
\
void name::Remove(int x) {\
\
	MOuint i,j;\
	T* arrayaux;\
	bool founded = false;\
\
	if ( (MOint)0<=x && x<(MOint)n && array!=NULL) {\
	\
		if ( n > 1 ) {\
			arrayaux = new T [n-1];\
            for( i=0, j=0; j < (n-1); i++, j++) {\
                if ( x == (MOint)i ) {\
                    i++;\
                }\
                arrayaux[j] = array[i];\
            }\
            n--;\
		} else {\
		    arrayaux = NULL;\
		    n = 0;\
		}\
\
		delete[] array;\
		array = arrayaux;\
	}\
}\
\
\
void name::Copy( const name &A) {\
	Empty();\
	for(MOuint i=0; i< A.n; i++) {\
		Add( A.array[i] );\
	}\
}\
\
void name::Copy( const name &A, int x0, int x1) {\
	Empty();\
	for(int i=x0; i<= x1; i++) {\
		Add( A.array[i] );\
	}\
}

// redefine the macro so that now it will generate the class implementation
// old value would provoke a compile-time error if this file is not included
#undef  moDefineDynamicArray
#define moDefineDynamicArray(name) _moDefineDynamicArray( _moObjArray##name, name)



#endif /* __MO_ARRAY_H__ */

