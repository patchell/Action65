#pragma once

constexpr auto VALUE_LUT_DIM = 3;

class CValue
{
public:
	enum class ValueType {
		NONE,
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
		{ValueType::CONSTANT,"CONSTANT"},
		{ValueType::SYMBOL,"SYMBOL"}
	};
public:
private:
	CBin* m_pSym;
	int m_ConstantValue;
	ValueType m_ValType;
public:
	CValue();
	virtual ~CValue();
	bool Create(CBin* pSym = 0);
	void SetSymbol(CBin* pSym);
	CBin* GetSymbol() { return m_pSym; }
	int GetConstVal();
	void SetConstVal(int v) {
		m_ConstantValue = v;
		m_ValType = ValueType::CONSTANT;
	}
	ValueType GetValueType() { return m_ValType; }
};

