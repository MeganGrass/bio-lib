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


#include "bio2_sld.h"


/*
	Decompress SLD
*/
bool Resident_Evil_2_SLD::Decompress(std::vector<std::uint8_t> SLD, std::vector<std::uint8_t>& Output)
{
	if (SLD.empty())
	{
		return false;
	}

	std::uint32_t OutputSize = 0;

	std::memcpy(&OutputSize, &SLD[0], 4);

	if (OutputSize > 0x10220)
	{
		std::cout << "SLD Decompression: Warning, output size (0x" << std::hex << OutputSize << ") typically isn't larger than 0x10220 bytes" << std::dec << std::endl;
	}

	Output.resize(OutputSize);

	std::uint32_t pSLD = 0x06;

	std::uint32_t pOutput = 0x00;

	std::int32_t Count = 0;

	while ((pSLD < SLD.size()) && (pOutput < OutputSize))
	{
		while ((SLD[pSLD] & 0x10) == 0)
		{
			Count = SLD[pSLD] & 0x0F;

			std::int32_t srcOffset = (-256 | (SLD[pSLD] & 0xE0)) << 3;

			srcOffset |= SLD.data()[pSLD + 1];

			if (Count == 0x0F)
			{
				Count += SLD.data()[pSLD + 2];

				pSLD += 3;
			}
			else
			{
				pSLD += 2;
			}

			Count += 3;

			for (std::int32_t i = 0; i < Count; i++)
			{
				Output.data()[pOutput + i] = Output.data()[(pOutput + srcOffset) + i];
			}

			pOutput += Count;
		}

		if (SLD[pSLD] == 0xFF) { break; }

		Count = ((SLD[pSLD++] | 0xFFE0) ^ 0xFFFF) + 1;

		if (Count == 0x10)
		{
			Count += SLD[pSLD++];
		}

		std::memcpy(&Output[pOutput], &SLD[pSLD], Count);

		pOutput += Count;

		pSLD += Count;
	}

	return true;
}


/*
	Decompress SLD from file
*/
bool Resident_Evil_2_SLD::Decompress(std::filesystem::path Input)
{
	Standard_String Str;

	StdFile m_Input { Input, FileAccessMode::Read_Ex, true, false };
	if (!m_Input)
	{
		Str.Message("SLD Decompression: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::vector<std::uint8_t> SLD(m_Input.Size());
	m_Input.Read(0, SLD.data(), m_Input.Size());
	m_Input.Close();

	std::vector<std::uint8_t> TIM(0x10220);

	if (!Decompress(SLD, TIM))
	{
		Str.Message("SLD Decompression: Error, could not decompress %s", Input.filename().string().c_str());
		return false;
	}

	StdFile m_Output { Input.replace_extension(".tim"), FileAccessMode::Write_Ex, true, true};
	if (!m_Output)
	{
		Str.Message("SLD Decompression: Error, could not create %s", Input.filename().string().c_str());
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