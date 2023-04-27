#ifndef CONFIG_H
#define CONFIG_H

//-----------------------------------------------------------------------------

struct LngNode
{
    int type; 

    // union in progress
    double dblValue;
    int    opValue;
    char*  varValue;
};

struct Node;
struct VarTable;

typedef  LngNode* TreeElem_t;
typedef VarTable*     Elem_t;

// #define STACK_DUMP

//-----------------------------------------------------------------------------

enum Types
{
    END_PROG_TYPE,
    SEMICOLON_TYPE,

    ST_TYPE,
    
    OP_TYPE, 

    VAL_TYPE, 
    VAR_TYPE,

    L_COMMENT_TYPE, // /*
    R_COMMENT_TYPE, // */

    L_BRACKET_TYPE, // (
    R_BRACKET_TYPE, // )

    L_BRACE_TYPE,
    R_BRACE_TYPE,

    VAR_INIT_TYPE,

    EQ_TYPE,

    IF_TYPE,
    ELSE_TYPE, 

    WHILE_TYPE, 
    
    FUNC_TYPE,
    PARAM_TYPE,
    COMMA_TYPE,
    TYPE_TYPE, 
    VOID_TYPE, 
    CALL_TYPE,
    RET_TYPE,

    IN_TYPE,
    OUT_TYPE
};

//-----------------------------------------------------------------------------


// Operation description
//-----------------------------------------------------------------------------

enum OperationTypes
{
    OP_ADD = 0, 
    OP_SUB = 1,

    OP_MUL = 3,
    OP_DIV = 4,

    OP_LN  = 6,

    OP_SIN = 8,
    OP_COS = 9,

    OP_DEG = 11, 

    OP_SQRT = 13,

    OP_EE, 
    OP_GE,
    OP_BE,
    OP_GT,
    OP_BT,
    OP_NE
};

static int UnaryOperations[] = 
{
    OP_SIN,
    OP_COS,
    OP_LN,
    OP_SQRT
};

static const int NumUnaryOperations = sizeof( UnaryOperations ) / sizeof( int ); 

struct OpStr
{
    int         opNum;
    const char* str;
    const char* strStandart;
    const char* strAsm;
};

static const OpStr OpStrings[] = 
{  
    { OP_ADD,  "+",   "ADD",    "add"   },
    { OP_SUB,  "-",   "SUB",    "sub"   },
    { OP_MUL,  "*",   "MUL",    "mul"   },
    { OP_DIV,  "/",   "DIV",    "div"   },
    { OP_DEG,  "^",   "POW",    "pow"   },
    { OP_SIN,  "син", "SIN",    "sin"   },
    { OP_COS,  "кос", "COS",    "cos"   },
    { OP_LN,   "лог", "LN",     "ln"    },
    { OP_SQRT, "кор", "SQRT",   "sqrt"  },

    { OP_EE,   "==",  "IS_EE",  "is_ee" },
    { OP_GE,   ">=",  "IS_GE",  "is_ge" },
    { OP_BE,   "<=",  "IS_BE",  "is_be" },
    { OP_GT,   ">",   "IS_GT",  "is_gt" },
    { OP_BT,   "<",   "IS_BT",  "is_bt" },
    { OP_NE,   "!=",  "IS_NE",  "is_ne" }
};

static const int NumOpStrings = sizeof( OpStrings ) / sizeof( OpStr );

// End operation description
//-----------------------------------------------------------------------------


//  Types description
//-----------------------------------------------------------------------------

struct TypeStr
{
    int         typeNum;
    const char* str;
    const char* strStandart;
};

static const TypeStr TypeStrings[] = 
{
    { END_PROG_TYPE,  "\\0", "" },
    { SEMICOLON_TYPE, ";", "" },

    { ST_TYPE, "ST", "ST" },
    
    { L_COMMENT_TYPE, "!%", "" },
    { R_COMMENT_TYPE, "%!", "" },

    { L_BRACE_TYPE, "!:", "" },
    { R_BRACE_TYPE, ":!", "" },
    
    { L_BRACKET_TYPE, "(", "" },
    { R_BRACKET_TYPE, ")", "" },

    { VAR_INIT_TYPE, "варибля", "VAR" },
    { EQ_TYPE, "=", "EQ" }, 

    { IF_TYPE,   "проверочка", "IF" },
    { ELSE_TYPE, "иначе", "ELSE" },

    { WHILE_TYPE, "живем", "WHILE" },
    
    { FUNC_TYPE,  "FUNC",   "FUNC" },
    { COMMA_TYPE, ",",      "" },
    { PARAM_TYPE, "PARAM",  "PARAM" },
    { TYPE_TYPE,  "TYPE",   "TYPE" },
    { VOID_TYPE,  "войд",   "VOID" },
    { CALL_TYPE,  "CALL",   "CALL" },
    { RET_TYPE,   "ретерн", "RET" },

    { IN_TYPE,  "сканф",  "IN" }, 
    { OUT_TYPE, "принтф", "OUT" }
};

static const int NumTypeStrings = sizeof( TypeStrings ) / sizeof( TypeStr );

//  End types description
//-----------------------------------------------------------------------------


//  Constants
//-----------------------------------------------------------------------------

const int MaxStrLen   = 255;
const int MaxNumNodes = 10000;

const double Accuracy = 0.001;

//-----------------------------------------------------------------------------

#endif