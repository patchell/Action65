#include "Global.h"

void CBin::Print(FILE* pOut, const char* s)
{
	fprintf(pOut, "CBin::%s\n", m_pName);
}
