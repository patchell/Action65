#pragma once

class CAct65STRING : public CAstNode
{
public:
	enum class StringType {
		ACTION_STRING,
		C_STRING
	};
private:
	struct StringTypeItem {
		StringType m_eType;
		const char* m_pName;
		StringTypeItem(){
			m_eType = StringType::ACTION_STRING;
			m_pName = 0;
		}
		StringTypeItem(StringType ST, const char* pS) {
			m_eType = ST;
			m_pName = pS;
		}
	};
	inline static StringTypeItem StringTypeLUT[2] = {
		{StringType::ACTION_STRING,"ACTION_STRING"},
		{StringType::C_STRING,"C_STRING"}
	};
	char* m_pString;
	StringType m_StringType;
	inline static const char* m_pNodeTyypeName = "STRING";
public:
	CAct65STRING();
	virtual ~CAct65STRING();
	const char* GetStringTypeName() const;
	void SetStringType(StringType ST) {
		m_StringType = ST;
	}
	StringType GetStringType() {
		return m_StringType;
	}
	char* GetString() {
		return m_pString;
	}
	void SetString(const char* s);
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

