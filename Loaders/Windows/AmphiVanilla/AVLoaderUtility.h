#ifndef AVLoaderUtility_h
#define AVLoaderUtility_h

#include "OpenVanilla.h"
#include "OVUtility.h"
#include <string>
#include <vector>
#ifndef WIN32 //WINCE
extern "C" {
#include "ltdl.h"
}
#else
#include <Windows.h>
#endif
using namespace std;

typedef OVModule* (*TypeGetModule)(int);
typedef int (*TypeInitLibrary)(OVService*, const char*);
typedef unsigned int (*TypeGetLibVersion)();
struct OVLibrary {
#ifdef WIN32 //WINCE
   HINSTANCE handle;
#else
   lt_dlhandle handle;
#endif
   TypeGetModule getModule;
   TypeInitLibrary initLibrary;
   TypeGetLibVersion getLibVersion;
};

void AVLoadEverything(string path, OVService *srv, vector<OVModule*> &vector);
void AVUnloadLibrary(vector<OVModule*> &vector);


#endif // AVLoaderUtility_h
