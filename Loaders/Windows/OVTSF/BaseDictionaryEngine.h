//
//
// Derived from Microsoft Sample IME by Jeremy '13,7,17
//
//



#pragma once

#include "File.h"
#include "OVTSFBaseStructure.h"

class CBaseDictionaryEngine
{
public:
    CBaseDictionaryEngine(LCID locale, _In_ CFile *pDictionaryFile);
    virtual ~CBaseDictionaryEngine();

    virtual VOID CollectWord(_In_ CStringRange *psrgKeyCode, _Out_ COVTSFArray<CStringRange> *pasrgWordString)
    { 
        psrgKeyCode; 
        pasrgWordString = nullptr;
    }

    virtual VOID CollectWord(_In_ CStringRange *psrgKeyCode, _Out_ COVTSFArray<CCandidateListItem> *pItemList)
    { 
        psrgKeyCode;
        pItemList = nullptr;
    }

    virtual VOID SortListItemByFindKeyCode(_Inout_ COVTSFArray<CCandidateListItem> *pItemList);

protected:
    CFile* _pDictionaryFile;
    LCID _locale;

private:
    VOID MergeSortByFindKeyCode(_Inout_ COVTSFArray<CCandidateListItem> *pItemList, int leftRange, int rightRange);
    int CalculateCandidateCount(int leftRange,  int rightRange);
};
