#pragma once

constexpr auto SYMBOL_SCOPE_ANY = 0;
constexpr auto SYMBOL_SCOPE_GLOBAL = 1;
constexpr auto SYMBOL_SCOPE_LOCAKL = 2;

class CSection;

class CSymbol: public CBin
{
	USHORT m_Address;
	LONG m_Value;
	int m_Scope;
	CSection* m_pSection;
	IdentType m_IdentType;
public:
	CSymbol() {
		m_Address = 0;
		m_Value = 0;
		m_Scope = SYMBOL_SCOPE_ANY;
		m_pSection = 0;
		m_IdentType = IdentType::NEW_SYMBOL;
	}
	virtual ~CSymbol() {}
	BOOL Create() { return TRUE; }
	virtual BOOL Compare(const char* name, int scope);
	virtual void Print(FILE* pOut, const char* s);
	//-----------------------------
	// Accessor Methods
	//-----------------------------
	USHORT GetAddress() { return m_Address; }
	void SetAddress(USHORT A) { m_Address = A; }
	LONG GetValue() { return m_Value; }
	void SetValue(LONG v) { m_Value = v; }
	int GetScope() { return m_Scope; }
	void SetScope(int S) { m_Scope = S; }
	CSection* GetSection() { return m_pSection; }
	void SetSection(CSection* pS) { m_pSection = pS; }
	void SetIdentType(IdentType IT) { m_IdentType = IT; }
	IdentType GetIdentType() { return m_IdentType; }
};

