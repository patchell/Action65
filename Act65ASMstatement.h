#pragma once

class CAct65ASMstatement : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ASMstmt";
public:
	CAct65ASMstatement();
	virtual ~CAct65ASMstatement();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

