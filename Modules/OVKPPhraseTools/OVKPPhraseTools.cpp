﻿// OVKPPhraseTools.cpp: Phrase management tool
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

#include "OVKPPhraseTools.h"
#include <time.h>

const char *PTMsg(OVService *s, const char *en, const char *tw=NULL, 
    const char *cn=NULL);
const char *PTMsg(const char *lc, const char *en, const char *tw=NULL,
    const char *cn=NULL);
bool PTDBErrMsg(const char *m, SQLite3 *db, bool returnValue=false);
bool PTDBErrMsg(OVService *s, const char *m, SQLite3 *db, bool
    returnValue=false);
void PTRecordingMessage(const string& rstr, OVService *s);
void PTModeOnMsg(OVService *s, const string& rstr);
string PTTime();

OVKPPhraseToolsContext::OVKPPhraseToolsContext(OVKPPhraseTools *p) {
    parent=p;
    clear();
}

void OVKPPhraseToolsContext::start(OVBuffer*, OVCandidate*, OVService*) {
}

void OVKPPhraseToolsContext::clear() {
    state=PTS_WAIT;
    parent->recordingMode=false;
    parent->recSeq.clear();
    
    wildcard=false;
    valuelist.clear();
    candilist.clear();
    keyseq.clear();
}

void OVKPPhraseToolsContext::end() {
    clear();
}

int OVKPPhraseToolsContext::keyEvent(OVKeyCode *pk, OVBuffer *pb, 
    OVCandidate *pc, OVService *ps)
{
    k=pk;
    b=pb;
    c=pc;
    s=ps;
    
    switch(state) {
        case PTS_MODE_ACTIVATED:
            return state_activated();
        case PTS_MODE_COMMAND:
            return state_command();
        case PTS_MODE_CANDIDATE:
            return state_candidate();
    }
    
    if (b->isEmpty() && (k->code()==ovkBackspace || k->code()==ovkDelete) &&
        !k->isFunctionKey()) {
        if (parent->recordingMode) {
            parent->recSeq.pop();
            if (!parent->recSeq.count()) {
                parent->recordingMode=false;
                return(cancelmsg(PTS_WAIT, false));
            }
            else {
                PTRecordingMessage(parent->recSeq.compose(), s);
            }
        }
        return false;
    }
    
    if (b->isEmpty() && k->code()==parent->actkey && !k->isFunctionKey()) {
        state=PTS_MODE_ACTIVATED;
        PTModeOnMsg(s, parent->recSeq.compose());
        return true;
    }
    
    return false;
}

// bool OVKPPhraseToolsContext::state_

bool OVKPPhraseToolsContext::state_activated() {
    char foo[8];
    
    // buffer is still empty at this stage,
    // all function key combinations will end this state
    if (k->isFunctionKey()) {
        return cancelmsg(PTS_WAIT, false);
    }   
    
    // if it's once again the activation key, send the same key code back
    if (k->code()==parent->actkey) {
        sprintf(foo, "%c", parent->actkey);
        b->clear()->append(foo)->send();
        return cancelmsg(PTS_WAIT, true);
    }

    if (k->code()==parent->catchkey) {
	//s->notify(PTMsg(s, "Start text recording", "進入記錄模式", "进入记录模式"));
		s->notify(PTMsg(s, "Start text recording", 
			"\xE9\x80\xB2\xE5\x85\xA5\xE8\xA8\x98\xE9\x8C\x84\xE6\xA8\xA1\xE5\xBC\x8F", 
			"\xE8\xBF\x9B\xE5\x85\xA5\xE8\xAE\xB0\xE5\xBD\x95\xE6\xA8\xA1\xE5\xBC\x8F"));
        parent->recordingMode=true;
        state=PTS_WAIT;
        return true;
    }    
    
    if (isprint(k->code())) {
        state=PTS_MODE_COMMAND;
        return state_command();
    }
    
    if (k->code()==ovkEsc) {
        clear();
        return cancelmsg(PTS_WAIT, true);
    }
    
    return cancelmsg(PTS_WAIT, false);
}

bool OVKPPhraseToolsContext::state_command() {
    // now we have something in the buffer, ban all function keys
	string v=parent->recSeq.compose();
    if (k->isFunctionKey()) {
        b->update();
        s->beep();
        PTModeOnMsg(s, v);
        return true;
    }
    
    if (k->code()==ovkDelete || k->code()==ovkBackspace) {
        keyseq.pop();
        if (!keyseq.count()) {  // keyseq now empty
            clearBufCandi();
            return cancelmsg(PTS_WAIT, true);
        }
        PTModeOnMsg(s, v);
        return composeAndUpdateBuffer();
    }
    
    if (k->code()==ovkReturn) {     // process the command!
        return parse_command();
    }
    
    if (k->code()==ovkEsc) {        // escape cancels altogether
        clearBufCandi();
        return cancelmsg(PTS_WAIT, true);
    }
    
    if (k->code()==ovkTab) {        // wildcard search
        return command_search(keyseq.compose(), true);
    }
    
    if (isprint(k->code())) {
        keyseq.add(string(1, k->code()));
        PTModeOnMsg(s, v);
        return composeAndUpdateBuffer();
    }
    
    s->beep();  // non-print char
    return true;
}

bool OVKPPhraseToolsContext::state_candidate() {
    if (k->code()==ovkEsc || k->isFunctionKey()) {
        s->beep();
        clearBufCandi();
        return cancelmsg(PTS_WAIT, true);
    }
    
    switch(k->code()) {    
        case ovkSpace:
        case ovkDown:
        case ovkPageDown:
        case ovkLeft:
            candictrl.pageDown();
            candictrl.update(c);
            return true;
        case ovkUp:
        case ovkPageUp:
        case ovkRight:
            candictrl.pageUp();
            candictrl.update(c);
            return true;
    }

    int selected;
    selected=(k->code()==ovkReturn) ? candictrl.select(parent->selkey[0]) :
        candictrl.select(k->code());

    if (selected == -1) {
        s->beep();
        return true;
    }
    string sel=valuelist[selected];
    b->clear()->append(sel.c_str())->send();
    clearBufCandi();
    
    //string msg(PTMsg(s, "Sent phrase: ", "送出詞彙：", "送出词汇："));
	string msg(PTMsg(s, "Sent phrase: ", 
		"\xE9\x80\x81\xE5\x87\xBA\xE8\xA9\x9E\xE5\xBD\x99\xEF\xBC\x9A", 
		"\xE9\x80\x81\xE5\x87\xBA\xE8\xAF\x8D\xE6\xB1\x87\xEF\xBC\x9A"));
    msg += sel;
    s->notify(msg.c_str());
    
    return true;   
}


bool OVKPPhraseToolsContext::parse_command() {
    string cps=keyseq.compose();
    
    // if it's help
    if (cps=="h" || cps=="H") {
        s->notify(PTMsg(s,
            "commands:\na key -- associate the recorded phrase with key", \
            //"指令：\na key ─ 將記錄下的詞彙以key值存入資料庫",
			"\xE6\x8C\x87\xE4\xBB\xA4\xEF\xBC\x9A\x5C\x6E\x61\x20\x6B\x65\x79\x20\xE2\x94\x80\x20\xE5\xB0\x87\xE8\xA8\x98\xE9\x8C\x84\xE4\xB8\x8B\xE7\x9A\x84\xE8\xA9\x9E\xE5\xBD\x99\xE4\xBB\xA5\x6B\x65\x79\xE5\x80\xBC\xE5\xAD\x98\xE5\x85\xA5\xE8\xB3\x87\xE6\x96\x99\xE5\xBA\xAB", \
            //"指令：\na key ─ 将记录下的词汇以key值存入数据库"));
			"\xE6\x8C\x87\xE4\xBB\xA4\xEF\xBC\x9A\x5C\x6E\x61\x20\x6B\x65\x79\x20\xE2\x94\x80\x20\xE5\xB0\x86\xE8\xAE\xB0\xE5\xBD\x95\xE4\xB8\x8B\xE7\x9A\x84\xE8\xAF\x8D\xE6\xB1\x87\xE4\xBB\xA5\x6B\x65\x79\xE5\x80\xBC\xE5\xAD\x98\xE5\x85\xA5\xE6\x95\xB0\xE6\x8D\xAE\xE5\xBA\x93"));
        return clearBufCandi();
    }
    
    // if it's a command
    char cmdbuf[1024];

    if (cps.length() > 2 && cps[1]==' ') {
        string param=cps.substr(2, cps.length()-2);
        switch(tolower(cps[0])) {
            case 'a':
                return command_add(param);
                
            case 'g':
                sprintf(cmdbuf, "open -a Safari.app \"http://www.google.com.tw/search?q=%s\"", param.c_str());
                system(cmdbuf);
                return clearBufCandi();
        }
            
        s->notify(PTMsg(
            //"command not recognied", "錯誤的指令", "错误的指令"));
			"command not recognied", 
			"\xE9\x8C\xAF\xE8\xAA\xA4\xE7\x9A\x84\xE6\x8C\x87\xE4\xBB\xA4",
			"\xE9\x94\x99\xE8\xAF\xAF\xE7\x9A\x84\xE6\x8C\x87\xE4\xBB\xA4"));
        return clearBufCandi();    
    }
    
    // search and candidate (if any) or commit
    return command_search(cps, false);    
}

bool OVKPPhraseToolsContext::command_add(const string& k) {
    string v=parent->recSeq.compose();
    if (!v.length()) {
        s->notify(PTMsg(s, "No recored phrase to add", \
            //"沒有任何記錄到的詞彙可存入數據庫",
			"\xE6\xB2\x92\xE6\x9C\x89\xE4\xBB\xBB\xE4\xBD\x95\xE8\xA8\x98\xE9\x8C\x84\xE5\x88\xB0\xE7\x9A\x84\xE8\xA9\x9E\xE5\xBD\x99\xE5\x8F\xAF\xE5\xAD\x98\xE5\x85\xA5\xE6\x95\xB8\xE6\x93\x9A\xE5\xBA\xAB", \
            //"没有任何记录到的词汇可存入数据库"));
			"\xE6\xB2\xA1\xE6\x9C\x89\xE4\xBB\xBB\xE4\xBD\x95\xE8\xAE\xB0\xE5\xBD\x95\xE5\x88\xB0\xE7\x9A\x84\xE8\xAF\x8D\xE6\xB1\x87\xE5\x8F\xAF\xE5\xAD\x98\xE5\x85\xA5\xE6\x95\xB0\xE6\x8D\xAE\xE5\xBA\x93"));
        return clearBufCandi();
    }

    if (SQLITE_OK==parent->db->execute(
        "insert into phrase values('%q', '%q', '', '%q', '0');",
        k.c_str(), v.c_str(), PTTime().c_str()))
    {
        string m = k + " => " + v;
        s->notify(m.c_str());
    }
    else {
        PTDBErrMsg(s, PTMsg(s, "Database error", 
			//"資料庫錯誤", "数据库错误"),
			"\xE8\xB3\x87\xE6\x96\x99\xE5\xBA\xAB\xE9\x8C\xAF\xE8\xAA\xA4", "\xE6\x95\xB0\xE6\x8D\xAE\xE5\xBA\x93\xE9\x94\x99\xE8\xAF\xAF"),
            parent->db);
    }
    
    return clearBufCandi();
}

bool OVKPPhraseToolsContext::command_search(const string& k, bool wcard) {
    try {
        SQLite3Statement* st;
        
        wildcard=wcard;
       
        st=parent->db->prepare(wildcard ?
    "select key, value, context from phrase where key like '%q%%' order by freq desc, time desc;" :
    "select key, value, context from phrase where lower(key)=lower('%q') order by freq desc, time desc;", k.c_str());
            
        if (!st) throw "database error";
            
        candilist.clear();
        valuelist.clear();        
        while (st->step()==SQLITE_ROW) {
            string k(st->column_text(0));
            string v(st->column_text(1));
            string x(st->column_text(2));   // context information
            
            valuelist.push_back(v);
            
            string d;
            if (wildcard) d=k + string(" => ");
            d+=v;
            
            if (x.length()) {
                d+=string(" (");
                d+=x;
                d+=string(")");
            }
            
            candilist.push_back(d);
        }
    
        delete st;
            
        // if there is nothing
        if (!valuelist.size()) {
            s->beep();
            s->notify(PTMsg(s, "Not found", 
            //    "找不到具有這個鍵碼的詞", "找不到具有这个键码的词"));
			    "\xE6\x89\xBE\xE4\xB8\x8D\xE5\x88\xB0\xE5\x85\xB7\xE6\x9C\x89\xE9\x80\x99\xE5\x80\x8B\xE9\x8D\xB5\xE7\xA2\xBC\xE7\x9A\x84\xE8\xA9\x9E",\
				"\xE6\x89\xBE\xE4\xB8\x8D\xE5\x88\xB0\xE5\x85\xB7\xE6\x9C\x89\xE8\xBF\x99\xE4\xB8\xAA\xE9\x94\xAE\xE7\xA0\x81\xE7\x9A\x84\xE8\xAF\x8D"));
            
            if (wildcard) return true;  // keep editing
            return clearBufCandi();     // clean everything
        }
        
        // if there is only one candidate and non-wildcard, just commit it
        if (valuelist.size()==1 && !wildcard) {
            b->clear()->append(valuelist[0].c_str())->send();
            return clearBufCandi();
        }
        
        candictrl.prepare(&candilist, (char*)parent->selkey.c_str(), c);
        state=PTS_MODE_CANDIDATE;
        return true;
    }
    catch (const char*) {
        PTDBErrMsg(s, PTMsg(s, "Database error", 
			//"資料庫錯誤", "数据库错误"),
			"\xE8\xB3\x87\xE6\x96\x99\xE5\xBA\xAB\xE9\x8C\xAF\xE8\xAA\xA4", "\xE6\x95\xB0\xE6\x8D\xAE\xE5\xBA\x93\xE9\x94\x99\xE8\xAF\xAF"),
            parent->db);
    }
    return clearBufCandi();
}

bool OVKPPhraseToolsContext::composeAndUpdateBuffer() {
    b->clear()->append(keyseq.compose().c_str())->update();
    return true;
}

bool OVKPPhraseToolsContext::clearBufCandi() {
    if (!b->isEmpty()) b->clear()->update();
    if (c->onScreen()) c->hide()->clear()->update();
    clear();
    return true;
}

bool OVKPPhraseToolsContext::cancelmsg(int newstate, bool returnvalue) {
    state=newstate;
    return cancelmsg(returnvalue);
}

bool OVKPPhraseToolsContext::cancelmsg(bool returnvalue) {
    s->notify(PTMsg(s, "Phrase management mode OFF", 
		//"跳出詞彙管理工具",  "跳出词汇管理工具"));
		"\xE8\xB7\xB3\xE5\x87\xBA\xE8\xA9\x9E\xE5\xBD\x99\xE7\xAE\xA1\xE7\x90\x86\xE5\xB7\xA5\xE5\x85\xB7", \
		"\xE8\xB7\xB3\xE5\x87\xBA\xE8\xAF\x8D\xE6\xB1\x87\xE7\xAE\xA1\xE7\x90\x86\xE5\xB7\xA5\xE5\x85\xB7"));
    clearBufCandi();
    return returnvalue;
}

OVKPPhraseTools::OVKPPhraseTools(bool &cflag, PTKeySequence &wseq, 
    SQLite3 *pdb) : recordingMode(cflag), recSeq(wseq)
{
    db=pdb;    
}

const char *OVKPPhraseTools::localizedName(const char *lc) {
    return PTMsg(lc, "Phrase Management", 
		//"詞彙管理工具", "词汇管理工具");
		"\xE8\xA9\x9E\xE5\xBD\x99\xE7\xAE\xA1\xE7\x90\x86\xE5\xB7\xA5\xE5\x85\xB7", 
        "\xE8\xAF\x8D\xE6\xB1\x87\xE7\xAE\xA1\xE7\x90\x86\xE5\xB7\xA5\xE5\x85\xB7");
}

int OVKPPhraseTools::initialize(OVDictionary *c, OVService* s, const char*) {
    update(c, s);
    return true;
}

void OVKPPhraseTools::update(OVDictionary *cfg, OVService*) {
    // this overwrites and fixes a bug
    const char *ak=cfg->getStringWithDefault("activationKey", PT_DEFKEY);
    const char *ck=cfg->getStringWithDefault("textRecordingKey", PT_CATCHKEY);
    
    if (strlen(ak) > 1) {   // wrong default
        cfg->setString("activationKey", PT_DEFKEY);
    }
    if (strlen(ck) > 1) {
        cfg->setString("textRecordingKey", PT_CATCHKEY);
    }

    actkey=*(cfg->getStringWithDefault("activationKey", PT_DEFKEY));
    catchkey=*(cfg->getStringWithDefault("textRecordingKey", PT_CATCHKEY));
    selkey=cfg->getStringWithDefault("selectionKey", "1234567890");
}

OVInputMethodContext *OVKPPhraseTools::newContext() {
    return new OVKPPhraseToolsContext(this);
}

OVOFTextRecorder::OVOFTextRecorder(bool &cflag, PTKeySequence &wseq) 
    : recordingMode(cflag), recSeq(wseq)
{
}

const char *OVOFTextRecorder::localizedName(const char *lc) {
    return PTMsg(lc, "Phrase Tools: Text Recorder", 
		//"詞彙管理─文字輸出記錄器", "词汇管理─文字输出记录器");
		"\xE8\xA9\x9E\xE5\xBD\x99\xE7\xAE\xA1\xE7\x90\x86\xE2\x94\x80\xE6\x96\x87\xE5\xAD\x97\xE8\xBC\xB8\xE5\x87\xBA\xE8\xA8\x98\xE9\x8C\x84\xE5\x99\xA8", 
        "\xE8\xAF\x8D\xE6\xB1\x87\xE7\xAE\xA1\xE7\x90\x86\xE2\x94\x80\xE6\x96\x87\xE5\xAD\x97\xE8\xBE\x93\xE5\x87\xBA\xE8\xAE\xB0\xE5\xBD\x95\xE5\x99\xA8");
}

const char *OVOFTextRecorder::process(const char* i, OVService* s) {
    if (!recordingMode) return i;

    recSeq.add(string(i));
    PTRecordingMessage(recSeq.compose(), s);
    
    return i; 
}

bool PTDBErrMsg(const char *m, SQLite3 *db, bool returnValue) {
    murmur("OVKPPhraseTools: %s (db errcode=%d, errmsg=%s)", m, db->errcode(), db->errmsg());
    return returnValue;
}

bool PTDBErrMsg(OVService *s, const char *m, SQLite3 *db, bool returnValue) {
    s->notify(m);
    murmur("OVKPPhraseTools: %s (db errcode=%d, errmsg=%s)", m, db->errcode(), db->errmsg());
    return returnValue;

}

const char *PTMsg(const char *lc, const char *en, const char *tw, 
    const char *cn) 
{
    if (!strcasecmp(lc, "zh_TW")) return tw ? tw : en;
    if (!strcasecmp(lc, "zh_CN")) return cn ? cn : en;
    return en;
}

const char *PTMsg(OVService *s, const char *en, const char *tw, const char *cn)
{
    return PTMsg(s->locale(), en, tw, cn);
}

void PTModeOnMsg(OVService *s, const string& rstr) {	
	string msgString = PTMsg(s, "Phrase management mode", //"詞彙管理工具", "词汇管理工具");
		"\xE8\xA9\x9E\xE5\xBD\x99\xE7\xAE\xA1\xE7\x90\x86\xE5\xB7\xA5\xE5\x85\xB7", 
        "\xE8\xAF\x8D\xE6\xB1\x87\xE7\xAE\xA1\xE7\x90\x86\xE5\xB7\xA5\xE5\x85\xB7");
	if(rstr.length() > 0) {
		msgString += PTMsg(s, ". Current phrase: ", \
			//"，目前詞彙: ", "，当前词汇: ");
			"\xEF\xBC\x8C\xE7\x9B\xAE\xE5\x89\x8D\xE8\xA9\x9E\xE5\xBD\x99\x3A ", "\xEF\xBC\x8C\xE5\xBD\x93\xE5\x89\x8D\xE8\xAF\x8D\xE6\xB1\x87\x3A ");
		msgString += rstr;
	}	
	s->notify(msgString.c_str());
}

void PTRecordingMessage(const string& rstr, OVService *s) {
    string msgString = PTMsg(s, "Recording: ", //"記錄中：", "记录中：");
		"\xE8\xA8\x98\xE9\x8C\x84\xE4\xB8\xAD\xEF\xBC\x9A", "\xE8\xAE\xB0\xE5\xBD\x95\xE4\xB8\xAD\xEF\xBC\x9A");
    msgString += rstr;
    s->notify(msgString.c_str());
}

string PTTime(){
    time_t tt;
	time(&tt);

	struct tm *tp;
	tp=gmtime(&tt);

	char buf[256];
	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
    return string(buf);
}


// Because the current architecture of OV loader does not allow
// attached module instance creation (e.g. getting an instance of
// OVInputMethod also gets you an OVOutputFilter that comes with it),
// we'll have to make do by putting control flag, working string etc.
// into the shared heap of the module library (the .dylib/.dll) instead
// of putting those information in class instances. It's ok if each
// GUI application loads an instance of OV loader, hence also an instance
// of the .dylib/.dll. It'll be more interesting if there is only
// one instance of loader (ie. loader server)... we have to be careful.

bool ptRecordingMode=false;
PTKeySequence ptRecSeq;
SQLite3 *ptDB=NULL;

extern "C" int OVInitializeLibrary(OVService *s, const char *mp) {
    const char *ps=s->pathSeparator();
    const char *up=s->userSpacePath("OVKPPhraseTools");
    
    char *pn=sqlite3_mprintf("%s%s%s", up, ps, PT_DBNAME);
    string dn(pn);
#ifndef WIN32
    free(pn);
#endif 
    
    murmur("OVKPPhraseTools: opening database %s", dn.c_str());
    
    try {
        ptDB=new SQLite3;
        if (!ptDB) {
            murmur("memory allocation error");
            return false;
        }

        if (ptDB->open(dn.c_str()) != SQLITE_OK) throw "error opening database";
        
        ptDB->execute(PT_DBTBLCREATE);
        ptDB->execute(PT_DBIDXCREATE);
    }
    catch (const char *m) {
        if (ptDB) {
            PTDBErrMsg(m, ptDB);
            delete ptDB;
        }
        ptDB=NULL;
        return false;
    }
    return true;
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
    switch(idx) {
        case 0:
            return new OVKPPhraseTools(ptRecordingMode, ptRecSeq, 
                ptDB);
        case 1:
            return new OVOFTextRecorder(ptRecordingMode, ptRecSeq);
    }    

    return NULL;
}

extern "C" unsigned int OVGetLibraryVersion() {
    return OV_VERSION;
}

