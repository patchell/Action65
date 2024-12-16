#pragma once

class CAstNode
{
	inline static int m_NodeCount = 0;
	int m_NodeID;
	int m_NodeType;
	const char* m_pNodeName;
	CAstNode* m_pStart;
	CAstNode* m_pNext;
	CAstNode* m_pPrev;
	CAstNode* m_pHead;
	CAstNode* m_pTail;
	CAstNode* m_pChild;
	CAstNode* m_pParent;
	CValue* m_pValue;
public:
	CAstNode();
	CAstNode(int NodeType);
	virtual ~CAstNode();
	bool CreateNode(
		CAstNode* pChild = 0,
		CAstNode* pNext = 0
	);
	virtual CValue* Process() = 0;
	virtual void Print(FILE* pOut, int Indent, char* s);
	void AddToHead(CAstNode* pN);
	void AddToTail(CAstNode* pN);
	// Getter/Setter Methods
	CAstNode* GetHead() { return m_pHead; }
	CAstNode* GetTail() { return m_pTail; }
	CAstNode* GetStart() { return m_pStart; }
	CAstNode* GetNext() { return m_pNext; }
	CAstNode* GetPrev() { return m_pPrev; }
	CAstNode* GetChild() { return m_pChild; }
	CAstNode* GetParent() { return m_pParent; }
	void SetHead(CAstNode* pAN) { m_pHead = pAN; }
	void SetTail(CAstNode* pAN) { m_pTail = pAN; }
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
	const char* GetNodeName() { return m_pNodeName; }
	void SetNodeName(const char* pName) {
		m_pNodeName = pName;
	}
	bool SetSymbol(CBin* pSym) {
		bool rV = true;

		if (m_pValue)
			m_pValue->SetSymbol(pSym);
		else
			rV = false;
		return rV;
	}
	CBin* GetSymbol() { 
		CBin* pSym = 0;

		if (m_pValue)
			pSym = m_pValue->GetSymbol();
		return pSym; 
	}
	void SetValue(CValue* pV) { m_pValue = pV; }
	CValue* GetVvalue() {
		return m_pValue
			;
	}
};
