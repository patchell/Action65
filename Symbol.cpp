#include "pch.h"

CSymbol::CSymbol() : CBin(CBin::BinType::SYMBOL)
{
	m_bDefined = false;
	m_Address = 0;
	m_pSection = 0;
	m_UnResolved = true;
	m_pTypeChain = 0;
	m_pParamChain = 0;
	m_pTypeDefFieldChain = 0;
	m_pLocalVariablesChain = 0;	//list of local variables
}

bool CSymbol::Compare(const char* name, BinType Type, int scope)
{
	bool rV = false;

	if (strcmp(GetName(), name) == 0)
	{
		if ((Type == BinType::ANY) || (GetType() == Type))
		{
			if ((scope == SYMBOL_SCOPE_ANY) || (GetScope() == scope))
			{
				rV = true;
			}
		}
	}
	return rV;
}


void CSymbol::BackFillUnresolved()
{
	int Address;
	int URLocation; //unresolved location
	CChain* pChain = GetWhereUsed();
//	CChainItem* pItem = (CChainItem*)pChain->GetHead();

	Address = GetAddress();
	fprintf(
		Act()->	LogFile(),
		"Back Fill %s @ 0x%4x\n",
		GetName(),
		Address
	);
	//while (pItem)
	//{
	//	if (!pWSIU->IsResolveProcessed())
	//	{
	//		URLocation = pWSIU->GetAddress();
	//		fprintf(
	//			Act()->LogFile(),
	//			"Unresolved: @0x%04x\n",
	//			URLocation
	//		);
	//		if (pWSIU->GetUnResType() == CWhereSymbolIsUsed::UnResolvedType::ABSOLUTE_REFERENCE)
	//		{
	//			//------------------------------------------
	//			// Get the location of an unresolved value,
	//			// and then put the data defined by this
	//			// symbol in there.
	//			//------------------------------------------
	//			pWSIU->GetSection()->AddDataAt(
	//				pWSIU->GetAddress(),	//address of data
	//				2,						// size of object
	//				Address					// object value
	//			);
	//		}
	//		else
	//		{
	//			//------------------------------------------
	//			// for relative address, subtract the
	//			// location of the unresoved symbol from
	//			// the value of this object.
	//			//------------------------------------------
	//			unsigned WhereSymIsUsedAddress = pWSIU->GetAddress();
	//			unsigned RelAddress = Address - WhereSymIsUsedAddress;

	//			RelAddress--;
	//			pWSIU->GetSection()->AddDataAt(
	//				WhereSymIsUsedAddress,
	//				1,
	//				RelAddress
	//			);
	//		}
	//	}
	//	pWSIU = (CWhereSymbolIsUsed*)pWSIU->GetNext();
	//}
	//SetResolved();
}

void CSymbol::CreateTypeChain(CChain* pTC)
{
	//---------------------------------------
	// Create a type chan for this symbol
	// Make a copy of the passed pointer
	// to a Type Chain
	//---------------------------------------
	m_pTypeChain = new CChainTypeSpec;
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

int CSymbol::Print(char* pSO, int l, int Indent, const char* s)
{
	int ls = 0;
	int size = l;
	char* pp = pSO;
	char* pIndentString = 0;
	static int Recursions = 0;

	++Recursions;
	pIndentString = new char[256];	
	Act()->IndentString(pIndentString, 256, Indent + 2, ' ');
	if (s)
	{
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "%s:", s);
	}
	if (GetSection())
	{
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "%s%s:Section = %s\n", pIndentString, GetName(), GetSection()->GetName());
	}
	if (GetTypeChain())
	{
		size = l - ls;
		ls += GetTypeChain()->Print(&pSO[ls], size, Indent + 2, 0);
	}
	else if (GetName())
	{
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "%s%s: ", pIndentString, GetName());
	}
	size = l - ls;
	ls += sprintf_s(&pSO[ls],size, "%sAddress = $%04lx\n",
		pIndentString,
		GetAddress()
	);
	if (GetParamChain())
	{
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "%sParameters:\n", pIndentString);
		size = l - ls;
		ls += GetParamChain()->Print(&pSO[ls], size);
	}
	if (GetWhereUsed())	//print where the symbol is used
	{
		CChainItem* pWSIU = 0;

		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "%sSymbol is used AT:\n", pIndentString);
		size = l - ls;
		pWSIU = GetWhereUsed()->GetHead();
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "\n");

		while (pWSIU)
		{
			size = l - ls;
			ls += sprintf_s(&pSO[ls], size, "\t\t");
			size = l - ls;
			ls += pWSIU->Print(&pSO[ls], size,0);
			pWSIU = pWSIU->GetNext();
			if (pWSIU)
			{
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "\n");
			}
		}
	}
	CChainLocalItem* pLocalSyms = 0;
	if (GetLocalVars())
	{
		pLocalSyms = (CChainLocalItem*)GetLocalVars()->GetHead();
	}
	if (pLocalSyms)
	{
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "Proceedure Local Variables:\n");
		while (pLocalSyms)
		{
			size = l - ls;
			ls += sprintf_s(&pSO[ls], size, "%s------------ %s -----------\n", pIndentString, pLocalSyms->GetName() );
			size = l - ls;
			ls += ((CSymbol*)pLocalSyms->GetSymbol())->Print(&pSO[ls], size, Indent+ 2, 0);
			if (pLocalSyms->GetNext())
			{
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "\n");
			}
			pLocalSyms = (CChainLocalItem*)pLocalSyms->GetNext();
		}
	}
	delete[] pIndentString;
	--Recursions;
	return ls;
}

void CSymbol::SetAddress(unsigned A)
{
	if (IsUnResolved())
	{
		if (A == 0xf805)
		{
			int debug = 0;
		}
		m_Address = A;
		SetResolved();
		BackFillUnresolved();
	}
	else
		printf("Error: Symbol address already resolved\n");
}

const char* CSymbol::CIdentType::LookupIdentType(IdentType IT)
{
	const char* rV = 0;

	for (int i = 0; IdentTypeLUT[i].m_pName && !rV; ++i)
	{
		if (IdentTypeLUT[i].m_Type == IT)
			rV = IdentTypeLUT[i].m_pName;
	}
    return rV;
}
