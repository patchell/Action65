#pragma once

class CChain;

class CBin
{
	int m_ID;
	inline static int m_IDcount = 0;
public:
	enum class IdentType {
		ERROR = -1,
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
		static const char* LookupIdentType(IdentType IT);
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
		{IdentType::SECTION , "SECTION"},
		{IdentType::TYPE_DEF , "TYPE DEF"},
		{IdentType::PARAMETER , "PARAMETER"},
		{IdentType::IRQPROC , "IRQPROC"},
		{IdentType::ERROR, 0}
	};
public:
	enum BinType {
		NONE,
		ANY,
		SYMBOL,
		SECTION,
		MACRO
	};
	struct BinTypeItem
	{
		BinType m_Type;
		const char* m_pName;
		BinTypeItem() {
			m_Type = BinType::NONE;
			m_pName = 0;
		}
		BinTypeItem(BinType Type, const char* pName) {
			m_Type = Type;
			m_pName = pName;
		}
	};
private:
	inline static BinTypeItem BinTypeLUT[] = {
		{BinType::NONE,"NONE"},
		{BinType::ANY,"ANY"},
		{BinType::SYMBOL,"SYMBOL"},
		{BinType::SECTION,"SECTION"},
		{BinType::MACRO,"MACRO"},
		{BinType(-1),0}
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
	// Chain List for where symbol
	// is used
	//-------------------------------
	CChain m_WhereUsed;
	//-------------------------------
	// Symbol Properties
	//-------------------------------
	int m_Scope;
	char* m_pName;
	BinType m_Type;
	Token m_Token;
	IdentType m_IdentType;
public:
	CBin() {
		m_Scope = 0;
		m_pHead = 0;
		m_pTail = 0;
		m_Token = Token(0);
		m_pNext = 0;
		m_pPrev = 0;
		m_pName = 0;
		m_Type = BinType::NONE;
		m_IdentType = IdentType::NEW_SYMBOL;
		m_ID = ++m_IDcount;
	}
	CBin(BinType type) {
		m_Scope = 0;
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
	//----------- Compare Method -------------
	// Used in searching table
	//----------------------------------------
	virtual bool Compare(const char* name, BinType Type = BinType::ANY, int aux = 0) = 0;
	//---------------------------------------
	BinType GetType() const { return m_Type; }
	virtual unsigned GetAddress() const { return 0; }
	virtual void SetAddress(unsigned A) {}
	void SetToken(Token t) { m_Token = t; }
	Token GetToken() const { return m_Token; }
	virtual int Print(char* pSO, int l, int Indent, const char* s) = 0;
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
	CChain* GetWhereUsed();
	const char* GetIdentTypeString(IdentType IT) const {

		const char* prStr = 0;

		prStr = CIdentType::LookupIdentType(IT);
		return prStr;
	}
	static const char* GetBinTypeString(BinType T) {
		const char* prStr = 0;

		for (int i = 0; BinTypeLUT[i].m_pName && !prStr; ++i)
		{
			if (BinTypeLUT[i].m_Type == T)
				prStr = BinTypeLUT[i].m_pName;
		}
		return prStr;
	}
	int GetScope() const { return m_Scope; }
	void SetScope(int S) { m_Scope = S; }
};

