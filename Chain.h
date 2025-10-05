#pragma once

class CChain
{
public:
	enum class ChainType {
		NONE,
		CHAINMISC,
		CHAINTYPE,
		ERROR
	};
	struct TypeItem
	{
		ChainType m_Type;
		const char* m_pName;
		TypeItem() {
			m_Type = ChainType::NONE;
			m_pName = 0;
		}
		TypeItem(ChainType T, const char* pN) {
			m_Type = T;
			m_pName = pN;
		}
		bool IsType(ChainType T)  const {
			return m_Type == T;
		}
	};
private:
	static inline TypeItem TypeItemLUT[] = {
		{ChainType::NONE,"NONE"},
		{ChainType::CHAINMISC,"CHAINMISC"},
		{ChainType::CHAINTYPE,"CHAINTYPE"},
		{ChainType::ERROR,nullptr}
	};
	ChainType m_ChainType;
	CChainItem* m_pHead;
	CChainItem* m_pTail;
public:
	CChain();
	CChain(ChainType T);
	virtual ~CChain();
	bool Create();
	virtual void Copy(CChain* pC = 0);
	void AddToTail(CChainItem* pItem);
	void AddToHead(CChainItem* pItem);
	void Unlink(CChainItem* pItem);
	CChainItem* FindItem(const char* pName);
	CChainItem* GetHead() { return m_pHead; }
	CChainItem* GetTail() { return m_pTail; }
	void SetHead(CChainItem* pH) { m_pHead = pH; }
	void SetTail(CChainItem* pT) { m_pTail = pT; }
	ChainType LookupType(const char* pName);
	const char* FindTypeName(ChainType T) const;
	virtual int Print(char* pSO, int l, int Indent = 0,  const char* s = 0);
};

