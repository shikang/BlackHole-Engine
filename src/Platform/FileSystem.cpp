// Precompiled Headers
#include "Platform/Stdafx.h"

#include "Platform/FileSystem.h"
#include "Platform/Platform.h"

// External Dependencies
#include <sys/stat.h> 
#include <locale>
#include <windows.h>

using namespace BH;


	bool FileSystem::FolderCheckExists( const Char* folder )
	{
		String name = folder;
		while ( name.length( ) && ( name[ name.size( ) - 1 ] == '\\' || name[ name.size( ) - 1 ] == '/' ) )
			name.pop_back( );

		if ( name.length( ) == 0 )
			return false;

		struct stat stFileInfo;
		// stat function returns information about the attributes of the file named by filename in the structure pointed to by buf
		int result = stat( name.c_str( ), &stFileInfo );
		return ( result == 0 ) && ( ( stFileInfo.st_mode & S_IFMT ) == S_IFDIR );
	}

	bool FileSystem::FileCheckExists( const Char * file )
	{
		String name = file;
		while ( name.length( ) && ( name[ name.size( ) - 1 ] == '\\' || name[ name.size( ) - 1 ] == '/' ) )
			name.pop_back( );

		if ( name.length( ) == 0 )
			return false;

		struct stat stFileInfo;
		// stat function returns information about the attributes of the file named by filename in the structure pointed to by buf
		int result = stat( name.c_str( ), &stFileInfo );
		return ( result == 0 ) && ( ( stFileInfo.st_mode & S_IFMT ) != S_IFDIR );
	}

	bool FileSystem::FileDelete( const Char* directory, const Char * file )
	{
		String f = directory;
		f += "./";
		f += file;

		// Remove file
		if ( ::remove( f.c_str( ) ) != 0 )
		{
			//LOGERROR( "Could not delete file: " << f );
			return false;
		}

		return true;
	}

	std::unordered_set<String> GetExtensionList( const Char* extList )
	{
		// Make sure that we skip all spaces.
		while ( *extList == ' ' && *extList != '\0' )
			++extList;

		std::unordered_set<String> exts;
		const Char* curr = extList;

		std::locale loc;
		Char currExt[ 64 ] = { '\0' };
		Char* writer = currExt;

		while ( *curr != '\0' )
		{
			// If current character is either alphabet or numbers
			if ( std::isalnum( *curr, loc ) )
				*writer++ = Char( ::tolower( *curr ) );	// Write lowercase letters to writter

			// If current character is ,
			if ( *curr == ',' )
			{
				// Add null terminated character
				*writer = '\0';
				// Insert hashed extension
				exts.insert(currExt);
				// Writer points to first character again
				writer = currExt;
			}
			++curr;
		}

		// If writer does not point to first character, meaning there is extension
		if ( writer != currExt )
			exts.insert(currExt);

		return exts;
	}

	FileInfo::List FileSystem::GetFileList( const Char * directory, bool recursive )
	{
		FileInfo::List files;
		GetFileFolderListing( files, directory, "", recursive, false, nullptr, false );
		return files;
	}

	FileInfo::List FileSystem::GetFileListExclude( const Char * directory, const Char* extExclude, bool recursive )
	{
		//BH_ASSERT_TRUE( extExclude != nullptr );

		FileInfo::List files;
		std::unordered_set<String> extList = GetExtensionList( extExclude );
		GetFileFolderListing( files, directory, "", recursive, false, ( extList.size( ) == 0 ? nullptr : &extList ), true );
		return files;
	}

	FileInfo::List FileSystem::GetFileListInclude( const Char * directory, const Char* extInclude, bool recursive )
	{
		//BH_ASSERT_TRUE( extInclude != nullptr );

		FileInfo::List files;
		std::unordered_set<String> extList = GetExtensionList( extInclude );
		GetFileFolderListing( files, directory, "", recursive, false, ( extList.size( ) == 0 ? nullptr : &extList ), false );
		return files;
	}

	FileInfo::List FileSystem::GetFolderList( const Char * directory, bool recursive )
	{
		FileInfo::List files;
		GetFileFolderListing( files, directory, "", recursive, true, nullptr, false );
		return files;
	}

	void FileSystem::GetFileFolderListing( FileInfo::List& files,
										   const Char * root_dir,
										   const String & child_dir,
										   bool recursive,
										   bool folders,
										   const std::unordered_set<String>* extChecks,
										   bool exclude )
	{
		String dir = root_dir;
		dir += ".\\"; dir += child_dir;
		dir += "\\*";

		WIN32_FIND_DATA data;
		HANDLE h = FindFirstFile( dir.c_str( ), &data );

		if ( h == INVALID_HANDLE_VALUE )
			return;

		do
		{
			if ( data.cFileName[ 0 ] == '.' )
				continue;

			bool isFolder = ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;
			String output = child_dir.length( ) > 0 ? child_dir + "\\" + data.cFileName : data.cFileName;

			// If is folder and recursive, go into older and grabs all files info
			if ( isFolder && recursive )
				GetFileFolderListing( files, root_dir, output, recursive, folders, extChecks, exclude );

			/*
				If folders is true and current file is folder, add it to list or
				If folders is false and current file is not folder, add it to list
				*/
			if ( folders == isFolder )
			{
				// If folder, file info stored path only
				FileInfo out = isFolder ? FileInfo( output.c_str( ), "" ) : FileInfo( output );

				/*
					If current extension is inside and extension list and exclude is false, do not exclude it or
					If current extension is not inside and exclude is true, do not eclude it
					*/
				if ( !extChecks || ( ( extChecks->find( out.GetExtension() ) == extChecks->end( ) ) == exclude ) )
					files.push_back( out );
			}

		} while ( ::FindNextFile( h, &data ) == TRUE );		// Find next file

		::FindClose( h );
	}

	bool FileSystem::FolderCreate( const Char * directory )
	{
		BOOL success = ::CreateDirectory( directory, NULL );
		if ( success == 0 )
		{
			if ( GetLastError( ) == ERROR_ALREADY_EXISTS )
			{
				//LOGWARNING( directory << " already exists." );
			}
			else
			{
				//LOGERROR( "Unable to create directory " << directory );
			}
			return false;
		}

		// Set directory attributes
		::SetFileAttributes( directory, FILE_ATTRIBUTE_NORMAL & FILE_ATTRIBUTE_DIRECTORY );

		return true;
	}

	bool FileSystem::FolderDelete( const Char * directory, bool full )
	{
		bool            bSubdirectory = false;      // Flag, indicating whether
		// subdirectories have been found
		HANDLE          hFile;                      // Handle to directory
		String			strFilePath;				// Filepath
		String			strPattern;					// Pattern
		WIN32_FIND_DATA fileInfo;					// File information

		strPattern = directory;
		strPattern += "\\*.*";

		hFile = ::FindFirstFile( strPattern.c_str( ), &fileInfo );
		if ( hFile != INVALID_HANDLE_VALUE )
		{
			// Delete all files and folder inside
			do
			{
				if ( fileInfo.cFileName[ 0 ] == '.' )
					continue;

				strFilePath = directory;
				strFilePath += "\\";
				strFilePath += fileInfo.cFileName;

				if ( fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					if ( full )
					{
						// Delete subdirectory
						if ( !FolderDelete( strFilePath.c_str( ), full ) )
							return false;
					}
					else
					{
						bSubdirectory = true;
					}
				}
				else if ( full )
				{
					// Set file attributes
					if ( ::SetFileAttributes( strFilePath.c_str( ),
						FILE_ATTRIBUTE_NORMAL ) == FALSE )
						return false;

					// Delete file
					if ( ::DeleteFile( strFilePath.c_str( ) ) == FALSE )
						return false;
				}
			} while ( ::FindNextFile( hFile, &fileInfo ) == TRUE );

			// Close handle
			::FindClose( hFile );

			DWORD dwError = ::GetLastError( );
			if ( dwError != ERROR_NO_MORE_FILES )
			{
				return false;
			}
			else if ( !bSubdirectory )
			{
				// Set directory attributes
				if ( ::SetFileAttributes( directory,
					FILE_ATTRIBUTE_NORMAL ) == FALSE )
					return false;

				// Delete directory
				return ::RemoveDirectory( directory ) == TRUE;
			}
		}

		return false;
	}

#include <Shlobj.h>
	FileInfo FileSystem::GetLocalAppDirectory( )
	{
		TCHAR szPath[ MAX_PATH ];
		SHGetFolderPath( NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath );

		return FileInfo( szPath, "" );
	}

	bool FileSystem::FileCreate( const Char* directory, const Char* fileName, bool overwrite )
	{
		String file = directory;
		file += "./";
		file += fileName;

		HANDLE result = CreateFile( file.c_str( ),
									GENERIC_READ | GENERIC_WRITE,
									0, NULL,
									( overwrite ? CREATE_ALWAYS : CREATE_NEW ),
									FILE_ATTRIBUTE_NORMAL,
									NULL );

		if ( result != INVALID_HANDLE_VALUE )
		{
			CloseHandle( result );
			return true;
		}

		return false;
	}

	bool FileSystem::FileCopy( const Char* source, const Char* dest, bool replace )
	{
		BOOL success = ::CopyFile( source, dest, replace ? FALSE : TRUE );
		return success != 0;

	}

	FileInfo FileSystem::GetWorkingDirectory( )
	{
		TCHAR curr_dir[ MAX_PATH ] = "";

		if ( !::GetCurrentDirectory( sizeof( curr_dir ) - 1, curr_dir ) )
		{
			//LOGERROR( "Unable to determine current working directory." );
			return String( );
		}

		return FileInfo( curr_dir );
	}
	
