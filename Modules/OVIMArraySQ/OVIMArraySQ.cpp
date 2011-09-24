﻿// OVIMArray.h: The Array Input Method
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

#define OV_DEBUG
#include "ctype.h"
#include "OVIMArraySQ.h"
#ifndef WIN32
	#include <OpenVanilla/OVLibrary.h>
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OVLibrary.h"
	#include "OVUtility.h"

	//<comment author='b6s'> Uses OVOSDef.h in OVIMArray.h instead.
	//#include <stdlib.h>
	//#define PATH_MAX _MAX_PATH
	//</comment>
#endif
#include <utility>
#ifdef OSX_INCLUDE
        #include <Carbon/Carbon.h>
#endif

using namespace std;
using namespace OV_Array;


OVCINSQList *cinlist=NULL;

void OVIMArrayContext::clear()
{
    keyseq.clear();
    changeState(STATE_WAIT_KEY1);
}

void OVIMArrayContext::updateDisplay(OVBuffer* buf)
{
    buf->clear();
    if (keyseq.length()) {
        string str;
        keyseq.compose(str);
        buf->append(str.c_str());
    }
    buf->update();
}
void OVIMArrayContext::updateFreqFetchAssociatedPhrase(const string& phrase, OVCandidate *textbar ){
    

	if(assoconduty) tabs[MAIN_TAB]->updatePhraseUserFrequency(
            (lastPhrase + phrase).c_str(), parent->doLearnAssociatedPhrase(), true); // update user freq.
	else	
        tabs[MAIN_TAB]->updatePhraseUserFrequency(
            phrase.c_str(), parent->doLearnAssociatedPhrase(), false); // update user freq.
			lastPhrase = phrase;

			if(assoconduty) assoconduty = false;
			
			if( parent->doAssociatedPhrase() &&
				tabs[MAIN_TAB]->getAssociatedPhrases(phrase, candidateStringVector, parent->doLearnAssociatedPhrase())) {
			//Fetch associated phrase... 
#if defined(WIN32) && !defined(WINCE)
					candi.prepare(&candidateStringVector, parent->getAssocSelKey()
						, textbar, "1234567890", string("Shift+#\xE3\x80\x80"));
#else
            murmur("fetching asscociated phrase list with selkey = %s", parent->getAssocSelKey().c_str());
			candi.prepare(&candidateStringVector, parent->getAssocSelKey().c_str() , textbar); 
#endif
			assoconduty = true;
			}
}

int OVIMArrayContext::updateCandidate(OVCINSQ *tab,OVBuffer *buf, OVCandidate *candibar)
{
    tab->getWordVectorByCharWithWildcardSupport(keyseq.getSeq(), candidateStringVector, '?', '*');
    string currentSelKey = tab->getSelKey();
    if (candidateStringVector.size() == 0) {
        clearCandidate(candibar);	
	}
    else {
        candi.prepare(&candidateStringVector,
                          const_cast<char*>(currentSelKey.c_str()), candibar);
	}
    return 1;
}

int OVIMArrayContext::WaitKey1(OVKeyCode* key, OVBuffer* buf, 
                               OVCandidate* candibar, OVService* srv)
{
    if (keyseq.length() != 1)
        return 0;

	if (!keyseq.hasWildcardCharacter())
    	updateCandidate(tabs[SHORT_TAB], buf, candibar);

    char keycode = keyseq.getSeq()[0];

    if (keycode == 't')
        buf->clear()->append((char*)"\xe7\x9a\x84")->update();

    if (isprint(keycode) && keyseq.valid(keycode))
        changeState(STATE_WAIT_KEY2);

    return 1;
}

int OVIMArrayContext::WaitKey2(OVKeyCode* key, OVBuffer* buf, 
                               OVCandidate* candibar, OVService* srv)
{
    if (keyseq.length() != 2)
        return 0;

    char keycode = keyseq.getSeq()[1];

    if (isWSeq(keyseq.getSeq()[0], keyseq.getSeq()[1])) {
        updateCandidate(tabs[MAIN_TAB], buf, candibar);
        buf->clear()->append(candidateStringVector[0].c_str())->update();
        changeState(STATE_WAIT_CANDIDATE);
    }
    else {
		if (!keyseq.hasWildcardCharacter())
        	updateCandidate(tabs[SHORT_TAB], buf, candibar);

        if (isprint(keycode) && keyseq.valid(keycode))
            changeState(STATE_WAIT_KEY3);
    }
    return 1;    
}

int OVIMArrayContext::WaitKey3(OVKeyCode* key, OVBuffer* buf, 
                               OVCandidate* candibar, OVService* srv)
{
    if (keyseq.length() >= 3) {
		if (!keyseq.hasWildcardCharacter())
        	updateCandidate(tabs[MAIN_TAB], buf, candibar);
    }
    return 1;    
}


int OVIMArrayContext::WaitCandidate(OVKeyCode* key, OVBuffer* buf, 
                                    OVCandidate* candibar, OVService* srv)
{
    const char keycode = key->code();
	murmur("WaitCandidate(), key->code = %c ", keycode);

    if (keycode == ovkEsc || keycode == ovkBackspace) {
        clearAll(buf, candibar);
        changeState(STATE_WAIT_KEY1);
        return RET_DONE;
    }

    if (keycode == ovkDown || keycode == ovkRight ||
        (!candi.onePage() && keycode == ovkSpace)) {
        candi.pageDown()->update(candibar);
        return RET_DONE;
    }

    if (keycode == ovkUp || keycode == ovkLeft) {
        candi.pageUp()->update(candibar);
        return RET_DONE;
    }

    // enter == first candidate
    // space (when candidate list has only one page) == first candidate
	char c = key->code();
	bool notSelkey = true;
    bool defaultSelKey = false;
    
    
	if(!assoconduty){
		notSelkey = tabs[MAIN_TAB]->getSelKey().find(keycode) == string::npos;
		defaultSelKey = (keycode == ovkReturn || (candi.onePage() && keycode == ovkSpace));
		if (defaultSelKey || notSelkey) 
			c = candi.getSelKey()[0];
		murmur("assoc is not on duty, notSelkey: %d", notSelkey);
	}else{
		notSelkey = string("!@#$%^&*()").find(keycode) == string::npos;
		defaultSelKey = false;
		murmur("assoc is on duty, notSelkey: %d", notSelkey);
	}
    string output;

    if (candi.select(c, output)) {
        sendAndReset(output.c_str(), buf, candibar, srv);
        if (notSelkey && !defaultSelKey)
            return RET_CONTINUE;
        return RET_DONE;
    }    
    return RET_PASS;
}


void OVIMArrayContext::queryKeyName(const char *keys, std::string& outKeyNames)
{
    int len = strlen(keys);
    for (int i = 0; i < len; i++) {
        string inKey(keys, i, 1);
        vector<string> keyName;
        if (tabs[MAIN_TAB]->getCharVectorByKey(inKey, keyName) > 0)
            outKeyNames.append(keyName[0]);
    }
}

void OVIMArrayContext::sendAndReset(const char *ch, OVBuffer* buf, 
                                    OVCandidate* candibar, OVService* srv)
{
    bool notifySP = false;
    
    // lookup special code
    if((parent->isAutoSP() || parent->isForceSP()) &&
       tabs[SPECIAL_TAB]->getWordVectorByCharWithWildcardSupport(ch, specialCodeVector, '?', '*') > 0) {
        int splen = specialCodeVector[0].length();
        const char *spcode = specialCodeVector[0].c_str();
        if (!(splen == keyseq.length() && 
              equal(spcode, spcode+splen, keyseq.getSeq()))) {
            char buf[16];
            string keynames;
            queryKeyName(specialCodeVector[0].c_str(), keynames);
            sprintf(buf, "%s: %s", ch, keynames.c_str());
            srv->notify(buf);
            notifySP = true;
        }
    }

    bool committed = false;

    if (isForceSPSeq()) {
        parent->setForceSP(!parent->isForceSP());
	}
    else {
        if (!(parent->isForceSP() && notifySP)) {
            buf->clear()->append(ch)->send();
            committed = true;
        }
        else {
            srv->beep();
		}
    }
    
    // clearAll(buf, candibar);
    
    // we can't call clearAll becaue clearAll does one extra clean-up, this causes problem in WoW
    clearCandidate(candibar);
    if (!committed)
        buf->clear()->update();
    keyseq.clear();
        
    changeState(STATE_WAIT_KEY1);
	updateFreqFetchAssociatedPhrase(ch,candibar);
}

void OVIMArrayContext::clearAll(OVBuffer* buf, OVCandidate* candi_bar)
{
    clearCandidate(candi_bar);
    buf->clear()->update();
    keyseq.clear();
}

void OVIMArrayContext::clearCandidate(OVCandidate *candi_bar)
{
    candi.cancel();
    candi_bar->hide()->clear();
}   

int OVIMArrayContext::selectCandidate(int num, string& out)
{
	char c = candi.getSelKey()[num];
	return candi.select(c, out);
}

int OVIMArrayContext::keyEvent(OVKeyCode* key, OVBuffer* buf, 
                               OVCandidate* candi_bar, OVService* srv)
{
	//murmur("keyEvent keycode= %c", key->code());
	
	int ret = 0;
    const char keycode = key->code();
    const bool validkey = keyseq.valid(keycode) || 
      ( keyseq.getSeq()[0] == 'w' && isdigit(keycode) );
	
	//proces associated phrase ------------------
	const bool validassockey = assoconduty &&
		(string(parent->getAssocSelKey()).find(keycode) != string::npos);
	murmur("assockey: %s , assoconduty: %d, validassockey: %d", parent->getAssocSelKey(), assoconduty, validassockey);
	if(assoconduty && !validassockey){
		assoconduty = false;
		clearCandidate(candi_bar);
	}
	//-------------------------------------------

    murmur("OVIMArray state: %d", state);
    if (!keyseq.length() && !isprint(keycode))
		return 0;
    if (!keyseq.length() && key->isFunctionKey())
		return 0;
    murmur("keyevent pos 0");
    if (isprint(key->code()) && key->isCapslock() && keyseq.length() == 0) {
        char cbuf[2];
        if (key->isShift())
            sprintf(cbuf, "%c", toupper(key->code()));
        else
            sprintf(cbuf, "%c", tolower(key->code()));

        buf->append(cbuf)->send();
        return 1;
    }
	murmur("keyevent pos 1");
    if (!validassockey && key->isShift() && isprint(keycode) && keyseq.length() == 0 && keycode != '*' && keycode != '?') {
        char k[2] = { tolower(keycode), 0 };
        buf->append(k)->send();
        return 1;
    }
	murmur("keyevent pos 2");
    if (!validassockey && !keyseq.length() && !validkey) {

        char k[2] = { tolower(keycode), 0 };
        buf->append(k)->send();
        return 1;
    }
	murmur("keyevent pos 3");
    if(keycode==ovkEsc){
        clearAll(buf, candi_bar);
        changeState(STATE_WAIT_KEY1);
        return 1;
    }

	// cancels candidate window if wildcard character is entered
	if (!keyseq.hasWildcardCharacter() && (keycode == '?' || keycode== '*')) {
		murmur("candidate canceled because of wildcard");
		clearCandidate(candi_bar);
		state = STATE_WAIT_KEY3;
	}

    if( validassockey || state == STATE_WAIT_CANDIDATE ){
        int r = WaitCandidate(key, buf, candi_bar, srv);
        if (r != RET_CONTINUE) 
			return r;
    }

    if (candi.onDuty() && isdigit(keycode) && 
        !(keyseq.length() == 1 && isWSeq(keyseq.getSeq()[0],keycode))) {
        string c;
        if (candi.select(keycode, c)){
            if (c != "?" ) {
                sendAndReset(c.c_str(), buf, candi_bar, srv);
            }
            else {
                srv->beep();
                if (state <= STATE_WAIT_KEY3 ) //dirty hack to set duty=1
                    updateCandidate(tabs[SHORT_TAB], buf, candi_bar);
            }
        }
        return 1;
    }
    
    if (keyseq.length() && keycode == ovkSpace) {
        tabs[MAIN_TAB]->getWordVectorByCharWithWildcardSupport(keyseq.getSeq(), candidateStringVector, '?', '*');
        string c;
        if (candidateStringVector.size() == 1) {
            if (selectCandidate(0, c))
                sendAndReset(c.c_str(), buf, candi_bar, srv);
        }
        else if (candidateStringVector.size() > 1) {
            updateCandidate(tabs[MAIN_TAB], buf, candi_bar);
            if (selectCandidate(0, c))
                buf->clear()->append(c.c_str())->update();
            changeState(STATE_WAIT_CANDIDATE);
        }
        return 1;
    }

    if (isprint(keycode) && validkey) {
        if( keyseq.length() >= 5 ||
            (keyseq.length() == 4 && keycode != 'i') )
            return 1;
        murmur("keyseq add [%c]", keycode);
        keyseq.add(keycode);
        updateDisplay(buf);
        ret = 1;
    }
    else if (key->code() == ovkDelete || key->code() == ovkBackspace) {
        keyseq.remove();
        updateDisplay(buf);
        if(keyseq.length() == 0 && candi.onDuty())
            clearCandidate(candi_bar);
        changeBackState(state);
        ret = 1;
    }
    dispatchStateHandler(key, buf, candi_bar, srv);
    return ret;
}

void OVIMArrayContext::dispatchStateHandler(OVKeyCode* key, OVBuffer* buf, 
                                            OVCandidate* candi_bar, OVService* srv)
{
	murmur("dispatchStateHandler():%d", state);
    switch(state){
        case STATE_WAIT_KEY1:
            WaitKey1(key, buf, candi_bar, srv);
            break;
        case STATE_WAIT_KEY2:
            WaitKey2(key, buf, candi_bar, srv);
            break;
        case STATE_WAIT_KEY3:
            WaitKey3(key, buf, candi_bar, srv);
            break;
        default:
            break;
    }
}

void OVIMArrayContext::changeBackState(STATE s)
{
    switch(s){
        case STATE_WAIT_CANDIDATE:
        case STATE_WAIT_KEY2: 
            changeState(STATE_WAIT_KEY1); 
            break;
        case STATE_WAIT_KEY3: 
            if (keyseq.length() == 2)
                changeState(STATE_WAIT_KEY2); 
            else if (keyseq.length() == 1)
                changeState(STATE_WAIT_KEY1);
            break;
        default: break;
    }
}

void OVIMArrayContext::changeState(STATE s)
{
    murmur("OVIMArray change state: %d -> %d", state, s);
    state = s;  
}

OVIMArray::OVIMArray(const OVCINSQList& cl,SQLite3 *dbo) : cinlist(cl), db(dbo), assoctab(NULL) {
   murmur("OVIMArray constructor");
}

int OVIMArray::initialize(OVDictionary *conf, OVService* s, const char *path)
{
	s->notify("Updating table..."); 
   
    char arraypath[PATH_MAX];
    char buf[PATH_MAX];
    char *cinfiles[] = { 
        "array30.cin",  
        "array-shortcode.cin",
        "array-special.cin"
    };

	if(!assoctab){
		murmur("OVIMArraySQ: open associate phrase table");
		assoctab = new OVCINSQ(cinlist.getAssocCinInfo(),db);	
		for (int i = 0; i < 3 ; i++) {
			for(int x =0; x< cinlist.count(); x++){
				if(cinlist.cinInfo(x).shortfilename == string(cinfiles[i])){
					tabs[i] = new OVCINSQ(cinlist.cinInfo(x), db); 
					murmur("OVIMArraySQ: open cin tabble %s", cinfiles[i]);
				}
			}
		}
    }
    updateConfig(conf);
    return 1;
}

int OVIMArray::updateConfig(OVDictionary *conf){
    const char *AutoSP = "spcialCode";//"\xE7\x89\xB9\xE5\x88\xA5\xE7\xA2\xBC\xE6\x8F\x90\xE7\xA4\xBA"; // 特别码模式
    const char *ForceSP = "quickMode"; //"\xE5\xBF\xAB\xE6\x89\x93\xE6\xA8\xA1\xE5\xBC\x8F"; // 快打模式
	
	#define CIN_ASSOCIATEDPHRASE	"associatedPhrase"
	#define CIN_LEARNASSOCIATEDPHRASE	"learnAssociatedPhrase"
	#define CIN_ORDERWORDSBYFREQ	"orderWordsByFreq"

    if (!conf->keyExist(AutoSP))
		conf->setInteger(AutoSP, 1);
		
    if (!conf->keyExist(ForceSP))
		conf->setInteger(ForceSP, 0);

    cfgAutoSP = conf->getInteger(AutoSP);
    cfgForceSP = conf->getInteger(ForceSP);
	cfgAssociatedPhrase=conf->getIntegerWithDefault(CIN_ASSOCIATEDPHRASE, 1) == 0 ? false : true;
	cfgLearnAssociatedPhrase=conf->getIntegerWithDefault(CIN_LEARNASSOCIATEDPHRASE, 1) == 0 ? false : true;
	cfgOrderWordsByFreq=conf->getIntegerWithDefault(CIN_ORDERWORDSBYFREQ, 0) == 0 ? false : true;


    return 1;
}

//OV_SINGLE_MODULE_WRAPPER(OVIMArray);
	extern "C" unsigned int OVGetLibraryVersion() { return OV_VERSION; } 
    extern "C" int OVInitializeLibrary(OVService* s, const char* libpath) {
		if (cinlist) {
        // already initializde
			return false;
		}
		const char *pathsep=s->pathSeparator();
 
		// will be something like this on OS X (IMK):
		//     ~/Library/Application Support/OpenVanilla/UserData/OVIMArray/
		// will be something like this on Windows Vista/7+
		//     c:\users\[username]\appdata\roadming\Openmanilla\OVIMArray
		string userpath=s->userSpacePath("OVIMArray");
		murmur(" userSpacePath %s", userpath.c_str());
    
		// will be something like this on OS X (TSM):
		//     /Library/OpenVanilla/version/Modules/OVIMGeneric/
		// and inside the bundle of OS X (IMK
#ifndef WIN32
        string datapath=string(libpath);
#else
		string datapath=string(libpath) + string(pathsep) + string("OVIMArray");
#endif
		murmur(" dataPath %s", datapath.c_str());
 
		cinlist = new OVCINSQList(pathsep, datapath.c_str(), userpath.c_str(), "ovimarray.db");
		if (!cinlist) return false;

		int loaded=0;
		loaded += cinlist->load(datapath.c_str(), ".cin", true);
    	if (!loaded) 		
			loaded = cinlist->loadfromdb(); // Jeremy '11,9,22 if no .cin found in datapath, load cinlist from imdb.

		loaded += cinlist->load(userpath.c_str(), ".cin", false);
		
		if (!loaded)
		{
			murmur ("OVIMArray: nothing loaded, init failed");
			return false;
		}
		murmur ("OVIMArray: cin files init finished.");
			return true; 
	} 
	extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
		  return (idx==0) ? new OVIMArray(*cinlist, cinlist->getdb()): NULL; 
    }

