/*
*
*	Megan Grass
*	April 20, 2024
*
*
*	TODO:
*
*/


#pragma once

#include <std_basic_fstream.h>

#include <std_text.h>

#include <sony_texture.h>

#include <sony_soundbank.h>

#include <bio_common.h>

#include <bio2_model.h>

#include "bio2_rdt_header.h"

#include "bio2_rdt_header_trial.h"

#include "../rid/bio2_rid.h"

#include "../pri/bio2_pri.h"

#include "../rvd/bio2_rvd.h"

#include "../lit/bio2_lit.h"

#include "../sca/bio2_sca.h"

#include "../blk/bio2_blk.h"

#include "../flr/bio2_flr.h"

#include "../edt/bio2_edt.h"

#include "../msg/bio2_msg.h"

#include "../scd/bio2_bytecode.h"


class Resident_Evil_2_RDT :
	private Resident_Evil_Common {
private:

	// Copy
	Resident_Evil_2_RDT(const Resident_Evil_2_RDT&) = delete;
	Resident_Evil_2_RDT& operator = (const Resident_Evil_2_RDT&) = delete;

	// Move
	Resident_Evil_2_RDT(Resident_Evil_2_RDT&&) = delete;
	Resident_Evil_2_RDT& operator = (Resident_Evil_2_RDT&&) = delete;

	// Flag
	bool b_Open;

public:

	// Resident Evil 2 RDT - Header
	Resident_Evil_2_RDT_Header Header;

	// Resident Evil 2 RID - Camera View and Projection
	std::unique_ptr<Resident_Evil_2_RID> Rid;

	// Resident Evil 2 PRI - Object Sprites
	std::vector<std::unique_ptr<Resident_Evil_2_PRI>> Pri;

	// Resident Evil 2 RVD - Camera View Frustum and Switches
	std::unique_ptr<Resident_Evil_2_RVD> Rvd;

	// Resident Evil 2 LIT - Lighting
	std::unique_ptr<Resident_Evil_2_LIT> Lit;

	// Resident Evil 2 SCA - Collision Mesh
	std::unique_ptr<Resident_Evil_2_SCA> Sca;

	// Resident Evil 2 BLK - Enemy Navigation Mesh
	std::unique_ptr<Resident_Evil_2_BLK> Blk;

	// Resident Evil 2 FLR - Floor Sound Mesh
	std::unique_ptr<Resident_Evil_2_FLR> Flr;

	// Resident Evil 2 SCD - System Function (called only once when setting a room)
	std::vector<std::unique_ptr<Resident_Evil_2_Bytecode>> ScdX;

	// Resident Evil 2 SCD - System Function (called every loop)
	std::vector<std::unique_ptr<Resident_Evil_2_Bytecode>> Scd;

	// Resident Evil 2 MSG - Japanese Text
	std::vector<std::unique_ptr<Resident_Evil_2_Message>> Message;

	// Resident Evil 2 MSG - Other Language Text
	std::vector<std::unique_ptr<Resident_Evil_2_Message>> Message_sub;

	// Resident Evil 2 Camera Scroll Texture (320x240x16 headerless Sony PlayStation Texture)
	std::unique_ptr<Sony_PlayStation_Texture> Scrl;

	// Resident Evil 2 MD1 - Object Models
	std::vector<std::pair<std::shared_ptr<Sony_PlayStation_Texture>, std::shared_ptr<Resident_Evil_2_MD1>>> Omodel;

	// Sony PlayStation Soundbank
	std::unique_ptr<Resident_Evil_2_EDT> Edt0;
	std::unique_ptr<Resident_Evil_2_EDT> Edt1;
	std::unique_ptr<Sony_PlayStation_Soundbank> Vab0;
	std::unique_ptr<Sony_PlayStation_Soundbank> Vab1;

	// Resident Evil 2 RBJ - Model Animation
	std::unique_ptr<Resident_Evil_2_Animation> Rbj;

	/*
		Construction
	*/
	explicit Resident_Evil_2_RDT(void) :
		b_Open(false),
		Header(),
		Rid(std::make_unique<Resident_Evil_2_RID>()),
		Rvd(std::make_unique<Resident_Evil_2_RVD>()),
		Lit(std::make_unique<Resident_Evil_2_LIT>()),
		Sca(std::make_unique<Resident_Evil_2_SCA>()),
		Blk(std::make_unique<Resident_Evil_2_BLK>()),
		Flr(std::make_unique<Resident_Evil_2_FLR>()),
		Scrl(std::make_unique<Sony_PlayStation_Texture>()),
		Edt0(std::make_unique<Resident_Evil_2_EDT>()),
		Edt1(std::make_unique<Resident_Evil_2_EDT>()),
		Vab0(std::make_unique<Sony_PlayStation_Soundbank>()),
		Vab1(std::make_unique<Sony_PlayStation_Soundbank>()),
		Rbj(std::make_unique<Resident_Evil_2_Animation>())
	{
		SetGame(Resident_Evil_Video_Game::Resident_Evil_2);
	}

	/*
		Destruction
	*/
	~Resident_Evil_2_RDT(void)
	{
		Close();
		Rid.reset();
		Rvd.reset();
		Lit.reset();
		Sca.reset();
		Blk.reset();
		Flr.reset();
		Scrl.reset();
		Edt0.reset();
		Edt1.reset();
		Vab0.reset();
		Vab1.reset();
		Rbj.reset();
	}

	/*
		Inheritance
	*/
	using Resident_Evil_Common::GetGame;
	using Resident_Evil_Common::Stage;
	using Resident_Evil_Common::Room;
	using Resident_Evil_Common::Disk;

	/*
		Set game
	*/
	virtual void SetGame(Resident_Evil_Video_Game _Game) override;

	/*
		Check if the room is open
	*/
	bool operator !() { return !b_Open; }

	/*
		Check if the room is open
	*/
	bool IsOpen(void) const noexcept { return b_Open; }

	/*
		Open
	*/
	bool Open(std::filesystem::path Path);

	/*
		Close
	*/
	void Close(void);

	/*
		Get camera count
	*/
	std::uint8_t GetCameraCount(void) const noexcept { return Header.nCut; }

	/*
		Set camera count
	*/
	void SetCameraCount(std::uint8_t Count) noexcept;

	/*
		Get object model count
	*/
	std::size_t GetObjectModelCount(void) const noexcept { return Omodel.size(); }

	/*
		Set object model count
	*/
	void SetObjectModelCount(std::uint8_t Count) noexcept;

};