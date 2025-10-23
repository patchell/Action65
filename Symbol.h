#pragma once

constexpr auto SYMBOL_SCOPE_ANY = 0;
constexpr auto SYMBOL_SCOPE_GLOBAL = 1;
constexpr auto SYMBOL_SCOPE_LOCAKL = 2;

class CSection;

class CSymbol: public CBin
{
	unsigned m_Address;		//physical address the symbol represents
	CSection* m_pSection;	//Memory Section where symbol is defined
	bool m_UnResolved;	//Unresolved/Not Defined
	bool m_bDefined;	//The address has been defined has been defined
	//---------------------------
	// Type Chain
	//---------------------------
	CChainTypeSpec* m_pTypeChain;	//Defines the Type of the Symbol
	//---------------------------
	// Parameter chain for a
	// function/proceedure
	// Symbol
	// Or:
	// Chain of data members for
	// a user defined TYPE
	//---------------------------
	CChainParameter* m_pParamChain;
	CChainTypeSpec* m_pTypeDefChain;
	CChainLocals* m_pLocalVariablesChain;
public:
	CSymbol();
	virtual ~CSymbol() {}
	bool Create() { return true; }
	virtual bool Compare(const char* name, BinType Type = BinType::ANY, int aux = 0);
	virtual void Print(FILE* pOut, const char* s);
	virtual int Print(char* pSO,int l, const char* s = 0);
	//-----------------------------
	// Accessor Methods
	//-----------------------------
	CChainParameter* GetParamChain() {
		return m_pParamChain;
	}
	void CreateParamChain() {
		m_pParamChain = new CChainParameter;
		m_pParamChain->Create();
	}
	CChain* GetTypeDefChain() {
		return m_pTypeDefChain;
	}
	void CreateTypeDefChain() {
		m_pTypeDefChain = new CChainTypeSpec;
		m_pTypeDefChain->Create();
	}
	virtual unsigned GetAddress() const { return m_Address; }
	virtual void SetAddress(unsigned A) { 
		m_Address = A; 
		SetResolved();
		BackFillUnresolved();
	}
	CSection* GetSection() { return m_pSection; }
	void SetSection(CSection* pS) { m_pSection = pS; }
	bool IsDefined() const { return m_bDefined && !m_UnResolved; }
	bool IsUnResolved() const {
		return m_UnResolved;
	}
	bool IsResolved() const {
		return !m_UnResolved;
	}
	bool SetDefined() {
		bool rV = false;
		if(IsResolved())
		{
			m_bDefined = true; 
			rV = true;
		}
		return rV;
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
	CChainTypeSpec* GetTypeChain() { return m_pTypeChain; }
//	void SetTypeChain(CChain* pTC) { m_pTypeChain = pTC; }
	virtual	void CreateTypeChain(CChain* pTC = 0);
	//---------------------------------------
	// Manage List of Local Variables/Labels
	//---------------------------------------
	CChainLocals* GetLocalVars() { 
		return m_pLocalVariablesChain;
	}
	void CreateLocalVars() {
		m_pLocalVariablesChain = new CChainLocals	;
	}
};

