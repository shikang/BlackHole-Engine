#ifndef BH_FILEINFO_H
#define BH_FILEINFO_H

#include <vector>

namespace BH
{
	// Utility to extract path and extension names out of a file path string.
	class FileInfo
	{
	public:
		typedef std::vector<FileInfo>			List;

	public:
		// GETTER_AUTO_CREF(String, Path);			//!< Get the file's path.
		// GETTER_AUTO_CREF(String, File);			//!< Get the file name (with extension).
		String GetFileNoExt() const;			//!< Get the file name without the extension.
		String GetExtension() const;			//!< Get the file's extension.
		String GetFullFile() const;				//!< Get the full file name (with path).
		String GetUpOneLevel() const;			//!< Get the path directory one level up.

		/*
			Get all files in the current fileinfo path.
			Recursively go into folders to search for more files.
		*/
		List GetFilesInPath(bool recursive = false) const;

		// Get all files in the current fileinfo path.
		List GetFilesInPathExclude(const Char * excludes, bool recursive = false) const;

		// Get all files in the current fileinfo path.
		List GetFilesInPathInclude(const Char * includes, bool recursive = false) const;

		/*
			Get all files in the current fileinfo path.
			Recursively go into folders to search for more folders.
		*/
		List GetFoldersInPath(bool recursive = false) const;

		// Check if file exists.
		const bool Exists() const;

		// Set a new file. ( = )
		void Set(const Char * file);

		// Set a new file with path. ( = )
		void Set(const Char * path, const Char * file);

	public:
		// Constructor.
		FileInfo();

		// Overloaded constructor.
		FileInfo(const Char * file);

		// Overloaded constructor.
		FileInfo(const String & file);

		// Overloaded constructor.
		FileInfo(const Char * path, const Char * file);

		// Assignment operator.
		FileInfo & operator = (const Char * rhs);

		// Assignment operator.
		FileInfo & operator = (const String & rhs);

		// Comparison operator to check if full path names are the same.
		const bool operator == (const Char * rhs) const;

		// Comparison operator to check if full path names are the same.
		const bool operator == (const String & rhs) const;

		// Comparison operator to check if full path names are the same.
		const bool operator == (const FileInfo & rhs) const;

		// Less-than operator for STL containers.
		const bool operator < (const FileInfo & rhs) const;

		// Conversion operator to string.
		operator const String() const;

	private:
		String		mPath;
		String		mFile;
	};
}

// Insertion operator for use with std::ostream.
BH::Ostream & operator << (BH::Ostream & ostream, const BH::FileInfo & fileInfo);

// Append a path in front of the file info.
BH::FileInfo operator + (const BH::String & lhs, const BH::FileInfo & rhs);

// Append a path in front of the file info.
BH::FileInfo operator + (const BH::Char * lhs, const BH::FileInfo & rhs);

#endif // BH_FILEINFO_H
