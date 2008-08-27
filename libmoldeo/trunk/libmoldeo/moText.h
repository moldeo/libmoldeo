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

#ifndef uint
#define uint    unsigned int
#endif

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

class moTextArray;

/// clase de para manejar textos
/**
 * Operaciones y funciones para la manipulación de cadenas de caracteres.
 */

class LIBMOLDEO_API moText0
{
public:
	moText0();
	moText0(const moText0&);
	moText0(const char*);

	virtual ~moText0();

    uint Length() const;               // devuelve el length del text sin contar el null del final.

    // operadores que usan solo objetos de la clase //
    moText0& operator = (const moText0& txt);
    moText0& operator +=( const moText0& txt);
    moText0 operator + ( const moText0& txt) const;

    int operator < ( const moText0& txt) const;
    int operator > ( const moText0& txt) const;
    int operator <=( const moText0& txt) const;
    int operator >=( const moText0& txt) const;
    int operator ==( const moText0& txt) const;
    int operator !=( const moText0& txt) const;

    // operadores que usan c-strings //
    moText0& operator = (const char* txt);
    moText0& operator = (const short* txt);
    moText0& operator +=(const char* txt);
    friend LIBMOLDEO_API moText0 operator +(const moText0& txt1,const char* txt2);
    friend LIBMOLDEO_API moText0 operator +(const char* txt1, const moText0& txt2);

    int operator < (const char* txt) const;
    int operator > (const char* txt) const;
    int operator <=(const char* txt) const;
    int operator >=(const char* txt) const;
    int operator ==(const char* txt) const;
    int operator !=(const char* txt) const;

    //char operator [](uint pos)            { return text[pos]; }

    // operadores de conversion para poder usar c-strings //
    virtual operator char*()                { return text; }

    // metodos comunes en una clase string//
    moText0& Left( uint);            // devuelve el comienzo de un text.
    moText0& Right( uint);           // devuelve el final de un text.
    moText0& Mid( uint, uint);       // devuelve un fragmento intermedio del text.(comienzo, cant de carac)
    moText0& SubText( uint, uint);   // devuelve un fragmento intermedio del text.(comienzo, final)

    moText0& Insert(const char*, uint);        // inserta un text dentro de otro.
    moText0& Delete( uint, uint);           // borra una parte del text.

    moText0  Scan(const char*);                  // saca y devuelve el primer fragmento de text que este separado por alguno de los caracteres de un cjto.
    moText0  ScanEx(const char*);                // saca y devuelve el primer fragmento de text que este separado por alguno de los caracteres de un cjto, y entre comillas.

    moText0& LTrim();                // saca los espacios del comienzo.
    moText0& RTrim();                // saca los espacios del final.
    moText0& Trim();                 // saca los espacios del comienzo y del final.

	unsigned short* Short();
    void ToUpper();                  // convierte el text a mayusculas.
    void ToLower();                  // convierte el text a minusculas.


    moTextArray     Explode(const char* ) const;
    void            ReplaceChar( const char* target, const char* replacement );
    void            Replace( const moText0& target, const moText0& replacement );

private:
    char*   text;
    uint    length;

    // Estos metodos son el motor de la clase. Contienen toda la logica
    // de como se maneja moText0. Todos los metodos publicos las usan.
public:
    void    txtcopy( const char* txt, uint pos /**/= 0, uint com /**/= 0, uint fin /**/= MO_TXT_COMPLETE);        // copy c-strings(reserva la memoria y pone los nulls al final)
	void    txtcopy( const short* txt, uint pos /**/= 0, uint com /**/= 0, uint fin /**/= MO_TXT_COMPLETE);        // copy c-strings(reserva la memoria y pone los nulls al final)
private:
    txtcval txtcomp( const char* txt, uint com1 = 0, uint com2 = 0) const;
    uint    txtfind( const char* txt, txtpert pert = MO_TXT_BELONG, uint com = 0, int dir = 1) const;
};

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
	moString(uint length, const char& ch) : std::string(length, ch) {}
	moString(const char* str) : std::string(str) {}
	moString(const char* str, uint length) : std::string(str, length) {}
	moString(const std::string& s) : std::string(s) {}
	moString(const moString& s) : std::string((std::string)s) {}
	moString(const std::string& str, uint index, uint length) : std::string(str, index, length) {}
	moString(const moString& str, uint index, uint length) : std::string((std::string)str, index, length) {}

	// Moldeo functions.
    moString& Left(uint nchar);					// Sets the string to the first nchar characters in the string.
    moString& Right(uint nchar);				// Sets the string to the last nchar characters in the string.
    moString& Mid(uint first, uint nchar);		// Sets the string to an intermediate substring.
    moString& SubText(uint first, uint last);	// Sets the string to an intermediate substring.

    moString& Insert(const char *str, uint pos);      // Inserts s at position pos.
    moString& Delete(uint first, uint nchar);   // Deletes a part of the string.

    moString Scan(const char *set);					// Returns the first substring separated by some of the characters in the set.
	moString ScanEx(const char *set);					// Returns the first substring separated by some of the characters in the set and between commas.

    moString& LTrim();							// Removes the spaces at the beginning of the string.
    moString& RTrim();							// Removes the spaces at the end of the string.
    moString& Trim();							// Removes the spaces at both ends of the string.

	unsigned short* Short();					// Converts the string to an array of shorts.
    void ToUpper();								// Converts the string to uppercase.
    void ToLower();								// Converts the string to lowercase.

	uint Length() { return uint(length()); }

	// String functions.
	moString& assign(const moString& str) { std::string::assign((std::string)str); return *this; }
	moString& assign(const char* str) { std::string::assign(str); return *this; }
	moString& assign(const char* str, uint num) { std::string::assign(str, num); return *this; }
	moString& assign(const moString& str, uint index, uint len) { std::string::assign(str, index, len); return *this; }
	moString& assign(uint num, const char& ch) { std::string::assign(num, ch); return *this; }

	moString& append(const moString& str) { std::string::append((std::string)str); return *this; }
	moString& append(const char* str) { std::string::append(str); return *this; }
	moString& append(const moString& str, uint index, uint len) { std::string::append(str, index, len); return *this; }
	moString& append(const char* str, uint num) { std::string::append(str, num); return *this; }
	moString& append(uint num, char ch) { std::string::append(num, ch); return *this; }

	moString substr(uint index, uint num = npos) { return std::string::substr(index, num); }
	moString& erase(uint index = 0, uint num = npos) { std::string::erase(index, num); return *this; }

	uint find(const moString& str, uint index) { return std::string::find((std::string)str, index); }
	uint find(const char* str, uint index) { return std::string::find(str, index); }
	uint find(const char* str, uint index, uint length) { return std::string::find(str, index, length); }
	uint find(const char ch, uint index) { return std::string::find(ch, index); }

	uint rfind(const moString& str, uint index) { return std::string::rfind((std::string)str, index); }
	uint rfind(const char* str, uint index) { return std::string::rfind(str, index); }
	uint rfind(const char* str, uint index, uint length) { return std::string::rfind(str, index, length); }
	uint rfind(const char ch, uint index) { return std::string::rfind(ch, index); }

	int compare(const moString& str) { return std::string::compare((std::string)str); }
	int compare(const char* str) { return std::string::compare(str); }
	int compare(uint index, uint length, const moString& str) { return std::string::compare(index, length, (std::string)str); }
	int compare(uint index, uint length, const moString& str, uint index2, uint length2) { return std::string::compare(index, length, (std::string)str, index2, length2); }

	moString& insert(uint index, const moString& str) { std::string::insert(index, (std::string)str); return *this; }
	moString& insert(uint index, const char* str) { std::string::insert(index, str); return *this; }
	moString& insert(uint index1, const moString& str, uint index2, uint num) { std::string::insert(index1, (std::string)str, index2, num); return *this; }
	moString& insert(uint index, const char* str, uint num) { std::string::insert(index, str, num); return *this; }
	moString& insert(uint index, uint num, char ch) { std::string::insert(index, num, ch); return *this; }

	moString& replace(uint index, uint num, const moString& str) { std::string::replace(index, num, (std::string)str); return *this; }
	moString& replace(uint index1, uint num1, const moString& str, uint index2, uint num2) { std::string::replace(index1, num1, (std::string)str, index2, num2); return *this; }
	moString& replace(uint index, uint num, const char* str) { std::string::replace(index, num, str); return *this; }
	moString& replace(uint index, uint num1, const char* str, uint num2) { std::string::replace(index, num1, str, num2); return *this; }
	moString& replace(uint index, uint num1, uint num2, char ch) { std::string::replace(index, num1, num2, ch); return *this; }

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

typedef moText0 moText;

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
	moText *array;
	int n;
	int count;

	moTextHeap();
	moTextHeap(int);
	~moTextHeap();

	void Push(moText);
	moText Pop();
	moText Get(int);
	void Set(int,moText);
};
/*
template class LIBMOLDEO_API moDynamicArray<moText>;
typedef moDynamicArray<moText> moTextArray;
*/
moDeclareExportedDynamicArray( moText, moTextArray )

//===========================================
//
//				Utilities
//
//===========================================

LIBMOLDEO_API moText IntToStr(int a);
LIBMOLDEO_API moText FloatToStr(double a);
LIBMOLDEO_API moText FloatToStr(double a, int n);


#endif
