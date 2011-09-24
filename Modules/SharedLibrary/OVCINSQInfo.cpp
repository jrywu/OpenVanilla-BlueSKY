// OVCINSQInfo.cpp 
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
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


//#define OV_DEBUG

#include "OVCINSQInfo.h"

#if !defined(WIN32) && !defined(WINCE)
    #include <OpenVanilla/OpenVanilla.h>
    #include <OpenVanilla/OVUtility.h>
    #include <sys/param.h>
    #include <sys/types.h>
    #include <dirent.h>
	#include <sys/stat.h> // for stat() file info retreviing.
    #include <unistd.h>
#else   
    #include "OpenVanilla.h"
    #include "OVUtility.h"
	//<comment author='b6s'> Uses OVOSDef.h instead.
	//#define strcasecmp stricmp
	//</comment>
	#include <string.h>	//memset
	#define bzero(addr, num) memset(addr, 0, num)

	#include <windows.h> //FindFirstFile
	#define strcasecmp _stricmp
#endif

#include "OVOSDef.h"

const char *clExtension=NULL;

void CLSplitString(const char *s, string& k, string& v) {
   
    // find first \s
    size_t fs=strcspn(s, " \t");
    
    // find then the non \s
    size_t fl=strspn(s+fs, " \t");
    
    // find until end
    size_t en=strcspn(s+fs+fl, "\n\r");

    string ss(s);
    k=ss.substr(0, fs);
    v=ss.substr(fs+fl, en);    
}

#ifndef WIN32
    #ifdef __linux__
       int CLFileSelect(const struct dirent *entry)
    #else
       int CLFileSelect(struct dirent *entry)
    #endif
    {
        const char *selectfilter = clExtension ? clExtension : ".cin";
        int p=strlen(entry->d_name)-strlen(selectfilter);
        if (p<0) return 0;
        if (!strcmp(entry->d_name+p, selectfilter)) return 1; 
        return 0;
    }
#else
    //#error This part of code won't run--needs refactoring

    int file_select(WIN32_FIND_DATA entry)
    {
        char *selectfilter = ".cin";
        size_t p=strlen(entry.cFileName)-strlen(selectfilter);
        if (p<0) return 0;
        if (!strcmp(entry.cFileName+p, selectfilter)) return 1; 
        return 0;
    }
#endif


OVCINSQList::OVCINSQList(const char *pathseparator, const char* datapath, const char* userpath, const char* preloaddbname){
    pathsep=pathseparator;

	string preloadDbpath = OVPathHelper::PathCat(datapath, string(preloaddbname)); //preloaded db path Jeremy '11,9,22
	string dbpath = OVPathHelper::PathCat (userpath, string("imdb.db"));
	string userdbpath = OVPathHelper::PathCat( userpath , string("userdb.db"));
	
	bool preloadDbExist = OVPathHelper::PathExists(preloadDbpath);
	bool imdbExist = OVPathHelper::PathExists(dbpath);
	murmur("Preload db %s exist: %d ; im db %s Exist: %d",preloadDbpath.c_str(), preloadDbExist, dbpath.c_str(), imdbExist);

	if(preloadDbExist && !imdbExist){ // copy preloaded db as imdb in userspace if it's not exist
		murmur("copying preloaded db as imdb in userspace...");
		OVPathHelper::CopyFile(preloadDbpath, dbpath);
	}

	db = new  SQLite3;  
	
	murmur(" Initializing SQLite3 db files...");
	murmur("dbfile:%s",dbpath.c_str());
	if (int err=db->open(dbpath.c_str()) )
        murmur("SQLite3 error! code=%d", err);
   	if(!preloadDbExist && !imdbExist) { // both preload db and user imdb are not exist. new imdb will be created.
		if (int err=db->execute(
			"create table tablelist (name, shortfilename, longfilename, ename, cname, tcname, scname, dwHighTimeStamp, dwLowTimeStamp);"))
			murmur("SQLite3 create table error! code=%d", err);
	}



	
	// Attach user frequency database...
	bool userDbExist = OVPathHelper::PathExists(userdbpath);
	murmur("Attach user database: attach %s as userdb. userDbExist= %d", userdbpath.c_str(), userDbExist);
	if (int err=db->execute("attach '%q' as userdb;", userdbpath.c_str()))
			murmur("SQLite3: attach user  db error! code=%d", err);
	if(!userDbExist) {  // userdb was not exist and new userdb created.
		murmur("userdb was not exist and new userdb created." );	
		if (int err=db->execute("create table userdb.phrase (key, count);"))
			murmur("SQLite3: create userdb.phrase table error! code=%d", err);
		if (int err=db->execute("create index userdb.phrase_index_key on phrase (key);"))
			murmur("SQLite3: create userdb.phrase table index error! code=%d", err);
		if (int err=db->execute("create table userdb.phraseLearned (key, value, ord);"))
			murmur("SQLite3: create userdb.phraseLearned table error! code=%d", err);
		if (int err=db->execute("create index userdb.phraseLearned_index_key on phraseLearned (key);"))
			murmur("SQLite3: create userdb.phraseLearned table key index error! code=%d", err);
		if (int err=db->execute("create index userdb.phraseLearned_index_value on phraseLearned (value);"))
			murmur("SQLite3: create userdb.phraseLearned table value index error! code=%d", err);
	}
	// ---------------------------------------------
}


    int OVCINSQList::load(const char *loadpath, const char *extension, bool preload) {

#ifndef WIN32
        clExtension=extension;
        
        struct dirent **files = NULL;
        struct stat fileinfo;
        int count=scandir(loadpath, &files, CLFileSelect, alphasort);        
        
        int loaded=0;
        for (int i=0; i<count; i++) {
			OVFileTimestamp timestamp = OVPathHelper::TimestampForPath(
				OVPathHelper::PathCat(loadpath, files[i]->d_name), true);
			if(preload)
				 timestamp = OVFileTimestamp(-1,-1);
            if (preparse(loadpath, files[i]->d_name, timestamp)) loaded++;
            free(files[i]);
        }
		
		if (files) {
			free(files);
		}
		
        return loaded;
        
#else   
		int loaded=0;
		BOOL fFinished;
		HANDLE hList;
		WIN32_FIND_DATAW FileData;
		
		string pathString(loadpath);
		pathString += "\\*";
		pathString += extension;
		const char* findpath = pathString.c_str();
		
		WCHAR wbuf[MAX_PATH];
		char buf[MAX_PATH];
		//mbstowcs(wbuf, findpath, MAX_PATH);
		MultiByteToWideChar(CP_UTF8, 0, findpath, (int)(strlen(findpath)+1), wbuf, MAX_PATH);

		hList = FindFirstFileW(wbuf, &FileData);
		if(hList == INVALID_HANDLE_VALUE)
		{
			murmur("No files found in %s\n", pathString.c_str());
		}
		else
		{
			fFinished = FALSE;
			while (!fFinished)
			{
				WideCharToMultiByte(CP_UTF8, 0, FileData.cFileName, (int)(wcslen(FileData.cFileName)+1), buf, MAX_PATH,NULL, NULL);
				murmur(" Checking %s...", buf);
				if(strstr(buf, extension) )
				{
					murmur(" Loading %s...", buf);
					OVFileTimestamp timestamp = OVPathHelper::TimestampForPath(
							OVPathHelper::PathCat(loadpath, OVUTF8::FromUTF16(FileData.cFileName)), true);
					
					if(preload)
						timestamp = OVFileTimestamp(-1,-1);

					if (preparse(loadpath, buf, timestamp)) loaded++;
					
					
				}
				if (!FindNextFileW(hList, &FileData))
				{
					if (GetLastError() == ERROR_NO_MORE_FILES)
					{
						fFinished = TRUE;
					}
				}

			}
		}
		FindClose(hList);
		return loaded;
        
 
#endif
	}
/*
bool OVCINSQList::fileExist(const char *filepath){
#ifndef WIN32
    struct stat fileinfo;
    if(stat(filespath, &fileinfo) == 0)
		return true;
#else
	WIN32_FIND_DATAW FileData;
	WCHAR wbuf[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, filepath, (int)(strlen(filepath)+1), wbuf, MAX_PATH);
	HANDLE hList = FindFirstFileW(wbuf, &FileData);
	if(hList != INVALID_HANDLE_VALUE)
		return true ;
#endif
	return false;
}
*/
bool OVCINSQList::preparse(const char *loadpath, const char *filename, OVFileTimestamp timestamp) {
    // check if a file of the same short name has been alread loaded
	bool exist = false;
    for (size_t i=0; i<list.size(); i++) {
        OVCINSQInfo &x=list[i];
        if (x.shortfilename==filename) {
            murmur("OVCINSQList: file %s not loaded, short name '%s' already exists", loadpath, filename);
			list.erase(list.begin()+i); //removed the old one. replacing for new one..
			exist = true;
            //return false;
        }
    }


    string longname = string(loadpath) + pathsep + string(filename);

    FILE *in=fopen(longname.c_str(), "r");
    if (!in) {
        murmur("OVCINSQList: opening %s failed", longname.c_str());
        return false;
    }

    OVCINSQInfo info;
    info.shortfilename=filename;
    info.longfilename=longname;
	info.timestamp = timestamp;
	//info.highTimeStamp = timestamp.m_timestamp;
	//info.lowTimeStamp = timestamp.m_timestamp;


    int line=0;
    const size_t bs=2049;
    char buf[bs];
    bzero(buf, bs);	
    
    string k, v;
    
    while (!feof(in))
    {
        fgets(buf, bs-1, in);
        if (buf[0]=='#') continue;      // ignore comment

        CLSplitString(buf, k, v);        
        const char *key=k.c_str();
            
		if (!strcasecmp(key, "%ename")) info.ename=v;
		else if (!strcasecmp(key, "%cname")) info.cname=v;
		else if (!strcasecmp(key, "%tcname")) info.tcname=v;
		else if (!strcasecmp(key, "%scname")) info.scname=v;

        line++;
        if (line >= CL_PREPARSELIMIT) break;
    }
   
    fclose(in);

    // some fallbacks..
    if (!info.ename.length()) info.ename=filename;
    if (!info.cname.length()) info.cname=info.ename;
    if (!info.tcname.length()) info.tcname=info.cname;
    if (!info.scname.length()) info.scname=info.cname;

	// insert assoc.cin as the first element.
	if(info.shortfilename==string("assoc.cin"))
	{
		assocCinInfo =info;
		murmur("assoc found");
	}
	else
		list.push_back(info);
	
	murmur("Loaded: longfilename=%s, shortfilename=%s, ename=%s, cname=%s, tcname=%s, scname=%s, highTimeStamp:%d, lowTimeStamp:%d",
	   info.longfilename.c_str(), info.shortfilename.c_str(), info.ename.c_str(), 
	   info.cname.c_str(), info.tcname.c_str(), info.scname.c_str(), (info.timestamp).getTimestamp(), (info.timestamp).getSubTimestamp());
	if(exist) return 0;
    else return 1;
}


int OVCINSQList::loadfromdb(){
		OVCINSQInfo info;
		SQLite3Statement *sth = db->prepare(
			"select shortfilename, longfilename, ename, cname, tcname, scname,\
			dwHighTimeStamp, dwLowTimeStamp from tablelist where name='assoc';"); 
	
		if (sth && sth->step()==SQLITE_ROW) {
			
			info.shortfilename = string(sth->column_text(0));
			info.longfilename = string(sth->column_text(1));
			info.ename = string(sth->column_text(2));
			info.cname = string(sth->column_text(3));
			info.tcname = string(sth->column_text(4));
			info.scname = string(sth->column_text(5));
			//info.highTimeStamp =atoi(sth->column_text(6));
			//info.lowTimeStamp =atoi(sth->column_text(7));
			info.timestamp = ( atol(sth->column_text(6)), atol(sth->column_text(7)));
			
			//list.push_back(info);
			assocCinInfo = info;
			delete sth;
			murmur("Associated phrase table founded, pushed as first element in cinlist");
		}

		sth = db->prepare(
			"select shortfilename, longfilename, ename, cname, tcname, scname, \
				dwHighTimeStamp, dwLowTimeStamp from tablelist where name != 'assoc' \
 				and dwHighTimeStamp = '-1' and dwLowTimeStamp = '-1' ;"); 


		if (sth){
			while (sth->step()==SQLITE_ROW) {
				
				info.shortfilename = string(sth->column_text(0));
				info.longfilename = string(sth->column_text(1));
				info.ename = string(sth->column_text(2));
				info.cname = string(sth->column_text(3));
				info.tcname = string(sth->column_text(4));
				info.scname = string(sth->column_text(5));
				info.timestamp = ( atol(sth->column_text(6)), atol(sth->column_text(7)));
				//info.highTimeStamp =atoi(sth->column_text(6));
				//info.lowTimeStamp =atoi(sth->column_text(7));
			
				list.push_back(info);
			
			}

			
			delete sth;
		} 
		murmur("loadfromdb::get %d tables.", list.size() );
		return (int)list.size();	
}

