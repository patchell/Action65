#pragma once

class CReg;

class CCodeGeneration
{
public:
	enum ByteOrder {
		LOWBYTE,
		HIGHBYTE
	};
public:
	class CodeTemplate {
		char* m_pName;
	public:
		CodeTemplate();
		virtual ~CodeTemplate();
	};
	class CodeObject {
		const char* m_pName;
	public:
		CodeObject();
		virtual ~CodeObject();
	};
	class CodeTable {
	public:
		CodeTable();
		virtual ~CodeTable();
	};
	CVirtualReg m_VirtualRegisterPool;
	CValue* m_pPendingLabel;
public:
	CCodeGeneration();
	virtual ~CCodeGeneration();
	bool Create(int VirtRegPoolSize, CSection* pVirtRegSection);
	CVirtualReg* GetVirtRegPool() {
		return &m_VirtualRegisterPool;
	}
	//---------------------------
	CValue* EmitBinaryOp(
		Token Op, 
		CValue* pV1, 
		CValue* pV2, 
		CValue* pV3, 
		CSection* pSection,
		Token OpAux = Token::NONE
	);
	CValue* EmitShift(
		Token Op,
		CValue* pV1,
		CValue* pV2,
		CValue* pV3,
		CSection* pSection,
		Token OpAux = Token::NONE
	);
	CValue* EmitMult(
		Token Op,
		CValue* pV1,
		CValue* pV2,
		CValue* pV3,
		CSection* pSection,
		Token OpAux = Token::NONE
	);
	CValue* EmitLogical(
		Token Op,
		CValue* pV1,
		CValue* pV2,
		CValue* pV3,
		CSection* pSection,
		Token OpAux = Token::NONE
	);
	CValue* EmitCompare(
		Token Op,
		CValue* pV1,
		CValue* pV2,
		CValue* pV3,
		CSection* pSection,
		Token OpAux = Token::NONE
	);
	//----------------------------------------------------------
	void EmitInstruction(
		Token OpToken,
		AdrModeType AddressMode,
		CValue* pOperandValue,
		CSection* pSection,
		CValue* pLabel
	);
	CValue* EmitImplied(Token Op, CSection* pSection, CValue* pLabel = 0);
	CValue* EmitAccumulator(Token Op, CSection* pSection, CValue* pLabel = 0);
	CValue* EmitImmediate(Token Op, CValue* pVsrc, int Byte, CSection* pSection, CValue* pLabel = 0);
	CValue* EmitRelative(Token Op, CValue* pVsrc, CSection* pSection, CValue* pLabel = 0);
	CValue* EmitDirect(Token Op, CValue* pVdest, int Byte, CSection* pSection, CValue* pLabel = 0);
	CValue* EmitIndirect(Token Op, CValue* pVdestPointer, int Byte, CSection* pSection, CValue* pLabel = 0, bool IncYreg = false);
	CValue* EmitIndexed(Token Op, CValue* pVdest, CValue* pIndex, int Byte, CSection* pSection, CValue* pLabel = 0);
	//----------------------------------------------------------
	bool SetPendingLabel(CValue* pPendingLabel);
	CValue* GetPendingLabel();
	void EmitSource();
	void EmitListing(Token Op, CSection* pSection, int Address, CValue* pLabel = 0);
};

