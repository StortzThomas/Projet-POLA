/**
 * @file polalib.c
 * @author Stortz Thomas
 * @version 1.0
 * @brief library for open function
 * 
 * This file contains open function, for polash projet, that reimplement 
 * standard open function.
 * */

#define _GNU_SOURCE
#include <dlfcn.h>
#define _FCNTL_H
#include <bits/fcntl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

extern int errorno;

int (*_open)(const char * pathname, int flags, ...);
int (*_open64)(const char * pathname, int flags, ...);
int (*_openat)(int dirfd,const char * pathname, int flags, ...);


int open(const char * pathname, int flags, mode_t mode)
{
    _open = (int (*)(const char * pathname, int flags, ...)) dlsym(RTLD_NEXT, "open");
	
	if(flags & O_CREAT)
	{
		return _open(pathname, flags | O_NOATIME, mode);
	}
	else
	{
		
		return _open(pathname, flags | O_NOATIME, 0);	
	}   
}

int openat(int dirfd,const char * pathname, int flags, mode_t mode)
{
    _openat = (int (*)(int dirfd,const char * pathname, int flags, ...)) dlsym(RTLD_NEXT, "openat");
	
	if(flags & O_CREAT)
	{
		return _openat(dirfd,pathname, flags | O_NOATIME, mode);
	}
	else
	{
		
		return _openat(dirfd,pathname, flags | O_NOATIME, 0);	
	}   
}


DIR *opendir(const char *pathname)
{
    	DIR *(*_opendir)(const char *pathname);
	*(void **)(&_opendir) = dlsym(RTLD_NEXT, "opendir");

	DIR *rep = _opendir(pathname);
	return  rep;
}

DIR *fdopendir(int fd)
{
    	DIR *(*_fdopendir)(int fd);
	*(void **)(&_fdopendir) = dlsym(RTLD_NEXT, "fdopendir");

	DIR *rep = _fdopendir(fd);
	return  rep;
}


int open64(const char * pathname, int flags, mode_t mode)
{
    _open64 = (int (*)(const char * pathname, int flags, ...)) dlsym(RTLD_NEXT, "open64");
	
	if(flags & O_CREAT)
	{
		return _open64(pathname, flags | O_NOATIME, mode);
	}
	else
	{
		
		return _open64(pathname, flags | O_NOATIME, 0);	
	}   
}
