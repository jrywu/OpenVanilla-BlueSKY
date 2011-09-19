// OVFileHandler.h: A fast file "slurping" class
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __OVFILEHANDLER_h
#define __OVFILEHANDLER_h

#include <string>
#include <vector>
#include <fstream>

#if defined (WIN32) && !defined (__CYGWIN__)
	#include <windows.h>

	typedef struct {
    	HANDLE hFile;
    	HANDLE hMap;
	} MmapHandles;
#else
	#include <sys/mman.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <stdio.h>
	#include <fcntl.h>
	#include <unistd.h>
#endif

using namespace std;

class OVFileHandler
{
public:
	OVFileHandler(const char* filename);
	~OVFileHandler();

	bool isOpenedByMMAP();
	//bool isOpenedBySTL();

	int getLines(vector<string>& outStringVectorRef);
	
private:
	//ifstream inFile;
	//int filePtr;
	//int getSize();
	//string getFileStringBySTL();
	string getFileStringByMMAP();

	void* openFileByMMAP (const char* /*file_name*/);
	int closeFileByMMAP ();

#if defined (WIN32) && !defined (__CYGWIN__)
	MmapHandles* m_mmapHandle;
#endif

	void* m_mmapPtr;
};

#endif