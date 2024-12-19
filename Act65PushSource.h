#pragma once

class CAct65PushSource : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PUSH-SRC";
public:
	CAct65PushSource();
	virtual ~CAct65PushSource();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

