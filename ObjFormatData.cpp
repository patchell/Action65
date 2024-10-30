#include "pch.h"

CObjFormatData::CObjFormatData()
{
	m_BlockSize = 0;
	m_Index = 0;
	m_pData = 0;
}

CObjFormatData::~CObjFormatData()
{
	if (m_pData) delete[] m_pData;
}

bool CObjFormatData::Create(int BlockSize)
{
	m_BlockSize = BlockSize;
	m_pData = new char[BlockSize];
	return CObjFormatSectionItem::Create(CObjFormatSectionItem::FormatType::DATA);
}

void CObjFormatData::Out(FILE* pOut)
{
}

void CObjFormatData::Print(FILE* pO, int Indent)
{
}

bool CObjFormatData::AddByte(char d)
{
	bool rV = true;
	if (m_Index + 1 < m_BlockSize)
	{
		m_pData[m_Index++] = d;
	}
	else
		rV = false;
	return 0;
}
