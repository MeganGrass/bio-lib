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
*		BS Decode: https://github.com/pmandin/reevengi-tools
*
*		SLD Decompression: https://github.com/pmandin/reevengi-tools
*
*/


#pragma once

#include <sony_executable.h>

#include <sony_bitstream.h>

#include "adt/bio2_adt.h"

#include "bgm/bio2_bgm.h"

#include "cpt/bio2_cpt.h"

#include "die/bio2_die.h"

#include "sld/bio2_sld.h"

#include "clut/bio2_clut.h"

#include "pdemo/bio2_pdemo.h"

#include "rdt/bio2_rdt.h"


class Resident_Evil_2 :
	private Resident_Evil_Common {
private:

	// Resident Evil 2 ADT
	std::unique_ptr<Resident_Evil_2_ADT> Adt;

	// Resident Evil 2 BGM
	std::unique_ptr<Resident_Evil_2_BGM> Bgm;

	// Resident Evil 2 CPT
	std::unique_ptr<Resident_Evil_2_CPT> Cpt;

	// Resident Evil 2 DIE
	std::unique_ptr<Resident_Evil_2_DIE> Die;

	// Sony PlayStation Executable
	std::unique_ptr<Sony_PlayStation_Executable> Exe;

	// Sony PlayStation Bitstream
	std::unique_ptr<Sony_PlayStation_Bitstream> Bs;

	// Resident Evil 2 SLD
	std::unique_ptr<Resident_Evil_2_SLD> Sld;

	// Resident Evil 2 Player Demo
	std::unique_ptr<Resident_Evil_2_PDEMO> Demo;

	// Print Command Line Help
	void PrintHelp(void);

public:

	// Resident Evil 2 Room Data
	std::unique_ptr<Resident_Evil_2_RDT> Rdt;

	// Prefer Assembly Text Output
	bool b_PreferAsm;

	/*
		Construction
	*/
	explicit Resident_Evil_2(void) :
		Adt(std::make_unique<Resident_Evil_2_ADT>()),
		Bgm(std::make_unique<Resident_Evil_2_BGM>()),
		Bs(std::make_unique<Sony_PlayStation_Bitstream>()),
		Cpt(std::make_unique<Resident_Evil_2_CPT>()),
		Die(std::make_unique<Resident_Evil_2_DIE>()),
		Exe(std::make_unique<Sony_PlayStation_Executable>()),
		Sld(std::make_unique<Resident_Evil_2_SLD>()),
		Demo(std::make_unique<Resident_Evil_2_PDEMO>()),
		Rdt(std::make_unique<Resident_Evil_2_RDT>()),
		b_PreferAsm(false)
	{
		m_Game = Video_Game::Resident_Evil_2;
	}
	virtual ~Resident_Evil_2(void)
	{
		Adt.reset();
		Bgm.reset();
		Bs.reset();
		Cpt.reset();
		Die.reset();
		Exe.reset();
		Sld.reset();
		Demo.reset();
	}

	/*
		Command Line Interface
	*/
	void Commandline(StrVec Args);

	/*
		Decompress ADT from unsigned char vector
	*/
	bool DecompressADT(std::vector<std::uint8_t> ADT, std::vector<std::uint8_t>& Output) { return Adt->Decompress(ADT, Output); }

	/*
		Decompress ADT from file
	*/
	bool DecompressADT(std::filesystem::path Input, std::filesystem::path Output) { return Adt->Decompress(Input, Output); }

	/*
		Decompress CPT from unsigned char vector
	*/
	bool DecompressCPT(std::vector<std::uint8_t> CPT, std::vector<std::uint8_t>& Output) { return Cpt->Decompress(CPT, Output); }

	/*
		Decompress CPT from file
	*/
	bool DecompressCPT(std::filesystem::path Input) { return Cpt->Decompress(Input); }

	/*
		Decompress SLD from unsigned char vector
	*/
	bool DecompressSLD(std::vector<std::uint8_t> SLD, std::vector<std::uint8_t>& Output) { return Sld->Decompress(SLD, Output); }

	/*
		Decompress SLD from file
	*/
	bool DecompressSLD(std::filesystem::path Input) { return Sld->Decompress(Input); }

	/*
		Extract contents of BGM file container
	*/
	bool ExtractBGM(std::filesystem::path Input) { return Bgm->Extract(Input); }

	/*
		Create BGM file container
	*/
	bool AssembleBGM(std::filesystem::path Input) { return Bgm->Assemble(Input); }

	/*
		Extract BGM table from save file
	*/
	bool ExtractBgmFromSaveFile(std::filesystem::path Input);

	/*
		Open BSS file container
	*/
	std::unique_ptr<Standard_Image> OpenBSS(std::filesystem::path Input, std::int32_t No);

	/*
		Extract contents of BSS file container
	*/
	bool ExtractBSS(std::filesystem::path Input);

	/*
		Extract contents of EMS file container (slus_00748 support only)
	*/
	bool ExtractEMS(std::filesystem::path Input);

	/*
		Assemble contents of EMS file container
	*/
	bool AssembleEMS(std::filesystem::path Directory, std::filesystem::path OutFilename);

	/*
		Extract contents of DIE file container
	*/
	bool ExtractDIE(std::filesystem::path Input) { return Die->Extract(Input); }

	/*
		Create DIE file container
	*/
	bool AssembleDIE(std::filesystem::path Input) { return Die->Assemble(Input); }

	/*
		Extract contents of ITP file container
	*/
	bool ExtractITP(std::filesystem::path Input);

	/*
		Assemble PDEMO file
	*/
	bool AssemblePDEMO(std::filesystem::path Input) { return Demo->AssembleText(Input); }

	/*
		Disassemble PDEMO file
	*/
	bool DisassemblePDEMO(std::filesystem::path Input) { return Demo->DisassembleToText(Input); }

	/*
		Extract contents of PIX file container
	*/
	bool ExtractPIX(std::filesystem::path Input);

	/*
		Extract contents of VBS file container (slus_00748 support only)
	*/
	bool ExtractVBS(std::filesystem::path Input);

	/*
		Assemble contents of VBS file container
	*/
	bool AssembleVBS(std::filesystem::path Directory, std::filesystem::path OutFilename);

	/*
		Extract EMD file tables from executable
	*/
	bool ExtractCdEmsTablesFromExe(std::uintmax_t StartAddress, std::filesystem::path Input);

	/*
		Extract XA sector from stage_.bin
	*/
	bool ExtractXaSectorFromStageBin(std::uintmax_t StartAddress, std::uintmax_t Offset, std::size_t Count, std::filesystem::path Input);

	// Disassemble ROOMCUT.BIN
	bool DisassembleRoomCut(std::filesystem::path Input,
		std::function<void(float, bool&, std::filesystem::path)> ProgressCallback, std::function<void(std::filesystem::path&)> OnComplete);

};