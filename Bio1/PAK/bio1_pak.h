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
*		PAK Decompression: https://github.com/pmandin/reevengi-tools
*
*/


#pragma once

#include <std_text.h>

#include <sony_texture.h>


class Resident_Evil_PAK {
private:

	#define RE1_PAK_DECODE_SIZE	35024

	std::uint8_t tmpMask;

	std::uint8_t srcByte;

	std::size_t srcOffset;

	std::size_t dstBufLen;

	std::size_t dstOffset;

	struct re1_pack_t {
		long flag;
		long index;
		long value;
	};

	std::vector<re1_pack_t> tmpArray2;

	std::vector<std::uint8_t> decodeStack;

	std::int32_t read_bits(std::vector<std::uint8_t> src, std::int32_t num_bits);

	std::int32_t decodeString(std::int32_t decodeStackOffset, unsigned long code);

public:

	/*
		Construction
	*/
	explicit Resident_Evil_PAK(void) :
		tmpMask(0),
		srcByte(0),
		srcOffset(0),
		dstBufLen(0),
		dstOffset(0),
		tmpArray2(0),
		decodeStack(0)
	{
	}
	virtual ~Resident_Evil_PAK(void)
	{
	}

	/*
		Decompress PAK from unsigned char vector
	*/
	bool Decompress(std::vector<std::uint8_t> PAK, std::vector<std::uint8_t>& Output);


	/*
		Decompress PAK from file
	*/
	bool Decompress(std::filesystem::path Input);

};