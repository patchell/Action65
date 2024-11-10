#include "pch.h"

CStack::CStack()
{
	m_pHead = 0;
	m_ItemCount = 0;
}

CStack::~CStack()
{
	CStackItem* pSI;

//	fprintf(
//		stderr,
//		"Stack Size = %d\n", m_ItemCount
//	);
	while (m_pHead)
	{
		pSI = m_pHead->GetNext();
//		fprintf(
//			stderr,
//			"Delete Stack Item %s\n", 
//			m_pHead->GetStackItemName()
//		);
		delete m_pHead;
		m_pHead = pSI;
	}
}

bool CStack::Create(const char* pName)
{
	m_pName = pName;
	return true;
}

void CStack::Push(CStackItem* pItem)
{
	if (pItem)
	{
		++m_ItemCount;
		pItem->SetNext(GetHead());
		SetHead(pItem);
//		fprintf(Act()->LogFile(),
//			"Pushed a %s  Items:%d\n",
//			CStackItem::LookupItemName(pItem->GetStackItemType()),
//			m_ItemCount
//		);
		if (m_ItemCount > 100)
			printf("Is Stack Overflowing? %d\n", m_ItemCount);
	}
//	else
//		printf("Attempt to push Nothing on Stack\n");
}

CStackItem* CStack::Pop(CStackItem::ItemType Type)
{
	CStackItem* pRV = 0;

	--m_ItemCount;
	if (GetHead())
	{
		pRV = GetHead();
		SetHead(GetHead()->GetNext());
	}
//	fprintf(Act()->LogFile(),
//		"Popped a %s, Wanted a %s  Items:%d Line:%d\n",
//		pRV->GetStackItemName(),
//		CStackItem::LookupItemName(Type),
//		m_ItemCount,
//		Act()->GetParser()->GetLexer()->GetLine()
//	);
	
	if (!pRV->Verify(Type))
	{
		ExceptionThrown.SetXCeptType(Exception::INTERNAL_ERROR);
		sprintf_s(
			ExceptionThrown.GetErrorString(),
			ExceptionThrown.GetMaxStringLen(),
			"Value Stack:Pop::Got %s Expected %s--Line %d Col %d",
			CStackItem::LookupItemName(pRV->GetStackItemType()),
			CStackItem::LookupItemName(Type),
			Act()->GetParser()->GetLexer()->GetLineNumber(),
			Act()->GetParser()->GetLexer()->GetColunm()
		);
		throw(ExceptionThrown);
	}
	return pRV;
}

CStackItem* CStack::Look(int Depth, CStackItem::ItemType Type)
{
	int i = 0;
	CStackItem* pRV = 0;;
	bool Loop = true;

	if (GetHead())
	{
		for (i = 0, pRV = GetHead(); i < Depth && Loop; ++i)
		{
			if (pRV == 0)
			{
				Loop = false;
			}
			else
			{
				pRV = pRV->GetNext();
			}
		}
		if (!pRV->Verify(Type))
		{
			ExceptionThrown.SetXCeptType(Exception::INTERNAL_ERROR);
			sprintf_s(
				ExceptionThrown.GetErrorString(),
				ExceptionThrown.GetMaxStringLen(),
				"Value Stack:Look:Got %s Expected %s--Line %d Col %d",
				CStackItem::LookupItemName(pRV->GetStackItemType()),
				CStackItem::LookupItemName(Type),
				Act()->GetParser()->GetLexer()->GetLineNumber(),
				Act()->GetParser()->GetLexer()->GetColunm()
			);
			throw(ExceptionThrown);
		}
	}
	return pRV;
}

bool CStack::IsTopOfType(CStackItem::ItemType Type)
{
	bool rV = false;

	if (GetHead())
	{
		if (GetHead()->GetStackItemType() == Type)
			rV = true;
	}
	else
	{
		fprintf(stderr,
			"Stack %s Is Empty Line:%d\n",
			GetStackName(),
			Act()->GetParser()->GetLexer()->GetLineNumber()
		);
	}
    return rV;
}
