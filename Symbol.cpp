#include "Global.h"

BOOL CSymbol::Compare(const char* name, int scope)
{
	BOOL rV = FALSE;

	if (strcmp(GetName(), name) == 0)
	{
		if (scope > 0)
		{
			if (scope == GetScope())
				rV = TRUE;
		}
		else
			rV = TRUE;
	}
	return rV;
}

void CSymbol::Print(FILE* pOut, const char* s)
{
	fprintf(pOut, "%s:Address=%08lx  Value=%04x  Scope=%d\n",
		GetName(),
		GetAddress(),
		GetValue(),
		GetScope()
	);
}
