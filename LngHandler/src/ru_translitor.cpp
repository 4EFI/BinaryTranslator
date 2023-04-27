
#include "ru_translitor.h"

#include <stdio.h>
#include <stdlib.h>

char* TranslitString( const char* src, int size )
{
    char* translit  = ( char* )calloc( size*4, sizeof( char ) );
    char* begin_ptr = translit;

    while( *src != '\0' )
    {
        switch( *src )
        {
            case USELESS2:
                break;
            case USELESS1:
                break;
            case ARUS:
                *translit = 'a';
                translit++;
                break;
            case BRUS:
                *translit = 'b';
                translit++;
                break;
            case VRUS:
                *translit = 'v';
                translit++;
                break;
            case GRUS:
                *translit = 'g';
                translit++;
                break;
            case DRUS:
                *translit = 'd';
                translit++;
                break;
            case ERUS:
                *translit = 'e';
                translit++;
                break;
            case ZHRUS:
                *translit = 'z';
                translit++;
                *translit = 'h';
                translit++;
                break;
            case ZRUS:
                *translit = 'z';
                translit++;
                break;
            case IRUS:
                *translit = 'i';
                translit++;
                break;
            case YIRUS:
                *translit = 'i';
                translit++;
                break;
            case KRUS:
                *translit = 'k';
                translit++;
                break;
            case LRUS:
                *translit = 'l';
                translit++;
                break;
            case MRUS:
                *translit = 'm';
                translit++;
                break;
            case NRUS:
                *translit = 'n';
                translit++;
                break;
            case ORUS:
                *translit = 'o';
                translit++;
                break;
            case PRUS:
                *translit = 'p';
                translit++;
                break;
            case RRUS:
                *translit = 'r';
                translit++;
                break;
            case SRUS:
                *translit = 's';
                translit++;
                break;
            case TRUS:
                *translit = 't';
                translit++;
                break;
            case URUS:
                *translit = 'u';
                translit++;
                break;
            case FRUS:
                *translit = 'f';
                translit++;
                break;
            case HRUS:
                *translit = 'k';
                translit++;
                *translit = 'h';
                translit++;
                break;
            case TSRUS:
                *translit = 't';
                translit++;
                *translit = 's';
                translit++;
                break;
            case CHRUS:
                *translit = 'c';
                translit++;
                *translit = 'h';
                translit++;
                break;
            case SHRUS:
                *translit = 's';
                translit++;
                *translit = 'h';
                translit++;
                break;
            case SHCHRUS:
                *translit = 's';
                translit++;
                *translit = 'h';
                translit++;
                *translit = 'c';
                translit++;
                *translit = 'h';
                translit++;
                break;
            case IERUS:
                *translit = 'i';
                translit++;
                *translit = 'e';
                translit++;
                break;
            case YRUS:
                *translit = 'y';
                translit++;
                break;
            case QRUS:
                *translit = '\'';
                translit++;
                break;
            case EERUS:
                *translit = 'e';
                translit++;
                break;
            case IURUS:
                *translit = 'i';
                translit++;
                *translit = 'u';
                translit++;
                break;
            case IARUS:
                *translit = 'i';
                translit++;
                *translit = 'a';
                translit++;
                break;
            case ABRUS:
                *translit = 'A';
                translit++;
                break;
            case BBRUS:
                *translit = 'B';
                translit++;
                break;
            case VBRUS:
                *translit = 'V';
                translit++;
                break;
            case GBRUS:
                *translit = 'G';
                translit++;
                break;
            case DBRUS:
                *translit = 'D';
                translit++;
                break;
            case EBRUS:
                *translit = 'E';
                translit++;
                break;
            case ZHBRUS:
                *translit = 'Z';
                translit++;
                *translit = 'H';
                translit++;
                break;
            case ZBRUS:
                *translit = 'Z';
                translit++;
                break;
            case IBRUS:
                *translit = 'I';
                translit++;
                break;
            case YIBRUS:
                *translit = 'I';
                translit++;
                break;
            case KBRUS:
                *translit = 'K';
                translit++;
                break;
            case LBRUS:
                *translit = 'L';
                translit++;
                break;
            case MBRUS:
                *translit = 'M';
                translit++;
                break;
            case NBRUS:
                *translit = 'N';
                translit++;
                break;
            case OBRUS:
                *translit = 'O';
                translit++;
                break;
            case PBRUS:
                *translit = 'P';
                translit++;
                break;
            case RBRUS:
                *translit = 'R';
                translit++;
                break;
            case SBRUS:
                *translit = 'S';
                translit++;
                break;
            case TBRUS:
                *translit = 'T';
                translit++;
                break;
            case UBRUS:
                *translit = 'U';
                translit++;
                break;
            case FBRUS:
                *translit = 'F';
                translit++;
                break;
            case HBRUS:
                *translit = 'K';
                translit++;
                *translit = 'H';
                translit++;
                break;
            case TSBRUS:
                *translit = 'T';
                translit++;
                *translit = 'S';
                translit++;
                break;
            case CHBRUS:
                *translit = 'C';
                translit++;
                *translit = 'H';
                translit++;
                break;
            case SHBRUS:
                *translit = 'S';
                translit++;
                *translit = 'H';
                translit++;
                break;
            case SHCHBRUS:
                *translit = 'S';
                translit++;
                *translit = 'H';
                translit++;
                *translit = 'C';
                translit++;
                *translit = 'H';
                translit++;
                break;
            case IEBRUS:
                *translit = 'I';
                translit++;
                *translit = 'E';
                translit++;
                break;
            case YBRUS:
                *translit = 'Y';
                translit++;
                break;
            case QBRUS:
                *translit = '\'';
                translit++;
                break;
            case EEBRUS:
                *translit = 'E';
                translit++;
                break;
            case IUBRUS:
                *translit = 'I';
                translit++;
                *translit = 'U';
                translit++;
                break;
            case IABRUS:
                *translit = 'I';
                translit++;
                *translit = 'A';
                translit++;
                break;
            default:
                *translit = *src;
                translit++;
                break;
        }
        
        src++;
    }

    *translit = '\0';

    return begin_ptr;
}