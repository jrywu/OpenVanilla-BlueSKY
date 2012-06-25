//////////////////////////////////////////////////////////////////////
//
// Derived from Microsoft TSF sample by Jeremy '12,6,25
//
//  CandidateList.h
//
//          CCandidateList declaration.
//
//////////////////////////////////////////////////////////////////////

#ifndef CANDIDATELIST_H
#define CANDIDATELIST_H

class CCandidateWindow;

class CCandidateList : public ITfContextKeyEventSink,
                       public ITfTextLayoutSink
{
public:
    CCandidateList(CTextService *pTextService);
    ~CCandidateList();

  //
  // IUnknown methods
  //
    STDMETHODIMP QueryInterface(REFIID riid, void **ppvObj);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

  //
  // ITfContextKeyEventSink
  //
    STDMETHODIMP OnKeyDown(WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
    STDMETHODIMP OnKeyUp(WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
    STDMETHODIMP OnTestKeyDown(WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
    STDMETHODIMP OnTestKeyUp(WPARAM wParam, LPARAM lParam, BOOL *pfEaten);

  //
  // ITfTextLayoutSink
  //
    STDMETHODIMP OnLayoutChange(ITfContext *pContext, TfLayoutCode lcode, ITfContextView *pContextView);

    HRESULT _StartCandidateList(TfClientId tid, ITfDocumentMgr *pDocumentMgr, ITfContext *pContextDocument, TfEditCookie ec, ITfRange *pRangeComposition);
    void _EndCandidateList();

    BOOL _IsContextCandidateWindow(ITfContext *pContext);

private:
    HRESULT _AdviseContextKeyEventSink();
    HRESULT _UnadviseContextKeyEventSink();
    HRESULT _AdviseTextLayoutSink();
    HRESULT _UnadviseTextLayoutSink();

    CTextService *_pTextService;
    ITfRange *_pRangeComposition;
    ITfContext *_pContextCandidateWindow;
    ITfContext *_pContextDocument;
    ITfDocumentMgr *_pDocumentMgr;

    DWORD _dwCookieContextKeyEventSink; // Cookie for ITfContextKeyEventSink
    DWORD _dwCookieTextLayoutSink; // Cookie for ITfContextKeyEventSink

    HWND _hwndParent;
    CCandidateWindow *_pCandidateWindow;

    LONG _cRef; // COM ref count
};

#endif // CANDIDATELIST_H
