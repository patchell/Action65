#include "pch.h"

bool CSymbol::Compare(const char* name, int scope)
{
	bool rV = false;

	if (strcmp(GetName(), name) == 0)
	{
		if (scope > 0)
		{
			if (scope == GetScope())
				rV = true;
		}
		else
			rV = true;
	}
	return rV;
}


void CSymbol::BackFillUnresolved()
{
	CWhereSymbolIsUsed* pWSIU;
	int Address;
	int URLocation; //unresolved location

	Address = GetAddress();
	//	fprintf(
	//		As65App.LogFile(),
	//		"Back Fill %s @ 0x%4x\n",
	//		GetName(),
	//		Address
	//	);
	pWSIU = (CWhereSymbolIsUsed*)GetHead();
	while (pWSIU)
	{
		URLocation = pWSIU->GetAddress();
		//		fprintf(
		//			As65App.LogFile(),
		//			"Unresolved: @0x%04x\n",
		//			URLocatioon
		//		);
		if (pWSIU->GetUnResType() == CWhereSymbolIsUsed::UnResolvedType::ABSOLUTE_REFERENCE)
		{
			//------------------------------------------
			// Get the location of an unresolved value,
			// and then put the data defined by this
			// symbol in there.
			//------------------------------------------
			pWSIU->GetSection()->AddDataAt(
				pWSIU->GetAddress(),	//address of data
				2,						// size of object
				Address					// object value
			);
		}
		else
		{
			//------------------------------------------
			// for relative address, subtract the
			// location of the unresoved symbol from
			// the value of this object.
			//------------------------------------------
			unsigned WhereSymIsUsedAddress = pWSIU->GetAddress();
			unsigned RelAddress = Address - WhereSymIsUsedAddress;

			RelAddress--;
			pWSIU->GetSection()->AddDataAt(
				WhereSymIsUsedAddress,
				1,
				RelAddress
			);
		}
		pWSIU = (CWhereSymbolIsUsed*)pWSIU->GetNext();
	}
	SetResolved();
}

void CSymbol::Print(FILE* pOut, const char* s)
{
	char* pSO = new char[512];

	memset(pSO, 0, 256);
	Print(pSO, 512, NULL);
	fprintf(pOut, "%s\n", pSO);
	delete[] pSO;
}

int CSymbol::Print(char* pSO, int l, const char* s)
{
	int ls = 0;
	int size;

	if (s)
	{
		size = l - ls;
		ls += sprintf_s(pSO, size, "%s", s);
	}
	if (GetName())
	{
		size = l - ls;
		ls += sprintf_s(pSO, size, "%s: ", GetName());
	}
	if (GetTypeChain())
	{
		ls += GetTypeChain()->Print(&pSO[ls], l - ls);
	}
	size = l - ls;
	ls += sprintf_s(&pSO[ls],size, ":Address=%08lx  Value=%04x\n",
		GetAddress(),
		GetValue()
	);
	return ls;
}

const char* CSymbol::CIdentType::LookupIdentType(IdentType IT)
{
    return IdentTypeLUT[int(IT)].m_pName;
}
