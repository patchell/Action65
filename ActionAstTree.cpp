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
	CAstNode* pParent = 0;
	CAstNode* pResult = 0;
	CAstNode* pBinOp = 0;
	CAstNode* pAssign = 0;
	char* pStr = 0;
	int size = 0;
	int l = 2048;
	int ls = 0;

	pStr = new char[l];
	fprintf(Act()->LogFile(), "CheckNode:%s\n", pNode->GetNodeName());
	switch (pNode->GetNodeType())
	{
	case CAstNode::NodeType::ASSIGN_EQ:
		//---------------------------------
		// Optiming an assignment node:
		// The direct child node is going
		// to be a binary operator node.
		// The "next" node of the binary
		// operator node is going to be
		// where the reslut is stored.
		// The result node needs to be
		// moved to the end of the child
		// next list, and the assign node
		// needs to be removed from tree
		// by substituting the binary
		// operator node.  Is this clear
		// class?
		//---------------------------------
		pAssign = pNode;
		fprintf(Act()->LogFile(), "Optimize this NODE: %s\n", pAssign->GetNodeName());
		if (pAssign->GetChild()->IsBinOpNode())
		{
			pBinOp = pAssign->GetChild();
			pParent = pAssign->GetParent();
			pResult = pAssign->GetChild()->GetNext();
			size = l - ls;
			ls += pBinOp->Print(&pStr[ls], size, 4, 0, 0);
			if (pParent)
			{
			}
			else
			{
				fprintf(Act()->LogFile(), "***** Parent Node is NULL ********\n");
			}
		}
		break;
	default:
		break;
	}
	delete[] pStr;
}

int CActionAstTree::Eol(char* pStr, int size)
{
	int ls = 0;

	ls = sprintf_s(pStr, size, "\n");
	return ls;
}
