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

constexpr auto INTERNAL_ERROR = 2;

enum  class Token 
{
	ENDOFFILE = -1,	// 1
	NONE,
	IDENT = 256,	//2
	STRING,			//3
	NUMBER,			//4
	CUR_LOC,
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
	PROC,				//Proc Keyword Token
	FUNC,				//Func Keyword Token
	INTERRUPT,			//Interrupt Keyword Token
	INTERRUPT_IDENT,	//Interrupt Symbol Token
	FUNC_IDENT,			//Function Symbol Token
	PROC_IDENT,			//Proc Symbol Keyword
	DECLARE,
	//------ Data Types -----------------
	BOOL,
	BYTE,
	CHAR,
	INT,
	CARD,
	POINTER,
	CONST,
	TYPE,
	TYPE_FIELD,
	RECORDTYPE,
	ARRAY,
	VAR_GLOBAL,
	VAR_LOCAL,
	VAR_PARAM,
	//--------Statements----------
	MODULE,
	ASMMODULE,
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
	//---------- Assembler Tokens ------------
	//--------- Section --------
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
	GLOBAL_LABEL,
	LABEL,
	//-------- Status Register bits
	NEG,
	ZERO,
	CARRY,
	OVERFLOW,
	IRQENABLE,
	DECIMAL_MODE,
	//---------- Compiler Directives -----------
	INCLUDE,
	SET,
	DEFINE,
	SECTION,
	SECTION_NAME,
	START,
	SIZE,
	NAME,
	MODE,
	READ_WRTE,
	READ_ONLY,
	PAGEZERO,
	TRUE,
	FALSE,
	PROCESSOR,
	W65C02,
	R6502,
	W65C816,
	//------- Misc
	CHAR_CONSTANT,
	EPROC,		// proceedure end
	EOL,
	//------------------------
	ENDOFTOKENS = 0
};


enum class AdrModeType {
	NA,
	IMPLIED,
	INDEXED_INDIRECT_X_ADR,
	ZERO_PAGE_ADR,
	IMMEDIATE_ADR,
	ABSOLUTE_ADR,
	INDIRECT_INDEXED_Y_ADR,
	ZERO_PAGE_X_ADR,
	ABSOLUTE_Y_ADR,
	ABSOLUTE_X_ADR,
	ZERO_PAGE_Y_ADR,
	INDIRECT_ADR,
	ACCUMULATOR,
	RELATIVE
};


struct AdrModeToTextLUT 
{
	struct AdrModeTypeToStringItem {
		AdrModeType m_AdressMode;
		const char* m_pName;
		AdrModeTypeToStringItem() {
			m_AdressMode = AdrModeType::NA;
			m_pName = 0;
		}
		AdrModeTypeToStringItem(AdrModeType T, const char* pName) {
			m_AdressMode = T;
			m_pName = pName;
		}
		const char* GetName(AdrModeType T) const {
			const char* pName = 0;

			if (T == m_AdressMode)
				pName = m_pName;
			return pName;
		}
	};
	inline static AdrModeTypeToStringItem AdrModeLUT[14] = {
		{AdrModeType::NA,""},
		{AdrModeType::IMPLIED,"IMPLIED"},
		{AdrModeType::INDEXED_INDIRECT_X_ADR,"INDEXED INDIRECT"},
		{AdrModeType::ZERO_PAGE_ADR,"ZERO PAGE"},
		{AdrModeType::IMMEDIATE_ADR,"IMMEDIATE"},
		{AdrModeType::ABSOLUTE_ADR,"ABSOLUTE"},
		{AdrModeType::INDIRECT_INDEXED_Y_ADR,"INDIRECT INDEXED"},
		{AdrModeType::ZERO_PAGE_X_ADR,"PAGE ZERO,X"},
		{AdrModeType::ABSOLUTE_Y_ADR,"ABSOLUTE,Y"},
		{AdrModeType::ABSOLUTE_X_ADR,"ABSOLUTE,X"},
		{AdrModeType::ZERO_PAGE_Y_ADR,"PAGE ZERO,Y"},
		{AdrModeType::INDIRECT_ADR,"INDIRECT"},
		{AdrModeType::ACCUMULATOR,"ACCUMULATOR"},
		{AdrModeType::RELATIVE,"RELATIVE"}
	};
	static const char* LookupAddressingMode(AdrModeType T)
	{
		const char* pN = 0;
		int i;
		bool Loop = true;

		for (i = 0; Loop && (i < 14); ++i)
		{
			if (AdrModeLUT[i].GetName(T))
			{
				pN = AdrModeLUT[i].GetName(T);
				Loop = false;
			}
		}
		return pN;
	}
	static int Print(char* s, int l, int Address, AdrModeType T);
};

extern AdrModeToTextLUT AdrModeToTxtTabel;

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

struct AdressModeItem 
{
	AdrModeType Mode;
	int inc;		// Amount to add to the base opcode 
					// to get the opcode for this addressing mode
	int m_numBytes;	// Number of bytes this instruction takes
					// in memory including the opcode byte
	AdressModeItem() {
		Mode = AdrModeType::NA;
		inc = 0;
		m_numBytes = 0;
	}
	AdressModeItem(AdrModeType T, int I, int N) {
		Mode = T;
		inc = I;
		m_numBytes = N;
	}
	int GetInc(AdrModeType ModeType) const {
		//---------------------------
		// GetInc
		// Check to see if this is
		// the amount to increment
		// the opcode by for the
		// given address mode
		// type.
		// 
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
	int GetNumBytes(AdrModeType ModeType) const {
		int rV = -1;
		if (ModeType == Mode)
			rV = m_numBytes;
		return rV;
	}
};

struct AdressModeLUT 
{
	AdressModeItem* m_pModeInc;
	int m_nElements;
	AdressModeLUT() {
		m_pModeInc = 0;
		m_nElements = 0;
	}
	//--------------------
	AdressModeLUT(int n, AdressModeItem* MI) {
		m_pModeInc = MI;
		m_nElements = n;
	}
	//-------------------
	int GetInc(AdrModeType Type) const {
		int i, rV = -1;
		bool Loop = true;

		for (i = 0; Loop && i < m_nElements; ++i)
		{
			rV = m_pModeInc[i].GetInc(Type);
			if (rV >= 0)
				Loop = false;
		}
		return rV;
	}
	int GetNumberOfBytes(AdrModeType Type) const {
		int i, rV = -1;
		bool Loop = true;

		for (i = 0; Loop && i < m_nElements; ++i)
		{
			rV = m_pModeInc[i].GetNumBytes(Type);
			if (rV >= 0)
				Loop = false;
		}
		return rV;
	}
	bool ValidAddressingMode(AdrModeType AMT) const;
};


#include "KeyWord.h"
#include "ChainItem.h"
#include "Chain.h"
#include "Bin.h"
#include "Bucket.h"
//--------------------------------------
#include "ChainTypeSpecItem.h"
#include "ChainItemItem.h"
#include "ChainParameterItem.h"
#include "ChainBinItem.h"
#include "ChainSymUsedItem.h"
#include "ChainLocalItem.h"
#include "ChainDataItem.h"
#include "ChainStorageItem.h"
#include "ChainTypeSpec.h"
#include "ChainLocals.h"
#include "ChainParameter.h"
//--------------------------------------
#include "Symbol.h"
#include "Macro.h"
#include "VirtualReg.h" //- Virtual Registers 
#include "Value.h"
#include "ChainValueItem.h"
#include "ChainValue.h"
#include "SymTab.h"
#include "Settings.h"

constexpr auto MAX_EXCEPTION_STRING_LEN = 512;

class Exception 
{
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
		INTERNAL_ERROR,
		EXPECTED_IDENT,
		EXPECTED_CONSTANT,
		EXPECTED_DATABLOCK,
		EXPECTED_INDEX_REG,
		EXPECTED_STRING,
		EXPECTED_PROC_FUNC_INTERRUPT,
		AST_RECURSION_LIMIT,
		INTERNAL_SYMBOL_NULL,
		INTERNAL_VALUE_NULL,
		//--------------------
		// Code Generation Exceptions
		//--------------------
		CODEGEN_VALUE_NULL,
		CODEGEN_NO_SECTION,
		CVALUE_NO_SYMBOL,
		CODEGEN_UNKNOWN_BYTE_ORDER,
		//--------------------
		// Initialization Exceptions
		//--------------------
		INIT_INPUT_FILE,
		INIT_SYMTAB,
		UNDEFINED_SECTION_NAME
	};
	struct ExceptionTypeStrings {
		ExceptionType m_Type;
		const char* m_Name;
		ExceptionTypeStrings() {
			m_Type = ExceptionType::WHOKNOWS;
			m_Name = 0;
		}
		ExceptionTypeStrings(ExceptionType Type, const char* pName) {
			m_Type = Type;
			m_Name = pName;
		}
		static const char* FindExceptionTypeString(ExceptionType Type);
	};
	static inline ExceptionTypeStrings ExceptionTypesLUT[] = {
		{ExceptionType:: WHOKNOWS, "WHO KNOWS, Whatever" },
		{ ExceptionType::UNEXPECTED_TOKEN, "UNEXPECTED Token" },
		{ ExceptionType::SECTION_ADDRES_RANGE_EXCEEDED, "SECTION ADDRES RANGE EXCEEDED" },
		{ ExceptionType::SECTION_UNDEFINED, "SECTION UNDEFINED" },
		{ ExceptionType::NOSECTION_DEFINED, "NO SECTION DEFINED" },
		{ ExceptionType::LEXER_STUMPTED, "LEXER is STUMPTED!" },
		{ ExceptionType::ILLEGAL_ADDRESSING_MODE, "ILLEGAL ADDRESSING MODE" },
		{ ExceptionType::TOKEN_OUT_OF_PLACE, "TOKEN OUT OF PLACE" },
		{ ExceptionType::VALUE_EXCEEDS_RANGE, "VALUE EXCEEDS RANGE" },
		{ ExceptionType::STACK, "STACK EXCEPTION" },
		{ ExceptionType::INTERNAL_ERROR, "This is too much, INTERNAL ERROR!" },
		{ ExceptionType::EXPECTED_IDENT, "Expected an Identifier::ERROR!" },
		{ ExceptionType::EXPECTED_CONSTANT, "Expected a Constant::ERROR!" },
		{ ExceptionType::EXPECTED_DATABLOCK, "Expected a Data Block::ERROR!" },
		{ ExceptionType::EXPECTED_INDEX_REG, "Expected an Index Register::ERROR!" },
		{ ExceptionType::EXPECTED_STRING, "Expected a String::ERROR!" },
		{ ExceptionType::EXPECTED_PROC_FUNC_INTERRUPT, "Expected a Proc, Func or Interrupt::ERROR!" },
		{ ExceptionType::AST_RECURSION_LIMIT, "AST Recursion Limit Exceeded::ERROR!" },
		{ ExceptionType::INTERNAL_SYMBOL_NULL, "Internal Symbol is NULL::ERROR!" },
		{ ExceptionType::INTERNAL_VALUE_NULL, "Internal Value is NULL::ERROR!" },
		{ ExceptionType::CODEGEN_VALUE_NULL, "CodeGen:  Variable Value is NULL::ERROR!" },
		{ ExceptionType::CODEGEN_NO_SECTION, "CodeGen: No Section Defined::ERROR!" },
		{ ExceptionType::CVALUE_NO_SYMBOL, "CValue: No Symbol Defined::ERROR!" },
		{ ExceptionType::CODEGEN_UNKNOWN_BYTE_ORDER, "CodeGen: Unknown Byte Order::ERROR!" },
		{ ExceptionType::INIT_INPUT_FILE, "Initialization: Input File Error::ERROR!" },
		{ ExceptionType::INIT_SYMTAB, "Initialization: Symbol Table Error::ERROR!" },
		{ ExceptionType::UNDEFINED_SECTION_NAME, "Undefined Section Name::ERROR!" },
		{ ExceptionType(-1), NULL}
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
	//--------------------------------------------------
	inline static ExceptionSubTypeStrings XcepSubTypesLUT[] =
	{
		{ExceptionSubType::WHOKNOWS,"NONE"},
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
	ExceptionSubType GetExceptionSubType() { return m_SubType; }
	void SetExceptionSubType(ExceptionSubType SubType) { m_SubType = SubType; }
	Token GetGotToken() { return badToken; }
	void SetGotToken(Token t) { badToken = t; }
	char* GetErrorString() { return ErrorString; }
	int GetMaxStringLen() {
		return MaxStringLen;
	}
	void SetSymbol(CBin* pSym) { m_pSymbol = pSym; }
	CBin* GetSymbol() { return m_pSymbol; }
	static const char* FindExceptionSubTypeString(ExceptionSubType Type) {
		return XcepSubTypesLUT->FindSubTypeString(Type);
	}
	static const char* FindExceptionTypeString(ExceptionType Type) {
		return ExceptionTypesLUT->FindExceptionTypeString(Type);
	}
};

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

#include "CodeGeneration.h"
#include "Section.h"
#include "Linker.h"
#include "Reg.h"
#include "RegPool.h"

//-------------- AST Base Class ----------
#include "AstNode.h"
//----------------------------------------
//------------- AST Node Classes ---------
//----------------------------------------
#include "Act65ACC.h"
#include "Act65ADD.h"
#include "Act65AddressOF.h"
#include "Act65AdrOfCONST.h"
#include "Act65AddrOfINTERRUPT.h"
#include "Act65ARRAY.h"
#include "Act65ARRAYdim.h"
#include "Act65ArrayINDEX.h"
#include "Act65ASM.h"
#include "Act65AsmModule.h"
#include "Act65ASMstatement.h"
#include "Act65ASMPROC.h"
#include "Act65Assignment.h"
#include "Act65AssignADD.h"
#include "Act65AssignAND.h"
#include "Act65AssignDIV.h"
#include "Act65AssignEQ.h"
#include "Act65AssignLSH.h"
#include "Act65AssignMOD.h"
#include "Act65AssignMULT.h"
#include "Act65AssignOR.h"
#include "Act65AssignRSH.h"
#include "Act65AssignSUB.h"
#include "Act65AssignXOR.h"
#include "Act65BEGIN.h"
#include "Act65BREAK.h"
#include "Act65BIT.h"
#include "Act65BITS.h"
#include "Act65BitWiseAND.h"
#include "Act65BitWiseOR.h"
#include "Act65BODY.h"
#include "Act65BOOL.h"
#include "Act65BYTE.h"
#include "Act65CARD.h"
#include "Act65CHAR.h"
#include "Act65CharConstant.h"
#include "Act65CodeBlock.h"
#include "Act65CONDITIONAL.h"
#include "Act65CONST.h"
#include "Act65CONSTANT.h"
#include "Act65ConstLIST.h"
#include "Act65CurrentLocation.h"
#include "Act65DAS.h"
#include "Act65DB.h"
#include "Act65DCS.h"
#include "Act65DeclareParams.h"
#include "Act65DefineOBJECT.h"
#include "Act65DEFINE.h"
#include "Act65DefLIST.h"
#include "Act65DIV.h"
#include "Act65DL.h"
#include "Act65DO.h"
#include "Act65DS.h"
#include "Act65DW.h"
#include "Act65ELSE.h"
#include "Act65ELSEIF.h"
#include "Act65END.h"
#include "Act65EPROC.h"
#include "Act65EqualTO.h"
#include "Act65EXIT.h"
#include "Act65FFI.h"
#include "Act65FI.h"
#include "Act65FOR.h"
#include "Act65FlagCARRY.h"
#include "Act65FlagNEG.h"
#include "Act65FlagOVERFLOW.h"
#include "Act65FlagZERO.h"
#include "Act65ForSTEP.h"
#include "Act65ForTO.h"
#include "Act65ForItterator.h"
#include "Act65FUNC.h"
#include "Act65FuncADDR.h"
#include "Act65FuncCall.h"
#include "Act65GreaterTHAN.h"
#include "Act65GTEQ.h"
#include "Act65IDENT.h"
#include "Act65IDENTaddress.h"
#include "Act65IdentList.h"
#include "Act65IF.h"
#include "Act65IFF.h"
#include "Act65InitDATA.h"
#include "Act65INT.h"
#include "Act65INTERRUPT.h"
#include "Act65Label.h"
#include "Act65LessTHAN.h"
#include "Act65LocalVar.h"
#include "Act65LogicalAND.h"
#include "Act65LogicalOR.h"
#include "Act65LowerPart.h"
#include "Act65LSH.h"
#include "Act65LTEQ.h"
#include "Act65MOD.h"
#include "Act65Module.h"
#include "Act65MUL.h"
#include "Act65NotEquelTO.h"
#include "Act65NUMBER.h"
#include "Act65OD.h"
#include "Act65Opcode.h"
#include "Act65OptNOT.h"
#include "Act65ORG.h"
#include "Act65ParamList.h"
#include "Act65PROC.h"
#include "Act65ProcADDR.h"
#include "Act65PROCasm.h"
#include "Act65ProcCall.h"
#include "Act65PROCname.h"
#include "Act65ProcParams.h"
#include "Act65PointerDeREF.h"
#include "Act65POINTER.h"
#include "Act65POP.h"
#include "Act65PopDest.h"
#include "Act65Private.h"
#include "Act65PROCESSOR.h"
#include "Act65PUSH.h"
#include "Act65PushSource.h"
#include "Act65R6502.h"
#include "Act65PSReg.h"
#include "Act65RECTYPE.h"
#include "Act65RETURN.h"
#include "Act65ROOT.h"
#include "Act65ROR.h"
#include "Act65ROL.h"
#include "Act65RSH.h"
#include "Act65RTI.h"
#include "Act65SECTION.h"
#include "Act65SET.h"
#include "Act65Statements.h"
#include "Act65StatusFLAGS.h"
#include "Act65STRING.h"
#include "Act65SUB.h"
#include "Act65THEN.h"
#include "Act65TRUE.h"
#include "Act65TYPE.h"
#include "Act65TYPEDEF.h"
#include "Act65TypeDotField.h"
#include "Act65TypeFIELDS.h"
#include "Act65TypeFIELD.h"
#include "Act65TypeFieldsEND.h"
#include "Act65TYPEname.h"
#include "Act65UnaryNEG.h"
#include "Act65UNTILL.h"
#include "Act65UpperPart.h"
#include "Act65VarGlobal.h"
#include "Act65VarLocal.h"
#include "Act65VarParameter.h"
#include "Act65VECTOR.h"
#include "Act65W65C02.h"
#include "Act65W65C816.h"
#include "Act65WHILE.h"
#include "Act65XOR.h"
#include "Act65XREG.h"
#include "Act65YREG.h"
//-------------------------------------
// Stack Class
//-------------------------------------
#include "StackItem.h"
#include "StackNodeItem.h"
#include "StackDOODItem.h"
#include "Stack.h"
//-------------------------------------
// Abstract Syntax Tree Class
//-------------------------------------
#include "AstTree.h"
#include "ActionAstTree.h"

#include "Instruction.h"
#include "ChainInstructionItem.h"
#include "Parser.h"
#include "ActionApp.h"

extern Exception ThrownException;


#endif // ! PCH__H

