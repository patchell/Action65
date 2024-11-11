#ifndef  PCH__H
#define PCH__H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys\stat.h>
#include <sys\types.h>

constexpr auto MAX_SYMBOL_NAME_LEN = 256;
constexpr auto MAX_NAME_LEN = 64;



enum class IdentType {
	NEW_SYMBOL,
	LABEL,
	PROC,
	FUNC,
	FUNC_BYTE,
	FUNC_CHAR,
	FUNC_CARD,
	FUNC_INT,
	FUNC_POINTER_BYTE,
	FUNC_POINTER_CHAR,
	FUNC_POINTER_CARD,
	FUNC_POINTER_INT,
	MEM_GLOBAL_BYTE,
	MEM_GLOBAL_CHAR,
	MEM_GLOBAL_CARD,
	MEM_GLOBAL_INT,
	MEM_GLOBAL_POINTER_BYTE,
	MEM_GLOBAL_POINTER_CHAR,
	MEM_GLOBAL_POINTER_CARD,
	MEM_GLOBAL_POINTER_INT,
	MEM_PARAM_BYTE,
	MEM_PARAM_CHAR,
	MEM_PARAM_CARD,
	MEM_PARAM_INT,
	MEM_PARAM_POINTER_BYTE,
	MEM_PARAM_POINTER_CHAR,
	MEM_PARAM_POINTER_CARD,
	MEM_PARAM_POINTER_INT,
	MEM_LOCAL_BYTE,
	MEM_LOCAL_CHAR,
	MEM_LOCAL_CARD,
	MEM_LOCAL_INT,
	MEM_LOCAL_POINTER_BYTE,
	MEM_LOCAL_POINTER_CHAR,
	MEM_LOCAL_POINTER_CARD,
	MEM_LOCAL_POINTER_INT
};

enum  class Token {
	ENDOFFILE = -1,
	NUMBER = 256,
	STRING,
	IDENT,
	ASSIGN_ADD,
	ASSIGN_SUB,
	ASSIGN_MUL,
	ASSIGN_DIV,
	ASSIGN_MOD,
	ASSIGN_AND,
	ASSIGN_OR,
	ASSIGN_XOR,
	ASSIGN_RSH,
	ASSIGN_LSH,
	MOD,
	LSH,
	RSH,
	GTEQ,
	LTEQ,
	OR,		//logical OR
	XOR,	//bitwise Exclusive OR
	//----------------
	PROC,
	FUNC,
	//------ Data Types -----------------
	BOOL,
	BYTE,
	CHAR,
	INT,
	CARD,
	POINTER,
	TYPE,
	RECORDTYPE,
	ARRAY,
	//------------------
	MODULE,
	VECTOR,
	FUNC_CALL,
	PROC_CALL,
	DO,
	OD,
	IF,
	THEN,
	ELSEIF,
	ELSE,
	FI,
	WHILE,
	UNTIL,
	FOR,
	TO,
	STEP,
	EXIT,
	RETURN,
	ASM,
	ENDASM,
	END,
	BEGIN,
	// Compiler Directives
	DEFINE,
	SET,
	INCLUDE,
	//---------- Assembler Tokens ------------
	ADC,
	AND,	//Logical Action AND or ASM Opcode
	ASL,
	BCC,
	BCS,
	BEQ,
	BMI,
	BNE,
	BPL,
	BVC,
	BVS,
	BIT,
	BRK,
	CLC,
	CLD,
	CLI,
	CLV,
	CMP,
	CPX,
	CPY,
	DEC,
	DEX,
	DEY,
	EOR,
	INC,
	INX,
	INY,
	JMP,
	JSR,
	LDA,
	LDX,
	LDY,
	LSR,
	NOP,
	ORA,
	PHA,
	PLA,
	PHP,
	PLP,
	ROL,
	ROR,
	RTI,
	RTS,
	SBC,
	STA,
	SEC,
	SED,
	SEI,
	STX,
	STY,
	TAX,
	TAY,
	TXA,
	TYA,
	TXS,
	TSX,
	ENDOFOPCODES,
	AREG,		//accumulator
	XREG,
	YREG,
	SREG,		//stack pointer
	PREG,		//processor status register
	LOCAL_LABEL,
	GLOBAL_LABLE,
	CHAR_CONSTANT,
	ORG,
	DB,
	DW,
	DL,
	DAS,	//define action! string
	DCS,	//define C String
	DS,		//define storage
	SECTION,
	SECTION_NAME,
	START,
	SIZE,
	NAME,
	MODE,
	READ_WRTE,
	READ_ONLY,
	ZEROPAGE,
	True,
	False,
	R6502,
	PROCESSOR,
	WD65C02,
	WD65C816,
	EPROC,		// proceedure end
	EOL,
	ENDOFTOKENS = 0
};

enum class AdrModeType {
	NA,
	IMPLIED,
	INDIRECT_X_ADR,
	ZERO_PAGE_ADR,
	IMMEDIATE_ADR,
	ABSOLUTE_ADR,
	INDIRECT_Y_ADR,
	ZERO_PAGE_X_ADR,
	ABSOLUTE_Y_ADR,
	ABSOLUTE_X_ADR,
	ZERO_PAGE_Y_ADR,
	INDIRECT_ADR,
	ACCUMULATOR
};
enum class AddressModesClass {	//ToDo: Delete?
	IMPLIED,		//0
	ONE_MODE,		//1
	TWO_MODES,		//2
	THREE_MODES,	//3
	FOUR_MODES,		//4
	FIVE_MODES,		//5
	SIX_MODES,		//6
	SEVEN_MODES,	//7
	EIGHT_MODES		//8
};
enum class Processor {
	ALL,
	R6502,
	WD65C02,
	WD65C816
};


#include "StackItem.h"
#include "DataSizeStackItem.h"
#include "RegisterStackItem.h"
#include "Instruction.h"
#include "StackSectionItem.h"
#include "NumberStackItem.h"

#include "Bin.h"
#include "WhereSymbolIsUsed.h"
#include "UnResolvedSymbol.h"
#include "Bucket.h"
#include "Symbol.h"

#include "BinStackItem.h"
#include "StackSymbolItem.h"
#include "Stack.h"
#include "SymTab.h"
//--------------------------------------
// Object File Format
//--------------------------------------
#include "ObjFormatSectionItem.h"
#include "ObjFormatSymbolItem.h"
#include "ObjFormatSymbols.h"
#include "ObjFormatCode.h"
#include "ObjFormatData.h"
#include "ObjFormatRelocation.h"
#include "ObjFormatSection.h"
#include "ObjFormatFile.h"
//--------------------------------------
#include "Section.h"
#include "AddressSizeStackItem.h"
#include "AccessModeStackItem.h"
//---------------------------------------
//----------------Value Stack -----------
//---------------------------------------
#include "Stack.h"
#include "StackItem.h"
#include "StackNodeItem.h"
#include "StackSectionItem.h"
#include "StackSymbolItem.h"

//-------------- AST Base Class ----------
#include "AstNode.h"
//----------------------------------------
//------------- AST Node Classes ---------
//----------------------------------------
//------------ Program Structure --------------
#include "Act65Module.h"
#include "Act65VECTOR.h"
#include "Act65INTERRUPT.h"
#include "Act65PROC.h"
//----------- Declaration AST Nodes -----------
//----- Types ------
#include "Act65BYTE.h"
#include "Act65CHAR.h"
#include "Act65CARD.h"
#include "Act65BOOL.h"
#include "Act65INT.h"
#include "Act65RECTYPE.h"
#include "Act65POINTER.h"
//----- Modifiers --------------
#include "Act65ARRAY.h"
#include "Act65DEFINE.h"
#include "Act65TYPEDEF.h"

//-----------------------------------------
//---- Logical/Arithmetic AST Nodes -------
//-----------------------------------------
#include "Act65GreaterTHAN.h"
#include "Act65LessTHAN.h"
#include "Act65GTEQ.h"
#include "Act65LTEQ.h"
#include "Act65EqualTO.h"
#include "Act65NotEquelTO.h"
#include "Act65LogicalOR.h"
#include "Act65LogicalAND.h"
#include "Act65BitWiseOR.h"
#include "Act65BitWiseAND.h"
#include "Act65XOR.h"
#include "Act65ADD.h"
#include "Act65SUB.h"
#include "Act65LSH.h"
#include "Act65RSH.h"
#include "Act65ROR.h"
#include "Act65ROL.h"
//---------------------------------------------
//----------------Statement AST Nodes ---------
//---------------------------------------------
#include "Act65ProcCall.h"
#include "Act65FuncCall.h"
#include "Act65ProcParams.h"
#include "Act65FOR.h"
#include "Act65ForStart.h"
#include "Act65ForFinish.h"
#include "Act65ForSTEP.h"
#include "Act65ForItterator.h"
#include "Act65IF.h"
#include "Act65THEN.h"
#include "Act65ELSEIF.h"
#include "Act65ELSE.h"
#include "Act65IFF.h"
#include "Act65BEGIN.h"
#include "Act65END.h"
#include "Act65FFI.h"
#include "Act65FlagNEG.h"
#include "Act65FlagCARRY.h"
#include "Act65FlagZERO.h"
#include "Act65FlagOVERFLOW.h"
#include "Act65BIT.h"
#include "Act65WHILE.h"
#include "Act65DO.h"
#include "Act65OD.h"
#include "Act65EXIT.h"
#include "Act65RETURN.h"
#include "Act65ASM.h"
#include "Act65ASMstatement.h"
#include "Act65CodeBlock.h"
#include "Act65UNTILL.h"
#include "Act65PUSH.h"
#include "Act65PushSource.h"
#include "Act65POP.h"
#include "Act65PopDest.h"
//--------- Assignment Node Classes ------
#include "Act65Assignment.h"
#include "Act65AssignADD.h"
#include "Act65AssignSUB.h"
#include "Act65AssignMULT.h"
#include "Act65AssignDIV.h"
#include "Act65AssignMOD.h"
#include "Act65AssignOR.h"
#include "Act65AssignAND.h"
#include "Act65AssignXOR.h"
#include "Act65AssignLSH.h"
#include "Act65AssignRSH.h"
//------------------------------------
// Assembler AST Nodes
//------------------------------------
#include "Act65PROCESSOR.h"
#include "Act65SECTION.h"
#include "Act65SecAtrbSTART.h"
#include "Act65SecAtrbSIZE.h"
#include "Act65SecAtrbMODE.h"
#include "Act65SecAtrbZEROPAGE.h"
#include "Act65SecAtrbREADONLY.h"
#include "Act65SecAtrbREADWRITE.h"
#include "Act65TRUE.h"
#include "Act65SecAtrbFALSE.h"
#include "Act65ORG.h"
#include "Act65DB.h"
#include "Act65DW.h"
#include "Act65DL.h"
#include "Act65DAS.h"
#include "Act65DCS.h"
#include "Act65DS.h"
#include "Act65ASMPROC.h"
#include "Act65EPROC.h"
#include "Act65Opcode.h"
#include "Act65Label.h"
#include "Act65Private.h"
#include "Act65XREG.h"
#include "Act65YREG.h"
#include "Act65ACC.h"
#include "Act65CharCibstabt.h"
#include "Act65LowerPart.h"
#include "Act65UpperPart.h"
#include "Act65CurrentLocation.h"

//------------------------------------
#include "AstTree.h"
#include "ActionAstTree.h"

#include "Lexer.h"
#include "Parser.h"
#include "ActionApp.h"


constexpr auto MAX_EXCEPTION_STRING_LEN = 512;

class Exception {
public:
	enum ExceptionType {
		WHOKNOWS,
		UNEXPECTED_TOKEN,
		SECTION_ADDRES_RANGE_EXCEEDED,
		SECTION_UNDEFINED,
		NOSECTION_DEFINED,
		LEXER_STUMPTED,
		ILLEGAL_ADDRESSING_MODE,
		TOKEN_OUT_OF_PLACE,
		VALUE_EXCEEDS_RANGE,
		INTERNAL_ERROR
	};
private:
	ExceptionType m_Type;
	Token badToken;
	CBin* m_pSymbol;
	char ErrorString[MAX_EXCEPTION_STRING_LEN];
	inline static int MaxStringLen = MAX_EXCEPTION_STRING_LEN;
	struct ExcepTypeToString {
		ExceptionType m_Type;
		const char* m_Name;
		ExcepTypeToString() {
			m_Type = ExceptionType::WHOKNOWS;
			m_Name = 0;
		}
		ExcepTypeToString(ExceptionType Type, const char* pName) {
			m_Type = Type;
			m_Name = pName;
		}
	};
	inline static ExcepTypeToString ExceptionTypesLUT[] = {
		{ WHOKNOWS, "WHO KNOWS, Whatever" },
		{ UNEXPECTED_TOKEN, "UNEXPECTED TOKEN" },
		{ SECTION_ADDRES_RANGE_EXCEEDED, "SECTION ADDRES RANGE EXCEEDED" },
		{ SECTION_UNDEFINED, "SECTION UNDEFINED" },
		{ NOSECTION_DEFINED, "NO SECTION DEFINED" },
		{ LEXER_STUMPTED, "LEXER is STUMPTED!" },
		{ ILLEGAL_ADDRESSING_MODE, "ILLEGAL ADDRESSING MODE" },
		{ TOKEN_OUT_OF_PLACE, "TOKEN OUT OF PLACE" },
		{ VALUE_EXCEEDS_RANGE, "VALUE EXCEEDS RANGE" },
		{ INTERNAL_ERROR, "This is too much, INTERNAL ERROR!" },
		{ ExceptionType(-1), NULL}
	};
public:
	Exception() {
		int i;

		m_Type = ExceptionType(0);
		badToken = Token(0);
		m_pSymbol = 0;
		for (i = 0; i < MAX_EXCEPTION_STRING_LEN; ++i)
			ErrorString[i] = 0;
	}
	const char* GetExceptionTypeString(ExceptionType xType);
	ExceptionType GetXCeptType() { return m_Type; }
	void SetXCeptType(ExceptionType Type) { m_Type = Type; }
	Token GetGotToken() { return badToken; }
	void SetGotToken(Token t) { badToken = t; }
	char* GetErrorString() { return ErrorString; }
	int GetMaxStringLen() {
		return MaxStringLen;
	}
	void SetSymbol(CBin* pSym) { m_pSymbol = pSym; }
	CBin* GetSymbol() { return m_pSymbol; }
};

extern Exception ExceptionThrown;


#endif // ! PCH__H

