#include "pch.h"

CObjFormatCode::CObjFormatCode()
{
	m_BlockSize = 0;
	m_pCode = 0;
	m_StartAddress = 0;
}

CObjFormatCode::~CObjFormatCode()
{
	if (m_pCode) delete m_pCode;
}

bool CObjFormatCode::Create(int StartAddress, int BlockSize, const char* pCode)
{
	m_pCode = new char[BlockSize];
	m_BlockSize = (unsigned short)BlockSize;
	m_StartAddress = (unsigned short)StartAddress;
	memcpy(m_pCode, pCode, BlockSize);
	return CObjFormatSectionItem::Create(CObjFormatSectionItem::FormatType::CODE);
}

void CObjFormatCode::Out(FILE* pOut)
{
}

void CObjFormatCode::Print(FILE* pO, int Indent)
{
}
