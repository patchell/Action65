#pragma once

class CSection;

class CChainItem
{
public:
	enum class ChainItemType {
		NONE,
		INSTRUCTION,
		VALUE,
		BIN,
		PARAMETER,
		TYPESPEC,
		SYMBOL_USED,
		LOCAL_SYMBOL,
		DATA,
		STORAGE,
		SUBITEM,
		END
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
		{ChainItemType::TYPESPEC,"TYPE SPEC"},
		{ChainItemType::SYMBOL_USED,"SYMBOL_USED"},
		{ChainItemType::LOCAL_SYMBOL,"LOCAL_SYMBOL"},
		{ChainItemType::DATA,"DATA"},
		{ChainItemType::STORAGE,"STORAGE"},
		{ChainItemType::SUBITEM,"SUBITEM"},
		{ChainItemType::END,0}
	};
private:
	ChainItemType m_ItemType;
	CChainItem* m_pNext;
	CChainItem* m_pPrev;
	int m_ItemDataSize;
public:
	CChainItem();
	CChainItem(ChainItemType T) {
		m_ItemType = T;
		m_pNext = 0;
		m_pPrev = 0;
		m_ItemDataSize = 0;
	}
	virtual ~CChainItem();
	virtual bool Create(int DataSize) { 
		m_ItemDataSize = DataSize; 
		return true; 
	}
	virtual void Copy(CChainItem* pI) = 0;
	virtual bool Is(ChainItemType T) const {
		return m_ItemType == T;
	}
	virtual bool Compare(const char* pName) = 0;
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0) = 0;
	CChainItem* GetNext() { return m_pNext; }
	CChainItem* GetPrev() { return m_pPrev; }
	void SetNext(CChainItem* pN) { m_pNext = pN; }
	void SetPrev(CChainItem* pP) { m_pPrev = pP; }
	ChainItemType GetItemType() const { return m_ItemType; }
	static const char* FindItemTypeName(ChainItemType T) {
		const char* rV = nullptr;

		for (int i = 0; ItemTypeLUT[i].m_pName && !rV; ++i) {
			if (ItemTypeLUT[i].m_Type == T)
				rV = ItemTypeLUT[i].m_pName;
		}
		return rV;
	}
	virtual void Emit(CSection* pSec) = 0;
	virtual void EmitListing(CSection* pSec) = 0;
};

