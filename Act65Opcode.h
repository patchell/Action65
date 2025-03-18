#pragma once


class CAct65Opcode : public CAstNode
{
	struct OperandByteCount {
		AdrModeType m_AdrMode;
		int m_OperandByteCount;
		OperandByteCount() {
			m_AdrMode = AdrModeType::NA;
			m_OperandByteCount = 0;
		}
		OperandByteCount(AdrModeType AMT, int Cnt)
		{
			m_AdrMode = AMT;
			m_OperandByteCount = Cnt;
		}
		static int GetOperandByteCount(AdrModeType AdrMode);
	};
	inline static OperandByteCount OperByteCntLUT[] = {
		{AdrModeType::IMPLIED,0},
		{AdrModeType::INDEXED_INDIRECT_X_ADR,1},
		{AdrModeType::ZERO_PAGE_ADR,1},
		{AdrModeType::IMMEDIATE_ADR,1},
		{AdrModeType::ABSOLUTE_ADR,2},
		{AdrModeType::INDIRECT_INDEXED_Y_ADR,1},
		{AdrModeType::ZERO_PAGE_X_ADR,1},
		{AdrModeType::ABSOLUTE_Y_ADR,2},
		{AdrModeType::ABSOLUTE_X_ADR,2},
		{AdrModeType::ZERO_PAGE_Y_ADR,1},
		{AdrModeType::INDIRECT_ADR,2},
		{AdrModeType::ACCUMULATOR,0},
		{AdrModeType::NA,-1}
	};

	inline static const char* m_pNodeTyypeName = "OPCODE";
	//----------- Source Location ------------------
	int m_LineNumber;
	int m_ColumnNumber;
	//------------------------
	Token m_OpcodeToken;
	KeyWord* m_pKeyWord;
	AdrModeType m_AdressMode;
	int m_OpCode;
	int m_ByteCount;
	//-----------------------
	CSymbol* m_pLabel;	// Symbol for instruction location
public:
	CAct65Opcode();
	virtual ~CAct65Opcode();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	//-----------------------------
	// Node Processor
	//-----------------------------
	virtual CValue* Process();
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual void PrepareInstruction(
		Token Tk, 
		AdrModeType AddressMode,
		CValue* pOperandValue_Node,
		CSymbol* pLabelSym = 0
	);
	int SaveInstruction(char* pM);
	//-----------------------------
	// Setter/Getter Methods
	//-----------------------------
	Token GetToken() const { return m_OpcodeToken; }
	void SetToken(Token t) { m_OpcodeToken = t; }
	const char* GetOpcodeName();
	int GetLineNumber() const  { return m_LineNumber; }
	void SetLineNumber(int n) { m_LineNumber = n; }
	int GetColumnNumber() const { return m_ColumnNumber; }
	void SetColumnNumber(int c) { m_ColumnNumber = c; }
	int GetOpCode() const { return m_OpCode; }
	void SetOpCode(int Op) { m_OpCode = Op; }
	CValue* GetOperand() { 
		return CAstNode::GetValue(); 
	}
	void SetOperand(CValue* pOprnd) { 
		CAstNode::SetValue(pOprnd);
	}
	AdrModeType GetAdrModeType() const { return m_AdressMode; }
	void SetAdrModeType(AdrModeType AMT) { m_AdressMode = AMT; }
	int GetByteCount() const { return m_ByteCount; }
	void SetByteCount(int BC) { m_ByteCount = BC; }
	KeyWord* GetKeyWord() { return m_pKeyWord; }
	CSymbol* GetLabelSymbol() { return m_pLabel; }
	void SetLabelSymbol(CSymbol* pSym) { m_pLabel = pSym; }
	int ToString(char* s, int Size);
};
