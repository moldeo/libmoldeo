/*******************************************************************************

                                  moFile.cpp

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
  Andr�s Colubri

*******************************************************************************/

#include "moFile.h"
#include "moResourceManager.h"

#include <boost/filesystem.hpp>
namespace bfs=boost::filesystem;

#include "moArray.cpp"
moDefineDynamicArray(moFileArray)
moDefineDynamicArray(moDirectoryArray)

//===========================================
//
//				moDirectory
//
//===========================================

moDirectory::moDirectory() {
	m_pFileManager = NULL;
	m_CompletePath = moText("");
	m_bExists = false;
	m_bRemote = false;
	m_FileIndex = 0;
}

moDirectory::moDirectory( moText p_CompletePath, moFileManager* p_pFileManager ) {
	m_pFileManager = p_pFileManager;
	m_CompletePath = p_CompletePath;
	m_bExists = false;
	m_bRemote = false;
	m_FileIndex = 0;
	Open(m_CompletePath);
}

moDirectory::~moDirectory() {
	Finish();
}

/*


    bool find_file( const path & dir_path,         // in this directory,
                    const std::string & file_name, // search for this name,
                    path & path_found )            // placing path here if found
    {
      if ( !exists( dir_path ) ) return false;
      directory_iterator end_itr; // default construction yields past-the-end
      for ( directory_iterator itr( dir_path );
            itr != end_itr;
            ++itr )
      {
        if ( is_directory(itr->status()) )
        {
          if ( find_file( itr->path(), file_name, path_found ) ) return true;
        }
        else if ( itr->path().leaf() == file_name ) // see below
        {
          path_found = itr->path();
          return true;
        }
      }
      return false;
    }



*/

MOboolean
moDirectory::Open( moText p_CompletePath, moText p_Search  ) {

	char *path;

    m_CompletePath = p_CompletePath;
	path = m_CompletePath;

	moText CompletePathSearch = (moText)p_CompletePath + (moText)p_Search;

	for( int i=0; i<(int)m_Files.Count(); i++) {
            moFile* pFile = m_Files[i];
            if  (pFile)
                delete pFile;
            m_Files[i] = NULL;

    }
    m_Files.Empty();

    m_bExists = false;
//    void show_files( const path & directory, bool recurse_into_subdirs = true )
//    {
      if( bfs::exists( path ) )
      {
          m_bExists = true;

         bfs::directory_iterator end ;
        for(  bfs::directory_iterator iter(path) ; iter != end ; ++iter )
          if (  bfs::is_directory( *iter ) )
          {
            //cout << iter->native_directory_string() << " (directory)\n" ;
            //if( recurse_into_subdirs ) show_files(*iter) ;
          } else {
            //cout << iter->native_file_string() << " (file)\n" ;
            moText pFileName( iter->path().file_string().c_str() );

            moText pCompletePathFilename;

            //pCompletePathFilename = m_CompletePath + moText("/") + moText(pFileName);
            pCompletePathFilename = pFileName;

            moFile*	pFile = NULL;
            if (m_pFileManager)
                pFile = m_pFileManager->GetFile( pCompletePathFilename );
            else
                pFile = new moFile( pCompletePathFilename );
            if (pFile) {
                m_Files.Add(pFile);
            }

            #ifdef _DEBUG
            MODebug2->Message( moText("moFileManager:: file:") + (moText)pCompletePathFilename);
            #endif
            //printf("%-32s %s %9.ld %s",fileInfo.name, attribs, fileInfo.size , timeBuff);

          }

      }

      return m_bExists;
//    }



/*
	printf(" checking path : %s\n",path);
	for( int i=0; i<m_Files.Count(); i++) {
            moFile* pFile = m_Files[i];
            if  (pFile)
                delete pFile;
            m_Files[i] = NULL;

    }
    m_Files.Empty();

	if((fPtr = _findfirst(path,  &fileInfo )) == -1L) {
		m_bExists = false;
		return false;
	} else {
		m_bExists = true;

		m_CompletePath = p_CompletePath;
		do {

			moText pFileName( fileInfo.name );

			moText pCompletePathFilename;

			pCompletePathFilename = m_CompletePath + moText("/") + moText(pFileName);

			moFile*	pFile = NULL;
            if (m_pFileManager)
                pFile = m_pFileManager->GetFile( pCompletePathFilename );
            else
                pFile = new moFile( pCompletePathFilename );
			if (pFile) {
				m_Files.Add(pFile);
			}
			//printf("%-32s %s %9.ld %s",fileInfo.name, attribs, fileInfo.size , timeBuff);

		} while(_findnext(fPtr, &fileInfo) == 0);
		_findclose(fPtr);
	}

#else
    //en linux hay que usar: opendir, readdir, closedir etc...
    //http://developer.novell.com/wiki/index.php/Programming_to_the_Linux_Filesystem

    //typedef bool         (*file_match_function)    ( const string&, const string& );

    void findMatch( const string& pattern, const string& path, file_match_function match )
{
    DIR* d = opendir( path.c_str() );
    static struct dirent* dirp;
    vector<string> dirs;
    if (! d)
    {
        cerr << "Unable to open directory " << path << endl;
        cerr << "Error is " << strerror(errno) << "(" << errno << ")" << endl;
        return;
    }
    while ( (dirp = readdir(d)) != NULL )
    {
        if ( 0 == strcmp( ".", dirp->d_name ) ||
             0 == strcmp( "..",dirp->d_name ) ) continue; // skip . and ..
        string candidate( path + "/" + dirp->d_name );
        try {
            if ( LinuxFileInfo::isRegularFile(candidate) )
            {
                if ( LinuxFileInfo::canRead(candidate) && match(pattern, candidate) )
                {
                    cout << candidate << endl;
                }
            }
            else if ( LinuxFileInfo::isDirectory(candidate) )
            {
                dirs.push_back( candidate );
            }
        }
        catch ( const invalid_argument& ia )
        {
            cerr << "Caught exception on path " << candidate << ":  " << ia.what() << endl;
        }
    }
    closedir(d);
    for ( vector<string>::const_iterator ci=dirs.begin(); ci!=dirs.end(); ++ci )
    {
        findMatch( pattern, *ci, match );
    }

#endif
*/
}


MOboolean
moDirectory::Init() {
	return true;
}

MOboolean
moDirectory::Init( moFileManager *p_FileManager ) {
	m_pFileManager = p_FileManager;
	m_FileIndex = 0;
	return true;
}

MOboolean
moDirectory::Finish() {

	for( int i=0; i<(int)m_Files.Count(); i++) {
            moFile* pFile = m_Files[i];
            if  (pFile)
                delete pFile;
            m_Files[i] = NULL;

    }
    m_Files.Empty();
	return true;
}

MOboolean
moDirectory::Exists() {
    m_bExists = bfs::exists((char*)m_CompletePath);
	return m_bExists;
}

MOboolean
moDirectory::IsRemote() {
	return m_bRemote;
}

moText
moDirectory::GetCompletePath() {
	return m_CompletePath;

}

moFileType
moDirectory::GetType() {
	return m_DirType;
}

moText
moDirectory::GetProtocol() {
	return m_Protocol;
}


moFileStatus
moDirectory::GetStatus() {
	return m_DirStatus;
}

moFile*
moDirectory::FindFirst(  ) {
	if (m_bExists) {
		if (m_Files.Count()>0) {
			m_FileIndex = 0;
			return m_Files[0];
		}
	}
	return NULL;
}

moFile*
moDirectory::FindNext(  ) {
	if (m_bExists) {
		if (m_Files.Count()>0) {
			if ( ((MOuint)m_FileIndex+1) < m_Files.Count() ) {
				m_FileIndex++;
				return m_Files[m_FileIndex];
			} else {
				m_FileIndex--;
				return NULL;
			}
		}
	}
	return NULL;
}

moFile*
moDirectory::FindLast(  ) {
	if (m_bExists) {
		if (m_Files.Count()>0) {
			m_FileIndex = m_Files.Count() - 1;
			return m_Files[m_FileIndex];
		}
	}
	return NULL;
}

moFile*
moDirectory::Find( MOint index ) {
	if (m_bExists) {
		if (0<=index && index<(MOint)m_Files.Count() ) {
			m_FileIndex = index;
			return m_Files[m_FileIndex];
		}
	}
	return NULL;
}

moFile*
moDirectory::Find( moText filename) {
	if (m_bExists) {
		for(m_FileIndex=0; m_FileIndex<(MOint)m_Files.Count(); m_FileIndex++) {
			if (m_Files[m_FileIndex]->GetFileName()==filename)
				return m_Files[m_FileIndex];
		}
	}
	return NULL;
}

void
moDirectory::Update() {
	//----
}



moFileArray&
moDirectory::GetFiles() {
	return m_Files;
}

//===========================================
//
//				moFile
//
//===========================================

moFile::moFile() {

	m_Path = moText("");
	m_FileName = moText("");
	m_CompletePath = moText("");
	m_Extension = moText("");
	m_Protocol =moText( "");

	m_bRemote = false;
	m_pBucketsPool = NULL;
	m_pData = NULL;
}

moFile::moFile( moText p_CompletePath ) {//could be: http://.... or ftp://... or c:\... or ...

    SetCompletePath( p_CompletePath );
}

moFile::~moFile() {
	Finish();
}

MOboolean
moFile::Init() {
	//check file protocol and existance
	//...todo

	//create pool for downloading or streaming
	if (m_bRemote && !m_pBucketsPool) {
		m_pBucketsPool = new moBucketsPool();
		//then start downloading...
	}
	return (m_pBucketsPool!=NULL);
}

MOboolean
moFile::Finish() {
	if (m_bRemote && m_pBucketsPool) {
		delete m_pBucketsPool;
		m_pBucketsPool = NULL;
	}
	return (m_pBucketsPool==NULL);
}

MOboolean
moFile::Exists() {
    m_bExists = bfs::exists((char*)m_CompletePath);
	return m_bExists;
}

moFileType
moFile::GetType() {
	return m_FileType;
}

moText
moFile::GetProtocol() {
	return m_Protocol;
}


moFileStatus
moFile::GetStatus() {
	return m_FileStatus;
}

MOboolean
moFile::IsRemote() {
	return m_bRemote;
}

MOubyte*
moFile::GetData() {
	if (m_pData && m_FileStatus==MO_FILESTATUS_READY) {
		return m_pData;
	}
	return NULL;
}

void
moFile::SetType( moFileType p_filetype ) {
	m_FileType = p_filetype;
}


moText
moFile::GetFileName() {
	return m_FileName;
}

void
moFile::SetCompletePath( moText p_completepath ) {

	//check if http. ftp or other...set remote
	m_CompletePath = p_completepath;

	moText Left7 = m_CompletePath;
	Left7.Left(7);
	moText Left6 = m_CompletePath;
	Left6.Left(6);

	if (Left7==moText("http://") ||
		Left7==moText("HTTP://")) {
		m_Protocol = moText("http://");
		m_FileType = MO_FILETYPE_HTTP;
		m_bRemote = true;
		m_bExists = false;
	} else if (Left6==moText("ftp://") ||
		Left6==moText("FTP://")) {
		m_Protocol = moText("ftp://");
		m_FileType = MO_FILETYPE_FTP;
		m_bRemote = true;
		m_bExists = false;
	} else {
	    moTextArray FileNameA;
		m_Protocol = moText("file:///");
		//moText m_Drive = m_CompletePath.Scan(":");

		std::string str;
		str = bfs::extension( (char*)m_CompletePath );

		m_Extension = str.c_str();

        m_Dirs = m_CompletePath.Explode(moText("\\/"));

        if ( m_Dirs.Count() > 0 ) {
            m_Drive = m_Dirs[0];
            m_FileName = m_Dirs[m_Dirs.Count()-1];
            FileNameA = m_FileName.Explode(moText("."));
            m_FileName = FileNameA[0];
        }

		m_FileType = MO_FILETYPE_LOCAL;
		m_bRemote = false;

		m_bExists = bfs::exists((char*)m_CompletePath);

	}

}

moText
moFile::GetPath() {
	return m_Path;
}

moText
moFile::GetCompletePath() {
	return m_CompletePath;
}

moText
moFile::GetExtension() {
	return m_Extension;
}

void
moFile::Update() {
	//----
}
