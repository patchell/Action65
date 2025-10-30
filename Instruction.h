#pragma once

class CInstruction
{
	char m_Data[3];
	KeyWord* m_pKeyWord;
	AdrModeType m_AdrMode;
	int m_numBytes;
	int m_Address;	//address where instruction is stored
	CValue* m_pLabel;
	CValue* m_pOperand;
public:
	CInstruction() {
		m_Data[0] = 0;
		m_Data[1] = 0;
		m_Data[2] = 0;
		m_pKeyWord = 0;
		m_AdrMode = AdrModeType::NA;
		m_numBytes = 0;
		m_Address = 0;
		m_pLabel = 0;
		m_pOperand = 0;
	}
	bool GenInstruction(
		Token OpToken,
		AdrModeType AdrMode,
		CValue* pOperand,
		CValue* pLabel,
		int Address
	);
	int GetNumBytes() const { return m_numBytes; }
	char* GetData() { return m_Data; }
	int GetData(int i) const {
		if (i >= 0 && i < 3)
			return (unsigned char)m_Data[i];
		else
			return -1;
	}
	KeyWord* GetKeyWord() { return m_pKeyWord; }
	AdrModeType GetAdrMode() const { return m_AdrMode; }
	CValue* GetLabel() { return m_pLabel; }
	CValue* GetOperand() { return m_pOperand; }
	void SetOperand(CValue* pV) { m_pOperand = pV; }
	void SetLabel(CValue* pL) { m_pLabel = pL; }
	void SetAdrMode(AdrModeType AM) { m_AdrMode = AM; }
	void SetKeyWord(KeyWord* pKW) { m_pKeyWord = pKW; }
	void SetNumBytes(int n) { m_numBytes = n; }
	void SetData(int i, char d) {
		if (i >= 0 && i < 3)
			m_Data[i] = d;
	}
	void SetAddress(int A) { m_Address = A; }
	int GetAddress() const { return m_Address; }
	void EmitListing();
	int Print(char* pSO, int l, int Indent, const char* s);
	char* GenOperand(char* s, int n);
	virtual int PrintBinary(char* pSO, int l, int Indent, const char* s);

};

