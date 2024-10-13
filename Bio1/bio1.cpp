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
*		BS Decode: https://github.com/XProger/OpenResident
*
*		PAK Decompression: https://github.com/pmandin/reevengi-tools
*
*/


#include <bio1.h>


/*
	Print Command Line Help
*/
void Resident_Evil::PrintHelp(void)
{
	std::cout << "Resident Evil: Help" << std::endl << std::endl;
	std::cout << "\t<op>\t\t\tasm (assemble) or dis (disassemble)" << std::endl << std::endl;
	std::cout << "\t<game>\t\t\t0 = Aug/Oct 95, 1 = Final" << std::endl << std::endl;
	std::cout << "\tBSS <file> - Extract contents of BSS file container" << std::endl;
	std::cout << "\tEMD <game> <op> <file> - Extract contents of EMD file container" << std::endl;
	std::cout << "\tPAK <file> - Decompress PAK file" << std::endl << std::endl;
	std::cout << "\tPIX <file> - Convert headerless TIM file to bitmap" << std::endl;
	std::cout << "BS decode: https://github.com/XProger/OpenResident" << std::endl;
	std::cout << "PAK decompression: https://github.com/pmandin/reevengi-tools" << std::endl;
}


/*
	Command Line Interface
*/
void Resident_Evil::Commandline(StrVec Args)
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
					std::cout << "Resident Evil: Extracting contents of " << Input.filename() << std::endl;
					if (ExtractBSS(Input))
					{
						std::cout << "Resident Evil: BSS extraction successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil: BSS error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil: BSS error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil: BSS error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "AUG95")
		{
			Game = Resident_Evil_Video_Game::Resident_Evil_Aug_4_1995;
		}

		if (Args[i] == "OCT95")
		{
			Game = Resident_Evil_Video_Game::Resident_Evil_Oct_4_1995;
		}

		if (Args[i] == "EMD")
		{
			if (i + 2 < Args.size())
			{
				String Op = Args[i + 1];
				std::filesystem::path Input = Args[i + 2];

				if (FS.Exists(Input))
				{
					if (Str.ToUpper(Op) == "ASM")
					{
						std::cout << "Resident Evil: Assembling " << Input.filename() << std::endl;
						if (AssembleEMD(Input))
						{
							std::cout << "Resident Evil: EMD assembly successful" << std::endl;
						}
						else
						{
							std::cout << "Resident Evil: EMD error" << std::endl;
						}
					}
					if (Str.ToUpper(Op) == "DIS")
					{
						/*std::cout << "Resident Evil: Extracting contents of " << Input.filename() << std::endl;
						if (ExtractEMD(Input, Game))
						{
							std::cout << "Resident Evil: EMD extraction successful" << std::endl;
						}
						else
						{
							std::cout << "Resident Evil: EMD error" << std::endl;
						}*/

						Emd->SetGame(Game);
						ExtractEMD(Input, Game);
						if (Emd->Open(Input))
						{
							std::cout << "Resident Evil: Successfully opened" << std::endl;
							if (Emd->Save(Input.replace_extension("test.emd")))
							{
								std::cout << "Resident Evil: Successfully saved" << std::endl;
							}
						}
					}
				}
				else
				{
					std::cout << "Resident Evil: EMD error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil: EMD error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "PAK")
		{
			if (i + 1 < Args.size())
			{
				std::filesystem::path Input = Args[i + 1];
				std::filesystem::path Output = Input;
				Output.replace_extension(".tim");
				if (FS.Exists(Input))
				{
					std::cout << "Resident Evil: PAK decompressing " << Input.filename() << " to " << Output.filename() << std::endl;
					if (DecompressPAK(Input))
					{
						std::cout << "Resident Evil: PAK decompression successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil: PAK error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil: PAK error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil: PAK error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "PIX")
		{
			if (i + 1 < Args.size())
			{
				std::filesystem::path Input = Args[i + 1];
				if (FS.Exists(Input))
				{
					std::cout << "Resident Evil: Extracting contents of " << Input.filename() << std::endl;
					if (ExtractPIX(Input))
					{
						std::cout << "Resident Evil: PIX conversion successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil: PIX error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil: PIX error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil: PIX error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

	}

}


/*
	Extract contents of BSS file container
*/
bool Resident_Evil::ExtractBSS(std::filesystem::path Input)
{
	StdFile m_Input { Input, FileAccessMode::Read_Ex, true, false };

	if (!m_Input)
	{
		Str->Message("BSS Extraction: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	if (m_Input.Size() % 32768 != 0)
	{
		Str->Message("BSS Extraction: Error, file size is not a multiple of 32768");
		return false;
	}

	size_t nBs = m_Input.Size() / 32768;

	if (!nBs)
	{
		Str->Message("BSS Extraction: Error, no bitstreams to extract");
		return false;
	}

	std::filesystem::path Dir = m_Input.GetDirectory() / m_Input.GetFileName().stem();

	m_Input.CreateDirectory(Dir);

	GetStageRoom(m_Input.GetFileName().stem().string());

	uintmax_t pBs = 0;

	uintmax_t pIndex = 0;

	uint32_t Flag = 0;

	size_t Bs_size = 0;

	struct BitStream_Header
	{
		std::uint16_t Length;
		std::uint16_t ID;
		std::uint16_t Q_Scale;
		std::uint16_t Version;
	};

	BitStream_Header Header{};

	for (size_t i = 0; i < nBs; i++, pBs += 32768)
	{
		uintmax_t ZeroBytes = 0;

		pIndex = m_Input.Find(&ZeroBytes, sizeof(uintmax_t), pBs);

		if (pIndex % 4 != 0) { pIndex += 4 - (pIndex % 4); }

		Bs_size = pIndex - pBs;

		std::cout << "Bs_size " << std::hex << Bs_size << std::dec << std::endl;

		std::vector<std::uint8_t> BS(Bs_size);

		m_Input.Read(pBs, BS.data(), Bs_size);

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
	}

	m_Input.Close();

	return true;
}


/*
	Extract contents of PIX file container
*/
bool Resident_Evil::ExtractPIX(std::filesystem::path Input)
{
	StdFile m_Input { Input, FileAccessMode::Read_Ex, true, false };

	if (!m_Input)
	{
		Str->Message("PIX Conversion: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::filesystem::path Dir = m_Input.GetDirectory();

	std::uintmax_t FileSize = m_Input.Size();

	std::uintmax_t Offset = 0;

	std::uintmax_t Counter = 0;

	if ((Input.filename() == "FONT01.PIX") || (Input.filename() == "FONT02.PIX"))
	{
		std::vector<std::uint8_t> Normal(sizeof(bio1_dc_font_normal));
		std::memcpy(Normal.data(), bio1_dc_font_normal, sizeof(bio1_dc_font_normal));

		std::vector<std::uint8_t> Green(sizeof(bio1_dc_font_green));
		std::memcpy(Green.data(), bio1_dc_font_green, sizeof(bio1_dc_font_green));

		std::vector<std::uint8_t> Red(sizeof(bio1_dc_font_red));
		std::memcpy(Red.data(), bio1_dc_font_red, sizeof(bio1_dc_font_red));

		std::vector<std::uint8_t> Grey(sizeof(bio1_dc_font_grey));
		std::memcpy(Grey.data(), bio1_dc_font_grey, sizeof(bio1_dc_font_grey));

		Sony_PlayStation_Texture Texture { 4, 256, 56, 4 };

		Texture.CopyPalette(Normal, 0);
		Texture.CopyPalette(Green, 1);
		Texture.CopyPalette(Red, 2);
		Texture.CopyPalette(Grey, 3);

		Texture.ReadPixels(m_Input, Offset, 0x1C00);

		Texture.Save(Input.replace_extension(".tim"));

		Texture.SetSTP4Bpp(false);

		std::unique_ptr<Standard_Image> Image = Texture.GetBitmap();

		Image->SaveAsBitmap(Input.replace_extension(".bmp"));

		Image->Close();

		m_Input.Close();

		return true;
	}

	if (Input.filename() == "FILEM.PIX")
	{
		m_Input.CreateDirectory(Dir / Input.stem());

		std::uintmax_t pPalette = 0;

		while (Offset < FileSize)
		{
			Sony_PlayStation_Texture Texture{ 4, 512, 192, 2 };

			Texture.SetSTP4Bpp(false);

			std::vector<std::uint8_t> Palette(0x20);
			m_Input.Read(Offset, &Palette.data()[0], 0x20);
			Texture.CopyPalette(Palette, 0);
			m_Input.Read(Offset + 0x20, &Palette.data()[0], 0x20);
			Texture.CopyPalette(Palette, 1);

			Offset += 0x40;
			Texture.ReadPixels(m_Input, Offset, 0xC000);
			Offset += 0xC800 - (Offset % 0xC800);

			std::filesystem::path Filename = Str->FormatCStyle("%s/%s/%s_%02d.tim", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str(), Counter);
			Texture.Save(Filename);

			std::unique_ptr<Standard_Image> Image = Texture.GetBitmap();
			for (uint32_t iClut = 0; iClut < Texture.GetClutSize(); iClut++)
			{
				Texture.UpdateBitmapPalette(Image, iClut);
				Filename = Str->FormatCStyle("%s/%s/%s_%02d_%02d.tim", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str(), Counter, iClut);
				Image->SaveAsBitmap(Filename.replace_extension(".bmp"));
			}
			Image->Close();

			Counter++;
		}

		m_Input.Close();
		return true;
	}

	if ((Input.filename() == "ITEM_ALL.PIX") || (Input.filename() == "ITEM_MIX.PIX") || (Input.filename() == "MEDAL.PIX"))
	{
		m_Input.CreateDirectory(Dir / Input.stem());

		std::vector<std::uint8_t> Palette(sizeof(bio1_status_clut02));

		std::memcpy(Palette.data(), bio1_status_clut02, sizeof(bio1_status_clut02));

		while (Offset < FileSize)
		{
			Sony_PlayStation_Texture Texture;

			Texture.SetSTP4Bpp(false);

			Texture.Create(8, 40, 30, 1);

			Texture.CopyPalette(Palette);

			Texture.ReadPixels(m_Input, Offset, 0x4B0);

			std::filesystem::path Filename = Str->FormatCStyle("%s/%s/%s_%02d.tim", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str(), Counter);

			Texture.Save(Filename);

			std::unique_ptr<Standard_Image> Image = Texture.GetBitmap();

			Image->SaveAsBitmap(Filename.replace_extension(".bmp"));

			Image->Close();

			Offset += 0x4B0;

			Counter++;
		}

		m_Input.Close();

		return true;
	}

	if (FileSize == 0x8820)
	{
		std::vector<std::uint8_t> Palette(0x20);
		m_Input.Read(Offset, Palette.data(), 0x20);

		Offset += 0x20;

		Sony_PlayStation_Texture Texture { 4, 512, 136, 1 };

		Texture.SetSTP4Bpp(false);

		Texture.CopyPalette(Palette);

		Texture.ReadPixels(m_Input, Offset, 0x8800);

		Texture.Save(Input.replace_extension(".tim"));

		std::unique_ptr<Standard_Image> Image = Texture.GetBitmap();

		Image->SaveAsBitmap(Input.replace_extension(".bmp"));

		Image->Close();

		m_Input.Close();

		return true;
	}

	if ((FileSize == 0x25800) || (Input.filename() == "RC1121.PIX"))
	{
		Sony_PlayStation_Texture Texture;

		Texture.Create(16, 320, 240, 0);

		Texture.ReadPixels(m_Input, 0, 0x25800);

		Texture.Save(Input.replace_extension(".tim"));

		std::unique_ptr<Standard_Image> Image = Texture.GetBitmap();

		Image->SaveAsBitmap(Input.replace_extension(".bmp"));

		Image->Close();
	}

	m_Input.Close();

	return true;
}