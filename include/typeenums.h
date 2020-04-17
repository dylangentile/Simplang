#pragma once


typedef enum
{
	kCat_NULL = 0,
	kCat_UNKNOWN,
	kCat_Keyword,
	kCat_WildIdentifier,
	kCat_Immediate,
	kCat_Operator,
	kCat_AssignmentOperator,
	kCat_LanguageOperator,
	kCat_BasicType

}TokenCategory;

typedef enum
{
	kToken_NULL = 0,
	kToken_UNKNOWN,
	
	
	kToken_STRING,
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
	

	kToken_PTR,
	kToken_UNIQUE,
	kToken_SHARED,

	kToken_IF,
	kToken_ELSE,
	kToken_FOR,
	kToken_WHILE,
	kToken_MATCH,

	kToken_ENUM,
	kToken_STRUCT,


	kToken_SEMICOLON,
	kToken_COLON,
	kToken_COMMA,
	kToken_LPAREN,
	kToken_RPAREN,
	kToken_LCURLY,
	kToken_RCURLY,
	kToken_LBRACKET,
	kToken_RBRACKET,

	kToken_ASSIGN_EQUAL,
	kToken_PLUS,
	kToken_MINUS,
	kToken_MULTIPLY,
	kToken_DIVIDE,
	kToken_MODULO,

	kToken_LOGIC_EQUAL,
	kToken_LESS,
	kToken_GREATER,
	kToken_NOT,

	kToken_LESS_EQUAL,
	kToken_GREATER_EQUAL,
	kToken_NOT_EQUAL,

	kToken_LOGIC_AND,
	kToken_LOGIC_OR,

	kToken_DEREF,

	kToken_BIT_AND,
	kToken_BIT_OR

}TokenType;