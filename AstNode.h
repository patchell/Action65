#pragma once

class CParser;

class CAstNode
{
public:
	enum class NodeType {
		BASE,
		AREG,
		ADDEXPR,
		ADDRESS_OF,
		ADDRESS_OF_INTERRUPT,
		ADDRESS_OF_CONSTANT,
		ARRAY,
		ARRAY_INDEX,
		ARRAY_DIM,
		ASM,
		ASM_PROC,
		ASM_STATEMENT,
		ASSIGN_ADD,
		ASSIGN_AND,
		ASSIGN_DIV,
		ASSIGN_LSH,
		ASSIGN_EQ,
		ASSIGN_MOD,
		ASSIGN_MULT,
		ASSIGN_OR,
		ASSIGN_RSH,
		ASSIGN_SUB,
		ASSIGN_XOR,
		BEGIN,
		BIT,
		BITS,
		BITWISE_AND,
		BITWISE_OR,
		BODY,
		BOOL,
		BREAK,
		BYTE,
		CARD,
		CHAR,
		CHAR_CONSTANT,
		CODE_BLOCK,
		CONSTANT,
		CONSTANT_LIST,
		CURRENT_LOCATION,
		DEFINE_ACTION_STRING,
		DEFINE_BYTE,
		DEFINE_C_STRING,
		DECLARE_PARAMS,
		DEFINE,
		DEFINE_OBJECT,
		DEFINE_LIST,
		DIVIDE,
		DEFINE_LONG,
		DO,
		DEFINE_STORAGE,
		DEFINE_WORD,
		CONDITIONAL,
		ELSE,
		ELSEIF,
		END,
		EPROC,
		EQUAL_TO,
		EXIT,
		FFI,
		FI,
		CARRY_FLAG,
		NEG_FLAG,
		OVERFLOW_FLAG,
		ZERO_FLAG,
		FOR,
		FOR_ITTERATOR,
		FOR_STEP,
		FOR_TO,
		FUNCTION,
		FUNCTION_ADDRESS,
		FUNCTION_CALL,
		GREATER_THAN,
		GREATER_THAN_OR_EQUAL,
		IDENT,
		IDENTIFIER,
		IDENT_LIST,
		IDENT_ADDRESS,
		IF,
		IFF,
		INIT_DATA,
		INT,
		INTERRUPT,
		LABEL,
		LESS_THAN,
		LOCAL_VARIABLE,
		LOGICAL_AND,
		LOGICAL_OR,
		LOWER_PART,
		LSH,
		LESS_THAN_OR_EQUAL,
		MOD,
		MODULE,
		MULTIPLY,
		NOT_EQUAL_TO,
		NUMBER,
		OD,
		OPCODE,
		OPTIONAL_NOT,
		ORG,
		PARAMETER_LIST,
		POINTER,
		POINTER_DEREFERENCE,
		POP,
		POP_DESTINATION,
		PRIVATE,
		PROC,
		PROC_ADDRESS,
		PROC_CALL,
		PROCESSOR,
		PROC_NAME,
		PROC_PARAMETERS,
		PROCESSOR_STATUS_REGISTER,
		PUSH,
		PUSH_SOURCE,
		R6502,
		RECORD_TYPE,
		RETURN,
		ROL,
		ROOT,
		ROR,
		RSH,
		RTI,
		SECTION_ATTRIBUTES,
		SECTION_ATTRIBUTE_FALSE,
		SECTION_ATTRIBUTE_MODE,
		SECTION_ATRRIBUTE_READ_ONLY,
		SECTION_ATTRIBUTE_READ_WRITE,
		SECTION_ATTRIBUTE_SIZE,
		SECTION_ATTRIBUTE_START,
		SECTION_ATTRIBUTE_PAGE_ZERO,
		SECTION_NAME,
		SECTION,
		STATEMENTS,
		STATUS_FLAGS,
		STRING,
		SUBTRACT,
		THEN,
		TRUE,
		TYPE,
		TYPEDEF,
		TYPED_DOT_FIELD,
		TYPE_FIELDS,
		TYPE_FIELDS_END,
		TYPE_NAME,
		UNARY_NEG,
		UNTIL,
		UPPER_PART,
		VALUE,
		VECTOR,
		W65C02,
		W65C816,
		WHILE,
		XOR,
		XREG,
		YREG,
		SET
	};
private:
	inline static int m_NodeCount = 0;
	int m_NodeID;
	NodeType m_NodeType;
	const char* m_pNodeName;
	//------ Links/ Lists
	CAstNode* m_pStart;
	CAstNode* m_pNext;	//next node in chain
	CAstNode* m_pPrev;
	CAstNode* m_pHead;
	CAstNode* m_pTail;
	CAstNode* m_pChild;	// Child node
	CAstNode* m_pParent;
	//---------- Value ---------------
	CValue* m_pValue;
	CSection* m_pSection;
	int m_Line;
	int m_Column;
public:
	CAstNode();
	CAstNode(const char* pName, NodeType NT);
	virtual ~CAstNode();
	virtual CAstNode* MakeNode(
		CAstNode* pChild = 0,
		CAstNode* pNext = 0
	);
	virtual void CreateValue(CBin* pSym);
	virtual void CreateValue(const char* s);
	virtual CAstNode* CreateValue(int V);
	virtual CValue* Process();
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual int Print(int Indent, char* s, int strLen, bool* pbNextFlag);
	virtual bool IsLabel() { return false; }
	// Getter/Setter Methods
	CAstNode* GetHead() { return m_pHead; }
	CAstNode* GetTail() { return m_pTail; }
	CAstNode* GetStart() { return m_pStart; }
	CAstNode* GetNext() { return m_pNext; }
	CAstNode* GetPrev() { return m_pPrev; }
	CAstNode* GetChild() { return m_pChild; } 
	CAstNode* GetParent() { return m_pParent; }
	CSection* GetSection() { return m_pSection; }
	void SetHead(CAstNode* pAN) { m_pHead = pAN; }
	void SetTail(CAstNode* pAN) { m_pTail = pAN; }
	void SetStart(CAstNode* pAN) { m_pStart = pAN; }
	void SetNext(CAstNode* pAN) { m_pNext = pAN; }
	void SetPrev(CAstNode* pAN) { m_pPrev = pAN; }
	CAstNode* SetChild(CAstNode* pAN);
	void SetSection(CSection* pSec) { m_pSection = pSec; }
	//	void SetChild(CAstNode* pAN) { m_pChild = pAN; }
	void SetParent(CAstNode* pAN) { m_pParent = pAN; }
	//---------------------------------------------------
	int GetID() const { return m_NodeID; }
	void SetID(int Id) { m_NodeID = Id; }
	NodeType GetNodeType() const { return m_NodeType; }
	virtual const char* GetNodeName() { return 0; }
	bool SetSymbol(CBin* pSym) {
		bool rV = true;

		if (m_pValue)
			m_pValue->SetSymbol(pSym);
		else
		{
			CreateValue(pSym);
		}
		return rV;
	}
	CBin* GetSymbol() { 
		CBin* pSym = 0;

		if (m_pValue)
			pSym = m_pValue->GetSymbol();
		return pSym; 
	}
	void SetValue(CValue* pV) { 
		m_pValue = pV; 
	}
	CValue* GetValue() {
		return m_pValue;
	}
	static CAstNode* MakeNextList(CAstNode* pList, CAstNode* pListMember);
	static CAstNode* MakeChildList(
		CAstNode* pList, 
		CAstNode* pChild
	);
	CAstNode* GetLastNext();
	int MakeIndentString(char* s, int size, int Indent, bool* pbNextFlag);
	int GetLine() const { return m_Line; }
	int GetColumn() const { return m_Column; }
	CParser* GetParser();
	FILE* LogFile();
};
