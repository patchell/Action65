#pragma once

class CChainTypeSpecItem : public CChainItem
{
public:
	enum class Spec {
		NONE,
		BYTE,
		CHAR,
		INT,
		CARD,
		BOOL,
		TYPE,
		TYPEDEF,
		ARRAY,
		POINTER,
		CONST,
		PROC,
		FUNC,
		INTERRUPT,
		LOCAL,
		PARAM,
		GLOBAL,
		TYPE_FIELD,
		POINTER_DREF,
		AREG,
		XREG,
		YREG,
		VIRTUAL_REG,
		CONSTANT
	};
private:
	struct Types {
		Spec m_SpecType;
		const char* m_pName;
		Types() {
			m_SpecType = Spec::NONE;
			m_pName = 0;
		}
		Types(Spec T, const char* pName) {
			m_SpecType = T;
			m_pName = pName;
		}
		static const char* FindName(Spec T);
	};
	static inline Types TypesLUT[] = {
		{Spec::NONE,"NONE"},
		{Spec::BYTE,"BYTE"},
		{Spec::CHAR,"CHAR"},
		{Spec::INT,"INT"},
		{Spec::CARD,"CARD"},
		{Spec::BOOL,"BOOL"},
		{Spec::TYPE,"TYPE"},
		{Spec::TYPEDEF,"TYPEDEF"},
		{Spec::ARRAY,"ARRAY"},
		{Spec::POINTER,"POINTER"},
		{Spec::CONST,"CONST"},
		{Spec::PROC,"PROC"},
		{Spec::FUNC,"FUNC"},
		{Spec::INTERRUPT,"INTERRUPT"},
		{Spec::LOCAL,"LOCAL"},
		{Spec::PARAM,"PARAM"},
		{Spec::GLOBAL,"GLOBAL"},
		{Spec::TYPE_FIELD,"Dereferenced POINTER"},
		{Spec::POINTER_DREF,"FIELD"},
		{Spec::AREG,"AREG"},
		{Spec::XREG,"XREG"},
		{Spec::YREG,"YREG"},
		{Spec::VIRTUAL_REG,"VIRTUAL REGister"},
		{Spec::CONSTANT,"CONSTANT"},
		{Spec(-1),0}
	};
	Spec m_SpecType;
public:
	CChainTypeSpecItem();
	virtual ~CChainTypeSpecItem();
	virtual bool Create();
	virtual void Copy(CChainItem* pI);
	virtual bool Compare(const char* pName);
	int Print(char* pSO, int l);
	const char* FindName(Spec T) {
		return Types::FindName(T);
	}
	Spec GetSpec() const { return m_SpecType; }
	void SetSpec(Spec s) { m_SpecType = s; }
	bool IsFundamentalType();
	bool Is(Spec SpecType) const;
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
};

