#pragma once

class CStack
{
	const char* m_pName;
	CStackItem* m_pHead;
	int m_ItemCount;
public:
	CStack();
	virtual ~CStack();
	bool Create(const char* pName);
	virtual void Push(CStackItem* pItem);
	virtual CStackItem* Pop(CStackItem::ItemType Type);
	virtual CStackItem* Look(int Depth, CStackItem::ItemType Type);
	virtual CStackItem* GetHead() { return m_pHead; }
	virtual void SetHead(CStackItem* pSI) { m_pHead = pSI; }
	virtual bool IsTopOfType(CStackItem::ItemType Type);
	virtual const char* GetStackName() { return m_pName; }
};

