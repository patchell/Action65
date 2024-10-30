#pragma once


class CInstruction : public CStackItem
{
	//-------------------------------
	// Bytes are Little Endian Order
	//-------------------------------
	char m_OpCode;
	char m_LowByte;
	char m_HighByte;
	char m_unused;
	Token m_OpCodeToken;
	int m_ByteCount;
	bool m_UnResolved;
public:
	CInstruction();
	~CInstruction();
	bool Create();
	void SetByteCount(int n) { m_ByteCount = n; }
	int GetByteCount() { return m_ByteCount; }
	void SetOpCodeToken(Token Op) { m_OpCodeToken = Op; }
	Token GetOpCodeToken() { return m_OpCodeToken; }
	void SetOpcode(unsigned op) { m_OpCode = op; }
	void AddToOpCode(unsigned v) { m_OpCode += v; }
	char GetOpCode() { return m_OpCode; }
	bool IsUnResolved() { return m_UnResolved; }
	bool IsResolved() { return !m_UnResolved; }
	void SetUnResolved() { m_UnResolved = true; }
	void SetOperandW(unsigned v) {
		m_LowByte = v & 0xff;
		m_HighByte = (v & 0x0ff00) >> 8;
	}
	unsigned GetOperandW() {
		unsigned v;

		v = (m_LowByte & 0x0ff) | ((m_HighByte & 0x0ff) << 8);
		return v;
	}
	unsigned char GetLowByte() { return m_LowByte; }
	void SetLowByte(unsigned v) { m_LowByte = v; }
	unsigned char GetHighByte() { return m_HighByte;}
	void SetHighByte(unsigned v) { m_HighByte = v; }
	void AddToCurrentSection();
	bool IsRelative();
};

