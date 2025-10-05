#include "pch.h"

CChainItem::CChainItem()
{
	m_ItemType = ChainItemType::NONE;
	m_pNext = 0;
	m_pPrev = 0;
}

CChainItem::~CChainItem()
{
}

int CChainItem::Print(char* pSO, int l, int Indent, const char* s)
{
	int ls = 0;
	char* pIndentString = new char[512];
	CSymbol* pSym = 0;

	Act()->IndentString(pIndentString, 512, Indent);
	if (s)
		ls += sprintf_s(pSO + ls, l - ls, "%s%s:", pIndentString, s);
	ls += sprintf_s(pSO + ls, l - ls, "%s\n", ItemTypeLUT[int(m_ItemType)].m_pName);
	switch(m_ItemType)
	{
	case ChainItemType::PARAMETER:
		ls += sprintf_s(pSO + ls, l - ls, "%s\n", "Parameter Item");
		break;
	case ChainItemType::VALUE:
		ls += sprintf_s(pSO + ls, l - ls, "%s\n", "Value Item");
		break;
	case ChainItemType::INSTRUCTION:
		ls += sprintf_s(pSO + ls, l - ls, "%s\n", "Instruction Item");
		break;
	case ChainItemType::BIN:
		ls += sprintf_s(pSO + ls, l - ls, "%s\n", "Binary Item");
		break;
	case ChainItemType::TYPE:
		ls += sprintf_s(pSO + ls, l - ls, "%s\n", "Type Item");
		break;
	case ChainItemType::SYMBOL_USED:
		ls += sprintf_s(pSO + ls, l - ls, "%s\n", "Symbol Used Item");
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Internal Error:Unknow Chain Type\n"
		);
		throw(ThrownException);

		break;
	}
	delete[] pIndentString;
	return ls;
}