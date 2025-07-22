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


#pragma once

#include <std_text.h>


class Resident_Evil_2_ADT {
private:

	#define NODE_LEFT	0

	#define NODE_RIGHT	1

	struct unpackArray8_t
	{
		long start;
		long length;
	};

	struct node_t
	{
		unsigned long nodes[2];
	};

	struct unpackArray_t
	{
		unsigned long start;
		unsigned long length;
		unsigned long* ptr4;
		unpackArray8_t* ptr8;
		node_t* tree;
	};

	std::size_t srcOffset;

	std::uint8_t srcByte;

	std::int32_t srcNumBit;

	unpackArray_t array1;

	unpackArray_t array2;

	unpackArray_t array3;

	std::vector<std::uint16_t> freqArray;

	void initTmpArray(unpackArray_t* array, std::vector<std::uint8_t>& tmp32k, unsigned long start, unsigned long length);

	void initTmpArrayData(unpackArray_t* array);

	std::int32_t readSrcBits(std::vector<std::uint8_t> src, std::int32_t numBits);

	std::int32_t readSrcOneBit(std::vector<std::uint8_t> src);

	std::int32_t readSrcBitfieldArray(std::vector<std::uint8_t> src, unpackArray_t* array, unsigned long curIndex);

	std::int32_t readSrcBitfield(std::vector<std::uint8_t> src);

	void initUnpackBlockArray(unpackArray_t* array) const;

	std::int32_t initUnpackBlockArray2(unpackArray_t* array);

	void initUnpackBlock(std::vector<std::uint8_t> src);

public:

	/*
		Construction
	*/
	explicit Resident_Evil_2_ADT(void) :
		srcOffset(0),
		srcByte(0),
		srcNumBit(0),
		array1{},
		array2{},
		array3{}
	{
	}
	virtual ~Resident_Evil_2_ADT(void)
	{
	}

	/*
		Decompress ADT from unsigned char vector
	*/
	bool Decompress(std::vector<std::uint8_t> src, std::vector<std::uint8_t>& dstBufPtr);

	/*
		Decompress ADT from file
	*/
	bool Decompress(std::filesystem::path Input, std::filesystem::path Output);

};