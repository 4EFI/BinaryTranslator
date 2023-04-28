
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "FileAlgorithms.h"
#include "StrAlgorithms.h"
#include "LOG.h"
#include "dsl.h"

#define _STR( str ) #str
#define  STR( str ) _STR( str )

//-----------------------------------------------------------------------------

int AsmCtor( ASM* asm_s )
{
    if( asm_s == NULL ) return 0;
    
    TextInit( &asm_s->text ); 

    // Fill all with zero
    memset( asm_s->labels, 0, NumLabels * sizeof( Label ) );

    asm_s->codeSize = 0;
    asm_s->code     = NULL;

    return 1;
}

//-----------------------------------------------------------------------------

int AsmGetCmds( ASM* asm_s, FILE* fileIn )
{
    if( asm_s == NULL || fileIn == NULL ) return 0;
    
    long numStrs    = TextSetFileLines( &asm_s->text, fileIn ); 
    asm_s->codeSize = 2 * numStrs * sizeof( Elem_t ); 

    asm_s->code = (char*)calloc( asm_s->codeSize, sizeof( char ) );

    return 1;
}

//-----------------------------------------------------------------------------

int FillListingFile( FILE* listing_file, const char* str, const char* code, CMD* cmd, int ip )
{
    if( !listing_file ) return 0;
        
    fprintf( listing_file, "%010X | %02X ", ip++, *cmd );

    int num_skips = 0;
    
    if( cmd->immed )
    {    
        for( int i = 0; i < sizeof( Elem_t ); i++ )
        {
            fprintf( listing_file, "%02X", ( u_char )code[ip + i] );
        }
            
        ip += sizeof( Elem_t );         

        fprintf( listing_file, " " );      
    }
    else num_skips += ( 2 * sizeof( Elem_t ) + 1 );
    

    if( cmd->reg )
    {
        fprintf( listing_file, "%02X ", code[ip] ); ip++;               
    }
    else num_skips += 3;
    

    fprintf( listing_file, "%*s %s\n", num_skips + 10, " ", str );
 
    return 1;
}

int AsmMakeArrCmds( ASM* asm_s, FILE* listing_file )
{   
    if( asm_s == NULL ) return 0;
    
    int ip = 0;
    
    for( int i = 0; i < asm_s->text.numLines; i++ )
    {        
        char* strForRead = asm_s->text.lines[i].str;

        // Check label
        int      isLabel = AsmLabelHandler( asm_s, strForRead, ip ); 
        if/* */( isLabel ==  1 ) continue;
        else if( isLabel == -1 ) return -1;
        
    
        // Get command
        char cmdName[MaxStrLen] = "";

        int numReadSyms = 0;
        sscanf( strForRead, " %" STR(MaxStrLen) "s%n", cmdName, &numReadSyms ); // Get command name

        if( numReadSyms == 0 ) continue;

        // Command handler
        CMD* cmd = (CMD*)(&asm_s->code[ip]);
        
        int old_ip = ip;
        AsmArgHandler( asm_s, strForRead + numReadSyms /*Str for read from*/, &ip );

        // Compare commands
        #define DEF_CMD( NAME, NUM, ... )                                                   \
            if( strcasecmp( cmdName, #NAME ) == 0 )                                         \
            {                                                                               \
                cmd->code = NUM;                                                            \
                FillListingFile( listing_file, strForRead, asm_s->code, cmd, old_ip );      \
            }                                                                               \
            else

        #include "commands.h"
        /*else*/
        {
            printf( "Command \"%s\" does not exist...\n", cmdName );
            return 0;
        }

        #undef DEF_CMD
    }

    asm_s->codeSize = ip;

    return 1;
}

//-----------------------------------------------------------------------------

int AsmLabelHandler( ASM* asm_s, const char* strForRead, int ip )
{
    if( asm_s == NULL || strForRead == NULL ) return 0;

    // Get label
    char label[MaxStrLen] = "";
    
    int len = 0;
    sscanf( strForRead, "%s%n", label, &len );

    if( !strchr( label, ':' ) ) return 0; // Not label


    int      numLeftIngSyms = NumLeftIgnoredSyms( strForRead, 0, " \t" ); 
    len -= ( numLeftIngSyms + 1 ); // Skip left ignored symbols and ':'

    int labelPos = GetLabelIndex( asm_s->labels, NumLabels, strForRead, len );
    if( labelPos == -1 )
    {
        int emptyLabel = GetEmptyLabelIndex( asm_s->labels, NumLabels );
        if( emptyLabel == -1 ) 
        {   
            printf( "Label array overflows...\n" );
            return -1;
        }

        SetLabel( &asm_s->labels[emptyLabel], (char*)(strForRead + numLeftIngSyms) /*name*/, len, ip );
    }
    else 
    {
        asm_s->labels[labelPos].val = ip;
    }

    return 1;
}

//-----------------------------------------------------------------------------

int AsmArgJumpHandler( ASM* asm_s, const char* strForRead, int* ip )
{
    if( asm_s == NULL || strForRead == NULL ) return 0;

    char* sym = strchr( ( char* )strForRead, ':' );

    if( sym )
    {
        CMD* cmd = (CMD*)(&asm_s->code[(*ip)++]);
        
        int len = 0;
        strForRead = sym + 1; // Skip all before ':' and the ':'

        sscanf( strForRead, "%*s%n", &len );

        int pos = GetLabelIndex( asm_s->labels, NumLabels, strForRead, len );
        
        if( pos == -1 ) ASM_PUT_CODE( 0                      )
        else            ASM_PUT_CODE( asm_s->labels[pos].val )

        cmd->immed = 1;

        return 1;
    }

    return 0;
}

//-----------------------------------------------------------------------------

int AsmArgHandler( ASM* asm_s, const char* strForRead, int* ip )
{
    if( asm_s == NULL || strForRead == NULL ) return 0;

    // Jump value handler
    if( AsmArgJumpHandler( asm_s, strForRead, ip ) ) return 1;

    // Regs & values handler
    CMD* cmd = (CMD*)(&asm_s->code[(*ip)++]);
    
    Elem_t val = 0;
    char reg_i[MaxStrLen] = "";

    bool isMemTreatment = false;
    
    // check [
    char* sym = strchr( ( char* )strForRead, '[' );
    
    int numSkipSyms = 0;
    if( sym ) { isMemTreatment = true; numSkipSyms = sym - strForRead + 1; }

    if/* */( sscanf( strForRead + numSkipSyms, "%lf + %" STR(MaxStrLen) "s",    &val,    reg_i ) == 2 || 
             sscanf( strForRead + numSkipSyms, " %" STR(MaxStrLen) "[^+] + %lf", reg_i, &val   ) == 2 ) 
    /* if (value + reg_value) || (reg_value + value) */
    {
        cmd->immed = 1;
        cmd->reg   = 1;

        ASM_PUT_CODE( val )

        asm_s->code[(*ip)++] = char(GetRegIndex( reg_i ));
    }

    else if( sscanf( strForRead + numSkipSyms, "%lf", &val ) == 1 ) // if value
    {   
        cmd->immed = 1;

        ASM_PUT_CODE( val )
    }
    
    else if( sscanf( strForRead + numSkipSyms, " %" STR(MaxStrLen) "s ", reg_i ) == 1 ) // if register
    {    
        cmd->reg = 1;

        asm_s->code[(*ip)++] = char( GetRegIndex( reg_i ) );
    }

    // Check ]
    if( isMemTreatment && strchr( strForRead + numSkipSyms, ']' ) ) cmd->memory = 1; 

    return 1;
}

//-----------------------------------------------------------------------------

int GetLabelIndex( Label labels[], int numLabels, const char* str, int len )
{
    for( int i = 0; i < numLabels; i++ )
    {
        if( labels[i].name.str != NULL && strncmp( str, labels[i].name.str, len ) == 0 )
        {
            return i;
        }
    }

    return -1;  
}

//-----------------------------------------------------------------------------

int GetEmptyLabelIndex( Label labels[], int numLabels )
{
    // Find empty label
    for( int i = 0; i < numLabels; i++ )
    {
        if( labels[i].name.str == NULL ) 
        {
            return i;  
        }
    }

    return -1;
}

//-----------------------------------------------------------------------------

int SetLabel( Label* label, const char* name, int len, Elem_t val )
{
    if( label == NULL ) return 0;

    label->name.str = (char*)name;
    label->name.len = len;
    label->val      = val;

    return 1;
}

//-----------------------------------------------------------------------------

int GetRegIndex( const char* reg )
{
    if( reg == NULL ) return 0;

    int numRightIgnSyms = NumRightIgnoredSyms( reg, strlen( reg ) - 1, " ]" );

    int len = strlen( reg ) - numRightIgnSyms;
    
    if/* */( strncasecmp( reg, "rax", len ) == 0 ) return RAX; 
    else if( strncasecmp( reg, "rbx", len ) == 0 ) return RBX;
    else if( strncasecmp( reg, "rcx", len ) == 0 ) return RCX;
    else if( strncasecmp( reg, "rdx", len ) == 0 ) return RDX; 

    printf( "Register \"%.*s\" does not exist...\n", len, reg );

    return 0;
}

//-----------------------------------------------------------------------------

int AsmMakeBinFile( ASM* asm_s, FILE* fileOut )
{   
    if( asm_s == NULL || fileOut == NULL ) return 0;
    
    // signature, version, number of commands -> out file
    fprintf( fileOut, "%s %d %d\n", Signature, Version, asm_s->codeSize );

    fwrite( asm_s->code, sizeof( char ), asm_s->codeSize, fileOut );
    
    return 1;
}

//-----------------------------------------------------------------------------