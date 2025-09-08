/*
*
*	Megan Grass
*	April 20, 2024
*
*/


#pragma once

#include <sony_soundbank.h>

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

#include "../esp/bio2_esp.h"

#include <bio_model.h>

#include <rdt/bio2_nov96_rdt.h>


#pragma pack(push, 1)


struct Resident_Evil_2_RDT_Header_Trial_Ver
{
	std::uint8_t nSprite;		// XX // 0x00
	std::uint8_t nCut;			// XX // 0x01
	std::uint8_t nOmodel;		// XX // 0x02
	std::uint8_t nItem;			// XX // 0x03
	std::uint8_t nDoor;			// XX // 0x04
	std::uint8_t nRoom_at;		// XX // 0x05
	std::uint8_t Reverb_lv;		// XX // 0x06
	std::uint8_t nSprite_max;	// XX // 0x07
	std::uint32_t pEdt0;		//  0 // 0x08
	std::uint32_t pVh0;			//  1 // 0x0C
	std::uint32_t pEdt1;		//  2 // 0x10
	std::uint32_t pVh1;			//  3 // 0x14
	std::uint32_t pVb;			//  4 // 0x18
	std::uint32_t pRbj_end;		//  5 // 0x1C
	std::uint32_t pSca;			//  6 // 0x20
	std::uint32_t pRcut;		//  7 // 0x24
	std::uint32_t pVcut;		//  8 // 0x28
	std::uint32_t pLight;		//  9 // 0x2C
	std::uint32_t pOmodel;		// 10 // 0x30	// tagMODEL_LINK_INFO
	std::uint32_t pFloor;		// 11 // 0x34
	std::uint32_t pBlock;		// 12 // 0x38
	std::uint32_t pMessage;		// 13 // 0x3C
	std::uint32_t pMessage_sub;	// 14 // 0x40
	std::uint32_t pScrl;		// 15 // 0x44
	std::uint32_t pScdx;		// 16 // 0x48	// System function (called only once when setting a room)
	std::uint32_t pScd;			// 17 // 0x4C	// System function (called every loop)
	std::uint32_t pEsp_hed;		// 18 // 0x50
	std::uint32_t pEsp_end;		// 19 // 0x54
	std::uint32_t pEsp_tim;		// 20 // 0x58
	std::uint32_t pEsp_tim_end;	// 21 // 0x5C
	std::uint32_t pRbj;			// 22 // 0x60
};

struct Resident_Evil_2_RDT_Header
{
	std::uint8_t nSprite;		// XX // 0x00
	std::uint8_t nCut;			// XX // 0x01
	std::uint8_t nOmodel;		// XX // 0x02
	std::uint8_t nItem;			// XX // 0x03
	std::uint8_t nDoor;			// XX // 0x04
	std::uint8_t nRoom_at;		// XX // 0x05
	std::uint8_t Reverb_lv;		// XX // 0x06
	std::uint8_t nSprite_max;	// XX // 0x07
	std::uint32_t pEdt;			//  0 // 0x08
	std::uint32_t pVh;			//  1 // 0x0C
	std::uint32_t pVb;			//  2 // 0x10
	std::uint32_t pZero0;		//  3 // 0x14	// always zero (0)
	std::uint32_t pZero1;		//  4 // 0x18	// always zero (0)
	std::uint32_t pRbj_end;		//  5 // 0x1C
	std::uint32_t pSca;			//  6 // 0x20
	std::uint32_t pRcut;		//  7 // 0x24
	std::uint32_t pVcut;		//  8 // 0x28
	std::uint32_t pLight;		//  9 // 0x2C
	std::uint32_t pOmodel;		// 10 // 0x30	// tagMODEL_LINK_INFO
	std::uint32_t pFloor;		// 11 // 0x34
	std::uint32_t pBlock;		// 12 // 0x38
	std::uint32_t pMessage;		// 13 // 0x3C
	std::uint32_t pMessage_sub;	// 14 // 0x40
	std::uint32_t pScrl;		// 15 // 0x44
	std::uint32_t pScdx;		// 16 // 0x48	// System function (called only once when setting a room)
	std::uint32_t pScd;			// 17 // 0x4C	// System function (called every loop)
	std::uint32_t pEsp_hed;		// 18 // 0x50
	std::uint32_t pEsp_end;		// 19 // 0x54
	std::uint32_t pEsp_tim;		// 20 // 0x58
	std::uint32_t pEsp_tim_end;	// 21 // 0x5C
	std::uint32_t pRbj;			// 22 // 0x60
};


#pragma pack(pop)


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

	using Resident_Evil_Common::m_Stage;
	using Resident_Evil_Common::m_Room;
	using Resident_Evil_Common::m_Disk;

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