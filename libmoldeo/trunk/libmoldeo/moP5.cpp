/*******************************************************************************

                                 moP5.cpp

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

#include "moP5.h"
#include "moMath.h"

moP5::moP5()
{
	currColorMode = MO_P5_RGB;
	float defCoeff = 1 / 255.0;
	for (int i = 0; i < 4; i++)
	{
		colorRGBCoeff[i] = defCoeff;
		colorHSBCoeff[i] = defCoeff;
	}

	strokeColor[0] = 0.7;
	strokeColor[1] = 0.7;
	strokeColor[2] = 0.7;
	strokeColor[3] = 1.0;

	fillColor[0] = 0.7;
	fillColor[1] = 0.7;
	fillColor[2] = 0.7;
	fillColor[3] = 1.0;
}

void moP5::triangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
	// Propper handling of stroke, fill color and line weight
	glBegin(GL_TRIANGLES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glVertex2f(x3, y3);
	glEnd();
}

void moP5::line(float x1, float y1, float x2, float y2)
{
    glEnable( GL_TEXTURE_2D );
  glBindTexture(GL_TEXTURE_2D,0);
	glColor4f(strokeColor[0], strokeColor[1], strokeColor[2], strokeColor[3]);
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
}

void moP5::line(float x1, float y1, float z1, float x2, float y2, float z2)
{
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D,0);
	glColor4f(strokeColor[0], strokeColor[1], strokeColor[2], strokeColor[3]);
	glBegin(GL_LINES);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
	glEnd();
}

void moP5::arc(float x, float y, float width, float height, float start, float stop, int slices )
{
	// Implement display list with precomputed circle coordinates...
  glEnable( GL_TEXTURE_2D );
  glBindTexture(GL_TEXTURE_2D,0);

	float N;
    float Nstep;

	N = (float)slices;
	Nstep = MO_P5_TWO_PI / N;

	switch(fillMode) {

        case MO_P5_NOFILL:
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glColor4f(strokeColor[0], strokeColor[1], strokeColor[2], strokeColor[3]);
            glBegin(GL_LINE_LOOP);
            break;

        case MO_P5_FILL:
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glColor4f(fillColor[0], fillColor[1], fillColor[2], fillColor[3]);
            glBegin(GL_POLYGON);
            break;
        default:
            return;
            break;

	}

        glVertex2f( (float)x, (float)y);
        for(float t = start; t <= stop; t += Nstep )
            glVertex2f(width* cos(t) + (float)x, height * sin(t) + (float)y);
    glEnd();

}

void moP5::point(float x, float y)
{
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}


void moP5::point(float x, float y, float z)
{
    glBegin(GL_POINTS);
        glVertex3f(x, y, z);
    glEnd();
}

void moP5::quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	glBegin(GL_QUADS);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glVertex2f(x3, y3);
		glVertex2f(x4, y4);
	glEnd();
}

void moP5::ellipse(float x, float y, float width, float height, int slices )
{
	// Implement display list with precomputed circle coordinates...
	// and scale it to the right size.
  glEnable( GL_TEXTURE_2D );
  glBindTexture(GL_TEXTURE_2D,0);

	float N;

	N = (float)slices;

	switch(fillMode) {

        case MO_P5_NOFILL:
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glColor4f(strokeColor[0], strokeColor[1], strokeColor[2], strokeColor[3]);
            glBegin(GL_LINE_LOOP);
                for(float t = 0; t <= MO_P5_TWO_PI; t += MO_P5_TWO_PI/N)
                    glVertex2f(width* cos(t) + (float)x, height * sin(t) + (float)y);
            glEnd();
            break;

        case MO_P5_FILL:
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glColor4f(fillColor[0], fillColor[1], fillColor[2], fillColor[3]);
            glBegin(GL_POLYGON);
                for(float t = 0; t <= MO_P5_TWO_PI; t += MO_P5_TWO_PI/N)
                    glVertex2f(width * cos(t) + (float)x, height * sin(t) + (float)y);
            glEnd();
            break;
	}

}

void moP5::rect(float x, float y, float width, float height)
{

	int N;

	N = 12;

	switch(fillMode) {

        case MO_P5_NOFILL:
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glBindTexture(GL_TEXTURE_2D,0);
            glColor4f(strokeColor[0], strokeColor[1], strokeColor[2], strokeColor[3]);
            break;

        case MO_P5_FILL:
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glBindTexture(GL_TEXTURE_2D,0);
            glColor4f(fillColor[0], fillColor[1], fillColor[2], fillColor[3]);
            break;
	}

	glRectf(x, y, x + width, y + height);
}

void moP5::strokeWeight(float width)
{
	glLineWidth(width);
}

/*
void moP5::smooth()
{
}

void moP5::strokeJoin(moP5StrokeJoinMode MODE)
{
}

void moP5::noSmooth()
{
}

void moP5::ellipseMode(moP5ShapeMode MODE)
{
}

void moP5::rectMode(moP5ShapeMode MODE)
{
}

void moP5::strokeCap(moP5StrokeCapMode MODE)
{
}
*/

void moP5::background(float gray)
{
	generateTmpColor(gray, gray, gray);
	glClearColor(tmpColor[0], tmpColor[1], tmpColor[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void moP5::background(float gray, float alpha)
{
	generateTmpColor(gray, gray, gray, alpha);
	glClearColor(tmpColor[0], tmpColor[1], tmpColor[2], tmpColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void moP5::background(float value1, float value2, float value3)
{
	generateTmpColor(value1, value2, value3);
	glClearColor(tmpColor[0], tmpColor[1], tmpColor[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void moP5::background(float value1, float value2, float value3, float alpha)
{
	generateTmpColor(value1, value2, value3, alpha);
	glClearColor(tmpColor[0], tmpColor[1], tmpColor[2], tmpColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void moP5::colorMode(int mode)
{
	currColorMode = mode;
}

void moP5::colorMode(int mode, float range)
{
	currColorMode = mode;

	float coeff = 1 / 255.0;
	if ((0 < range) && (range <= 255.0)) coeff = 1 / range;

	if (currColorMode == MO_P5_RGB)
	{
		for (int i = 0; i < 4; i++) colorRGBCoeff[i] = coeff;
	}
	else
	{
		for (int i = 0; i < 4; i++) colorHSBCoeff[i] = coeff;
	}
}

void moP5::colorMode(int mode, float range1, float range2, float range3)
{
	currColorMode = mode;

	float coeff1 = 1 / 255.0;
	float coeff2 = 1 / 255.0;
	float coeff3 = 1 / 255.0;
	if ((0 < range1) && (range1 <= 255.0)) coeff1 = 1 / range1;
	if ((0 < range2) && (range2 <= 255.0)) coeff2 = 1 / range2;
	if ((0 < range3) && (range3 <= 255.0)) coeff3 = 1 / range3;

	if (currColorMode == MO_P5_RGB)
	{
		colorRGBCoeff[0] = coeff1;
		colorRGBCoeff[1] = coeff2;
		colorRGBCoeff[2] = coeff3;
	}
	else
	{
		colorHSBCoeff[0] = coeff1;
		colorHSBCoeff[1] = coeff2;
		colorHSBCoeff[2] = coeff3;
	}
}

void moP5::colorMode(int mode, float range1, float range2, float range3, float range4)
{
	currColorMode = mode;

	float coeff1 = 1 / 255.0;
	float coeff2 = 1 / 255.0;
	float coeff3 = 1 / 255.0;
	float coeff4 = 1 / 255.0;
	if ((0 < range1) && (range1 <= 255.0)) coeff1 = 1 / range1;
	if ((0 < range2) && (range2 <= 255.0)) coeff2 = 1 / range2;
	if ((0 < range3) && (range3 <= 255.0)) coeff3 = 1 / range3;
	if ((0 < range4) && (range4 <= 255.0)) coeff4 = 1 / range4;

	if (currColorMode == MO_P5_RGB)
	{
		colorRGBCoeff[0] = coeff1;
		colorRGBCoeff[1] = coeff2;
		colorRGBCoeff[2] = coeff3;
		colorRGBCoeff[3] = coeff4;
	}
	else
	{
		colorHSBCoeff[0] = coeff1;
		colorHSBCoeff[1] = coeff2;
		colorHSBCoeff[2] = coeff3;
		colorHSBCoeff[3] = coeff4;
	}
}

void moP5::stroke(float gray)
{
    strokeColor[0] = gray;
    strokeColor[1] = gray;
    strokeColor[2] = gray;
    strokeColor[3] = 1.0;
}

void moP5::stroke(float gray, float alpha)
{
    stroke(gray);
    strokeColor[3] = alpha;
}

void moP5::stroke(float value1, float value2, float value3)
{
    strokeColor[0] = value1;
    strokeColor[1] = value2;
    strokeColor[2] = value3;
    strokeColor[3] = 1.0;
}

void moP5::stroke(float value1, float value2, float value3, float alpha)
{
    strokeColor[0] = value1;
    strokeColor[1] = value2;
    strokeColor[2] = value3;
    strokeColor[3] = alpha;
}

void moP5::noFill()
{
    fillMode = MO_P5_NOFILL;
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

void moP5::noStroke()
{
    strokeColor[0] = 0.0;
    strokeColor[1] = 0.0;
    strokeColor[2] = 0.0;
    strokeColor[3] = 0.0;
}

void moP5::fill(float gray)
{
    fill( gray, gray, gray , 1.0 );
}

void moP5::fill(float gray, float alpha)
{
    fill( gray, gray, gray , alpha );
}

void moP5::fill(float value1, float value2, float value3)
{
    fill( value1, value2, value3 , 1.0 );
}

void moP5::fill(float value1, float value2, float value3, float alpha)
{
    fillMode = MO_P5_FILL;
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    fillColor[0] = value1;
    fillColor[1] = value2;
    fillColor[2] = value3;
    fillColor[3] = alpha;
}

void moP5::pushMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void moP5::popMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void moP5::resetMatrix()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void moP5::scale(float size)
{
	glScalef(size, size, size);
}

void moP5::scale(float x, float y, float z)
{
	glScalef(x, y, z);
}

void moP5::translate(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void moP5::rotate(float angle, float x, float y, float z)
{
	glRotatef(angle * moMathf::RAD_TO_DEG, x, y, z);
}

void moP5::generateTmpColor(float comp1, float comp2, float comp3)
{
	if (currColorMode == MO_P5_RGB) generateTmpColor(comp1, comp2, comp3, 1 / colorRGBCoeff[3]);
	else generateTmpColor(comp1, comp2, comp3, 1 / colorHSBCoeff[3]);
}

void moP5::generateTmpColor(float comp1, float comp2, float comp3, float comp4)
{
	if (currColorMode == MO_P5_RGB)
	{
		// Direct translation to RGBA.
		tmpColor[0] = comp1 * colorRGBCoeff[0];
		tmpColor[1] = comp2 * colorRGBCoeff[1];
		tmpColor[2] = comp3 * colorRGBCoeff[2];
		tmpColor[3] = comp4 * colorRGBCoeff[3];
	}
	else
	{
		// Converting HSB to RGB.

		// Normalizing HSB components first.
		float h = comp1 * colorHSBCoeff[0];
		float s = comp2 * colorHSBCoeff[1];
		float l = comp3 * colorHSBCoeff[2];
		float r, g, b;

		convertHSLtoRGB(h, s, l, r, g, b);

		// convertRGBtoHSB returns normalized RGB components.
		tmpColor[0] = r;
		tmpColor[1] = g;
		tmpColor[2] = b;
		tmpColor[3] = comp4 * colorHSBCoeff[3];
	}
}

void moP5::convertHSLtoRGB(float h, float s, float l, float& r, float& g, float& b)
{
	float m1, m2;

	if (s == 0.0)
	{
		r = l;
		g = l;
		b = l;
	}
	else
	{
		if (l <= 0.5) m2 = l * (1.0 + s);
		else m2 = l + s - (l * s);
		m1 = 2.0 * l - m2;
		r = HuetoRGB(m1, m2, h + 1.0 / 3.0);
		g = HuetoRGB(m1, m2, h);
		b = HuetoRGB(m1, m2, h - 1.0 / 3.0);
	}
}

float moP5::HuetoRGB(float m1, float m2, float h)
{
	if (h < 0) h = h + 1.0;
	if (h > 1) h = h - 1.0;
	if (6.0 * h < 1) return (m1 + (m2 - m1) * h * 6.0);
	else
	{
		if (2.0 * h < 1) return m2;
		else
			if (3.0 * h < 2.0) return (m1 + (m2 - m1) * ((2.0 / 3.0) - h) * 6.0);
			else return m1;
	}
}

const float moP5::MO_P5_HALF_PI = 1.57079632679489661923;
const float moP5::MO_P5_TWO_PI = 6.28318530717958647693;
const float moP5::MO_P5_PI = 3.14159265358979323846;


/*
Code to create display list to show ellipses:
(display list tutorial here:
http://www.lighthouse3d.com/opengl/displaylists/)
Thread about VBO vs DL:
http://www.gamedev.net/community/forums/topic.asp?topic_id=378417

1) Construct Lookup table (LUT) (it will be used also for arcs.

float sinLUT[];
float cosLUT[];
float SINCOS_PRECISION = 0.1f;
int SINCOS_LENGTH = (int) (360f / SINCOS_PRECISION);

    // Initializing sine and cosine lookup tables.
    sinLUT = new float[SINCOS_LENGTH];
    cosLUT = new float[SINCOS_LENGTH];
    for (int i = 0; i < SINCOS_LENGTH; i++)
    {
      sinLUT[i] = (float) Math.sin(i * DEG_TO_RAD * SINCOS_PRECISION);
      cosLUT[i] = (float) Math.cos(i * DEG_TO_RAD * SINCOS_PRECISION);
    }

2) Create Display lis

GLuint unityEllipse;
unityEllipse = glGenLists(1);

glNewList(unityEllipse, GL_COMPILE);
  drawUnityEllipse();
glEndList();

glScalef(x, y, z);
glTranslate(x, y, z);
glCallList(unityEllipse);

3) drawUnityEllipse() implementation:

            int increment = 1;
            for (int i = startLUT; i < stopLUT; i += increment)
            {
                int ii = i % SINCOS_LENGTH;
                vertex(cosLUT[ii] * circleRadius0,
                       sinLUT[ii] * circleRadius0);
                vertex(cosLUT[ii] * circleRadius1,
                       sinLUT[ii] * circleRadius1);
            }

			*/

