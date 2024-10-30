#pragma once

class CObjFormatCode : public CObjFormatSectionItem
{
	unsigned short m_BlockSize;
	unsigned short m_StartAddress;
	char* m_pCode;
public:
	CObjFormatCode();
	virtual ~CObjFormatCode();
	bool Create(int StartAddress, int BlockSize, const char* pCode);
	virtual void Out(FILE* pOut);
	virtual void Print(FILE* pO, int Indent);
};

