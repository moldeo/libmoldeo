/*******************************************************************************

                              moFileManager.cpp

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

#include <boost/version.hpp>
#include <boost/filesystem.hpp>

#include "moFileManager.h"

#include "moArray.h"
moDefineDynamicArray(moFileArray)
moDefineDynamicArray(moDirectoryArray)


namespace bfs=boost::filesystem;

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

#include <set>

MOboolean
moDirectory::Open( moText p_CompletePath, moText p_Search  ) {

	char *path;
    string stdFileName;
    string stdCompleteFileName;
    set<string> stdListOfFileNames;
    set<string> stdListOfCompleteFileNames;

  m_CompletePath = p_CompletePath;
	path = m_CompletePath;

  //m_CompletePath = "\\\\\\";

	m_DirNameArray = m_CompletePath.Explode(moText("\\/"));

	if (m_DirNameArray.Count()>0) {

    m_DirName = m_DirNameArray[ m_DirNameArray.Count() - 1 ];

	} else {
    m_DirName = m_CompletePath;
	}


	moText CompletePathSearch = (moText)p_CompletePath + (moText)p_Search;

    /** Empty file array*/
	for( int i=0; i<(int)m_Files.Count(); i++) {
            moFile* pFile = m_Files[i];
            if  (pFile)
                delete pFile;
            m_Files[i] = NULL;

    }
    m_Files.Empty();

    /** Empty subdirs array*/
	for( int i=0; i<(int)m_SubDirs.Count(); i++) {
            moDirectory* pDir = m_SubDirs[i];
            if  (pDir)
                delete pDir;
            m_SubDirs[i] = NULL;

    }
    m_SubDirs.Empty();

    /** Set by default m_bExists on false*/
    m_bExists = false;
//    void show_files( const path & directory, bool recurse_into_subdirs = true )
//    {
    /** Check files*/
      if( bfs::exists( path ) )
      {
          m_bExists = true;

         bfs::directory_iterator end ;
        for(  bfs::directory_iterator iter(path) ; iter != end ; ++iter )
          if (  bfs::is_directory( *iter ) )
          {
            //cout << iter->native_directory_string() << " (directory)\n" ;
            //if( recurse_into_subdirs ) show_files(*iter) ;
            #if BOOST_VERSION > 103500

                #if BOOST_VERSION < 104800
                    moText pSubDirName( iter->path().filename().c_str() );
                #else
                    moText pSubDirName( iter->path().filename().string().c_str() );
                #endif
            #else
            moText pSubDirName( iter->path().leaf().c_str() );
            #endif

            moText pCompletePathSubdirName( iter->path().string().c_str() );

            if (pSubDirName.Left(1) != "." ) {
              moDirectory* pSubdir = new moDirectory( pCompletePathSubdirName );
              if (pSubdir)
                m_SubDirs.Add( pSubdir );
            }

          } else {
            //cout << iter->native_file_string() << " (file)\n" ;
            //ATENCION SEGUN LA VERSION DE BOOST hya que usar filename() o leaf()
            //moText pFileName( iter->path().leaf().c_str() );


            #if BOOST_VERSION > 103500


                #if BOOST_VERSION < 104800
                    moText pFileName( iter->path().filename().c_str() );
                    stdFileName = iter->path().filename().c_str();
                #else
                    moText pFileName( iter->path().filename().string().c_str() );
                    stdFileName = iter->path().filename().string().c_str();
                #endif

            #else

            moText pFileName( iter->path().leaf().c_str() );

            stdFileName = iter->path().leaf().c_str();

            #endif

            moText pCompletePathFilename( iter->path().string().c_str() );
            stdCompleteFileName = iter->path().string().c_str();



            if (stdFileName!="Thumbs.db") {
                stdListOfFileNames.insert(stdFileName);
                stdListOfCompleteFileNames.insert(stdCompleteFileName);
            }

            #ifdef _DEBUG
            //MODebug2->Message( moText("moFileManager:: file:") + (moText)pCompletePathFilename);
            //MODebug2->Message( moText("moFileManager:: filesize:") + IntToStr((int) bfs::file_size( iter->path().file_string().c_str() )));
            #endif
            //printf("%-32s %s %9.ld %s",fileInfo.name, attribs, fileInfo.size , timeBuff);

          }

      }

        /** Sorted for linux */
        moText pCompletePathFilename;

        for(std::set<string>::iterator Name = stdListOfCompleteFileNames.begin() ; Name != stdListOfCompleteFileNames.end() ; ++Name)
        {
            //cout << *Name << endl;
            string comp = *Name;
            pCompletePathFilename = moText((char*)comp.c_str());

            moFile*	pFile = NULL;
            if (m_pFileManager)
                pFile = m_pFileManager->GetFile( pCompletePathFilename );
            else
                pFile = new moFile( pCompletePathFilename );

            if (pFile) m_Files.Add(pFile);
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

    /** Empty subdirs array*/
	for( int i=0; i<(int)m_SubDirs.Count(); i++) {
            moDirectory* pDir = m_SubDirs[i];
            if  (pDir)
                delete pDir;
            m_SubDirs[i] = NULL;

    }
    m_SubDirs.Empty();

	return true;
}

MOboolean
moDirectory::Exists() {
    m_bExists = bfs::exists((char*)m_CompletePath);
	return m_bExists;
}

MOboolean
moDirectory::HasSubdirs() {
  return m_SubDirs.Count();
}

MOboolean
moDirectory::IsRemote() {
	return m_bRemote;
}

moText
moDirectory::GetCompletePath() {
	return m_CompletePath;

}

moText
moDirectory::GetDirName() {
  return m_DirName;
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

    char *path;

	path = m_CompletePath;

//    void show_files( const path & directory, bool recurse_into_subdirs = true )
//    {
    /** Check files*/
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

            //ATENCION SEGUN LA VERSION DE BOOST hya que usar filename() o leaf()

                #if BOOST_VERSION < 104300
                    moText pFileName( iter->path().leaf().c_str() );
                    moText pCompletePathFilename( iter->path().string().c_str() );
                #else
                    moText pFileName( iter->path().filename().string().c_str() );
                    moText pCompletePathFilename( iter->path().string().c_str() );
                #endif




            moFile*	pFile = NULL;

            if (pFileName!=moText("Thumbs.db")) {

                bool founded = false;

                for( int i=0; i<(int)m_Files.Count(); i++) {
                    moFile* pFile = m_Files[i];
                    if  (pFile->GetCompletePath()==pCompletePathFilename) {
                        founded = true;
                    }
                }
                if (!founded) {
                        if (m_pFileManager) {
                            pFile = m_pFileManager->GetFile( pCompletePathFilename );
                        } else {
                            pFile = new moFile( pCompletePathFilename );
                        }
                        if (pFile) {
                            MODebug2->Message( moText("moFileManager::moDirectory::Update file added:") + (moText)pCompletePathFilename);
                            m_Files.Add(pFile);
                        }
                }

            }


            //#ifdef _DEBUG
            //MODebug2->Message( moText("moFileManager:: moDirectory::Update:") + (moText)pCompletePathFilename);
            //#endif
            //printf("%-32s %s %9.ld %s",fileInfo.name, attribs, fileInfo.size , timeBuff);

          }

      }

      return;

}



moFileArray&
moDirectory::GetFiles() {
	return m_Files;
}

moDirectoryArray&
moDirectory::GetSubDirs() {
  return m_SubDirs;
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

    char *path;
    path = p_CompletePath;

    if (Exists() && !bfs::is_directory(path)) m_FileSize = (long) bfs::file_size( path );
    else m_FileSize = 0;

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

moFileDate	moFile::GetDate() {
        return m_FileDate;
}

moFileSize	moFile::GetSize() {
        return m_FileSize;
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

moText
moFile::GetFullName() {
	return ( m_FileName + m_Extension );
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
        #ifdef MO_WIN32
        m_Path = "";
        #else
        m_Path = moSlash;
        #endif

        if ( m_Dirs.Count() > 0 ) {
            m_Drive = m_Dirs[0];
            m_FileName = m_Dirs[m_Dirs.Count()-1];
            FileNameA = m_FileName.Explode(moText("."));
            m_FileName = FileNameA[0];
            m_Dirs.Remove(m_Dirs.Count()-1);
        }

        for( MOuint d=0; d < m_Dirs.Count(); d++ ) {
          if (m_Dirs[d]!="" && m_Dirs[d]!="/" && m_Dirs[d]!="." && m_Dirs[d]!="..")
            m_Path+= m_Dirs[d] + "/";
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

moText
moFile::GetFolderName() {
  if (m_Dirs.Count()>0) {
    return m_Dirs[m_Dirs.Count()-1];
  }
  return moText("");
}

void
moFile::Update() {
	//----
}

//===========================================
//
//				moFileManager
//
//===========================================

moFileManager::moFileManager() {

	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_FILE );

	SetName("filemanager");
	SetLabelName("filemanager");

}

moFileManager::~moFileManager() {

}

MOboolean moFileManager::Init() {
	return true;
}


MOboolean moFileManager::Finish() {
	return true;
}


MOboolean
moFileManager::Load( moText p_FileName, MOboolean bWaitForDownload ) {

	for(MOuint i = 0; i< m_Files.Count(); i++ ) {
		if ( m_Files[i]->GetCompletePath() == p_FileName ) {
			return true;
		}
	}

	moFile* pFile =  new moFile( p_FileName );

	if ( pFile!=NULL ) {
		if (pFile->GetType()==MO_FILETYPE_LOCAL && !pFile->Exists())
			return false;
		if (bWaitForDownload && pFile->IsRemote()) {
			while(pFile->GetStatus()!=MO_FILESTATUS_READY) {
				pFile->Update();
			}
		}
		m_Files.Add(pFile);
		return true;
	}
	return false;
}

moFile*
moFileManager::GetFile( moText p_FileName ) {

	if ( Load(p_FileName) ) {
		for(MOuint i = 0; i< m_Files.Count(); i++ ) {
			if ( m_Files[i]->GetCompletePath() == p_FileName ) {
				return m_Files[i];
			}
		}
	}
	return NULL;
}


MOboolean
moFileManager::Open( moText p_Path, MOboolean bWaitForDownload ) {

	for(MOuint i = 0; i< m_Directories.Count(); i++ ) {
		if ( m_Directories[i]->GetCompletePath() == p_Path ) {
			return true;
		}
	}

	moDirectory* pDir =  new moDirectory( p_Path, this );

	if ( pDir!=NULL ) {
		if (pDir->GetType()==MO_FILETYPE_LOCAL && !pDir->Exists()) {
		    delete pDir;
			return false;
		}
		if (bWaitForDownload && pDir->IsRemote()) {
			while(pDir->GetStatus()!=MO_FILESTATUS_READY) {
				pDir->Update();
			}
		}
		m_Directories.Add(pDir);
		return true;
	}
	return false;
}

moDirectory*
moFileManager::GetDirectory( moText p_Path ) {
	if ( Open(p_Path) ) {
		for(MOuint i = 0; i< m_Directories.Count(); i++ ) {
			if ( m_Directories[i]->GetCompletePath() == p_Path ) {
				return m_Directories[i];
			}
		}
	}
	return NULL;
}

moText
moFileManager::GetExePath() {

    return moText("");

}

moText
moFileManager::GetWorkPath() {
    char lbuf[1024];

    getcwd( lbuf , sizeof( lbuf ) );

    moText workpath = lbuf;

    return workpath;

}

bool
moFileManager::CreateDirectory( moDirectory Dir ) {

    std::string dirname;

    dirname = Dir.GetCompletePath();

    bfs::create_directory( dirname );

    return Dir.Exists();
}


bool
moFileManager::FileExists( moText filename ) {
        moFile file( filename );
        return file.Exists();
}

bool
moFileManager::DirectoryExists( moText dirname ) {
        moDirectory   directory( dirname );
        return directory.Exists();
}



bool
moFileManager::CopyFile( moText FileSrc, moText FileDst ) {

/*
	ifstream initialFile(FileSrc, ios::in|ios::binary);
	ofstream outputFile(FileDst, ios::out|ios::binary);
	//defines the size of the buffer
	initialFile.seekg(0, ios::end);
	long fileSize = initialFile.tellg();
	//Requests the buffer of the predefined size


	//As long as both the input and output files are open...
	if(initialFile.is_open() && outputFile.is_open())
	{
		short * buffer = new short[fileSize/2];
		//Determine the file's size
		//Then starts from the beginning
		initialFile.seekg(0, ios::beg);
		//Then read enough of the file to fill the buffer
		initialFile.read((char*)buffer, fileSize);
		//And then write out all that was read
		outputFile.write((char*)buffer, fileSize);
		delete[] buffer;
	}
	//If there were any problems with the copying process, let the user know
	else if(!outputFile.is_open())
	{
		MODebug2->Error( moText("I couldn't open ") + (moText)FileDst + moText(" for copying!"));
		return 0;
	}
	else if(!initialFile.is_open())
	{
		MODebug2->Error( moText("I couldn't open ") + (moText)FileSrc + moText(" for copying!"));
		return 0;
	}

	initialFile.close();
	outputFile.close();
*/
    std::string name, new_name;

    name = FileSrc;
    new_name = FileDst;

    try {
        bfs::copy_file(name, new_name);
    }
    catch( const std::exception & ex ) {
        MODebug2->Error( ex.what() );
    }


	return 1;

}

