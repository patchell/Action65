#pragma once

class CMacro : public CBin
{
public:
	CMacro();
	virtual ~CMacro();
	bool Create();
	virtual bool Compare(const char* name, BinType Type = BinType::ANY, int aux = 0);
	virtual void Print(FILE* pOut, const char* s);
	virtual int Print(char* pSO, int l, const char* s = 0);
	virtual void Process();
};

