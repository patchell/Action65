#pragma once

class CAct65ASMstatement : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ASMstmt";
public:
	CAct65ASMstatement();
	virtual ~CAct65ASMstatement();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

