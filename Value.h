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
private:
	inline static CValueTypeItem ValTypeLUT[VALUE_LUT_DIM] = {
		{ValueType::NONE,"NONE"},
		{ValueType::CONSTANT,"CONSTANT"},
		{ValueType::SYMBOL,"SYMBOL"},
		{ ValueType::STRING,"STRING" }
	};
private:
	CBin* m_pSym;
	int m_ConstantValue;
	char* m_pString;
	ValueType m_ValType;
public:
	CValue();
	virtual ~CValue();
	bool Create(const char* s);
	bool Create(CBin* pSym = 0);
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
};

