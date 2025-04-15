#pragma once

class CObjTypeChain
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
		YREG
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
		{Spec(-1),0}
	};
	Spec m_SpecType;
	CObjTypeChain* m_pNext;
	CObjTypeChain* m_pPrev;
public:
	CObjTypeChain();
	virtual ~CObjTypeChain();
	bool Create();
	int Print(char* pSO, int l);
	const char* FindName(Spec T) {
		return Types::FindName(T);
	}
	CObjTypeChain* GetNext() { return m_pNext; }
	void SetNext(CObjTypeChain* pN) { m_pNext = pN; }
	CObjTypeChain* GetPrev() { return m_pPrev; }
	void SetPrev(CObjTypeChain* pP) { m_pPrev = pP; }
	Spec GetSpec() const { return m_SpecType; }
	void SetSpec(Spec s) { m_SpecType = s; }
	bool IsFundamentalType();
	bool Is(Spec SpecType) const;
};

