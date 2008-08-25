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
  Andrés Colubri

*******************************************************************************/

#ifndef __MO_P5_H__
#define __MO_P5_H__

#include "moAbstract.h"

/**
 * Clase que encapsula la API de Processing. Las funciones están implementadas
 * con OpenGL.
 */
class LIBMOLDEO_API moP5 : public moAbstract
{
public:
	moP5();
	
	/**
     * Un triángulo es un plano creado al conectar tres puntos. Los dos primeros argumentos 
	 * especifican el primer punto, los dos argumentos centrales especifican el segundo punto,
	 * y los dos últimos argumentos especifican el tercer punto.
	 * @param x1 coordenada x del primer punto.
	 * @param y1 coordenada y del primer punto.
	 * @param x2 coordenada x del segundo punto.
	 * @param y2 coordenada y del segundo punto.
	 * @param x3 coordenada x del tercer punto.
	 * @param y3 coordenada y del tercer punto.
	 */
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3);

	/**
	 * Dibuja una línea (el recorrido más directo entre dos puntos) en la pantalla. 
	 * Esta versión de line() con cuatro parámetros dibuja la línea en 2D.
	 * Para colorear una línea, usar la función stroke(). Una línea no puede ser rellenada, por lo
	 * tanto el método fill() no afecta el color de una línea. Líneas 2D son dibujadas con un ancho
	 * de un píxel por defecto, pero esto puede ser modificado con la función strokeWeight().
	 * @param x1 coordenada x del primer punto.
	 * @param y1 coordenada y del primer punto.
	 * @param x2 coordenada x del segundo punto.
	 * @param y2 coordenada y del segundo punto.
	 * @see stroke()
	 * @see strokeWeight()
	 */
	void line(float x1, float y1, float x2, float y2);

	/**
	 * Esta es la versión de 6 parámetros de la función line() que permite colocar una línea
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
	 * función ellipseMode(). Los parámetros start y stop especifican los ángulos donde dibujar el arco.
	 * @param x coordenada x de la elipse del arco.
	 * @param y coordenada y de la elipse del arco.
	 * @param width ancho de la elipse del arco.
	 * @param height altura de la elipse del arco.
	 * @param start ángulo donde comenzar el arco, especificado en radianes o grados dependiendo del modo de ángulo actual.
	 * @param stop ángulo donde detener el arco, especificado en radianes o grados dependiendo del modo de ángulo actual.
	 */
	void arc(float x, float y, float width, float height, float start, float stop);

	/**
	 * Dibuja un punto, una coordenada en el espacio de las dimensiones de un píxel. El primer parámetro
	 * es el valor horizontal del punto y el segundo valor es el valor vertical del punto.
	 * @param x coordenada x del punto.
	 * @param y coordenada y del punto.
	 */
	void point(float x, float y);

	/**
	 * Versión 3D de la función point(), donde el tercer valor corresponde al valor de profundidad.
	 * @param x coordenada x del punto.
	 * @param y coordenada y del punto.
	 * @param z coordenada z del punto.
	 */
	void point(float x, float y, float z);

	/**
	 * Un quad es un cuadrilátero, un polígono de 4 lados. Es similar a un rectángulo, pero los
	 * ángulos entre sus bordes no estan restringidos a 90 grados. El primer par de parámetros (x1,y1)
	 * define el primer vértice, y los pares subsiguientes deben ser procesados en la dirección de las
	 * agujas del reloj o bien en la dirección opuesta, alrededor de la figura especificada.
	 * @param x1 coordenada x del primer vértice.
	 * @param y1 coordenada y del primer vértice.
	 * @param x2 coordenada x del segundo vértice.
	 * @param y2 coordenada y del segundo vértice.
	 * @param x3 coordenada x del tercer vértice.
	 * @param y3 coordenada y del tercer vértice.
	 * @param x4 coordenada x del cuarto vértice.
	 * @param y4 coordenada y del cuarto vértice.
	 */
	void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

	/**
	 * Dibuja una elipse (óvalo) en la pantalla. Una elipse con su ancho igual a su alto es un círculo.
	 * Los dos parámetros iniciales determinan la ubicación, el tercero determina el ancho y el cuarto
	 * determina el alto. El origen puede ser cambiado con la función ellipseMode().
	 * @param x coordenada x de la elipse.
	 * @param y coordenada y de la elipse.
	 * @param width ancho de la elipse.
	 * @param height alto de la elipse.
	 * @see ellipseMode()
	 */
	void ellipse(float x, float y, float width, float height);

	/**
	 * Dibuja un rectángulo en la pantalla. Un rectángulo es un figura de cuatro lados en la cual cada ángulo
	 * es de 90 grados. Los dos primeros parámetros determinan la ubicación, el tercero determina el ancho
	 * y el cuarto determina la altura. El orígen es cambiado con la función rectMode().
	 * @param x coordenada x del rectángulo.
	 * @param y coordenada y del rectángulo.
	 * @param width ancho del rectángulo.
	 * @param height altura del rectángulo.
	 * @see rectMode()
	 */
	void rect(float x, float y, float width, float height);

	/**
	 * Setea el ancho del trazo utilizado para líneas, puntos y el borde alrededor de las figuras. Todos los anchos
	 * son especificados en unidades de píxel. 
	 * @param width el ancho (en píxeles) del trazo.
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
	 * La función background() determina el color de fondo utilizado por el objeto moP5. El color por defecto es gris claro.
	 * @param gray especifica un valor entre blanco y negro.
	 * @param alpha opacidad del fondo.
	 * @param value1 Componente rojo o tinte (dependiendo en el modo de color actual).
	 * @param value2 Componente verde o saturación (dependiendo en el modo de color actual).
	 * @param value3 Componente azul o brillo (dependiendo en el modo de color actual).
	 */
	void background(float gray);
	void background(float gray, float alpha);
	void background(float value1, float value2, float value3);
	void background(float value1, float value2, float value3, float alpha);

	/**
	 * Cambia la manera en la que se interpretan los datos de color. Por defecto, los colores de fill(), stroke() y
	 * background() están seteados en valores entre 0 y 255 utilizando el modelo de color RGB.
	 * @param mode Es MO_P5_RGB o MO_P5_HSB, correspondiendo a Red/Green/Blue (Rojo/Verde/Azul) y Hue/Saturation/Brightness (Tinte/Saturación/Brillo).
	 * @param range Rango para todos los elementos de color.
	 * @param range1 Rango para el componente rojo o tinte dependiendo del modo seleccionado.
	 * @param range2 Rango para el componente verde o saturación dependiendo del modo seleccionado.
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
	 * @param value2 valor de verde o saturación (dependiendo del modo de color actual).
	 * @param value3 valor de azul o brillo (dependiendo del modo de color actual).
	 */
	void stroke(float gray);
	void stroke(float gray, float alpha);
	void stroke(float value1, float value2, float value3);
	void stroke(float value1, float value2, float value3, float alpha);

	/**
	 * Deshabilita el llenado de la geometría. Si noStroke() y noFill() son llamados, nada será dibujado en la pantalla.
	 * @see noStroke()
	 */
	void noFill();

	/**
	 * Deshabilita el dibujo del trazo (borde exterior). Si noStroke() y noFill() son llamados, nada será dibujado en la pantalla.
	 * @see noFill()
	 */
	void noStroke();

	/**
	 * Setea el color utilizado para rellenar figuras. Por ejemplo, si la función fill(204, 102, 0) es llamada, todas las figuras subsequentes
	 * serán rellenadas con el color naranja. Este color es especificado en términos de los modos de color RGB o HSB, dependiendo del colorMode()
	 * actual (el espacio de color por defecto es RGB, con cada valor en el rango de 0 a 255).
	 * @param gray número especificando valor entre blanco y negro.
	 * @param alpha opacidad del rellenado
	 * @param value1 Valor de rojo o tinte.
	 * @param value2 Valor de verde o saturación.
	 * @param value3 Valor de azul o brillo.
	 * @see colorMode()
	 */
	void fill(float gray);
	void fill(float gray, float alpha);
	void fill(float value1, float value2, float value3);
	void fill(float value1, float value2, float value3, float alpha);

	/**
	 * Agrega la matriz de transformaciones actual sobre la pila de matrices. Entender pushMatrix() y popMatrix() 
	 * requiere entender el concepto de pila de matrices. La función pushMatrix() salva el sistema coordenadas actual
	 * en la pila y popMatrix() restaura el sistema de coordenadas anterior. pushMatrix() and popMatrix() son utilizados
	 * en conjunto con ls otros métodos de transformación y pueden ser anidados para controlar el alcance de las transformaciones.
	 * @see popMatrix()
	 */
	void pushMatrix();

	/**
	 * Retira la matriz de transformaciones actual de la pila de matrices. Entender pushMatrix() y popMatrix() 
	 * requiere entender el concepto de pila de matrices. La función pushMatrix() salva el sistema coordenadas actual
	 * en la pila y popMatrix() restaura el sistema de coordenadas anterior. pushMatrix() and popMatrix() son utilizados
	 * en conjunto con ls otros métodos de transformación y pueden ser anidados para controlar el alcance de las transformaciones.
	 * @see pushMatrix()
	 */
	void popMatrix();

	/**
	 * Reemplaza la matriz actual con la matriz identidad. La función equivalente en OpenGL es glLoadIdentity().
	 */
	void resetMatrix();

	/**
	 * Aumenta y disminuy el tamaño de un objeto expandiendo y contrayendo vértices. Valores de escala son especificados
	 * como porcentajes decimales. El llamado scale(2.0) aumenta las dimensiones de una figura en un 200%.
	 * Los objetos siempre son escalados desde su orígen relativo al orígen de coordenadas. Las transformaciones se aplican
	 * a todo lo que ocurre despues y las llamadas subsequentes a esta función multiplican el efecto. Por ejemplo, llamando scale(2.0) 
	 * y luego scale(1.5) es equivalente a scale(3.0). Esta función puede ser controlada adicionalmente con pushMatrix() y popMatrix().
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
	 * Especifica una cantidad a desplazar los objetos dentro de la pantalla. El parámetro x especifica translación de izquierda
	 * a derecha, el parámetro y especifica translación de arriba hacia abajo, y parámetro z especifica translaciones 
	 * hacia y desde la pantalla. Las transformaciones se aplican a todo lo que ocurre despues y llamadas subsiguientes
	 * a esta función acumlan el efecto. Por ejemplo, llamar translate(50, 0) y luego translate(20, 0) es equivalente a 
	 * translate(70, 0). Esta función puede ser controlada adicionalmente con pushMatrix() y popMatrix().
	 * @param x translación izquierda/derecha.
	 * @param y translación arriba/abajo.
	 * @param z translación adelante/atrás.
	 * @see pushMatrix()
	 * @see popMatrix()
	 */
	void translate(float x, float y);
	void translate(float x, float y, float z);

	/**
	 * Gira un objeto en la cantidad especificada por el parámero de ángulo. Los ángulos deben ser especificados 
	 * en radianes (valores de 0 to PI*2) o convertidos a radianes con la función radians(). Los objetos son siempre
	 * rotados alrededor de su posición relativa al orgígen y números positivos giran los objetos en la dirección horaria.
	 * Las transformaciones se aplican a todo lo que ocurre después y llamadas subsequentes a la función acumulan 
	 * el efecto. Por ejemplo, llamar rotate(PI/2) y luego rotate(PI/2) es lo mismo que rotate(PI). Técnicamente, rotate()
	 * multiplica la matriz de transformación acutal por una matriz de rotación. Esta función puede ser controlada adicionalmente
	 * con pushMatrix() y popMatrix().
	 * @param angle ángulo de rotación especificado en radianes.
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
