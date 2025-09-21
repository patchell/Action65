#pragma once


class CReg
{
public:
	enum class RegType {
		NONE,
		A,
		X,
		Y,
		S,
		P
	};
private:
	struct RegTypeItem
	{
		RegType m_Reg;
		const char* m_pName;
		const char* GetRegName(RegType Reg);
	};
	inline static RegTypeItem RegTypeLUT[] = {
		{RegType::NONE,"NONE"},
		{RegType::A,".A"},
		{RegType::X,".X"},
		{RegType::Y,".Y"},
		{RegType::S,".S"},
		{RegType::P,".P"}
	};
	bool m_InUse;
	RegType m_Type;
	CTypeChain* m_pTypeChain;
public:
	CReg() {
		m_InUse = false;
		m_Type = RegType::NONE;
		m_pTypeChain = 0;
	}
	virtual ~CReg() {}
	void Lock() { m_InUse = true; }
	void UnLock() { m_InUse = false; }
	bool IsUnLocked() const { return !m_InUse; }
	void SetType(RegType Type) { m_Type = Type; }
	RegType GetType() const { return m_Type; }
	const char* ToString() const { return RegTypeLUT[int(m_Type)].m_pName; }
	CTypeChain* GetTypeChain() {
		return m_pTypeChain;
	}
	bool CreateTypeChain(CTypeChain* pTC = 0);
};

