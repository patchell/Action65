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
		TYPEDEF,
		ARRAY,
		POINTER,
		CONST,
		PROC_SCOPE,
		FUNC_SCOPE,
		INTERRUPT_SCOPE,
		LOCAL_SCOPE,
		PARAM_SCOPE,
		GLOBAL_SCOPE,
		TYPE_FIELD,
		POINTER_DREF,
		AREG,
		XREG,
		YREG,
		VIRTUAL_REG,
		CONSTANT,
		END
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
		{Spec::TYPEDEF,"TYPE DEF"},
		{Spec::ARRAY,"ARRAY"},
		{Spec::POINTER,"POINTER"},
		{Spec::CONST,"CONST"},
		{Spec::PROC_SCOPE,"PROC SCOPE"},
		{Spec::FUNC_SCOPE,"FUNC SCOPE"},
		{Spec::INTERRUPT_SCOPE,"INTERRUPT SCOPE"},
		{Spec::LOCAL_SCOPE,"LOCAL SCOPE"},
		{Spec::PARAM_SCOPE,"PARAM SCOPE"},
		{Spec::GLOBAL_SCOPE,"GLOBAL SCOPE"},
		{Spec::TYPE_FIELD,"Dereferenced POINTER"},
		{Spec::POINTER_DREF,"FIELD"},
		{Spec::AREG,"AREG"},
		{Spec::XREG,"XREG"},
		{Spec::YREG,"YREG"},
		{Spec::VIRTUAL_REG,"VIRTUAL REGister"},
		{Spec::CONSTANT,"CONSTANT"},
		{Spec::END,0},
	};
	Spec m_SpecType;
public:
	CChainTypeSpecItem();
	virtual ~CChainTypeSpecItem();
	virtual bool Create();
	virtual void Copy(CChainItem* pI);
	virtual bool Compare(const char* pName);
	const char* FindName(Spec T) {
		return Types::FindName(T);
	}
	Spec GetSpec() const { 
		return m_SpecType; 
	}
	void SetSpec(Spec s) { m_SpecType = s; }
	bool IsFundamentalType();
	bool IsScopeType();
	bool Is(Spec SpecType) const;
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
	virtual void Emit(CSection* pSec) {}
	virtual void EmitListing(CSection* pSec) {}
};

