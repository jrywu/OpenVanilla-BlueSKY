#ifndef IMMMODEL_H
#define IMMMODEL_H

#include <windows.h>
#ifdef WINCE
#include <imm.h>
#else
#include <immdev.h>
#endif

#include "ExtraStructs.h"

class ImmModel
{
public:
	static ImmModel* open(HIMC h);
	static void close(void);

	LPINPUTCONTEXT getIMC(void);
	LPCOMPOSITIONSTRING getCompStr(void);
	LPMYPRIVATE getMyPrivate(void);
	LPCANDIDATEINFO getCandInfo(void);

protected:
	ImmModel(HIMC h);
	~ImmModel(void);

private:
	static ImmModel* m_self;

	static HIMC m_hIMC;

	static LPINPUTCONTEXT m_lpIMC;
	static LPCOMPOSITIONSTRING m_lpCompStr;
	static LPMYPRIVATE m_lpMyPrivate;
	static LPCANDIDATEINFO m_lpCandInfo;

	LPINPUTCONTEXT lockIMC(void);
	LPCOMPOSITIONSTRING lockCompStr(void);
	LPMYPRIVATE lockMyPrivate(void);
	LPCANDIDATEINFO lockCandInfo(void);

	void unlockIMC(void);
	void unlockCompStr(void);
	void unlockMyPrivate(void);
	void unlockCandInfo(void);
};

#endif
