#ifndef __OVCINSQList_h
#define __OVCINSQList_h

#include <string>
#include <vector>
#include "OVSQLite3.h"
using namespace std;

const int CL_PREPARSELIMIT=32;      // preparse won't exceed first 64 lines

struct OVCINSQInfo 
{
	OVCINSQInfo() {}
	OVCINSQInfo(const OVCINSQInfo &c) :
		longfilename(c.longfilename), shortfilename(c.shortfilename),
        ename(c.ename), cname(c.cname), tcname(c.tcname), scname(c.scname), 
		lowTimeStamp(c.lowTimeStamp), highTimeStamp(c.highTimeStamp) {}
    const OVCINSQInfo& operator=(const OVCINSQInfo& c) {
        longfilename=c.longfilename;
        shortfilename=c.shortfilename;
        ename=c.ename;
        cname=c.cname;
        tcname=c.tcname;
        scname=c.scname;
		lowTimeStamp=c.lowTimeStamp;
		highTimeStamp=c.highTimeStamp;
        return *this;
    }

	string longfilename;
    string shortfilename;
    string ename;
    string cname;
    string tcname;
    string scname;

	long lowTimeStamp;
	long highTimeStamp;

};

class OVCINSQList //: public OVCINList
{
public:
	OVCINSQList(const char *pathseparator, const char* datapath, const char* userpath);
    int load(const char *loadpath, const char *extension=".cin", bool preLoad = false);

	int loadfromdb();
	bool OVCINSQList::fileExist(const char *filepath);
	

    size_t count() { return list.size(); }
    const OVCINSQInfo& cinInfo(size_t i) { return list[i]; }
	const OVCINSQInfo& getAssocCinInfo() { return assocCinInfo;}

	SQLite3 *getdb(){ return db;};
	
protected:
    bool preparse(const char *loadpath, const char *filename, long highTimeStamp, long lowTimeStamp);
	SQLite3 *db;
    string pathsep;	

	OVCINSQInfo assocCinInfo;
    vector<OVCINSQInfo> list;
	
	
};

#endif


