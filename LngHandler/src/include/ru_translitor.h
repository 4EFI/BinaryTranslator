#ifndef RU_TRANSLITOR_H
#define RU_TRANSLITOR_H

enum RussianSymb // Thanks @aleksplast for that very much
{
    ARUS     = -80,
    BRUS     = -79,
    VRUS     = -78,
    GRUS     = -77,
    DRUS     = -76,
    ERUS     = -75,
    ZHRUS    = -74,
    ZRUS     = -73,
    IRUS     = -72,
    YIRUS    = -71,
    KRUS     = -70,
    LRUS     = -69,
    MRUS     = -68,
    NRUS     = -67,
    ORUS     = -66,
    PRUS     = -65,
    RRUS     = -128,
    SRUS     = -127,
    TRUS     = -126,
    URUS     = -125,
    FRUS     = -124,
    HRUS     = -123,
    TSRUS    = -122,
    CHRUS    = -121,
    SHRUS    = -120,
    SHCHRUS  = -119,
    IERUS    = -118, 
    YRUS     = -117,
    QRUS     = -116,
    EERUS    = -115,
    IURUS    = -114,
    IARUS    = -113,
    ABRUS    = -112,
    BBRUS    = -111,
    VBRUS    = -110,
    GBRUS    = -109,
    DBRUS    = -108,
    EBRUS    = -107,
    ZHBRUS   = -106,
    ZBRUS    = -105,
    IBRUS    = -104,
    YIBRUS   = -103,
    KBRUS    = -102,
    LBRUS    = -101,
    MBRUS    = -100,
    NBRUS    = -99,
    OBRUS    = -98,
    PBRUS    = -97,
    RBRUS    = -96,
    SBRUS    = -95,
    TBRUS    = -94,
    UBRUS    = -93,
    FBRUS    = -92,
    HBRUS    = -91,
    TSBRUS   = -90,
    CHBRUS   = -89,
    SHBRUS   = -88,
    SHCHBRUS = -87,
    IEBRUS   = -86,
    YBRUS    = -85,
    QBRUS    = -84,
    EEBRUS   = -83,
    IUBRUS   = -82,
    IABRUS   = -81,
    USELESS1 = -47,
    USELESS2 = -48,
};

struct RuSymPair
{
    int  ruSym;
    char enSym;  
};

char* TranslitString( const char* src, int size );

#endif