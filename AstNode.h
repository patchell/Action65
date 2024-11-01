#pragma once

class CAstNode
{
	inline static int m_NodeCount = 0;
	int m_NodeID;
	int m_NodeType;
	char m_sNodeName[64];
	CAstNode* m_pStart;
	CAstNode* m_pNext;
	CAstNode* m_pPrev;
	CAstNode* m_pChild;
	CAstNode* m_pParent;
public:
	CAstNode();
	CAstNode(int NodeType);
	virtual ~CAstNode();
	bool Create(
		CAstNode* pChild,
		CAstNode* pNext,
		CAstNode* pPrev = 0,
		CAstNode* pStart = 0,
		CAstNode* pParent = 0
	);
	virtual void Print(FILE* pOut, int Indent);
	virtual void NodeProc();
	virtual void TopDown();
	virtual void BottomUp();
	virtual void Sideways();
	// Getter/Setter Methods
	CAstNode* GetStart() { return m_pStart; }
	CAstNode* GetNext() { return m_pNext; }
	CAstNode* GetPrev() { return m_pPrev; }
	CAstNode* GetChild() { return m_pChild; }
	CAstNode* GetParent() { return m_pParent; }
	void SetStart(CAstNode* pAN) { m_pStart = pAN; }
	void SetNext(CAstNode* pAN) { m_pNext = pAN; }
	void SetPrev(CAstNode* pAN) { m_pPrev = pAN; }
	void SetChild(CAstNode* pAN) { m_pChild = pAN; }
	void SetParent(CAstNode* pAN) { m_pParent = pAN; }
	//---------------------------------------------------
	int GetID() { return m_NodeID; }
	void SetID(int Id) { m_NodeID = Id; }
	int GetNodeType() { return m_NodeType; }
	void SetNodeType(int Nt) { m_NodeType = Nt; }
	char* GetNodeName() { return m_sNodeName; }
	void SetNodeName(const char* pName) {
		strcpy_s(m_sNodeName, 64, pName);
	}
};
