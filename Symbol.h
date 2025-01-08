#pragma once

constexpr auto SYMBOL_SCOPE_ANY = 0;
constexpr auto SYMBOL_SCOPE_GLOBAL = 1;
constexpr auto SYMBOL_SCOPE_LOCAKL = 2;

class CSection;

class CSymbol: public CBin
{
	unsigned m_Address;
	int m_Value;
	int m_Scope;
	CSection* m_pSection;
	bool m_UnResolved;
	class CIdentType {
		IdentType m_Type;
		const char* m_pName;
	public:
		CIdentType() {
			m_Type = IdentType(0);
			m_pName = 0;
		}
		CIdentType(IdentType IT, const char* pN) {
			m_Type = IT;
			m_pName = pN;
		}
		const char* LookupIdentType(IdentType IT);
	};
	inline static CSymbol::CIdentType IdentTypeLUT[] = {
		{IdentType::NEW_SYMBOL , "NEW SYMBOL"},
		{IdentType::LABEL , "LABEL"},
		{IdentType::PROC , "PROC"},
		{IdentType::FUNC , "FUNC"},
		{IdentType::GLOBAL , "GLOBAL"},
		{IdentType::LOCAL , "LOCAL"},
		{IdentType::SECTION , "SECTION"}
	};
	//---------------------------
	// Type Chain
	//---------------------------
	CTypeChain* m_pTypeChain;
	CParameterChain* m_pParamChain;
public:
	CSymbol() {
		m_Address = 0;
		m_Value = 0;
		m_Scope = SYMBOL_SCOPE_ANY;
		m_pSection = 0;
		m_UnResolved = true;
		m_pTypeChain = 0;
		m_pParamChain = 0;
	}
	virtual ~CSymbol() {}
	bool Create() { return true; }
	virtual bool Compare(const char* name, int scope);
	virtual void Print(FILE* pOut, const char* s);
	virtual int Print(char* pSO,int l, const char* s);
	//-----------------------------
	// Accessor Methods
	//-----------------------------
	CParameterChain* GetParamChain() {
		return m_pParamChain
			;
	}
	void CreateParamChain() {
		m_pParamChain = new CParameterChain;
		m_pParamChain->Create();
	}
	unsigned GetAddress() const { return m_Address; }
	void SetAddress(unsigned A) { m_Address = A; }
	int GetValue() const { return m_Value; }
	void SetValue(int v) { m_Value = v; }
	int GetScope() const { return m_Scope; }
	void SetScope(int S) { m_Scope = S; }
	CSection* GetSection() { return m_pSection; }
	void SetSection(CSection* pS) { m_pSection = pS; }
	bool IsUnResolved() const {
		return m_UnResolved;
	}
	bool IsResolved() const {
		return !m_UnResolved;
	}
	void SetResolved() { m_UnResolved = false; }
	void SetUnResolved() { m_UnResolved = true; }
	//-------------------------------------------------
	// Backfill unresolved references
	//-------------------------------------------------
	void BackFillUnresolved();
	//-----------------------------------
	// Type chain methods
	//-----------------------------------
	CTypeChain* GetTypeChain() { return m_pTypeChain; }
	void SetTypeChain(CTypeChain* pTC) { m_pTypeChain = pTC; }
};

