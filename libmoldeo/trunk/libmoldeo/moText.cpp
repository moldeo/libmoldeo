/*******************************************************************************

                              moText.cpp

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

#include "moText.h"


#include "moArray.cpp"

moDefineDynamicArray( moTextArray )


//===========================================
//
//				moText0
//
//===========================================

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

moText0::moText0()
{
    text = NULL;;
    length = 0;
    txtcopy("");
}

moText0::moText0(const moText0& txt)
{
    text = NULL;
    length = 0;
    txtcopy( txt.text);
}

moText0::moText0( const char* txt)
{
    text = NULL;
    length = 0;
    txtcopy( txt);
}

/* Eliminado por cristian ya que no tiene sentido *
moText0::moText0(const char* txt)
{
    text = NULL;
    length = 0;
    txtcopy( txt);
}
*/

moText0::~moText0()
{
    if(text != NULL)
        delete [] text;
	text = NULL;
}

//////////////////////////////////////////////////////////////////////
// Metodos privados
//////////////////////////////////////////////////////////////////////

// txtcopy copy del c-string [txt] desde la posicion [com] hasta [fin]
// a la posicion [pos] de moText0 sobreescribiendo y siempre agregando
// un null al final.

void moText0::txtcopy( const char* txt, uint pos, uint com, uint fin)
{
    uint i;
    char *finaltxt;

	if (txt==NULL) txt = ""; //cadena vacia

    if(pos >= length)               // si pide copyr mas all�al final concatena
        pos = length;

    if(fin == MO_TXT_COMPLETE)        // si se pide copyr todo el string en necesario
    {                               // saber cuantos caracteres son.
        fin = com;
        while( txt[fin]!='\0')
            fin++;
    }

    if(fin < com)                  // si el final esta antes del comienzo al menos
        fin = com;                  // un caracter es copydo.


    finaltxt = new char[pos+(fin-com+1)+1]; // pido memoria para la parte del text original
                                            // + el nuevo text + el null.

    i = 0;
    while( i<pos)                       // copio la parte del text original.
    {
        finaltxt[i] = text[i];
        i++;
    }

    i = com;
    while( i<=fin && txt[i]!='\0')      // copio la parte del nuevo text.
    {
        finaltxt[pos+i-com] = txt[i];
        i++;
    }
    finaltxt[pos+i-com] = '\0';         // pongo el null al final.

    if(text != NULL)                  // actualizo el objeto moText0.
		delete text;
    text = finaltxt;
    length = pos+i-com;

    //return finaltxt;
}


void moText0::txtcopy( const short* txt, uint pos, uint com, uint fin)
{
    uint i;
    char *finaltxt;

    if(pos >= length)               // si pide copyr mas all�al final concatena
        pos = length;

    if(fin == MO_TXT_COMPLETE)        // si se pide copyr todo el string en necesario
    {                               // saber cuantos caracteres son.
        fin = com;
        while( txt[fin]!='\0')
            fin++;
    }

    if(fin < com)                  // si el final esta antes del comienzo al menos
        fin = com;                  // un caracter es copydo.


    finaltxt = new char[pos+(fin-com+1)+1]; // pido memoria para la parte del text original
                                            // + el nuevo text + el null.

    i = 0;
    while( i<pos)                       // copio la parte del text original.
    {
        finaltxt[i] = text[i];
        i++;
    }

    i = com;
    while( i<=fin && txt[i]!='\0')      // copio la parte del nuevo text.
    {
        finaltxt[pos+i-com] =(char)txt[i];
        i++;
    }
    finaltxt[pos+i-com] = '\0';         // pongo el null al final.

    if(text != NULL)                  // actualizo el objeto moText0.
		delete text;
    text = finaltxt;
    length = pos+i-com;

    //return finaltxt;
}

// txtcomp compara caracter a caracter [text] con [txt] a partir de las posiciones
// [com1] y [com2] respectivamente.  Es CASE-SENSITIVE.

txtcval moText0::txtcomp( const char* txt, uint com1, uint com2) const
{
    uint i1, i2;

    if(com1 > length)           // si el comienzo supera el length del text empieza
        com1 = length - 1;       // por el last caracter.

    i1 = com1;
    i2 = com2;
    while( text[i1]!='\0' && txt[i2]!='\0')
    {
        if(text[i1] < txt[i2])        // para que devuelva IGUAL ambos texts tienen
            return MO_TXT_LESSER;           // que tener los mismos caracteres y la misma
        else                            // longitud,
        if(text[i1] > txt[i2])
            return MO_TXT_GREATER;           // si primero aparece una diferencia en los ca_
        else                            // racteres es menos el que este primero en la
        {                               // tabla ASCII.
            i1++;
            i2++;
        }
    }

    if(text[i1] == '\0' && txt[i2] != '\0')       // si tienen los mismos caracteres
        return MO_TXT_LESSER;                           // pero uno es mas corto, entonces
    else                                            // ese es el menor.
    if(text[i1] != '\0' && txt[i2] == '\0')
        return MO_TXT_GREATER;

    return MO_TXT_EQUAL;
}


// txtfind busca en [text] desde la posicion [com] la pertenencia de los caracteres
// de [cjto].  Si [pert] es PERTENECE devuelve la posicion del primer caracter de
// [text] perteneciente a [cjto], si es NOT_BELONG la posicion del primer caracter
// de [text] no perteneciente a [cjto]. La busqueda se realiza en la direccion [dir].

uint moText0::txtfind( const char* cjto, txtpert pert, uint com, int dir) const
{
    uint i, j, found;

    if(dir > 1)                // si la direccion es distinta de 1 o -1 es corregida.
        dir = 1;

    if(dir <= 0)
        dir = -1;

    if(com >= length)           // si el comienzo supera el length del text comienza
        com = length - 1;        // por el last caracter.

    switch(pert)
    {
        case MO_TXT_BELONG:
            i = com;
            while( i<length)             // recorre el text.
            {
                j = 0;
                while( cjto[j] != '\0')         // recorre el cjto de caracteres.
                {
                    if(text[i] == cjto[j])    // si encuentra un caracter...
                        return i;               // devuelve la posicion...
                    j++;
                }
                i += dir;                       // sino sigue buscando.
            }
            break;

        case MO_TXT_NOT_BELONG:
            i = com;
            while( i<length)             // recorre el text.
            {
                found = 0;
                j = 0;
                while( cjto[j] != '\0')         // recorre el cjto de caracteres.
                {
                    if(text[i] == cjto[j])    // si encuentra un caracter..
                        found = 1;              // va a seguir buscando...
                    j++;
                }
                if(!found)                     // sino devuelve la posicion.
                    return i;
                i += dir;
            }
            break;
    }

    return MO_TXT_NOT_FOUND;
}


//////////////////////////////////////////////////////////////////////
// Metodos publicos
//////////////////////////////////////////////////////////////////////

uint moText0::Length() const
{
    return length;
}

moText0& moText0::operator =( const moText0& txt)
{
    txtcopy( txt.text, 0, 0, txt.length);
    return *this;
}

moText0& moText0::operator +=(const moText0& txt)
{
    txtcopy( txt.text, MO_TXT_COMPLETE, 0, txt.length);
    return *this;
}

moText0 moText0::operator +(const moText0& txt) const
{
    moText0 txtres(text);
    txtres.txtcopy(txt.text, MO_TXT_COMPLETE, 0, txt.length);
    return txtres;
}

int moText0::operator <(const moText0& txt) const
{
    return( txtcomp(txt.text) == MO_TXT_LESSER);
}

int moText0::operator >(const moText0& txt) const
{
    return( txtcomp(txt.text) == MO_TXT_GREATER);
}

int moText0::operator <=(const moText0& txt) const
{
    return( txtcomp(txt.text)==MO_TXT_LESSER || txtcomp(txt.text)==MO_TXT_EQUAL);
}

int moText0::operator >=(const moText0& txt) const
{
    return( txtcomp(txt.text)==MO_TXT_GREATER || txtcomp(txt.text)==MO_TXT_EQUAL);
}

int moText0::operator ==(const moText0& txt) const
{
    return( txtcomp(txt.text) == MO_TXT_EQUAL);
}

int moText0::operator !=(const moText0& txt) const
{
    return( txtcomp(txt.text) != MO_TXT_EQUAL);
}


moText0& moText0::operator =(const char* txt)
{
    txtcopy(txt);
    return *this;
}

moText0& moText0::operator =(const short* txt)
{
    txtcopy(txt);
    return *this;
}

moText0& moText0::operator +=(const char* txt)
{
    txtcopy(txt,MO_TXT_COMPLETE);
    return *this;
}

LIBMOLDEO_API moText0 operator +(const moText0& txt1,const char* txt2)
{
    moText0 txtres(txt1);
    txtres.txtcopy( txt2, MO_TXT_COMPLETE);
    return txtres;
}

LIBMOLDEO_API moText0 operator +(const char* txt1, const moText0& txt2)
{
    moText0 txtres(txt1);
    txtres.txtcopy( txt2.text, MO_TXT_COMPLETE, 0, txt2.Length());
    return txtres;
}


int moText0::operator < (const char* txt) const
{
    return( txtcomp(txt) == MO_TXT_LESSER);
}

int moText0::operator > (const char* txt) const
{
    return( txtcomp(txt) == MO_TXT_GREATER);
}

int moText0::operator <=(const char* txt) const
{
    return( txtcomp(txt)==MO_TXT_LESSER || txtcomp(txt)==MO_TXT_EQUAL);
}

int moText0::operator >=(const char* txt) const
{
    return( txtcomp(txt)==MO_TXT_GREATER || txtcomp(txt)==MO_TXT_EQUAL);
}

int moText0::operator ==(const char* txt) const
{
    return( txtcomp(txt) == MO_TXT_EQUAL);
}

int moText0::operator !=(const char* txt) const
{
    return( txtcomp(txt) != MO_TXT_EQUAL);
}

unsigned short* moText0::Short() {

	unsigned short *txtshort;
	uint i = 0;

	txtshort = new unsigned short [length+1];

    while( i < length )                       // copio la parte del text original.
    {
        txtshort[i] =(char)text[i];
        i++;
    }
	txtshort[length] = '\0';

	return txtshort;
}

moText0& moText0::Left( uint cant)
{
    txtcopy( text, 0, 0, cant);
    return *this;
}

moText0& moText0::Right( uint cant)
{
    txtcopy( text, 0, length-cant, MO_TXT_COMPLETE);
    return *this;
}

moText0& moText0::Mid( uint com, uint cant)
{
    txtcopy( text, 0, com, com+cant-1);
    return *this;
}

moText0& moText0::SubText( uint com, uint fin)
{
    txtcopy( text, 0, com, fin);
    return *this;
}


moText0& moText0::Insert(const char* txt, uint pos)
{
    moText0 tmp( text+pos);
    txtcopy( txt, pos);
    txtcopy( tmp.text, MO_TXT_COMPLETE);
    return *this;
}

moText0& moText0::Delete( uint pos, uint cant)
{
    txtcopy( text, pos, pos+cant, MO_TXT_COMPLETE);
    return *this;
}


// Scan devuelve un nuevo text que contiene todos los caracteres de [text] desde el
// primer caracter no perteneciente a [cjto] hasta el next caracter perteneciente
// a [cjto] y saca esta parte de [text].

moText0 moText0::Scan(const char* cjto)
{
    moText0 newtxt;
    uint pos1 = txtfind( cjto, MO_TXT_NOT_BELONG);
    uint pos2 = txtfind( cjto, MO_TXT_BELONG, pos1);
    if(pos1 == MO_TXT_NOT_FOUND)  pos1 = 0;
    if(pos2 == MO_TXT_NOT_FOUND)  pos2 = length;
    newtxt.txtcopy( text, 0, pos1, pos2-1);
    txtcopy( text, 0, pos2);
    return newtxt;
}

moText0 moText0::ScanEx(const char* cjto)
{
    moText0 newtxt;
	uint pos2;
	uint pos1;

	pos1 = txtfind( cjto, MO_TXT_NOT_BELONG);

    if(pos1 == MO_TXT_NOT_FOUND)  {
		pos1 = 0;
		pos2 = txtfind( cjto, MO_TXT_BELONG, pos1);
	} else if ( text[pos1] == '\"') {//empieza por una comilla
		if ( (pos1+1) < length ) {
			pos2 = txtfind( "\"", MO_TXT_BELONG, pos1+1);//buscamos la q cierra
		} else pos2 = pos1+1;

		if ( pos2 == MO_TXT_NOT_FOUND ) {//sino buscamos el separador mas cercano
			pos1 = pos1+1;
			pos2 = txtfind( cjto, MO_TXT_BELONG, pos1);
		} else {
			//texto entre comillas
			pos1 = pos1+1;
			pos2 = pos2;
			newtxt.txtcopy( text, 0, pos1, pos2-1);
			txtcopy( text, 0, pos2+1);
			return newtxt;
		}

	} else {
		pos2 = txtfind( cjto, MO_TXT_BELONG, pos1);
	}

    if(pos2 == MO_TXT_NOT_FOUND)  pos2 = length;

	newtxt.txtcopy( text, 0, pos1, pos2-1);
    txtcopy( text, 0, pos2);
    return newtxt;
}

moText0& moText0::LTrim()
{
    uint pos = txtfind( " \t\n", MO_TXT_NOT_BELONG);
    if(pos != MO_TXT_NOT_FOUND)
        Right( length-pos);
    else if(txtfind( " \t\n", MO_TXT_BELONG) != MO_TXT_NOT_FOUND)
        txtcopy("");
    return *this;
}

moText0& moText0::RTrim()
{
    uint pos = txtfind( " \t\n", MO_TXT_NOT_BELONG, length, -1);
    if(pos != MO_TXT_NOT_FOUND)
        Left( pos);
    else if(txtfind( " \t\n", MO_TXT_BELONG, length, -1) != MO_TXT_NOT_FOUND)
        txtcopy("");
    return *this;
}

moText0& moText0::Trim()
{
    LTrim();
    RTrim();
    return *this;
}

void moText0::ToUpper()
{
    // Revisar... en Linux los caracteres de inicializacion de interchar1, interchar2 eran ilegibles.
    moText0 interchar1(" ");
    moText0 interchar2(" ");

    char tmp[2] = " ";
    uint i, pos;

    for( i=0; i<length; i++)
        if(text[i] >= 'a' && text[i] <= 'z')
            text[i] = text[i] -('a'-'A');
        else
        {
            tmp[0] = text[i];
            pos = interchar1.txtfind(tmp);
            if(pos != MO_TXT_NOT_FOUND)
                text[i] = interchar2[pos];
        }
}

void moText0::ToLower()
{
    // Revisar... en Linux los caracteres de inicializacion de interchar1, interchar2 eran ilegibles.
    moText0 interchar1(" ");
    moText0 interchar2(" ");

    char tmp[2] = " ";
    uint i, pos;

    for( i=0; i<length; i++)
        if(text[i] >= 'A' && text[i] <= 'Z')
            text[i] = text[i] +('a'-'A');
        else
        {
            tmp[0] = text[i];
            pos = interchar2.txtfind(tmp);
            if(pos != MO_TXT_NOT_FOUND)
                text[i] = interchar1[pos];
        }
}


moTextArray
moText0::Explode(const char* separator ) const
{

    moTextArray Tarray;
    moText0 Ltext, Rtext;

    //std::string txtstr = text;
    Rtext = (*this);

    uint posa, posb;
    posa = 0;
    posb = 0;

    Ltext = Rtext.Scan(separator);

    while ( Ltext.Length() > 0 ) {

        if (Ltext.Length())
            Tarray.Add( Ltext );

        Ltext = Rtext.Scan(separator);

    }

    return Tarray;
}


void
moText0::Replace( const moText0& target, const moText0& replacement ) {

    moText0 newone;
    int toreplace,i,j;
    moText0 mLeft;
    moText0 mRight = (*this);

    i = 0;

    while( mRight.Length()>0 ) {

        bool cmp = false;

        toreplace = -1;

        for(j =0; j<target.Length(); j++ ) {

            if ( target.text[j] == mRight.text[i+j] ) {
                cmp = true;
                if ((j+1)==target.Length()) {
                    toreplace = i;
                }
            } else {
                cmp = false;
                break;
            }

        }

        i++;

        if (toreplace>-1) {
            mLeft = mRight;
            mLeft.Left(toreplace);
            mRight.Right( Length() - (toreplace+target.Length()));
            newone+= mLeft;
            newone+= replacement;
            i = 0;
        }



    }

}

void
moText0::ReplaceChar( const char* target, const char* replacement ) {

    moText tt = (char*)target;
    moText tr = (char*)replacement;
    if ( tt.Length()==1 && tr.Length()==1 )
        for( int i=0; i<Length(); i++) {
            char c = target[0];
            if (text[i]==c) {
                text[i] = replacement[0];
            }
        }

}

/*
//===========================================
//
//				moString
//
//===========================================

moString& moString::Left(uint nchar)
{
	erase(nchar, length() - 1);
	return *this;
}

moString& moString::Right(uint nchar)
{
	erase(0, length() - 1 - nchar);
	return *this;
}

moString& moString::Mid(uint first, uint nchar)
{
	uint last = first + nchar - 1;
	return SubText(first, last);
}

moString& moString::SubText(uint first, uint last)
{
	erase(last + 1, length() - 1);
	erase(0, first - 1);
	return *this;
}

moString& moString::Insert(const char *str, uint pos)
{
	insert(pos, str);
	return *this;
}

moString& moString::Delete(uint first, uint nchar)
{
	erase(first, first + nchar - 1);
	return *this;
}

moString moString::Scan(const char *set)
{
	moString new_str;
	if (set != NULL)
	{
		char c;
		int j = 0;
		std::string::size_type p0, p1;
		while(set[j] != '\0')
		{
			c = set[j];
			p0 = find(c, 0);
			if (std::string::npos != p0)
			{
				p1 = find(c, p0 + 1);
				if (std::string::npos == p1) p1 = length() - 1;
				new_str = substr(p0, p1 - p0 + 1);
				return new_str;
			}
			j++;
		}
	}
	return new_str;
}

moString moString::ScanEx(const char *set)
{
	moString new_str0;
	if (set != NULL)
	{
		char c;
		int j = 0;
		std::string::size_type p0, p1;
		while(set[j] != '\0')
		{
			c = set[j];
			p0 = find(c, 0);
			if (std::string::npos != p0)
			{
				moString new_str;

				p1 = find(c, p0 + 1);
				if (std::string::npos == p1) p1 = length() - 1;
				new_str0 = substr(p0, p1 - p0 + 1);

				// Getting substring between commas in new_str0.
				p0 = new_str0.find_first_not_of('\"');
				if (std::string::npos == p0) p0 = 0;
				p1 = new_str0.find_last_not_of('\"');
				if (std::string::npos == p1) p0 = new_str0.length() - 1;

				new_str = new_str0.substr(p0, p1 - p0 + 1);
				return new_str;
			}
			j++;
		}
	}
	return new_str0;
}

moString& moString::LTrim()
{
	erase(0, find_first_not_of(' '));
	return *this;
}

moString& moString::RTrim()
{
	erase(find_last_not_of(' ') + 1);
	return *this;
}

moString& moString::Trim()
{
	LTrim();
	RTrim();
	return *this;
}

unsigned short* moString::Short()
{
	unsigned short *str_short;
	str_short = new unsigned short [length() +1];

	for (uint i = 0; i < length(); i++) str_short[i] = (unsigned short)(*this)[i];
	str_short[length()] = '\0';

	return str_short;
}

void moString::ToUpper()
{
	char c;
	for (uint i = 0; i < length(); i++)
	{
		c = (*this)[i];
		if (c >= 'A' && c <= 'Z') (*this)[i] = c - ('a' - 'A');
	}
}

void moString::ToLower()
{
	char c;
	for (uint i = 0; i < length(); i++)
	{
		c = (*this)[i];
		if (c >= 'A' && c <= 'Z') (*this)[i] = c + ('a' - 'A');
	}
}
*/

//===========================================
//
//				moTextHeap
//
//===========================================

moTextHeap::moTextHeap() {
	n = MO_MAX_DEBUG;
	array = new moText [n];
	count = 0;
}

moTextHeap::moTextHeap(int m) {
	n = m;
	array = new moText [n];
}


moTextHeap::~moTextHeap() {
	if(array!=NULL) delete[] array;
	array = NULL;
}


void
moTextHeap::Push(moText T) {
	if ( n>=1 && (n-1-count)>=0 ) {array[n-1-count] = T;}
	count++;
}


moText
moTextHeap::Pop() {
	int i;
	moText ret;
	if(n>=1) {ret = array[n-1];}
	if(n>=2) {for(i=(n-1);i>0;i--) array[i] = array[i-1];}
	if (count>0) count--;
	return ret;
}


moText
moTextHeap::Get(int x) {
	if(x<n) return(array[x]);
	else printf("moTextHeap:: Error");
	return moText("");
}

void
moTextHeap::Set(int x,moText T) {
	if(x<n)  { array[x] = T; }
	else printf("moTextHeap:: Error");
}

//===========================================
//
//				Utilities
//
//===========================================

LIBMOLDEO_API moText IntToStr(int a)
{
    char buffer[100];
    snprintf(buffer, 100, "%i", a); // Memory-safe version of sprintf.

    moText str = buffer;
    return str;
}

LIBMOLDEO_API moText0 FloatToStr(double a)
{
    char buffer[100];
    snprintf(buffer, 100, "%f", a); // Memory-safe version of sprintf.

    moText str = buffer;

    str.ReplaceChar( ",",  "." );

    return str;
}

LIBMOLDEO_API moText0 FloatToStr(double a, int n)
{
	char *buffer;
	buffer = new char[n];
    snprintf(buffer, n, "%f", a); // Memory-safe version of sprintf.

    moText str = buffer;
	delete buffer;

	str.ReplaceChar( ",",  "." );

    return str;
}
