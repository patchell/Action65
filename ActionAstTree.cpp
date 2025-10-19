#include "pch.h"

CActionAstTree::CActionAstTree()
{
}

CActionAstTree::~CActionAstTree()
{
}

bool CActionAstTree::Create()
{
	return CAstTree::Create();
}

void CActionAstTree::Optimize()
{
	int Changes = 0;

	Changes = OptimizeTree(GetRootNode(), 100, 1000);
}

int CActionAstTree::OptimizeTree(
	CAstNode* pNode,
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
	int Changes = 0;

	--Recursions;
//	fprintf(Act()->LogFile(), "Node %5d::%s\n", pNode->GetID(), pNode->GetNodeName());
	if (Recursions <= 0)
	{
		fprintf(Act()->LogFile(), "Too Many Recursions in CAstTree::TraverseTree  Line:%d Col:%d\n",
			pNode->GetLine(),
			pNode->GetColumn()
		);
		Act()->CloseAll();
		ThrownException.SetXCeptType(Exception::ExceptionType::AST_RECURSION_LIMIT);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CActionAstTree::OptimizeTree: Too Many Recursions in CAstTree::TraverseTree  Line:%d Col:%d\n",
			pNode->GetLine(),
			pNode->GetColumn());
		throw(ThrownException);
	}

	pAN = pNode;
	while (pAN && pAN->GetNext())	/*	while next pointer points to valid node	*/
	{
		CheckNode(pAN);
		fprintf(Act()->LogFile(), "Node %5d::%s\n", pAN->GetID(), pAN->GetNodeName());
		if (pAN->GetChild())
		{
			OptimizeTree(
				pAN->GetChild(),
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
			fprintf(Act()->LogFile(), "Internal Error::Infinate Loop in CAstTree::TraverseTree  Line:%d Col:%d\n",
				pAN->GetLine(),
				pAN->GetColumn()
			);
			i = 0;
			while (Buffer[i])
			{
				if (Buffer[i]->GetNodeName())
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
			Act()->Exit(2);
		}
		pAN = pAN->GetNext();
	}
	CheckNode(pAN);
	if(pAN)
		fprintf(Act()->LogFile(), "Node %5d::%s\n", pAN->GetID(), pAN->GetNodeName());
	if (pAN->GetChild())
	{
		OptimizeTree(
			pAN->GetChild(),
			Recursions,
			MaxLoops
		);
	}
	Recursions++;
	return Changes;
}

void CActionAstTree::CheckNode(CAstNode* pNode)
{
	CAstNode* pChild = 0;
	CAstNode* pNext = 0;

	fprintf(Act()->LogFile(), "CheckNode:%s\n", pNode->GetNodeName());
	switch (pNode->GetNodeType())
	{
	case CAstNode::NodeType::ASSIGN_EQ:
		fprintf(Act()->LogFile(), "Optimize this NODE: %s\n", pNode->GetNodeName());
		pChild = pNode->GetChild();
		fprintf(Act()->LogFile(), "CheckNode::Child:%s\n", pChild->GetNodeName());;
		if (pChild->IsBinOpNode())
		{
			pNext = pNode->GetChild()->GetNext();
			pNode->GetChild()->SetNext(0);
			CAstNode::MakeChildList(pChild,pNext);
		}
		break;
	default:
		break;
	}
}
