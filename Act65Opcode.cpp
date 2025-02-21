#include "pch.h"

CAct65Opcode::CAct65Opcode():CAstNode(m_pNodeTyypeName, NodeType::OPCODE)
{
	m_OpcodeToken = Token(-1);
	m_LineNumber = 0;
	m_ColumnNumber = 0;
	m_pSym = 0;
	m_pLabel = 0;
	m_AdressMode = AdrModeType::NA;
	m_pKeyWord = 0;
	m_ByteCount = 0;
	m_OpCode = 0;
	m_Operand = 0;
}

CAct65Opcode::~CAct65Opcode()
{
}

bool CAct65Opcode::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65Opcode::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValueChild = 0, * pValueNext = 0;
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		pValueChild = pChild->Process();
	}
	if (pNext)
	{
		pValueNext = pNext->Process();
	}
	//-----------------------
	// Code Generation
	//-----------------------
	return pValueChild;
}

void CAct65Opcode::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		
		l = Print(Indent, s, l, pbNextFlag);
		sprintf_s(&s[l], 256 - l, " %s", GetKeyWord()->m_Name);
		fprintf(pOut, "%s\n", s);
		delete[]s;
	}
}

int CAct65Opcode::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	int size = Strlen - l;
	l += sprintf_s(&s[l],size, " %s ($%02X) %s", 
		GetKeyWord()->m_Name, 
		GetOpCode() &0x0ff,
		AdrModeToTxtTabel.LookupAddressingMode(GetAdrModeType())
	);
	return l;
}

void CAct65Opcode::PrepareInstruction(
	Token OpToken, 
	AdrModeType AddressMode,
	CAstNode* pOperandValue_Node
)
{
	CLexer* pLex = Act()->GetParser()->GetLexer();

	m_pKeyWord = pLex->FindKeyword(OpToken);
	if (m_pKeyWord->m_pAddresModeLUT->ValidAddressingMode(AddressMode))
	{
		m_AdressMode = AddressMode;
		SetToken(OpToken);
		SetLineNumber(pLex->GetLineNumber());
		SetColumnNumber(pLex->GetColunm());
		SetOpCode(pLex->MakeOpcode(OpToken, AddressMode));
		SetByteCount(OperandByteCount::GetOperandByteCount(AddressMode) + 1);
		SetChild(pOperandValue_Node);
	}
	else   // :(
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::ILLEGAL_ADDRESSING_MODE);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Line %d: Illegal Addressing Mode\n",
			Act()->GetParser()->GetLexer()->GetLineNumber()
		);
		throw(ThrownException);
	}
}

int CAct65Opcode::SaveInstruction(char* pM)
{
	*pM++ = (char)GetOpCode();
	if (GetByteCount() > 1)
		*pM++ = (char)GetOperand() & 0xff;
	if(GetByteCount() > 2)
		*pM++ = (char)((GetOperand() & 0xff00) >> 8);
	return GetByteCount();
}

const char* CAct65Opcode::GetOpcodeName()
{
	return m_pKeyWord->m_Name;
}

int CAct65Opcode::OperandByteCount::GetOperandByteCount(AdrModeType AdrMode)
{
	int rV = -1;
	int i;
	bool Loop = true;

	for (i = 0; (OperByteCntLUT[i].m_OperandByteCount >= 0) && Loop; ++i)
	{
		if (OperByteCntLUT[i].m_AdrMode == AdrMode)
		{
			Loop = false;
			rV = OperByteCntLUT[i].m_OperandByteCount;
		}
	}
	return 0;
}
