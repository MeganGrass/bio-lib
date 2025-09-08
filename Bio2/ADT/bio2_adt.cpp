/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*
*	CREDIT:
*
*		ADT Decompression: https://github.com/pmandin/reevengi-tools
*
*/


#include "bio2_adt.h"


void Resident_Evil_2_ADT::initTmpArray(unpackArray_t* array, std::vector<std::uint8_t>& tmp32k, unsigned long start, unsigned long length)
{
	array->start = start;

	array->length = length;

	std::size_t tmp32kOffset = 0;

	array->tree = (node_t*)&tmp32k[tmp32kOffset];
	tmp32kOffset += static_cast<unsigned long long>(length) * 2 * sizeof(node_t);

	array->ptr8 = (unpackArray8_t*)&tmp32k[tmp32kOffset];
	tmp32kOffset += array->length * sizeof(unpackArray8_t);

	array->ptr4 = (unsigned long*)&tmp32k[tmp32kOffset];
	tmp32kOffset += array->length * sizeof(unsigned long);
}


void Resident_Evil_2_ADT::initTmpArrayData(unpackArray_t* array)
{
	unsigned long i = 0;

	for (unsigned long i = 0; i < array->length; i++)
	{
		array->ptr4[i] =
			array->ptr8[i].start =
			array->ptr8[i].length =
			array->tree[i].nodes[NODE_LEFT] =
			array->tree[i].nodes[NODE_RIGHT] = -1;
	}

	while (i < array->length << 1)
	{
		array->tree[i].nodes[NODE_LEFT] =
			array->tree[i].nodes[NODE_RIGHT] = -1;
		i++;
	}
}


std::int32_t Resident_Evil_2_ADT::readSrcBits(std::vector<std::uint8_t> src, std::int32_t numBits)
{
	std::int32_t orMask = 0, andMask;
	std::int32_t finalValue;

	finalValue = srcByte;

	while (numBits > srcNumBit)
	{
		numBits -= srcNumBit;
		andMask = (1 << srcNumBit) - 1;
		andMask &= finalValue;
		andMask <<= numBits;
		if (srcOffset >= src.size())
		{
			srcByte = 0;
		}
		else
		{
			srcByte = src[srcOffset++];
		}
		finalValue = srcByte;
		srcNumBit = 8;
		orMask |= andMask;
	}

	srcNumBit -= numBits;
	finalValue >>= srcNumBit;
	finalValue = (finalValue & ((1 << numBits) - 1)) | orMask;
	return finalValue;
}


std::int32_t Resident_Evil_2_ADT::readSrcOneBit(std::vector<std::uint8_t> src)
{
	srcNumBit--;

	if (srcNumBit < 0)
	{
		srcNumBit = 7;
		
		if (srcOffset >= src.size())
		{
			srcByte = 0;
		}
		else
		{
			srcByte = src[srcOffset++];
		}
	}

	return (srcByte >> srcNumBit) & 1;
}


std::int32_t Resident_Evil_2_ADT::readSrcBitfieldArray(std::vector<std::uint8_t> src, unpackArray_t* array, unsigned long curIndex)
{
	do {
		if (readSrcOneBit(src))
		{
			curIndex = array->tree[curIndex].nodes[NODE_RIGHT];
		}
		else
		{
			curIndex = array->tree[curIndex].nodes[NODE_LEFT];
		}
	} while (curIndex >= array->length);

	return curIndex;
}


std::int32_t Resident_Evil_2_ADT::readSrcBitfield(std::vector<std::uint8_t> src)
{
	std::int32_t numZeroBits = 0;

	std::int32_t bitfieldValue = 1;

	while (readSrcOneBit(src) == 0)
	{
		numZeroBits++;
	}

	while (numZeroBits > 0)
	{
		bitfieldValue = readSrcOneBit(src) + (bitfieldValue << 1);

		numZeroBits--;
	}

	return bitfieldValue;
}


void Resident_Evil_2_ADT::initUnpackBlockArray(unpackArray_t* array) const
{
	std::vector<std::uint16_t> tmp(18);

	for (std::int32_t i = 0; i < 16; i++)
	{
		tmp.data()[i + 2] = (tmp.data()[i + 1] + freqArray.data()[i + 1]) << 1;
	}

	for (std::size_t i = 0; i < tmp.size(); i++)
	{
		for (unsigned long j = 0; j < array->length; j++)
		{
			if (array->ptr8[j].length == i)
			{
				array->ptr8[j].start = tmp.data()[i]++ & 0xFFFF;
			}
		}
	}
}


std::int32_t Resident_Evil_2_ADT::initUnpackBlockArray2(unpackArray_t* array)
{
	unsigned long Len = array->length;

	unsigned long Index = Len + 1;

	array->tree[Len].nodes[NODE_LEFT] =
		array->tree[Len].nodes[NODE_RIGHT] =
		array->tree[Index].nodes[NODE_LEFT] =
		array->tree[Index].nodes[NODE_RIGHT] = -1;

	for (unsigned long i = 0; i < array->length; i++)
	{
		Len = array->length;

		for (long j = 0; j < array->ptr8[i].length; j++)
		{
			long curMask = 1 << (array->ptr8[i].length - j - 1);

			long arrayOffset;

			if ((curMask & array->ptr8[i].start) != 0)
			{
				arrayOffset = NODE_RIGHT;
			}
			else
			{
				arrayOffset = NODE_LEFT;
			}

			if (j + 1 == array->ptr8[i].length)
			{
				array->tree[Len].nodes[arrayOffset] = i;
				break;
			}

			if (array->tree[Len].nodes[arrayOffset] == -1)
			{
				array->tree[Len].nodes[arrayOffset] = Index;
				array->tree[Index].nodes[NODE_LEFT] = array->tree[Index].nodes[NODE_RIGHT] = -1;
				Len = Index++;
			}
			else
			{
				Len = array->tree[Len].nodes[arrayOffset];
			}
		}
	}

	return array->length;
}


void Resident_Evil_2_ADT::initUnpackBlock(std::vector<std::uint8_t> src)
{
	std::uint16_t j = 0;

	unsigned long x = 0;

	int prevValue, curBit, curBitfield;

	int numValues;

	std::vector<std::uint16_t> tmp(512);

	unsigned long tmpBufLen;

	freqArray.resize(17);

	/* Initialize array 1 to unpack block */
	prevValue = 0;
	for (unsigned long i = 0; i < array1.length; i++)
	{
		if (readSrcOneBit(src))
		{
			array1.ptr8[i].length = readSrcBitfield(src) ^ prevValue;
		}
		else
		{
			array1.ptr8[i].length = prevValue;
		}
		prevValue = array1.ptr8[i].length;
	}

	/* Count frequency of values in array 1 */
	std::memset(&freqArray.data()[0], 0, (sizeof(std::uint16_t) * freqArray.size()));

	for (unsigned long i = 0; i < array1.length; i++)
	{
		numValues = array1.ptr8[i].length;
		if (numValues <= 16)
		{
			freqArray[numValues]++;
		}
	}

	initUnpackBlockArray(&array1);

	tmpBufLen = initUnpackBlockArray2(&array1);

	/* Initialize array 2 to unpack block */
	if (array2.length > 0)
	{
		std::memset(&tmp.data()[0], 0, array2.length);
	}

	curBit = readSrcOneBit(src);
	while (x < array2.length)
	{
		if (curBit)
		{
			curBitfield = readSrcBitfield(src);

			for (std::int32_t i = 0; i < curBitfield; i++)
			{
				tmp.data()[x + i] = readSrcBitfieldArray(src, &array1, tmpBufLen);
			}

			x += curBitfield;

			curBit = 0;

			continue;
		}

		curBitfield = readSrcBitfield(src);

		if (curBitfield > 0)
		{
			std::memset(&tmp.data()[x], 0, curBitfield * sizeof(unsigned short));

			x += curBitfield;
		}

		curBit = 1;
	}

	for (unsigned long i = 0; i < array2.length; i++)
	{
		j = j ^ tmp.data()[i];
		array2.ptr8[i].length = j;
	}

	/* Count frequency of values in array 2 */
	std::memset(&freqArray.data()[0], 0, (sizeof(std::uint16_t) * freqArray.size()));

	for (unsigned long i = 0; i < array2.length; i++)
	{
		numValues = array2.ptr8[i].length;
		if (numValues <= 16)
		{
			freqArray[numValues]++;
		}
	}

	initUnpackBlockArray(&array2);

	/* Initialize array 3 to unpack block */
	prevValue = 0;
	for (unsigned long i = 0; i < array3.length; i++)
	{
		if (readSrcOneBit(src))
		{
			array3.ptr8[i].length = readSrcBitfield(src) ^ prevValue;
		}
		else
		{
			array3.ptr8[i].length = prevValue;
		}
		prevValue = array3.ptr8[i].length;
	}

	/* Count frequency of values in array 3 */
	std::memset(&freqArray.data()[0], 0, (sizeof(std::uint16_t) * freqArray.size()));

	for (unsigned long i = 0; i < array3.length; i++)
	{
		numValues = array3.ptr8[i].length;
		if (numValues <= 16)
		{
			freqArray[numValues]++;
		}
	}

	initUnpackBlockArray(&array3);
}


/*
	Decompress ADT
*/
bool Resident_Evil_2_ADT::Decompress(std::vector<std::uint8_t> src, std::vector<std::uint8_t>& dstBufPtr)
{
	std::int32_t blockLength = 0;

	std::size_t dstBufLen = 0;

	std::size_t dstOffset = 0;

	std::vector<std::uint8_t> tmp16k(0x4000);

	std::vector<std::uint8_t> tmp32k(0x4000);

	std::size_t tmp16kOffset = 0;

	if (dstBufPtr.size() < 0x8000) { dstBufPtr.resize(0x8000); }

	srcOffset = 0x04;

	srcByte = 0x00;

	srcNumBit = 0;

	initTmpArray(&array1, tmp32k, 8, 0x10);
	initTmpArray(&array2, tmp32k, 8, 0x200);
	initTmpArray(&array3, tmp32k, 8, 0x10);

	initTmpArrayData(&array1);
	initTmpArrayData(&array2);
	initTmpArrayData(&array3);

	blockLength = readSrcBits(src, 8);

	blockLength |= readSrcBits(src, 8) << 8;

	while ((blockLength > 0) && (src[srcOffset] != 0) && (srcOffset < src.size()))
	{
		initUnpackBlock(src);

		std::int32_t tmpBufLen = initUnpackBlockArray2(&array2);

		std::int32_t tmpBufLen1 = initUnpackBlockArray2(&array3);

		std::int32_t curBlockLength = 0;

		while (curBlockLength < blockLength)
		{
			std::int32_t curBitfield = readSrcBitfieldArray(src, &array2, tmpBufLen);

			if (curBitfield < 0x100)
			{
				if (dstOffset + 1 > dstBufLen)
				{
					dstBufLen += 0x8000;

					dstBufPtr.resize(dstBufLen);
				}

				dstBufPtr.data()[dstOffset++] = tmp16k[tmp16kOffset++] = curBitfield;

				tmp16kOffset &= 0x3FFF;
			}
			else
			{
				std::int32_t numValues = curBitfield - 0xFD;

				curBitfield = readSrcBitfieldArray(src, &array3, tmpBufLen1);

				if (curBitfield != 0)
				{
					std::int32_t numBits = curBitfield - 1;

					curBitfield = readSrcBits(src, numBits) & 0xFFFF;

					curBitfield += 1 << numBits;
				}

				if (dstOffset + numValues > dstBufLen)
				{
					dstBufLen += 0x8000;

					dstBufPtr.resize(dstBufLen);
				}

				std::int32_t startOffset = (tmp16kOffset - curBitfield - 1) & 0x3FFF;

				for (std::int32_t i = 0; i < numValues; i++)
				{
					dstBufPtr.data()[dstOffset++] = tmp16k[tmp16kOffset++] = tmp16k[startOffset++];

					startOffset &= 0x3FFF;

					tmp16kOffset &= 0x3FFF;
				}
			}

			curBlockLength++;
		}

		blockLength = readSrcBits(src, 8);

		blockLength |= readSrcBits(src, 8) << 8;
	}

	tmp16k.clear();
	tmp16k.shrink_to_fit();

	tmp32k.clear();
	tmp32k.shrink_to_fit();

	dstBufPtr.resize(dstBufLen);

	return true;
}


/*
	Decompress ADT from file
*/
bool Resident_Evil_2_ADT::Decompress(std::filesystem::path Input, std::filesystem::path Output)
{
	Standard_String Str;

	StdFile m_Input{ Input, FileAccessMode::Read_Ex, true, false };
	if (!m_Input)
	{
		Str.Message("ADT Compression: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::vector<std::uint8_t> ADT(m_Input.Size());
	m_Input.Read(0, ADT.data(), m_Input.Size());
	m_Input.Close();

	std::vector<std::uint8_t> Binary;

	if (!Decompress(ADT, Binary))
	{
		Str.Message("ADT Compression: Error, could not decompress %s", Input.filename().string().c_str());
		return false;
	}

	StdFile m_Output{ Output, FileAccessMode::Write_Ex, true, true };
	if (!m_Output)
	{
		Str.Message("ADT Compression: Error, could not create %s", Output.filename().string().c_str());
		return false;
	}
	m_Output.Write(0, Binary.data(), Binary.size());
	m_Output.Close();

	return true;
}