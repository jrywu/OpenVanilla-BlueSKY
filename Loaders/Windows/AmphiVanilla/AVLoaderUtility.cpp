//#define OV_DEBUG
#ifndef WINCE
#ifdef UNICODE
#undef UNICODE
#endif
#ifdef _UNICODE
#undef _UNICODE
#endif
#endif

#include "AVLoaderUtility.h"
#include <string>
using namespace std;

vector<OVLibrary*> ovlib_vector;
vector<OVModule*> *mod_vector;
static OVService *lsrv;

static OVLibrary* open_module(const char* modname){
	murmur("enter open_module for %s", modname);
    OVLibrary* mod = new OVLibrary();
#ifndef WIN32 //WINCE
    mod->handle = lt_dlopen(modname);
#else
	WCHAR wbuf[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, modname, (int)strlen(modname)+1, wbuf, MAX_PATH);
    mod->handle = LoadLibraryW(wbuf);
	
#endif
    if(mod->handle == NULL){
		murmur("dynamic library open %s failed\n", modname);
		goto OPEN_FAILED;
    }
#ifdef WIN32
	
#endif 
#ifdef WINCE
	mod->getModule = (TypeGetModule)GetProcAddressW( mod->handle, 
	    L"OVGetModuleFromLibrary" );
    mod->getLibVersion = (TypeGetLibVersion)GetProcAddressW( mod->handle, 
		L"OVGetLibraryVersion" );
	mod->initLibrary = (TypeInitLibrary)GetProcAddressW( mod->handle,
	    L"OVInitializeLibrary" );
#elif WIN32
	mod->getModule = (TypeGetModule)GetProcAddress( mod->handle, 
	    "OVGetModuleFromLibrary" );
    mod->getLibVersion = (TypeGetLibVersion)GetProcAddress( mod->handle, 
		"OVGetLibraryVersion" );
	mod->initLibrary = (TypeInitLibrary)GetProcAddress( mod->handle,
	    "OVInitializeLibrary" );
#else
	mod->getModule = (TypeGetModule)lt_dlsym( mod->handle, 
	    "OVGetModuleFromLibrary" );
    mod->getLibVersion = (TypeGetLibVersion)lt_dlsym( mod->handle, 
	    "OVGetLibraryVersion" );
	mod->initLibrary = (TypeInitLibrary)lt_dlsym( mod->handle,
	    "OVInitializeLibrary" );
#endif
	if( !mod->getModule || !mod->getLibVersion || !mod->initLibrary ){
		murmur("open dynamic library failed %s failed\n", modname);
		goto OPEN_FAILED;
    }
    if( mod->getLibVersion() < OV_VERSION ){
		murmur("%s %d is too old\n", modname, mod->getLibVersion());
		goto OPEN_FAILED;
    }

    return mod;

OPEN_FAILED:
    delete mod;
    return NULL;
}

static void load_module(string path, string file)
{
    murmur("Load OV module: %s\n", file.c_str());

#if defined(WIN32) && !defined(WINCE)
	string xpath;
#ifdef X64
	xpath= path + "x64\\";
#else
	xpath= path + "x86\\";
#endif	
	OVLibrary* mod = open_module((xpath + file).c_str());
#else
	OVLibrary* mod = open_module((path + file).c_str());
#endif
    
    if(mod){
		OVModule* m;
		mod->initLibrary(lsrv, path.c_str());
		for(int i=0; m = mod->getModule(i); i++)
		{	
			mod_vector->push_back(m);
			murmur("Load OVModule: %s\n", m->localizedName("zh_TW"));
		}
		ovlib_vector.push_back(mod);
	}
}
typedef void (*loadfunc)(string path, string file);

// scan_dir is platform dependent, each platform must have it's own implement
static void scan_dir(string path, loadfunc func)
{
#if defined(WIN32)||defined(WINCE)
    BOOL fFinished;
    HANDLE hList;
    WIN32_FIND_DATAW FileData;
	
	WCHAR wbuf[MAX_PATH];
	char buf[MAX_PATH];

	
#ifndef WINCE
	string xpath;
#if defined X64
	xpath= path + "x64\\";
#else
	xpath= path + "x86\\";
#endif	
	murmur("Altered Module dir:%s\n", xpath.c_str());	
	MultiByteToWideChar(CP_UTF8, 0, (xpath + "*").c_str(), (int)(xpath.length()+2), wbuf, MAX_PATH);
#else
	MultiByteToWideChar(CP_UTF8, 0, (path + "*").c_str(), (int)(path.length()+2), wbuf, MAX_PATH);
#endif
	

    hList = FindFirstFileW(wbuf, &FileData);
    if(hList == INVALID_HANDLE_VALUE)
    {
		murmur("No files found\n");
    }
    else
    {
	fFinished = FALSE;
	while (!fFinished)
	{
	    if(wcsstr(FileData.cFileName, L".dll") || wcsstr(FileData.cFileName, L".DLL"))
	    {
			WideCharToMultiByte(CP_UTF8, 0, FileData.cFileName, (int)(wcslen(FileData.cFileName)+1), buf, MAX_PATH,NULL, NULL);
			func(path, buf);
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
#else
#endif
}

void AVLoadEverything(string path, OVService *srv, vector<OVModule*> &vector)
{   
    mod_vector = &vector;
#ifndef WIN32 // WINCE
    lt_dlinit();
    lt_dlsetsearchpath(path.c_str());
#endif
    lsrv = srv;
    scan_dir(path, load_module);
}

void AVUnloadLibrary(vector<OVModule*> &vec)
{
	murmur("AVUnloadLibrary()" );
	
#ifdef WIN32 //WINCE
	//unload all dynamic loaded DLLs.
	for(size_t i = 0; i < ovlib_vector.size(); i++)
		if(ovlib_vector.at(i) != NULL){
			FreeLibrary(ovlib_vector.at(i)->handle);
		}
#else
	lt_dlexit();
#endif
	// clear vectors;
	ovlib_vector.clear();
	vec.clear();

}
