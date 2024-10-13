/*
*
*	Haruhiko Okumura
*
*
*	TODO:
*
*/


#pragma once

#include <std_basic_fstream.h>

#include <std_string.h>


class LZO_Compression {
private:

	std::int32_t bit_buffer{};

	std::int32_t bit_mask{};

	unsigned long codecount{};

	unsigned long textcount{};

	bool b_Fail { false };

	void error(void)
	{
		b_Fail = true;
	}

	void putbit1(void);

	void putbit0(void);

	void flush_bit_buffer(void);

	void output1(std::int32_t c);

	void output2(std::int32_t x, std::int32_t y);

	std::unique_ptr<StdFile> m_Input;

	std::unique_ptr<StdFile> m_Output;

public:

	/*
		Construction
	*/
	explicit LZO_Compression(void) : bit_buffer(0), bit_mask(0), codecount(0), textcount(0), b_Fail(false)
	{
		m_Input = std::make_unique<StdFile>();
		m_Output = std::make_unique<StdFile>();
	}
	virtual ~LZO_Compression(void)
	{
		m_Input.reset();
		m_Output.reset();
	}

	/*
		Compress
	*/
	bool Compress(std::filesystem::path Input, std::filesystem::path Output);

	/*
		Decompress
	*/
	void Decompress(std::uint8_t* in, std::uint8_t* out);

};