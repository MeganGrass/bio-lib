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
*		SLD Decompression: https://github.com/pmandin/reevengi-tools
*
*/


#include <bio2_nov96.h>


/*
	Print Command Line Help
*/
void Resident_Evil_2_Nov96::PrintHelp(void)
{
	std::cout << "Resident Evil 2 Nov '96: Help" << std::endl << std::endl;
	std::cout << "\tBSS <file> - Extract contents of BSS file container" << std::endl;
	std::cout << "\tITP <file> - Extract contents of ITP file container" << std::endl;
	std::cout << "\tPIX <file> - Convert headerless TIM file to bitmap" << std::endl;
	std::cout << "\tSLD <file> - Decompress SLD file" << std::endl << std::endl;
	std::cout << "BS decode: https://github.com/XProger/OpenResident" << std::endl;
	std::cout << "SLD decompression: https://github.com/pmandin/reevengi-tools" << std::endl;
}


/*
	Command Line Interface
*/
void Resident_Evil_2_Nov96::Commandline(StrVec Args)
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
					std::cout << "Resident Evil 2 Nov '96: Extracting contents of " << Input.filename() << std::endl;
					if (ExtractBSS(Input))
					{
						std::cout << "Resident Evil 2 Nov '96: BSS extraction successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 2 Nov '96: BSS error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 2 Nov '96: BSS error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2 Nov '96: BSS error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "ITP")
		{
			if (i + 1 < Args.size())
			{
				std::filesystem::path Input = Args[i + 1];
				if (FS.Exists(Input))
				{
					std::cout << "Resident Evil 2 Nov '96: Extracting contents of " << Input.filename() << std::endl;
					if (ExtractITP(Input))
					{
						std::cout << "Resident Evil 2 Nov '96: ITP extraction successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 2 Nov '96: ITP error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 2 Nov '96: ITP error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2 Nov '96: ITP error, not enough arguments" << std::endl << std::endl;
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
					std::cout << "Resident Evil 2 Nov '96: Extracting contents of " << Input.filename() << std::endl;
					if (ExtractPIX(Input))
					{
						std::cout << "Resident Evil 2 Nov '96: PIX conversion successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 2 Nov '96: PIX error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 2 Nov '96: PIX error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2 Nov '96: PIX error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "SLD")
		{
			if (i + 1 < Args.size())
			{
				std::filesystem::path Input = Args[i + 1];
				if (FS.Exists(Input))
				{
					std::filesystem::path Output = Input;
					Output.replace_extension(".tim");
					std::cout << "Resident Evil 2 Nov '96: SLD decompressing " << Input.filename() << " to " << Output.filename() << std::endl;
					if (DecompressSLD(Input))
					{
						std::cout << "Resident Evil 2 Nov '96: SLD decompression successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 2 Nov '96: SLD error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 2 Nov '96: SLD error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2 Nov '96: SLD error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

	}

}


/*
	Extract contents of ITP file container
*/
bool Resident_Evil_2_Nov96::ExtractITP(std::filesystem::path Input)
{
	StdFile m_Input{ Input, FileAccessMode::Read_Ex, true, false };

	if (!m_Input)
	{
		Str->Message("ITP Extraction: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::uintmax_t FileSize = m_Input.Size();

	std::filesystem::path Dir = m_Input.GetDirectory();

	m_Input.CreateDirectory(Dir / Input.stem());

	std::uintmax_t Offset = 0;

	std::uintmax_t Counter = 0;

	std::vector<std::uint8_t> Palette(sizeof(bio2_nov96_st_00_clut00));

	std::memcpy(Palette.data(), bio2_nov96_st_00_clut00, sizeof(bio2_nov96_st_00_clut00));

	while (Offset < FileSize)
	{
		Sony_PlayStation_Texture Texture;

		Texture.STP4Bpp() = false;

		Texture.Open(m_Input, Offset);

		std::uintmax_t TextureSize = Texture.Size();

		if (!TextureSize) { break; }

		std::filesystem::path Filename = Str->FormatCStyle("%s/%s/%s_%02d_Large.tim", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str(), Counter);

		Texture.Save(Filename);

		std::unique_ptr<Standard_Image> Image = Texture.GetBitmap();

		Image->SaveAsBitmap(Filename.replace_extension(".bmp"));

		Image->Close();

		Offset += TextureSize;

		std::uint32_t DummyBytes = 0;

		m_Input.Read(Offset + 0x4B0, &DummyBytes, sizeof(std::uint32_t));

		if (DummyBytes)
		{
			Texture.SetWidth(80);

			Texture.SetHeight(30);

			Texture.ImportPixels(m_Input, Offset, 0x960);
		}
		else
		{
			Texture.SetWidth(40);

			Texture.SetHeight(30);

			Texture.ImportPixels(m_Input, Offset, 0x4B0);
		}

		Texture.ImportPalette(Palette);

		Filename = Str->FormatCStyle("%s/%s/%s_%02d_Small.tim", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str(), Counter);

		Texture.Save(Filename);

		Image = Texture.GetBitmap();

		Image->SaveAsBitmap(Filename.replace_extension(".bmp"));

		Image->Close();

		Offset += 0x1000 - (Offset % 0x1000);

		Counter++;
	}

	m_Input.Close();

	return true;
}


/*
	Extract contents of PIX file container
*/
bool Resident_Evil_2_Nov96::ExtractPIX(std::filesystem::path Input)
{
	StdFile m_Input{ Input, FileAccessMode::Read_Ex, true, false };

	if (!m_Input)
	{
		Str->Message("PIX Conversion: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::filesystem::path Dir = m_Input.GetDirectory();

	std::uintmax_t FileSize = m_Input.Size();

	std::uintmax_t Offset = 0;

	std::uintmax_t Counter = 0;

	if ((Input.filename() == "ITEMALL.PIX") || (Input.filename() == "MIXITEM.PIX"))
	{
		m_Input.CreateDirectory(Dir / Input.stem());

		std::vector<std::uint8_t> Palette(sizeof(bio2_nov96_st_00_clut00));

		std::memcpy(Palette.data(), bio2_nov96_st_00_clut00, sizeof(bio2_nov96_st_00_clut00));

		while (Offset < FileSize)
		{
			Sony_PlayStation_Texture Texture;

			Texture.STP4Bpp() = false;

			Texture.Create(8, 40, 30, 1);

			Texture.ImportPalette(Palette);

			Texture.ImportPixels(m_Input, Offset, 0x4B0);

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

	if ((Input.filename() == "MAP01.PIX") ||
		(Input.filename() == "MAP02.PIX") ||
		(Input.filename() == "MAP03.PIX") ||
		(Input.filename() == "MAP04.PIX") ||
		(Input.filename() == "MAP05.PIX") ||
		(Input.filename() == "MAP06.PIX") ||
		(Input.filename() == "MAP07.PIX") ||
		(Input.filename() == "MAP08.PIX") ||
		(Input.filename() == "MAP09.PIX") ||
		(Input.filename() == "MAP0A.PIX") ||
		(Input.filename() == "MAP0B.PIX") ||
		(Input.filename() == "MAP0C.PIX") ||
		(Input.filename() == "MAP0D.PIX"))
	{
		m_Input.CreateDirectory(Dir / Input.stem());

		Sony_PlayStation_Texture Texture;

		Texture.STP4Bpp() = false;

		Texture.Create(4, 256, 256, 3);

		std::uintmax_t ClutPtr = 0;

		std::vector<std::uint8_t> Palette(sizeof(std::uint16_t) * 16);
		std::memcpy(Palette.data(), &bio2_nov96_tex_clut[0x540], (sizeof(std::uint16_t) * 16));
		Texture.ImportPalette(Palette, 0);
		std::memcpy(Palette.data(), &bio2_nov96_tex_clut[0x580], (sizeof(std::uint16_t) * 16));
		Texture.ImportPalette(Palette, 1);
		std::memcpy(Palette.data(), &bio2_nov96_tex_clut[0x5C0], (sizeof(std::uint16_t) * 16));
		Texture.ImportPalette(Palette, 2);

		Texture.ImportPixels(m_Input, Offset, 0x8000);

		std::filesystem::path Filename = Str->FormatCStyle("%s/%s/%s.tim", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str());

		Texture.Save(Filename);

		std::unique_ptr<Standard_Image> Image = Texture.GetBitmap();

		for (uint32_t iClut = 0; iClut < Texture.GetClutSize(); iClut++)
		{
			Texture.UpdateBitmapPalette(Image, iClut);

			Filename = Str->FormatCStyle("%s/%s/%s_%02d.tim", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str(), iClut);

			Image->SaveAsBitmap(Filename.replace_extension(".bmp"));
		}

		Image->Close();

		m_Input.Close();

		return true;
	}

	if (FileSize == 0x25800)
	{
		Sony_PlayStation_Texture Texture;

		Texture.Create(16, 320, 240, 0);

		Texture.ImportPixels(m_Input, 0, 0x25800);

		Texture.Save(Input.replace_extension(".tim"));

		std::unique_ptr<Standard_Image> Image = Texture.GetBitmap();

		Image->SaveAsBitmap(Input.replace_extension(".bmp"));

		Image->Close();
	}

	m_Input.Close();

	return true;
}