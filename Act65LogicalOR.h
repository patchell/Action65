#pragma once

class CAct65LogicalOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "L-OR";
public:
	CAct65LogicalOR();
	virtual ~CAct65LogicalOR();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};
