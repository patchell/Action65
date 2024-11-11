#pragma once

class CAct65SecAtrbREADWRITE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "R/W";
public:
	CAct65SecAtrbREADWRITE();
	virtual ~CAct65SecAtrbREADWRITE();
	bool Create();
};

