// Precompiled Headers
#include "Platform/Stdafx.h"

#include "Platform/FileInfo.h"
#include "Platform/FileSystem.h"

namespace BH
{

	String FileInfo::GetFileNoExt() const
	{
		size_t pos = mFile.find_last_of('.');
		return mFile.substr(0, pos);
	}

	String FileInfo::GetExtension() const
	{
		size_t pos = mFile.find_last_of('.');
		return (pos == std::string::npos && (pos > 0)) ? "" : mFile.substr(pos + 1);
	}

	String FileInfo::GetFullFile() const
	{
		return mPath + mFile;
	}

	String FileInfo::GetUpOneLevel() const
	{
		size_t pos = mPath.find_last_of('/');

		// Find second last /
		if (pos != std::string::npos && (pos > 0))
			pos = mPath.find_last_of('/', pos - 1);

		return (pos != std::string::npos) ? mPath.substr(0, pos + 1) : "";
	}

	FileInfo::List FileInfo::GetFilesInPath(bool recursive) const
	{
		return FileSystem::GetFileList(mPath.c_str(), recursive);
	}

	FileInfo::List FileInfo::GetFilesInPathExclude(const Char * excludes, bool recursive) const
	{
		return FileSystem::GetFileListExclude(mPath.c_str(), excludes, recursive);
	}

	FileInfo::List FileInfo::GetFilesInPathInclude(const Char * includes, bool recursive) const
	{
		return FileSystem::GetFileListInclude(mPath.c_str(), includes, recursive);
	}

	FileInfo::List FileInfo::GetFoldersInPath(bool recursive) const
	{
		return FileSystem::GetFolderList(mPath.c_str(), recursive);
	}

	FileInfo::FileInfo()
	{
	}

	FileInfo::FileInfo(const Char * file)
	{
		Set(file);
	}

	FileInfo::FileInfo(const String & file)
	{
		Set(file.c_str());
	}

	FileInfo::FileInfo(const Char * path, const Char * file)
	{
		Set(path, file);
	}

	FileInfo & FileInfo::operator = (const Char * rhs)
	{
		Set(rhs);
		return *this;
	}

	FileInfo & FileInfo::operator = (const String & rhs)
	{
		Set(rhs.c_str());
		return *this;
	}

	const bool FileInfo::operator == (const Char * rhs) const
	{
		return GetFullFile() == rhs;
	}

	const bool FileInfo::operator == (const String & rhs) const
	{
		return GetFullFile() == rhs;
	}

	const bool FileInfo::Exists() const
	{
		return FileSystem::FileCheckExists(GetFullFile().c_str());
	}

	const bool FileInfo::operator == (const FileInfo & rhs) const
	{
		return GetFullFile() == rhs.GetFullFile();
	}

	const bool FileInfo::operator < (const FileInfo & rhs) const
	{
		return GetFullFile() < rhs.GetFullFile();
	}

	void FileInfo::Set(const Char * file)
	{
		mPath = "";
		mFile = "";

		if (!file || strlen(file) == 0)
			return;

		String stripper = file;
		std::replace(stripper.begin(), stripper.end(), '\\', '/'); // Replace all backslashes with slashes.

		s32 slashCount = 1; // remove the slash if it happens to be the first character.
		String fullname = "";
		for (size_t i = 0; i < stripper.size(); ++i)
		{
			if (stripper[i] == '/')
			{
				// If only 1 slash, copy it
				if (slashCount == 0)
					fullname.push_back(stripper[i]);

				++slashCount;
				continue;
			}

			slashCount = 0;
			fullname.push_back(stripper[i]);
		}

		size_t pos = fullname.find_last_of('/');
		if (pos != std::string::npos)
		{
			mPath = fullname.substr(0, pos + 1);
			mFile = fullname.substr(pos + 1);
		}
		else
		{
			mFile = fullname;
		}

		// if "./" is in front, remove it
		if (mPath.length() > 1 && mPath[0] == '.' && mPath[1] == '/')
			mPath = mPath.substr(2);

		// Make sure our file extension is lower case.
		pos = mFile.find_last_of('.');
		if (pos != std::string::npos)
		{
			for (size_t i = pos + 1; i < mFile.size(); ++i)
			{
				mFile[i] = Char(::tolower(mFile[i]));
			}
		}
	}

	void FileInfo::Set(const Char * path, const Char * file)
	{
		if (path && (strlen(path) > 0))
		{
			std::string full(path);
			char c = full[full.length() - 1];

			// If the last character is not slash, add it
			if (!(c == '/' || c == '\\'))
				full += '/';

			full += file;
			Set(full.c_str());
		}
		else
		{
			Set(file);
		}
	}

	FileInfo::operator const String() const
	{
		return mPath + mFile;
	}

	Ostream & operator << (Ostream & ostream, const FileInfo & fileInfo)
	{
		ostream << fileInfo.GetFullFile();
		return ostream;
	}

	FileInfo operator + (const String& lhs, const FileInfo & rhs)
	{
		return FileInfo((lhs + "/") + rhs.GetFullFile());
	}

	FileInfo operator + (const Char* lhs, const FileInfo & rhs)
	{
		return FileInfo((String(lhs) + "/") + rhs.GetFullFile());
	}

}