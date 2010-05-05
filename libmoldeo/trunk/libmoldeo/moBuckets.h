/*******************************************************************************

								moBuckets.h

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

  Class:
	moBuckets

  Description:
	Bucket's class, accumulation buffer

*******************************************************************************/

#ifndef __MO_BUCKETS_H
#define __MO_BUCKETS_H
#include "moTypes.h"
#include "moLock.h"

#define MAX_BUCKETS 2

/// Espacio en memoria para compartir datos entre objetos
/**
 *   Memoria o "balde" que permite el acceso asincrnico a objetos como son cuadros reproducidos de video u otro tipo de datos.
 *   Es a su vez un nodo de una lista enlazada.
 *   @see moBucketsPool
 *   @see moLock
 */
class LIBMOLDEO_API moBucket
{
	public:moBucket ();
	virtual ~ moBucket ();

	/// Paraliza el acceso al buffer interno
	/**
	 *   Utiliza un moLock para proteger los datos para el acceso asincrnico
	 *   @see moLock
	 *   @return verdadero si fue exitoso o falso si no
	 */
	bool Lock ();

	/// Libera el acceso al buffer interno
	/**
	 *   Utiliza un moLock para proteger los datos para el acceso asincrnico.
	 *   @see moLock
	 *   @return verdadero si fue exitoso o falso si no
	 */
	bool Unlock ();				 ///< Libera el acceso al buffer interno

	/// Devuelve el puntero al buffer de datos
	/**
	 *   @return puntero al buffer
	 */
	MOubyte *GetBuffer ();		 //gets the bucket's buffer pointer

	/// Devuelve el tamao en bytes asignado por el buffer
	/**
	 *   @return tamao en bytes del buffer
	 */
	MOlong GetSize ();

	/// Crea un espacio de memoria y asigna los valores desde un puntero a otro espacio de memoria
	/**
	 *   @param size tamao del buffer de memoria, en bytes
	 *   @param pbuf puntero de dnde copiar los datos al nuevo espacio de memoria
	 *   @param setvalue valor con el que fijar el buffer recien creado
	 */
	void SetBuffer (MOlong size, MOubyte * pbuf);

	/// Copia al espacio de memoria los valores de otro espacio de memoria
	/**
	 *   @param size tamao del buffer de memoria a copiar, en bytes
	 *   @param pbuf puntero de dnde copiar los datos al nuevo espacio de memoria
	 */
								 //copy size bytes from the bucket's buffer to pbuf
	void Copy (MOlong size, MOubyte * pbuf);

	/// Enlaza un moBucket
	/**
	 *   @param pbucket puntero al moBucket a enlazar
	 */
	void AttachBucket (moBucket * pbucket);

	/// Devuelve el moBucket enlazado a este
	/**
	 *   @return puntero al moBucket enlazado
	 */
								 //get the bucket's linked bucked
	moBucket * GetAttachedBucket ();

	/// Libera el espacio de memoria
	/**
	 *   No liberar memoria si no fue alojada anteriormente.
	 */
	void EmptyBucket ();		 //free the bucket's buffer

	/// Habilita el buffer en memoria con el valor prefijado
	/**
	 *   @param size tamao del buffer de memoria, en bytes
	 *   @param setvalue valor con el que fijar el buffer recien creado
	 */
	void BuildBucket (MOlong size, int setvalue);
	protected: MOlong m_lBufferSize;
	///< tamao del espacio en memoria (buffer)
	bool m_bAlligned;			 ///< alineacin de la memoria, siempre debe estar alineada
	bool m_bEmpty;				 ///< indicador si el buffer est vaco o lleno (con datos)
	moLock m_Lock;				 ///< semforo para el acceso asincrnico
	MOubyte *m_pBuffer;			 ///< puntero al espacio en memoria
	moBucket *m_pAttachedBucket; ///< moBucket enlazado
								 ///< moBucket enlazado para upstreaming
	moBucket * m_pAttachedBucketUp;
								 ///< moBucket enlazado para downstreaming
	moBucket *m_pAttachedBucketDown;
};

//SPECIAL FOR NO LOOSING DATA.....

/// Administrador de moBucket 's
/**
 *   Objeto para administrar la lista de moBucket 's.
 *   El manejo es del tipo FIFO. First In, First Out. Primero que entra, primero que sale.
 *   No pierde cuadros ni memoria, siempre y cuando se use la funcin RetreiveBucket
 */
class LIBMOLDEO_API moBucketsPool
{								 //Last In Last Out
	public:moBucketsPool ();
	virtual ~ moBucketsPool ();
								 //add a created bucket to the pool
	bool AddBucket (moBucket * pBucket);
								 //take an empty created bucket and fill it with pbuf
	bool AddBucket (MOlong size, MOubyte * pbuf);
	moBucket *GetEmptyBucket (); //get an empty bucket to fill it
	moBucket *RetreiveBucket (); //retreive from the pool the filled bucket to go
	moBucket *GetActualBucket ();//gets from the pool the pointer to the filled bucket to go, but do not retreive it!!! so beware
	moBucket *RetreivedBucket ();//get the pointer to the retreived bucket
								 //deletes the retreivedbucket...
	bool DestroyRetreivedBucket ();
	bool IsEmpty ();
	bool IsFull ();
	protected:moBucket * m_pRetreivedBucket;
	MOlong m_nBuckets;
	MOlong m_lMaxBuckets;		 //60MB = 60 buckets
	moLock m_PoolLock;			 //lock access to pool
	moBucket *m_pFirstBucketToGo;
	moBucket * m_pLastBucketToGo;
	moBucket * m_pFirstEmptyBucket;
	moBucket * m_pLastEmptyBucket;
	MOlong m_nEmptyBuckets;
};

/*
//SPECIAL FOR LIVE DATA.....
class LIBMOLDEO_API moBucketsStack {//Last In First Out

	public:
		moBucketsStack();
		~moBucketsStack();

		void AddBucket( moBucket *pBucket );
		moBucket *RetreiveBucket();

	protected:
		moBucket *m_pFirstBucketToGo;
		moBucket *m_pLastBucketToGo;

};

//SPECIAL FOR SCRATCH VIDEOS OR STREAMING.....
class LIBMOLDEO_API moBucketsBuffer {//arbitrary access

	public:
		moBucketsBuffer();
		~moBucketsBuffer();

		Cyclic();

		void AddBucket( moBucket *pBucket );
		moBucket *RetreiveBucket();

	protected:
		moBucket *m_pFirstBucketToGo;
		moBucket *m_pLastBucketToGo;

};
*/
#endif							 /*  */
