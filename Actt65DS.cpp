#include "pch.h"

CAct65DS::CAct65DS()
{
}

CAct65DS::~CAct65DS()
{
}

bool CAct65DS::Create(CAstNode* pChild, CAstNode* pNext)
{
    return false;
}

CValue* CAct65DS::Process()
{
    return nullptr;
}

void CAct65DS::Print(FILE* pOut, int Indent, char* s, int l)
{
	//char* pIndentStr = new char[256];
	//int i = 0, l = 0;
	//int Id, Child, Next;

	//Id = GetID();
	//if (GetChild())
	//	Child = GetChild()->GetID();
	//else
	//	Child = -1;
	//if (GetNext())
	//	Next = GetNext()->GetID();
	//else
	//	Next = -1;
	//l += sprintf_s(pIndentStr, 256, "%6d %6d %6d ", Id, Child, Next);
	//for (i = 0; i < Indent; ++i)
	//{
	//	l += sprintf_s(&pIndentStr[l], 256 - l, "%| ");
	//}
	//l += sprintf_s(&pIndentStr[l], 256 - l, "%+-");
	//fprintf(pOut, "%s%s\n",
	//	pIndentStr,
	//	GetNodeName()
	//);
	CAstNode::Print(pOut, Indent + 1, s, l);
	//delete[] pIndentStr;
}
