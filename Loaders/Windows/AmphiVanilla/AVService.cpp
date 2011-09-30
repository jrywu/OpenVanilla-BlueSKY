//#define OV_DEBUG

#ifndef WIN32
#include <OpenVanilla/OVUtility.h>
#include <OpenVanilla/OVUTF8Helper.h>
#include <OpenVanilla/OVWildcard.h>
#else
#include "OVWildcard.h"
#include "OVUtility.h"
#include "OVUTF8Helper.h"
#endif
#include "AVService.h"
#include "AVDisplayServer.h"
#include "OVUTF8Helper.h"

//<comment author='b6s'>Disable iconv temporarily.
//#include "iconv.h"
//</comment>
#include <cstring>

#include <string>
#include <io.h>
#include <windows.h>	

using namespace OpenVanilla;


AVService::AVService() {} 
AVService::AVService() {} 

AVService::AVService(const char* dir) : userdir(dir),  _UTF16ShortBuffer(0)
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


const char *AVService::toUTF8(const char *encoding, const char *src)
{
#ifdef WIN32
	if (OVWildcard::Match(encoding, "big5*") || OVWildcard::Match(encoding, "big-5")) {
		WCHAR wbuf[256];
		MultiByteToWideChar(950, 0, src, -1 , wbuf, 256);
		_UTF8Buffer = OVUTF8::FromUTF16(wbuf);
		return _UTF8Buffer.c_str();
	}
	
	return src;
#elif _APPLE
	if (OVWildcard::Match(encoding, "big5*") || OVWildcard::Match(encoding, "big-5")) {
		CFStringRef convStr = CFStringCreateWithBytes(NULL, (const UInt8 *)src, (CFIndex)strlen(src), kCFStringEncodingBig5_HKSCS_1999, false);
		if (convStr) {
			[_conversionBuffer setString:(NSString *)convStr];
			CFRelease(convStr);
			return [_conversionBuffer UTF8String];
		}
	}
		
    return src;

#else //UNIX variant
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
#endif

}


const char *AVService::fromUTF8(const char *encoding, const char *src)
{
#ifdef WIN32
	if (OVWildcard::Match(encoding, "big5*") || OVWildcard::Match(encoding, "big-5")) {
		_UTF16Buffer = OVUTF16::FromUTF8(src);
		char buf[256];
		WideCharToMultiByte(950,0, _UTF16Buffer.c_str(), -1, buf,256,NULL,NULL);
		return buf;		
	}
	return src;

#elif _APPLE
	if (OVWildcard::Match(encoding, "big5*") || OVWildcard::Match(encoding, "big-5")) {
		[_conversionBuffer setString:[NSString stringWithUTF8String:src]];
		return [_conversionBuffer cStringUsingEncoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingBig5_HKSCS_1999)];
	}
    return src;

#else
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
#endif
}


const char *AVService::UTF16ToUTF8(unsigned short *src, int len)
{
	murmur("AVService::UTF16ToUTF8(), len = %d", len);

	wstring tmp(len, 0);
		for (int i = 0 ; i < len ; i++) {
			tmp[i] = src[i];
		}
		
		_UTF8Buffer = OVUTF8::FromUTF16(tmp);
		return _UTF8Buffer.c_str();
	
}

enum { bit7=0x80, bit6=0x40, bit5=0x20, bit4=0x10, bit3=8, bit2=4, bit1=2, bit0=1 };

int AVService::UTF8ToUTF16(const char *src, unsigned short **rcvr)
		
{
	murmur("AVService::UTF8ToUTF16()");

	_UTF16Buffer = OVUTF16::FromUTF8(src);
		
		_UTF16ShortBuffer = (unsigned short *)realloc(_UTF16ShortBuffer, (_UTF16Buffer.length() + 1) * sizeof(unsigned short));
		size_t i;
		for (i = 0 ; i < _UTF16Buffer.length() ; i++) {
			_UTF16ShortBuffer[i] = _UTF16Buffer[i];
		}
		
		_UTF16ShortBuffer[i] = 0;
		
		*rcvr = _UTF16ShortBuffer;
		return _UTF16Buffer.length();
	
	
}
