//////////////////////////////////////////////////////////////////////
//
// Derived from Microsoft TSF sample by Jeremy '12,6,25
//
//  Globals.cpp
//
//          Global variables
//
//////////////////////////////////////////////////////////////////////

#include "globals.h"

HINSTANCE g_hInst;

LONG g_cRefDll = -1; // -1 /w no refs, for win95 InterlockedIncrement/Decrement compat

CRITICAL_SECTION g_cs;


// {D20EE398-268D-48EA-A1B3-9827D3520B3E}
static const GUID c_clsidTextService = 
{ 0xd20ee398, 0x268d, 0x48ea, { 0xa1, 0xb3, 0x98, 0x27, 0xd3, 0x52, 0xb, 0x3e } };

// {628207E1-5E21-4653-AC7B-6E9B70FA19AC}
static const GUID c_guidProfile = 
{ 0x628207e1, 0x5e21, 0x4653, { 0xac, 0x7b, 0x6e, 0x9b, 0x70, 0xfa, 0x19, 0xac } };

// {85CF40AB-F6E0-470E-969E-BB4C99505F37}
static const GUID c_guidLangBarItemButton = 
{ 0x85cf40ab, 0xf6e0, 0x470e, { 0x96, 0x9e, 0xbb, 0x4c, 0x99, 0x50, 0x5f, 0x37 } };

//
//  define two guids for display attribute info. This textservice has
//  two display attribute. One is for input text and the other is for the
//  converted text.
//
//      c_guidDisplayAttributeInput 
//      c_guidDisplayAttributeConverted
//

// {1DA200C1-F344-45DA-B46B-0155E49E0B73}
const GUID c_guidDisplayAttributeInput = { 
 0x1da200c1, 0xf344, 0x45da, { 0xb4, 0x6b, 0x1, 0x55, 0xe4, 0x9e, 0xb, 0x73 } };
/* 4e1aa3fe-6c7f-11d7-a6ec-00065b84435c 
    0x4e1aa3fe,
    0x6c7f,
    0x11d7,
    {0xa6, 0xec, 0x00, 0x06, 0x5b, 0x84, 0x43, 0x5c}
  };*/


// {EAA18A73-4D3B-44A6-B624-B74DE871447A}
const GUID c_guidDisplayAttributeConverted = { 
 0xeaa18a73, 0x4d3b, 0x44a6, { 0xb6, 0x24, 0xb7, 0x4d, 0xe8, 0x71, 0x44, 0x7a } };
/* 4e1aa3ff-6c7f-11d7-a6ec-00065b84435c *
    0x4e1aa3ff,
    0x6c7f,
    0x11d7,
    {0xa6, 0xec, 0x00, 0x06, 0x5b, 0x84, 0x43, 0x5c}
  };
  */
