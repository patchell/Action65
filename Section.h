#pragma once

class CAct65Opcode;

class CSection : public CBin
{
public:
	enum class Mode {
		MODE_READ_ONLY,
		MODE_READ_WRITE
	};
	enum class AddressSize {
		ADDRESSSIZE_WORD,
		ADDRESSSIZE_ZEROPAGE
	};
	inline static const char* ModeLUT[2] = {
		"MODE_READ_ONLY",
		"MODE_READ_WRITE"
	};
	inline static const char* AddressSizeLUT[2] = {
		"FALSE",
		"TRUE"
	};
private:
	unsigned m_StartAddress;	//where section starts in memory
	unsigned m_Size;		//max size of the memory block
	unsigned m_LocationCounter;	//current location to store data
	char* m_pSectionData;	//where the data is
	Mode m_AccessMode;	//read only or read write
	AddressSize m_ZeroPageAddressSize;
public:
	CSection();
	virtual ~CSection(); 
	bool Create();
	unsigned GetStartAddress() { return m_StartAddress; }
	void SetStartAddress(unsigned SA);
	unsigned GetSectionSize() { return m_Size; }
	void SetSectionSize(unsigned S);
	unsigned short GetLocationCounter() { return m_LocationCounter; }
	unsigned short IncrementLocationCounterBy(unsigned short v) { 
		m_LocationCounter += v;
		return m_LocationCounter;
	}
	void SetLocationCounter(unsigned short NewAddress);
	unsigned short AddInstruction(CAct65Opcode* pINS);
	unsigned short AddData(unsigned ObjectSize, int Value);
	unsigned short AddData(unsigned ObjSize, const char* pData);
	void AddDataAt(unsigned Adr, unsigned ObjectSize, int valu);
	unsigned short AllocateDataBlock(unsigned short size);	//define Storage
	void SetAccessMode(Mode Am) { m_AccessMode = Am; }
	CSection::Mode GetAccessMode() { return m_AccessMode; }
	void SetZeroPageFlag(AddressSize SizeFlag) { m_ZeroPageAddressSize = SizeFlag; }
	CSection::AddressSize GetZeroPageFlag() { return m_ZeroPageAddressSize; }
	virtual void Print(FILE* pOut, const char* s = 0);
	void Info();
	bool Compare(const char* name, int scope);
	bool IsPageZero() const {
		bool rV = false;

		if (m_ZeroPageAddressSize == AddressSize::ADDRESSSIZE_ZEROPAGE)
			rV = true;
		return rV;
	}
};

