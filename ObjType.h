#pragma once

class CObjType
{
public:
	enum class Spec {
		NONE,
		BYTE,
		CHAR,
		INT,
		CARD,
		BOOL,
		TYPEDEF,
		ARRAY,
		POINTER,
		PROC,
		FUNC
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
		{Spec::TYPEDEF,"TYPEDEF"},
		{Spec::ARRAY,"ARRAY"},
		{Spec::POINTER,"POINTER"},
		{Spec::PROC,"PROC"},
		{Spec::FUNC,"FUNC"},
		{Spec(-1),0}
	};
	Spec m_SpecType;
	CObjType* m_pNext;
	CObjType* m_pPrev;
public:
	CObjType();
	virtual ~CObjType();
	bool Create();
	const char* FindName(Spec T) {
		return Types::FindName(T);
	}
	CObjType* GetNext() { return m_pNext; }
	void SetNext(CObjType* pN) { m_pNext = pN; }
	CObjType* GetPrev() { return m_pPrev; }
	void SetPrev(CObjType* pP) { m_pPrev = pP; }
};

