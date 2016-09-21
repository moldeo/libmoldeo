/*******************************************************************************

                                 moShader.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *   (at your option) any later version.                                    *
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

  Copyright (C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa
  Andres Colubri

  Description:
  Base class for GLSL and Cg shaders.

*******************************************************************************/

#ifndef __MO_SHADER_H__
#define __MO_SHADER_H__

#include "moTypes.h"
#include "moAbstract.h"
#include "moConfig.h"
#include "moArray.h"

#define MO_MAX_TEXTURE_UNITS 4

typedef enum { MO_SHADER_GLSL, MO_SHADER_CG } moShaderLanguage;

/**
 * Esta clase define una grilla 2D donde mapear y distorsionar texturas. La grilla
 * está definida por el número de divisiones (número de puntos - 1) a lo largo de las direcciones X e Y.
 * En principio, el delta en el espacio de coordenadas de texturas entre un punto de la grilla y el
 * siguiente es igual a 1 / divisiones, pero este delta puede ser modificado para que adopte un valor
 * arbitrario dx, dy a lo largo de cada dirección. Asimismo, como se pueden aplicar texturas múltiples
 * sobre un a misma grilla, de pueden definir varios deltas (dx_i, dy_i) con i = 1... n, donde n es el
 * número de texturas (o capas) que se aplican sobre la grilla.
 * Por abuso de terminología, la palabra shader denota a veces el programa de shader completo (incluyendo
 * los estadios de vértices, geometría y fragmentos), mientras que otras veces denota solamente a un estadio
 * particular (shader de vértices, shader de fragmentos, etc.).
 */
class LIBMOLDEO_API moTexturedGrid : public moAbstract
{
public:
    /**
     * El constructor por defecto de la clase.
     */
	moTexturedGrid();
    /**
     * El destructor por defecto de la clase.
     */
	virtual ~moTexturedGrid();

    /**
     * Método de inicialización de la grilla.
     * @param p_size_x número de puntos en la dirección X.
     * @param p_size_y número de puntos en la dirección Y.
     * @param p_num_layers número de capas.
     * @param p_grid_dx deltas para las texturas en la dirección de X.
     * @param p_grid_dy deltas para las texturas  en la dirección de Y.
     * @return true si la operación fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(MOint p_size_x, MOint p_size_y, MOint p_num_layers, const MOfloat p_grid_dx[], const MOfloat p_grid_dy[]);
    /**
     * Método de inicialización.
     * @param p_cfg puntero al objeto de configuración que contiene los parámetros de la grilla.
     * @param p_param_idx índice de los parámetros de la grilla en el objeto de configuración.
     * @return true si la operación fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(moConfig* p_cfg, MOuint p_param_idx);
    /**
     * Método de finalización.
     * @return true si la operación fue exitosa, false en caso contrario.
     */
	virtual MOboolean Finish();

    /**
     * Crea una grilla de 1x1 con una sola capa de textura.
     */
	void Set1QuadGrid();

    /**
     * Dibuja la grilla con ancho w y alto w, aplicando las capas hasta la número l.
     * @param w ancho con el que se dibuja la grilla.
     * @param h alto con el que se dibuja la grilla.
     * @param l número de capas de textura a aplicar.
     */
	void Draw(MOint w, MOint h, MOint l);

    /**
     * Devuelve el número de puntos en la dirección X.
     * @return número de puntos.
     */
	MOint GetWidth() { return m_size_x; }
    /**
     * Devuelve el número de puntos en la dirección Y.
     * @return número de puntos.
     */
	MOint GetHeight() { return m_size_y; }
    /**
     * Devuelve el punto (i, j) en la capa i. La capa 0 corresponde a las coordenadas de la
     * grilla propiamente dicha. Las coordenadas de las capas subsiguientes son los deltas de
     * de las texturas.
     * @param layer capa de donde se requiere el punto.
     * @param i índice del punto en la dirección X.
     * @param j índice del punto en la dirección Y.
     * @param x en esta variable pasada por referencia se devuelve la coordenada X del punto.
     * @param y en esta variable pasada por referencia se devuelve la coordenada Y del punto.
     */
	void GetPoint(MOint layer, MOint i, MOint j, MOfloat &x, MOfloat &y);
    /**
     * Establece las coordenadas de texturas hasta la capa l, para el punto (i, j) de la grilla.
     * @param i índice del punto de la grilla a lo largo de la dirección X.
     * @param j índice del punto de la grilla a lo largo de la dirección Y.
     * @param l número de capas de textura a establecer las coordenadas.
     */
	void SetTexCoord(MOint i, MOint j, MOint l);

    /**
     * Copia los datos desde la grilla p_src_grid.
     * @param p_src_grid grilla fuente para la copia.
     * @return referencia a esta grilla.
     */
	moTexturedGrid &operator = (const moTexturedGrid &p_src_grid);
protected:
	// Number of points in the grid, along each direction.
	MOint m_size_x;
	MOint m_size_y;

	// The layer 0 stores the actual grid, the other layers store the texture coordinates.
	MOint m_num_layers;
	MOfloat m_grid_dx[MO_MAX_TEXTURE_UNITS + 1];
	MOfloat m_grid_dy[MO_MAX_TEXTURE_UNITS + 1];
};

/**
 * Clase base para encapsular shaders de OpenGL. Un programa de shaders tiene tres etapas:
 * el shader de vértices, donde las coordenadas 3D de los vértices son proyectadas a coordenadas homogéneas
 * de pantalla, el shader de geometría, donde nuevos vértices y conectividades entre los mismos pueden ser
 * definidos, y el shader de fragmentos, donde las primitivas geométricas, ya rasterizadas, son dibujadas
 * en la panalla u otro búfer de escritura.
 * Un programa de shader no necesita tener estas tres etapas definidas. Puede consistir sólo en un shader
 * de fragmentos, con lo cual los estadios de vértices y geometría utilizan la funcionalidad por defecto
 * ofrecida por OpenGL en el pipeline gráfico por defecto.
 */
class LIBMOLDEO_API moShader : public moAbstract
{
public:
    /**
     * El constructor por defecto de la clase.
     */
    moShader();
    /**
     * El destructor por defecto de la clase.
     */
    virtual ~moShader();

    /**
     * Método de inicialización.
     * @return true si la operación fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init();
    /**
     * Método de finalización.
     * @return true si la operación fue exitosa, false en caso contrario.
     */
	virtual MOboolean Finish();

    /**
     * Crea el shader de vértices a partir del código fuente en vert_source.
     * @param vert_source código fuente del shader de vértices.
     */
    virtual void CreateVertShader(const moText& vert_source) = 0;
    /**
     * Crea el shader de fragmentos a partir del código fuente en frag_source.
     * @param frag_source código fuente del shader de vértices.
     */
    virtual void CreateFragShader(const moText& frag_source) = 0;
    /**
     * Crea los shaders de vértices y fragmentos a partir del código fuente en vert_source y frag_source.
     * @param vert_source código fuente del shader de vértices.
     * @param frag_source código fuente del shader de vértices.
     */
    virtual void CreateShader(const moText& vert_source, const moText& frag_source) = 0;
    /**
     * Crea el shader de vértices a partir del código fuente en el archivo vert_filename.
     * @param vert_filename nombre del archivo con el código fuente del shader de vértices.
     */
    virtual void LoadVertShader(const moText& vert_filename) = 0;
    /**
     * Crea el shader de fragmentos a partir del código fuente en el archivo frag_filename.
     * @param frag_filename nombre del archivo con el código fuente del shader de fragmentos.
     */
    virtual void LoadFragShader(const moText& frag_filename) = 0;
    /**
     * Crea los shaders de vértices y fragmentos a partir del código fuente en los archivos vert_filename y
     * frag_filename.
     * @param vert_filename nombre del archivo con el código fuente del shader de vértices.
     * @param frag_filename nombre del archivo con el código fuente del shader de fragmentos.
     */
    virtual void LoadShader(const moText& vert_filename, const moText& frag_filename) = 0;

    /**
     * Activa el shader.
     */
    virtual void StartShader();
    /**
     * Desactiva el shader.
     */
    virtual void StopShader();
    /**
     * Devuelve el estado del shader.
     * @return true si el shader está activo, false en caso contrario.
     */
    virtual MOboolean ShaderActive();

    /**
     * Devuelve el código del último error ocurrido en el shader de vértices.
     * @return código de error.
     */
    MOint VertErrorCode();
    /**
     * Devuelve el código del último error ocurrido en el shader de fragmentos.
     * @return código de error.
     */
    MOint FragErrorCode();

    /**
     * Devuelve el tipo de shader: MO_SHADER_GLSL o MO_SHADER_CG.
     * @return tipo de shader.
     */
	MOuint GetType() { return m_type; }
    /**
     * Establece el tipo de shader.
     * @param p_type nuevo tipo de shader.
     */
	void SetType(MOuint p_type) { m_type = p_type; }
    /**
     * Devuelve el nombre del shader.
     * @return nombre del shader.
     */
	moText GetName() { return m_name; }
    /**
     * Establece el nombre del shader.
     * @param p_name nuevo nombre del shader.
     */
	void SetName(const moText& p_name) { m_name = p_name; }

    /**
     * Devuelve el objeto de grilla 2D utilizado por este shader.
     * @return objeto de grilla 2D.
     */
	moTexturedGrid GetGrid() { return m_grid; }
    /**
     * Establece el objeto de grilla 2D a utilizar por este shader.
     * @param m_grid nuevo objeto de grilla 2D.
     */
	void SetGrid(moTexturedGrid p_grid) { m_grid = p_grid; }

    /**
     * Dibuja la grilla asociada a este shader con ancho w y alto w, aplicando las capas hasta la número l.
     * @param w ancho con el que se dibuja la grilla.
     * @param h alto con el que se dibuja la grilla.
     * @param l número de capas de textura a aplicar.
     */
	void DrawGrid(MOint w, MOint h, MOint l) { m_grid.Draw(w, h, l); }


	moConfig   m_Config;

protected:
	MOuint m_type;
	moText m_name;

	moTexturedGrid m_grid;

    MOboolean m_Active;
    MOint m_VertErrorCode;
    MOint m_FragErrorCode;

    virtual void compileVertShader(const moText& vert_source) = 0;
    virtual void compileFragShader(const moText& frag_source) = 0;

    virtual moText LoadShaderSource(const moText& p_fn);
};

moDeclareExportedDynamicArray( moShader*, moShaderArray);


#endif

