/*******************************************************************************

                              moBuckets.cpp

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

#include "moBuckets.h"

moBucket::moBucket() {
	m_pBuffer = NULL;
	m_pAttachedBucket = NULL;
	m_bEmpty = true;
	m_lBufferSize = 0;
}

moBucket::~moBucket() {

}


bool moBucket::Lock() {
	return m_Lock.Lock();
}

bool moBucket::Unlock() {
	return m_Lock.Unlock();
}

MOlong moBucket::GetSize() {
	return m_lBufferSize;
}


MOubyte *moBucket::GetBuffer() {
	return m_pBuffer;
}

void moBucket::EmptyBucket() {
	Lock();
	if(m_lBufferSize!=0 && m_pBuffer!=NULL) {
		delete [] m_pBuffer;
		m_lBufferSize = 0;
	}
	m_bEmpty = true;
	Unlock();
}

void moBucket::BuildBucket( MOlong size , int setvalue ) {
    Lock();
	m_lBufferSize = size;
	m_pBuffer = new MOubyte [m_lBufferSize];
	memset((void*)m_pBuffer,setvalue,  m_lBufferSize );
	m_bEmpty = false;
	Unlock();
}


void moBucket::SetBuffer( MOlong size, MOubyte *pbuf) {
	Lock();
	m_lBufferSize = size;
	m_pBuffer = new MOubyte [m_lBufferSize];
	memcpy((void*)m_pBuffer,(const void*) pbuf,  m_lBufferSize );
	m_bEmpty = false;
	Unlock();
}


void moBucket::Copy( MOlong size , MOubyte *pbuf ) {
	Lock();
	if(m_pBuffer && pbuf &&(m_lBufferSize>=size) )  {
		memcpy((void*)m_pBuffer,(const void*) pbuf,  size );
		m_bEmpty = false;
	}
	Unlock();
}


void moBucket::AttachBucket( moBucket *pbucket) {
	Lock();
	if(m_pAttachedBucket==NULL)
		m_pAttachedBucket = pbucket;
	Unlock();
}

moBucket* moBucket::GetAttachedBucket() {
	return m_pAttachedBucket;
}


//================================================
//  BUCKETS POOL
//================================================

moBucketsPool::moBucketsPool() {
	m_pRetreivedBucket = NULL;
	m_pFirstBucketToGo = m_pLastBucketToGo = NULL;
	m_pFirstEmptyBucket = m_pLastEmptyBucket = NULL;
	m_nBuckets = 0;
	m_lMaxBuckets = 1;
}

moBucketsPool::~moBucketsPool() {
	while(RetreiveBucket()!=NULL) {
		DestroyRetreivedBucket();
	}
}

bool
moBucketsPool::IsEmpty() {
	return( m_nBuckets == 0 );
}

bool
moBucketsPool::IsFull() {
	return( m_nBuckets == m_lMaxBuckets );
}



bool moBucketsPool::AddBucket(moBucket *pBucket ) {

	if((pBucket==NULL) ||(m_nBuckets == m_lMaxBuckets) )
		return false;


	m_PoolLock.Lock();
	if( m_nBuckets == 0) {//lista vacia
		m_pFirstBucketToGo = pBucket;
		m_pLastBucketToGo = pBucket;
	} else {
		m_pLastBucketToGo->AttachBucket( pBucket );
		m_pLastBucketToGo = m_pLastBucketToGo->GetAttachedBucket();
	}
	m_nBuckets++;
	m_PoolLock.Unlock();
	return true;

}

bool moBucketsPool::AddBucket( MOlong size , MOubyte *pbuf ) {//take an empty created bucket and fill it with pbuf
/*	moBucket*	pB = NULL;

	if(!IsFull()) {
		pB = GetEmptyBucket();
		if(pB!=NULL) {//EMPTY TO FILL
			if(pB->GetSize() >= size)
				pB->Copy( size, pbuf );
			else return false;
		} else {//CREATE
			pB = new moBucket();
			if(pB)
				pB->SetBuffer( size, pbuf );//create buffer and fill it
			else return false;
		}
	} else return false;
	if(pB!=NULL)
		return AddBucket(pB);
	else */
		return false;
}

moBucket*	moBucketsPool::GetEmptyBucket() {//get an empty bucket to fill it
	m_PoolLock.Lock();

	moBucket *emptyone = NULL;
/*
	emptyone = m_pFirstEmptyBucket;

	if(m_pFirstEmptyBucket==NULL) {
		return NULL;
	} else if(m_pFirstEmptyBucket->GetAttachedBucket()==NULL) {
		m_pFirstEmptyBucket = NULL;
		m_pLastEmptyBucket = NULL;
	} else if(m_pEmptyBuckets->GetAttachedBucket()!=NULL) {
		m_pFirstEmptyBucket = m_pFirstEmptyBucket->GetAttachedBucket();
	}*/
	m_PoolLock.Unlock();
	return emptyone;
}


moBucket *moBucketsPool::RetreiveBucket() {
	m_PoolLock.Lock();

	moBucket* retreived = NULL;

	retreived = m_pFirstBucketToGo;
		//testing, unicamente el primer cuadro...
	//reacomodamos
	if( m_nBuckets == 0 ) {
		retreived = NULL;
	} else if( m_nBuckets == 1 ) {//solo uno en la lista
		m_pFirstBucketToGo = NULL;
		m_pLastBucketToGo = NULL;//la lista queda anulada
	} else if( m_nBuckets > 1 ) {
		m_pFirstBucketToGo = m_pFirstBucketToGo->GetAttachedBucket();//default
	}
	if(m_nBuckets>0) {
		m_nBuckets--;
		//caso de haber sacado un bucket, lo ponemos en la lista de los vacios...
		/*
		if( m_pLastEmptyBucket == NULL ) {
			m_pFirstEmptyBucket = m_pLastEmptyBucket = retreived;
			m_pFirstEmptyBucket->AttachBucket(NULL);
		} else {
			m_pLastEmptyBucket->AttachBucket(retreived);
			m_pLastEmptyBucket = retreived;
		}	*/
	}

	m_pRetreivedBucket = retreived;

	m_PoolLock.Unlock();
	return retreived;
}

moBucket *moBucketsPool::RetreivedBucket() {
	return m_pRetreivedBucket;
}

bool moBucketsPool::DestroyRetreivedBucket() {
	if (m_pRetreivedBucket) {
		m_pRetreivedBucket->EmptyBucket();
		delete m_pRetreivedBucket;
		m_pRetreivedBucket = NULL;
	}
	if (m_pRetreivedBucket==NULL) { return true; }
	else  { return false; }
}

moBucket *moBucketsPool::GetActualBucket() {
	m_PoolLock.Lock();

	moBucket* retreived = NULL;

	retreived = m_pFirstBucketToGo;

	m_PoolLock.Unlock();
	return retreived;
}
