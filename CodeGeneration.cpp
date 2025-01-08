#include "pch.h"

CCodeGeneration::CCodeGeneration()
{
}

CCodeGeneration::~CCodeGeneration()
{
}

bool CCodeGeneration::Create(CAstTree* pAstTree)
{
	return false;
}

bool CCodeGeneration::Run()
{
	return false;
}

//------------------ Emit Operations -----------------

void CCodeGeneration::EmitBinary()
{
}

void CCodeGeneration::EmitSource()
{
}

CCodeGeneration::CodeTemplate::CodeTemplate()
{
	m_pName = 0;
}

CCodeGeneration::CodeTemplate::~CodeTemplate()
{
}

CCodeGeneration::CodeObject::CodeObject()
{
	m_pName = 0;
}

CCodeGeneration::CodeObject::~CodeObject()
{
}

CCodeGeneration::CodeTable::CodeTable()
{
}

CCodeGeneration::CodeTable::~CodeTable()
{
}
