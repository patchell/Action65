#include "pch.h"

CStack::CStack()
{
	m_pHead = 0;
	m_ItemCount = 0;
	m_pName = 0;
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
		if (m_ItemCount > 100)
			printf("Is Stack Overflowing? %d\n", m_ItemCount);
//		Print(Act()->LogFile(), "PUSH", 4, pItem);
	}
}

CStackItem* CStack::Pop()
{
	CStackItem* pRV = 0;

	if (m_ItemCount)
	{
		--m_ItemCount;
		if (GetHead())
		{
				pRV = GetHead();
				SetHead(GetHead()->GetNext());
//				Print(Act()->LogFile(), "POP", 4, pRV);
		}
		else
		{
			StackException(
				"Stack Emptyh:Count wrong", 
				Exception::ExceptionSubType::STACK_UNEXPECTED_NULL
			);
		}
	}
	else
	{
		StackException(
			"Cannot Pop an Empty Stack",
			Exception::ExceptionSubType::STACK_EMPTY
		);

	}
	return pRV;
}

CStackItem* CStack::Look(int Depth)
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

void CStack::Print(
	FILE* pLog,  
	const char* pOperation,
	int Indent,
	CStackItem* pRV
)
{
	char* IndentString = new char[256];
	const char* pItemTypeName;

	Act()->CreateIndentString(IndentString, 256, Indent);
	if (pLog)
	{
		if (pRV)
		{
			pItemTypeName =pRV->LookupItemName(pRV->GetStackItemType());
		}
		else
			pItemTypeName = "";
		fprintf(pLog, "\n%s------------------\n", IndentString);
		fprintf(pLog, 
			"%sStack:%s:%s:TYPE:%s Line:%d\n", 
			IndentString,
			GetStackName(), 
			pOperation,
			pItemTypeName,
			Act()->GetParser()->GetLexer()->GetLineNumber()
		);
		fprintf(pLog, "%sItemCount:%d\n", IndentString,GetItemCount());
		fprintf(pLog, "%s------------------\n\n", IndentString);
	}
	delete[]IndentString;
}

void CStack::StackException(const char* pDesc, Exception::ExceptionSubType SubType)
{
	ThrownException.SetXCeptType(Exception::ExceptionType::STACK);
	sprintf_s(
		ThrownException.GetErrorString(),
		ThrownException.GetMaxStringLen(),
		"Stack:%s::%s\n--Line: %d Col: %d",
		GetStackName(),
		pDesc,
		Act()->GetParser()->GetLexer()->GetLineNumber(),
		Act()->GetParser()->GetLexer()->GetColunm()
	);
	throw(ThrownException);
}
