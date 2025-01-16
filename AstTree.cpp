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

	m_pRoot = new CAct65ROOT;
	return true;
}

void CAstTree::Print(FILE* pOut)
{
	char* s = 0;

	if (GetRootNode() && pOut)
	{
		s = new char[4096];
		memset(s, 0, 4096);
		fprintf(
			pOut,
			"\t NUMB  CHILD  NEXT\n"
		);
		TraverseTree(pOut, GetRootNode(), s, 4096, 0);
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
	int Indent
)
{
	//-------------------------------
	// Navigate the Syntax Tree
	//-------------------------------
	static int Recursions = 0;
	CAstNode* pAN = 0;
	int Loops = 0;

	if (++Recursions > 100)
	{
		fprintf(stderr, "Too Many Recursions\n");
		fprintf(pOut, "Too Many Recursions\n");
		Act()->CloseAll();
		exit(2);
	}
	
	//pAN = pNode->GetHead();
	//while (pAN)
	//{
	//	fprintf(Act()->GetParser()->LogFile(),"%d:Node Name:%s\n",pAN->GetID(),pAN->GetNodeName());
	//	pAN = pAN->GetNext();
	//}
	pAN = pNode;
	while (pAN && pAN->GetNext())	/*	while next pointer points to valid node	*/
	{
		pAN->Print(Indent,s,StringSize);
		fprintf(pOut, "%s\n", s);
		if (pAN->GetChild())
		{
			TraverseTree(pOut, pAN->GetChild(),s, StringSize, Indent+1);
		}
		pAN = pAN->GetNext();
		if (++Loops > 10)
		{
			Act()->CloseAll();
			exit(7);
		}
	}
	pAN->Print(Indent,s, StringSize);
	fprintf(pOut, "%s\n", s);
	if (pAN->GetChild())
	{
		TraverseTree(pOut, pAN->GetChild(),s, StringSize, Indent + 1);
	}
	Recursions--;
}

