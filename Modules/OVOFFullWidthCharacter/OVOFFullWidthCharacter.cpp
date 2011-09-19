// OVOFFullWidthCharacter.cpp: Full-width character output filter
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

#ifndef WIN32
    #include <OpenVanilla/OpenVanilla.h>  
    #include <OpenVanilla/OVLibrary.h>
#else
    #include "OpenVanilla.h"
    #include "OVLibrary.h"
#endif

#include "OVOSDef.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

unsigned short fullWidthChars[95]=
{
    0x3000, 0xff01, 0x201d, 0xff03, 0xff04, 0xff05, 0xff06, 0x2019, 0xff08, 0xff09, 
    0xff0a, 0xff0b, 0xff0c, 0xff0d, 0xff0e, 0xff0f, 0xff10, 0xff11, 0xff12, 0xff13, 
    0xff14, 0xff15, 0xff16, 0xff17, 0xff18, 0xff19, 0xff1a, 0xff1b, 0xff1c, 0xff1d, 
    0xff1e, 0xff1f, 0xff20, 0xff21, 0xff22, 0xff23, 0xff24, 0xff25, 0xff26, 0xff27, 
    0xff28, 0xff29, 0xff2a, 0xff2b, 0xff2c, 0xff2d, 0xff2e, 0xff2f, 0xff30, 0xff31, 
    0xff32, 0xff33, 0xff34, 0xff35, 0xff36, 0xff37, 0xff38, 0xff39, 0xff3a, 0x3014, 
    0xff3c, 0x3015, 0xff3e, 0xff3f, 0x2018, 0xff41, 0xff42, 0xff43, 0xff44, 0xff45, 
    0xff46, 0xff47, 0xff48, 0xff49, 0xff4a, 0xff4b, 0xff4c, 0xff4d, 0xff4e, 0xff4f, 
    0xff50, 0xff51, 0xff52, 0xff53, 0xff54, 0xff55, 0xff56, 0xff57, 0xff58, 0xff59, 
    0xff5a, 0xff5b, 0xff5c, 0xff5d, 0xff5e
};

char *halfWidthChars=" !\"#$%&'()*+,-./0123456789:;<=>?"
                     "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
                     "abcdefghijklmnopqrstuvwxyz{|}~";

class OVOFFullWidthCharacter : public OVOutputFilter
{
public:
    OVOFFullWidthCharacter() { u16buf=NULL; }
    int initialize(OVDictionary *cfg, OVService *srv, const char *modpath) {
        // fprintf(stderr, "OVOFFullWidthCharacter being initialized, module path=%s\n", modpath);
        return 1;
    }
    const char *identifier() { return "OVOFFullWidthCharacter"; }
    virtual const char *localizedName(const char *locale);
    virtual const char *process (const char *src, OVService *srv);
protected:
	unsigned short *u16buf;
};
                       
const char *OVOFFullWidthCharacter::localizedName(const char *locale)
{
    if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh-Hant"))
		return "\xE5\x85\xA8\xE5\xBD\xA2\xE8\x8B\xB1\xE6\x95\xB8\xE5\xAD\x97";//"?�形?�數�?;
    if (!strcasecmp(locale, "zh_CN") || !strcasecmp(locale, "zh-Hans"))
		return "\xE5\x85\xA8\xE8\xA7\x92\xE8\x8B\xB1\xE6\x95\xB8\xE5\xAD\x97";//"?��??�数�?;
    return "Full-Width ASCII Character";
}

const char *OVOFFullWidthCharacter::process(const char *src, OVService *srv)
{
    if (u16buf) {
        free(u16buf);
        u16buf=NULL;
    }
    
    unsigned short *u16p;
    int l=srv->UTF8ToUTF16(src, &u16p);
    
    if (!l) return src;
    u16buf=(unsigned short*)calloc(1,l*sizeof(unsigned short));
    memcpy(u16buf, u16p, l*sizeof(unsigned short));
	
	for (int i=0; i<l; i++)
    {
        if (u16buf[i] > 0x7f) continue; 
        char *p=strchr(halfWidthChars, (char)u16buf[i]);
        if (p) u16buf[i]=fullWidthChars[(int)(p-halfWidthChars)];
    }
	return srv->UTF16ToUTF8(u16buf, l);  
}

OV_SINGLE_MODULE_WRAPPER(OVOFFullWidthCharacter);
