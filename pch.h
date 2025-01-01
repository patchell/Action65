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
constexpr auto MAX_STRING_LEN = 512;

enum  class Token {
	ENDOFFILE = -1,	// 1
	NONE,
	IDENT = 256,	//2
	STRING,			//3
	NUMBER,			//4
	DEFINED_IDENT,
	//------ Assignment
	ASSIGN_ADD,		//5
	ASSIGN_SUB,		//6
	ASSIGN_MUL,		//7
	ASSIGN_DIV,		//8
	ASSIGN_MOD,		//9
	ASSIGN_AND,		//10
	ASSIGN_OR,
	ASSIGN_XOR,
	ASSIGN_RSH,
	ASSIGN_LSH,
	//------- Operators
	MOD,
	LSH,
	RSH,
	GTEQ,
	LTEQ,
	OR,		//logical OR
	XOR,	//bitwise Exclusive OR
	//-------- Functions --------
	PROC,
	FUNC,
	INTERRUPT,
	INTERRUPT_IDENT,
	FUNC_IDENT,
	PROC_IDENT,
	DECLARE,
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
	VAR_GLOBAL,
	VAR_LOCAL,
	VAR_PARAM,
	//--------Statements----------
	MODULE,
	VECTOR,
	FOR,
	TO,
	STEP,
	IF,
	IFF,
	THEN,
	ELSEIF,
	ELSE,
	FI,
	FFI,
	WHILE,
	DO,
	OD,
	UNTIL,
	EXIT,
	RETURN,
	ASM,
	PUSH,
	POP,
	BREAK,
	BITS,
	BEGIN,
	END,
	// Compiler Directives
	DEFINE,
	SET,
	INCLUDE,
	//---------- Assembler Tokens ------------
	//--------- Section --------
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
	//----- Assembler declarators
	ORG,
	DB,
	DW,
	DL,
	DAS,	//define action! string
	DCS,	//define C String
	DS,		//define storage
	//------- Opcodes ALL
	ADC,
	AND,	//Logical Action or ASM AND  Opcode
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
	//---------------------
	// 65C02S opcodes
	//---------------------
	BRA,		//  Branch Always
	PHX,		// push index reg X
	PHY,		// push Y register
	PLX,		// pop stack to X
	PLY,		// pop stack to Y
	STZ,		// Store Zero to Memory
	TRB,		// Test and reset bits
	TSB,		// Test and Set bits
	ENDOFOPCODES,
	//----- Registers
	AREG,		//accumulator
	XREG,
	YREG,
	SPREG,		//stack pointer
	PSREG,		//processor status register
	//---------- Lables 
	LOCAL_LABEL,
	GLOBAL_LABLE,
	//-------- Status Register bits
	NEG,
	ZERO,
	CARRY,
	OVERFLOW,
	IRQENABLE,
	DECIMAL_MODE,
	//------- Processor Selector
	PROCESSOR,
	R6502,
	W65C02,
	W65C816,
	//------- Misc
	CHAR_CONSTANT,
	EPROC,		// proceedure end
	EOL,
	//------------------------
	ENDOFTOKENS = 0
};

enum class IdentType {
	NEW_SYMBOL,
	LABEL,
	PROC,
	FUNC,
	GLOBAL,
	LOCAL,
	SECTION
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
	W65C02,
	W65C816
};

struct AdressModeItem {
	AdrModeType Mode;
	int inc;
	AdressModeItem() {
		Mode = AdrModeType::NA;
		inc = 0;
	}
	AdressModeItem(AdrModeType T, int I) {
		Mode = T;
		inc = I;
	}
	int GetInc(AdrModeType ModeType) const {
		//---------------------------
		// GetInc
		//	Compares the address mode
		// type of this object to the
		// one that is desired.  If
		// they are equal, we get
		// back the increment value,
		// If they are not equal, we
		// get back -1
		//---------------------------
		int rV = -1;

		if (ModeType == Mode)
			rV = inc;
		return rV;
	}
};

struct AdressModeLUT {
	AdressModeItem* ModeInc;
	int m_nElements;
	AdressModeLUT() {
		ModeInc = 0;
		m_nElements = 0;
	}
	//--------------------
	AdressModeLUT(int n, AdressModeItem* MI) {
		ModeInc = MI;
		m_nElements = n;
	}
	//-------------------
	int GetInc(AdrModeType Type) {
		int i, rV = -1;
		bool Loop = true;

		for (i = 0; Loop && i < m_nElements; ++i)
		{
			rV = ModeInc[i].GetInc(Type);
			if (rV >= 0)
				Loop = false;
		}
		return rV;
	}
};

struct KeyWord {
	Token m_TokenID;	//token value
	const char* m_Name;	//token name
	int m_MaxBytes;			//Maximum number of bytes for instruction
	int m_OpCode;			//base opcode
	Processor m_Processor;	//for which processor
	AdressModeLUT* m_pAddresModeLUT;
	int NumOfAdrModes;
	//------ Methods ------
	int FindInc(AdrModeType AdrMode);
	static Token LookupToToken(const char* pName);
};


#include "ObjType.h"

#include "Bin.h"
#include "Bucket.h"
#include "TypeChain.h"
#include "ParameterChain.h"
#include "Symbol.h"
#include "Value.h"

constexpr auto MAX_EXCEPTION_STRING_LEN = 512;

class Exception {
public:
	enum class ExceptionType {
		WHOKNOWS,
		UNEXPECTED_TOKEN,
		SECTION_ADDRES_RANGE_EXCEEDED,
		SECTION_UNDEFINED,
		NOSECTION_DEFINED,
		LEXER_STUMPTED,
		ILLEGAL_ADDRESSING_MODE,
		TOKEN_OUT_OF_PLACE,
		VALUE_EXCEEDS_RANGE,
		STACK,
		INTERNAL_ERROR
	};
	enum class ExceptionSubType {
		WHOKNOWS,
		//--------------------
		// Stack Exceptions Subtypes
		//--------------------
		STACK_UNEXPECTED_NULL,
		STACK_EMPTY,
		STACK_ITEM_MISMATCH
	};
private:
	ExceptionType m_Type;
	ExceptionSubType m_SubType;		
	const char* m_Name;
	Token badToken;
	CBin* m_pSymbol;
	char ErrorString[MAX_EXCEPTION_STRING_LEN];
	inline static int MaxStringLen = MAX_EXCEPTION_STRING_LEN;
	//------------------------------------------------
	struct ExceptionSubTypeStrings {
		ExceptionSubType m_Type;
		const char* m_Name;
		ExceptionSubTypeStrings() {
			m_Type = ExceptionSubType::WHOKNOWS;
			m_Name = 0;
		}
		ExceptionSubTypeStrings(ExceptionSubType Type, const char* pName) {
			m_Type = Type;
			m_Name = pName;
		}
		const char* FindSubTypeString(ExceptionSubType Type);
	};
	//------------------------------------------------
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
		const char* FindString(ExceptionType Type);
	};
	//-------------------------------------------------------
	inline static ExcepTypeToString ExceptionTypesLUT[] = {
		{ExceptionType:: WHOKNOWS, "WHO KNOWS, Whatever" },
		{ ExceptionType::UNEXPECTED_TOKEN, "UNEXPECTED TOKEN" },
		{ ExceptionType::SECTION_ADDRES_RANGE_EXCEEDED, "SECTION ADDRES RANGE EXCEEDED" },
		{ ExceptionType::SECTION_UNDEFINED, "SECTION UNDEFINED" },
		{ ExceptionType::NOSECTION_DEFINED, "NO SECTION DEFINED" },
		{ExceptionType::LEXER_STUMPTED, "LEXER is STUMPTED!" },
		{ ExceptionType::ILLEGAL_ADDRESSING_MODE, "ILLEGAL ADDRESSING MODE" },
		{ ExceptionType::TOKEN_OUT_OF_PLACE, "TOKEN OUT OF PLACE" },
		{ ExceptionType::VALUE_EXCEEDS_RANGE, "VALUE EXCEEDS RANGE" },
		{ ExceptionType::INTERNAL_ERROR, "This is too much, INTERNAL ERROR!" },
		{ ExceptionType(-1), NULL}
	};
	//--------------------------------------------------
	inline static ExceptionSubTypeStrings XcepSubTypesLUT[] =
	{
		{ExceptionSubType::WHOKNOWS,"Who Knows, Not me!"},
		{ExceptionSubType::STACK_UNEXPECTED_NULL,"Top of Stack is NULL"},
		{ExceptionSubType::STACK_EMPTY,"Stack is Empty"},
		{ExceptionSubType::STACK_ITEM_MISMATCH,"Mismatch"},
		{ExceptionSubType(-1),0}
	};
public:
	Exception() {
		int i;

		m_Name = 0;
		m_Type = ExceptionType::WHOKNOWS;
		m_SubType = ExceptionSubType::WHOKNOWS;
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
	const char* FindXceptnSubType(ExceptionSubType Type) {
		return XcepSubTypesLUT->FindSubTypeString(Type);
	}
	const char* FindXceptnType(ExceptionType Type) {
		return ExceptionTypesLUT->FindString(Type);
	}
};

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
#include "WhereSymbolIsUsed.h"
#include "UnResolvedSymbol.h"
//---------------------------------------
//----------------Value Stack -----------
//---------------------------------------

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
#include "Act65ForSTEP.h"
#include "Act65ForTO.h"
#include "Act65ForItterator.h"
#include "Act65IF.h"
#include "Act65FI.h"
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
#include "Act65OptNOT.h"
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
#include "Act65RTI.h"
#include "Act65BREAK.h"
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
#include "Act65W65C02.h"
#include "Act65W65C816.h"
#include "Act65R6502.h"
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
#include "Act65ParamList.h"
#include "Act65PROCname.h"
#include "Act65IDENT.h"
#include "Act65CONSTANT.h"
#include "Act65AddressOF.h"
#include "Act65AdrOfCONST.h"
#include "Act65FuncADDR.h"
#include "Act65AddrOfINTERRUPT.h"
#include "Acrt65ProcADDR.h"
#include "Act65IdentList.h"
#include "Act65ConstLIST.h"
#include "Act65STRING.h"
#include "Act65InitDATA.h"
#include "Act65Identifier.h"
#include "Act65FUNC.h"
#include "Act65TypeFIELDS.h"
#include "Act65DefineOBJECT.h"
#include "Act65TYPE.h"
#include "Act65DefLIST.h"
#include "Act65StatusFLAGS.h"
#include "Act65ArrayINDEX.h"
#include "Act65PointerDeREF.h"
#include "Act65TypeDotField.h"
#include "Act65VALUE.h"
#include "Act65UnaryNEG.h"
#include "Act65MUL.h"
#include "Act65MOD.h"
#include "Act65DIV.h"
#include "AstTree.h"
#include "ActionAstTree.h"

#include "LkHead.h"

#include "Lexer.h"
#include "Parser.h"
#include "ActionApp.h"


extern Exception ThrownException;


#endif // ! PCH__H

