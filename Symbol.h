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
	IdentType m_IdentType;
	bool m_UnResolved;
	//---------------------------
	// Type Chain
	//---------------------------
	CObjType* m_pTcHead;
	CObjType* m_pTcTail;
public:
	CSymbol() {
		m_Address = 0;
		m_Value = 0;
		m_Scope = SYMBOL_SCOPE_ANY;
		m_pSection = 0;
		m_IdentType = IdentType::NEW_SYMBOL;
		m_UnResolved = true;
		m_pTcHead = 0;
		m_pTcTail = 0;
	}
	virtual ~CSymbol() {}
	bool Create() { return true; }
	virtual bool Compare(const char* name, int scope);
	virtual void Print(FILE* pOut, const char* s);
	//-----------------------------
	// Accessor Methods
	//-----------------------------
	unsigned GetAddress() { return m_Address; }
	void SetAddress(unsigned A) { m_Address = A; }
	int GetValue() { return m_Value; }
	void SetValue(int v) { m_Value = v; }
	int GetScope() { return m_Scope; }
	void SetScope(int S) { m_Scope = S; }
	CSection* GetSection() { return m_pSection; }
	void SetSection(CSection* pS) { m_pSection = pS; }
	void SetIdentType(IdentType IT) { m_IdentType = IT; }
	IdentType GetIdentType() { return m_IdentType; }
	bool IsUnResolved() {
		return m_UnResolved;
	}
	bool IsResolved() {
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
	void AddAtHead(CObjType* pObjType);
	void AddAtTail(CObjType* pObjType);
	void Delete(CObjType* pObjType);
	void Unlink(CObjType* pObjType);
	void SetTcHead(CObjType* pOT) { m_pTcHead = pOT; }
	CObjType* GetTcHead() { return m_pTcHead; }
	void SetTcTail(CObjType* pOT) { m_pTcTail = pOT; }
	CObjType* GetTcTail() { return m_pTcTail; }

};

