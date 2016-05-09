/*******************************************************************************

                                moText.h

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
  Andres Colubri

*******************************************************************************/

#include "moTypes.h"
#include "moArray.h"

#ifndef __MO_TEXT_H
#define __MO_TEXT_H

#include <stdio.h>
#include <string.h>
#include <sstream>

#ifdef WIN32
#define snprintf _snprintf
#endif

//====================================================================================
//  class: moText
//  derivada de:
//  descripcion: clase original de manejo de strings en moldeo
//  author: Gustavo Lado
//  fecha: ?
//====================================================================================

#define MO_TXT_COMPLETE        65534
#define MO_TXT_NOT_FOUND   65534

enum txtcval    { MO_TXT_LESSER, MO_TXT_EQUAL, MO_TXT_GREATER };
enum txtpert    { MO_TXT_BELONG, MO_TXT_NOT_BELONG };

typedef char* CHARP;
typedef int MOINT;
typedef unsigned int MOUINT;

class moTextArray;

/// clase de para manejar textos
/**
 * Operaciones y funciones para la manipulación de cadenas de caracteres.
 */
#ifndef USE_MOTEXT0
#define USE_MOTEXT0 1
#endif
#ifdef USE_MOTEXT0

class LIBMOLDEO_API moText0
{
public:
	moText0();
	moText0(const moText0&);
	moText0( char*);
	moText0( const char*);
	moText0( wchar_t*);

	moText0( int );
	moText0( unsigned int );
	moText0( long );
	moText0( unsigned long );
	moText0( long long );
	moText0( unsigned long long );
	moText0( float );
	moText0( double );

	virtual ~moText0();

    MOuint Length() const;               // devuelve el length del text sin contar el null del final.

    // operadores que usan solo objetos de la clase //
    moText0& operator = ( const moText0& txt);
    moText0& operator +=( const moText0& txt);
    //friend moText0 operator + ( const moText0& txt, );

    int operator < ( const moText0& txt) const;
    int operator > ( const moText0& txt) const;
    int operator <=( const moText0& txt) const;
    int operator >=( const moText0& txt) const;
    int operator ==( const moText0& txt) const;
    int operator !=( const moText0& txt) const;

    // operadores que usan c-strings //
    moText0& operator = ( const char* txt);
    moText0& operator = ( const short* txt);
    moText0& operator +=( const char* txt);

    friend LIBMOLDEO_API moText0 operator +( const moText0& txt1, const moText0& txt2);
    friend LIBMOLDEO_API moText0 operator +( const moText0& txt1, const char* txt2);
    friend LIBMOLDEO_API moText0 operator +( const char* txt1,    const moText0& txt2);

    int operator < ( const char* txt) const;
    int operator > ( const char* txt) const;
    int operator <=( const char* txt) const;
    int operator >=( const char* txt) const;
    int operator ==( const char* txt) const;
    int operator !=( const char* txt) const;

    //char operator [](MOuint pos)            { return text[pos]; }

    // operadores de conversion para poder usar c-strings //
    virtual operator char*() const                { return text; }
    wchar_t*  Unicode() const {
        std::wstring wc( 1024, L'#' );
        mbstowcs( &wc[0], text, 1024 );
        return (wchar_t*)wc.c_str();
    }


    // metodos comunes en una clase string//
    moText0& Left( MOuint);            // devuelve el comienzo de un text.
    moText0& Right( MOuint);           // devuelve el final de un text.
    moText0& Mid( MOuint, MOuint);       // devuelve un fragmento intermedio del text.(comienzo, cant de carac)
    moText0& SubText( MOuint, MOuint);   // devuelve un fragmento intermedio del text.(comienzo, final)

    moText0& Insert( char*, MOuint);        // inserta un text dentro de otro.
    moText0& Delete( MOuint, MOuint);           // borra una parte del text.

    moText0  Scan( char*);                  // saca y devuelve el primer fragmento de text que este separado por alguno de los caracteres de un cjto.
	moText0  ScanEx( char*);                // saca y devuelve el primer fragmento de text que este separado por alguno de los caracteres de un cjto, y entre comillas.

    moText0& LTrim();                // saca los espacios del comienzo.
    moText0& RTrim();                // saca los espacios del final.
    moText0& Trim();                 // saca los espacios del comienzo y del final.

	unsigned short* Short();
    void ToUpper();                  // convierte el text a mayusculas.
    void ToLower();                  // convierte el text a minusculas.

    moTextArray     Explode( char* separator ) const; ///divide el texto separado por el caracter especificado
    int             Find( const moText0& target );
    void            ReplaceChar( const char* target, const char* replacement );
    void            Replace( const moText0& target, const moText0& replacement );

private:
    char*   text;
    MOuint    length;

    // Estos metodos son el motor de la clase. Contienen toda la logica
    // de como se maneja moText0. Todos los metodos publicos las usan.
public:
    void    txtcopy( const char* txt, MOuint pos = 0, MOuint com = 0, MOuint fin = MO_TXT_COMPLETE);        // copy c-strings(reserva la memoria y pone los nulls al final)
    void    txtcopy( const short* txt, MOuint pos = 0, MOuint com = 0, MOuint fin = MO_TXT_COMPLETE);        // copy c-strings(reserva la memoria y pone los nulls al final)
private:
    txtcval txtcomp( const char* txt, MOuint com1 = 0, MOuint com2 = 0) const;
    MOuint    txtfind( const char* txt, txtpert pert = MO_TXT_BELONG, MOuint com = 0, int dir = 1) const;
};
#endif

/*
//====================================================================================
//  class: moString
//  derivada de: string
//  descripcion: clase que agrega al string del STL las funciones del moText original.
//  author: Andres Colubri
//  fecha: 01/20/2007
//====================================================================================

class LIBMOLDEO_API moString : public std::string
{
public:
	// Constructors.
	moString() : std::string() {}
	moString(MOuint length, const char& ch) : std::string(length, ch) {}
	moString(const char* str) : std::string(str) {}
	moString(const char* str, MOuint length) : std::string(str, length) {}
	moString(const std::string& s) : std::string(s) {}
	moString(const moString& s) : std::string((std::string)s) {}
	moString(const std::string& str, MOuint index, MOuint length) : std::string(str, index, length) {}
	moString(const moString& str, MOuint index, MOuint length) : std::string((std::string)str, index, length) {}

	// Moldeo functions.
    moString& Left(MOuint nchar);					// Sets the string to the first nchar characters in the string.
    moString& Right(MOuint nchar);				// Sets the string to the last nchar characters in the string.
    moString& Mid(MOuint first, MOuint nchar);		// Sets the string to an intermediate substring.
    moString& SubText(MOuint first, MOuint last);	// Sets the string to an intermediate substring.

    moString& Insert(char *str, MOuint pos);      // Inserts s at position pos.
    moString& Delete(MOuint first, MOuint nchar);   // Deletes a part of the string.

    moString Scan(char *set);					// Returns the first substring separated by some of the characters in the set.
	moString ScanEx(char *set);					// Returns the first substring separated by some of the characters in the set and between commas.

    moString& LTrim();							// Removes the spaces at the beginning of the string.
    moString& RTrim();							// Removes the spaces at the end of the string.
    moString& Trim();							// Removes the spaces at both ends of the string.

	unsigned short* Short();					// Converts the string to an array of shorts.
    void ToUpper();								// Converts the string to uppercase.
    void ToLower();								// Converts the string to lowercase.

	MOuint Length() { return MOuint(length()); }

	// String functions.
	moString& assign(const moString& str) { std::string::assign((std::string)str); return *this; }
	moString& assign(const char* str) { std::string::assign(str); return *this; }
	moString& assign(const char* str, MOuint num) { std::string::assign(str, num); return *this; }
	moString& assign(const moString& str, MOuint index, MOuint len) { std::string::assign(str, index, len); return *this; }
	moString& assign(MOuint num, const char& ch) { std::string::assign(num, ch); return *this; }

	moString& append(const moString& str) { std::string::append((std::string)str); return *this; }
	moString& append(const char* str) { std::string::append(str); return *this; }
	moString& append(const moString& str, MOuint index, MOuint len) { std::string::append(str, index, len); return *this; }
	moString& append(const char* str, MOuint num) { std::string::append(str, num); return *this; }
	moString& append(MOuint num, char ch) { std::string::append(num, ch); return *this; }

	moString substr(MOuint index, MOuint num = npos) { return std::string::substr(index, num); }
	moString& erase(MOuint index = 0, MOuint num = npos) { std::string::erase(index, num); return *this; }

	MOuint find(const moString& str, MOuint index) { return std::string::find((std::string)str, index); }
	MOuint find(const char* str, MOuint index) { return std::string::find(str, index); }
	MOuint find(const char* str, MOuint index, MOuint length) { return std::string::find(str, index, length); }
	MOuint find(char ch, MOuint index) { return std::string::find(ch, index); }

	MOuint rfind(const moString& str, MOuint index) { return std::string::rfind((std::string)str, index); }
	MOuint rfind(const char* str, MOuint index) { return std::string::rfind(str, index); }
	MOuint rfind(const char* str, MOuint index, MOuint length) { return std::string::rfind(str, index, length); }
	MOuint rfind(char ch, MOuint index) { return std::string::rfind(ch, index); }

	int compare(const moString& str) { return std::string::compare((std::string)str); }
	int compare(const char* str) { return std::string::compare(str); }
	int compare(MOuint index, MOuint length, const moString& str) { return std::string::compare(index, length, (std::string)str); }
	int compare(MOuint index, MOuint length, const moString& str, MOuint index2, MOuint length2) { return std::string::compare(index, length, (std::string)str, index2, length2); }

	moString& insert(MOuint index, const moString& str) { std::string::insert(index, (std::string)str); return *this; }
	moString& insert(MOuint index, const char* str) { std::string::insert(index, str); return *this; }
	moString& insert(MOuint index1, const moString& str, MOuint index2, MOuint num) { std::string::insert(index1, (std::string)str, index2, num); return *this; }
	moString& insert(MOuint index, const char* str, MOuint num) { std::string::insert(index, str, num); return *this; }
	moString& insert(MOuint index, MOuint num, char ch) { std::string::insert(index, num, ch); return *this; }

	moString& replace(MOuint index, MOuint num, const moString& str) { std::string::replace(index, num, (std::string)str); return *this; }
	moString& replace(MOuint index1, MOuint num1, const moString& str, MOuint index2, MOuint num2) { std::string::replace(index1, num1, (std::string)str, index2, num2); return *this; }
	moString& replace(MOuint index, MOuint num, const char* str) { std::string::replace(index, num, str); return *this; }
	moString& replace(MOuint index, MOuint num1, const char* str, MOuint num2) { std::string::replace(index, num1, str, num2); return *this; }
	moString& replace(MOuint index, MOuint num1, MOuint num2, char ch) { std::string::replace(index, num1, num2, ch); return *this; }

	// Operators.
	friend LIBMOLDEO_API bool operator == (const moString& c1, const moString& c2) { return (std::string)c1 == (std::string)c2; }
	friend LIBMOLDEO_API bool operator != (const moString& c1, const moString& c2) { return (std::string)c1 != (std::string)c2; }
	friend LIBMOLDEO_API bool operator < (const moString& c1, const moString& c2) { return (std::string)c1 < (std::string)c2; }
	friend LIBMOLDEO_API bool operator > (const moString& c1, const moString& c2) { return (std::string)c1 > (std::string)c2; }
	friend LIBMOLDEO_API bool operator <= (const moString& c1, const moString& c2) { return (std::string)c1 <= (std::string)c2; }
	friend LIBMOLDEO_API bool operator >= (const moString& c1, const moString& c2) { return (std::string)c1 >= (std::string)c2; }

	friend LIBMOLDEO_API moString operator + (const moString& s1, const moString& s2) { return (std::string)s1 + (std::string)s2; }
	friend LIBMOLDEO_API moString operator + (const char* s, const moString& s2) { return s + (std::string)s2; }
	friend LIBMOLDEO_API moString operator + (char c, const moString& s2) { return c + (std::string)s2; }
	friend LIBMOLDEO_API moString operator + (const moString& s1, const char* s) { return (std::string)s1 + s; }
	friend LIBMOLDEO_API moString operator + (const moString& s1, char c) { return (std::string)s1 + c; }

	moString& operator = (const std::string& str) { (std::string)(*this) = str; return *this; }
	moString& operator = (const moString& str) { (std::string)(*this) = (std::string)str; return *this; }
	moString& operator = (const char* s) { (std::string)(*this) = s; return *this; }
	moString& operator = (char ch) { (std::string)(*this) = ch; return *this; }

	virtual operator const char* () { return c_str(); }
};
*/

#ifndef USE_MOTEXT0
typedef std::string moText;
#endif

#ifdef USE_MOTEXT0
typedef moText0 moText;
#endif

//====================================================================================
//  class: moTextHeap
//  derivada de:
//  descripcion: heap de strings
//  author: Fabricio Costa
//  fecha: ?
//====================================================================================

/// lista de textos
/**
 * lista tipo FIFO para textos.
 */
class LIBMOLDEO_API moTextHeap {

  public:
    moTextHeap();
    moTextHeap( int max_heap );
    moTextHeap( const moTextHeap& p_copy );
    moTextHeap& operator =( const moTextHeap& p_src );
    virtual ~moTextHeap();

    void Init( int max_heap = MO_MAX_DEBUG );
    void Push( const moText& p_text ) {
    	if ( n>=1 && (n-1-count)>=0 ) {array[n-1-count] = p_text;}
      count++;
    }
    moText Pop();
    const moText& Get( int x ) const;
    int  Count() const { return count; }
    int  GetMaxHeap() const { return n; }
    void Set( int p_position , const moText& p_text  );
    void Clean();

  private:
    	moText *array;
      int n;
      int count;
      moText m_NULL;

};

moDeclareExportedDynamicArray( moText, moTextArray );

//===========================================
//
//				Utilities
//
//===========================================

LIBMOLDEO_API moText IntToStr(int a);
LIBMOLDEO_API moText IntToStr( int a, int nzeros );
LIBMOLDEO_API moText IntToStr(unsigned int a);
LIBMOLDEO_API moText IntToStr(long a);
LIBMOLDEO_API moText IntToStr(unsigned long a);
LIBMOLDEO_API moText IntToStr(long long a);
LIBMOLDEO_API moText IntToStr(unsigned long long a);
LIBMOLDEO_API moText FloatToStr(double a);
LIBMOLDEO_API moText FloatToStr(double a, int n);
LIBMOLDEO_API moText FloatToStr(double a, int nzeros, int ndecimals );
LIBMOLDEO_API int HexToInt( const moText& hex );
LIBMOLDEO_API long HexToLong( const moText& hex );
LIBMOLDEO_API float StrToFloat( const moText& hex );
LIBMOLDEO_API double StrToDouble( const moText& hex );
LIBMOLDEO_API int StrToInt( const moText& hex );
LIBMOLDEO_API long StrToLong( const moText& hex );

#endif

