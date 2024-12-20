/*
*
*	Megan Grass
*	September 20, 2024
*
*
*	TODO:
*
*/


#include <bio_cdx.h>

#include <bio_disk.h>

#include <bio1.h>

#include <bio2.h>

#include <bio3.h>


/*
	Print Command Line Help
*/
void CDX_File_Container::PrintHelp(void)
{
	std::cout << "CDX File Container: Help" << std::endl << std::endl;

	std::cout << "Required Parameters:" << std::endl;
	std::cout << "\tTYPE <id>\t0=Normal (default)" << std::endl;
	std::cout << "\t\t\t1=BIN" << std::endl;
	std::cout << "\t\t\t2=BSS" << std::endl;
	std::cout << "\t\t\t3=DAT" << std::endl;
	std::cout << "\t\t\t4=DO2" << std::endl;
	std::cout << "\t\t\t5=EMD" << std::endl;
	std::cout << "\t\t\t6=PLD" << std::endl;
	std::cout << "\t\t\t7=PLW" << std::endl;
	std::cout << "\t\t\t8=SND_ARMS" << std::endl;
	std::cout << "\t\t\t9=SND_CORE" << std::endl;
	std::cout << "\t\t\t10=SND_EMD" << std::endl;
	std::cout << "\t\t\t11=SND_MAIN" << std::endl;
	std::cout << "\t\t\t12=SND_SUB" << std::endl;
	std::cout << "\t\t\t13=RDT" << std::endl;
	std::cout << "\t\t\t14=XA" << std::endl;
	std::cout << "\t\t\t15=STR" << std::endl;
	std::cout << "\tDIR <path>\tInput file directory" << std::endl << std::endl;

	std::cout << "EMD Parameters:" << std::endl;
	std::cout << "\tOUT <file/dir>\tOutput filename/directory" << std::endl;
	std::cout << "\tCONV\t\tConvert CDEMS in DIR to CDX (placed in OUT directory)" << std::endl << std::endl;

	std::cout << "PLW Parameters:" << std::endl;
	std::cout << "\tCONV\t\tInitial setup; Create \"missing\" PLW for each PLD" << std::endl << std::endl;

	std::cout << "RDT Parameters:" << std::endl;
	std::cout << "\tOUT <file>\tOutput filename" << std::endl;
	std::cout << "\tDISK <id>\t0=PL0 (default)" << std::endl;
	std::cout << "\t\t\t1=PL1" << std::endl;
	std::cout << "\t\t\t2=Ex-Battle" << std::endl << std::endl;

	std::cout << "BSS Parameters:" << std::endl;
	std::cout << "\tOUT <dir>\tOutput directory" << std::endl;
	std::cout << "\tCONV\t\tConvert GAME type BSS to CDX" << std::endl;
	std::cout << "\tGAME <id>\t1=Bio1, 2=Bio2 (default), 3=Bio3" << std::endl << std::endl;
	std::cout << "BSS Naming Convention, S=Stage (hex), R=Room (hex), C=Camera (dec):" << std::endl;
	std::cout << "\t%DIR%\\ROOM[S][RR]\\ROOM_[S][RR]_[CC].BS" << std::endl;
	std::cout << "\t%DIR%\\ROOM[S][RR]\\ROOM_[S][RR]_[CC]_MASK.TIM" << std::endl;
	std::cout << "Example:" << std::endl;
	std::cout << "\t%DIR%\\ROOM100\\ROOM_100_00.BS" << std::endl;
	std::cout << "\t%DIR%\\ROOM100\\ROOM_100_00_MASK.TIM" << std::endl << std::endl;
}


/*
	Command Line Interface
*/
void CDX_File_Container::Commandline(StrVec Args)
{
	Standard_String Str;

	Standard_FileSystem FS;

	CDX_File_Type Type = CDX_File_Type::Normal;

	Resident_Evil_Video_Game Game = Resident_Evil_Video_Game::Resident_Evil_2;

	bool bConvert = false;

	std::uint8_t Disk = 0;

	std::filesystem::path Directory;

	std::filesystem::path Out;

	for (std::size_t i = 0; i < Args.size(); i++)
	{
		Str.ToUpper(Args[i]);

		if (Args[i] == "HELP")
		{
			PrintHelp();
		}

		if (Args[i] == "TYPE")
		{
			if ((i + 1) < Args.size())
			{
				Type = GetType(std::stoull(Args[i + 1]));
				std::cout << "CDX File Container: Type " << GetType(Type) << std::endl;
			}
			else
			{
				std::cout << "CDX File Container: Parse error, not enough arguments for " << Args[i] << std::endl << std::endl;
				PrintHelp();
			}
		}

		if (Args[i] == "DIR")
		{
			if ((i + 1) < Args.size())
			{
				Directory.clear();
				if (FS.Exists(Args[i + 1]))
				{
					Directory = Args[i + 1];
					std::cout << "CDX File Container: Directory " << Directory << std::endl;
				}
				else
				{
					std::cout << "CDX File Container: Directory error, not found" << std::endl;
				}
			}
			else
			{
				std::cout << "CDX File Container: Parse error, not enough arguments for " << Args[i] << std::endl << std::endl;
				PrintHelp();
			}
		}

		if (Args[i] == "CONV")
		{
			bConvert = true;
		}

		if (Args[i] == "DISK")
		{
			if ((i + 1) < Args.size())
			{
				Disk = static_cast<std::uint8_t>(std::stoull(Args[i + 1]));
				std::cout << "CDX File Container: Disk " << Disk << std::endl;
			}
			else
			{
				std::cout << "CDX File Container: Parse error, not enough arguments for " << Args[i] << std::endl << std::endl;
				PrintHelp();
			}
		}

		if (Args[i] == "GAME")
		{
			if ((i + 1) < Args.size())
			{
				switch (std::stoull(Args[i + 1]))
				{
				case 1:
					Game = Resident_Evil_Video_Game::Resident_Evil;
					std::cout << "CDX File Container: BSS type " << "Resident Evil" << std::endl;
					break;
				case 2:
					Game = Resident_Evil_Video_Game::Resident_Evil_2;
					std::cout << "CDX File Container: BSS type " << "Resident Evil 2" << std::endl;
					break;
				case 3:
					Game = Resident_Evil_Video_Game::Resident_Evil_3;
					std::cout << "CDX File Container: BSS type " << "Resident Evil 3" << std::endl;
					break;
				default:
					std::cout << "CDX File Container: Parse error, unsupported game type " << Args[i + 1] << std::endl;
					break;
				}
			}
			else
			{
				std::cout << "CDX File Container: Parse error, not enough arguments for " << Args[i] << std::endl << std::endl;
				PrintHelp();
			}
		}

		if (Args[i] == "OUT")
		{
			Out.clear();
			if ((i + 1) < Args.size())
			{
				Out = Args[i + 1];
				std::cout << "CDX File Container: Output " << Out << std::endl;
			}
			else
			{
				std::cout << "CDX File Container: Parse error, not enough arguments for " << Args[i] << std::endl << std::endl;
				PrintHelp();
			}
		}
	}

	switch (std::to_underlying(Type) & (NORMAL | BIN | BSS | DAT | DO2 | EMD | PLD | PLW | SND_ARMS | SND_CORE | SND_EMD | SND_MAIN | SND_SUB | RDT | XA | STR))
	{
	case NORMAL:
	case BIN:
	case DAT: Create(Type, Directory, Out); return;
	case DO2: CreateDO2(Directory, Out); return;
	case BSS: CreateBSS(Directory, Out, Game, bConvert); return;
	case EMD: CreateEMD(Directory, Out, bConvert); return;
	case PLD: CreatePLD(Directory, Out); return;
	case PLW: CreatePLW(Directory, Out, bConvert); return;
	case SND_ARMS: CreateSndARMS(Directory, Out); return;
	case SND_CORE: CreateSndCORE(Directory, Out, bConvert); return;
	case SND_MAIN: CreateSndBGM(Directory, Out, false); return;
	case SND_SUB: CreateSndBGM(Directory, Out, true); return;
	case RDT: CreateRDT(Directory, Out, Disk); return;
	}
}


/*
	Get CDX file type from integer
*/
CDX_File_Type CDX_File_Container::GetType(std::uintmax_t Integral)
{
	switch (Integral)
	{
	case 0: return CDX_File_Type::Normal;
	case 1: return CDX_File_Type::BIN;
	case 2: return CDX_File_Type::BSS;
	case 3: return CDX_File_Type::DAT;
	case 4: return CDX_File_Type::DO2;
	case 5: return CDX_File_Type::EMD;
	case 6: return CDX_File_Type::PLD;
	case 7: return CDX_File_Type::PLW;
	case 8: return CDX_File_Type::SND_ARMS;
	case 9: return CDX_File_Type::SND_CORE;
	case 10: return CDX_File_Type::SND_EMD;
	case 11: return CDX_File_Type::SND_MAIN;
	case 12: return CDX_File_Type::SND_SUB;
	case 13: return CDX_File_Type::RDT;
	case 14: return CDX_File_Type::XA;
	case 15: return CDX_File_Type::STR;
	}
	return CDX_File_Type::Normal;
}


/*
	Get CDX file type string
*/
std::string CDX_File_Container::GetType(CDX_File_Type Type)
{
	switch (Type)
	{
	case CDX_File_Type::Normal: return "Normal";
	case CDX_File_Type::BIN: return "BIN";
	case CDX_File_Type::BSS: return "BSS";
	case CDX_File_Type::DAT: return "DAT";
	case CDX_File_Type::DO2: return "DO2";
	case CDX_File_Type::EMD: return "EMD";
	case CDX_File_Type::PLD: return "PLD";
	case CDX_File_Type::PLW: return "PLW";
	case CDX_File_Type::SND_ARMS: return "SND_ARMS";
	case CDX_File_Type::SND_CORE: return "SND_CORE";
	case CDX_File_Type::SND_EMD: return "SND_EMD";
	case CDX_File_Type::SND_MAIN: return "SND_MAIN";
	case CDX_File_Type::SND_SUB: return "SND_SUB";
	case CDX_File_Type::RDT: return "RDT";
	case CDX_File_Type::XA: return "XA";
	case CDX_File_Type::STR: return "STR";
	}
	return "Normal";
}


/*
	Write list of files to CDX
*/
bool CDX_File_Container::Write(StdFile& CDX, std::vector<std::filesystem::path> FileList)
{
	Standard_String Str;

	if (!CDX.IsOpen())
	{
		if (!CDX.Open(CDX.GetPath(), FileAccessMode::Write, true, true))
		{
			Str.Message("CDX File Container: Write error, cannot create or write to %s", CDX.GetPath().filename().string().c_str());
			return false;
		}
	}

	if (FileList.empty())
	{
		Str.Message("CDX File Container: Write error, filelist is empty");
		return false;
	}

	Standard_FileSystem FS;

	std::vector<Capcom_Disk_Version_Custom> floc(FileList.size());

	std::uintmax_t Sector = ((floc.size() * sizeof(Capcom_Disk_Version_Custom)) + 0x7FF) & ~0x7FF;
	Sector /= 0x800;

	for (std::size_t i = 0; i < FileList.size(); i++)
	{
		floc[i].size = static_cast<ULONG>(FS.FileSize(FileList[i]));
		floc[i].sector = static_cast<ULONG>(Sector);

		StdFile File { FileList[i], FileAccessMode::Read, true, false };
		if (!File.IsOpen())
		{
			std::cout << "CDX File Container: Write error, cannot open " << File.GetPath().filename() << std::endl;
			continue;
		}

		std::cout << "CDX File Container: " << FileList[i].filename() << " (" << floc[i].size << " bytes) at sector [" << floc[i].sector << "]" << std::endl;

		CDX.Write(Sector * 0x800, File.buffer().data(), floc[i].size);

		File.Close();

		Sector += GetSectorSize(floc[i].size);
	}

	CDX.Write(0, floc.data(), floc.size() * sizeof(Capcom_Disk_Version_Custom));

	return true;
}


/*
	Create a Normal CDX file container
*/
bool CDX_File_Container::Create(CDX_File_Type Type, std::filesystem::path Directory, std::filesystem::path Filename)
{
	Standard_String Str;

	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Directory);
	if (!FS.Exists(Dir) && !FS.IsDirectory(Dir))
	{
		Str.Message("CDX File Container: Directory error, not found or isn't a directory: \"%s\"", Dir.string().c_str());
		return false;
	}

	std::vector<std::filesystem::path> FileList = FS.GetFileList(Dir);

	if (FileList.empty())
	{
		Str.Message("CDX File Container: Create error, directory is empty");
		return false;
	}

	if (std::to_underlying(Type) & BIN)
	{
		FileList.erase(std::remove_if(FileList.begin(), FileList.end(), [](std::filesystem::path File) { return File.extension() != ".BIN"; }), FileList.end());
	}

	if (std::to_underlying(Type) & XA)
	{
		FileList.erase(std::remove_if(FileList.begin(), FileList.end(), [](std::filesystem::path File) { return File.extension() != ".XAS"; }), FileList.end());
	}

	if (std::to_underlying(Type) & STR)
	{
		FileList.erase(std::remove_if(FileList.begin(), FileList.end(), [](std::filesystem::path File) { return File.extension() != ".STR"; }), FileList.end());
	}

	StdFile CDX { Filename, FileAccessMode::Write, true, true };

	bool bRet = Write(CDX, FileList);

	CDX.Close();

	return bRet;
}


/*
	Create a BSS CDX file containers
*/
bool CDX_File_Container::CreateBSS(std::filesystem::path Directory, std::filesystem::path OutDirectory, Resident_Evil_Video_Game Game, bool bConvertBss)
{
	Standard_String Str;

	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Directory);
	if (!FS.Exists(Dir) && !FS.IsDirectory(Dir))
	{
		Str.Message("CDX File Container: Directory error, not found or isn't a directory: \"%s\"", Dir.string().c_str());
		return false;
	}

	std::vector<std::filesystem::path> FileList = FS.GetFileList(Dir);
	if (FileList.empty())
	{
		Str.Message("CDX File Container: Create error, directory is empty");
		return false;
	}

	std::uint16_t iStageMax = 7;
	std::uint16_t iRoomMax = 0x30;
	std::uint16_t iCutMax = 0;

	std::vector<Capcom_Disk_Version_Custom> floc(iRoomMax);

	switch (Game)
	{
	case Resident_Evil_Video_Game::Resident_Evil:
		iCutMax = BIO1_CAMERA_MAX;
		break;
	case Resident_Evil_Video_Game::Resident_Evil_2:
		iCutMax = BIO2_CAMERA_MAX;
		break;
	case Resident_Evil_Video_Game::Resident_Evil_3:
		iCutMax = BIO3_CAMERA_MAX;
		break;
	default:
		std::cout << "CDX File Container: Create error, unsupported game: " << std::to_underlying(Game) << std::endl;
		return false;
	}

	std::cout << "CDX File Container: " << "BSS max camera " << iCutMax << std::endl;

	if (bConvertBss)
	{
		FileList.erase(std::remove_if(FileList.begin(), FileList.end(), [](std::filesystem::path File) { return File.extension() != ".BSS"; }), FileList.end());
		if (FileList.empty())
		{
			Str.Message("CDX File Container: Create error, directory doesn't contain any BSS files");
			return false;
		}

		std::cout << "CDX File Container: Converting BSS archives to CDX" << std::endl;

		switch (Game)
		{
		case Resident_Evil_Video_Game::Resident_Evil:
		{
			Resident_Evil Bio1;
			for (std::size_t i = 0; i < FileList.size(); i++)
			{
				Bio1.ExtractBSS(FileList[i]);
			}
		}
			break;
		case Resident_Evil_Video_Game::Resident_Evil_2:
		{
			Resident_Evil_2 Bio2;
			for (std::size_t i = 0; i < FileList.size(); i++)
			{
				Bio2.ExtractBSS(FileList[i]);
			}
		}
			break;
		case Resident_Evil_Video_Game::Resident_Evil_3:
		{
			Resident_Evil_3 Bio3;
			for (std::size_t i = 0; i < FileList.size(); i++)
			{
				Bio3.ExtractBSS(FileList[i]);
			}
		}
			break;
		}
	}

	std::unique_ptr<LZO_Compression> Lz = std::make_unique<LZO_Compression>();

	std::filesystem::path OutDir = FS.GetDirectory(OutDirectory);
	if (OutDir.empty()) { OutDir = Dir; }

	for (std::uint8_t Stage = 0; Stage < iStageMax; Stage++)
	{
		std::filesystem::path CdxFilename = OutDir / Str.FormatCStyle("%s\\BSS%02X.CDX", OutDir.string().c_str(), Stage + 1);
		std::cout << "CDX File Container: Creating " << CdxFilename.filename() << std::endl;

		StdFile CDX { CdxFilename, FileAccessMode::Write, true, true };
		if (!CDX.IsOpen())
		{
			std::cout << "CDX File Container: Write error, cannot create " << CDX.GetPath().filename() << std::endl;
			continue;
		}

		std::uintmax_t Sector = 1;

		for (std::uint16_t Room = 0; Room < iRoomMax; Room++)
		{
			floc[Room].sector = static_cast<ULONG>(Sector);

			for (std::uint16_t Cut = 0; Cut < iCutMax; Cut++)
			{
				std::filesystem::path BsFilename = Dir / Str.FormatCStyle("ROOM%X%02X\\ROOM_%X%02X_%02d.BS", Stage + 1, Room, Stage + 1, Room, Cut);
				std::filesystem::path TimFilename = Dir / Str.FormatCStyle("ROOM%X%02X\\ROOM_%X%02X_%02d_MASK.TIM", Stage + 1, Room, Stage + 1, Room, Cut);
				std::filesystem::path LzFilename = Dir / Str.FormatCStyle("ROOM%X%02X\\ROOM_%X%02X_%02d_MASK.LZ", Stage + 1, Room, Stage + 1, Room, Cut);

				StdFile BS { BsFilename, FileAccessMode::Read, true, false };
				if (!BS.IsOpen()) { continue; }

				CDX.Write(Sector * 0x800, BS.buffer().data(), BS.Size());

				BS.Close();

				if (FS.Exists(TimFilename))
				{
					if (!Lz->Compress(TimFilename, LzFilename))
					{
						StdFile LZ { LzFilename, FileAccessMode::Read, true, false };
						if (!LZ.IsOpen()) { continue; }

						CDX.Write((Sector * 0x800) + 0x8000, LZ.buffer().data(), LZ.Size());

						LZ.Close();
					}
					else
					{
						std::cout << "CDX File Container: LZ error when trying to compress " << TimFilename.stem() << std::endl;
					}
				}

				floc[Room].size += 0x10000;

				Sector += GetSectorSize(0x10000);
			}
		}

		CDX.Write(0, floc.data(), floc.size() * sizeof(Capcom_Disk_Version_Custom));

		CDX.Close();

		CDX.ResizeAlign(0x800);

		std::cout << "CDX File Container: " << CdxFilename.filename() << " successfully created" << std::endl;
	}

	return true;
}


/*
	Create a DO2 CDX file container
*/
bool CDX_File_Container::CreateDO2(std::filesystem::path Directory, std::filesystem::path OutDirectory)
{
	Standard_String Str;

	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Directory);
	if (!FS.Exists(Dir) && !FS.IsDirectory(Dir))
	{
		Str.Message("CDX File Container: Directory error, not found or isn't a directory: \"%s\"", Dir.string().c_str());
		return false;
	}

	std::vector<std::filesystem::path> FileList = FS.GetFileList(Dir);
	FileList.erase(std::remove_if(FileList.begin(), FileList.end(), [](std::filesystem::path File) { return File.extension() != ".DO2"; }), FileList.end());
	if (FileList.empty())
	{
		Str.Message("CDX File Container: Create error, directory is empty or doesn't contain any DO2 files");
		return false;
	}

	std::filesystem::path OutDir = FS.GetDirectory(OutDirectory);

	StdFile CDX { OutDir / "DO2.CDX", FileAccessMode::Write, true, true };
	if (!CDX.IsOpen())
	{
		std::cout << "CDX File Container: Create error, cannot create " << CDX.GetPath().filename() << std::endl;
		return false;
	}

	std::vector<std::filesystem::path> Do2List(64);

	for (std::size_t i = 0; i < Do2List.size(); i++)
	{
		Do2List[i] = Dir / Str.FormatCStyle("DOOR%02X.DO2", i);
	}

	bool bRet = Write(CDX, Do2List);

	CDX.Close();

	CDX.ResizeAlign(0x800);

	return bRet;
}


/*
	Create a EMD CDX file container/s
*/
bool CDX_File_Container::CreateEMD(std::filesystem::path Directory, std::filesystem::path OutFilename, bool bConvertCdEms)
{
	Standard_String Str;

	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Directory);
	if (!FS.Exists(Dir) && !FS.IsDirectory(Dir))
	{
		Str.Message("CDX File Container: Directory error, not found or isn't a directory: \"%s\"", Dir.string().c_str());
		return false;
	}

	std::vector<std::filesystem::path> FileList = FS.GetFileList(Dir);
	if (FileList.empty())
	{
		Str.Message("CDX File Container: Create error, directory is empty");
		return false;
	}

	Resident_Evil_2 Bio2;

	if (bConvertCdEms)
	{
		FileList.erase(std::remove_if(FileList.begin(), FileList.end(), [](std::filesystem::path File) { return (File.filename().string().compare("CDEMD0.EMS")) && (File.filename().string().compare("CDEMD1.EMS")); }), FileList.end());
		if (FileList.empty())
		{
			Str.Message("CDX File Container: Create error, directory doesn't contain any EMS files");
			return false;
		}

		std::filesystem::path OutDir = FS.GetDirectory(OutFilename);

		for (std::size_t i = 0; i < FileList.size(); i++)
		{
			if (!Bio2.ExtractEMS(FileList[i])) { return false; }
			if (!Bio2.AssembleEMS(Dir / FileList[i].stem(), OutDir / Str.FormatCStyle("EMD%02X.CDX", i))) { return false; }
		}

		return true;
	}

	/*
		EMD CDX is simply an EMS with a CDX file extension
		In fact, EMS was the inspiration for the CDX functionality... :)
	*/
	return Bio2.AssembleEMS(Dir, OutFilename);
}


/*
	Create a PLD CDX file container
*/
bool CDX_File_Container::CreatePLD(std::filesystem::path Directory, std::filesystem::path OutDirectory)
{
	Standard_String Str;

	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Directory);
	if (!FS.Exists(Dir) && !FS.IsDirectory(Dir))
	{
		Str.Message("CDX File Container: Directory error, not found or isn't a directory: \"%s\"", Dir.string().c_str());
		return false;
	}

	std::vector<std::filesystem::path> FileList = FS.GetFileList(Dir);
	FileList.erase(std::remove_if(FileList.begin(), FileList.end(), [](std::filesystem::path File) { return File.extension() != ".PLD"; }), FileList.end());
	if (FileList.empty())
	{
		Str.Message("CDX File Container: Create error, directory is empty or doesn't contain any PLD files");
		return false;
	}

	std::filesystem::path OutDir = FS.GetDirectory(OutDirectory);

	StdFile CDX { OutDir / "PLD.CDX", FileAccessMode::Write, true, true };
	if (!CDX.IsOpen())
	{
		std::cout << "CDX File Container: Create error, cannot create " << CDX.GetPath().filename() << std::endl;
		return false;
	}

	std::vector<std::filesystem::path> PldList(32);

	for (std::size_t i = 0; i < PldList.size(); i++)
	{
		if (i >= PldList.size() / 2)
		{
			PldList[i] = Dir / Str.FormatCStyle("PL%02XCH.PLD", i  - PldList.size() / 2);
		}
		else
		{
			PldList[i] = Dir / Str.FormatCStyle("PL%02X.PLD", i);
		}
	}

	bool bRet = Write(CDX, PldList);

	CDX.Close();

	CDX.ResizeAlign(0x800);

	return bRet;
}


/*
	Create a PLW CDX file containers
*/
bool CDX_File_Container::CreatePLW(std::filesystem::path Directory, std::filesystem::path OutDirectory, bool bInitialBuild)
{
	Standard_String Str;

	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Directory);
	if (!FS.Exists(Dir) && !FS.IsDirectory(Dir))
	{
		Str.Message("CDX File Container: Directory error, not found or isn't a directory: \"%s\"", Dir.string().c_str());
		return false;
	}

	std::vector<std::filesystem::path> FileList = FS.GetFileList(Dir);
	FileList.erase(std::remove_if(FileList.begin(), FileList.end(), [](std::filesystem::path File) { return File.extension() != ".PLW"; }), FileList.end());
	if (FileList.empty())
	{
		Str.Message("CDX File Container: Create error, directory is empty or doesn't contain any PLW files");
		return false;
	}

	std::filesystem::path OutDir = FS.GetDirectory(OutDirectory);

	if (bInitialBuild)
	{
		std::cout << "CDX File Container: Initial setup for PLW" << std::endl;
		for (std::size_t i = 2; i < 16; i++)
		{
			for (std::size_t x = 0; x < 20; x++)
			{
				if ((i != 0x0B) && (i != 0x0C) && (i != 0x0D) && (i != 0x0E) && (i != 0x0F))
				{
					if (i & 1)
					{
						std::cout << "CDX File Container: Copying " << Str.FormatCStyle("PL%02XW%02X.PLW", 1, x) << " to " << Str.FormatCStyle("PL%02XW%02X.PLW", i, x) << std::endl;
						FS.Copy(Dir / Str.FormatCStyle("PL%02XW%02X.PLW", 1, x), Dir / Str.FormatCStyle("PL%02XW%02X.PLW", i, x), true);
					}
					else
					{
						bool bCopy = true;
						if (i == 0x08)
						{
							if (x == 2) { bCopy = false; }
						}
						if (bCopy)
						{
							std::cout << "CDX File Container: Copying " << Str.FormatCStyle("PL%02XW%02X.PLW", 0, x) << " to " << Str.FormatCStyle("PL%02XW%02X.PLW", i, x) << std::endl;
							FS.Copy(Dir / Str.FormatCStyle("PL%02XW%02X.PLW", 0, x), Dir / Str.FormatCStyle("PL%02XW%02X.PLW", i, x), true);
						}
					}
				}
			}
		}
		std::cout << "CDX File Container: Initial setup for PLW complete" << std::endl;
	}

	for (std::size_t i = 0; i < 16; i++)
	{
		std::filesystem::path Filename = OutDir / Str.FormatCStyle("PLW%02X.CDX", i);
		std::cout << "CDX File Container: Creating " << Filename.filename() << std::endl;

		StdFile CDX { OutDir / Str.FormatCStyle("PLW%02X.CDX", i), FileAccessMode::Write, true, true };
		if (!CDX.IsOpen())
		{
			std::cout << "CDX File Container: Create error, cannot create " << CDX.GetPath().filename() << std::endl;
			return false;
		}

		std::vector<std::filesystem::path> PlwList(20);

		for (std::size_t x = 0; x < PlwList.size(); x++)
		{
			PlwList[x] = Dir / Str.FormatCStyle("PL%02XW%02X.PLW", i, x);

			StdFile PLW { PlwList[x], FileAccessMode::Read, true, false };
			if (!PLW.IsOpen())
			{
				PlwList[x] = "DEADBEEF";
				continue;
			}

			Cyclic_Redundancy_Check CRC;
			std::uint32_t Crc32 = CRC.GetCRC32(PLW.buffer().data(), PLW.Size());

			PLW.Close();

			if (Crc32 == 0xF025E6A8)	// pl1 dummy
			{
				PlwList[x] = "DEADBEEF";
			}

			if (Crc32 == 0x0B0916CE)	// pl1 dummy
			{
				PlwList[x] = "DEADBEEF";
			}
		}

		Write(CDX, PlwList);

		CDX.Write(0xA0, &i, 1);

		CDX.Close();

		CDX.ResizeAlign(0x800);
	}

	return true;
}


/*
	Create SND ARMS CDX file container
*/
bool CDX_File_Container::CreateSndARMS(std::filesystem::path Directory, std::filesystem::path OutDirectory)
{
	Standard_String Str;

	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Directory);
	if (!FS.Exists(Dir) && !FS.IsDirectory(Dir))
	{
		Str.Message("CDX File Container: Directory error, not found or isn't a directory: \"%s\"", Dir.string().c_str());
		return false;
	}

	std::vector<std::filesystem::path> FileList = FS.GetFileList(Dir);
	FileList.erase(std::remove_if(FileList.begin(), FileList.end(), [](std::filesystem::path File) { return (File.extension() != ".EDH") && (File.extension() != ".VB"); }), FileList.end());
	if (FileList.empty())
	{
		Str.Message("CDX File Container: Create error, directory is empty or doesn't contain any EDH/VB files");
		return false;
	}

	std::filesystem::path OutDir = FS.GetDirectory(OutDirectory);

	StdFile CDX { OutDir / "SNDARMS.CDX", FileAccessMode::Write, true, true };
	if (!CDX.IsOpen())
	{
		std::cout << "CDX File Container: Create error, cannot create " << CDX.GetPath().filename() << std::endl;
		return false;
	}

	std::vector<std::filesystem::path> SndList(40);

	for (std::size_t i = 0; i < SndList.size(); i++)
	{
		if (i >= SndList.size() / 2)
		{
			SndList[i] = Dir / Str.FormatCStyle("ARMS%02X.VB", i - SndList.size() / 2);
		}
		else
		{
			SndList[i] = Dir / Str.FormatCStyle("ARMS%02X.EDH", i);
		}
	}

	bool bRet = Write(CDX, SndList);

	CDX.Close();

	CDX.ResizeAlign(0x800);

	return bRet;
}


/*
	Create SND CORE CDX file container
*/
bool CDX_File_Container::CreateSndCORE(std::filesystem::path Directory, std::filesystem::path OutDirectory, bool bInitialBuild)
{
	Standard_String Str;

	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Directory);
	if (!FS.Exists(Dir) && !FS.IsDirectory(Dir))
	{
		Str.Message("CDX File Container: Directory error, not found or isn't a directory: \"%s\"", Dir.string().c_str());
		return false;
	}

	std::vector<std::filesystem::path> FileList = FS.GetFileList(Dir);
	FileList.erase(std::remove_if(FileList.begin(), FileList.end(), [](std::filesystem::path File) { return (File.extension() != ".EDH") && (File.extension() != ".VB"); }), FileList.end());
	if (FileList.empty())
	{
		Str.Message("CDX File Container: Create error, directory is empty or doesn't contain any EDH/VB files");
		return false;
	}

	std::filesystem::path OutDir = FS.GetDirectory(OutDirectory);

	if (bInitialBuild)
	{
		std::cout << "CDX File Container: Initial setup for SND CORE" << std::endl;
		for (std::size_t i = 2; i < 16; i++)
		{
			if ((i != 0x0B) && (i != 0x0C) && (i != 0x0D) && (i != 0x0E) && (i != 0x0F))
			{
				if (i & 1)
				{
					std::cout << "CDX File Container: Copying " << Str.FormatCStyle("CORE%02X.EDH", 1) << " to " << Str.FormatCStyle("CORE%02X.EDH", i) << std::endl;
					FS.Copy(Dir / Str.FormatCStyle("CORE%02X.EDH", 1), Dir / Str.FormatCStyle("CORE%02X.EDH", i), true);
					std::cout << "CDX File Container: Copying " << Str.FormatCStyle("CORE%02X.VB", 1) << " to " << Str.FormatCStyle("CORE%02X.VB", i) << std::endl;
					FS.Copy(Dir / Str.FormatCStyle("CORE%02X.VB", 1), Dir / Str.FormatCStyle("CORE%02X.VB", i), true);
				}
				else
				{
					std::cout << "CDX File Container: Copying " << Str.FormatCStyle("CORE%02X.EDH", 0) << " to " << Str.FormatCStyle("CORE%02X.EDH", i) << std::endl;
					FS.Copy(Dir / Str.FormatCStyle("CORE%02X.EDH", 0), Dir / Str.FormatCStyle("CORE%02X.EDH", i), true);
					std::cout << "CDX File Container: Copying " << Str.FormatCStyle("CORE%02X.VB", 0) << " to " << Str.FormatCStyle("CORE%02X.VB", i) << std::endl;
					FS.Copy(Dir / Str.FormatCStyle("CORE%02X.VB", 0), Dir / Str.FormatCStyle("CORE%02X.VB", i), true);
				}
			}
		}
		std::cout << "CDX File Container: Initial setup for SND CORE complete" << std::endl;
	}

	StdFile CDX{ OutDir / "SNDCORE.CDX", FileAccessMode::Write, true, true };
	if (!CDX.IsOpen())
	{
		std::cout << "CDX File Container: Create error, cannot create " << CDX.GetPath().filename() << std::endl;
		return false;
	}

	std::vector<std::filesystem::path> SndList(44);

	for (std::size_t i = 0; i < SndList.size(); i++)
	{
		if (i >= SndList.size() / 2)
		{
			SndList[i] = Dir / Str.FormatCStyle("CORE%02X.VB", i - SndList.size() / 2);
		}
		else
		{
			SndList[i] = Dir / Str.FormatCStyle("CORE%02X.EDH", i);
		}
	}

	bool bRet = Write(CDX, SndList);

	CDX.Close();

	CDX.ResizeAlign(0x800);

	return bRet;
}


/*
	Create BGM CDX file containers
*/
bool CDX_File_Container::CreateSndBGM(std::filesystem::path Directory, std::filesystem::path OutDirectory, bool bSub)
{
	Standard_String Str;

	Standard_FileSystem FS;

	std::vector<Capcom_Disk_Version_Custom> floc(0x40);

	std::uintmax_t Sector = 1;

	std::filesystem::path OutDir = FS.GetDirectory(OutDirectory);

	std::filesystem::path Dir = FS.GetDirectory(Directory);
	if (!FS.Exists(Dir) && !FS.IsDirectory(Dir))
	{
		Str.Message("CDX File Container: Directory error, not found or isn't a directory: \"%s\"", Dir.string().c_str());
		return false;
	}

	std::vector<std::filesystem::path> FileList = FS.GetFileList(Dir);
	FileList.erase(std::remove_if(FileList.begin(), FileList.end(), [](std::filesystem::path File) { return File.extension() != ".BGM"; }), FileList.end());
	if (FileList.empty())
	{
		Str.Message("CDX File Container: Create error, directory is empty or doesn't contain any BGM files");
		return false;
	}

	std::filesystem::path Filename;
	std::string Typename;

	if (!bSub)
	{
		Filename = OutDir / Str.FormatCStyle("%s\\SNDMAIN.CDX", OutDir.string().c_str());
		Typename = "MAIN";
	}
	else
	{
		Filename = OutDir / Str.FormatCStyle("%s\\SNDSUB.CDX", OutDir.string().c_str());
		Typename = "SUB_";
	}

	StdFile CDX { Filename, FileAccessMode::Write, true, true };
	if (!CDX.IsOpen())
	{
		std::cout << "CDX File Container: Write error, cannot create " << CDX.GetPath().filename() << std::endl;
		return false;
	}

	for (std::uint8_t id = 0; id < 0x40; id++)
	{
		floc[id].sector = static_cast<ULONG>(Sector);

		StdFile BGM{ Dir / Str.FormatCStyle("%s%02X.BGM", Typename.c_str(), id), FileAccessMode::Read, true, false };
		if (!BGM.IsOpen()) { continue; }

		std::cout << "CDX File Container: " << BGM.GetPath().filename() << " (" << BGM.Size() << " bytes) at sector [" << floc[id].sector << "]" << std::endl;

		CDX.Write(Sector * 0x800, BGM.buffer().data(), BGM.Size());

		floc[id].size = static_cast<ULONG>(BGM.Size());

		Sector += GetSectorSize(BGM.Size());

		BGM.Close();
	}

	CDX.Write(0, floc.data(), floc.size() * sizeof(Capcom_Disk_Version_Custom));
	CDX.Close();
	CDX.ResizeAlign(0x800);

	std::cout << "CDX File Container: " << CDX.GetPath().filename() << " successfully created" << std::endl;

	return true;
}


/*
	Create RDT CDX file container
*/
bool CDX_File_Container::CreateRDT(std::filesystem::path Directory, std::filesystem::path OutDirectory, std::uint8_t Disk)
{
	Standard_String Str;

	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Directory);
	if (!FS.Exists(Dir) && !FS.IsDirectory(Dir))
	{
		Str.Message("CDX File Container: Directory error, not found or isn't a directory: \"%s\"", Dir.string().c_str());
		return false;
	}

	std::vector<std::filesystem::path> FileList = FS.GetFileList(Dir);
	FileList.erase(std::remove_if(FileList.begin(), FileList.end(), [](std::filesystem::path File) { return File.extension() != ".RDT"; }), FileList.end());
	if (FileList.empty())
	{
		Str.Message("CDX File Container: Create error, directory is empty or doesn't contain any RDT files");
		return false;
	}

	std::filesystem::path OutDir = FS.GetDirectory(OutDirectory);

	std::int8_t ExStage = 'A';

	for (std::uint8_t Stage = 0; Stage < 7; Stage++)
	{
		std::filesystem::path CdxFilename;
		if (Disk == 2)
		{
			CdxFilename = OutDir / Str.FormatCStyle("%s\\STGEX%02X.CDX", OutDir.string().c_str(), Stage + 1);
		}
		else
		{
			CdxFilename = OutDir / Str.FormatCStyle("%s\\STG%02X%02X.CDX", OutDir.string().c_str(), Disk, Stage + 1);
		}
		std::cout << "CDX File Container: Creating " << CdxFilename.filename() << std::endl;

		StdFile CDX { CdxFilename, FileAccessMode::Write, true, true };
		if (!CDX.IsOpen())
		{
			std::cout << "CDX File Container: Write error, cannot create " << CDX.GetPath().filename() << std::endl;
			continue;
		}

		std::vector<Capcom_Disk_Version_Custom> floc(0x30);

		std::uintmax_t Sector = 1;

		for (std::uint16_t Room = 0; Room < 0x30; Room++)
		{
			floc[Room].sector = static_cast<ULONG>(Sector);

			std::filesystem::path Filename;
			if (Disk == 2)
			{
				Filename = Dir / Str.FormatCStyle("ROOM%C%02X%X.RDT", ExStage + Stage, Room, 0);
			}
			else
			{
				Filename = Dir / Str.FormatCStyle("ROOM%X%02X%X.RDT", Stage + 1, Room, Disk);
			}

			StdFile RDT { Filename, FileAccessMode::Read, true, false };
			if (!RDT.IsOpen()) { continue; }

			std::cout << "CDX File Container: " << Filename.filename() << " (" << RDT.Size() << " bytes) at sector [" << floc[Room].sector << "]" << std::endl;

			CDX.Write(Sector * 0x800, RDT.buffer().data(), RDT.Size());

			floc[Room].size = static_cast<ULONG>(RDT.Size());

			Sector += GetSectorSize(RDT.Size());

			RDT.Close();
		}

		CDX.Write(0, floc.data(), floc.size() * sizeof(Capcom_Disk_Version_Custom));

		CDX.Close();

		CDX.ResizeAlign(0x800);

		std::cout << "CDX File Container: " << CdxFilename.filename() << " successfully created" << std::endl;
	}

	return true;
}