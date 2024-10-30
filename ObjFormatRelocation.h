#pragma once

class CObjFormatRelocation : public CObjFormatSectionItem
{

public:
	CObjFormatRelocation();
	virtual ~CObjFormatRelocation();
	bool Create();
	virtual void Out(FILE* pO);
	virtual void Print(FILE* pO, int Indent);
};

