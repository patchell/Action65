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
	bool* pbNextFlags = 0;
	int i = 0;
	int Indent = 0;

	pbNextFlags = new bool[2048];
	for (i = 0; i < 2048; ++i)
		pbNextFlags[i] = false;
	if (GetRootNode() && pOut)
	{
		s = new char[4096];
		memset(s, 0, 4096);
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

	--Recursions;
	if (Recursions <= 0)
	{
		fprintf(Act()->LogFile(), "Too Many Recursions in CAstTree::TraverseTree  Line:%d Col:%d\n",
			pNode->GetLine(),
			pNode->GetColumn()
		);
		Act()->CloseAll();
		Act()->Exit(2);
	}
	
	pAN = pNode;
	while (pAN && pAN->GetNext())	/*	while next pointer points to valid node	*/
	{
		if (pAN->GetNext())
			pbNextFlags[Indent] = true;
		else
			pbNextFlags[Indent] = false;
		pAN->Print(Indent, s, StringSize, pbNextFlags);
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
			Act()->CloseAll();
			Act()->Exit(7);
		}
		pAN = pAN->GetNext();
	}
	pAN->Print(Indent, s, StringSize, pbNextFlags);
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

