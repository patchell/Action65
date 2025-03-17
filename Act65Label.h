#pragma once

class CAct65Label : public CAstNode
{
public:
	enum class LabelType {
		PRIVATE,
		GLOBAL
	};
private:
	LabelType m_LabelType;
	inline static const char* m_pNodeTyypeName = "LABEL";
public:
	CAct65Label();
	virtual ~CAct65Label();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	LabelType GetLabelType() { return m_LabelType; }
	void SetLabelType(LabelType LT) { m_LabelType = LT; }
	virtual bool IsLabel() { return true; }
};

