#pragma once

class CBin
{
	int m_ID;
	inline static int m_IDcount = 0;
public:
	enum class IdentType {
		NONE,
		NEW_SYMBOL,
		LABEL_GLOBAL,
		LABEL_PRIVATE,
		LABEL,
		PROC,
		FUNC,
		IRQPROC,
		GLOBAL,
		LOCAL,
		PARAMETER,
		SECTION,
		TYPE_DEF
	};
private:
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
	inline static CIdentType IdentTypeLUT[] = {
		{IdentType::NONE , "NONE"},
		{IdentType::NEW_SYMBOL , "NEW SYMBOL"},
		{IdentType::LABEL_GLOBAL , "Global LABEL"},
		{IdentType::LABEL_PRIVATE , "Private LABEL"},
		{IdentType::LABEL , "Unresolved LABEL"},
		{IdentType::PROC , "PROC"},
		{IdentType::FUNC , "FUNC"},
		{IdentType::GLOBAL , "GLOBAL"},
		{IdentType::LOCAL , "LOCAL"},
		{IdentType::SECTION , "SECTION"}
	};
public:
	enum BinType {
		NONE,
		SYMBOL,
		SECTION,
		WHERE_USED,
	};
private:
	//-------------------------------
	// Chain Links
	//-------------------------------
	CBin* m_pNext;
	CBin* m_pPrev;
	//-------------------------------
	// Sub List Chain
	//-------------------------------
	CBin* m_pHead;
	CBin* m_pTail;
	//-------------------------------
	// Symbol Properties
	//-------------------------------
	char* m_pName;
	BinType m_Type;
	Token m_Token;
	IdentType m_IdentType;
public:
	CBin() {
		m_pHead = 0;
		m_pTail = 0;
		m_Token = Token(0);
		m_pNext = 0;
		m_pPrev = 0;
		m_pName = 0;
		m_Type = BinType::NONE;
		m_Token = Token::NONE;
		m_IdentType = IdentType::NEW_SYMBOL;
		m_ID = ++m_IDcount;
	}
	CBin(BinType type) {
		m_pName = 0;
		m_pNext = 0;
		m_pPrev = 0;
		m_pHead = 0;
		m_pTail = 0;
		m_Token = Token::NONE;
		m_Type = type;
		m_IdentType = IdentType::NEW_SYMBOL;
		m_ID = ++m_IDcount;
	}
	virtual ~CBin() {
		if (m_pName)
			delete[]m_pName;
	};
	bool Create() { return true; }
	CBin* GetNext() { return m_pNext; }
	void SetNext(CBin* pB) { m_pNext = pB; }
	CBin* GetPrev() { return m_pPrev; }
	void SetPrev(CBin* pB) { m_pPrev = pB; }
	virtual void SetIdentType(IdentType IT) { m_IdentType = IT; }
	virtual IdentType GetIdentType() { return m_IdentType; }
	virtual bool IsNewSymbol() {
		bool rV = false;

		if (m_IdentType == IdentType::NEW_SYMBOL)
			rV = true;
		return rV;
	}

	virtual char* GetName(void) { return m_pName; }
	 virtual void SetName(const char* pName) {
		int l = (int)strlen(pName) + 1;
		if (m_pName) 
			delete[] m_pName;
		m_pName = new char[l];
		strcpy_s(m_pName, l, pName);
	}
	 virtual bool Compare(const char *name, int aux = 0) = 0;
	 BinType GetType() const { return m_Type; }
	 virtual unsigned GetAddress() const { return 0; }
	 virtual void SetAddress(unsigned A) {}
	 //	virtual bool Compare(const char* name) {
//		bool rV = false;
//		if (strcmp(m_pName, name) == 0) rV = true;
//		return rV;
	//}
	 void SetToken(Token t) { m_Token = t; }
	 Token GetToken() const { return m_Token; }
	 virtual void Print(FILE* pOut, const char* s = 0);
	 //------------------------------------------
	 // SubList Methods
	 //------------------------------------------
	 void Add(CBin* pSym);
	 CBin* Find(const char* pName, CBin::BinType Type);
	 void Delete(CBin* pSym);
	 void Unlink(CBin* pSym);
	 void SetHead(CBin* pH) { m_pHead = pH; }
	 CBin* GetHead() { return m_pHead; }
	 void SetTail(CBin* pT) { m_pTail = pT; }
	 CBin* GetTail() { return m_pTail; }
	 int GetID() const { return m_ID; }
};

