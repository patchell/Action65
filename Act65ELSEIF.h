#pragma once

class CAct65ELSEIF : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ELSEIF";
public:
	CAct65ELSEIF();
	virtual ~CAct65ELSEIF();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
};

