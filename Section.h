#pragma once

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
	unsigned short AddData(unsigned ObjectSize, int Value);
	void AddDataAt(unsigned Adr, unsigned ObjectSize, int valu);
	unsigned short AllocateDataBlock(unsigned short size);	//define Storage
	void SetAccessMode(Mode Am) { m_AccessMode = Am; }
	CSection::Mode GetAccessMode() { return m_AccessMode; }
	void SetZeroPageFlag(AddressSize SizeFlag) { m_ZeroPageAddressSize = SizeFlag; }
	CSection::AddressSize GetZeroPageFlag() { return m_ZeroPageAddressSize; }
	virtual void Print(FILE* pOut, const char* s = 0);
	void Info();
};

