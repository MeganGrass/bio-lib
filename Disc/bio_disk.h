/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#include <std_text.h>

#include <sony_executable.h>


#pragma pack(push, 1)


struct Capcom_Disk_Version_Alpha
{
	unsigned long sector;	// Disk Sector
	unsigned long size;		// File Size
};


struct Capcom_Disk_Version_1
{
	std::uint16_t sector;	// Disk Sector
	std::uint16_t pad0;		// Always Zero (0)
	unsigned long size;		// File Size
	std::uint8_t sec_high;	// Disk Sector Overflow
	std::uint8_t sum;		// File Checksum
	std::uint16_t pad1;		// Always Zero (0)
};


struct Capcom_Disk_Version_2
{
	unsigned long size;		// File Size
	std::uint16_t sector;	// Disk Sector
	std::uint8_t sec_high;	// Disk Sector Overflow
	std::uint8_t sum;		// File Checksum
};


struct Capcom_Disk_Version_3
{
	unsigned long sector;	// Disk Sector
	unsigned long size;		// File Size
	unsigned long check;	// 
};


struct Capcom_Disk_Version_Custom
{
	unsigned long size;		// File Size
	unsigned long sector;	// Disk Sector
};


#pragma pack(pop)


enum class Capcom_Disk_Version
{
	AlphaAug95 = 1 << 0,	// Bio1 Aug 1995
	AlphaOct95 = 1 << 1,	// Bio1 Oct 1995
	Ver1 = 1 << 2,			// Bio1, Bio1 Director's Cut
	Ver2 = 1 << 3,			// Bio1 Dual Shock, Bio2, Bio3, Mega Man Legends Trial
	Custom = 0,				// Custom Resident Evil 2 Dual Shock Assembly Project
	Unknown = -1			// Unknown Version
};


enum class Capcom_File
{
	Normal = 1 << 0,		// Checksum required
	XA = 1 << 1,			// Checksum is not included in file list
	STR = 1 << 2,			// Checksum is not included in file list
	DA = 1 << 3,			// Checksum is not included in file list
	BS = 1 << 4,			// Checksum is not included in file list
	ITP = 1 << 5,			// Checksum is not included in file list
	VBS = 1 << 6,			// Checksum is not included in file list
	EMS = 1 << 7,			// Checksum is not included in file list
	EXE = 1 << 8			// Special checksum required (Bio1 Only)
};


struct Capcom_Executable {
	String ExeName;
	ULONG pFileList{};
	Capcom_Disk_Version Disk{};
	String FriendlyName;
};


extern int mkpsxiso(char* commandline);


class Capcom_Disk {
private:

	// List of Capcom Executables
	std::vector<Capcom_Executable> List;

	// Sony PlayStation Executable
	std::unique_ptr<Sony_PlayStation_Executable> Exe;

	// Text Reader/Writer
	std::unique_ptr<Standard_Text> Text;

public:

	/*
		Construction
	*/
	explicit Capcom_Disk(void)
	{
		Exe = std::make_unique<Sony_PlayStation_Executable>();
		Text = std::make_unique<Standard_Text>();
	}
	virtual ~Capcom_Disk(void)
	{
		Exe.reset();
		Text.reset();
	}

	/*
		Print Command Line Help
	*/
	static void PrintHelp(void);

	/*
		Command Line Interface
	*/
	void Commandline(StrVec Args);

	/*
		Get disk version from integer
	*/
	Capcom_Disk_Version GetVersion(std::uintmax_t Integral);

	/*
		Get disk version string
	*/
	std::string GetVersion(Capcom_Disk_Version Version);

	/*
		Get Filetype from path
	*/
	Capcom_File GetFiletype(std::filesystem::path Path);

	/*
		Get Filetype string
	*/
	std::string GetFiletype(Capcom_File Filetype);

	/*
		Update Capcom executable disk file list
	*/
	bool Update(std::uintmax_t pFileList, Capcom_Disk_Version Version, std::filesystem::path ExeFilename, std::filesystem::path LbaHeader);


};