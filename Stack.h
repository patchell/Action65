#pragma once

class CStack
{
	static inline char m_sExceptionString[MAX_STRING_LEN];
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
	virtual void Print(
		FILE* pLog, 
		const char* pName, 
		int Indent = 0,
		CStackItem* pRV = 0
	);
	void StackException(const char* pDesc, Exception::ExceptionSubType SubType);
	virtual const char* GetStackName() { return m_pName; }
	int GetItemCount() { return m_ItemCount; }
};

