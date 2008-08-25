/*******************************************************************************

                                 moP5.h

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
  Andr�s Colubri

*******************************************************************************/

#ifndef __MO_P5_H__
#define __MO_P5_H__

#include "moAbstract.h"

/**
 * Clase que encapsula la API de Processing. Las funciones est�n implementadas
 * con OpenGL.
 */
class LIBMOLDEO_API moP5 : public moAbstract
{
public:
	moP5();
	
	/**
     * Un tri�ngulo es un plano creado al conectar tres puntos. Los dos primeros argumentos 
	 * especifican el primer punto, los dos argumentos centrales especifican el segundo punto,
	 * y los dos �ltimos argumentos especifican el tercer punto.
	 * @param x1 coordenada x del primer punto.
	 * @param y1 coordenada y del primer punto.
	 * @param x2 coordenada x del segundo punto.
	 * @param y2 coordenada y del segundo punto.
	 * @param x3 coordenada x del tercer punto.
	 * @param y3 coordenada y del tercer punto.
	 */
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3);

	/**
	 * Dibuja una l�nea (el recorrido m�s directo entre dos puntos) en la pantalla. 
	 * Esta versi�n de line() con cuatro par�metros dibuja la l�nea en 2D.
	 * Para colorear una l�nea, usar la funci�n stroke(). Una l�nea no puede ser rellenada, por lo
	 * tanto el m�todo fill() no afecta el color de una l�nea. L�neas 2D son dibujadas con un ancho
	 * de un p�xel por defecto, pero esto puede ser modificado con la funci�n strokeWeight().
	 * @param x1 coordenada x del primer punto.
	 * @param y1 coordenada y del primer punto.
	 * @param x2 coordenada x del segundo punto.
	 * @param y2 coordenada y del segundo punto.
	 * @see stroke()
	 * @see strokeWeight()
	 */
	void line(float x1, float y1, float x2, float y2);

	/**
	 * Esta es la versi�n de 6 par�metros de la funci�n line() que permite colocar una l�nea
	 * en cualquier lugar del espacio XYZ.
	 * @param x1 coordenada x del primer punto.
	 * @param y1 coordenada y del primer punto.
	 * @param z1 coordenada z del primer punto.
	 * @param x2 coordenada x del segundo punto.
	 * @param y2 coordenada y del segundo punto.
	 * @param z2 coordenada z del segundo punto.
	 */
	void line(float x1, float y1, float z1, float x2, float y2, float z2);

	/**
	 * Dibuja un arco en la pantalla. Los arcos son dibujados a lo largo del borde exterior de una elipse
	 * definida por x, y, width y height. El origen de la elipse del arco puede ser cambiada con la
	 * funci�n ellipseMode(). Los par�metros start y stop especifican los �ngulos donde dibujar el arco.
	 * @param x coordenada x de la elipse del arco.
	 * @param y coordenada y de la elipse del arco.
	 * @param width ancho de la elipse del arco.
	 * @param height altura de la elipse del arco.
	 * @param start �ngulo donde comenzar el arco, especificado en radianes o grados dependiendo del modo de �ngulo actual.
	 * @param stop �ngulo donde detener el arco, especificado en radianes o grados dependiendo del modo de �ngulo actual.
	 */
	void arc(float x, float y, float width, float height, float start, float stop);

	/**
	 * Dibuja un punto, una coordenada en el espacio de las dimensiones de un p�xel. El primer par�metro
	 * es el valor horizontal del punto y el segundo valor es el valor vertical del punto.
	 * @param x coordenada x del punto.
	 * @param y coordenada y del punto.
	 */
	void point(float x, float y);

	/**
	 * Versi�n 3D de la funci�n point(), donde el tercer valor corresponde al valor de profundidad.
	 * @param x coordenada x del punto.
	 * @param y coordenada y del punto.
	 * @param z coordenada z del punto.
	 */
	void point(float x, float y, float z);

	/**
	 * Un quad es un cuadril�tero, un pol�gono de 4 lados. Es similar a un rect�ngulo, pero los
	 * �ngulos entre sus bordes no estan restringidos a 90 grados. El primer par de par�metros (x1,y1)
	 * define el primer v�rtice, y los pares subsiguientes deben ser procesados en la direcci�n de las
	 * agujas del reloj o bien en la direcci�n opuesta, alrededor de la figura especificada.
	 * @param x1 coordenada x del primer v�rtice.
	 * @param y1 coordenada y del primer v�rtice.
	 * @param x2 coordenada x del segundo v�rtice.
	 * @param y2 coordenada y del segundo v�rtice.
	 * @param x3 coordenada x del tercer v�rtice.
	 * @param y3 coordenada y del tercer v�rtice.
	 * @param x4 coordenada x del cuarto v�rtice.
	 * @param y4 coordenada y del cuarto v�rtice.
	 */
	void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

	/**
	 * Dibuja una elipse (�valo) en la pantalla. Una elipse con su ancho igual a su alto es un c�rculo.
	 * Los dos par�metros iniciales determinan la ubicaci�n, el tercero determina el ancho y el cuarto
	 * determina el alto. El origen puede ser cambiado con la funci�n ellipseMode().
	 * @param x coordenada x de la elipse.
	 * @param y coordenada y de la elipse.
	 * @param width ancho de la elipse.
	 * @param height alto de la elipse.
	 * @see ellipseMode()
	 */
	void ellipse(float x, float y, float width, float height);

	/**
	 * Dibuja un rect�ngulo en la pantalla. Un rect�ngulo es un figura de cuatro lados en la cual cada �ngulo
	 * es de 90 grados. Los dos primeros par�metros determinan la ubicaci�n, el tercero determina el ancho
	 * y el cuarto determina la altura. El or�gen es cambiado con la funci�n rectMode().
	 * @param x coordenada x del rect�ngulo.
	 * @param y coordenada y del rect�ngulo.
	 * @param width ancho del rect�ngulo.
	 * @param height altura del rect�ngulo.
	 * @see rectMode()
	 */
	void rect(float x, float y, float width, float height);

	/**
	 * Setea el ancho del trazo utilizado para l�neas, puntos y el borde alrededor de las figuras. Todos los anchos
	 * son especificados en unidades de p�xel. 
	 * @param width el ancho (en p�xeles) del trazo.
	 */
	void strokeWeight(float width);

/*
	//Draws all geometry with smooth (anti-aliased) edges. This will slow down the frame rate of the application, but will enhance the visual refinement.
	void smooth();

	//Draws all geometry with jagged (aliased) edges.
	void noSmooth();

	//Sets the style of the joints which connect line segments. These joints are either mitered, beveled, or rounded and specified with the corresponding parameters MITER, BEVEL, and ROUND. The default joint is MITER.
	void strokeJoin(moP5StrokeJoinMode MODE);

	//The origin of the ellipse is modified by the ellipseMode() function. The default configuration is ellipseMode(CENTER), which specifies the location of the ellipse as the center of the shape. The RADIUS mode is the same, but the width and height parameters to ellipse() specify the radius of the ellipse, rather than the diameter. The CORNER mode draws the shape from the upper-left corner of its bounding box. The CORNERS mode uses the four parameters to ellipse() to set two opposing corners of the ellipse's bounding box. 
	void ellipseMode(moP5ShapeMode MODE);

	//Modifies the location from which rectangles draw. The default mode is rectMode(CORNER), which specifies the location to be the upper left corner of the shape and uses the third and fourth parameters of rect() to specify the width and height. The syntax rectMode(CORNERS) uses the first and second parameters of rect() to set the location of one corner and uses the third and fourth parameters to set the opposite corner. The syntax rectMode(CENTER) draws the image from its center point and uses the third and forth parameters of rect() to specify the image's width and height. The syntax rectMode(RADIUS) draws the image from its center point and uses the third and forth parameters of rect() to specify half of the image's width and height. 
	void rectMode(moP5ShapeMode MODE);

	//Sets the style for rendering line endings. These ends are either squared, extended, or rounded and specified with the corresponding parameters SQUARE, PROJECT, and ROUND. The default cap is ROUND.
	void strokeCap(moP5StrokeCapMode MODE);
*/

	/**
	 * La funci�n background() determina el color de fondo utilizado por el objeto moP5. El color por defecto es gris claro.
	 * @param gray especifica un valor entre blanco y negro.
	 * @param alpha opacidad del fondo.
	 * @param value1 Componente rojo o tinte (dependiendo en el modo de color actual).
	 * @param value2 Componente verde o saturaci�n (dependiendo en el modo de color actual).
	 * @param value3 Componente azul o brillo (dependiendo en el modo de color actual).
	 */
	void background(float gray);
	void background(float gray, float alpha);
	void background(float value1, float value2, float value3);
	void background(float value1, float value2, float value3, float alpha);

	/**
	 * Cambia la manera en la que se interpretan los datos de color. Por defecto, los colores de fill(), stroke() y
	 * background() est�n seteados en valores entre 0 y 255 utilizando el modelo de color RGB.
	 * @param mode Es MO_P5_RGB o MO_P5_HSB, correspondiendo a Red/Green/Blue (Rojo/Verde/Azul) y Hue/Saturation/Brightness (Tinte/Saturaci�n/Brillo).
	 * @param range Rango para todos los elementos de color.
	 * @param range1 Rango para el componente rojo o tinte dependiendo del modo seleccionado.
	 * @param range2 Rango para el componente verde o saturaci�n dependiendo del modo seleccionado.
	 * @param range3 Rango para el componente azul o brillo dependiendo del modo seleccionado.
	 * @param range4 Rango para el componente alpha (transparencia).
	 * @see fill() 
	 * @see stroke()
	 * @see background()
	 */
	void colorMode(int mode);
	void colorMode(int mode, float range);
	void colorMode(int mode, float range1, float range2, float range3);
	void colorMode(int mode, float range1, float range2, float range3, float range4);

	/**
	 * Sets the color used to draw lines and borders around shapes. This color is either specified in terms of the RGB or HSB color depending on the current colorMode() (the default color space is RGB, with each value in the range from 0 to 255). 
	 * @param gray especifica un valor entre blanco y negro.
	 * @param alpha opacidad del trazo.
	 * @param value1 valor de rojo o tinte (dependiendo del modo de color actual).
	 * @param value2 valor de verde o saturaci�n (dependiendo del modo de color actual).
	 * @param value3 valor de azul o brillo (dependiendo del modo de color actual).
	 */
	void stroke(float gray);
	void stroke(float gray, float alpha);
	void stroke(float value1, float value2, float value3);
	void stroke(float value1, float value2, float value3, float alpha);

	/**
	 * Deshabilita el llenado de la geometr�a. Si noStroke() y noFill() son llamados, nada ser� dibujado en la pantalla.
	 * @see noStroke()
	 */
	void noFill();

	/**
	 * Deshabilita el dibujo del trazo (borde exterior). Si noStroke() y noFill() son llamados, nada ser� dibujado en la pantalla.
	 * @see noFill()
	 */
	void noStroke();

	/**
	 * Setea el color utilizado para rellenar figuras. Por ejemplo, si la funci�n fill(204, 102, 0) es llamada, todas las figuras subsequentes
	 * ser�n rellenadas con el color naranja. Este color es especificado en t�rminos de los modos de color RGB o HSB, dependiendo del colorMode()
	 * actual (el espacio de color por defecto es RGB, con cada valor en el rango de 0 a 255).
	 * @param gray n�mero especificando valor entre blanco y negro.
	 * @param alpha opacidad del rellenado
	 * @param value1 Valor de rojo o tinte.
	 * @param value2 Valor de verde o saturaci�n.
	 * @param value3 Valor de azul o brillo.
	 * @see colorMode()
	 */
	void fill(float gray);
	void fill(float gray, float alpha);
	void fill(float value1, float value2, float value3);
	void fill(float value1, float value2, float value3, float alpha);

	/**
	 * Agrega la matriz de transformaciones actual sobre la pila de matrices. Entender pushMatrix() y popMatrix() 
	 * requiere entender el concepto de pila de matrices. La funci�n pushMatrix() salva el sistema coordenadas actual
	 * en la pila y popMatrix() restaura el sistema de coordenadas anterior. pushMatrix() and popMatrix() son utilizados
	 * en conjunto con ls otros m�todos de transformaci�n y pueden ser anidados para controlar el alcance de las transformaciones.
	 * @see popMatrix()
	 */
	void pushMatrix();

	/**
	 * Retira la matriz de transformaciones actual de la pila de matrices. Entender pushMatrix() y popMatrix() 
	 * requiere entender el concepto de pila de matrices. La funci�n pushMatrix() salva el sistema coordenadas actual
	 * en la pila y popMatrix() restaura el sistema de coordenadas anterior. pushMatrix() and popMatrix() son utilizados
	 * en conjunto con ls otros m�todos de transformaci�n y pueden ser anidados para controlar el alcance de las transformaciones.
	 * @see pushMatrix()
	 */
	void popMatrix();

	/**
	 * Reemplaza la matriz actual con la matriz identidad. La funci�n equivalente en OpenGL es glLoadIdentity().
	 */
	void resetMatrix();

	/**
	 * Aumenta y disminuy el tama�o de un objeto expandiendo y contrayendo v�rtices. Valores de escala son especificados
	 * como porcentajes decimales. El llamado scale(2.0) aumenta las dimensiones de una figura en un 200%.
	 * Los objetos siempre son escalados desde su or�gen relativo al or�gen de coordenadas. Las transformaciones se aplican
	 * a todo lo que ocurre despues y las llamadas subsequentes a esta funci�n multiplican el efecto. Por ejemplo, llamando scale(2.0) 
	 * y luego scale(1.5) es equivalente a scale(3.0). Esta funci�n puede ser controlada adicionalmente con pushMatrix() y popMatrix().
	 * @param size 	porcentaje a escalar el objeto.
	 * @param x porcentaje a escalar el objeto a lo largo del eje "x".
	 * @param y porcentaje a escalar el objeto a lo largo del eje "y".
	 * @param z porcentaje a escalar el objeto a lo largo del eje "z".
	 * @see pushMatrix()
	 * @see popMatrix()
	 */
	void scale(float size);
	void scale(float x, float y);
	void scale(float x, float y, float z);

	/**
	 * Especifica una cantidad a desplazar los objetos dentro de la pantalla. El par�metro x especifica translaci�n de izquierda
	 * a derecha, el par�metro y especifica translaci�n de arriba hacia abajo, y par�metro z especifica translaciones 
	 * hacia y desde la pantalla. Las transformaciones se aplican a todo lo que ocurre despues y llamadas subsiguientes
	 * a esta funci�n acumlan el efecto. Por ejemplo, llamar translate(50, 0) y luego translate(20, 0) es equivalente a 
	 * translate(70, 0). Esta funci�n puede ser controlada adicionalmente con pushMatrix() y popMatrix().
	 * @param x translaci�n izquierda/derecha.
	 * @param y translaci�n arriba/abajo.
	 * @param z translaci�n adelante/atr�s.
	 * @see pushMatrix()
	 * @see popMatrix()
	 */
	void translate(float x, float y);
	void translate(float x, float y, float z);

	/**
	 * Gira un objeto en la cantidad especificada por el par�mero de �ngulo. Los �ngulos deben ser especificados 
	 * en radianes (valores de 0 to PI*2) o convertidos a radianes con la funci�n radians(). Los objetos son siempre
	 * rotados alrededor de su posici�n relativa al org�gen y n�meros positivos giran los objetos en la direcci�n horaria.
	 * Las transformaciones se aplican a todo lo que ocurre despu�s y llamadas subsequentes a la funci�n acumulan 
	 * el efecto. Por ejemplo, llamar rotate(PI/2) y luego rotate(PI/2) es lo mismo que rotate(PI). T�cnicamente, rotate()
	 * multiplica la matriz de transformaci�n acutal por una matriz de rotaci�n. Esta funci�n puede ser controlada adicionalmente
	 * con pushMatrix() y popMatrix().
	 * @param angle �ngulo de rotaci�n especificado en radianes.
	 * @see pushMatrix()
	 * @see popMatrix()
	 */
	void rotate(float angle);

	enum moP5StrokeJoinMode 
	{
		MO_P5_MITER = 0,
		MO_P5_BEVEL = 1,
		MO_P5_ROUND = 2
	};

	enum moP5ShapeMode
	{
		MO_P5_CENTER = 0, 
		MO_P5_RADIUS = 1, 
		MO_P5_CORNER = 2, 
		MO_P5_CORNERS = 3
	};

	enum moP5StrokeCapMode
	{
		MO_P5_SQUARE = 0,
		MO_P5_PROJECT = 1, 
	};

	enum moP5ColorMode
	{
		MO_P5_RGB = 0,
		MO_P5_HSB = 1
	};

	static const float MO_P5_HALF_PI;
	static const float MO_P5_TWO_PI;
	static const float MO_P5_PI;
protected:
	int currColorMode;
	float colorRGBCoeff[4];
	float colorHSBCoeff[4];

	float tmpColor[4];
	float strokeColor[4];
	float fillColor[4];

	void generateTmpColor(float comp1, float comp2, float comp3);
	void generateTmpColor(float comp1, float comp2, float comp3, float comp4);
	void convertHSLtoRGB(float h, float s, float l, float& r, float& g, float& b);
	float HuetoRGB(float m1, float m2, float h);
};



#endif
