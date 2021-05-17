/*******************************************************************************

                                moMathFunction.h

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

  \copyright copyright(C) 2006 Fabricio Costa

  \author Fabricio Costa
  \author Andrés Colubri

*******************************************************************************/

/**
* \page MoldeoFunctions
* \brief \if spanish Funciones en Moldeo, sintaxis y operaciones. \else Functions in Moldeo platform, syntax and operations. \endif
*
* Las funciones son fórmulas permiten expresar un valor
* como una función dinámica dependiente de números, funciones exponenciales, logarítmicas ( exp, log ),
* funciones trigonométricas ( cos, sin, tan, acos, asin, atan ), y variables relacionadas con el tiempo transcurrido ( tempo, time ),
* y también parámetros personalizados (alpha, translatex, translatey, translatez, ... )
*
*
* Algunos ejemplos:
*
* \f$ 1 + 2 x coseno( time ) \f$
* se escribe sencillamente:
*  \code 1 + 2 * cos( time ) \endcode
*
* \f$ \sqrt{(translatex+translatey )^2} ) \f$
* se escribe sencillamente:
*  \code sqrt(exp(translatex+translatey),2)) \endcode
*
* \anchor muparser
*
* \if spanish
* Funciones predefinidas:
* \else
* Built in functions:
* \endif
*
* \if spanish	Seno \else	Sine function	\endif
* \image html function_sin.png
* \code sin(t) \endcode
* \if spanish	Coseno	\else	Cosine function	\endif
* \image html function_cos.png
* \code cos(t) \endcode
* \if spanish	Tangente	\else	Tangens function	\endif
* \image html function_tan.png
* \code tan(t) \endcode
* \if spanish	Arcoseno	\else	Arcus sine function	\endif
* \image html function_asin.png
* \code asin(t) \endcode
* \if spanish	Arco coseno	\else	Arcus cosine function	\endif
* \image html function_acos.png
* \code acos(t) \endcode
* \if spanish	Arco tangente	\else	Arcus tangens function	\endif
* \image html function_atan.png
* \code atan(t) \endcode
* \if spanish	Seno hyperbolico	\else	Hyperbolic sine function	\endif
* \image html function_sinh.png
* \code sinh(t) \endcode
* \if spanish	Coseno hyperbólico	\else	Hyperbolic cosine	\endif
* \image html function_cosh.png
* \code cosh(t) \endcode
* \if spanish	Tangente hyperbólica	\else	Hyperbolic tangens function	\endif
* \image html function_tanh.png
* \code tanh(t) \endcode
* \if spanish	Arcoseno hyperbólico	\else	hyperbolic arcus sine function	\endif
* \image html function_asinh.png
* \code asinh(t) \endcode
* \if spanish	arco coseno hyperbólico	\else	hyperbolic arcus cosine function	\endif
* \image html function_acosh.png
* \code acosh(t) \endcode
* \if spanish	arco tangente hyperbólico	\else	hyperbolic arcus tangens function	\endif
* \image html function_atanh.png
* \code atanh(t) \endcode
* \if spanish	logaritmo de base 2	\else	logarithm to the base 2	\endif
* \image html function_log2.png
* \code log2(t) \endcode
* \if spanish	logaritmo de base 10	\else	logarithm to the base 10	\endif
* \image html function_log10.png
* \code log10(t) \endcode
* \if spanish	logaritmo de base 10	\else	logarithm to the base 10	\endif
* \image html function_log.png
* \code log(t) \endcode
* \if spanish	logaritmo de base e (2.71828...)	\else	logarithm to base e (2.71828...)	\endif
* \image html function_ln.png
* \code ln(t) \endcode
* \if spanish	e elevado a la potencia de x	\else	e raised to the power of x	\endif
* \image html function_exp.png
* \code exp(t) \endcode
* \if spanish	raiz cuadrada	\else	square root of a value	\endif
* \image html function_sqrt.png
* \code sqrt(t) \endcode
* \if spanish	Signo, 1 si x positivo, -1 si x negativo	\else	sign function -1 if x<0; 1 if x>0 \endif
* \image html function_sign.png
* \code sign(t) \endcode
* \if spanish	redondeo al entero más cercano	\else	round to nearest integer	\endif
* \code rint(t) \endcode
* \if spanish	valor absoluto	\else	absolute value	\endif
* \code abs(t) \endcode
* \if spanish	mínimo de todos los argumentos	\else	min of all arguments	\endif
* \code min(x,y,z,...) \endcode
* \if spanish	máximo de todos los argumentos	\else	max of all arguments	\endif
* \code max(x,y,z,...) \endcode
* \if spanish	suma de todos los argumentos	\else	sum of all arguments	\endif
* \code sum(x,y,z,...) \endcode
* \if spanish	promedio de todos los argumentos	\else	mean value of all arguments	\endif
* \code avg(x,y,z,...) \endcode
*
* \if spanish
* Funciones especiales:
* \else
* Special functions:
* \endif
* \if spanish Valor aleatorio entre 0 y 1 \else random number between 0 and 1 \endif dónde \c s es la semilla y puede ser 0 u otro número.
* \code UnitRandom(s) \endcode
* \if spanish Valor aleatorio simétrico entre -1 y 1 \else symmetric random number between -1 and 1 \endif
* \code SymetricRandom( s ) \endcode
* \if spanish Valor alteatorio en el intervalo (\c a,\c b) con la semilla \c s. \else random number between \c a and \c b with the seed \c s. \endif
* \code IntervalRandom( a, b, s ) \endcode
*
* \if spanish
* Operadores predefinidos:
* \else
* Built in operators:
* \endif
*
*	\li	\c =		\if spanish	asignación	\else	assignement	\endif
*	\li	\c &&		\if spanish	y lógico	\else	logical and	\endif
*	\li	\c ||		\if spanish	o lógico	\else	logical or	\endif
*	\li	\c <=		\if spanish	inferior o igual	\else	less or equal	\endif
*	\li	\c >=		\if spanish	superior o igual	\else	greater or equal	\endif
*	\li	\c !=		\if spanish	distinto de	\else	not equal	\endif
*	\li	\c ==		\if spanish	igual a	\else	equal	\endif
*	\li	\c >		\if spanish	superior a	\else	greater than	\endif
*	\li	\c <		\if spanish	inferior a	\else	less than	\endif
*	\li	\c +		\if spanish	adición	\else	addition	\endif
*	\li	\c -		\if spanish	substracción	\else	subtraction	\endif
*	\li	\c *		\if spanish	multiplicación	\else	multiplication	\endif
*	\li	\c /		\if spanish	division	\else	division	\endif
*	\li	\c ^		\if spanish	elevar x a la potencia de y	\else	raise x to the power of y	\endif
*
*
* \if spanish	Ejemplos de funciones:	\else	Function example:	\endif
*
* \b Oscilación (inicia su valor en 1 y luego oscila entre 1 y -1):
* \code
* 1.0*cos(time)
* \endcode
*
* \b Oscilación (inicia su valor en 0 y luego oscila entre 1 y -1):
* \code
* 1.0*sin(time)
* \endcode
*
* \b Agrandamiento infinito ( de 0 a oo ):
* \code
* time*0.1
* \endcode
*
* \b Achicamiento infinito ( de 1.0 a 0.0):
* \code
* 1.0/(1.0+time*0.1)
* \endcode
*
* <H3>Funciones paramétricas</H3>
* \b Movimiento CIRCULAR dónde \c v es la velocidad y \c r es el radio.
* \image html funcion_circular.png
* \code
* r*cos( v*time )
* \endcode
* \code
* r*sin( v*time )
* \endcode
*
* \b Movimiento DIAGONAL dónde \c v es la velocidad y \c a el ancho y b el alto de la diagonal.
* \image html funcion_diagonal.png
* \code
* a*cos( v*time )
* \endcode
* \code
* b*cos( v*time )
* \endcode
*
*
* \b Movimiento BUCLE: dónde \c v es la velocidad y \c a y \c b el ancho y alto del bucle
* \image html funcion_bucle.png
* \code
* a*0.5*cos( v*time )
* \endcode
* \code
* b*0.5*sin( v*2*time )
* \endcode
*
* \b Movimiento BUCLE TREBOL: dónde \c v es la velocidad y \c a y \c b el ancho y alto del bucle
* \image html funcion_bucle_trebol.png
* \code
* a*0.5*cos( v*time )*cos( v*time )*sin( v*time )
* \endcode
* \code
* b*0.5*cos( v*time )*sin( v*time )*sin( v*time )
* \endcode
*
* \b Movimiento BUCLE CUADRADO
* \image html funcion_bucle_trebol.png
* \code
* (cos(3*t)-2*sin(t))*cos(t)*cos(t)
* \endcode
* \code
* (sin(3*t)-2*cos(t))*sin(t)*sin(t)
* \endcode
*
* \b Movimiento BUCLE BUO
* \image html funcion_bucle_buo.png
* \code
* (cos(t)-sin(t))*cos(2*t)
* \endcode
* \code
* (sin(t)-cos(t))*sin(2*t)
* \endcode
*
*
* \endif
*
*/

#ifndef __MO_MATH_FUNCTION_H__
#define __MO_MATH_FUNCTION_H__

#include "moTypes.h"
#include "moArray.h"
#include "moAbstract.h"
#include "moMath.h"

class moParam;
class moConfig;
class moMoldeoObject;
class moInlet;

typedef void moParser;

moDeclareExportedDynamicArray( MOuint, moIntArray );
moDeclareExportedDynamicArray( bool, moBoolArray );
moDeclareExportedDynamicArray( float, moFloatArray );

/// moMathVariable
/**
 * Clase que encapsula una variable flotante de precisión doble. Esta clase almacena
 * el nombre y valor actual de la variable, y es utilizada para generar la lista de
 * variables y parámetros en la clase 'moMathFunction'.
 *
 * una variable se define como una etiqueta con o sin numeros, por ejemplo: variable1, variable2, varx, vary
 * que indica un valor en memoria.
 *
 * Hay variables predefinidas para el sistema de funciones como son:
 * \li "tempo" el reloj interno rítmico, su ciclo interno.
 * \li "time" el reloj interno absoluto, el tiempo interno.
 *
 * A su vez, por cada parámetro definido en un objeto configurado, se definirá una variable del mismo nombre,
 * por ejemplo: "translatex", o "alpha".
 * Esa variable puede ser usada localmente por cualquier función e incluso combinarlas: por ejemplo:
 *
 * \f$ 1.0*sin( 1.618 * tempo ) \f$
 * \f$ cos( time )*alpha + translatey \f$
 * \f$ sin( time ) \f$
 *
 * \li
 *
 * La variable particletime, es definida en el plugin de ParticlesSimple y representa un parámetro
 * de referencia de tiempo particular en un grupo de elementos.
 * \li particletime
 *
 * @see moMathFunction
 */
class moMathVariable : public moAbstract
{
public:
    /**
     * El constructor de la clase.
     * @param p_name el nombre de la variable.
     * @param p_value0 el valor inicial de la variable.
     */
    moMathVariable();
	moMathVariable(const char* p_name, double p_value0 = 0.0);
	moMathVariable( moParam* p_Param  );
	moMathVariable( moInlet* p_Inlet  );

	void SetParam( moParam* p_Param );
	void SetInlet( moInlet* p_Inlet );

	/**
     * Asigna el nombre de la variable.
	 * @param p_name el nuevo nombre de la variable.
	 */
	void SetName(moText& p_name) { m_name = p_name; }
	/**
     * Devuelve el nombre de la variable.
	 * @return El nombre de la variable.
	 */
	moText& GetName() { return m_name; }

	/**
     * Asigna un nuevo valor flotante para la variable.
	 * @param p_name el nuevo valor de la variable.
	 */
	void SetValue(double p_value) { m_value = p_value; }
	/**
     * Devuelve el valor actual de la variable.
	 * @return El valor de la variable.
	 */
	double GetValue();




	/**
     * Devuelve el puntero a la variable privada de la clase que almacena el valor flotante actual.
	 * @return El puntero a la variable 'm_value'.
	 * @see m_value
	 */
	double* GetValuePointer();

private:
	/**
     * Almacena el nombre de la variable.
	 */
    moText m_name;
	/**
     * Almacena el valor actual de la variable.
	 */
    double m_value;

    /**
    *   External param data retreival, name must match!!!!
    */
    moParam*    m_pParam;

    /**
    *   External inlet data retreival, name must match!!!!
    */
    moInlet*    m_pInlet;
};

typedef moMathVariable* moMathVariablePtr;
moDeclareExportedDynamicArray( moMathVariablePtr, moMathVariableArray );

/// moMathFunction
/**
 * Clase base para definir funciones matemáticas.
 *
 * Las funciones son fórmulas permiten expresar un valor
 * como una función dinámica dependiente de números, funciones exponenciales, logarítmicas,
 * funciones trigonométricas ( cos, sin, tan, acos, ... ), y variables ( tempo, time, )
 *
 */
class LIBMOLDEO_API moMathFunction : public moAbstract
{
public:
	/**
     * Constructor por defecto. Solamente inicializa m_Expression y m_EmptyName como cadenas de texto vacias.
	 */
    moMathFunction();
	/**
     * Destructor por defecto.
	 */
	virtual ~moMathFunction();

	/**
     * Función de inicializacion en donde se asigna la expresión que caracteriza la función y se
	 * construyen las listas de parámetros y variables.
	 * @param p_Expression la expresión matemática.
	 * @return El resultado de la operación de inicialización.
	 */
	virtual MOboolean Init(const moText& p_Expression, moMoldeoObject* p_pMOB = NULL );
	/**
     * Función de finalización.
	 * @return El resultado de la operación de finalización.
	 */
    virtual MOboolean Finish();

	/**
     * Asigna la cadena de texto que define la función matemática.
	 * @param p_Expression la expresión matemática.
	 */
	virtual void SetExpression(const moText& p_Expression) { m_Expression = p_Expression; }
	/**
     * Devuelve la cadena de texto que define la función matemática.
	 * @return El texto de la expresión.
	 */
    virtual moText& GetExpression() { return m_Expression; }
	/**
     * Asigna los parámetros de la función.
	 * @param s el primer parámetro.
	 */
    virtual void SetParameters(double s, ...);

	/**
     * Calcula la función con las variables automaticamente evaluadas desde el config
	 * @return El valor de la función.
	 */
	virtual double Eval();

	/**
     * Devuelve el último valor resultante de la última evaluación de esta función
	 * @return El valor de la función.
	 */
	double LastEval();

	/**
     * Calcula la función para los valores de las variables dados.
	 * @param x el valor de la primer variable.
	 * @return El valor de la función.
	 */
	virtual double Eval(double x, ...);

	/**
     * Calcula la derivada n-ésima de la función para los valores de las variables dados.
	 * @param n el orden de la derivada.
	 * @return El valor de la derivada.
	 */
	virtual double DEval(int n, ...);

	/**
     * Devuelve el número de parámetros.
	 * @return El número de parámetros.
	 */
    MOuint GetParameterCount();
	/**
     * Devuelve el nombre del parámetro que corresponde al índice dado.
	 * @param i el índice del parámetro.
	 * @return El nombre del parámetro.
	 */
    moText& GetParameterName(int i);
	/**
     * Devuelve el valor del parámetro que corresponde al índice dado.
	 * @param i el índice del parámetro.
	 * @return El valor del parámetro.
	 */
    double GetParameterValue(int i);

	/**
     * Devuelve el número de variables.
	 * @return El número de variables.
	 */
	MOuint GetVariableCount();
	/**
     * Devuelve el nombre de la variable que corresponde al índice dado.
	 * @param i el índice de la variable.
	 * @return El nombre de la variable.
	 */
    moText& GetVariableName(int i);
	/**
     * Devuelve el valor actual de la variable que corresponde al índice dado.
	 * @param i el índice de la variable.
	 * @return El valor actual de la variable.
	 */
    double GetVariableValue(int i);

	/**
     * Calcula la función para los valores de las variables dados.
	 * @param x el valor de la primer variable.
	 * @return El valor de la función.
	 */
	double operator () (double x, ...);
protected:
	/**
     * Almacena una cadena de texto vacía para devolver cuando se intenta acceder un índice de parámetro
	 * o variable inválido.
	 */
	moText m_EmptyName;
	/**
     * La expresión matemática que define la función.
	 */
	moText m_Expression;
	/**
     * Lista de parámetros.
	 */
	moMathVariableArray m_Parameters;
	/**
     * Lista de variables.
	 */
	moMathVariableArray m_Variables;

	double m_LastEval;

	/**
     * Función abstracta pura que es llamada desde la función 'SetParameters'.
	 * Debe implementar los cálculos que actualizan la función dado un nuevo conjunto de parámetros.
	 * @see SetParameters
	 */
	virtual void OnParamUpdate() = 0;
	/**
     * Función abstracta pura que es llamada desde la función 'Eval'.
	 * Debe implementar el cálculo de la función dados los valores actuales de las variables.
	 * @return El valor de la función
	 * @see Eval
	 */
	virtual double OnFuncEval() = 0;
	/**
     * Función abstracta pura que es llamada desde la función 'DEval'.
	 * Debe implementar el cálculo de la derivada n-ésima de la función dados los valores actuales de las variables.
	 * @return El valor de la función
	 * @see DEval
	 */
	virtual double OnDerivEval(int n) = 0;

	/**
     * Función abstracta pura que es llamada desde la función 'Init'.
	 * Debe implemtar la construcción de la lista de parámetros.
	 * @see Init
	 */
	virtual void BuildParamList() = 0;
	/**
     * Función abstracta pura que es llamada desde la función 'Init'.
	 * Debe implemtar la construcción de la lista de variables.
	 * @see Init
	 */
	virtual void BuildVarList() = 0;

    moConfig*   m_pConfig;
    moMoldeoObject* m_pMOB;
};

typedef moMathFunction* MathFunctionPtr;
moDeclareExportedDynamicArray( MathFunctionPtr, moMathFunctionArray );

/*******************************************************************************
                     Algunas funciones matemáticas pre-defindas
*******************************************************************************/

/**
 * Clase que encapsula un polinomio interpolante de grado 3. Esta función pasa
 * por los puntos (x0, y0) y (x1, y1) de manera tal que las primera derivadas
 * en x0 y x1 sean d0 y d1, respectivamente.
 * El ordenamiento de los parámetros es x0, y0, d0, x1, y1, d1.
 */
class LIBMOLDEO_API moCubicInterpolant : public moMathFunction
{
public:
	/**
     * Función de inicializacion en donde se asigna la expresión que caracteriza la función y se
	 * construyen las listas de parámetros y variables.
	 * @param p_Expression la expresión matemática.
	 * @return El resultado de la operación de inicialización.
	 */
	MOboolean Init(moText& p_Expression);
private:
    double C1, C2, C3, C4;

	void OnParamUpdate();
	double OnFuncEval();
	double OnDerivEval(int n);

	void BuildParamList();
	void BuildVarList();
};

/**
 * Clase que encapsula un spline "tenso" de grado 3. Pasa por los puntos (x0, y0) y
 * (x1, y1), las primera derivadas en x0 y x1 son d0 y d1, respectivamente, y además
 * reproduce la convexidad/concavidad de los puntos. El parámetro gamma en (0, 3)
 * incrementa la suavidad de la curva a medida que se acerca a 3.
 * El ordenamiento de los parámetros es x0, y0, d0, x1, y1, d1, gamma.
 * Para mas información, ver la siguiente referencia:
 * "A Practical Guide to Splines", Carl de Boor, Springer, chapter XVI.
 */
class LIBMOLDEO_API moTautInterpolant : public moMathFunction
{
public:
	/**
     * Función de inicializacion en donde se asigna la expresión que caracteriza la función y se
	 * construyen las listas de parámetros y variables.
	 * @param p_Expression la expresión matemática.
	 * @return El resultado de la operación de inicialización.
	 */
	MOboolean Init(moText& p_Expression);
private:
    double A, B, C, D;
    double delta, alpha, zeta, knot;
    double delta_inv, one_minus_knot_inv;
	double alpha2, knot2, one_minus_knot_inv2;

    double PhiFunc1(double u);
    double DPhiFunc1(double u);

    double PhiFunc2(double u);
    double DPhiFunc2(double u);

	void OnParamUpdate();
	double OnFuncEval();
	double OnDerivEval(int n);

	void BuildParamList();
	void BuildVarList();
};

moMathFunction* BuiltInMathFunctionFactory(const moText& p_expr);

/**
 * Clase de creación de variables. Las variables son clasificadas en dos tipos:
 * variables propiamente dichas, como t en la expresión sin(t), y parámetrso,
 * que se identifican con el prefijo "_": sin(_w * t). Esta distinción, que para
 * muParser es totalmente arbitraria, permite distingir entre valores que son
 * actualizados con mucha frecuencia (variables) y aquellos que solo cambian
 * unas pocas veces (parámetros).
 */
class LIBMOLDEO_API moMathVariableFactory : public moAbstract
{
public:
	/**
     * Constructor donde se especifican las listas de parámetros y variables.
	 * @param p_pParArray puntero a la lista de variables.
	 * @param p_pParArray puntero a la lista de parámetros.
	 */
	moMathVariableFactory(moMathVariableArray* p_pParArray, moMathVariableArray* p_pVarArray)
	{
		m_pParArray = p_pParArray;
		m_pVarArray = p_pVarArray;
	}

	/**
     * Agrega una nueva variable o parámetro (uno u otro dependiendo de la presencia
	 * del prefijo "_" en el nombre.
	 * @param p_pNewName nombre de la variable.
	 * @return puntero double donde se almacena el valor de la variable.
	 */
    double* CreateNewVariable(const char *p_pNewName)
	{
		moMathVariable* pvar = new moMathVariable(p_pNewName, 0.0);

		if (p_pNewName != NULL) {
			if (p_pNewName[0] == '_') m_pParArray->Add(pvar); // Agregando parámetro.
			else m_pVarArray->Add(pvar);                      // Agregando variable.
		}
        return pvar->GetValuePointer();
	}
private:
	moMathVariableArray* m_pParArray;
	moMathVariableArray* m_pVarArray;
};

/**
 * Función de creación de variables, utilizada por muParser.
 */
double* AddParserVariableFunction(const char *p_pVarName, void *p_pUserData);

/**
 * Clase que encapsula un muParser.
 */
class LIBMOLDEO_API moParserFunction : public moMathFunction
{
public:

    moParserFunction();


	/**
     * Función de inicializacion en donde se asigna la expresión que caracteriza la función y se
	 * construyen las listas de parámetros y variables.
	 * @param p_Expression la expresión matemática.
	 * @return El resultado de la operación de inicialización.
	 */
	virtual MOboolean Init(const moText& p_Expression, moMoldeoObject* p_pMOB = NULL );
	/**
     * Función de finalización.
	 * @return El resultado de la operación de finalización.
	 */
	MOboolean Finish();
protected:

	double x;

	moParser*   m_pParser;

	void AddMathFunctions();
	void AddMathConstants();
	MOboolean CheckVariables();

	void OnParamUpdate() {}
	double OnFuncEval();
	double OnDerivEval(int n) { return n; }

	void BuildParamList() {}
	void BuildVarList() {}
};



#endif
