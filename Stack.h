#pragma once

class CStack
{
	CStackItem* m_pHead;
	int m_ItemCount;
public:
	CStack();
	virtual ~CStack();
	bool Create();
	virtual void Push(CStackItem* pItem);
	virtual CStackItem* Pop(CStackItem::ItemType Type);
	virtual CStackItem* Look(int Depth, CStackItem::ItemType Type);
	virtual CStackItem* GetHead() { return m_pHead; }
	virtual void SetHead(CStackItem* pSI) { m_pHead = pSI; }
};

