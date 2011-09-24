// OVCandidateList.cpp: A simple candidate list class
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

#include "OVCandidateList.h"
#include "OVCIN.h"
#include <iostream>

#ifdef __linux__
	#include <string.h>
#endif

using namespace std;

void OVCandidateList::prepare(vector<string>* l, char* skey, OVCandidate *textbar)
{
    onduty=true;
    strcpy(selkey, skey);
    perpage=static_cast<int>(strlen(selkey));
    pos=0;
	list = l;	
    count=static_cast<int>(list->size());
        
    update(textbar);
    textbar->show();
}

void OVCandidateList::update(OVCandidate *textbar)
{
    char buf[256];
    int bound=pos+perpage;
    if (bound > count) bound=count;
    
    textbar->clear();
    
    for (int i=pos, j=0; i<bound; i++, j++)
    {
        sprintf (buf, "%c.", selkey[j]);
        textbar->append(buf)->
			append(const_cast<char*>(list->at(i).c_str()))->
            append(" ");
    }
    
    int totalpage=(count % perpage) ? (count/perpage)+1 : (count/perpage);
    int currentpage=(pos/perpage)+1;
//  if (totalpage >1)
//  {
        sprintf (buf, "(%d/%d)", currentpage, totalpage);
        textbar->append(buf);
//  }
    textbar->update();   
}

OVCandidateList* OVCandidateList::pageUp()
{
    pos-=perpage;
    if (pos < 0) pos=0;
    return this;
}

OVCandidateList* OVCandidateList::pageDown()
{
    pos+=perpage;  
    if (pos >= count) pos=0;
    return this;
}

bool OVCandidateList::select(char inKey, string& outStringRef)
{
    int i;
    for (i=0; i<perpage; i++)
	{
        if (selkey[i]==inKey && (pos+i < count)) {
            onduty=false;
            outStringRef = list->at(pos+i);
			return true;
        }
	}
	
    return false;
}
