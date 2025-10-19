#include "pch.h"

CObjFormatSection::CObjFormatSection()
{
	m_pSection = 0;
	m_pNext = 0;
	m_pPrev = 0;
	m_pHead = 0;
	m_pTail = 0;
}

CObjFormatSection::~CObjFormatSection()
{
}

bool CObjFormatSection::Create(CSection* pSection)
{
	bool rV = true;

	m_pSection = pSection;
	//--------------------------------
	// Create a list of locations
	// that need to be updated in
	// this section
	//--------------------------------
	return rV;
}

void CObjFormatSection::AddItem(CObjFormatSectionItem* pItem)
{
	if (m_pHead)
	{
		m_pTail->SetNext(pItem);
		pItem->SetPrev(m_pTail);
		m_pTail = pItem;
	}
	else
	{
		m_pTail = pItem;
		m_pHead = pItem;
	}
}

void CObjFormatSection::Print(FILE* pO, int Indent)
{
	char* pStr = new char[256];

	fprintf(pO, "%sSection:%s\n",
		Act()->IndentString(pStr, 256, Indent,' '),
		GetSection()->GetName()
	);
	delete[] pStr;
}

void CObjFormatSection::Out(FILE* pO)
{
}
