/*******************************************************************************

                                moFileManager.h

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

#ifndef __MOFILEMANAGER_H
#define __MOFILEMANAGER_H

#include "moTypes.h"
#include "moAbstract.h"
#include "moText.h"
#include "moResourceManager.h"
#include "moBuckets.h"
#include "moParam.h"
#include "moOGLFT.h"

/**
* Los distintos tipos de accesos a los archivos soportados por el moFileManager
*/

enum moFileType {
	MO_FILETYPE_LOCAL,
	MO_FILETYPE_HTTP,
	MO_FILETYPE_FTP,
	MO_FILETYPE_HTTPS,
	MO_FILETYPE_FTPS,
	MO_FILETYPE_UDP,
	MO_FILETYPE_UTP
};

#ifdef MO_WIN32
    #define moSlash moText("/")
#endif

#ifdef MO_LINUX
    #define moSlash moText("/")
#endif

#ifdef MO_MACOSX
    #define moSlash moText("/")
#endif


/**
* Estado del archivo
*/
enum moFileStatus {
	MO_FILESTATUS_READY,
	MO_FILESTATUS_DOWNLOADING,
	MO_FILESTATUS_STREAMING,
	MO_FILESTATUS_NOT_READY
};

/**
* clase para el manejo de un archivo
*/

class LIBMOLDEO_API moFile : public moAbstract {

	public:
		moFile();
		moFile( moText p_CompletePath );//could be: http://.... or ftp://... or c:\... or ...
		virtual ~moFile();

		virtual MOboolean Init();
		virtual MOboolean Finish();

		MOboolean Exists();

		moFileType GetType();
		MOboolean IsRemote();
		void SetType( moFileType p_filetype );
		void	SetCompletePath( moText p_completepath );

		moText	GetFileName();
		moText	GetPath();
		moText	GetCompletePath();
		moText	GetExtension();
		moText	GetProtocol();

		moFileStatus GetStatus();

		MOubyte* GetData();
		void Update();

	private:

		moFileType		m_FileType;
		moFileStatus	m_FileStatus;

		moText		m_CompletePath;
		moText		m_Drive;

		moText		m_Path;
		moText		m_FileName;

		moText		m_Extension;
		moText		m_Protocol;

		MOboolean	m_bExists;
		MOboolean	m_bRemote;
		//buckets...for downloading...
		moBucketsPool	*m_pBucketsPool;
		//
		MOubyte			*m_pData;

		moTextArray     m_Dirs;

};

/*
template class LIBMOLDEO_API moDynamicArray<moFile*>;
typedef moDynamicArray<moFile*> moFileArray;
*/
moDeclareExportedDynamicArray( moFile*, moFileArray )

/**
* clase para el manejo de un directorio
*/

class LIBMOLDEO_API moDirectory : public moAbstract {

	public:
		moDirectory();
		moDirectory( moText p_CompletePath, moFileManager* p_pFileManager = NULL );//could be: http://.... or ftp://... or c:\... or ...
		virtual ~moDirectory();

		virtual MOboolean Init();
		virtual MOboolean Init( moFileManager *p_FileManager );
		virtual MOboolean Finish();

		MOboolean Open( moText p_CompletePath, moText p_Search = "/*.*"  );

		MOboolean Exists();
		MOboolean IsRemote();
		void	SetType( moFileType p_filetype );
		void	SetCompletePath( moText p_completepath );

		moFileType GetType();
		moFileStatus GetStatus();
		moText	GetCompletePath();
		moText	GetProtocol();

		moFile* FindFirst(  );
		moFile* FindNext(  );
		moFile* FindLast(  );
		moFile* Find( MOint index );
		moFile* Find( moText filename);

		void Update();

		moFileArray& GetFiles();

	protected:

		moFileType		m_DirType;
		moFileStatus	m_DirStatus;
		MOint			m_FileIndex;

		moText		m_CompletePath;
		moText		m_Protocol;

		MOboolean	m_bExists;
		MOboolean	m_bRemote;

		moFileManager*	m_pFileManager;

		moFileArray	m_Files;
};

/*
template class LIBMOLDEO_API moDynamicArray<moDirectory*>;
typedef moDynamicArray<moDirectory*> moDirectoryArray;
*/
moDeclareExportedDynamicArray( moDirectory*, moDirectoryArray )

/**
* Recurso para el acceso a archivos
*/

class LIBMOLDEO_API moFileManager : public moResource
{
	public:

		moFileManager();
		virtual ~moFileManager();

		virtual MOboolean Init();
		virtual MOboolean Finish();


		MOboolean		Load( moText p_FileName, MOboolean bWaitForDownload = false );
		moFile*			GetFile( moText p_FileName );

		MOboolean		Open ( moText p_Path,  MOboolean bWaitForDownload = false );
		moDirectory*	GetDirectory( moText p_Path );


		MOboolean       CopyFile( moFile FileSrc, moFile FileDst );
		MOboolean       MoveFile( moFile FileSrc, moFile FileDst );

		static moText          GetExePath();
		static moText          GetWorkPath();
		static bool            FileExists( moText filename );
        static bool            DirectoryExists( moText dirname );

	protected:

		moFileArray	m_Files;
		moDirectoryArray m_Directories;

};

#endif
