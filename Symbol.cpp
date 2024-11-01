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
	fprintf(pOut, "%s:Address=%08lx  Value=%04x  Scope=%d\n",
		GetName(),
		GetAddress(),
		GetValue(),
		GetScope()
	);
}
