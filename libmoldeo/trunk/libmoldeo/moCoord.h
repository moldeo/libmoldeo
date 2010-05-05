#ifndef __MO_COORD_H__
#define __MO_COORD_H__

#include <moMathVector.h>
#include <moMathVector3.h>

struct LIBMOLDEO_API moCoord
{
	moVector3f Position;
	moVector3f Normal;
	moVector3f NextPosition;
	moVector2f TextureCoords;
	moVector2f Raster;
	GLuint TextureId;
	MOfloat Scale;
};

#endif /* __MO_COORD_H__ */

