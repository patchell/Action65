#pragma once

constexpr auto VALUE_LUT_DIM = 4;

class CValue
{
public:
	enum class ValueType {
		NONE,
		INSTRUCTION,
		CONSTANT,
		SYMBOL
	};
	struct CValueTypeItem {
		ValueType m_Type;
		const char* m_pName;
		CValueTypeItem(ValueType Vt, const char* pN) {
			m_Type = Vt;
			m_pName = pN;
		}
	};
private:
	inline static CValueTypeItem ValTypeLUT[VALUE_LUT_DIM] = {
		{ValueType::NONE,"NONE"},
		{ValueType::INSTRUCTION,"INSTRUCTION"},
		{ValueType::CONSTANT,"CONSTANT"},
		{ValueType::SYMBOL,"SYMBOL"}
	};
public:
	class AsmInstruction
	{
		int m_OpCode;
		int m_Operand;
		AdrModeType m_AdrType;
		int m_ByteCount;
		const char* m_pOpString;
		CSymbol* m_pOprndSym;
		KeyWord* m_pKeyWord;
	public:
		AsmInstruction() {
			m_OpCode = 0;
			m_Operand = 0;
			m_AdrType = AdrModeType::NA;
			m_ByteCount = 0;
			m_pOpString = 0;
			m_pOprndSym = 0;
			m_pOprndSym = 0;
			m_pOprndSym = 0;
			m_pKeyWord = 0;
		}
		virtual ~AsmInstruction() {}
		bool Create();
		int GetOpCode() const { return m_OpCode; }
		void SetOpCode(int Op) { m_OpCode = Op; }
		int GetOperand() const { return m_Operand; }
		void SetOperand(int Oprnd) { m_Operand = Oprnd; }
		AdrModeType GetAdrModeType() const { return m_AdrType; }
		void SetAdrModeType(AdrModeType AMT) { m_AdrType = AMT; }
		int GetByteCount() const { return m_ByteCount; }
		void SetByteCount(int BC) { m_ByteCount = BC; }
		const char* GetOpString() { return m_pOpString; }
		void SetOpString(const char* pOS) { m_pOpString = pOS; }
		CSymbol* GetOprndSym() { return m_pOprndSym; }
		void SetOprndSym(CSymbol* pSym) { m_pOprndSym = pSym; }
		KeyWord* GetKeyWord() { return m_pKeyWord; }
		void SetKeyWord(KeyWord* pKW) { m_pKeyWord = pKW; }
	};
private:
	AsmInstruction m_Instruction;
	CBin* m_pSym;
	int m_ConstantValue;
public:
	CValue();
	virtual ~CValue();
	bool Create(CBin* pSym = 0);
	AsmInstruction* GetAsmInst() { return &m_Instruction; }
	void SetSymbol(CBin* pSym) { m_pSym = pSym; }
	CBin* GetSymbol() { return m_pSym; }
	int GetConstVal() { return m_ConstantValue; }
	void SetConstVal(int v) {
		m_ConstantValue = v;
	}
};

