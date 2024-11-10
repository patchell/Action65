#pragma once

class CAct65BitWiseOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BW-OR";
public:
	CAct65BitWiseOR();
	virtual ~CAct65BitWiseOR();
	virtual CAstNode* Process(
		CAstNode* pChild,
		CAstNode* pNext
	);
};

