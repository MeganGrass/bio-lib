/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#pragma once

#include <lzo.h>

#include <bio_common.h>


enum class CDX_File_Type
{
	Normal = (1 << 0),		// Generic File Container
	BIN = (1 << 1),			// Overlay File Container
	BSS = (1 << 2),			// BS File Container
	DAT = (1 << 3),			// Data File Container
	DO2 = (1 << 4),			// Door File Container
	EMD = (1 << 5),			// EMD File Container
	PLD = (1 << 6),			// PLD File Container
	PLW = (1 << 7),			// PLW File Container
	SND_ARMS = (1 << 8),	// Sound File Container
	SND_CORE = (1 << 9),	// Sound File Container
	SND_EMD = (1 << 10),	// Sound File Container
	SND_MAIN = (1 << 11),	// Sound File Container
	SND_SUB = (1 << 12),	// Sound File Container
	RDT = (1 << 13),		// RDT File Container
	XA = (1 << 14),			// XA File Container
	STR = (1 << 15),		// STR File Container
};


class CDX_File_Container {
private:

	// CDX File Type constants
	static constexpr int NORMAL = std::to_underlying(CDX_File_Type::Normal);
	static constexpr int BIN = std::to_underlying(CDX_File_Type::BIN);
	static constexpr int BSS = std::to_underlying(CDX_File_Type::BSS);
	static constexpr int DAT = std::to_underlying(CDX_File_Type::DAT);
	static constexpr int DO2 = std::to_underlying(CDX_File_Type::DO2);
	static constexpr int EMD = std::to_underlying(CDX_File_Type::EMD);
	static constexpr int PLD = std::to_underlying(CDX_File_Type::PLD);
	static constexpr int PLW = std::to_underlying(CDX_File_Type::PLW);
	static constexpr int SND_ARMS = std::to_underlying(CDX_File_Type::SND_ARMS);
	static constexpr int SND_CORE = std::to_underlying(CDX_File_Type::SND_CORE);
	static constexpr int SND_EMD = std::to_underlying(CDX_File_Type::SND_EMD);
	static constexpr int SND_MAIN = std::to_underlying(CDX_File_Type::SND_MAIN);
	static constexpr int SND_SUB = std::to_underlying(CDX_File_Type::SND_SUB);
	static constexpr int RDT = std::to_underlying(CDX_File_Type::RDT);
	static constexpr int XA = std::to_underlying(CDX_File_Type::XA);
	static constexpr int STR = std::to_underlying(CDX_File_Type::STR);

	// Print Command Line Help
	void PrintHelp(void);

	// Get CDX file type from integer
	CDX_File_Type GetType(std::uintmax_t Integral);

	// Get CDX file type string
	std::string GetType(CDX_File_Type Type);

	// Calculate the sector size from a given file size
	std::uintmax_t GetSectorSize(std::uintmax_t FileSize) { return ((FileSize + 0x7FF) >> 11); }

	// Write list of files to CDX
	bool Write(StdFile& CDX, std::vector<std::filesystem::path> FileList);

public:

	/*
		Construction
	*/
	explicit CDX_File_Container(void)
	{
	}
	virtual ~CDX_File_Container(void)
	{
	}

	/*
		Command Line Interface
	*/
	void Commandline(StrVec Args);

	/*
		Create Normal CDX file container
		 - archive all regular files in Directory to Filename
	*/
	bool Create(CDX_File_Type Type, std::filesystem::path Directory, std::filesystem::path Filename);

	/*
		Create BSS CDX file containers
		 - if bConvertBss is true, BSS files in Directory are converted to the new format
	*/
	bool CreateBSS(std::filesystem::path Directory, std::filesystem::path OutDirectory, Video_Game Game = Video_Game::Resident_Evil_2, bool bConvertBss = false);

	/*
		Create DO2 CDX file container
	*/
	bool CreateDO2(std::filesystem::path Directory, std::filesystem::path OutDirectory);

	/*
		Create EMD CDX file container/s
		 - if bConvertCdEms is true, OutFilename is output directory and CDEMD0.EMS/CDEMD1.EMS (slus_00748 only supported) are converted to the new format
	*/
	bool CreateEMD(std::filesystem::path Directory, std::filesystem::path OutFilename, bool bConvertCdEms = false);

	/*
		Create PLD CDX file container
	*/
	bool CreatePLD(std::filesystem::path Directory, std::filesystem::path OutDirectory);

	/*
		Create PLW CDX file containers
		 - if bInitialBuild is true, missing PLWs for each PLD are created
	*/
	bool CreatePLW(std::filesystem::path Directory, std::filesystem::path OutDirectory, bool bInitialBuild = false);

	/*
		Create SND ARMS CDX file container
	*/
	bool CreateSndARMS(std::filesystem::path Directory, std::filesystem::path OutDirectory);

	/*
		Create SND CORE CDX file container
		 - if bInitialBuild is true, missing COREs for each PLD are created
	*/
	bool CreateSndCORE(std::filesystem::path Directory, std::filesystem::path OutDirectory, bool bInitialBuild = false);

	/*
		Create BGM CDX file containers
		 - if bSub is true, SNDSUB.CDX is created instead of SNDMAIN.CDX
	*/
	bool CreateSndBGM(std::filesystem::path Directory, std::filesystem::path OutDirectory, bool bSub = false);

	/*
		Create RDT CDX file container
		 - Disk: 0=PL0, 1=PL1, 2=Ex-Battle
	*/
	bool CreateRDT(std::filesystem::path Directory, std::filesystem::path OutDirectory, std::uint8_t Disk);

};