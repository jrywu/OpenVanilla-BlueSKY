//#define OV_DEBUG
#include "OVUtility.h"
#include "AVService.h"
#include "AVDisplayServer.h"
#include "OVUTF8Helper.h"

//<comment author='b6s'>Disable iconv temporarily.
//#include "iconv.h"
//</comment>
#include <cstring>
#include <io.h>
#include <tchar.h>
#include <windows.h>	//< For Beep(DWORD, DWORD);

using namespace OpenVanilla;

AVService::AVService() {} 

AVService::AVService(const char* dir) : userdir(dir) 
{
	LCID ulcid=GetUserDefaultLCID();
	murmur("::AVService();LCID:%x",ulcid);
	localname = string("en");
	if(ulcid ==0x0404 ||ulcid ==0x0c04L || ulcid==0x1404  )  localname = string("zh_TW");// TW, HK, MACAU
	if(ulcid ==0x0804 ||ulcid ==0x1004  )  localname = string("zh_CN"); // CN, Singapore
	
	murmur("Current user locale:%s", localname.c_str());
	
	_UTF16ShortBuffer = (unsigned short *)malloc(32 * sizeof(unsigned short));
}

const char *AVService::userSpacePath(const char *modid)
{
	returnuserdir.clear();
	returnuserdir.append(userdir.c_str()).append(modid).append(this->pathSeparator()).c_str();
	if (_access(returnuserdir.c_str(), 0))
			CreateDirectoryA(returnuserdir.c_str(), NULL);
	return returnuserdir.c_str();
}
const char *AVService::locale()
{	
	return localname.c_str();

}

const char *AVService::pathSeparator()
{
#ifdef WIN32
	return "\\"; 
#else
	return "/";
#endif
}

void AVService::beep()
{
	MessageBeep(-1); 
} 

void AVService::notify(const char *msg)

{
	dsvr->showNotify(msg);
}

//<comment author='b6s'>Disable iconv temporarily.
const char *AVService::toUTF8(const char *encoding, const char *src)
{
	return 0;
}
/*
const char *AVService::toUTF8(const char *encoding, const char *src)
{
	char *out = NULL;
	size_t inbytesleft = strlen(src) + 1;
	size_t outbytesleft = 1024;
	iconv_t cd;
	memset(internal, 0, 1024);
	out = internal;
	cd = iconv_open("UTF-8", encoding);
	iconv (cd, &src, &inbytesleft, &out, &outbytesleft);
	iconv_close(cd);
	return internal;
}
*/
//</comment>

//<comment author='b6s'>Disable iconv temporarily.
const char *AVService::fromUTF8(const char *encoding, const char *src)
{
	return 0;
}
/*
const char *AVService::fromUTF8(const char *encoding, const char *src)
{
	char *out = NULL;
	size_t inbytesleft = strlen(src) + 1;
	size_t outbytesleft = 1024;
	iconv_t cd;
	memset(internal, 0, 1024);
	out = internal;
	cd = iconv_open(encoding, "UTF-8");
	iconv (cd, &src, &inbytesleft, &out, &outbytesleft);
	iconv_close(cd);
	return internal;
}
*/
//</comment>

const char *AVService::UTF16ToUTF8(unsigned short *s, int l)
{


	    char *b =(char *) malloc(sizeof(char) * l * 4);
		char *result;
		result = b;
	    for (int i=0; i<l; i++)
	    {
		    if (s[i] < 0x80)
		    {
			    *b++=static_cast<char>(s[i]);
		    }
		    else if (s[i] < 0x800)
		    {
			    *b++=(0xc0 | s[i]>>6);
			    *b++=(0x80 | s[i] & 0x3f);
		    }
		    else if (s[i] < 0xd800 || s[i] > 0xdbff)
		    {
			    *b++ = (0xe0 | s[i]>>12);
			    *b++ = (0x80 | s[i]>>6 & 0x3f);
			    *b++ = (0x80 | s[i] & 0x3f);

		    }
		    else
		    {
			    unsigned int offset= 0x10000 - (0xd800 << 10) - 0xdc00;
			    unsigned int codepoint=(s[i] << 10) + s[i+1]+offset;
			    i++;
			    *b++=(0xf0 | codepoint>>18);
			    *b++=(0x80 | codepoint>>12 & 0x3f);
			    *b++=(0x80 | codepoint>>6 & 0x3f);
			    *b++=(0x80 | codepoint & 0x3F);
		    }
	    }

	    *b=0;
	    return result;
		//return internal;
		
}

enum { bit7=0x80, bit6=0x40, bit5=0x20, bit4=0x10, bit3=8, bit2=4, bit1=2, bit0=1 };

int AVService::UTF8ToUTF16(const char *src, unsigned short **rcvr)
{

	
	char *s1=(char*)src;
	int len=0;
	char a, b, c;
	while (*s1)
	{
		a=*s1++;
		if ((a & (bit7|bit6|bit5))==(bit7|bit6)) { // 0x000080-0x0007ff
			b=*s1++;

			u_internal[len] = ((a & (bit4|bit3|bit2)) >> 2) * 0x100;
			u_internal[len] += (((a & (bit1|bit0)) << 6) | (b & (bit5|bit4|bit3|bit2|bit1|bit0)));
		}
		else if ((a & (bit7|bit6|bit5|bit4))==(bit7|bit6|bit5)) // 0x000800-0x00ffff
		{
			b=*s1++;
			c=*s1++;

			u_internal[len] = (((a & (bit3|bit2|bit1|bit0)) << 4) | ((b & (bit5|bit4|bit3|bit2)) >> 2)) * 0x100;
			u_internal[len] += (((b & (bit1|bit0)) << 6) | (c & (bit5|bit4|bit3|bit2|bit1|bit0)));
		}
		else 
		{
			u_internal[len] =(0);
			u_internal[len] +=(a);
		}
		len++;
	}
	*rcvr = u_internal;
	return len;
	
}
