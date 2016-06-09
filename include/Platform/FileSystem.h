#ifndef BH_FILESYSTEM_H
#define BH_FILESYSTEM_H

#include "Platform/Typedefs.h"
#include "Platform/FileInfo.h"

#include <list>
#include <unordered_set>

namespace BH
{
	// File system utilties.
	class FileSystem
	{
	public:
		// Get local applications directory.
		static FileInfo GetLocalAppDirectory();

		// Get the current woring directory.
		static FileInfo GetWorkingDirectory();

		// Check if a folder exists.
		static bool FolderCheckExists(const Char * folder);

		// Check if a file exists (.
		static bool FileCheckExists(const Char * file);

		// Get file listing from directory.
		static FileInfo::List GetFileList(const Char * directory, bool recursive = false);

		// Get file listing from directory.
		static FileInfo::List GetFileListExclude(const Char * directory, const Char* extExclude, bool recursive = false);

		// Get file listing from directory.
		static FileInfo::List GetFileListInclude(const Char * directory, const Char* extExclude, bool recursive = false);

		// Get directory listing from directory.
		static FileInfo::List GetFolderList(const Char * directory, bool recursive = false);

		// Create a directory.
		static bool FolderCreate(const Char * directory);

		// Remove a directory.
		static bool FolderDelete(const Char * directory, bool recursive = true);

		// Create a new blank file.
		static bool FileCreate(const Char* directory, const Char* fileName, bool overwrite = true);

		// Copy a file from a source to directory.
		static bool FileCopy(const Char* source, const Char* dest, bool full = true);

		// Delete a specified file.
		static bool FileDelete(const Char* directory, const Char * file);

	private:
		static void GetFileFolderListing(FileInfo::List& files,								// Container to store the gotten file info
										 const Char* root_dir,								// Root directory
										 const String& child_dir,							// Child directory
										 bool recursive,									// Search recursively inside folders
										 bool folders,										// If true, container will store all folders else it will store all files (not folders)
										 const std::unordered_set<String>* extChecks,		// Extension to include or exclude, null will include everything
										 bool exclude);										// If true, container will exclude extension specified else container will only include extension specify
	};
}

#endif // BH_FILESYSTEM_H