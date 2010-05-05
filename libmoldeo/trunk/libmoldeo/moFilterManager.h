/*******************************************************************************

								moFilterManager.h

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
  Andrs Colubri

*******************************************************************************/

#ifndef __MOFILTERMANAGER_H
#define __MOFILTERMANAGER_H

#include "moTypes.h"
#include "moAbstract.h"
#include "moText.h"
#include "moVideoGraph.h"
#include "moResourceManager.h"
#include "moBuckets.h"
#include "moParam.h"
#include "moMathVector.h"

//these includes TuioPoint.h TuioTime.h and TuioContainer.h
#ifdef USE_TUIO
#include "tuio/TuioObject.h"
#include "tuio/TuioCursor.h"
#endif

#define MO_TRACKER_TRACKED 0
#define MO_TRACKER_NOT_FOUND -1
#define MO_TRACKER_SMALL_DET -2
#define MO_TRACKER_MAX_ITERATIONS -3
#define MO_TRACKER_OOB -4
#define MO_TRACKER_LARGE_RESIDUE -5

#ifdef USE_TUIO
using namespace TUIO;
moDeclareExportedDynamicArray (TuioCursor *, moTuioCursorArray);
moDeclareExportedDynamicArray (TuioObject *, moTuioObjectArray);
#endif /* USE_TUIO */

/// sistema de datos TUIO
/**
 * sistema de datos tangibles, TUIO ( Tangible Universal Input/Output )
 *   esta clase encapsula los objetos TUIO creados
 */
#ifdef USE_TUIO
class LIBMOLDEO_API moTUIOSystemData
{
	public:moTUIOSystemData ();
	virtual ~ moTUIOSystemData ();
	virtual moVideoFormat & SetVideoFormat (moVideoFormat p_videoformat)
	{
		m_VideoFormat = p_videoformat;
	} virtual moVideoFormat & GetVideoFormat ()
	{
		return m_VideoFormat;
	}
	virtual int GetCursorCount ()
	{
		return m_Cursors.Count ();
	}
	virtual int GetObjectCount ()
	{
		return m_Objects.Count ();
	}
	virtual TuioCursor * GetCursor (int i)
	{
		return m_Cursors[i];
	}
	virtual moTuioCursorArray & GetCursors ()
	{
		return m_Cursors;
	}
	virtual TuioObject * GetObject (int i)
	{
		return m_Objects[i];
	}
	virtual moTuioObjectArray & GetObjects ()
	{
		return m_Objects;
	}

	/**
	 * Creates a new TuioCursor based on the given arguments.
	 * The new TuioCursor is added to the TuioServer's internal list of active TuioCursors
	 * and a reference is returned to the caller.
	 *
	 * @param	xp	the X coordinate to assign
	 * @param	yp	the Y coordinate to assign
	 * @return	reference to the created TuioCursor
	 */
	TuioCursor * addTuioCursor (float xp, float yp);

	/**
	 * Updates the referenced TuioCursor based on the given arguments.
	 *
	 * @param	tcur	the TuioObject to update
	 * @param	xp	the X coordinate to assign
	 * @param	yp	the Y coordinate to assign
	 */
	void updateTuioCursor (TuioCursor * tcur, float xp, float yp);

	/**
	 * Removes the referenced TuioCursor from the TuioServer's internal list of TuioCursors
	 * and deletes the referenced TuioCursor afterwards
	 *
	 * @param	tcur	the TuioCursor to remove
	 */
	void removeTuioCursor (TuioCursor * tcur);

	/**
	 * Updates an externally managed TuioCursor
	 *
	 * @param	tcur	the TuioCursor to update
	 */
	void addExternalTuioCursor (TuioCursor * tcur);

	/**
	 * Updates an externally managed TuioCursor
	 *
	 * @param	tcur	the TuioCursor to update
	 */
	void updateExternalTuioCursor (TuioCursor * tcur);

	/**
	 * Removes an externally managed TuioCursor from the TuioServer's internal list of TuioCursor
	 * The referenced TuioCursor is not deleted
	 *
	 * @param	tcur	the TuioCursor to remove
	 */
	void removeExternalTuioCursor (TuioCursor * tcur);

	/**
	 * Creates a new TuioObject based on the given arguments.
	 * The new TuioObject is added to the TuioServer's internal list of active TuioObjects
	 * and a reference is returned to the caller.
	 *
	 * @param	sym	the Symbol ID  to assign
	 * @param	xp	the X coordinate to assign
	 * @param	yp	the Y coordinate to assign
	 * @param	a	the angle to assign
	 * @return	reference to the created TuioObject
	 */
	virtual TuioObject * addTuioObject (int sym, float xp, float yp, float a);

	/**
	 * Updates the referenced TuioObject based on the given arguments.
	 *
	 * @param	tobj	the TuioObject to update
	 * @param	xp	the X coordinate to assign
	 * @param	yp	the Y coordinate to assign
	 * @param	a	the angle to assign
	 */
	virtual void updateTuioObject (TuioObject * tobj, float xp, float yp,
		float a);

	/**
	 * Removes the referenced TuioObject from the TuioServer's internal list of TuioObjects
	 * and deletes the referenced TuioObject afterwards
	 *
	 * @param	tobj	the TuioObject to remove
	 */
	virtual void removeTuioObject (TuioObject * tobj);

	/**
	 * Adds an externally managed TuioObject to the TuioServer's internal list of active TuioObjects
	 *
	 * @param	tobj	the TuioObject to add
	 */
	void addExternalTuioObject (TuioObject * tobj);

	/**
	 * Updates an externally managed TuioObject
	 *
	 * @param	tobj	the TuioObject to update
	 */
	void updateExternalTuioObject (TuioObject * tobj);

	/**
	 * Removes an externally managed TuioObject from the TuioServer's internal list of TuioObjects
	 * The referenced TuioObject is not deleted
	 *
	 * @param	tobj	the TuioObject to remove
	 */
	void removeExternalTuioObject (TuioObject * tobj);

	/**
	 * Initializes a new frame with the given TuioTime
	 *
	 * @param	ttime	the frame time
	 */
	virtual void initFrame (TuioTime ttime);

	/**
	 * Commits the current frame.
	 * Generates and sends TUIO messages of all currently active and updated TuioObjects and TuioCursors.
	 */
	virtual void commitFrame ();

	/**
	 * Commits the current frame.
	 * Generates and sends TUIO messages of all currently active and updated TuioObjects and TuioCursors.
	 */
	//virtual void commitFrame();
	/**
	 * Returns the next available Session ID for external use.
	 * @return	the next available Session ID for external use
	 */
	virtual long getSessionID ();

	/**
	 * Returns the current frame ID for external use.
	 * @return	the current frame ID for external use
	 */
	virtual long getFrameID ();

	/**
	 * Returns the current frame ID for external use.
	 * @return	the current frame ID for external use
	 */
	virtual TuioTime getFrameTime ();

	/**
	 * Returns a List of all currently inactive TuioObjects
	 *
	 * @return  a List of all currently inactive TuioObjects
	 */
	std::list < TuioObject * >getUntouchedObjects ();

	/**
	 * Returns a List of all currently inactive TuioCursors
	 *
	 * @return  a List of all currently inactive TuioCursors
	 */
	std::list < TuioCursor * >getUntouchedCursors ();

	/**
	 * Calculates speed and acceleration values for all currently inactive TuioObjects
	 */
	void stopUntouchedMovingObjects ();

	/**
	 * Calculates speed and acceleration values for all currently inactive TuioCursors
	 */
	void stopUntouchedMovingCursors ();

	/**
	 * Removes all currently inactive TuioObjects from the TuioServer's internal list of TuioObjects
	 */
	void removeUntouchedStoppedObjects ();

	/**
	 * Removes all currently inactive TuioCursors from the TuioServer's internal list of TuioCursors
	 */
	void removeUntouchedStoppedCursors ();

	/**
	 * Returns a List of all currently active TuioObjects
	 *
	 * @return  a List of all currently active TuioObjects
	 */
	std::list < TuioObject * >getTuioObjects ();

	/**
	 * Returns a List of all currently active TuioCursors
	 *
	 * @return  a List of all currently active TuioCursors
	 */
	std::list < TuioCursor * >getTuioCursors ();

	/**
	 * Returns the TuioObject corresponding to the provided Session ID
	 * or NULL if the Session ID does not refer to an active TuioObject
	 *
	 * @return  an active TuioObject corresponding to the provided Session ID or NULL
	 */
	TuioObject * getTuioObject (long s_id);

	/**
	 * Returns the TuioCursor corresponding to the provided Session ID
	 * or NULL if the Session ID does not refer to an active TuioCursor
	 *
	 * @return  an active TuioCursor corresponding to the provided Session ID or NULL
	 */
	TuioCursor * getTuioCursor (long s_id);

	/**
	 * Returns the TuioObject closest to the provided coordinates
	 * or NULL if there isn't any active TuioObject
	 *
	 * @return  the closest TuioObject to the provided coordinates or NULL
	 */
	TuioObject * getClosestTuioObject (float xp, float yp);

	/**
	 * Returns the TuioCursor closest to the provided coordinates
	 * or NULL if there isn't any active TuioCursor
	 *
	 * @return  the closest TuioCursor corresponding to the provided coordinates or NULL
	 */
	TuioCursor * getClosestTuioCursor (float xp, float yp);
	private:moVideoFormat m_VideoFormat;
	moTuioCursorArray m_Cursors;
	moTuioObjectArray m_Objects;
	std::list < TuioObject * >objectList;
	std::list < TuioCursor * >cursorList;
	int maxCursorID;

	//moTuioCursorArray m_freeCursorList;
	//moTuioObjectArray m_freeCursorBuffer;
	std::list < TuioCursor * >freeCursorList;
	std::list < TuioCursor * >freeCursorBuffer;
	bool full_update;
	int update_interval;
	bool periodic_update;
	long currentFrame;
	TuioTime currentFrameTime;
	bool updateObject, updateCursor;
	long lastCursorUpdate, lastObjectUpdate;
	long sessionID;
	bool verbose;
};
#endif /* USE_TUIO */

class moTrackerFeature;
moDeclareExportedDynamicArray (moTrackerFeature *, moTrackerFeatureArray);

/// Caracterstica de seguimiento ( sensado )
/**
 *   Caracterstica de seguimiento ( sensado )
 *	Punto reconocido y sensado en un espacio bidimensional,
 *	esta clase es abstracta y debe ser implementada en cada dispositivo de sensado y seguimiento
 *	@see moTrackerSystemData
 *	@see moTrackerFeatureArray
 *	@see moFilterManager
 */
class LIBMOLDEO_API moTrackerFeature
{								 //de GpuKLT_Feature
	public:bool is_object;		 //!< once recognized
	bool is_parent;				 //!< once recognized
	bool is_cursor;				 //!< once recognized
	long stime;					 //!< once recognized
	long sframe;				 //!< once recognized
	long utime;					 //!< once recognized
	long uframe;				 //!< once recognized
	moTrackerFeatureArray FeaturesCaptured;
	moTrackerFeature * Parent;
	float x, y;					 //!< Location
	float normx, normy;			 //!< Normalized Feature Coordinates [ 0 - 1 ]
	float tr_x, tr_y;			 //!< Feature position in the previous frame.
	float v_x, v_y;				 //!< Speed in the actual frame.
	float vp_x, vp_y;			 //!< Speed in the previous frame.
	float a_x, a_y;				 //!< Acceleration in the actual frame.
	bool valid;					 //!< True for a valid feature point.
	int val;					 //!<Other states for valid feature point
	moVector2fArray track;		 //!< list of feature positions in the past frames. Forms the feature tracks in video.

	//! Constructor
	moTrackerFeature ();

	//! Destructor
	~moTrackerFeature ();

	//! Imprime los detalles de este punto
	void print ();

	//! Imprime el valor de los valores trazados por este punto
	void printTracks ();

	//! Update Feature Positions
	int updatePos (float kltConvergeThreshold, float kltSSDthresh,
		int kltborder, float delta, float res, float d1, float d2,
		float w, float h);
};

/// Sistema de caractersticas
/**
 *	Clase abstracta, interfaz de acceso a conjunto de datos de seguimiento [tracking]
 *	esta clase es abstracta y debe ser implementada en cada dispositivo de seguimiento
 *	@see moTrackerFeature
 *	@see moTrackerFeatureArray
 *	@see moFilterManager
 */
class LIBMOLDEO_API moTrackerSystemData
{
	public:moTrackerSystemData ();
	moTrackerSystemData (int ZoneW, int ZoneH);
	virtual ~ moTrackerSystemData ();
	virtual int GetFeaturesCount ();
	virtual moVector2f GetBarycenter ();
	virtual moVector2f GetBarycenterMotion ();
	virtual moVector2f GetBarycenterAcceleration ();
	virtual moVector2f GetVariance ();
	virtual moVector2f GetMax ();
	virtual moVector2f GetMin ();
	virtual void SetMax (float x, float y);
	virtual void SetMin (float x, float y);
	virtual int GetValidFeatures ();
	virtual moTrackerFeature * GetFeature (int i);
	virtual moTrackerFeatureArray & GetFeatures ();
	virtual moVideoFormat & GetVideoFormat ()
	{
		return m_VideoFormat;
	}
	virtual void SetBarycenter (float b_x, float b_y)
	{
		m_Barycenter = moVector2f (b_x, b_y);
	} virtual void SetBarycenterMotion (float b_x, float b_y)
	{
		m_BarycenterMotion = moVector2f (b_x, b_y);
	} virtual void SetBarycenterAcceleration (float b_x, float b_y)
	{
		m_BarycenterAcceleration = moVector2f (b_x, b_y);
	} virtual void SetVariance (float v_x, float v_y)
	{
		m_Variance = moVector2f (v_x, v_y);
	} virtual void SetValidFeatures (int validfeatures)
	{
		m_ValidFeatures = validfeatures;
	}
	///Transforma el vector de posicion (0..1) al indice de zona de la matriz cuadrada
	virtual int PositionToZone (float x, float y);

	///Transforma el indice de zona al vector posicion
	virtual moVector2f ZoneToPosition (int zone);

	///Transforma el vector de posicion (0..1) a coordenadas polares luego al indice de zona de la matriz
	virtual int PositionToZoneC (float x, float y);

	///Transforma el indice de zona al vector posicion circular!
	virtual moVector2f ZoneToPositionC (int zone);
	virtual void SetPositionMatrix (float x, float y, int nfeatures);
	virtual void SetPositionMatrix (moVector2f pos, int nfeatures);
	virtual int GetPositionMatrix (float x, float y);
	virtual int GetPositionMatrix (moVector2f pos);
	virtual void SetMotionMatrix (float x, float y, int nfeatures);
	virtual void SetMotionMatrix (moVector2f pos, int nfeatures);
	virtual int GetMotionMatrix (float x, float y);
	virtual int GetMotionMatrix (moVector2f pos);
	virtual void SetAccelerationMatrix (float x, float y, int nfeatures);
	virtual void SetAccelerationMatrix (moVector2f pos, int nfeatures);
	virtual int GetAccelerationMatrix (float x, float y);
	virtual int GetAccelerationMatrix (moVector2f pos);
	virtual void SetPositionMatrixC (float x, float y, int nfeatures);
	virtual int GetPositionMatrixC (float x, float y);
	virtual int GetPositionMatrixC (moVector2f pos);
	virtual int GetPositionMatrixC (int zone);
	virtual void SetMotionMatrixC (float x, float y, int nfeatures);
	virtual int GetMotionMatrixC (float x, float y);
	virtual int GetMotionMatrixC (moVector2f pos);
	virtual int GetMotionMatrixC (int zone);
	virtual void ResetMatrix ();
	virtual void DrawFeatures (int w, int h, float offsetx, float offsety);
	virtual void SetMaxFeatures (int p_nFeatures)
	{
		m_nFeatures = p_nFeatures;
	} virtual int GetMaxFeatures ()
	{
		return m_nFeatures;
	}
	int m_ZoneW;
	int m_ZoneH;
	int m_Zones;
	float **m_Distancias;
	int **m_Pares;
	int nPares;
	protected:float _zonewf;
	float _zonehf;
	moVideoFormat m_VideoFormat;
	moTrackerFeatureArray m_Features;
	moVector2f m_Barycenter;
	moVector2f m_BarycenterMotion;
	moVector2f m_BarycenterAcceleration;
	moVector2f m_Variance;
	int m_ValidFeatures;
	int m_nFeatures;
	moVector2f m_Max;
	moVector2f m_Min;
	int *m_PositionMatrix;		 ///4 x 4 zones
	int *m_MotionMatrix;		 ///4 x 4 zones
	int *m_AccelerationMatrix;	 ///4 x 4 zones
								 ///12 semitones, 3 levels
	int *m_CircularPositionMatrix;
	int *m_CircularMotionMatrix; ///12 semitones, 3 levels
};

/**
 *	Administrador de recursos de procesamiento o filtro de diferentes datos
 *	este administrador de filtros permite procesar disintas fuentes de datos
 *	como son el seguimiento de caractersticas de una imagen en movimiento  o bien
 *	la transformacin de un tipo de dato a otro
 *	@see moTrackerData
 *	@see moTrackerFeature
 *	@see moTrackerFeatureArray
 *	@see moFilterManager
 */
class LIBMOLDEO_API moFilterManager:public moResource
{
	public:moFilterManager ();
	virtual ~ moFilterManager ();
	virtual MOboolean Init ();
	virtual MOboolean Finish ();
	protected:
};
#endif							 /*  */
