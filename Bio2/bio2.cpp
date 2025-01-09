/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO: 
* 
*		Find CLUT for CORE01.TIM
*
*
*	CREDIT:
*
*		ADT Decompression: https://github.com/pmandin/reevengi-tools
*
*		BS Decode: https://github.com/XProger/OpenResident
*
*		SLD Decompression: https://github.com/pmandin/reevengi-tools
*
*/


#include <bio2.h>

#include "ems/cdemd_pos.h"


/*
	Print Command Line Help
*/
void Resident_Evil_2::PrintHelp(void)
{
	std::cout << "Resident Evil 2: Help" << std::endl << std::endl;
	std::cout << "\t<op>\t\t\tASM (assemble) or DIS (disassemble)" << std::endl;
	std::cout << "\t<pl>\t\t\tCDEMD id (0 or 1) for ASM" << std::endl << std::endl;
	std::cout << "\tASMPREF - Prefer assembly text file output (where applicable)\n";
	std::cout << "\tADT <file>\t\tDecompress ADT file" << std::endl;
	std::cout << "\tBGM <op> <file>\t\tExtract or repack contents of BGM file container" << std::endl;
	std::cout << "\tBSS <file>\t\tExtract contents of BSS file container" << std::endl;
	std::cout << "\tEMS <op> <input> <output>\t\tExtract or repack contents of CDEMD0.EMS/CDEMD1.EMS from slus_00748" << std::endl;
	std::cout << "\t\t<output> valid only when <op> is ASM" << std::endl;
	std::cout << "\tCPT <file>\t\tDecompress CPT file" << std::endl;
	std::cout << "\tDIE <op> <file>\t\tExtract or repack contents of DIE file container" << std::endl;
	std::cout << "\tITP <file>\t\tExtract contents of ITP file container" << std::endl;
	std::cout << "\tPDEMO <op> <file>\t\tAssemble or disassemble PDEMO file from/to text" << std::endl;
	std::cout << "\tPIX <file>\t\tConvert headerless TIM file to bitmap" << std::endl;
	std::cout << "\tSAVEBGM <file>\tExtract BGM table from save file" << std::endl;
	std::cout << "\tSLD <file>\t\tDecompress SLD file" << std::endl << std::endl;
	std::cout << "\tCDEMS <ExecutableAddress> <file>\tExtract cdemd tables from slus_00748 executable" << std::endl;
	std::cout << "\tSTAGEXA <OverlayAddress> <offset> <count> <file>\tExtract XA_sector from STAGEx.BIN" << std::endl;
	std::cout << "BS decode: https://github.com/XProger/OpenResident" << std::endl;
	std::cout << "ADT and SLD decompression: https://github.com/pmandin/reevengi-tools" << std::endl;
}


/*
	Command Line Interface
*/
void Resident_Evil_2::Commandline(StrVec Args)
{
	Standard_String Str;

	Standard_FileSystem FS;

	std::uintmax_t Pl = 0;

	for (std::size_t i = 0; i < Args.size(); i++)
	{
		Str.ToUpper(Args[i]);

		if (Args[i] == "ASMPREF")
		{
			b_PreferAsm = true;
		}

		if (Args[i] == "HELP")
		{
			PrintHelp();
		}

		if (Args[i] == "PL")
		{
			if (i + 1 < Args.size())
			{
				Pl = std::strtoul(Args[i + 1].c_str(), nullptr, 10);
			}
			else
			{
				std::cout << "Resident Evil 2: PL error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}
		}

		if (Args[i] == "ADT")
		{
			if (i + 1 < Args.size())
			{
				std::filesystem::path Input = Args[i + 1];
				if (FS.Exists(Input))
				{
					std::filesystem::path Output = Input;
					Output.replace_extension(".out");
					std::cout << "Resident Evil 2: ADT decompressing " << Input.filename() << " to " << Output.filename() << std::endl;
					if (Adt->Decompress(Input, Output))
					{
						std::cout << "Resident Evil 2: ADT decompression successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 2: ADT error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 2: ADT error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2: ADT error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "BGM")
		{
			if (i + 2 < Args.size())
			{
				String Op = Args[i + 1];
				std::filesystem::path Input = Args[i + 2];
				if (FS.Exists(Input))
				{
					if (Str.ToUpper(Op) == "ASM")
					{
						std::cout << "Resident Evil 2: Assembling " << Input.filename() << std::endl;
						if (AssembleBGM(Input))
						{
							std::cout << "Resident Evil 2: BGM assembly successful" << std::endl;
						}
						else
						{
							std::cout << "Resident Evil 2: BGM error" << std::endl;
						}
					}
					if (Str.ToUpper(Op) == "DIS")
					{
						std::cout << "Resident Evil 2: Extracting contents of " << Input.filename() << std::endl;
						if (ExtractBGM(Input))
						{
							std::cout << "Resident Evil 2: BGM extraction successful" << std::endl;
						}
						else
						{
							std::cout << "Resident Evil 2: BGM error" << std::endl;
						}
					}
				}
				else
				{
					std::cout << "Resident Evil 2: BGM error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2: BGM error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "BSS")
		{
			if (i + 1 < Args.size())
			{
				std::filesystem::path Input = Args[i + 1];
				if (FS.Exists(Input))
				{
					std::cout << "Resident Evil 2: Extracting contents of " << Input.filename() << std::endl;
					if (ExtractBSS(Input))
					{
						std::cout << "Resident Evil 2: BSS extraction successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 2: BSS error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 2: BSS error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2: BSS error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "EMS")
		{
			String Op;
			std::filesystem::path Input;

			if (i + 2 < Args.size())
			{
				Op = Args[i + 1];
				Input = Args[i + 2];
			}
			else
			{
				std::cout << "Resident Evil 2: EMS error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
				break;
			}

			if (Str.ToUpper(Op) == "DIS")
			{
				ExtractEMS(Input);
			}

			if (Str.ToUpper(Op) == "ASM")
			{
				if (i + 3 < Args.size())
				{
					std::filesystem::path Output = Args[i + 3];
					AssembleEMS(Input, Output);
				}
				else
				{
					std::cout << "Resident Evil 2: EMS error, not enough arguments" << std::endl << std::endl;
					PrintHelp();
				}
			}
		}

		if (Args[i] == "CPT")
		{
			if (i + 1 < Args.size())
			{
				std::filesystem::path Input = Args[i + 1];
				if (FS.Exists(Input))
				{
					std::cout << "Resident Evil 2: Decompressing " << Input.filename() << std::endl;
					if (DecompressCPT(Input))
					{
						std::cout << "Resident Evil 2: CPT Decompression successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 2: CPT error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 2: CPT error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2: CPT error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "DIE")
		{
			if (i + 2 < Args.size())
			{
				String Op = Args[i + 1];
				std::filesystem::path Input = Args[i + 2];
				if (FS.Exists(Input))
				{
					if (Str.ToUpper(Op) == "ASM")
					{
						std::cout << "Resident Evil 2: Assembling " << Input.filename() << std::endl;
						if (AssembleDIE(Input))
						{
							std::cout << "Resident Evil 2: DIE assembly successful" << std::endl;
						}
						else
						{
							std::cout << "Resident Evil 2: DIE error" << std::endl;
						}
					}
					if (Str.ToUpper(Op) == "DIS")
					{
						std::cout << "Resident Evil 2: Extracting contents of " << Input.filename() << std::endl;
						if (ExtractDIE(Input))
						{
							std::cout << "Resident Evil 2: DIE extraction successful" << std::endl;
						}
						else
						{
							std::cout << "Resident Evil 2: DIE error" << std::endl;
						}
					}
				}
				else
				{
					std::cout << "Resident Evil 2: DIE error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2: DIE error, not enough arguments" << std::endl << std::endl;
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
					std::cout << "Resident Evil 2: Extracting contents of " << Input.filename() << std::endl;
					if (ExtractITP(Input))
					{
						std::cout << "Resident Evil 2: ITP extraction successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 2: ITP error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 2: ITP error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2: ITP error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "PDEMO")
		{
			if (i + 2 < Args.size())
			{
				String Op = Args[i + 1];
				std::filesystem::path Input = Args[i + 2];
				if (Str.ToUpper(Op) == "ASM")
				{
					std::cout << "Resident Evil 2: Assembling " << Input.filename() << std::endl;
					if (AssemblePDEMO(Input))
					{
						std::cout << "Resident Evil 2: PDEMO assembly successful" << std::endl;
					}
				}
				if (Str.ToUpper(Op) == "DIS")
				{
					std::cout << "Resident Evil 2: Disassembling contents of " << Input.filename() << std::endl;
					if (DisassemblePDEMO(Input))
					{
						std::cout << "Resident Evil 2: PDEMO disassembly successful" << std::endl;
					}
				}
			}
			else
			{
				std::cout << "Resident Evil 2: PDEMO error, not enough arguments" << std::endl << std::endl;
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
					std::cout << "Resident Evil 2: Extracting contents of " << Input.filename() << std::endl;
					if (ExtractPIX(Input))
					{
						std::cout << "Resident Evil 2: PIX conversion successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 2: PIX error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 2: PIX error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2: PIX error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "SAVEBGM")
		{
			if ((i + 1) < Args.size())
			{
				std::filesystem::path Filename = Args[i + 1];
				std::cout << "Resident Evil 2: Extracting BGM table from " << Filename.filename() << std::endl;
				if (ExtractBgmFromSaveFile(Filename))
				{
					std::cout << "Resident Evil 2: Successfully extracted BGM table" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2: SAVEBGM error, not enough arguments" << std::endl << std::endl;
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
					std::cout << "Resident Evil 2: SLD decompressing " << Input.filename() << " to " << Output.filename() << std::endl;
					if (DecompressSLD(Input))
					{
						std::cout << "Resident Evil 2: SLD decompression successful" << std::endl;
					}
					else
					{
						std::cout << "Resident Evil 2: SLD error" << std::endl;
					}
				}
				else
				{
					std::cout << "Resident Evil 2: SLD error, input file not found" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2: SLD error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}

		}

		if (Args[i] == "CDEMS")
		{
			if ((i + 2) < Args.size())
			{
				std::uintmax_t ExecutableAddress = std::strtoull(Args[i + 1].c_str(), nullptr, 16);
				std::filesystem::path Filename = Args[i + 2];
				std::cout << "Resident Evil 2: Extracting cdemd tables from " << Filename.filename() << " at address " << std::hex << ExecutableAddress << std::dec << std::endl;
				if (ExtractCdEmsTablesFromExe(ExecutableAddress, Filename))
				{
					std::cout << "Resident Evil 2: Successfully extracted cdemd tables" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2: CDEMS error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}
		}

		if (Args[i] == "STAGEXA")
		{
			if ((i + 4) < Args.size())
			{
				std::uintmax_t StartAddress = std::strtoull(Args[i + 1].c_str(), nullptr, 16);
				std::uintmax_t Offset = std::strtoull(Args[i + 2].c_str(), nullptr, 16);
				std::size_t Count = std::strtoull(Args[i + 3].c_str(), nullptr, 10);
				std::filesystem::path Filename = Args[i + 4];
				std::cout << "Resident Evil 2: Extracting Xa_sector[" << std::dec << Count << "][3] from " << Filename.filename() << " at offset " << std::hex << Offset << std::endl;
				if (ExtractXaSectorFromStageBin(StartAddress, Offset, Count, Filename))
				{
					std::cout << "Resident Evil 2: Successfully extracted Xa_sector table" << std::endl;
				}
			}
			else
			{
				std::cout << "Resident Evil 2: STAGEXA error, not enough arguments" << std::endl << std::endl;
				PrintHelp();
			}
		}

	}

}


/*
	Extract BGM table from save file
*/
bool Resident_Evil_2::ExtractBgmFromSaveFile(std::filesystem::path Input)
{
	std::unique_ptr<StdFile> File = std::make_unique<StdFile>();
	if (!File->Open(Input, FileAccessMode::Read, true, false))
	{
		Str->Message("SAVEBGM: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::unique_ptr<StdText> Text = std::make_unique<StdText>();
	Text->SetBOM(TextFileBOM::UTF8);

	std::uintmax_t nroom[7] = { 0x1F, 0x1E, 0x10, 0x13, 0x0C, 0x1A, 0x07 };

	struct BGMCTR
	{
		std::uint8_t Main;
		std::uint8_t Sub;
	};

	std::vector<BGMCTR> BGM(0x30);

	std::uintmax_t pBGMCTR = 0x22C;

	for (std::uintmax_t i = 0; i < 7; i++)
	{
		for (auto& b : BGM)
		{
			b.Main = 0xFF;
			b.Sub = 0xFF;
		}

		for (std::uintmax_t x = 0; x < nroom[i]; x++, pBGMCTR += sizeof(BGMCTR))
		{
			File->Read(pBGMCTR, &BGM[x], sizeof(BGMCTR));

			std::cout << "SAVEBGM: " << Text->FormatCStyle("STAGE %X ROOM %02X: %02X %02X", i + 1, x, BGM[x].Main, BGM[x].Sub) << std::endl;
		}

		String Output = File->GetDirectory(Input).string() += "/" + Input.stem().string() + "_stage" + std::to_string(i + 1) + "_bgm";

		Output += b_PreferAsm ? ".asm" : ".txt";

		if (Text->Open(Output, FileAccessMode::Write))
		{
			std::size_t r = 0;

			for (auto& b : BGM)
			{
				if (b_PreferAsm)
				{
					Text->AddLine("\t.byte\t0x%02X,\t0x%02X\t;; R%X%02X\n", b.Main, b.Sub, (i + 1), r);
				}
				else
				{
					Text->AddLine("0x%02X 0x%02X\tR%X%02X\n", b.Main, b.Sub, (i + 1), r);
				}

				r++;
			}

			Text->FlushUTF8();
			Text->Close();
		}
		else
		{
			std::cout << "SAVEBGM: Warning, could not create " << Output << std::endl;
		}
	}

	File->Close();

	return true;
}


/*
	Open BSS file container
*/
std::unique_ptr<Standard_Image> Resident_Evil_2::OpenBSS(std::filesystem::path Input, std::int32_t No)
{
	std::unique_ptr<Standard_Image> Image = std::make_unique<Standard_Image>(ImageFormat::BMP, 320, 240, 32, 0);

	StdFile m_Input { Input, FileAccessMode::Read_Ex, true, false };
	if (!m_Input)
	{
		Str->Message("Resident Evil 2: BSS error, could not open %s", Input.filename().string().c_str());
		return Image;
	}

	if (m_Input.Size() % 65536 != 0)
	{
		Str->Message("Resident Evil 2: BSS error, file size is not a multiple of 65536");
		return Image;
	}

	size_t nBs = m_Input.Size() / 65536;

	if (!nBs)
	{
		Str->Message("Resident Evil 2: BSS error, no bitstreams");
		return Image;
	}

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

	uintmax_t pBs = 65536 * No;
	uint32_t Flag = 0;

	uintmax_t ZeroBytes = 0;
	uintmax_t pIndex = m_Input.Find(&ZeroBytes, sizeof(uintmax_t), pBs);
	m_Input.Align(pIndex, 4);

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

	if (!Index.Bs_size)
	{
		Str->Message("Resident Evil 2: BSS error, no bitstream");
		return Image;
	}

	std::vector<std::uint8_t> BS(Index.Bs_size);
	m_Input.Read(pBs, BS.data(), Index.Bs_size);

	std::vector<std::uint8_t> BMP(320 * 240 * 4);
	std::memcpy(&Header, &BS.data()[0], sizeof(BitStream_Header));
	Bs->mdec_decode(&BS.data()[8], Header.Version, 320, 240, Header.Q_Scale, &BMP.data()[0]);

	size_t pImg = 0;
	for (int32_t y = 0; y < 240; y++)
	{
		for (int32_t x = 0; x < 320; x++, pImg += 4)
		{
			uint32_t Color = BMP[pImg + 2] | (BMP[pImg + 1] << 8) | (BMP[pImg + 0] << 16) | (BMP[pImg + 3] << 24);
			Image->SetPixel(x, y, Color);
		}
	}

	return Image;
}


/*
	Extract contents of BSS file container
*/
bool Resident_Evil_2::ExtractBSS(std::filesystem::path Input)
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

	std::filesystem::path Dir = m_Input.GetDirectory() / m_Input.GetFileName().stem();

	m_Input.CreateDirectory(Dir);

	GetStageRoom(m_Input.GetFileName().stem().string());

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


/*
	Extract contents of EMS file container (slus_00748 support only)
*/
bool Resident_Evil_2::ExtractEMS(std::filesystem::path Input)
{
	if (Input.filename().string().compare("CDEMD0.EMS") == -1 && Input.filename().string().compare("CDEMD1.EMS") == -1)
	{
		Str->Message("EMS Extraction: Error, unsupported file %s", Input.filename().string().c_str());
		return false;
	}

	StdFile m_Input { Input, FileAccessMode::Read_Ex, true, false };
	if (!m_Input)
	{
		Str->Message("EMS Extraction: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::filesystem::path Dir = m_Input.GetDirectory() / m_Input.GetFileName().stem();
	m_Input.CreateDirectory(Dir);

	std::vector<std::uint8_t> File = m_Input.buffer();

	m_Input.Close();

	Cyclic_Redundancy_Check Crc32;
	std::uint32_t Crc = Crc32.GetCRC32(File.data(), File.size());
	if (!Input.filename().string().compare("CDEMD0.EMS"))
	{
		if (Crc != 0x8D379101)
		{
			Str->Message("EMS Extraction: Error, invalid CRC32 for %s", Input.filename().string().c_str());
			return false;
		}
	}
	else if (!Input.filename().string().compare("CDEMD1.EMS"))
	{
		if (Crc != 0xD9CEB158)
		{
			Str->Message("EMS Extraction: Error, invalid CRC32 for %s", Input.filename().string().c_str());
			return false;
		}
	}

	std::cout << "EMS Extraction: Extracting contents of " << Input.filename() << std::endl;

	emd_pos* Emd = (emd_pos*)cdemd0_pos;
	if (!Input.filename().string().compare("CDEMD1.EMS"))
	{
		Emd = (emd_pos*)cdemd1_pos;
	}

	std::uintmax_t Offset = 0;

	for (std::size_t i = 0; i < 0x4B; i++)
	{
		m_Input.Create(Dir / Str->FormatCStyle("EMD%02X00.BIN", Emd[i].id), &File.data()[Offset], Emd[i].bin0_size);
		Offset += Emd[i].bin0_size;
		m_Input.PosAlign(Offset, 0x800);

		m_Input.Create(Dir / Str->FormatCStyle("EMD%02X01.BIN", Emd[i].id), &File.data()[Offset], Emd[i].bin1_size);
		Offset += Emd[i].bin1_size;
		m_Input.PosAlign(Offset, 0x800);

		m_Input.Create(Dir / Str->FormatCStyle("EMD%02X.TIM", Emd[i].id), &File.data()[Offset], Emd[i].tim_size);
		Offset += Emd[i].tim_size;
		m_Input.PosAlign(Offset, 0x800);

		m_Input.Create(Dir / Str->FormatCStyle("EMD%02X.EMD", Emd[i].id), &File.data()[Offset], Emd[i].emd_size);
		Offset += Emd[i].emd_size;
		m_Input.PosAlign(Offset, 0x800);
	}

	std::cout << "EMS Extraction: Successfully extracted contents of " << Input.filename() << std::endl;

	return true;
}


/*
	Assemble contents of EMS file container
*/
bool Resident_Evil_2::AssembleEMS(std::filesystem::path Directory, std::filesystem::path OutFilename)
{
	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Directory);
	if (!FS.Exists(Dir) && !FS.IsDirectory(Dir))
	{
		Str->Message("EMS Assemble: Directory not found or isn't a directory: \"%s\"", Dir.string().c_str());
		return false;
	}

	if (FS.GetFileList(Dir).empty())
	{
		Str->Message("EMS Assemble: Create error, directory is empty");
		return false;
	}

	StdFile EMS { OutFilename, FileAccessMode::Write_Ex, true, true};
	if (!EMS.IsOpen())
	{
		Str->Message("EMS Assemble: Error, could not create %s", OutFilename.filename().string());
		return false;
	}

	std::wstring OutFileStem = OutFilename.stem().wstring();
	std::transform(OutFileStem.begin(), OutFileStem.end(), OutFileStem.begin(), ::towlower);

	std::unique_ptr<StdText> Text = std::make_unique<StdText>();
	Text->SetBOM(TextFileBOM::UTF8);
	if (!Text->Open(FS.GetDirectory(OutFilename) / OutFileStem += ".s", FileAccessMode::Write))
	{
		Str->Message("EMS Assemble: Error, could not create %ws.asm", OutFileStem.c_str());
		return false;
	}

	std::cout << "EMS Assemble: Assembling directory " << Directory << " to " << EMS.GetPath().filename() << std::endl;

	Text->AddLine(".org %ws_pos\n", OutFileStem.c_str());

	std::uintmax_t Offset = 0;
	std::uintmax_t Type = 0;
	std::uint8_t Id = 0x10;

	for (std::size_t i = 0; i < 320; i++, Type++)
	{
		std::filesystem::path Filename;

		if (Type >= 4) { Type = 0; Id++; }
		if (Type == 0) { Filename = Dir / Str->FormatCStyle("EMD%02X00.BIN", Id); }
		if (Type == 1) { Filename = Dir / Str->FormatCStyle("EMD%02X01.BIN", Id); }
		if (Type == 2) { Filename = Dir / Str->FormatCStyle("EMD%02X.TIM", Id); }
		if (Type == 3) { Filename = Dir / Str->FormatCStyle("EMD%02X.EMD", Id); }

		StdFile File { Filename, FileAccessMode::Read, true, false };
		if (!File.IsOpen())
		{
			std::cout << "EMS Assemble: Write error, cannot open " << File.GetPath().filename() << std::endl;
			Text->AddLine("\t.word 0x%08X, 0x%08X	;; %s\n", 0, 0, Filename.stem().string().c_str());
			continue;
		}

		DATA_POS cdemd_pos{};

		std::uintmax_t FileSize = File.Size();

		if (FileSize)
		{
			cdemd_pos.Size = static_cast<std::uint32_t>(FileSize);
			cdemd_pos.Offset = static_cast<std::uint32_t>(Offset / 0x800);

			std::cout << "EMS Assemble: " << Filename.filename() << " (" << cdemd_pos.Size << " bytes) at sector [" << cdemd_pos.Offset << "]" << std::endl;

			std::vector<std::uint8_t> Buffer = File.buffer();

			EMS.Write(Offset, Buffer.data(), Buffer.size());

			Offset += FileSize;
			File.PosAlign(Offset, 0x800);
		}

		Text->AddLine("\t.word 0x%08X, 0x%08X	;; %s\n", cdemd_pos.Offset, cdemd_pos.Size, Filename.stem().string().c_str());
	}

	std::cout << "EMS Assemble: Successfully assembled " << EMS.GetPath().filename() << std::endl;

	Text->FlushUTF8();
	Text->Close();

	EMS.Close();

	EMS.ResizeAlign(0x800);

	return true;
}


/*
	Extract contents of ITP file container
*/
bool Resident_Evil_2::ExtractITP(std::filesystem::path Input)
{
	StdFile m_Input { Input, FileAccessMode::Read_Ex, true, false };
	if (!m_Input)
	{
		Str->Message("ITP Extraction: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::uintmax_t Offset = 0;
	std::uintmax_t Counter = 0;
	std::uintmax_t FileSize = m_Input.Size();
	std::filesystem::path Dir = m_Input.GetDirectory();

	m_Input.CreateDirectory(Dir / Input.stem());

	std::vector<std::uint8_t> Palette(sizeof(bio2_ds_ST0_texture00_clut01));
	std::memcpy(Palette.data(), bio2_ds_ST0_texture00_clut01, sizeof(bio2_ds_ST0_texture00_clut01));

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
			Texture.SetWidth(40);

			Texture.SetHeight(60);

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
bool Resident_Evil_2::ExtractPIX(std::filesystem::path Input)
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

	if ((Input.filename() == "ITEMALL.PIX") || (Input.filename() == "MIX.PIX"))
	{
		m_Input.CreateDirectory(Dir / Input.stem());

		std::vector<std::uint8_t> Palette(sizeof(bio2_ds_ST0_texture00_clut01));

		std::memcpy(Palette.data(), bio2_ds_ST0_texture00_clut01, sizeof(bio2_ds_ST0_texture00_clut01));

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

	if ((Input.filename() == "MAPS.PIX") ||
		(Input.filename() == "MAP00.PIX") ||
		(Input.filename() == "MAP01.PIX") ||
		(Input.filename() == "MAP02.PIX") ||
		(Input.filename() == "MAP03.PIX") ||
		(Input.filename() == "MAP10.PIX"))
	{
		m_Input.CreateDirectory(Dir / Input.stem());

		while (Offset < FileSize)
		{
			Sony_PlayStation_Texture Texture;

			Texture.STP4Bpp() = false;

			Texture.Create(4, 256, 256, 21);

			std::uintmax_t ClutPtr = 0;

			for (size_t iClut = 0; iClut < 21; iClut++)
			{
				std::vector<std::uint8_t> Palette(sizeof(std::uint16_t) * 16);

				std::memcpy(Palette.data(), &bio2_ds_ST0_texture01_clut[ClutPtr], (sizeof(std::uint16_t) * 16));

				ClutPtr += (sizeof(std::uint16_t) * 16);

				Texture.ImportPalette(Palette, iClut);
			}

			Texture.ImportPixels(m_Input, Offset, 0x8000);

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

			Offset += 0x8000;

			Counter++;
		}

		m_Input.Close();

		return true;
	}

	if (FileSize == 0x4000)
	{
		std::vector<std::uint8_t> Pixels(0x4000);

		m_Input.Read(0, Pixels.data(), 0x4000);

		Sony_PlayStation_Texture Texture;

		Texture.Create(16, 256, 256, 0);

		Texture.ImportPixels(Pixels, 0);

		Texture.Save(Input.replace_extension(".tim"));

		std::unique_ptr<Standard_Image> Image = Texture.GetBitmap();

		Image->SaveAsBitmap(Input.replace_extension(".bmp"));

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


/*
	Extract contents of VBS file container (slus_00748 support only)
*/
bool Resident_Evil_2::ExtractVBS(std::filesystem::path Input)
{
	return true;
}


/*
	Assemble contents of VBS file container
*/
bool Resident_Evil_2::AssembleVBS(std::filesystem::path Directory, std::filesystem::path OutFilename)
{
	return true;
}


/*
	Extract EMD file tables from executable
*/
bool Resident_Evil_2::ExtractCdEmsTablesFromExe(std::uintmax_t StartAddress, std::filesystem::path Input)
{
	if (!Exe->Open(Input.string()))
	{
		Str->Message("CDEMS: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	Standard_FileSystem FS;

	std::unique_ptr<StdText> Text = std::make_unique<StdText>();
	Text->SetBOM(TextFileBOM::UTF8);

	std::vector<DATA_POS> cdemd_pos(300);

	if (!b_PreferAsm)
	{
		if (!Text->Open(FS.GetDirectory(Input) / Text->FormatCStyle("cdemd_pos.h"), FileAccessMode::Write))
		{
			Str->Message("CDEMS: Error, could not create %s", Input.stem().string().c_str());
			Exe->Close();
			return false;
		}

		Text->AddLine("struct emd_pos {\n");
		Text->AddLine("\tunsigned char id;\n");
		Text->AddLine("\tunsigned long int bin0_size;\n");
		Text->AddLine("\tunsigned long int bin1_size;\n");
		Text->AddLine("\tunsigned long int tim_size;\n");
		Text->AddLine("\tunsigned long int emd_size;\n};\n");
	}

	for (std::size_t i = 0; i < 4; i++)
	{
		std::uintmax_t pData = (StartAddress + (i * (sizeof(DATA_POS) * 300)));

		StartAddress += 300;	// checksum table size

		Exe->Read(pData, cdemd_pos.data(), (sizeof(DATA_POS) * 300));

		if (!b_PreferAsm)
		{
			Text->AddLine("emd_pos cdemd%d_pos[0x4B] = {\n", i);

			Text->AddLine("\t//	ID		BIN			BIN			TIM			EMD\n");

			std::uint8_t Id = 0x10;

			for (std::size_t x = 0; x < 300; x += 4, Id++)
			{
				Text->AddLine("\t{	0x%02X,	0x%08X,	0x%08X,	0x%08X, 0x%08X	},	// EM%02X\n",
					Id,
					cdemd_pos[x + 0].Size,
					cdemd_pos[x + 1].Size,
					cdemd_pos[x + 2].Size,
					cdemd_pos[x + 3].Size,
					Id);
			}

			Text->AddLine("};\n");
		}
		else
		{
			String Filename = Text->FormatCStyle("cdemd%d_pos.asm", i);

			if (!Text->Open(FS.GetDirectory(Input) / Filename, FileAccessMode::Write))
			{
				Str->Message("CDEMS: Error, could not create %s", Input.stem().string().c_str());
				Exe->Close();
				return false;
			}

			Text->AddLine(".org %08X\t\t\t\t\t\t;; cdemd%d_pos\n", pData, i);

			std::uint8_t Id = 0x10;

			for (std::size_t x = 0; x < 300; x += 4, Id++)
			{
				Text->AddLine("\t.word 0x%08X, 0x%08X	;; EM%02X00.BIN\n", cdemd_pos[x + 0].Offset, cdemd_pos[x + 0].Size, Id);
				Text->AddLine("\t.word 0x%08X, 0x%08X	;; EM%02X01.BIN\n", cdemd_pos[x + 1].Offset, cdemd_pos[x + 1].Size, Id);
				Text->AddLine("\t.word 0x%08X, 0x%08X	;; EM%02X.TIM\n", cdemd_pos[x + 2].Offset, cdemd_pos[x + 2].Size, Id);
				Text->AddLine("\t.word 0x%08X, 0x%08X	;; EM%02X.EMD\n", cdemd_pos[x + 3].Offset, cdemd_pos[x + 3].Size, Id);
			}

			Text->FlushUTF8();

			Text->Close();
		}

	}

	if (!b_PreferAsm)
	{
		Text->FlushUTF8();

		Text->Close();
	}

	Exe->Close();

	return true;
}


/*
	Extract XA sector from stage_.bin
*/
bool Resident_Evil_2::ExtractXaSectorFromStageBin(std::uintmax_t StartAddress, std::uintmax_t Offset, std::size_t Count, std::filesystem::path Input)
{
	if (!Exe->OpenOverlay(StartAddress, Input.string()))
	{
		Str->Message("STAGEXA: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	Standard_FileSystem FS;

	std::unique_ptr<StdText> Text = std::make_unique<StdText>();
	Text->SetBOM(TextFileBOM::UTF8);
	if (!Text->Open(FS.GetDirectory(Input) / Input.stem().string() += b_PreferAsm ? "_xas.asm" : "_xas.txt", FileAccessMode::Write))
	{
		Str->Message("STAGEXA: Error, could not create %s_xas", Input.stem().string().c_str());
		Exe->Close();
		return false;
	}

	std::uint16_t Xa_sector[3]{};

	for (std::size_t i = 0; i < Count; i++)
	{
		std::uintmax_t pData = (Offset + (i * (sizeof(std::uint16_t) * 3)));

		Exe->Read(pData, Xa_sector, sizeof(std::uint16_t) * 3);

		if (b_PreferAsm)
		{
			Text->AddLine("\t.dh\t\t0x%02X,\t0x%02X,\t0x%02X\t;;\n", Xa_sector[0], Xa_sector[1], Xa_sector[2]);
		}
		else
		{
			Text->AddLine("%d %d %d\n", Xa_sector[0], Xa_sector[1], Xa_sector[2]);
		}

		std::cout << "STAGEXA: " << Text->FormatCStyle("%d %d %d", Xa_sector[0], Xa_sector[1], Xa_sector[2]) << std::endl;
	}

	Text->FlushUTF8();
	Text->Close();

	Exe->Close();

	return true;
}