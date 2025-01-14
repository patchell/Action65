#pragma once

class CAct65Opcode : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "OP";
	Token m_OpcodeToken;
	const char* m_pOpCodeName;
	int m_LineNumber;
	int m_ColumnNumber;
	CSymbol* m_pSym;	// Symbol for argument
	CSymbol* m_pLabel;	// Symbol for instruction location
	AdrModeType m_AdressMode;
//	CInstruction* m_pInstruction;
public:
	CAct65Opcode();
	virtual ~CAct65Opcode();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	//-----------------------------
	// Node Processor
	//-----------------------------
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
	//-----------------------------
	// Setter/Getter Methods
	//-----------------------------
	Token GetToken() { return m_OpcodeToken; }
	void SetToken(Token t) { m_OpcodeToken = t; }
	const char* GetOpcodeName() { return m_pOpCodeName; }
	void SetOpcodeName(const char* pN) { m_pOpCodeName = pN; }
	int GetLineNumber() { return m_LineNumber; }
	void SetLineNumber(int n) { m_LineNumber = n; }
	int GetColumnNumber() { return m_ColumnNumber; }
	void SetColumnNumber(int c) { m_ColumnNumber = c; }
};

