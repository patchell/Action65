#include "pch.h"

CMacro::CMacro() : CBin(BinType::MACRO)
{
}

CMacro::~CMacro()
{
}

bool CMacro::Create()
{
	bool rV = true;
	rV = CBin::Create();
	return rV;
}

bool CMacro::Compare(const char* name, BinType Type, int aux)
{
	bool rV = false;
	if (name && (GetName() != 0))
	{
		if (strcmp(name, GetName()) == 0)
		{
			if ((Type == BinType::ANY) || (Type == GetType()))
			{
				rV = true;
			}
		}
	}
	return rV;
}	

void CMacro::Print(FILE* pOut, const char* s)
{
	if (s)
		fprintf(pOut, "%s", s);
	if (GetName())
		fprintf(pOut, "Macro:%s Type:%s\n", GetName(), CBin::GetBinTypeString(GetType()));
	else
		fprintf(pOut, "Macro:NONAME Type:%s\n", CBin::GetBinTypeString(GetType()));
}

int CMacro::Print(char* pSO, int l, const char* s)
{
	int rV = 0;
	if (s)
		rV += sprintf_s(&pSO[rV], l - rV, "%s", s);
	if (GetName())
		rV += sprintf_s(&pSO[rV], l - rV, "Macro:%s Type:%s\n", GetName(), CBin::GetBinTypeString(GetType()));
	else
		rV += sprintf_s(&pSO[rV], l - rV, "Macro:NONAME Type:%s\n", CBin::GetBinTypeString(GetType()));
	return rV;
}

void CMacro::Process()
{
	
}
