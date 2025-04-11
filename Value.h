#pragma once

constexpr auto VALUE_LUT_DIM = 12;

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
		AREG,
		XREG,
		YREG,
		SREG,
		PREG,
		VIRTUAL_REGISTER,
		STRING,
		CSTRING,	// "C" string
		ASTRING		// ACTION! String
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
		{ ValueType::AREG,"REGISTER" },
		{ ValueType::XREG,"REGISTER" },
		{ ValueType::YREG,"REGISTER" },
		{ ValueType::SREG,"REGISTER" },
		{ ValueType::PREG,"REGISTER" },
		{ ValueType::VIRTUAL_REGISTER,"VIRTUAL REGISTER" },
		{ ValueType::STRING,"STRING" },
		{ ValueType::CSTRING,"C STRING" },
		{ ValueType::ASTRING,"Act STRING" }
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
	char* m_pString;
	ValueType m_ValType;
	UpperLower m_UpperLOwer;
	CReg m_Reg;
	CTypeChain m_TypeChain;
public:
	CValue();
	virtual ~CValue();
	bool Create(const char* s);
	bool Create(CSymbol* pSym = 0);
	bool Create(ValueType VT);
	bool Create(int V);
	void SetSymbol(CSymbol* pSym);
	CSymbol* GetSymbol() { return m_pSym; }
	CTypeChain* GetTypeChain() { return &m_TypeChain; }
	char* GetName();
	int GetConstVal();
	void SetConstVal(int v) {
		m_ConstantValue = v;
		m_ValType = ValueType::CONSTANT;
	}
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
	//-------------------------------------------
	// Code Generation
	//-------------------------------------------
	CValue* BinaryOp(BinaryOps Op, CValue* pOperand);
	CValue* Addition(CValue* pOperand);
	CValue* Subtraction(CValue* pOperand);
	CValue* Mul(CValue* pOperand);
	CValue* Div(CValue* pOperand);
	CValue* Mod(CValue* pOperand);
	CValue* And(CValue* pOperand);
	CValue* Or(CValue* pOperand);
	CValue* Xor(CValue* pOperand);
	CValue* LSH(CValue* pOperand);
	CValue* RSH(CValue* pOperand);
	CValue* Assign(BinaryOps Op, CValue* pValue);
	CValue* Assign(CValue* pValue);
	int SizeOf();
};

