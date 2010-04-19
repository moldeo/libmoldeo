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
  Andrés Colubri

*******************************************************************************/

#include "moFilterManager.h"

#include "moArray.h"
moDefineDynamicArray(moTrackerFeatureArray)


/*!
\fn moTrackerFeature::moTrackerFeature()
\brief Constructor
*/
moTrackerFeature::moTrackerFeature()
{
	x = -1; y = -1; valid = false;
	track.Empty();
}


moTrackerFeature::~moTrackerFeature()
{
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

	SetName("Filter Manager");

}

moFilterManager::~moFilterManager() {

}

MOboolean moFilterManager::Init() {
	return true;
}


MOboolean moFilterManager::Finish() {
	return true;
}



