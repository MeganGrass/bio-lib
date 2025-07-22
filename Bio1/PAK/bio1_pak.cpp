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


#include "bio1_pak.h"


std::int32_t Resident_Evil_PAK::read_bits(std::vector<std::uint8_t> src, std::int32_t num_bits)
{
	unsigned long value = 0, mask;

	mask = 1 << (--num_bits);

	while (mask > 0)
	{
		if (tmpMask == 0x80)
		{
			if ((srcOffset + 1) >= src.size())
			{
				srcByte = 0;
			}
			else
			{
				srcByte = src.data()[srcOffset++];
			}
		}

		if ((tmpMask & srcByte) != 0)
		{
			value |= mask;
		}

		tmpMask >>= 1;

		mask >>= 1;

		if (tmpMask == 0)
		{
			tmpMask = 0x80;
		}
	}

	return value;
}


std::int32_t Resident_Evil_PAK::decodeString(std::int32_t decodeStackOffset, unsigned long code)
{
	while (code > 255)
	{
		decodeStack.data()[decodeStackOffset++] = static_cast<std::uint8_t>(tmpArray2.data()[code].value);

		code = tmpArray2.data()[code].index;
	}

	decodeStack.data()[decodeStackOffset] = static_cast<std::uint8_t>(code);

	return decodeStackOffset;
}


bool Resident_Evil_PAK::Decompress(std::vector<std::uint8_t> PAK, std::vector<std::uint8_t>& Output)
{
	std::int32_t num_bits_to_read, i;

	std::int32_t lzwnew, c, lzwold, lzwnext;

	bool stop = false;

	tmpMask = 0x80;

	srcByte = 0;

	srcOffset = 0;

	dstOffset = 0;

	decodeStack.resize(RE1_PAK_DECODE_SIZE);

	tmpArray2.resize(RE1_PAK_DECODE_SIZE);

	dstBufLen = 0x8000;

	if (Output.size() < dstBufLen) { Output.resize(dstBufLen); }

	while ((!stop) && (srcOffset < PAK.size()))
	{
		for (i = 0; i < RE1_PAK_DECODE_SIZE; i++)
		{
			tmpArray2.data()[i].flag = 0xFFFFFFFF;
		}

		lzwnext = 0x103;

		num_bits_to_read = 9;

		c = lzwold = read_bits(PAK, num_bits_to_read);

		if (lzwold == 0x100)
		{
			break;
		}

		if ((dstOffset + 1) >= Output.size())
		{
			dstBufLen += 0x8000;

			Output.resize(dstBufLen);
		}

		Output.data()[dstOffset++] = c;

		for (;;)
		{
			lzwnew = read_bits(PAK, num_bits_to_read);

			if (lzwnew == 0x100)
			{
				stop = true;
				break;
			}

			if (lzwnew == 0x102)
			{
				break;
			}

			if (lzwnew == 0x101)
			{
				num_bits_to_read++;
				continue;
			}

			if (lzwnew >= lzwnext)
			{
				decodeStack.data()[0] = c;
				i = decodeString(1, lzwold);
			}
			else
			{
				i = decodeString(0, lzwnew);
			}

			c = decodeStack.data()[i];

			while (i >= 0)
			{
				if ((dstOffset + 1) >= Output.size())
				{
					dstBufLen += 0x8000;

					Output.resize(dstBufLen);
				}

				Output.data()[dstOffset++] = decodeStack.data()[i--];
			}

			tmpArray2.data()[lzwnext].index = lzwold;

			tmpArray2.data()[lzwnext].value = c;

			lzwnext++;

			lzwold = lzwnew;
		}
	}

	Output.resize(dstOffset);

	return true;
}


/*
	Decompress PAK from file
*/
bool Resident_Evil_PAK::Decompress(std::filesystem::path Input)
{
	Standard_String Str;

	StdFile m_Input{ Input, FileAccessMode::Read_Ex, true, false };
	if (!m_Input)
	{
		Str.Message("PAK Compression: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::vector<std::uint8_t> PAK(m_Input.Size());
	m_Input.Read(0, PAK.data(), m_Input.Size());
	m_Input.Close();

	std::vector<std::uint8_t> TIM;

	if (!Decompress(PAK, TIM))
	{
		Str.Message("PAK Compression: Error, could not decompress %s", Input.filename().string().c_str());
		return false;
	}

	StdFile m_Output{ Input.replace_extension(".tim"), FileAccessMode::Write_Ex, true, true };
	if (!m_Output)
	{
		Str.Message("PAK Compression: Error, could not create %s", Input.filename().string().c_str());
		return false;
	}
	m_Output.Write(0, TIM.data(), TIM.size());
	m_Output.Close();

	Sony_PlayStation_Texture Texture { Input };
	if (!Texture.IsOpen())
	{
		Str.Message("PAK Compression: Error, decompressed data (%llx bytes) does not contain TIM header", TIM.size(), Input.filename().string().c_str());
		return false;
	}

	std::unique_ptr<Standard_Image> Image = Texture.ExportImage();
	Image->SaveBMP(Input.replace_extension(".bmp"));
	Image->Close();

	return true;
}