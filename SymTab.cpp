#include "pch.h"

//***************************************
// Implementation of Hash Table Member
// Functions
//***************************************

//-------------------------------------------
// Constructor
//
// parameter:
//	size......number of Bins in the table
//
// Now, don't ask me why, but I heard that
// it is best to use a prime number for the
// number of bins.  I suspect the reason why
// is that we are doing a MOD opertion in
// the hashing function to make sure the
// index ends up in the table.
//
// And if you are like me, you can't
// remember what the prime number are.
// So, here is a list form 0 to 200.
// 2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
//  31, 37, 41, 43, 47, 53, 59, 61, 67
// 71, 73, 79, 83, 89, 97, 101, 103, 107,
// 109, 113, 127, 131, 137, 139, 149,
// 151, 157, 163, 167, 173, 179, 181,
// 191, 193, 197, 199
//--------------------------------------------

//*****************************************************
// Destructor
//
// destroy HashTab and all of its components
//*****************************************************

CSymTab::~CSymTab()
{
	CBucket* pBucket;
	int i = 0;

	for (i = 0; i < m_tSize; ++i)
	{
		pBucket = m_ppTab[i];
		delete pBucket;
	}
	
	if (m_ppTab) delete[] m_ppTab;
}

bool CSymTab::Create(int TableDepth)
{
	m_ppTab = new CBucket * [TableDepth];
	m_tSize = TableDepth;
	for (int i = 0; i < m_tSize; ++i)
		m_ppTab[i] = 0;
	m_nElements = 0;
	return true;
}

//*****************************************************
// FindSym
//
// parameter:
//	name....name of object to find
//
//******************************************************

CBin* CSymTab::FindSymbol(const char* name, int scope)
{
	CBin* pRV = NULL;

	int Index = Hash(name);	//get index from hash of name
	if(m_ppTab[Index] != 0)
		pRV = m_ppTab[Index]->Find(name, scope);
	return pRV;
}

//***********************************************************
// AddSym
//
// parameter:
//	pSym....pointer to Bin to add to table
//
//***********************************************************

void CSymTab::AddSymbol(CBin* pSym)
{
	int Index = Hash(pSym->GetName());	//generate index
	
	if (m_ppTab[Index] == NULL)	//there is NO bucket here
	{
		m_ppTab[Index] = new CBucket;
		m_ppTab[Index]->Create();
	}

	if (m_ppTab[Index]->GetHead() == NULL)	//nobody home
	{
		m_ppTab[Index]->SetHead(pSym);
		m_ppTab[Index]->SetTail(pSym);
	}
	else
	{
		CBucket* pB = m_ppTab[Index];
		pB->GetTail()->SetNext(pSym);
		pSym->SetPrev(pB->GetTail());
		pB->SetTail(pSym);
	}
	++m_nElements;	//increment number of symbols
}

//*****************************************************
// DelSym
//
// parameter:
//	pSym.....pointer to symbol to remove
//*****************************************************

void CSymTab::DelSymbol(CBin* pSym)
{
	int HashValue = Hash(pSym->GetName());
	m_ppTab[HashValue]->Delete(pSym);
}

//*********************************************************
// Hash
//
// parameter
//	name.....pointer to name to Hash
//
//**********************************************************

int CSymTab::Hash(const char* name)
{
	unsigned HashVal = 0;
	unsigned TopBits;
	unsigned char* n;

	n = (unsigned char*)name;
	while (*n)
	{
		HashVal = (HashVal << 12) + *n;
		if ((TopBits = HashVal & 0xff000000) != 0)
			HashVal = ((TopBits >> 24) ^ HashVal) & 0x00ffffff;
		++n;	//next char please...
	}
	return HashVal % m_tSize;	//returns index in table
}


//***************************************************
// PrinTable
//
// paramter:
//	pOut.....output file stream
//
//*****************************************************

void CSymTab::PrintTable(FILE* pOut)
{
	char* s = new char[512];
	CBin* pB;
	int i;
	int maxStringLen = 0;
	int l;

	//------------------------------
	// Scan table to find longest
	// name
	//------------------------------
	for (i = 0; i < m_tSize; ++i)
	{
		if (m_ppTab[i] == NULL)
		{
			pB = NULL;
		}
		else
			pB = m_ppTab[i]->GetHead();
		while (pB)
		{
			l = (int)strlen(pB->GetName());
			if (l > maxStringLen)
				maxStringLen = l;
			pB = pB->GetNext();
		}
	}

	for (i = 0; i < m_tSize; ++i)
	{
		if (m_ppTab[i] == NULL)
		{
			pB = NULL;
		}
		else
			pB = m_ppTab[i]->GetHead();
		while (pB)
		{
			pB->Print(pOut);
			pB = pB->GetNext();
		}
	}
}


bool CSymTab::FindAll(CBin::BinType Type, int NumberOfObject, CBin** ppObjects)
{
	bool rV = true;
	int ItemCount = 0;
	CBucket* pBucket;
	CBin* pBin;
	int i;

	for (i = 0; i < m_tSize; ++i)
	{
		pBucket = this->GetTable()[i];
		if (pBucket)
		{
			pBin = pBucket->GetHead();
			while (pBin)
			{
				if (pBin->GetType() == Type)
				{
					if (ItemCount < NumberOfObject)
					{
						ppObjects[ItemCount] = pBin;
						ItemCount++;
					}
					else
					{
						++ItemCount;
					}
				}
				pBin = pBin->GetNext();
			}
		}
	}
	if (ItemCount > NumberOfObject - 1)
		rV = false;
	return rV;
}

