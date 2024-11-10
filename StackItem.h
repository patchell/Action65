#pragma once

class CStackItem
{
public:
	enum ItemType {
		NONE,
		SYMBOL,
		SECTION,
		INTVALUE,
		INSTRUCTION,
		REGTYPE,
		ADDRESSSIZE,
		ACCESS_MODE,
		DATA_SIZE,
		BIN,
		NODE
	};
	struct ItemTypeLUT {
		ItemType m_Type;
		const char* m_pName;
		ItemTypeLUT() {
			m_Type = ItemType::NONE;
			m_pName = 0;
		}
		ItemTypeLUT(ItemType T, const char* s) {
			m_Type = T;
			m_pName = s;
		}
	};
private:
	inline static ItemTypeLUT TypeString[] = {
		{ItemType::NONE,"NONE"},
		{ItemType::SYMBOL,"SYMBOL"},
		{ItemType::SECTION,"SECTION"},
		{ItemType::INTVALUE,"INT VALUE"},
		{ItemType::INSTRUCTION,"INSTRUCTION"},
		{ItemType::REGTYPE,"REGTYPE"},
		{ItemType::ADDRESSSIZE,"ADDRESS SIZE"},
		{ItemType::ACCESS_MODE,"ACCESS MODE"},
		{ItemType::DATA_SIZE,"DATA SIZE"},
		{ItemType::BIN,"BIN"},
		{ItemType::NODE,"NODE"},
		{ItemType(-1),NULL}
	};
	ItemType m_Type;
	CStackItem* m_pNext;
public:
	CStackItem();
	virtual ~CStackItem();
	bool Create(ItemType Type);
	void SetNext(CStackItem* pSI) {	m_pNext = pSI; }
	CStackItem* GetNext() { return m_pNext; }
	ItemType GetStackItemType() { return m_Type; }
	bool Verify(CStackItem::ItemType Type);
	static const char* LookupItemName(ItemType Type);
	const char* GetStackItemName();
};

