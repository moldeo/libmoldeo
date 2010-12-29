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
  Andr�s Colubri

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
 * Clase que encapsula una variable flotante de precisi�n doble. Esta clase almacena
 * el nombre y valor actual de la variable, y es utilizada para generar la lista de
 * variables y par�metros en la clase 'moMathFunction'.
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
 * Clase base para definir funciones matem�ticas.
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
     * Funci�n de inicializacion en donde se asigna la expresi�n que caracteriza la funci�n y se
	 * construyen las listas de par�metros y variables.
	 * @param p_Expression la expresi�n matem�tica.
	 * @return El resultado de la operaci�n de inicializaci�n.
	 */
	virtual MOboolean Init(const moText& p_Expression, moMoldeoObject* p_pMOB = NULL );
	/**
     * Funci�n de finalizaci�n.
	 * @return El resultado de la operaci�n de finalizaci�n.
	 */
    virtual MOboolean Finish();

	/**
     * Asigna la cadena de texto que define la funci�n matem�tica.
	 * @param p_Expression la expresi�n matem�tica.
	 */
	virtual void SetExpression(const moText& p_Expression) { m_Expression = p_Expression; }
	/**
     * Devuelve la cadena de texto que define la funci�n matem�tica.
	 * @return El texto de la expresi�n.
	 */
    virtual moText& GetExpression() { return m_Expression; }
	/**
     * Asigna los par�metros de la funci�n.
	 * @param s el primer par�metro.
	 */
    virtual void SetParameters(double s, ...);

	/**
     * Calcula la funci�n con las variables automaticamente evaluadas desde el config
	 * @return El valor de la funci�n.
	 */
	virtual double Eval();

	/**
     * Devuelve el �ltimo valor resultante de la �ltima evaluaci�n de esta funci�n
	 * @return El valor de la funci�n.
	 */
	double LastEval();

	/**
     * Calcula la funci�n para los valores de las variables dados.
	 * @param x el valor de la primer variable.
	 * @return El valor de la funci�n.
	 */
	virtual double Eval(double x, ...);

	/**
     * Calcula la derivada n-�sima de la funci�n para los valores de las variables dados.
	 * @param n el orden de la derivada.
	 * @return El valor de la derivada.
	 */
	virtual double DEval(int n, ...);

	/**
     * Devuelve el n�mero de par�metros.
	 * @return El n�mero de par�metros.
	 */
    MOuint GetParameterCount();
	/**
     * Devuelve el nombre del par�metro que corresponde al �ndice dado.
	 * @param i el �ndice del par�metro.
	 * @return El nombre del par�metro.
	 */
    moText& GetParameterName(int i);
	/**
     * Devuelve el valor del par�metro que corresponde al �ndice dado.
	 * @param i el �ndice del par�metro.
	 * @return El valor del par�metro.
	 */
    double GetParameterValue(int i);

	/**
     * Devuelve el n�mero de variables.
	 * @return El n�mero de variables.
	 */
	MOuint GetVariableCount();
	/**
     * Devuelve el nombre de la variable que corresponde al �ndice dado.
	 * @param i el �ndice de la variable.
	 * @return El nombre de la variable.
	 */
    moText& GetVariableName(int i);
	/**
     * Devuelve el valor actual de la variable que corresponde al �ndice dado.
	 * @param i el �ndice de la variable.
	 * @return El valor actual de la variable.
	 */
    double GetVariableValue(int i);

	/**
     * Calcula la funci�n para los valores de las variables dados.
	 * @param x el valor de la primer variable.
	 * @return El valor de la funci�n.
	 */
	double operator () (double x, ...);
protected:
	/**
     * Almacena una cadena de texto vac�a para devolver cuando se intenta acceder un �ndice de par�metro
	 * o variable inv�lido.
	 */
	moText m_EmptyName;
	/**
     * La expresi�n matem�tica que define la funci�n.
	 */
	moText m_Expression;
	/**
     * Lista de par�metros.
	 */
	moMathVariableArray m_Parameters;
	/**
     * Lista de variables.
	 */
	moMathVariableArray m_Variables;

	double m_LastEval;

	/**
     * Funci�n abstracta pura que es llamada desde la funci�n 'SetParameters'.
	 * Debe implementar los c�lculos que actualizan la funci�n dado un nuevo conjunto de par�metros.
	 * @see SetParameters
	 */
	virtual void OnParamUpdate() = 0;
	/**
     * Funci�n abstracta pura que es llamada desde la funci�n 'Eval'.
	 * Debe implementar el c�lculo de la funci�n dados los valores actuales de las variables.
	 * @return El valor de la funci�n
	 * @see Eval
	 */
	virtual double OnFuncEval() = 0;
	/**
     * Funci�n abstracta pura que es llamada desde la funci�n 'DEval'.
	 * Debe implementar el c�lculo de la derivada n-�sima de la funci�n dados los valores actuales de las variables.
	 * @return El valor de la funci�n
	 * @see DEval
	 */
	virtual double OnDerivEval(int n) = 0;

	/**
     * Funci�n abstracta pura que es llamada desde la funci�n 'Init'.
	 * Debe implemtar la construcci�n de la lista de par�metros.
	 * @see Init
	 */
	virtual void BuildParamList() = 0;
	/**
     * Funci�n abstracta pura que es llamada desde la funci�n 'Init'.
	 * Debe implemtar la construcci�n de la lista de variables.
	 * @see Init
	 */
	virtual void BuildVarList() = 0;

    moConfig*   m_pConfig;
    moMoldeoObject* m_pMOB;
};

typedef moMathFunction* MathFunctionPtr;
moDeclareExportedDynamicArray( MathFunctionPtr, moMathFunctionArray );

/*******************************************************************************
                     Algunas funciones matem�ticas pre-defindas
*******************************************************************************/

/**
 * Clase que encapsula un polinomio interpolante de grado 3. Esta funci�n pasa
 * por los puntos (x0, y0) y (x1, y1) de manera tal que las primera derivadas
 * en x0 y x1 sean d0 y d1, respectivamente.
 * El ordenamiento de los par�metros es x0, y0, d0, x1, y1, d1.
 */
class LIBMOLDEO_API moCubicInterpolant : public moMathFunction
{
public:
	/**
     * Funci�n de inicializacion en donde se asigna la expresi�n que caracteriza la funci�n y se
	 * construyen las listas de par�metros y variables.
	 * @param p_Expression la expresi�n matem�tica.
	 * @return El resultado de la operaci�n de inicializaci�n.
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
 * (x1, y1), las primera derivadas en x0 y x1 son d0 y d1, respectivamente, y adem�s
 * reproduce la convexidad/concavidad de los puntos. El par�metro gamma en (0, 3)
 * incrementa la suavidad de la curva a medida que se acerca a 3.
 * El ordenamiento de los par�metros es x0, y0, d0, x1, y1, d1, gamma.
 * Para mas informaci�n, ver la siguiente referencia:
 * "A Practical Guide to Splines", Carl de Boor, Springer, chapter XVI.
 */
class LIBMOLDEO_API moTautInterpolant : public moMathFunction
{
public:
	/**
     * Funci�n de inicializacion en donde se asigna la expresi�n que caracteriza la funci�n y se
	 * construyen las listas de par�metros y variables.
	 * @param p_Expression la expresi�n matem�tica.
	 * @return El resultado de la operaci�n de inicializaci�n.
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
 * Clase de creaci�n de variables. Las variables son clasificadas en dos tipos:
 * variables propiamente dichas, como t en la expresi�n sin(t), y par�metrso,
 * que se identifican con el prefijo "_": sin(_w * t). Esta distinci�n, que para
 * muParser es totalmente arbitraria, permite distingir entre valores que son
 * actualizados con mucha frecuencia (variables) y aquellos que solo cambian
 * unas pocas veces (par�metros).
 */
class LIBMOLDEO_API moMathVariableFactory : public moAbstract
{
public:
	/**
     * Constructor donde se especifican las listas de par�metros y variables.
	 * @param p_pParArray puntero a la lista de variables.
	 * @param p_pParArray puntero a la lista de par�metros.
	 */
	moMathVariableFactory(moMathVariableArray* p_pParArray, moMathVariableArray* p_pVarArray)
	{
		m_pParArray = p_pParArray;
		m_pVarArray = p_pVarArray;
	}

	/**
     * Agrega una nueva variable o par�metro (uno u otro dependiendo de la presencia
	 * del prefijo "_" en el nombre.
	 * @param p_pNewName nombre de la variable.
	 * @return puntero double donde se almacena el valor de la variable.
	 */
    double* CreateNewVariable(const char *p_pNewName)
	{
		moMathVariable* pvar = new moMathVariable(p_pNewName, 0.0);

		if (p_pNewName != NULL) {
			if (p_pNewName[0] == '_') m_pParArray->Add(pvar); // Agregando par�metro.
			else m_pVarArray->Add(pvar);                      // Agregando variable.
		}
        return pvar->GetValuePointer();
	}
private:
	moMathVariableArray* m_pParArray;
	moMathVariableArray* m_pVarArray;
};

/**
 * Funci�n de creaci�n de variables, utilizada por muParser.
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
     * Funci�n de inicializacion en donde se asigna la expresi�n que caracteriza la funci�n y se
	 * construyen las listas de par�metros y variables.
	 * @param p_Expression la expresi�n matem�tica.
	 * @return El resultado de la operaci�n de inicializaci�n.
	 */
	virtual MOboolean Init(const moText& p_Expression, moMoldeoObject* p_pMOB = NULL );
	/**
     * Funci�n de finalizaci�n.
	 * @return El resultado de la operaci�n de finalizaci�n.
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
