#pragma once

constexpr auto VALUE_LUT_DIM = 4;

class CValue
{
public:
	enum class ValueType {
		NONE,
		CONSTANT,
		SYMBOL,
		STRING
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
		UPPER,
		LOWER
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
		{ ValueType::STRING,"STRING" }
	};
	inline static UpperLowerItem UpperLowerLUT[3] = {
		{UpperLower::NONE,"NONE"},
		{UpperLower::UPPER,"UPPER"},
		{UpperLower::LOWER,"LOWER"},
	};

	CBin* m_pSym;
	int m_ConstantValue;
	char* m_pString;
	ValueType m_ValType;
	UpperLower m_UpperLOwer;
public:
	CValue();
	virtual ~CValue();
	bool Create(const char* s);
	bool Create(CBin* pSym = 0);
	bool Create(ValueType VT);
	bool Create(int V);
	void SetSymbol(CBin* pSym);
	CBin* GetSymbol() { return m_pSym; }
	int GetConstVal();
	void SetConstVal(int v) {
		m_ConstantValue = v;
		m_ValType = ValueType::CONSTANT;
	}
	ValueType GetValueType() const { return m_ValType; }
	void SetString(const char* s);
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
};

