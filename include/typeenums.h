#pragma once





typedef enum
{
	kCat_NULL = 0,
	kCat_UNKNOWN,
	kCat_Keyword,
	kCat_WildIdentifier,
	kCat_Immediate,
	kCat_Operator,
	kCat_LanguageOperator,
	kCat_BasicType

}TokenCategory;

typedef enum
{
	kToken_NULL = 0,
	kToken_UNKNOWN,
	
	kToken_VOID,
	kToken_STRING,
//	kToken_CHAR,
	kToken_BYTE,
	kToken_BOOL,

	kToken_INT8,
	kToken_INT16,
	kToken_INT32,
	kToken_INT64,

	kToken_UINT8,
	kToken_UINT16,
	kToken_UINT32,
	kToken_UINT64,

	kToken_SINGLE,
	kToken_DOUBLE,

	kToken_DYNAMIC,
	kToken_TYPENAME,
//	kToken_VOIDPTR,
	

	kToken_PTR,
	kToken_UNIQUE,
	kToken_SHARED,

	kToken_IF,
	kToken_ELSE,
	kToken_FOR,
	kToken_WHILE,
	kToken_MATCH,
	kToken_IS,

	kToken_BREAK,
	kToken_CONTINUE,
	kToken_RETURN,

	kToken_ENUM,
	kToken_STRUCT,

	kToken_USE,
	kToken_IMPORT,
	kToken_AS,
	kToken_UNIT,
	kToken_CINCLUDE,

	kToken_PUBLIC,
	kToken_PRIVATE,

	kToken_ISNULL,
	kToken_SIZEOF,


	kToken_SEMICOLON,
	kToken_COLON,
	kToken_COMMA,
	kToken_LPAREN,
	kToken_RPAREN,
	kToken_LCURLY,
	kToken_RCURLY,
	kToken_LBRACKET,
	kToken_RBRACKET,

	kToken_DECLARE_EQUAL,

	kToken_ASSIGN_EQUAL,
	kToken_PLUS,
	kToken_MINUS,
	kToken_MULTIPLY,
	kToken_DIVIDE,
	kToken_MODULO,

	kToken_PLUS_EQUAL,
	kToken_MINUS_EQUAL,
	kToken_MULTIPLY_EQUAL,
	kToken_DIVIDE_EQUAL,
	kToken_MODULO_EQUAL,

	kToken_INCREMENT,
	kToken_DECREMENT,

	kToken_LOGIC_EQUAL,
	kToken_LESS,
	kToken_GREATER,
	kToken_NOT,

	kToken_LESS_EQUAL,
	kToken_GREATER_EQUAL,
	kToken_NOT_EQUAL,

	kToken_LOGIC_AND,
	kToken_LOGIC_OR,

	kToken_ADDR,
	kToken_DOT,

	kToken_SFNAV, //safe navigation
	kToken_NULLCC, //null coalesce

	kToken_BIT_AND,
	kToken_BIT_OR,
	kToken_BIT_COMPL

}TokenType;