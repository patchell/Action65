#pragma once

constexpr auto VALUE_LUT_DIM = 9;
class CChainTypeSpec;

class CReg;

class CValue
{
	enum class BinaryOps {
		ADD,
		SUB,
		LSH,
		RSH,
		MOD,
		AND,
		MUL,
		DIV,
		OR,
		XOR,
		CMP
	};
public:
	enum class ValueType {
		NONE,
		CONSTANT,
		SYMBOL,
		REG,
		VIRTUAL_REGISTER,
		STRING,
		CSTRING,	// "C" string
		ASTRING,	// ACTION! String
		ADDRESS_OF	// Address of a Symbol
	};
	struct CValueTypeItem {
		ValueType m_Type;
		const char* m_pName;
		CValueTypeItem(ValueType Vt, const char* pN) {
			m_Type = Vt;
			m_pName = pN;
		}
	};
	enum class UpperLower {
		NONE,
		UPPER,	//upper part of word
		LOWER	// Lower part of word
	};
	struct UpperLowerItem {
		UpperLower m_UL;
		const char* m_pName;
		UpperLowerItem() {
			m_UL = UpperLower::NONE;
			m_pName = 0;
		}
		UpperLowerItem(UpperLower UL, const char* pName) {
			m_UL = UL;
			m_pName = pName;
		}
	};
private:
	inline static CValueTypeItem ValTypeLUT[VALUE_LUT_DIM] = {
		{ValueType::NONE,"NONE"},
		{ValueType::CONSTANT,"CONSTANT"},
		{ValueType::SYMBOL,"SYMBOL"},
		{ ValueType::REG,"REGISTER" },
		{ ValueType::VIRTUAL_REGISTER,"VIRTUAL REGISTER" },
		{ ValueType::STRING,"STRING" },
		{ ValueType::CSTRING,"C STRING" },
		{ ValueType::ASTRING,"Act STRING" },
		{ ValueType::ADDRESS_OF,"Address Of" }
	};
	inline static UpperLowerItem UpperLowerLUT[3] = {
		{UpperLower::NONE,"NONE"},
		{UpperLower::UPPER,"UPPER"},
		{UpperLower::LOWER,"LOWER"},
	};
	CSymbol* m_pSym;
	int m_ConstantValue;	// this value can be:
							// 1. A constant value
							// 2. An offset for a symbol
							// 3. The length of a string
							// A string is store as a
							// C - String, i.e. the
							// last character is '0'
							// which is not included
							// in the length
							// 4, The dimension of an ARRAY
	char* m_pString;
	ValueType m_ValType;
	UpperLower m_UpperLOwer;
	CReg* m_pReg;
	CVirtualReg::VREG* m_pVirtualReg;
	CChainTypeSpec m_AltTypeChain;
public:
	CValue();
	virtual ~CValue();
	bool Create(CVirtualReg::VREG* pVReg);
	bool Create(const char* s);
	bool Create(CSymbol* pSym);
	bool Create(ValueType VT);
	bool Create(int V);
    bool Create(CReg* pReg);
	int Print(char* pSO, int l, int Indent = 0, const char* s = 0);
	void SetSymbol(CSymbol* pSym);
	CReg* GetRegister() { return m_pReg; }
	CVirtualReg::VREG* GetVirtualReg() { return m_pVirtualReg; }
	CSymbol* GetSymbol();
	CChainTypeSpec* GetTypeChain();
	void SetTypeChain(CChain* pTC);
	char* GetName();
	int GetConstVal();
	void SetConstVal(int v) {
		m_ConstantValue = v;
		m_ValType = ValueType::CONSTANT;
	}
	int Inc() { return ++m_ConstantValue; }
	int Dec() { return --m_ConstantValue; }
	int GetTotalValue();
	void SetValueType(ValueType VT) { m_ValType = VT; }
	ValueType GetValueType() const { return m_ValType; }
	void SetString(const char* s, ValueType Type);
	char* GetString() { return m_pString; }
	void Add(CValue* pVal);
	void Sub(CValue* pVal);
	int Upper();
	int Lower();
	bool IsPageZero();
	inline static const char* ValueTypeStr(ValueType VT) {
		return ValTypeLUT[int(VT)].m_pName;
	}
	inline static const char* UpLowStr(UpperLower UL) {
		return UpperLowerLUT[int(UL)].m_pName;
	}
	int SizeOf();
	int SizeOfTypeDef();
	void SetAddress(int Address);
	void SetResolved(bool bRes);
	void BackFillUnresolved();
	CChain* GetTypeTypeDefFieldChain();
};

