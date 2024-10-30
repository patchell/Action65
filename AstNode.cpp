#include "pch.h"

CAstNode::CAstNode()
{
	m_NodeID = ++m_NodeCount;
	m_NodeType = 0;
	m_pStart = 0;
	m_pNext = 0;
	m_pPrev = 0;
	m_pChild = 0;
	m_pParent = 0;
	for (int i = 0; i < 64; ++i)
		m_sNodeName[i] = 0;
}

CAstNode::CAstNode(int NodeType)
{
	m_NodeID = ++m_NodeCount;
	m_NodeType = NodeType;
	m_pStart = 0;
	m_pNext = 0;
	m_pPrev = 0;
	m_pChild = 0;
	m_pParent = 0;
	for (int i = 0; i < 64; ++i)
		m_sNodeName[i] = 0;
}

CAstNode::~CAstNode()
{
}

bool CAstNode::Create(
	CAstNode* pChild,
	CAstNode* pNext,
	CAstNode* pPrev,
	CAstNode* pStart,
	CAstNode* pParent
)
{
    return false;
}

void CAstNode::Print(FILE* pOut, int Indent)
{
}

void CAstNode::NodeProc()
{
	TopDown();
	if (m_pChild)
		m_pChild->NodeProc();
	Sideways();
	if (m_pNext)m_pNext->NodeProc();
	BottomUp();
}

void CAstNode::TopDown()
{
}

void CAstNode::BottomUp()
{
}

void CAstNode::Sideways()
{
}
