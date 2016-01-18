/*******************************************************************************

                              moFilterManager.cpp

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

*******************************************************************************/

#include "moFilterManager.h"

#include "moArray.h"
#ifdef USE_TUIO
moDefineDynamicArray( moTuioCursorArray )
moDefineDynamicArray( moTuioObjectArray )
#endif
moDefineDynamicArray( moTrackerFeatureArray )
moDefineDynamicArray( moTrackerInstanceRecords );


moTrackerInstanceRecord::moTrackerInstanceRecord() {
  m_Variance = moVector2f(0,0);
  m_SpeedVariance = moVector2f(0,0);
  m_AccelerationVariance = moVector2f(0,0);

  m_BoundingRectangle = moVector4f(0,0,0,0);
  m_BoundingRectangleAngle = 0.0;
  m_BlobCandidates =  0;

  m_AbsoluteSpeedAverage =  0;
  m_AbsoluteAccelerationAverage =  0;
  m_AbsoluteTorqueAverage =  0;

  m_Barycenter =  moVector2f(0,0);
  m_BarycenterMotion =  moVector2f(0,0);
  m_BarycenterAcceleration =  moVector2f(0,0);


  m_Max = moVector2f(0,0);
  m_Min = moVector2f(0,0);

  m_nFeatures = 0;
  m_SurfaceCovered = 0;
  m_Tick = -1;
  m_ValidFeatures = 0;
  m_DeltaValidFeatures = 0;
}

moTrackerInstanceRecord & moTrackerInstanceRecord::operator = (const moTrackerInstanceRecord &src) {

  m_Variance = src.m_Variance;
  m_SpeedVariance = src.m_SpeedVariance;
  m_AccelerationVariance = src.m_AccelerationVariance;

  m_BoundingRectangle = src.m_BoundingRectangle;
  m_BoundingRectangleAngle = src.m_BoundingRectangleAngle;
  m_BlobCandidates =  src.m_BlobCandidates;

  m_AbsoluteSpeedAverage =  src.m_AbsoluteSpeedAverage;
  m_AbsoluteAccelerationAverage =  src.m_AbsoluteAccelerationAverage;
  m_AbsoluteTorqueAverage =  src.m_AbsoluteTorqueAverage;

  m_Barycenter =  src.m_Barycenter;
  m_BarycenterMotion =  src.m_BarycenterMotion;
  m_BarycenterAcceleration =  src.m_BarycenterAcceleration;


  m_Max = src.m_Max;
  m_Min = src.m_Min;

  m_nFeatures = src.m_nFeatures;
  m_SurfaceCovered = src.m_SurfaceCovered;
  m_Tick = src.m_Tick;
  m_ValidFeatures = src.m_ValidFeatures;
  m_DeltaValidFeatures = src.m_DeltaValidFeatures;
  return(*this);
}

moTrackerSystemHistory::moTrackerSystemHistory() {
  m_MaxTime = 60000;
  m_Granularity = 33;
  m_RecordFactor = m_MaxTime / m_Granularity;
}

moTrackerSystemHistory::~moTrackerSystemHistory() {

}

void
moTrackerSystemHistory::StartRecording( long maxtime, long granularity ) {

  if ( !m_Timer.Started() ) {
    m_Timer.Start();

    ///tiempo máximo de duracion de la grabacion
    m_MaxTime = maxtime;

    ///granularidad de la grabacion ( 30 ms [tiempo ] )
    m_Granularity = granularity;

    m_nRecorded = 0;

    m_RecordFactor =  m_MaxTime / m_Granularity;

    m_History.Empty();

    moTrackerInstanceRecord dummyRec;

    m_History.Init( (int)(m_RecordFactor)+1, dummyRec );

    MODebug2->Message("Tracker History Started with" + IntToStr(m_History.Count()) + " records in max " + IntToStr(m_MaxTime) + " ms." );

  } else {
    m_Timer.Continue();
  }

}

void
moTrackerSystemHistory::ContinueRecording() {
  m_Timer.Continue();
  MODebug2->Message("Tracker History Resumed");
}

long  moTrackerSystemHistory::CountRecords() {
  return m_nRecorded;
}


void
moTrackerSystemHistory::Record( moTrackerInstanceRecord& p_InstanceRecord, long p_Tick ) {

  double rec_pos = 0;
  MOulong rec_posl = 0;

  if (m_Timer.Started()) {


      if ( p_Tick>=0 && p_InstanceRecord.m_Tick<0) p_InstanceRecord.m_Tick = p_Tick;
      else if ( p_Tick == -1 ) {
          p_InstanceRecord.m_Tick = m_Timer.Duration();
      }

      if ( m_History.Count()==0 ) {

        m_History.Add( p_InstanceRecord );
        //MODebug2->Message("Tracker History Record Added at the end.");
      } else {

        rec_pos = (double) ( p_InstanceRecord.m_Tick / m_Granularity );

        rec_posl = (MOulong)rec_pos;

        if ( rec_posl < m_History.Count() ) {
          m_History.Set( rec_posl, p_InstanceRecord );
          m_nRecorded = rec_posl+1;
          //MODebug2->Message("Tracker History Record Added at "+IntToStr(rec_posl));
        }

      }
  }
}

void moTrackerSystemData::Record() {

   m_History.Record( m_ActualRecord );

}

///Util cuando no hay señales interesantes...
void
moTrackerSystemHistory::PauseRecording() {
  m_Timer.Pause();
  MODebug2->Message("Tracker History Recording Paused.");
}

void
moTrackerSystemHistory::StopRecording() {
  m_Timer.Stop();
  MODebug2->Message("Tracker History Recording Stopped.");
}

void
moTrackerSystemHistory::Reset() {
  m_Timer.Stop();
  m_History.Empty();
  MODebug2->Message("Tracker History Recording Reset.");
}

bool
moTrackerSystemHistory::SaveToFile( moText filename ) {
    MODebug2->Message("moTrackerSystemHistory::SaveToFile > not implemented! argument: " + filename);
    return false;
}

bool
moTrackerSystemHistory::SaveToXML( moText filename ) {
  MODebug2->Message("moTrackerSystemHistory::SaveToXML > not implemented! argument: " + filename);
  return false;
}

moTrackerInstanceRecord&
moTrackerSystemHistory::Get( int index ) {
    return m_History[index];
}

#ifdef USE_TUIO

moTUIOSystemData::moTUIOSystemData() {

	currentFrameTime = TuioTime::getSessionTime().getSeconds();
	currentFrame = sessionID = maxCursorID = -1;
	verbose = updateObject = updateCursor = false;
	lastObjectUpdate = lastCursorUpdate = currentFrameTime.getSeconds();

	periodic_update = false;
	full_update = false;
}

moTUIOSystemData::~moTUIOSystemData() {


}

TuioObject* moTUIOSystemData::addTuioObject(int f_id, float x, float y, float a) {
	sessionID++;
	TuioObject *tobj = new TuioObject(currentFrameTime, sessionID, f_id, x, y, a);
	objectList.push_back(tobj);
	updateObject = true;

	if (verbose)
		std::cout << "add obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() << std::endl;

	return tobj;
}

void moTUIOSystemData::addExternalTuioObject(TuioObject *tobj) {
	if (tobj==NULL) return;
	objectList.push_back(tobj);
	updateObject = true;

	if (verbose)
		std::cout << "add obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() << std::endl;
}

void moTUIOSystemData::updateTuioObject(TuioObject *tobj, float x, float y, float a) {
	if (tobj==NULL) return;
	if (tobj->getTuioTime()==currentFrameTime) return;
	tobj->update(currentFrameTime,x,y,a);
	updateObject = true;

	if (verbose && tobj->isMoving())
		std::cout << "set obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle()
		<< " " << tobj->getXSpeed() << " " << tobj->getYSpeed() << " " << tobj->getRotationSpeed() << " " << tobj->getMotionAccel() << " " << tobj->getRotationAccel() << std::endl;
}

void moTUIOSystemData::updateExternalTuioObject(TuioObject *tobj) {
	if (tobj==NULL) return;
	updateObject = true;
	if (verbose && tobj->isMoving())
		std::cout << "set obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle()
		<< " " << tobj->getXSpeed() << " " << tobj->getYSpeed() << " " << tobj->getRotationSpeed() << " " << tobj->getMotionAccel() << " " << tobj->getRotationAccel() << std::endl;
}

void moTUIOSystemData::removeTuioObject(TuioObject *tobj) {
	if (tobj==NULL) return;
	objectList.remove(tobj);
	delete tobj;
	updateObject = true;

	if (verbose)
		std::cout << "del obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ")" << std::endl;
}

void moTUIOSystemData::removeExternalTuioObject(TuioObject *tobj) {
	if (tobj==NULL) return;
	objectList.remove(tobj);
	updateObject = true;

	if (verbose)
		std::cout << "del obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ")" << std::endl;
}

TuioCursor* moTUIOSystemData::addTuioCursor(float x, float y) {
	sessionID++;

	int cursorID = (int)cursorList.size();
	if (((int)(cursorList.size())<=maxCursorID) && ((int)(freeCursorList.size())>0)) {
		std::list<TuioCursor*>::iterator closestCursor = freeCursorList.begin();

		for(std::list<TuioCursor*>::iterator iter = freeCursorList.begin();iter!= freeCursorList.end(); iter++) {
			if((*iter)->getDistance(x,y)<(*closestCursor)->getDistance(x,y)) closestCursor = iter;
		}

		TuioCursor *freeCursor = (*closestCursor);
		cursorID = (*closestCursor)->getCursorID();
		freeCursorList.erase(closestCursor);
		delete freeCursor;
	} else maxCursorID = cursorID;

	TuioCursor *tcur = new TuioCursor(currentFrameTime, sessionID, cursorID, x, y);
	cursorList.push_back(tcur);
	updateCursor = true;

	if (verbose)
		std::cout << "add cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() << std::endl;

	return tcur;
}

void moTUIOSystemData::addExternalTuioCursor(TuioCursor *tcur) {
	if (tcur==NULL) return;
	cursorList.push_back(tcur);
	updateCursor = true;

	if (verbose)
		std::cout << "add cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() << std::endl;
}

void moTUIOSystemData::updateTuioCursor(TuioCursor *tcur,float x, float y) {
	if (tcur==NULL) return;
	if (tcur->getTuioTime()==currentFrameTime) return;
	tcur->update(currentFrameTime,x,y);
	updateCursor = true;

	if (verbose && tcur->isMoving())
		std::cout << "set cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY()
		<< " " << tcur->getXSpeed() << " " << tcur->getYSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;
}

void moTUIOSystemData::updateExternalTuioCursor(TuioCursor *tcur) {
	if (tcur==NULL) return;
	updateCursor = true;
	if (verbose && tcur->isMoving())
		std::cout << "set cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY()
		<< " " << tcur->getXSpeed() << " " << tcur->getYSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;
}

void moTUIOSystemData::removeTuioCursor(TuioCursor *tcur) {
	if (tcur==NULL) return;
	cursorList.remove(tcur);
	tcur->remove(currentFrameTime);
	updateCursor = true;

	if (verbose)
		std::cout << "del cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ")" << std::endl;

	if (tcur->getCursorID()==maxCursorID) {
		maxCursorID = -1;
		delete tcur;

		if (cursorList.size()>0) {
			std::list<TuioCursor*>::iterator clist;
			for (clist=cursorList.begin(); clist != cursorList.end(); clist++) {
				int cursorID = (*clist)->getCursorID();
				if (cursorID>maxCursorID) maxCursorID=cursorID;
			}

			freeCursorBuffer.clear();
			for (std::list<TuioCursor*>::iterator flist=freeCursorList.begin(); flist != freeCursorList.end(); flist++) {
				TuioCursor *freeCursor = (*flist);
				if (freeCursor->getCursorID()>maxCursorID) delete freeCursor;
				else freeCursorBuffer.push_back(freeCursor);
			}
			freeCursorList = freeCursorBuffer;

		} else {
			for (std::list<TuioCursor*>::iterator flist=freeCursorList.begin(); flist != freeCursorList.end(); flist++) {
				TuioCursor *freeCursor = (*flist);
				delete freeCursor;
			}
			freeCursorList.clear();
		}
	} else if (tcur->getCursorID()<maxCursorID) {
		freeCursorList.push_back(tcur);
	}
}

void moTUIOSystemData::removeExternalTuioCursor(TuioCursor *tcur) {
	if (tcur==NULL) return;
	cursorList.remove(tcur);
	updateCursor = true;

	if (verbose)
		std::cout << "del cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ")" << std::endl;
}

long moTUIOSystemData::getSessionID() {
	sessionID++;
	return sessionID;
}

long moTUIOSystemData::getFrameID() {
	return currentFrame;
}

TuioTime moTUIOSystemData::getFrameTime() {
	return currentFrameTime;
}

void moTUIOSystemData::initFrame(TuioTime ttime) {
	currentFrameTime = ttime;
	currentFrame++;
}

void moTUIOSystemData::commitFrame() {
/*
	if(updateCursor) {
		startCursorBundle();
		for (std::list<TuioCursor*>::iterator tuioCursor = cursorList.begin(); tuioCursor!=cursorList.end(); tuioCursor++) {

			// start a new packet if we exceed the packet capacity

			if ((oscPacket->Capacity()-oscPacket->Size())<CUR_MESSAGE_SIZE) {
				sendCursorBundle(currentFrame);
				startCursorBundle();
			}


			TuioCursor *tcur = (*tuioCursor);
			if ((full_update) || (tcur->getTuioTime()==currentFrameTime)) addCursorMessage(tcur);
		}
		//sendCursorBundle(currentFrame);
	} else if ((!periodic_update) && (lastCursorUpdate<currentFrameTime.getSeconds())) {
		lastCursorUpdate = currentFrameTime.getSeconds();
		startCursorBundle();
		sendCursorBundle(currentFrame);
	}
	updateCursor = false;

	if(updateObject) {
		startObjectBundle();
		for (std::list<TuioObject*>::iterator  tuioObject = objectList.begin(); tuioObject!=objectList.end(); tuioObject++) {

			// start a new packet if we exceed the packet capacity
			if ((oscPacket->Capacity()-oscPacket->Size())<OBJ_MESSAGE_SIZE) {
				sendObjectBundle(currentFrame);
				startObjectBundle();
			}

			TuioObject *tobj = (*tuioObject);
			if  ((full_update) || (tobj->getTuioTime()==currentFrameTime)) addObjectMessage(tobj);
		}
		sendObjectBundle(currentFrame);
	} else if ((!periodic_update) && (lastObjectUpdate<currentFrameTime.getSeconds())) {
		lastObjectUpdate = currentFrameTime.getSeconds();
		startObjectBundle();
		sendObjectBundle(currentFrame);
	}
	*/
	updateCursor = false;
	updateObject = false;
}


TuioObject* moTUIOSystemData::getTuioObject(long s_id) {
	for (std::list<TuioObject*>::iterator iter=objectList.begin(); iter != objectList.end(); iter++)
		if((*iter)->getSessionID()==s_id) return (*iter);

	return NULL;
}

TuioCursor* moTUIOSystemData::getTuioCursor(long s_id) {
	for (std::list<TuioCursor*>::iterator iter=cursorList.begin(); iter != cursorList.end(); iter++)
		if((*iter)->getSessionID()==s_id) return (*iter);

	return NULL;
}

TuioObject* moTUIOSystemData::getClosestTuioObject(float xp, float yp) {

	TuioObject *closestObject = NULL;
	float closestDistance = 1.0f;

	for (std::list<TuioObject*>::iterator iter=objectList.begin(); iter != objectList.end(); iter++) {
		float distance = (*iter)->getDistance(xp,yp);
		if(distance<closestDistance) {
			closestObject = (*iter);
			closestDistance = distance;
		}
	}

	return closestObject;
}

TuioCursor* moTUIOSystemData::getClosestTuioCursor(float xp, float yp) {

	TuioCursor *closestCursor = NULL;
	float closestDistance = 1.0f;

	for (std::list<TuioCursor*>::iterator iter=cursorList.begin(); iter != cursorList.end(); iter++) {
		float distance = (*iter)->getDistance(xp,yp);
		if(distance<closestDistance) {
			closestCursor = (*iter);
			closestDistance = distance;
		}
	}

	return closestCursor;
}

std::list<TuioObject*> moTUIOSystemData::getTuioObjects() {
	return objectList;
}

std::list<TuioCursor*> moTUIOSystemData::getTuioCursors() {
	return cursorList;
}

std::list<TuioObject*> moTUIOSystemData::getUntouchedObjects() {

	std::list<TuioObject*> untouched;
	for (std::list<TuioObject*>::iterator tuioObject = objectList.begin(); tuioObject!=objectList.end(); tuioObject++) {
		TuioObject *tobj = (*tuioObject);
		if (tobj->getTuioTime()!=currentFrameTime) untouched.push_back(tobj);
	}
	return untouched;
}

void moTUIOSystemData::stopUntouchedMovingObjects() {

	std::list<TuioObject*> untouched;
	for (std::list<TuioObject*>::iterator tuioObject = objectList.begin(); tuioObject!=objectList.end(); tuioObject++) {

		TuioObject *tobj = (*tuioObject);
		if ((tobj->getTuioTime()!=currentFrameTime) && (tobj->isMoving())) {
			tobj->stop(currentFrameTime);
			updateObject = true;
			/*if (verbose)
				std::cout << "set obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle()
				<< " " << tobj->getXSpeed() << " " << tobj->getYSpeed() << " " << tobj->getRotationSpeed() << " " << tobj->getMotionAccel() << " " << tobj->getRotationAccel() << std::endl;
				*/
		}
	}
}

void moTUIOSystemData::removeUntouchedStoppedObjects() {

	std::list<TuioObject*>::iterator tuioObject = objectList.begin();
	while (tuioObject!=objectList.end()) {
		TuioObject *tobj = (*tuioObject);
		if ((tobj->getTuioTime()!=currentFrameTime) && (!tobj->isMoving())) {
			removeTuioObject(tobj);
			tuioObject = objectList.begin();
		} else tuioObject++;
	}
}


std::list<TuioCursor*> moTUIOSystemData::getUntouchedCursors() {

	std::list<TuioCursor*> untouched;
	for (std::list<TuioCursor*>::iterator tuioCursor = cursorList.begin(); tuioCursor!=cursorList.end(); tuioCursor++) {
		TuioCursor *tcur = (*tuioCursor);
		if (tcur->getTuioTime()!=currentFrameTime) untouched.push_back(tcur);
	}
	return untouched;
}

void moTUIOSystemData::stopUntouchedMovingCursors() {

	std::list<TuioCursor*> untouched;
	for (std::list<TuioCursor*>::iterator tuioCursor = cursorList.begin(); tuioCursor!=cursorList.end(); tuioCursor++) {
		TuioCursor *tcur = (*tuioCursor);
		if ((tcur->getTuioTime()!=currentFrameTime) && (tcur->isMoving())) {
			tcur->stop(currentFrameTime);
			updateCursor = true;
			/*
			if (verbose)
				std::cout << "set cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY()
				<< " " << tcur->getXSpeed() << " " << tcur->getYSpeed()<< " " << tcur->getMotionAccel() << " " << std::endl;
				*/
		}
	}
}

void moTUIOSystemData::removeUntouchedStoppedCursors() {

	if (cursorList.size()==0) return;
	std::list<TuioCursor*>::iterator tuioCursor = cursorList.begin();
	while (tuioCursor!=cursorList.end()) {
		TuioCursor *tcur = (*tuioCursor);
		if ((tcur->getTuioTime()!=currentFrameTime) && (!tcur->isMoving())) {
			removeTuioCursor(tcur);
			tuioCursor = cursorList.begin();
		} else tuioCursor++;
	}
}

#endif







moTrackerSystemData::moTrackerSystemData() {
        m_ZoneW = 4;
        m_ZoneH = 4;
        m_ZoneCW = 12;
        m_ZoneCH = 3;

        _zonewf = (float)(m_ZoneW) - 0.000001;
        _zonehf = (float)(m_ZoneH) - 0.000001;
        _zonecwf = (float)(m_ZoneCW) - 0.000001;
        _zonechf = (float)(m_ZoneCH) - 0.000001;
        m_Zones = m_ZoneW*m_ZoneH;
        m_ZonesC = m_ZoneCW*m_ZoneCH;
        m_PositionMatrix =  new int [m_Zones];
        m_MotionMatrix =  new int [m_Zones];
        m_AccelerationMatrix =  new int [m_Zones];
        m_CircularPositionMatrix =  new int [m_ZonesC];
        m_CircularMotionMatrix =  new int [m_ZonesC];
        m_Features.Init( 0, NULL );
        m_ActualRecord.m_nFeatures = 0;

        m_Distancias = NULL;
        m_Pares = NULL;
        nPares = 0;
        ResetMatrix();
}

moTrackerSystemData::moTrackerSystemData( int ZoneW, int ZoneH ) {
        m_ZoneW = ZoneW;
        m_ZoneH = ZoneH;
        m_ZoneCW = ZoneW;
        m_ZoneCH = ZoneH;

        _zonewf = (float)(m_ZoneW) - 0.000001;
        _zonehf = (float)(m_ZoneH) - 0.000001;
        _zonecwf = (float)(m_ZoneCW) - 0.000001;
        _zonechf = (float)(m_ZoneCH) - 0.000001;
        m_Zones = m_ZoneW*m_ZoneH;
        m_ZonesC = m_ZoneCW*m_ZoneCH;
        m_PositionMatrix =  new int [m_Zones];
        m_MotionMatrix =  new int [m_Zones];
        m_AccelerationMatrix =  new int [m_Zones];
        m_CircularPositionMatrix =  new int [m_ZonesC];
        m_CircularMotionMatrix =  new int [m_ZonesC];
        m_Features.Init( 0, NULL );
        m_ActualRecord.m_nFeatures = 0;

        m_Distancias = NULL;
        m_Pares = NULL;
        nPares = 0;

        ResetMatrix();

}

moTrackerSystemData::moTrackerSystemData( int ZoneW, int ZoneH, int ZoneCW, int ZoneCH ) {
        m_ZoneW = ZoneW;
        m_ZoneH = ZoneH;
        m_ZoneCW = ZoneCW;
        m_ZoneCH = ZoneCH;

        _zonewf = (float)(m_ZoneW) - 0.000001;
        _zonehf = (float)(m_ZoneH) - 0.000001;
        _zonecwf = (float)(m_ZoneCW) - 0.000001;
        _zonechf = (float)(m_ZoneCH) - 0.000001;
        m_Zones = m_ZoneW*m_ZoneH;
        m_ZonesC = m_ZoneCW*m_ZoneCH;
        m_PositionMatrix =  new int [m_Zones];
        m_MotionMatrix =  new int [m_Zones];
        m_AccelerationMatrix =  new int [m_Zones];
        m_CircularPositionMatrix =  new int [m_ZonesC];
        m_CircularMotionMatrix =  new int [m_ZonesC];
        m_Features.Init( 0, NULL );
        m_ActualRecord.m_nFeatures = 0;

        m_Distancias = NULL;
        m_Pares = NULL;
        nPares = 0;

        ResetMatrix();

}

moTrackerSystemData::~moTrackerSystemData() {

        if (m_PositionMatrix) {
          delete [] m_PositionMatrix;
          m_PositionMatrix = NULL;
        }

        if (m_MotionMatrix) {
          delete [] m_MotionMatrix;
          m_MotionMatrix = NULL;
        }

        if (m_CircularMotionMatrix) {
          delete [] m_CircularMotionMatrix;
          m_CircularMotionMatrix = NULL;
        }

        if (m_CircularPositionMatrix) {
          delete [] m_CircularPositionMatrix;
          m_CircularPositionMatrix = NULL;
        }
}

void moTrackerSystemData::SetMax( float x, float y ) {
    m_ActualRecord.m_Max = moVector2f(x,y);
}

void moTrackerSystemData::SetMin( float x, float y ) {
    m_ActualRecord.m_Min = moVector2f(x,y);
}

moVector2f moTrackerSystemData::GetMax() {
    return m_ActualRecord.m_Max;
}

moVector2f moTrackerSystemData::GetMin() {
    return m_ActualRecord.m_Min;
}

int moTrackerSystemData::GetFeaturesCount() {
    return m_Features.Count();
}

moTrackerFeature* moTrackerSystemData::GetFeature(int i) {
    return m_Features.GetRef(i);
}

moTrackerFeatureArray& moTrackerSystemData::GetFeatures() {
    return m_Features;
}

moVector2f moTrackerSystemData::GetBarycenter() {
        return m_ActualRecord.m_Barycenter;
}

moVector2f moTrackerSystemData::GetBarycenterMotion() {
        return m_ActualRecord.m_BarycenterMotion;
}

moVector2f moTrackerSystemData::GetBarycenterAcceleration() {
        return m_ActualRecord.m_BarycenterAcceleration;
}

moVector2f moTrackerSystemData::GetVariance() {
        return m_ActualRecord.m_Variance;
}

moVector2f moTrackerSystemData::GetSpeedVariance() {
    return m_ActualRecord.m_SpeedVariance;
}

moVector2f moTrackerSystemData::GetAccelerationVariance() {
  return m_ActualRecord.m_AccelerationVariance;
}

moVector4f moTrackerSystemData::GetBoundingRectangle() {
  return m_ActualRecord.m_BoundingRectangle;
}

int moTrackerSystemData::GetValidFeatures() {
        return m_ActualRecord.m_ValidFeatures;
}

int moTrackerSystemData::GetDeltaValidFeatures() {
        return m_ActualRecord.m_DeltaValidFeatures;
}

void moTrackerSystemData::ResetMatrix() {
    for(int i=0; i<m_Zones;i++) {
        m_PositionMatrix[i] = 0;
        m_MotionMatrix[i] = 0;
        m_AccelerationMatrix[i] = 0;
    }

    for(int i=0; i<m_Zones;i++) {
        m_CircularMotionMatrix[i] = 0;
        m_CircularPositionMatrix[i] = 0;
    }
}

///Transforma el vector de posicion (0..1) al indice de zona de la matriz cuadrada
/// x must be 0..1
int moTrackerSystemData::PositionToZone( float x, float y ) {
        float i = floor( x * (float) _zonewf );
        float j = floor( y * (float) _zonehf );

        int pos  = (int)i + ((int)j)*m_ZoneW ;

        return ( pos );
}

///Transforma el indice de zona al vector posicion
/// es el centro del cuadrado lo q devuelve!!!
moVector2f moTrackerSystemData::ZoneToPosition( int zone ) {
        int j = zone / m_ZoneW;
        int i = zone - j*m_ZoneW;
        return moVector2f( (1.0/ (2.0*float(m_ZoneW))) + (float)i / (float) m_ZoneW, (1.0/(2.0*float(m_ZoneH))) + (float)j / (float) m_ZoneH);
}

///Transforma el vector de posicion (0..1) a coordenadas polares luego al indice de zona de la matriz
int moTrackerSystemData::PositionToZoneC( float x, float y )  {

        moVector2f PosRePos;
        float Teta;
        float Radius;
        float MaxRadius;
        moVector2f MaxCuad;

        PosRePos = moVector2f( x - m_ActualRecord.m_Barycenter.X(), y - m_ActualRecord.m_Barycenter.Y() );

        if ( PosRePos.X() > 0 && PosRePos.Y() >= 0 ) {
            Teta = atan( PosRePos.Y() / PosRePos.X() );
        } else if ( PosRePos.X() > 0 && PosRePos.Y() < 0 ) {
            Teta = atan( PosRePos.Y() / PosRePos.X() ) + moMathf::TWO_PI;
        } else if( PosRePos.X() < 0 ) {
            Teta = atan( PosRePos.Y() / PosRePos.X() ) + moMathf::PI;
        } else if (PosRePos.X() == 0 && PosRePos.Y() > 0) {
            Teta = moMathf::PI / 2.0;
        } else if (PosRePos.X() == 0 && PosRePos.Y() < 0) {
            Teta = 3.0 * moMathf::PI / 2.0;
        }

        Radius = PosRePos.Length();

        MaxCuad = ( m_ActualRecord.m_Max - m_ActualRecord.m_Min ) ;
        MaxRadius = fabs( MaxCuad.Length() / 2.0); ///&????
        ( MaxRadius > 0 ) ? MaxRadius = MaxRadius : MaxRadius = 1.5; ///sqrt(2)

        float i = floor( (Teta / moMathf::TWO_PI ) * (float) _zonecwf );
        float j = floor( ( Radius / MaxRadius ) * (float) _zonechf );

        return ( (int)i + ((int)j)*m_ZoneCW );
}

///Transforma el indice de zona al vector posicion circular!
moVector2f moTrackerSystemData::ZoneToPositionC( int zone ) {

        moVector2f PosRePos;
        float Teta;
        float Radius;
        float MaxRadius;
        moVector2f MaxCuad;

        int j = zone / m_ZoneCW;
        int i = zone - j*m_ZoneCW;

        PosRePos = moVector2f( m_ActualRecord.m_Barycenter.X(), m_ActualRecord.m_Barycenter.Y() );

        MaxCuad = ( m_ActualRecord.m_Max - m_ActualRecord.m_Min ) ;
        MaxRadius = fabs( MaxCuad.Length() / 2.0 ); ///&????
        ( MaxRadius > 0 ) ? MaxRadius = MaxRadius : MaxRadius = 1.5; ///sqrt(2)

        Teta = ( moMathf::TWO_PI * i ) / _zonecwf  - moMathf::TWO_PI / (_zonecwf*2);
        Radius = ( MaxRadius * (j+1) ) / _zonechf;

        float x,y;

        x = Radius * cos ( Teta ) + PosRePos.X();
        y = Radius * sin ( Teta ) + PosRePos.Y();

        return moVector2f( x, y);
}





/*
moVector2f& pos
*/
///POSITION

void moTrackerSystemData::SetPositionMatrix( float x, float y, int nfeatures ) {
    int pos = PositionToZone(x,y);
    if ( pos >=0 && pos <m_Zones ) {
        m_PositionMatrix[pos]+=nfeatures;
    }
}

void moTrackerSystemData::SetPositionMatrix( moVector2f pos, int nfeatures ) {
    int posi = PositionToZone(pos.X(),pos.Y());
    if ( posi >=0 && posi <m_Zones ) {
      m_PositionMatrix[posi]+=nfeatures;
    }
}


int moTrackerSystemData::GetPositionMatrix( float x, float y ) {
    int pos = PositionToZone(x,y);
    if ( pos >=0 && pos <m_Zones ) return m_PositionMatrix[pos];
    return 0;
}

int moTrackerSystemData::GetPositionMatrix( moVector2f pos ) {
    int posi = PositionToZone(pos.X(),pos.Y());
    if ( posi >=0 && posi <m_Zones ) return m_PositionMatrix[posi];
    return 0;
}


///MOTION



void moTrackerSystemData::SetMotionMatrix( float x, float y, int nfeatures ) {
    int pos = PositionToZone(x,y);
    if ( pos >=0 && pos <m_Zones ) m_MotionMatrix[pos]+=nfeatures;
}

int moTrackerSystemData::GetMotionMatrix( float x, float y ) {
    int pos = PositionToZone(x,y);
    if ( pos >=0 && pos <m_Zones ) return m_MotionMatrix[pos];
    return 0;
}

void moTrackerSystemData::SetMotionMatrix( moVector2f pos, int nfeatures ) {
    int posi = PositionToZone(pos.X(),pos.Y());
    if ( posi >=0 && posi <m_Zones ) m_MotionMatrix[posi]+=nfeatures;
}

int moTrackerSystemData::GetMotionMatrix( moVector2f pos ) {
    int posi = PositionToZone(pos.X(),pos.Y());
    if ( posi >=0 && posi <m_Zones ) return m_MotionMatrix[posi];
    return 0;
}



///ACCELERATION

void moTrackerSystemData::SetAccelerationMatrix( float x, float y, int nfeatures ) {
    int pos = PositionToZone(x,y);
    if ( pos >=0 && pos <m_Zones ) m_AccelerationMatrix[pos]+=nfeatures;
}

int moTrackerSystemData::GetAccelerationMatrix( float x, float y ) {
    int pos = PositionToZone(x,y);
    if ( pos >=0 && pos <m_Zones ) return m_AccelerationMatrix[pos];
    return 0;
}

void moTrackerSystemData::SetAccelerationMatrix( moVector2f pos, int nfeatures ) {
    int posi = PositionToZone(pos.X(),pos.Y());
    if ( posi >=0 && posi <m_Zones ) m_AccelerationMatrix[posi]+=nfeatures;
}

int moTrackerSystemData::GetAccelerationMatrix( moVector2f pos ) {
    int posi = PositionToZone(pos.X(),pos.Y());
    if ( posi >=0 && posi <m_Zones ) return m_AccelerationMatrix[posi];
    return 0;
}


///POSITION CIRCULAR



void moTrackerSystemData::SetPositionMatrixC( float x, float y, int nfeatures ) {
    int pos = PositionToZoneC(x,y);
    if ( pos >=0 && pos <m_ZonesC ) m_CircularPositionMatrix[pos]+=nfeatures;
}

int moTrackerSystemData::GetPositionMatrixC( float x, float y ) {
    int pos = PositionToZoneC(x,y);
    if ( pos >=0 && pos <m_ZonesC ) return m_CircularPositionMatrix[pos];
    return 0;
}

int moTrackerSystemData::GetPositionMatrixC( moVector2f pos ) {
    int posi = PositionToZoneC(pos.X(),pos.Y());
    if ( posi >=0 && posi <m_ZonesC ) return m_CircularPositionMatrix[posi];
    return 0;
}

int moTrackerSystemData::GetPositionMatrixC( int zone ) {
    if ( zone >=0 && zone <m_ZonesC ) return m_CircularPositionMatrix[zone];
    return 0;
}


void moTrackerSystemData::SetMotionMatrixC( float x, float y, int nfeatures ) {
    int pos = PositionToZoneC(x,y);
    if ( pos >=0 && pos <m_ZonesC ) m_CircularMotionMatrix[pos]+=nfeatures;
}

int moTrackerSystemData::GetMotionMatrixC( float x, float y ) {
    int pos = PositionToZoneC(x,y);
    if ( pos >=0 && pos <m_ZonesC ) return m_CircularMotionMatrix[pos];
    return 0;
}

int moTrackerSystemData::GetMotionMatrixC( moVector2f pos ) {
    int posi = PositionToZoneC(pos.X(),pos.Y());
    if ( posi >=0 && posi <m_ZonesC ) return m_CircularMotionMatrix[posi];
    return 0;
}
int moTrackerSystemData::GetMotionMatrixC( int zone ) {
    if ( zone >=0 && zone <m_ZonesC ) return m_CircularMotionMatrix[zone];
    return 0;
}




/*************************

*
        DRAW FEATURES

        values are supposed to be between 0.0 and 1.0
        normalized values means:
        0.0,0.0 = > pixel ( 0, 0 )
        1.0,1.0 = > pixel ( width, height )

*

****************************/

#include "moDebugManager.h"

void moTrackerSystemData::DrawFeatures( float w, float h, float offsetx, float offsety ) {

    float x=0.0,y=0.0;
    float d = 1.0;

    //m_pResourceManager->GetGuiMan()->DisplayInfoWindow( m_DisplayX , m_DisplayY, m_DisplayW, m_DisplayH, pInfo );

    glColor4f( 1.0, 0.0, 0.0, 1.0 );

#ifndef OPENGLESV2
    glDisable( GL_TEXTURE_2D );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    //glEnable( GL_BLEND );
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glLineWidth((GLfloat)2.0);
    glBegin(GL_QUADS);
        glVertex2f((x - 0.5*d)*w, (y - 0.5*d)*h);
        glVertex2f((x - 0.5*d)*w, (y + 0.5*d)*h);
        glVertex2f((x + 0.5*d)*w, (y + 0.5*d)*h);
        glVertex2f((x + 0.5*d)*w, (y - 0.5*d)*h);
    glEnd();

    glLineWidth((GLfloat)1.0);
    glBegin(GL_LINES);

        glVertex2f( x*w, (y - 0.1*d)*h);
        glVertex2f( x*w, (y + 0.1*d)*h);

        //flecha (puntita arriba en el eje Y)
        glVertex2f( x*w, (y + 0.1*d)*h);
        glVertex2f( (x-0.0008*d)*w, (y + 0.1*d - 0.0008*d)*h);

        glVertex2f((x - 0.1*d)*w, y*d*h);
        glVertex2f((x + 0.1*d)*w, y*d*h);

        //flecha (puntita )
        glVertex2f((x + 0.1*d)*w, y*d*h);
        glVertex2f((x + 0.1*d - 0.0008*d)*w, (y+0.0008)*h);


    glEnd();

#endif // OPENGLESV2

    if (GetFeaturesCount()>0) {

            for (int f = 0; f < GetFeaturesCount(); f++)
            {

                moTrackerFeature* pF = GetFeature(f);

                ///solo dibujamos los que son validos...
                if (pF && pF->val>=MO_TRACKER_TRACKED) {
              //if (pF) {

                x = (pF->x) - offsetx;
                y = (pF->y) - offsety;
                float tr_x = (pF->tr_x) - offsetx;
                float tr_y = (pF->tr_y) - offsety;
                ///float v_x = (pF->v_x);
                ///float v_y = (pF->v_y);
                ///float vel = sqrtf( v_x*v_x+v_y*v_y );
                int v = pF->val;
                ///int is_parent = pF->is_parent;

                //MODebug2->Log(moText("    x:")+FloatToStr(pF->x) + moText(" y:")+FloatToStr(pF->y) );

                glBindTexture(GL_TEXTURE_2D,0);
                glColor4f(1.0, 0.0, 0.0, 0.15);

                if (v >= MO_TRACKER_TRACKED) glColor4f(0.0, 1.0, 0.0, 1.0);
                else if (v == MO_TRACKER_NOT_FOUND) glColor4f(1.0, 0.0, 1.0, 1.0);
                else if (v == MO_TRACKER_SMALL_DET) glColor4f(1.0, 0.0, 1.0, 1.0);
                else if (v == MO_TRACKER_MAX_ITERATIONS) glColor4f(1.0, 0.0, 1.0, 1.0);
                else if (v == MO_TRACKER_OOB) glColor4f(1.0, 0.0, 1.0, 1.0);
                else if (v == MO_TRACKER_LARGE_RESIDUE) glColor4f(1.0, 0.0, 1.0, 1.0);

                if ( pF->val>=MO_TRACKER_TRACKED ) {

                    if (f==0) {
                      d = 3.0;
                      glColor4f(1.0, 0.0, 0.0, 1.0);
                    }
#ifndef OPENGLESV2
                    glBegin(GL_QUADS);
                        glVertex2f((x - 0.008*d)*w, (y - 0.008*d)*h);
                        glVertex2f((x - 0.008*d)*w, (y + 0.008*d)*h);
                        glVertex2f((x + 0.008*d)*w, (y + 0.008*d)*h);
                        glVertex2f((x + 0.008*d)*w, (y - 0.008*d)*h);
                    glEnd();
#endif


                    glColor4f(1.0, 1.0, 1.0, 1.0);
                    if (f==0) {
                      glColor4f(1.0, 0.0, 0.0, 1.0);
                    }
                    glBindTexture(GL_TEXTURE_2D,0);
                    glDisable(GL_TEXTURE_2D);
                    glLineWidth((GLfloat)1.2);
#ifndef OPENGLESV2

                    glBegin(GL_LINES);
                        glVertex2f( x*w, y*h);
                        glVertex2f( tr_x*w, tr_y*h);
                    glEnd();
#endif

                    /*if ( vel > 0.001 ) {
                        glDisable(GL_TEXTURE_2D);
                        glColor4f(0.0, 0.0, 1.0, 1.0);
                        //glPointSize((GLfloat)10);
                        glLineWidth((GLfloat)10.0);
                        glBegin(GL_LINES);
                            glVertex2f( x*w, y*h);
                            glVertex2f( x*w+v_x*w, y*h+v_y*h);
                        glEnd();
                    }*/


                }


            }


    /***********************************************************************************
    **          UPLAS - JUNTANDO VECINOS
    ***********************************************************************************/
/*
            float p1x,p1y;
            float p2x,p2y;
            int k1, k2;
            int Uplas[GetFeaturesCount()];
            ///Reconocer los que tienen un par, de los que tienen 2
            for( int g = 0; g < nPares; g++ ) {
                k1 = m_Pares[g][0];
                k2 = m_Pares[g][1];
                Uplas[k1]++;
                Uplas[k2]++;

                p1x = GetFeature(k1)->x - offsetx;
                p1y = GetFeature(k1)->y - offsety;

                p2x = GetFeature(k2)->x - offsetx;
                p2y = GetFeature(k2)->y - offsety;


                glColor4f(1.0, 1.0, 1.0, 0.1);
                glDisable(GL_TEXTURE_2D);
                glLineWidth((GLfloat)1.0);
                glBegin(GL_LINES);
                    glVertex2f( p1x*w, p1y*h);
                    glVertex2f( p2x*w, p2y*h);
                glEnd();

            }

            int myuplas[3];
            for( int u=0; u<GetFeaturesCount(); u++) {
                if( Uplas[u]>=2) {

                    p1x = GetFeature(u)->x - offsetx;
                    p1y = GetFeature(u)->y - offsety;

                    ///DrawUpla(u);
                    ///search for all points!
                    if (Uplas[u]==3) {
                        int o = 0;
                        moVector2f aver(p1x,p1y);
                        for( int pp=0; pp<nPares; pp++) {
                            k1 = m_Pares[pp][0];
                            k2 = m_Pares[pp][1];
                            if (k1==u) myuplas[o++] = k2;
                            if (k2==u) myuplas[o++] = k1;
                        }
                        for(int pp=0;pp<3;pp++) {
                            aver+= moVector2f( GetFeature(myuplas[pp])->x - offsetx,
                                            GetFeature(myuplas[pp])->y - offsety );
                        }
                        aver/=4;
                        moVector2f aver2;
                        for(int pp=0;pp<3;pp++) {
                            aver2 = aver - moVector2f( GetFeature(myuplas[pp])->x - offsetx,
                                            GetFeature(myuplas[pp])->y - offsety );
                        }

                        glColor4f(1.0, 1.0, 0.0, 0.1);
                        glBegin(GL_QUADS);
                            glVertex2f((aver.X() - 0.02)*w, (aver.Y() - 0.02)*h);
                            glVertex2f((aver.X() - 0.02)*w, (aver.Y() + 0.02)*h);
                            glVertex2f((aver.X() + 0.02)*w, (aver.Y()+ 0.02)*h);
                            glVertex2f((aver.X() + 0.02)*w, (aver.Y() - 0.02)*h);
                        glEnd();
                    }
              }
            }

*/
    /***********************************************************************************
    **          VARIANCE - BARYCENTER - ETC
    ***********************************************************************************/

            /** Only show segments */
#ifndef OPENGLESV2
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
#endif
            moVector2f m_TrackerBarycenter = moVector2f(    GetBarycenter().X() - offsetx,
                                                            GetBarycenter().Y() - offsety );
            moVector2f m_TrackerMin = moVector2f(   GetMin().X() - offsetx,
                                                    GetMin().Y() - offsety );
            moVector2f m_TrackerMax = moVector2f(   GetMax().X() - offsetx,
                                                    GetMax().Y() - offsety );


            glBindTexture(GL_TEXTURE_2D,0);
            glDisable(GL_TEXTURE_2D);
            glColor4f(1.0, 1.0, 0.0, 1.0);  //yellow

            //GL_MAX_TEXTURE_UNITS_ARB
            //GL_TEXTURE0_ARB
#ifndef OPENGLESV2
            glBegin(GL_QUADS);
                //glColor4f(0.7, 1.0, 0.5, 0.2);
                glVertex2f((m_TrackerBarycenter.X() - 0.02)*w, (m_TrackerBarycenter.Y() - 0.02)*h);
                //glColor4f(0.7, 1.0, 0.5, 0.2);
                glVertex2f((m_TrackerBarycenter.X() - 0.02)*w, (m_TrackerBarycenter.Y() + 0.02)*h);
                //glColor4f(0.7, 1.0, 0.5, 0.2);
                glVertex2f((m_TrackerBarycenter.X() + 0.02)*w, (m_TrackerBarycenter.Y() + 0.02)*h);
                //glColor4f(0.7, 1.0, 0.5, 0.2);
                glVertex2f((m_TrackerBarycenter.X() + 0.02)*w, (m_TrackerBarycenter.Y() - 0.02)*h);
            glEnd();
#endif
            moVector2f m_TrackerVariance = moVector2f(  GetVariance().X(),
                                                        GetVariance().Y());

            ///Max min  green
            glColor4f(0.0, 1.0, 0.0, 1.0);
            glLineWidth((GLfloat)1.0);
#ifndef OPENGLESV2
            glBegin(GL_QUADS);
                //glColor4f(0.7, 1.0, 0.5, 0.5);
                glVertex2f( m_TrackerMin.X()*w, m_TrackerMax.Y()*h);
                glVertex2f( m_TrackerMax.X()*w, m_TrackerMax.Y()*h);
                glVertex2f( m_TrackerMax.X()*w, m_TrackerMin.Y()*h);
                glVertex2f( m_TrackerMin.X()*w, m_TrackerMin.Y()*h);
            glEnd();
#endif

            ///Variance cyan
            glColor4f(0.0, 1.0, 1.0, 1.0);
            glLineWidth((GLfloat)1.5);
#ifndef OPENGLESV2
            glBegin(GL_QUADS);
                glVertex2f( m_TrackerBarycenter.X()*w - m_TrackerVariance.X()*w*5, m_TrackerBarycenter.Y()*h-m_TrackerVariance.Y()*h*5);
                glVertex2f( m_TrackerBarycenter.X()*w + m_TrackerVariance.X()*w*5, m_TrackerBarycenter.Y()*h-m_TrackerVariance.Y()*h*5);
                glVertex2f( m_TrackerBarycenter.X()*w + m_TrackerVariance.X()*w*5, m_TrackerBarycenter.Y()*h+m_TrackerVariance.Y()*h*5);
                glVertex2f( m_TrackerBarycenter.X()*w - m_TrackerVariance.X()*w*5, m_TrackerBarycenter.Y()*h+m_TrackerVariance.Y()*h*5);
            glEnd();
#endif
            /*
            moDebugManager::Push(moText("varianza: vx:")+FloatToStr(m_TrackerVariance.X()*w*10)
                             +moText(" vy")+FloatToStr(m_TrackerVariance.Y()*h*10) );
*/

    /***********************************************************************************
    **          ZONES
    ***********************************************************************************/

            glEnable( GL_BLEND );
            glDisable(GL_TEXTURE_2D);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE);


            moText matrix;
            int npz,nmz;
            int n = GetValidFeatures();
            int z;
            float off_w, off_h, off_w_m, off_h_m;
            off_w = off_h = 1.0 / 8.0;

            if ( m_ZoneW > 0 ) {
              off_w = 1.0 / ( (float)(m_ZoneW)*2.0 );
            }
            if ( m_ZoneH > 0 ) {
              off_h = 1.0 / ( float(m_ZoneH)*2.0 );
            }
            off_w_m = off_w - off_w/8.0;
            off_h_m = off_h - off_h/8.0;

            /**RECTANGULAR MATRIX*/

            for(z=0; z<m_Zones; z++) {

                    moVector2f poscuad = ZoneToPosition(z);

                    npz = GetPositionMatrix( poscuad );
                    nmz = GetMotionMatrix( poscuad );

                    //matrix = matrix + moText(" ")+IntToStr(npz);
                    poscuad = moVector2f( poscuad.X()-offsetx, poscuad.Y()-offsety);
                    if ( npz > 0 ) {
#ifndef OPENGLESV2
                            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
#endif
                            //glColor4f( 0.5+0.5*(float)z/(m_Zones-1), 0.5+0.5*(float)z/(m_Zones-1), 0.0, 0.25);
                            glColor4f(0.5, 0.5, 0.0, 0.002);
#ifndef OPENGLESV2
                            glBegin(GL_QUADS);
                                glVertex2f((poscuad.X() - off_w)*w, (poscuad.Y() - off_h)*h);
                                glVertex2f((poscuad.X() - off_w)*w, (poscuad.Y() + off_h)*h);
                                glVertex2f((poscuad.X() + off_w)*w, (poscuad.Y() + off_h)*h);
                                glVertex2f((poscuad.X() + off_w)*w, (poscuad.Y() - off_h)*h);
                            glEnd();
#endif
                    }
                    if ( nmz > 0 ) {

                            ///DRAW ZONE WITH FEATURES IN MOTION
#ifndef OPENGLESV2
                            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
#endif
                            //glColor4f( 0.0, 0.0, 0.5+0.5*(float)z/(m_Zones-1), 0.25);
                            glColor4f(0.0, 0.0, 0.5, 0.02);
#ifndef OPENGLESV2
                            glBegin(GL_QUADS);
                                glVertex2f((poscuad.X() - off_w_m)*w, (poscuad.Y() - off_h_m)*h);
                                glVertex2f((poscuad.X() - off_w_m)*w, (poscuad.Y() + off_h_m)*h);
                                glVertex2f((poscuad.X() + off_w_m)*w, (poscuad.Y() + off_h_m)*h);
                                glVertex2f((poscuad.X() + off_w_m)*w, (poscuad.Y() - off_h_m)*h);
                            glEnd();
#endif

                    }
            }

            /**CIRCULAR MATRIX*/
#ifndef OPENGLESV2
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
#endif
            if (n>0)
            for(int cc=0; cc < m_ZonesC; cc++) {

                    moVector2f poscuadC = ZoneToPositionC(cc);
                    moVector2f p2,p3,p4;
                    if (((cc+1)%m_ZoneCW)==0) { p2 = ZoneToPositionC( cc - m_ZoneCW -1); }
                    else { p2 = ZoneToPositionC( cc + 1 ); }

                    if (cc<12) {
                        p3 = GetBarycenter();
                        p4 = GetBarycenter();
                    }
                    else {
                        if (((cc+1)%m_ZoneCW)==0) { p3 = ZoneToPositionC( cc - m_ZoneCW -1 - m_ZoneCW); }
                        else { p3 = ZoneToPositionC( cc + 1 - m_ZoneCW); }
                        p4 = ZoneToPositionC( cc - m_ZoneCW );

                    }

                    npz = GetPositionMatrixC( cc );
                    nmz = GetMotionMatrixC( cc );

                    //matrix = matrix + moText(" ")+IntToStr(npz);
                    poscuadC = moVector2f( poscuadC.X()-offsetx, poscuadC.Y()-offsety);
                    p2 = moVector2f( p2.X()-offsetx, p2.Y()-offsety );
                    p3 = moVector2f( p3.X()-offsetx, p3.Y()-offsety );
                    p4 = moVector2f( p4.X()-offsetx, p4.Y()-offsety );
                   if ( npz == 0 ) {
                            glColor4f( 0.3+0.2*(float)cc/(m_ZonesC-1), 0.3+0.2*(float)cc/(m_ZonesC-1), 0.2, 0.0015);
#ifndef OPENGLESV2
                            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
#endif
                   } else {
                       glColor4f( 0.7+0.3*(float)cc/(m_ZonesC-1), 0.0, 0.0, 0.0025);
#ifndef OPENGLESV2
                       glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
#endif
                    }


                            //moVector2f p3 = m_pTrackerData->ZoneToPositionC( cc + 1 + 12 );
                            //moVector2f p4 = m_pTrackerData->ZoneToPositionC( cc + 12 );
#ifndef OPENGLESV2
                            glBegin(GL_QUADS);
                                glVertex2f( poscuadC.X()*w, poscuadC.Y()*h);
                                glVertex2f( p2.X()*w, p2.Y()*h);
                                glVertex2f( p3.X()*w, p3.Y()*h);
                                glVertex2f( p4.X()*w, p4.Y()*h);
                                //glVertex2f( p3.X()*w, p3.Y()*h);
                                //glVertex2f( p4.X()*w, p4.Y()*h);
                            glEnd();
#endif

                   //}
                    if ( nmz > 0 ) {
                            glColor4f( 0.0, 0.0, 0.5+0.5*(float)cc/(m_ZonesC-1), 0.0025);

                    }
            }

            //MODebug2->Push(moText("N:")+(moText)IntToStr(m_pTrackerData->GetValidFeatures())+moText("matrix: vx:")+(moText)matrix);

        }

    }

    #ifndef OPENGLESV2
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    #endif

}

/*!
\fn moTrackerFeature::moTrackerFeature()
\brief Constructor
*/
moTrackerFeature::moTrackerFeature()
{
	x = -1; y = -1; valid = false;
	track.Empty();
	is_object = false;
	is_parent = false;
	is_cursor = false;
	stime = 0;
	sframe = 0;
	utime = 0;
	uframe = 0;
	Parent = NULL;
}


moTrackerFeature::~moTrackerFeature()
{
}


moTrackerFeature & moTrackerFeature::operator = (const moTrackerFeature &src) {

  x = src.x;
  y =  src.y;

  normx = src.normx;
  normy = src.normy;

  tr_x = src.tr_x;
  tr_y = src.tr_y;

  v_x = src.v_x;
  v_y = src.v_y;
  vp_x = src.vp_x;
  vp_y = src.vp_y;
  a_x = src.a_x;
  a_y = src.a_y;
  ap_x = src.ap_x;
  ap_y = src.ap_y;
  t_x = src.t_x;
  t_y = src.t_y;

  val = src.val;
  valid = src.valid;

  track = src.track;

  FeaturesCaptured = src.FeaturesCaptured;
  Parent = src.Parent;

  is_object = src.is_object;
  is_parent = src.is_parent;
  is_cursor = src.is_cursor;
  stime = src.stime;
  sframe = src.sframe;
  utime = src.utime;
  uframe = src.uframe;
  return (*this);
}

/*!
\fn void GpuKLT_Feature::print();
\brief Display Feature Details
*/
void moTrackerFeature::print()
{
	printf("Pos:\t%10.5f,%10.5f\t(%7.2f,%7.2f)\n",normx,normy,x,y);
}

/*!
\fn void GpuKLT_Feature::printTracks()
\brief Display Feature Tracks
*/
void moTrackerFeature::printTracks()
{
	MOuint	n = track.Count();

	if (n==0)
		printf("(%7.2f,%7.2f)\n", x, y);
	else
		printf("(%7.2f,%7.2f) -> (%7.2f,%7.2f)\n", track[n-1]->X(), track[n-1]->Y(), x, y);
}



/*!
\fn int moTrackerFeature::updatePos(float kltConvergeThreshold, float kltSSDthresh, int kltborder, float delta, float res, float d1, float d2, float w, float h)
\brief Update the Feature Position (d1,d2) are the delta_x and delta_y resp.
*/
int moTrackerFeature::updatePos(float kltConvergeThreshold, float kltSSDthresh, int kltborder, float delta, float res, float d1, float d2, float w, float h)
{

  kltConvergeThreshold = 0.0; ///unused
  delta = 0.0; ///unused

	double delx, dely;
	bool  discardFlag;

	if (res > kltSSDthresh)
		discardFlag = true;
	else if (d1 < 0.0 && d2 < 0.0)
		discardFlag = true;
	else
	{
		moVector2f * pt = new moVector2f(x,y);
		track.Add(pt);

		delx         = normx - d1;
		dely         = normy - d2;
		normx        = normx - delx;
		normy		 = normy - dely;
		x			 = normx * w;
		y			 = normy * h;

		if ( (x < kltborder) || (x > (w - kltborder)) || (y < kltborder) || (y > (h - kltborder)) )
			discardFlag = true;
		else
			discardFlag = false;
	}

	if (discardFlag)
	{

		//for (int j=0;j<track.size();j++)
		//	delete track[j];
		track.Empty();
		valid = false;
		return -1;
	}
	else
		return 1;
}


//===========================================
//
//				moFilter
//
//===========================================




//===========================================
//
//				moFilterManager
//
//===========================================

moFilterManager::moFilterManager() {

	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_FILTER );

	SetName("filtermanager");
	SetLabelName("filtermanager");

}

moFilterManager::~moFilterManager() {

}

MOboolean moFilterManager::Init() {
	return true;
}


MOboolean moFilterManager::Finish() {
	return true;
}

