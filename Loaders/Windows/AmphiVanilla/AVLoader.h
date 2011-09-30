#include "AVEmbeddedLoader.h"
#include "AVDisplayServer.h"
#include "AVBuffer.h"
#include "AVCandidate.h"
#include "AVKeyCode.h"

#include <vector>

using std::vector;

class AVLoader : public OVBase
{
public:
	static AVLoader* open();
	
	static void close();	

	void initContext(int n);
	void connectDisplayServer(AVDisplayServer *svr);
	bool keyEvent(int n, AVKeyCode c);
	bool moduleName(int i, char *str);
	void unloadCurrentModule();

	
	int getInputMethodCount() { return (int) imCount;} // excluding kp
	int getKeypreprocessorCount() { return (int) kpCount;}
	int getOutputFilterCount() {return (int) ofCount;} // -1 for excluding Fullcharaterwidth OF module.
	

	int getSwitchedBoPoMoFoLayoutModIndex(int currentId);
	void getAllModuleNames(const char* nameList[]);
	OVInputMethodContext* GetcurrentContext() {  
		if(primaryInputMethodIndex > -1)
		return activatedContextArray[primaryInputMethodIndex]; 
		else 
		return NULL;
	};
	const char* getBaseDirectory() ;
	const char* getGlobalConfigKey(const char*);
	void setGlobalConfigKey(const char*, const char*);
	void setGlobalConfig(const char* name);
	void AVLoader::syncMenuToConfig(int);
	void AVLoader::syncConfigToMenu(int);

protected:
	AVLoader();
	~AVLoader();

	void loadPrefs();
	void loadModules();

	AVEmbeddedLoader *em;
	AVDisplayServer *dsvr;
	AVCandidate *candi;
	AVBuffer *buf;

	vector<OVInputMethodContext*> activatedContextArray;
	vector<OVOutputFilter*> activatedOutputFilterArray;


	int primaryInputMethodIndex;

	size_t kpCount;
	size_t imCount;
	size_t ofCount;

	bool hasBoPoMoFo;
	bool hasPhoneticHsu;

private:
	static AVLoader *globalLoader;
};
