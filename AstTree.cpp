#include "pch.h"

CAstTree::CAstTree()
{
	m_pRoot = 0;
}

CAstTree::~CAstTree()
{
}

bool CAstTree::Create()
{
	bool rV = true;

	return rV;
}

int CAstTree::PrintHeading(char* pSO, int l, int Indent, const char* s, bool bFull)
{
	int ls = 0;

	if (!bFull)
	{
		ls = sprintf_s(
			pSO,
			l,
			"%s\t NUMB  CHILD  NEXT\n",
			s ? s : ""
		);
	}
	else
	{
		ls = sprintf_s(
			pSO,
			l,
			"%s\t NUMB  CHILD  NEXT  PREV  PARENT\n",
			s ? s : ""
		);
	}
	return ls;
}

void CAstTree::Print(FILE* pOut)
{
	char* s = 0;
	bool* pbNextFlags = 0;
	int i = 0;
	int Indent = 0;

	pbNextFlags = new bool[2048];
	for (i = 0; i < 2048; ++i)
		pbNextFlags[i] = false;
	if (GetRootNode() && pOut)
	{
		s = new char[8192];
		memset(s, 0, 8192);
		fprintf(
			pOut,
			"\t NUMB  CHILD  NEXT\n"
		);
		if (GetRootNode()->GetNext())
			pbNextFlags[Indent] = true;
		TraverseTree(pOut, GetRootNode(), s, 4096, Indent, pbNextFlags);
		delete[] s;
	}
	else
	{
		if (Act()->LogFile())
			fprintf(Act()->LogFile(), "Root Node is NULL");
	}
}

void CAstTree::TraverseTree(
	FILE* pOut,
	CAstNode* pNode,
	char* s,
	int StringSize,
	int Indent,
	bool* pbNextFlags,
	int Recursions,
	int MaxLoops
)
{
	//-------------------------------
	// Navigate the Syntax Tree
	//-------------------------------
	CAstNode* pAN = 0;
	int Loops = MaxLoops;
	static CAstNode* Buffer[15] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};
	int i = 0;
	int l = 0;

	--Recursions;
	if (Recursions <= 0)
	{
		fprintf(Act()->LogFile(), 
			"CAstTree::TraverseTree: Too Many Recursions in:%s   Line:%d Col:%d\n",
			pNode->GetNodeName() ? pNode->GetNodeName() : "<unnamed>",
			pNode->GetLine(),
			pNode->GetColumn()
		);
		Act()->CloseAll();
		ThrownException.SetXCeptType(Exception::ExceptionType::AST_RECURSION_LIMIT);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CAstTree::TraverseTree: Too Many Recursions in:%s   Line:%d Col:%d\n",
			pNode->GetNodeName() ? pNode->GetNodeName() : "<unnamed>",
			pNode->GetLine(),
			pNode->GetColumn()
		);
		throw(ThrownException);
	}
	
	pAN = pNode;
	while (pAN && pAN->GetNext())	/*	while next pointer points to valid node	*/
	{
		if (pAN->GetNext())
			pbNextFlags[Indent] = true;
		else
			pbNextFlags[Indent] = false;
		l = pAN->Print(s, StringSize, Indent, 0, pbNextFlags);
		fprintf(pOut, "%s\n", s);
		if (pAN->GetChild())
		{
			if (pAN->GetNext())
				pbNextFlags[Indent] = true;
			else
				pbNextFlags[Indent] = false;
			TraverseTree(
				pOut, 
				pAN->GetChild(), 
				s, 
				StringSize, 
				Indent + 1, 
				pbNextFlags, 
				Recursions, 
				MaxLoops
			);
		}
		if (Loops < 10)
		{
			Buffer[i++] = pAN;
		}
		if (--Loops < 0)
		{
			fprintf(Act()->LogFile(), "Infinate Loop in CAstTree::TraverseTree  Line:%d Col:%d\n",
				pAN->GetLine(),
				pAN->GetColumn()
			);
			i = 0;
			while (Buffer[i])
			{
				if(Buffer[i]->GetNodeName())
					fprintf(Act()->LogFile(), "Node:%d Name:%s\n",
						Buffer[i]->GetID(),
						Buffer[i]->GetNodeName()
					);
				else
					fprintf(Act()->LogFile(), "Node:%d\n",
						Buffer[i]->GetID()	
					);
				++i;
			}
			ThrownException.SetXCeptType(Exception::ExceptionType::AST_RECURSION_LIMIT);
			sprintf_s(
				ThrownException.GetErrorString(),
				ThrownException.GetMaxStringLen(),
				"CAstTree::TraverseTree: Too Many Recursions in:%s   Line:%d Col:%d\n",
				pNode->GetNodeName() ? pNode->GetNodeName() : "<unnamed>", 
				pNode->GetLine(),
				pNode->GetColumn()
			);
			throw(ThrownException);
		}
		pAN = pAN->GetNext();
	}
	l = pAN->Print(s, StringSize, Indent, 0, pbNextFlags);
	fprintf(pOut, "%s\n", s);
	if (pAN->GetChild())
	{
		if (pAN->GetNext())
			pbNextFlags[Indent] = true;
		else
			pbNextFlags[Indent] = false;
		TraverseTree(
			pOut, 
			pAN->GetChild(), 
			s, 
			StringSize, 
			Indent + 1, 
			pbNextFlags, 
			Recursions, 
			MaxLoops
		);
	}
//	printf("Recursions:%5d  Loops:%5d\n", Recursions, Loops);
	pbNextFlags[Indent] = false;
	Recursions++;
}

void CAstTree::Run()
{
	try 
	{
		if (GetRootNode())
		{
			GetRootNode()->Process();
		}
		else
		{
			if (Act()->LogFile())
				fprintf(Act()->LogFile(), "Root Node is NULL");
		}
	}
	catch (Exception& BooBoo)
	{
		fprintf(Act()->LogFile(),
			"AST ERROR: %s SubType:%s: %s\n",
			Exception::FindExceptionTypeString(BooBoo.GetXCeptType()),
			Exception::FindExceptionSubTypeString(BooBoo.GetExceptionSubType()),
			BooBoo.GetErrorString()
		);
		Act()->CloseAll();
		Act()->Exit(1);
	}
}

CValue* CAstTree::Process()
{
    return GetRootNode()->Process();
}

