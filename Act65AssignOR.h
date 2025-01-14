#pragma once

class CAct65AssignOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=OR";
public:
	CAct65AssignOR();
	virtual ~CAct65AssignOR();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

