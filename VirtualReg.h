#pragma once

class CValue;

class CVirtualReg
{
	static inline int m_ID = 0;
	int m_MaxRegs;
	int* m_pLockTabel;
	CSection* m_pSection;	//memory section where virtual registers reside
	int m_BaseAddress;
public:
	enum RegStatus {
		FREE,
		LOCKED_BYTE,
		LOCKED_WORD
	};
public:
	//--------------------------------------
	// Virtual Register Object Definition
	//--------------------------------------
	class VREG {
		RegStatus m_nBytes;
		int m_TableIndex;
		char* m_pName;
		CSymbol* m_pSym;
	public:
		VREG() {
			m_nBytes = RegStatus::LOCKED_BYTE;
			m_TableIndex = 0;
			m_pName = 0;
			m_pSym = 0;
		}
		virtual ~VREG() {
			if (m_pName)
				delete[] m_pName;
		}
		bool Create(RegStatus Size, int Address, int TabelIndex);
		RegStatus GetSize() const { return m_nBytes; }
		int GetTableIndex() const { return m_TableIndex; }
		CSymbol* GetSymbol() { return m_pSym; }
		void SetSymbol(CSymbol * pSym) { m_pSym = pSym; }
	};
	//------------------------------------------
	// VirtualRegister Pool Methods
	//------------------------------------------
public:
	CVirtualReg();
	virtual ~CVirtualReg();
	bool Create(int RHeapSize, CSection* pSection);
	CValue* Lock(RegStatus size, CTypeChain* pTC = 0);
	bool UnLock(CValue* pVReg);
};
