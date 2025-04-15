#pragma once

class CVirtualReg
{
	inline static int m_ID = 0;
	int m_MaxRegs;
	int* m_pLockTabel;
	CSection* m_pSection;	//memory section where virtual registers reside
	int m_BaseAddress;
public:
	enum class RegSize {
		NONE,
		EIGHT_BITS,
		SIXTEEN_BITS
	};
public:
	//--------------------------------------
	// Virtual Register Object Definition
	//--------------------------------------
	struct VREG {
		RegSize m_nBytes;
		int m_RegID;
		char* m_pName;
		VREG() {
			m_nBytes = RegSize::EIGHT_BITS;
			m_RegID = 0;
			m_pName = 0;
		}
		virtual ~VREG() {
			if (m_pName)
				delete[] m_pName;
		}
		bool Create(RegSize Size, int Index) { 
			m_nBytes = Size; 
			m_RegID = ++m_ID;
			m_pName = new char[16];
			sprintf_s(m_pName, 16, "VR%d", Index);
		}
		RegSize GetSize() const { return m_nBytes; }
	};
	//------------------------------------------
	// VirtualRegister Pool Methods
	//------------------------------------------
public:
	CVirtualReg();
	virtual ~CVirtualReg();
	bool Create(int RHeapSize);
	CValue* Lock(RegSize size);
	bool UnLock(VREG* pReg);
};
