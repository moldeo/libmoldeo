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

  Copyright(C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa
  Andrés Colubri

*******************************************************************************/

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

/**
 * Clase que encapsula una variable flotante de precisión doble. Esta clase almacena
 * el nombre y valor actual de la variable, y es utilizada para generar la lista de
 * variables y parámetros en la clase 'moMathFunction'.
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

/**
 * Clase base para definir funciones matemáticas.
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
	double OnDerivEval(int n) { return 0; }

	void BuildParamList() {}
	void BuildVarList() {}
};



#endif
