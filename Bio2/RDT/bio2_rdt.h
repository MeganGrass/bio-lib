/*
*
*	Megan Grass
*	April 20, 2024
*
*/


#pragma once

#include <sony_soundbank.h>

#include "bio2_rdt_header.h"

#include "bio2_rdt_header_trial.h"

#include "bio2_rdt_header_nov96.h"

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

#include <bio_model.h>


class Resident_Evil_2_RDT :
	private Resident_Evil_Common {
private:

	// Standard String for messages/debugging
	Standard_String Str;

	// Copy
	Resident_Evil_2_RDT(const Resident_Evil_2_RDT&) = delete;
	Resident_Evil_2_RDT& operator = (const Resident_Evil_2_RDT&) = delete;

	// Move
	Resident_Evil_2_RDT(Resident_Evil_2_RDT&&) = delete;
	Resident_Evil_2_RDT& operator = (Resident_Evil_2_RDT&&) = delete;

	// Flag
	bool b_Open;

public:

	using Resident_Evil_Common::Stage;
	using Resident_Evil_Common::Room;
	using Resident_Evil_Common::Disk;

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
		Rbj(std::make_shared<Resident_Evil_Animation>())
	{
		SetGame(Video_Game::Resident_Evil_2);
		Rbj->SetGame(Video_Game::Resident_Evil_2);
		Rbj->SetType(Resident_Evil_Animation_Type::Room);
	}

	~Resident_Evil_2_RDT(void) = default;

	// File Path
	std::filesystem::path m_Path;

	// File Header
	Resident_Evil_2_RDT_Header Header;

	// RID - Camera FOV, View and Projection
	std::unique_ptr<Resident_Evil_2_RID> Rid;

	// PRI - Object Sprites
	std::vector<std::unique_ptr<Resident_Evil_2_PRI>> Pri;

	// RVD - Camera View Frustum and Switches
	std::unique_ptr<Resident_Evil_2_RVD> Rvd;

	// LIT - Lighting
	std::unique_ptr<Resident_Evil_2_LIT> Lit;

	// SCA - Collision Mesh
	std::unique_ptr<Resident_Evil_2_SCA> Sca;

	// BLK - Enemy Navigation Mesh
	std::unique_ptr<Resident_Evil_2_BLK> Blk;

	// FLR - Floor Sound Mesh
	std::unique_ptr<Resident_Evil_2_FLR> Flr;

	// SCD - System Function (called only once when setting a room)
	std::vector<std::unique_ptr<Resident_Evil_2_Bytecode>> ScdX;

	// SCD - System Function (called every loop)
	std::vector<std::unique_ptr<Resident_Evil_2_Bytecode>> Scd;

	// MSG - Japanese Text
	std::vector<std::unique_ptr<Resident_Evil_2_Message>> Message;

	// MSG - Other Language Text
	std::vector<std::unique_ptr<Resident_Evil_2_Message>> Message_sub;

	// Camera Scroll Texture (320x240x16 headerless Sony PlayStation Texture)
	std::unique_ptr<Sony_PlayStation_Texture> Scrl;

	// MD1 - Object Models
	std::vector<std::pair<std::shared_ptr<Sony_PlayStation_Texture>, std::shared_ptr<Resident_Evil_2_MD1>>> Omodel;

	// Sony PlayStation Soundbank
	std::unique_ptr<Resident_Evil_2_EDT> Edt0;
	std::unique_ptr<Resident_Evil_2_EDT> Edt1;
	std::unique_ptr<Sony_PlayStation_Soundbank> Vab0;
	std::unique_ptr<Sony_PlayStation_Soundbank> Vab1;

	// RBJ - Model Animation
	std::shared_ptr<Resident_Evil_Animation> Rbj;

	// set window handle for message/debugging
	void SetWindow(HWND hWnd)
	{
		Str.hWnd = hWnd;
		Rbj->Str.hWnd = hWnd;
	}

	// Set video game type
	virtual void SetGame(Video_Game Game) override
	{
		switch (Game)
		{
		case Video_Game::Resident_Evil_2_Nov_6_1996:
		case Video_Game::Resident_Evil_2_Trial:
		case Video_Game::Resident_Evil_2_Oct_30_1997:
		case Video_Game::Resident_Evil_2:
		case Video_Game::Resident_Evil_2_Dual_Shock:
			Resident_Evil_Common::SetGame(Game);
			break;
		default:
			SetGame(Video_Game::Resident_Evil_2);
			break;
		}
		Rbj->SetGame(Game);
	}

	// Is the room open?
	bool IsOpen(void) const noexcept { return b_Open; }

	// Open
	bool Open(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	// Close
	void Close(void);

	// Get camera count
	std::uint8_t GetCameraCount(void) const noexcept { return Header.nCut; }

};