#include "pch.h"

CObjFormatFile::CObjFormatFile()
{
	int i;

	for (i = 0; i < 512; ++i)
	{
		m_strName[i] = 0;
	}
	m_pHead = 0;
	m_pTail = 0;
}

CObjFormatFile::~CObjFormatFile()
{
}

bool CObjFormatFile::Create(const char* pName)
{
	bool rV = true;
	int NumberOfSections;
	CSection** ppSections;
	int i;

	fprintf(Act()->GetParser()->LogFile(), "File:%s\n", pName);
	strcpy_s(m_strName, 512, pName);
	NumberOfSections = GetSymTab()->GetSectionCount();
	ppSections = new CSection * [NumberOfSections];
	GetSymTab()->FindAll(CBin::BinType::SECTION, GetSymTab()->GetSectionCount(),(CBin**) ppSections);
	for (i = 0; i < NumberOfSections; ++i)
	{
		AddSection(ppSections[i]);
	}
	Print(Act()->LogFile());
	return rV;
}

CSymTab* CObjFormatFile::GetSymTab()
{
	return Act()->GetParser()->GetLexer()->GetSymTab();
}

void CObjFormatFile::Out(FILE* pOut)
{
	FILE* pLog;

	pLog = Act()->LogFile();
	Print(pLog);
	fprintf(pLog, "Number of section:%d\n", GetSymTab()->GetSectionCount());
	fprintf(pLog,"Number of Symbols:%d", GetSymTab()->GetSymbolCount());
}

void CObjFormatFile::Print(FILE* pO)
{
	CObjFormatSection* pOFS;

	fprintf(pO, "Name:%s\n" ,m_strName);
	pOFS = GetHead();
	while (pOFS)
	{
		pOFS->Print(pO, 2);
		pOFS = pOFS->GetNext();
	}
}

void CObjFormatFile::AddSection(CSection* pSection)
{
	CObjFormatSection* pOFS;

	pOFS = new CObjFormatSection;
	pOFS->Create(pSection);
	AddObjFormatSection(pOFS);
}

void CObjFormatFile::AddObjFormatSection(CObjFormatSection* pOFS)
{
	if (GetHead())
	{
		GetTail()->SetNext(pOFS);
		pOFS->SetPrev(GetTail());
		SetTail(pOFS);
	}
	else
	{
		SetTail(pOFS);
		SetHead(pOFS);
	}
}
