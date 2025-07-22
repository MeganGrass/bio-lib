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
*		SLD Decompression: https://github.com/pmandin/reevengi-tools
*
*/


#include "bio3_sld.h"


/*
	Decompress SLD
*/
bool Resident_Evil_3_SLD::Decompress(std::vector<std::uint8_t> SLD, std::vector<std::uint8_t>& Output)
{
	if (SLD.empty())
	{
		return false;
	}

	std::uint8_t* dstPtr;

	std::int32_t srcPos = 0x04;

	std::size_t dstPos = 0x00;

	std::uint32_t numBlocks = 0;

	std::int32_t count = 0;

	std::int32_t offset = 0;

	std::memcpy(&numBlocks, &SLD[0], 4);

	std::size_t OutputSize = 0x10220;

	Output.resize(OutputSize);

	dstPtr = Output.data();

	for (std::uint32_t i = 0; (i < numBlocks) && (srcPos < SLD.size()); i++)
	{
		if ((SLD[srcPos] & 0x80) != 0)
		{
			count = SLD[srcPos++] & 0x7F;

			if ((dstPos + 1) + count >= Output.size())
			{
				OutputSize += 0x10220;

				Output.resize(OutputSize);

				dstPtr = &Output.data()[OutputSize];
			}

			std::memcpy(&dstPtr[dstPos], &SLD[srcPos], count);

			srcPos += count;

			dstPos += count;
		}
		else
		{
			offset = SLD[srcPos++] << 8;

			offset |= SLD[srcPos++];

			count = (offset >> 11) + 2;

			offset &= 0x7FF;

			if ((dstPos + 1) + count >= Output.size())
			{
				OutputSize += 0x10220;

				Output.resize(OutputSize);

				dstPtr = &Output.data()[OutputSize];
			}

			for (std::int32_t x = 0; x < count; x++)
			{
				Output.data()[dstPos + x] = Output.data()[(dstPos - (offset + 4)) + x];
			}

			dstPos += count;
		}
	}

	Output.resize(dstPos);

	return true;
}


/*
	Decompress SLD from file
*/
bool Resident_Evil_3_SLD::Decompress(std::filesystem::path Input)
{
	Standard_String Str;

	StdFile m_Input { Input, FileAccessMode::Read_Ex, true, false };
	if (!m_Input)
	{
		Str.Message("SLD Compression: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::vector<std::uint8_t> SLD(m_Input.Size());
	m_Input.Read(0, SLD.data(), m_Input.Size());
	m_Input.Close();

	std::vector<std::uint8_t> TIM(0x10220);

	if (!Decompress(SLD, TIM))
	{
		Str.Message("SLD Compression: Error, could not decompress %s", Input.filename().string().c_str());
		return false;
	}

	StdFile m_Output { Input.replace_extension(".tim"), FileAccessMode::Write_Ex, true, true};
	if (!m_Output)
	{
		Str.Message("SLD Compression: Error, could not create %s", Input.filename().string().c_str());
		return false;
	}
	m_Output.Write(0, TIM.data(), TIM.size());
	m_Output.Close();

	Sony_PlayStation_Texture Texture { Input };
	if (!Texture.IsOpen())
	{
		Str.Message("SLD Compression: Error, decompressed data (%llx bytes) does not contain TIM header", TIM.size(), Input.filename().string().c_str());
		return false;
	}

	std::unique_ptr<Standard_Image> Image = Texture.ExportImage();
	Image->SaveBMP(Input.replace_extension(".bmp"));
	Image->Close();

	return true;
}