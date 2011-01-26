/*******************************************************************************

                               moArrayH.h

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
  Andrés Colubri

*******************************************************************************/

#ifndef __MO_ARRAYH_H__
#define __MO_ARRAYH_H__

#include "moTypes.h"

template <class T, int N>
class moArray {

private:
    T array[N];
    int n;

public:
	/*
    moArray();
    void Set(int x, T value);
    T Get(int x);
	*/
	moArray() {
	  n = N;
	}

	void Set(int x, T value) {
	  array[x]=value;
	}

	T Get(int x) {
	  return array[x];
	}

};

template <class T>
class moDynamicArray {

private:
    T *array;
    MOuint n;
	T m_NULL;

public:

	moDynamicArray() {
	  array = NULL;
	  n = 0;
	}

	moDynamicArray(const moDynamicArray &src) {
		*this= src ;
	}

	moDynamicArray &operator = (const moDynamicArray &src) {
		Empty();
		Init( src.n, src.m_NULL );
		for(MOuint i=0; i< src.n; i++) {
			array[i] = src.array[i];
		}
		return *this;
	}

	moDynamicArray(int N) {

		if (N>0) {
			array = new T [N];
			n = N;
		} else {
			n=0;
			array = NULL;
		}

	}


	MOboolean Init( int N, T initvalue ) {

		m_NULL = initvalue;

		if (n>0) Empty();

		if (N>0) {
			array = new T [N];
			n = N;
			for( MOuint i=0; i<n; i++) array[i] = initvalue;
		} else {
			n=0;
			array = NULL;
		}
		return (array!=NULL);
	}

	MOboolean Finish() {
		Empty();
		return true;
	}

	virtual ~moDynamicArray() {
		if ( array != NULL ) {
			delete[] array;
			array = NULL;
			n = 0;
		}
	}

	void Empty() {

		n = 0;
		if (array!=NULL) {
			delete[] array;
			array = NULL;
		}

	}

	void Set(int x, const T &value) {

		if ( 0<=x && x<(MOint)n && array!=NULL) array[x]=value;

	}

	T& Get(int x) {

		if ( 0<=x && x<(MOint)n && array!=NULL) {

			return array[x];

		} else return m_NULL;
	}

	T& operator [] (int x) {
		if ( 0<=x && x<(MOint)n && array!=NULL) {

			return array[x];

		} else return m_NULL;
	}

	MOuint Count() {
		return n;
	}

	void Add( const T &value ) {

		MOuint i;
		T* arrayaux;

		arrayaux = new T [n+1];

		for( i=0 ; i < n ; i++ ) {
			arrayaux[i] = array[i];
		}
		if (array!=NULL) {
				delete[] array;
		}
		arrayaux[n] = value;
		n++;
		array = arrayaux;
	}

	void Remove(int x) {

		MOuint i,j;
		T* arrayaux;

		if ( 0<=x && x<(MOint)n && array!=NULL) {

			if ( n > 1 ) {
				arrayaux = new T [n-1];
			} else arrayaux = NULL;

			for( i=0, j=0; j < (n-1); i++, j++) {
				if ( x == (int)i ) i++;
				arrayaux[j] = array[i];
			}
			n--;

			delete[] array;
			array = arrayaux;
		}
	}

	void Copy( moDynamicArray &A) {
		Copy(A, 0, A.Count()-1);
	}

	void Copy( moDynamicArray &A, int x0, int x1) {
		Empty();
		T cpy;
		for(int i=x0; i<= x1; i++) {
			cpy = A.Get(i);
			Add( cpy );
		}
	}

};

/*
Ejemplo:

Header:
#include "moArray.h"
class MyClass;
moDeclareDynamicArray( MyClass, ArrayOfMyClass);

Source:
#include "moArray.cpp" // this is a magic incantation which must be done!
moDefineDynamicArray( ArrayOfMyClass );
*/

#define _moDeclareDynamicArray( T, name, classexp)						 \
classexp name															 \
{                                                                        \
public:                                                                  \
    name();																 \
    name(const name& src);                                               \
	name(int N);														 \
    name& operator=(const name& src);                                    \
                                                                         \
    virtual ~name();                                                     \
                                                                         \
	MOboolean Init( int N, T initvalue );								 \
	MOboolean Finish();													 \
	void Empty();														 \
	void Set(int x, const T &value);									 \
	void Insert(int x, const T &value);									 \
	T& Get(int x);														 \
	T& operator[] (int x) { return Get(x); }						     \
	MOuint Count() const;														 \
	void Add( const T &value );											 \
	void Remove(int x);													 \
	void Copy( const name &A);											 \
	void Copy( const name &A, int x0, int x1);							 \
																		 \
private:																 \
    T *array;															 \
    MOuint n;																 \
	T m_NULL;															 \
																		 \
};



#define moDeclareDynamicArray(T, name)									\
    moDeclareDynamicArrayDecl(T, name, class )

#define moDeclareExportedDynamicArray(T, name)							\
    moDeclareUserExportedDynamicArray(T, name, LIBMOLDEO_API)


#define moDeclareDynamicArrayDecl(T, name, decl)						\
    typedef T _moObjArray##name;										\
    _moDeclareDynamicArray(_moObjArray##name, name, decl)

#define moDeclareUserExportedDynamicArray(T, name, expmode)				\
    moDeclareDynamicArrayDecl(T, name, class expmode)

// moDefineDynamicArray is going to be redefined when arrimpl.cpp is included,
// try to provoke a human-understandable error if it used incorrectly.
//
// there is no real need for 3 different macros in the DEFINE case but do it
// anyhow for consistency
#define moDefineDynamicArray(name) DidYouIncludeArrayCpp
#define moDefineExportedDynamicArray(name)   moDefineDynamicArray(name)
#define moDefineUserExportedDynamicArray(name)   moDefineDynamicArray(name)

#endif /* __MO_ARRAYH_H__ */
