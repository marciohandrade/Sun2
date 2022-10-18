/*
    ParaGUI - crossplatform widgetset
    Copyright (C) 2000,2001,2002  Alexander Pipelka
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 
    Alexander Pipelka
    pipelka@teleweb.at
 
    Last Update:      $Author: braindead $
    Update Date:      $Date: 2003/04/18 11:15:28 $
    Source File:      $Source: /cvsroot/paragui/paragui/include/pgfilearchive.h,v $
    CVS/RCS Revision: $Revision: 1.3.6.6 $
    Status:           $State: Exp $
*/

/** \file pgfilearchive.h
	Header file of the PG_FileArchive class.
	It also defines the PG_OPEN_MODE enumeration.
*/

#ifndef PG_FILEARCHIVE_H
#define PG_FILEARCHIVE_H


#include "pgfile.h"
#pragma warning(disable: 4786)
#include <vector>
#include <string>

//! File open mode
enum PG_OPEN_MODE {
  PG_OPEN_READ, //!< Open file for reading
  PG_OPEN_WRITE, //!< Open file for writing
  PG_OPEN_APPEND //!< Open file for writing, appending data to the end of the file
};

//! PG_FileList type (vector of strings)
typedef std::vector< std::string > PG_FileList;

/**
 * @author Alexander Pipelka
 *
 * @short A C++ wrapper for the PhysicsFS
*/

class PG_FileArchive {
public:
	PG_FileArchive();
	~PG_FileArchive();

	static void Deinit();

	/**
	Add a directory or archive file to the pool
	@param arch	directorypath or filename
	@param append true - append / false - prepend
	@return true on success
	*/
	static bool AddArchive(const char* arch, bool append = true);

	/**
	Remove a directory or archive file from the pool
	@param arch	directorypath or filename
	@return true on success
	*/
	static bool RemoveArchive(const char* arch);

	/**
	Remove all currently registered directories or archive files from the pool
	*/
	static bool RemoveAllArchives();

	//! Get a list of registered archives
	/*!

	  This function returns a NULL terminated char ** with all
	  currently registered archives (AddArchive).

	 \note The caller is responsible for freeing the array and the
	 entries within the array. (FreeList)
	
	 \return the file list
	*/
	static char **GetSearchPath();

	//! Free a list of entries
	/*!

	  This function returns frees a previously retrieved list (GetSearchPath()).

	 \param the list to free
	*/
	static void FreeList(void* list);

	//! Get a list of files in a directory
	/*!

	  This function returns a NULL terminated char ** with the
	  files in the specified directory. If the directory doesn't
	  exist, NULL is returned.

	 \note The caller is responsible for freeing the array and the
	 directories within the array.
	
	 \param dir directory to list files in
	 \return the file list
	*/
	static char **EnumerateFiles(const char *dir);

	/**
	Check if a file exists in the archive
	@param filename file to check
	@return true if the file exists
	*/
	static bool Exists(const char *filename);

	/**
	Check if the "filename" is a directory
	@param filename filename to check
	@return true if it's a directory
	*/
	static bool IsDirectory(const char *filename);

	/**
	Get the platform specifiy directory separator
	@return pointer to separator string
	*/
	static const char* GetDirSeparator();

	static const char* GetRealDir(const char* filename);

	static const char* GetLastError();
	//! Create a new directory
	/*! 
	  This function tries to create a directory in the application
	  write directory, if any. 
	\param dir directory name
	*/
	static bool MakeDir(const char* dir); 

	//! Set write directory
	/*! 
	  This function sets the write directory to use.
	  \param dir directory name
	*/
	static bool SetWriteDir(const char* dir); 

	//! convert a path from UNIX to system dependent format
	/*!
	  This function converts / in a path to the system
	  dependent format, which for example is \\ in Windows and :
	  on MacOS.
	\param path path to convert
	\return a string with the converted path
	*/
	static std::string *PathToPlatform(const char *path);

	//! Get the application base directory
	/*! 
	  \return the application base directory
	*/
	static const char* GetBaseDir();

	//! Get the users home directory
	/*! 
	  This functions tries to find the users home directory. If it can't
	  find it, it returns a directory relative to the application
	  base directory.
	  \return the path to the user home directory
	*/
	static const char* GetUserDir();

	//! Get the designated write directory
	/*! 
	  This function returns the directory which is opened for writing,
	  if any.  
	  \return the directory opened for writing
	*/
	static const char* GetWriteDir();

	//! Setup a sane config for a typical application
	/*! 
	  This function is a convenience function which tries to open / create
	  a directory in USERHOME/.organization/appName/ and add it as the write directory,
	  add the base directory to the load path, optionally adds mounted cdroms
	  to the searchpath and finally optiopnally adds files with
	  the extension archiveExt to the load path (if any).

	  \param organization name of entity owning the application
	  \param appName name of application
	  \param archiveExt archive file extension to look for (0)
	  \param includeCdRoms whether to add mounted CDROMS to the search path (false)
	  \param archivesFirst put archive files before the real directory (true)
	  
	  \return true for success, false for failure
	*/
	static bool SetSaneConfig(const char *organization,
				  const char* appName,
				  const char* archiveExt = 0,
				  bool includeCdRoms = false,
				  bool archivesFirst = true);
	
	/**
	   Open a file from the archive
	   @param filename name of the file to open
	   @param mode the open mode
	   @return pointer to newly created PG_File object or NULL
	*/
	static PG_File* OpenFile(const char* filename,
				 PG_OPEN_MODE mode = PG_OPEN_READ);
	  
//	/**
//	   Open a file from the archive
//	   @param filename name of the file to open
//	   @param mode the open mode
//	   @return pointer SDL_RWops structure
//	*/
//	static SDL_RWops* OpenFileRWops(const char* filename,
//				 PG_OPEN_MODE mode = PG_OPEN_READ);
//	  
//	//! Open and read a file from the archive
//	/*!
//	  This is a utility function which opens and reads a file in one step.
//	  The returned string should be deleted when it's not needed anymore.
//	  \param filename name of file to read
//	  \return pointer to a string with the data of the named file
//	  or 0 if the file doesn't exist or was empty
//	*/
//	static PG_DataContainer* ReadFile(const char* filename);
//	
//	/**
//	Load a surface (image) from the archive
//	all loaded surface will be cached
//	@param filename imagefile to load
//	@param convert convert the loaded surface into screen format (default = false)
//	@return pointer to loaded SDL_Surface or NULL
//	*/
//	static SDL_Surface* LoadSurface(const char* filename, bool convert = false);
//	/**
//	Load a surface (image) from the archive and apply a colorkey
//	all loaded surface will be cached
//	@param filename imagefile to load
//	@param usekey true - apply colorkey
//	@param colorkey colorkey
//	@param convert convert the loaded surface into screen format (default = false)
//	@return pointer to loaded SDL_Surface or NULL
//	*/
//	static SDL_Surface* LoadSurface(const char* filename, bool usekey, Uint32 colorkey, bool convert = false);
//	
//	/**
//	Removes the surface from the cache
//	@param bDeleteIfExists delete (free) the surface if it isn't in the cache
//	@return pointer to the SDL_Surface to remove
//	*/
//	static bool UnloadSurface(SDL_Surface* surface, bool bDeleteIfNotExists = true);

	/**
	Enable / disable the use of symlinks.
	By default, the file I/O routines will check if a file is really a symlink during open
		calls and fail if it is. Otherwise, the link could take you outside the
		write and search paths, and compromise security.

		Symbolic link permission can be enabled or disabled at any time, and is
		disabled by default
	*/
	static void EnableSymlinks(bool followSymlinks);

	//! Get the names of all registered archives
	/*!
	
		This function returns a vector of strings containing the names
		of all currently registered archives (Added via AddArchive)
	
	\note The caller owns the returned vector and is responsible for deleting the returned structure.
	
	\return the file list
	*/
	static PG_FileList* GetSearchPathList();
	
	
	//! Get the names of all files in a directory
	/*!
	
		This function returns a vector of strings containing the names
		of all files in the specified directory.
	
	\note The caller owns the returned vector and is responsible for deleting the returned structure.
	\param dir directory to list files in
	\param wildcard wildcard for file pattern matching
	\return the file list
	*/
	static PG_FileList* GetFileList(const char *dir, const char* wildcard="*");
	
private:

	static unsigned int my_instance_count;
//	static PG_SurfaceCache my_cache;
};

#endif
