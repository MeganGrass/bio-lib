/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO: 
* 
*		Complete SLD extraction
*
*
*	CREDIT:
*
*		BS Decode: https://github.com/XProger/OpenResident
*
*		SLD Decompression: https://github.com/pmandin/reevengi-tools
*
*/


#include <bio3.h>


/*
	Print Command Line Help
*/
void Resident_Evil_3::PrintHelp(void)
{
	std::cout << "Resident Evil 3: Help" << std::endl << std::endl;
	std::cout << "\tBSS <file> - Extract contents of BSS file container" << std::endl;
	std::cout << "\tSLD <file> - Decompress SLD file" << std::endl << std::endl;
	std::cout << "BS decode: https://github.com/XProger/OpenResident" << std::endl;
	std::cout << "SLD decompression: https://github.com/pmandin/reevengi-tools" << std::endl;
}


/*
	Command Line Interface
*/
void Resident_Evil_3::Commandline(StrVec Args)
{
	Standard_String Str;

	Standard_FileSystem FS;

	for (std::size_t i = 0; i < Args.size(); i++)
	{
		Str.ToUpper(Args[i]);

		if (Args[i] == "HELP")
		{
			PrintHelp();
		}

		if (Args[i] == "BSS")
		{
			if (i + 1 < Args.size())
			{
				std::filesystem::path Input = Args[i + 1];
				if (FS.Exists(Input))
				{
					std::cout << "Resident Evil 3: Extracting contents of " << Input.filename() << std::endl;
					if (ExtractBSS(Input))
					{
						std::cout << "Resident Evil 3: BSS extraction successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 3: BSS error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 3: BSS error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 3: BSS error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "EMD")
		{
			if (i + 1 < Args.size())
			{
				std::filesystem::path Input = Args[i + 1];
				if (FS.Exists(Input))
				{
					std::cout << "Resident Evil 3: Extracting contents of " << Input.filename() << std::endl;
					if (ExtractModel(Input))
					{
						std::cout << "Resident Evil 3: Model extraction successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 3: Model error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 3: Model error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 3: Model error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "SLD")
		{
			if (i + 1 < Args.size())
			{
				std::filesystem::path Input = Args[i + 1];
				std::filesystem::path Output = Input;
				Output.replace_extension(".tim");
				if (FS.Exists(Input))
				{
					std::cout << "Resident Evil 3: SLD decompressing " << Input.filename() << " to " << Output.filename() << std::endl;
					if (DecompressSLD(Input))
					{
						std::cout << "Resident Evil 3: SLD decompression successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 3: SLD error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 3: SLD error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 3: SLD error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

	}

}


/*
	Extract contents of BSS file container
*/
bool Resident_Evil_3::ExtractBSS(std::filesystem::path Input)
{
	StdFile m_Input { Input, FileAccessMode::Read_Ex, true, false };

	if (!m_Input)
	{
		Str->Message("BSS Extraction: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	if (m_Input.Size() % 65536 != 0)
	{
		Str->Message("BSS Extraction: Error, file size is not a multiple of 65536");
		return false;
	}

	size_t nBs = m_Input.Size() / 65536;

	if (!nBs)
	{
		Str->Message("BSS Extraction: Error, no bitstreams to extract");
		return false;
	}

	GetStageRoom(m_Input.GetFileName().stem().string());

	std::filesystem::path Dir = m_Input.GetDirectory() / Str->FormatCStyle("ROOM%X%02X", Stage, Room);

	m_Input.CreateDirectory(Dir);

	uintmax_t pBs = 0;

	uintmax_t pIndex = 0;

	uint32_t Flag = 0;

	struct BitStream_Header
	{
		std::uint16_t Length;
		std::uint16_t ID;
		std::uint16_t Q_Scale;
		std::uint16_t Version;
	};

	BitStream_Header Header{};

	struct BSS_Index
	{
		uint32_t Bs_size;
		uint32_t b_Sld;
	};

	BSS_Index Index{};

	for (size_t i = 0; i < nBs; i++, pBs += 65536)
	{
		uintmax_t ZeroBytes = 0;

		pIndex = m_Input.Find(&ZeroBytes, sizeof(uintmax_t), pBs);

		if (pIndex % 4 != 0) { pIndex += 4 - (pIndex % 4); }

		m_Input.Read(pIndex - 4, &Flag, sizeof(uint32_t));

		if (Flag == 1)
		{
			pIndex -= 8;
			m_Input.Read(pIndex, &Index, sizeof(BSS_Index));
		}
		else
		{
			pIndex -= 4;
			m_Input.Read(pIndex, &Index, sizeof(BSS_Index));
		}

		uintmax_t pSld = pBs + Index.Bs_size;

		uintmax_t Sld_size = pIndex - pSld;

		std::cout << "Bs_size " << std::hex << Index.Bs_size << std::dec << std::endl;
		std::cout << "b_Sld " << std::hex << Index.b_Sld << std::dec << std::endl;
		std::cout << "pSld " << std::hex << pSld << std::dec << std::endl;
		std::cout << "Sld_size " << std::hex << Sld_size << std::dec << std::endl;

		std::vector<std::uint8_t> BS(Index.Bs_size);

		m_Input.Read(pBs, BS.data(), Index.Bs_size);

		std::filesystem::path OutStr = Str->FormatCStyle("%s/ROOM_%X%02X_%02d.bs", Dir.string().c_str(), Stage, Room, i);

		m_Input.Create(OutStr, BS);

		std::cout << "BSS Extraction: Decoding bitstream" << std::endl;

		std::vector<std::uint8_t> BMP(320 * 240 * 4);

		std::memcpy(&Header, &BS.data()[0], sizeof(BitStream_Header));

		Bs->mdec_decode(&BS.data()[8], Header.Version, 320, 240, Header.Q_Scale, &BMP.data()[0]);

		std::cout << "BSS Extraction: Decoding bitstream complete" << std::endl;

		std::cout << "BSS Extraction: Converting to bitmap" << std::endl;

		Standard_Image Image { ImageFormat::BMP, 320, 240, 32, 0 };

		size_t pImg = 0;

		for (int32_t y = 0; y < 240; y++)
		{
			for (int32_t x = 0; x < 320; x++, pImg += 4)
			{
				uint32_t Color = BMP[pImg + 2] | (BMP[pImg + 1] << 8) | (BMP[pImg + 0] << 16) | (BMP[pImg + 3] << 24);
				Image.SetPixel(x, y, Color);
			}
		}

		Image.SaveAsBitmap(OutStr.replace_extension(".bmp"));

		std::cout << "BSS Extraction: Conversion complete" << std::endl;

		if (Index.b_Sld)
		{
			std::vector<std::uint8_t> SLD(Sld_size);

			m_Input.Read(pSld, SLD.data(), Sld_size);

			OutStr = Str->FormatCStyle("%s/ROOM_%X%02X_%02d_MASK.sld", Dir.string().c_str(), Stage, Room, i);

			m_Input.Create(OutStr, SLD);

			std::cout << "BSS Extraction: Decompressing SLD to TIM" << std::endl;

			std::vector<std::uint8_t> TIM(0x10220);

			if (DecompressSLD(SLD, TIM))
			{
				OutStr.replace_extension(".tim");

				m_Input.Create(OutStr, TIM);

				std::cout << "BSS Extraction: Decompressing SLD complete" << std::endl;

				Sony_PlayStation_Texture Texture { OutStr };

				std::unique_ptr<Standard_Image> Image = Texture.GetBitmap();

				Image->SaveAsBitmap(OutStr.replace_extension(".bmp"));

				Image->Close();
			}
		}
	}

	m_Input.Close();

	return true;
}