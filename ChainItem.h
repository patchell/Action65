#pragma once

class CChainItem
{
public:
	enum class ChainItemType {
		NONE,
		INSTRUCTION,
		VALUE,
		BIN,
		PARAMETER,
		TYPE,
		SYMBOL_USED,
		LOCAL_SYMBOL
	};
	struct ItemType {
		ChainItemType m_Type;
		const char* m_pName;
		ItemType() {
			m_Type = ChainItemType::NONE;
			m_pName = 0;
		}
		ItemType(ChainItemType T, const char* pN) {
			m_Type = T;
			m_pName = pN;
		}
		bool IsType(ChainItemType T)  const{
			return m_Type == T;
		}
	};
	static inline ItemType ItemTypeLUT[] = {
		{ChainItemType::NONE,"NONE"},
		{ChainItemType::INSTRUCTION,"INSTRUCTION"},
		{ChainItemType::VALUE,"VALUE"},
		{ChainItemType::BIN,"BIN"},
		{ChainItemType::PARAMETER,"PARAMETER"},
		{ChainItemType::TYPE,"TYPE"},
		{ChainItemType::SYMBOL_USED,"SYMBOL_USED"},
		{ChainItemType::LOCAL_SYMBOL,"LOCAL_SYMBOL"}
	};
private:
	ChainItemType m_ItemType;
	CChainItem* m_pNext;
	CChainItem* m_pPrev;
public:
	CChainItem();
	CChainItem(ChainItemType T) {
		m_ItemType = T;
		m_pNext = 0;
		m_pPrev = 0;
	}
	virtual ~CChainItem();
	virtual bool Create() { return true; }
	virtual void Copy(CChainItem* pI) = 0;
	virtual bool Is(ChainItemType T) const {
		return m_ItemType == T;
	}
	virtual bool Compare(const char* pName) = 0;
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
	CChainItem* GetNext() { return m_pNext; }
	CChainItem* GetPrev() { return m_pPrev; }
	void SetNext(CChainItem* pN) { m_pNext = pN; }
	void SetPrev(CChainItem* pP) { m_pPrev = pP; }
	ChainItemType GetItemType() const { return m_ItemType; }
};

