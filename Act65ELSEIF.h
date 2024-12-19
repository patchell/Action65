#pragma once

class CAct65ELSEIF : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ELSEIF";
public:
	CAct65ELSEIF();
	virtual ~CAct65ELSEIF();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

