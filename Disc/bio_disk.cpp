/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#include <bio_disk.h>


void Capcom_Disk::PrintHelp(void)
{
    std::cout << "Capcom Disk: Help" << std::endl << std::endl;

    std::cout << "Required Parameters:" << std::endl;
    std::cout << "\tVER <ver>\t\t0=custom, 1=aug95, 2=oct95, 3=bio1, 4=bio2, bio3" << std::endl;
    std::cout << "\tFLOC <address>\t\t\"fpos\" structure address, must be hexadecimal: range 0x80000000 - 0x81FFFFFF" << std::endl;
    std::cout << "\tUPDATE <exe> <lba>\tupdate \"fpos\" structure" << std::endl;
    std::cout << "\t\t<exe> Sony PlayStation executable filename" << std::endl;
    std::cout << "\t\t<lba> Parsable text file created with -retext from cutom mkpsxiso" << std::endl << std::endl;
}

void Capcom_Disk::Commandline(StrVec Args)
{
    Standard_String Str;

    Standard_FileSystem FS;

	Capcom_Disk_Version Version = Capcom_Disk_Version::Unknown;

	std::uintmax_t pFloc = 0x80000000;

    for (std::size_t i = 0; i < Args.size(); i++)
    {
        Str.ToUpper(Args[i]);

        if (Args[i] == "HELP")
        {
            PrintHelp();
        }

        if (Args[i] == "VER")
        {
            if ((i + 1) < Args.size())
            {
                switch (std::stoull(Args[i + 1]))
                {
				case 0: Version = Capcom_Disk_Version::Custom; break;
                case 1: Version = Capcom_Disk_Version::AlphaAug95; break;
				case 2: Version = Capcom_Disk_Version::AlphaOct95; break;
				case 3: Version = Capcom_Disk_Version::Ver1; break;
				case 4: Version = Capcom_Disk_Version::Ver2; break;
                }
                std::cout << "Capcom Disk: Type \"" << GetVersion(Version) << "\"" << std::endl;
            }
            else
            {
                std::cout << "Capcom Disk: Parse error, not enough arguments for " << Args[i] << std::endl << std::endl;
                PrintHelp();
            }
        }

        if (Args[i] == "FLOC")
        {
            if ((i + 1) < Args.size())
            {
                pFloc = std::strtoull(Args[i + 1].c_str(), nullptr, 16);
                std::cout << "Capcom Disk: LBA table at " << std::hex << "0x" << pFloc << std::dec << std::endl;
            }
            else
            {
                std::cout << "Capcom Disk: Parse error, not enough arguments for " << Args[i] << std::endl << std::endl;
                PrintHelp();
            }
        }

        if (Args[i] == "UPDATE")
        {
            if (i + 2 < Args.size())
            {
                std::filesystem::path ExeFilename = Args[i + 1];
                std::filesystem::path LbaHeader = Args[i + 2];
                if ((FS.Exists(ExeFilename)) && (FS.Exists(LbaHeader)))
                {
                    std::cout << "Capcom Disk: Updating " << ExeFilename.filename() << " LBA table with " << LbaHeader.filename() << " at " << std::hex << "0x" << pFloc << std::dec << std::endl;
                    if (Update(pFloc, Version, ExeFilename, LbaHeader))
                    {
                        std::cout << "Capcom Disk: Successfully updated " << ExeFilename.filename() << std::endl;
                    }
                }
                else
				{
					std::cout << "Capcom Disk: Update error, executable and/or mkpsxiso file (-retext) not found" << std::endl;
				}
            }
            else
            {
                std::cout << "Capcom Disk: Parse error, not enough arguments for " << Args[i] << std::endl << std::endl;
                PrintHelp();
            }
        }

    }

}

std::string Capcom_Disk::GetVersion(Capcom_Disk_Version Version)
{
    switch (Version)
    {
    case Capcom_Disk_Version::AlphaAug95: return "BioHazard August 1995 Alpha Ver.";
    case Capcom_Disk_Version::AlphaOct95: return "BioHazard October 1995 Alpha Ver.";
    case Capcom_Disk_Version::Ver1: return "BioHazard (Trial, Director's Cut)";
    case Capcom_Disk_Version::Ver2: return "BioHazard (Director's Cut Dual Shock), BioHazard 2, BioHazard 3";
    case Capcom_Disk_Version::Custom: return "Custom Resident Evil 2 Dual Shock Assembly Project";
    }
    return "Unknown";
}

Capcom_File Capcom_Disk::GetFiletype(std::filesystem::path Path)
{
    if (!Path.extension().string().compare(".XA")) { return Capcom_File::XA; }
    if (!Path.extension().string().compare(".XAS")) { return Capcom_File::XA; }
    if (!Path.extension().string().compare(".STR")) { return Capcom_File::STR; }
    if (!Path.extension().string().compare(".DA")) { return Capcom_File::DA; }
    if (!Path.extension().string().compare(".BS")) { return Capcom_File::BS; }
    if (!Path.extension().string().compare(".BSS")) { return Capcom_File::BS; }
    if (!Path.extension().string().compare(".ITP")) { return Capcom_File::ITP; }
    if (!Path.extension().string().compare(".VBS")) { return Capcom_File::VBS; }
    if (!Path.extension().string().compare(".EMS")) { return Capcom_File::EMS; }
    if (!Path.extension().string().compare(".EXE")) { return Capcom_File::EXE; }
    return Capcom_File::Normal;
}

std::string Capcom_Disk::GetFiletype(Capcom_File File)
{
    switch (File)
    {
    case Capcom_File::Normal: return "Normal Binary Data";
    case Capcom_File::XA: return "CD-ROM eXtended Architecture Data (XA)";
    case Capcom_File::STR: return "Sony PlayStation Motion Decoder Stream (STR)";
    case Capcom_File::DA: return "Compact Disc-Digital Audio (DA)";
    case Capcom_File::BS: return "Sony PlayStation Motion Decoder Bit Stream (BS)";
    case Capcom_File::ITP: return "Resident Evil Item Picture Archive (ITP)";
    case Capcom_File::VBS: return "Resident Evil Enemy Sound Archive (VBS)";
    case Capcom_File::EMS: return "Resident Evil Enemy Data Archive (EMS)";
    case Capcom_File::EXE: return "Sony PlayStation Executable (EXE)";
	}
	return "Unknown";
}

bool Capcom_Disk::Update(std::uintmax_t pFileList, Capcom_Disk_Version Version, std::filesystem::path ExeFilename, std::filesystem::path LbaHeader)
{
	std::unique_ptr<Sony_PlayStation_Executable> Exe = std::make_unique<Sony_PlayStation_Executable>();
    std::unique_ptr<Standard_Text> Text = std::make_unique<Standard_Text>();

    if (!Exe->Open(ExeFilename))
    {
        Text->Message("Capcom Disk: Update error, executable file not found");
        return false;
    }

    if (!Text->Open(LbaHeader, FileAccessMode::Read))
    {
        Text->Message("Capcom Disk: Update error, header file not found");
        Exe->Close();
        return false;
    }

    Exe->PrintHeader(Exe->GetHeader());

    Capcom_Disk_Version_1 FilePos{};

    unsigned long Sector = 0;

    for (std::size_t i = 0; i < Text->GetLineCount(); i++)
    {
        StrVec Args = Text->GetStrVec(Text->GetLine(i));
        if (Args.empty() || (Args.size() < 4))
        {
            Text->Message("Capcom Disk: Update error, not enough arguments in line: %d", i);
            break;
        }

        std::filesystem::path Filename = Args[0];
        if (!Filename.filename().string().compare(ExeFilename.filename().string())) { break; }
        if (!Filename.filename().string().compare("ZNULL.WAV")) { break; }
        if (!Filename.filename().string().compare("ZNULL.DAT")) { break; }
        if (!Filename.filename().string().compare("SYSTEM.CNF")) { break; }
        std::cout << "File: " << Filename.filename() << std::endl;

        Capcom_File Filetype = GetFiletype(Filename);
        std::cout << "\ttype: " << GetFiletype(Filetype) << std::endl;

        FilePos.size = std::strtoul(Args[1].c_str(), nullptr, 16);
        std::cout << "\tsize: " << std::hex << FilePos.size << std::dec << std::endl;

        Sector = std::strtoul(Args[2].c_str(), nullptr, 16);
        if (Sector == 0xDEADBEEF) { break; }
        FilePos.sector = static_cast<std::uint16_t>(Sector);
        FilePos.sec_high = static_cast<std::uint8_t>(Sector >> 16);
        std::cout << "\tsector: " << std::hex << Sector << std::dec << std::endl;

        FilePos.sum = static_cast<std::uint8_t>(std::strtoul(Args[3].c_str(), nullptr, 16));
        std::cout << "\tsum: " << Text->FormatCStyle("%02X", FilePos.sum) << std::endl;

        switch (Version)
        {
        case Capcom_Disk_Version::AlphaAug95:
        {
        }
            break;
        case Capcom_Disk_Version::AlphaOct95:
        {
        }
            break;
        case Capcom_Disk_Version::Ver1:
            Exe->Write(pFileList + (i * sizeof(Capcom_Disk_Version_1)), &FilePos, sizeof(Capcom_Disk_Version_1));
            std::cout << "\tpos: " << std::hex << pFileList + (i * sizeof(Capcom_Disk_Version_1)) << std::dec << std::endl;
            break;
        case Capcom_Disk_Version::Ver2:
        {
            Capcom_Disk_Version_2 FilePos2{};
            FilePos2.size = FilePos.size;
            FilePos2.sector = FilePos.sector;
            FilePos2.sec_high = FilePos.sec_high;
            FilePos2.sum = FilePos.sum;
            Exe->Write(pFileList + (i * sizeof(Capcom_Disk_Version_2)), &FilePos2, sizeof(Capcom_Disk_Version_2));
            std::cout << "\tpos: " << std::hex << pFileList + (i * sizeof(Capcom_Disk_Version_2)) << std::dec << std::endl;
        }
            break;
        case Capcom_Disk_Version::Custom:
        {
            Capcom_Disk_Version_Custom FilePosC{};
            FilePosC.size = FilePos.size;
            FilePosC.sector = Sector;
            Exe->Write(pFileList + (i * sizeof(Capcom_Disk_Version_Custom)), &FilePosC, sizeof(Capcom_Disk_Version_Custom));
            std::cout << "\texe pos: " << std::hex << pFileList + (i * sizeof(Capcom_Disk_Version_Custom)) << std::dec << std::endl;
            std::cout << "\tfile pos: " << std::hex << Exe->GetFileOffset(pFileList + (i * sizeof(Capcom_Disk_Version_Custom))) << std::dec << std::endl;
        }
            break;
        }
	}

    bool bRet = Exe->ExportEXE(ExeFilename.string());

    Exe->Close();

    Text->Close();

    return bRet;
}