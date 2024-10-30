#pragma once

class CObjFormatData : public CObjFormatSectionItem
{
	int m_BlockSize;
	int m_Index;
	char* m_pData;
public:
	CObjFormatData();
	virtual ~CObjFormatData();
	bool Create(int BlockSize);
	virtual void Out(FILE* pOut);
	virtual void Print(FILE* pO, int Indent);
	bool AddByte(char d);
};

