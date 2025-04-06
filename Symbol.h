#pragma once

constexpr auto SYMBOL_SCOPE_ANY = 0;
constexpr auto SYMBOL_SCOPE_GLOBAL = 1;
constexpr auto SYMBOL_SCOPE_LOCAKL = 2;

class CSection;

class CSymbol: public CBin
{
	unsigned m_Address;		//physical address the symbol represents
	int m_Scope;
	CSection* m_pSection;	//Memory Section where symbol is defined
	bool m_UnResolved;	//Unresolved/Not Defined
	//---------------------------
	// Type Chain
	//---------------------------
	CTypeChain* m_pTypeChain;	//Defines the Type of the Symbol
	//---------------------------
	// Parameter chain for a
	// function/proceedure
	// Symbol
	// Or:
	// Chain of data members for
	// a user defined TYPE
	//---------------------------
	CParameterChain* m_pParamChain;
	CParameterChain* m_pTypeDefChain;
	CChain* m_pLocalVariables;
public:
	CSymbol();
	virtual ~CSymbol() {}
	bool Create() { return true; }
	virtual bool Compare(const char* name, int scope);
	virtual void Print(FILE* pOut, const char* s);
	virtual int Print(char* pSO,int l, const char* s = 0);
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
	CParameterChain* GetTypeDefChain() {
		return m_pTypeDefChain;
	}
	void CreateTypeDefChain() {
		m_pTypeDefChain = new CParameterChain;
		m_pTypeDefChain->Create();
	}
	virtual unsigned GetAddress() const { return m_Address; }
	void SetAddress(unsigned A) { 
		m_Address = A; 
		SetResolved();
		BackFillUnresolved();
	}
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
//	void SetTypeChain(CTypeChain* pTC) { m_pTypeChain = pTC; }
	virtual	void CreateTypeChain(CTypeChain* pTC = 0);
	//---------------------------------------
	// Manage List of Local Variables/Labels
	//---------------------------------------
	CChain* GetLocalVars() { return m_pLocalVariables; }
	void CreateLocalVars() {
		m_pLocalVariables = new CChain;
	}
};

