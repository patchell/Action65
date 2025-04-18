#include "pch.h"

CSymbol::CSymbol() : CBin(CBin::BinType::SYMBOL)
{
	m_Address = 0;
	m_Scope = SYMBOL_SCOPE_ANY;
	m_pSection = 0;
	m_UnResolved = true;
	m_pTypeChain = 0;
	m_pParamChain = 0;
	m_pTypeDefChain = 0;
	m_pLocalVariables = 0;	//list of local variables
}

bool CSymbol::Compare(const char* name, int scope)
{
	bool rV = false;

	if (strcmp(GetName(), name) == 0)
	{
		rV = true;
		//if (scope > 0)
		//{
		//	if (scope == GetScope())
		//		rV = true;
		//}
		//else
		//	rV = true;
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
		if (!pWSIU->IsResolveProcessed())
		{
			URLocation = pWSIU->GetAddress();
			fprintf(
				Act()->LogFile(),
				"Unresolved: @0x%04x\n",
				URLocation
			);
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
		}
		pWSIU = (CWhereSymbolIsUsed*)pWSIU->GetNext();
	}
	SetResolved();
}

void CSymbol::CreateTypeChain(CTypeChain* pTC)
{
	//---------------------------------------
	// Create a type chan for this symbol
	// Make a copy of the passed pointer
	// to a Type Chain
	//---------------------------------------
	m_pTypeChain = new CTypeChain;
	GetTypeChain()->Create();
	if (pTC)
	{
		GetTypeChain()->CopyTypeChain(pTC);
	}
}

void CSymbol::Print(FILE* pOut, const char* s)
{
	char* pSO = new char[2048];

	memset(pSO, 0, 2048);
	Print(pSO, 2048, NULL);
	fprintf(pOut, "%s\n", pSO);
	delete[] pSO;
}

int CSymbol::Print(char* pSO, int l, const char* s)
{
	int ls = 0;
	int size;
	char* pp = pSO;

	if (s)
	{
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "%s", s);
	}
	if(GetTypeChain())
		if (GetTypeChain()->Is(CObjTypeChain::Spec::FUNC))
			printf("Opps\n");
	if (GetName())
	{
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "%s: ", GetName());
	}
	if (GetTypeChain())	
	{
		ls += GetTypeChain()->Print(&pSO[ls], l - ls);
	}
	size = l - ls;
	ls += sprintf_s(&pSO[ls],size, ":Address=$%04lx",
		GetAddress()
	);
	if (GetParamChain())
	{
		size = l - ls;
		ls += GetParamChain()->Print(&pSO[ls], size);
	}
	if (GetHead())	//print where the symbol is used
	{
		CWhereSymbolIsUsed* pWSIU = 0;

		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "\n");

		pWSIU = (CWhereSymbolIsUsed*)GetHead();
		while (pWSIU)
		{
			size = l - ls;
			ls += sprintf_s(&pSO[ls], size, "\t\t");
			size = l - ls;
			ls += pWSIU->Print(&pSO[ls], size,0);
			pWSIU = (CWhereSymbolIsUsed*)pWSIU->GetNext();
			if (pWSIU)
			{
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "\n");
			}
		}
	}
	CChainBinItem* pLocalSyms = 0;
	if (GetLocalVars())
	{
		pLocalSyms = (CChainBinItem*)GetLocalVars()->GetHead();
	}
	if (pLocalSyms)
	{
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "Proceedure Local Variables:\n");
		while (pLocalSyms)
		{
			size = l - ls;
			ls += ((CSymbol*)pLocalSyms->GetSymbol())->Print(&pSO[ls], size, "\t");
			if (pLocalSyms->GetNext())
			{
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "\n");
			}
			pLocalSyms = (CChainBinItem*)pLocalSyms->GetNext();
		}
	}
	return ls;
}

const char* CSymbol::CIdentType::LookupIdentType(IdentType IT)
{
    return IdentTypeLUT[int(IT)].m_pName;
}
