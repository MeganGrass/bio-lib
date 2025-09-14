/*
*
*	Megan Grass
*	September 4, 2025
*
*/


#pragma once

#include <bio_common.h>

#include <rdt/bio1_rdt.h>

#include <rdt/bio2_rdt.h>

#include <rdt/bio3_rdt.h>


struct Resident_Evil_Room_Header
{
	std::uintmax_t nSprite;
	std::uintmax_t nCut;
	std::uintmax_t nOmodel;
	std::uintmax_t nItem;
	std::uintmax_t nDoor;
	std::uintmax_t nRoom_at;
	std::uintmax_t Reverb_lv;
	std::uintmax_t nSprite_max;
	std::uintmax_t pRcut;
	std::uintmax_t pVcut;
	std::uintmax_t pLight;
	std::uintmax_t pSca;
	std::uintmax_t pBlock;
	std::uintmax_t pFloor;
	std::uintmax_t pOta;
	std::uintmax_t pScdX0;
	std::uintmax_t pScdX1;
	std::uintmax_t pScd0;
	std::uintmax_t pScd1;
	std::uintmax_t pScdEv0;
	std::uintmax_t pScdEv1;
	std::uintmax_t pMessage;
	std::uintmax_t pMessage_sub;
	std::uintmax_t pItemPix;
	std::uintmax_t pScrl;
	std::uintmax_t pOmodel;
	std::uintmax_t pImodel;
	std::uintmax_t pEsp_hed;
	std::uintmax_t pEsp_end;
	std::uintmax_t pEsp_tim;
	std::uintmax_t pEsp_tim_end;
	std::uintmax_t pEdd;
	std::uintmax_t pEmr;
	std::uintmax_t pRbj;
	std::uintmax_t pRbj_end;
	std::uintmax_t pEdt0;
	std::uintmax_t pVh0;
	std::uintmax_t pVb0;
	std::uintmax_t pEdt1;
	std::uintmax_t pVh1;
	std::uintmax_t pVb1;
};


enum class Room_Pointer_Type :std::uint32_t
{
	RCUT,
	PRI,
	VCUT,
	LIT,
	SCA,
	BLK,
	FLR,
	OTA,
	SCDX0,
	SCDX1,
	SCD0,
	SCD1,
	SCDEV0,
	SCDEV1,
	MSG,
	MSG_SUB,
	IPIX,
	SCRL,
	IMODEL,
	OMODEL,
	IMODEL_MODEL,
	OMODEL_MODEL,
	IMODEL_TEX,
	OMODEL_TEX,
	ESP_HED,
	ESP_END,
	ESP_TIM,
	ESP_TIM_END,
	EDD,
	EMR,
	RBJ,
	RBJ_END,
	EDT0,
	EDT1,
	VH0,
	VB0,
	VH1,
	VB1,
	FILE_SIZE
};


enum class Room_Editor_Type
{
	RCUT,
	PRI,
	VCUT,
	LIT,
	SCA,
	BLK,
	FLR,
	OTA,
	SCD,
	MSG,
	SCRL,
	MODEL,
	ESP,
	RBJ,
	SOUND,
};


class Resident_Evil_Room :
	private Resident_Evil_Common {
private:

	// Copy
	Resident_Evil_Room(const Resident_Evil_Room&) = delete;
	Resident_Evil_Room& operator = (const Resident_Evil_Room&) = delete;

	// Move
	Resident_Evil_Room(Resident_Evil_Room&&) = delete;
	Resident_Evil_Room& operator = (Resident_Evil_Room&&) = delete;

	// Messages/Debugging
	Standard_String Str;

	// Sony PlayStation (1994) Geometry Transformation Engine
	std::shared_ptr<Sony_PlayStation_GTE> GTE;

#ifdef MSTD_DX9

	// Direct-X 9 Render Context
	std::shared_ptr<Standard_DirectX_9> Render;

#endif

	// Header
	Resident_Evil_Room_Header m_Header;

	// Flag
	std::atomic<bool> b_Open;

	/*
		Perspective Flip
		 - requires camera to be flipped
	*/
	bool b_HorzFlip, b_VertFlip;

	const bool ReadHeader(StdFile& File);

	const bool ReadRID(StdFile& File);

	const bool ReadRVD(StdFile& File);

	const bool ReadLIT(StdFile& File);

	const bool ReadSCA(StdFile& File);

	const bool ReadBLK(StdFile& File);

	const bool ReadFLR(StdFile& File);

	const bool ReadOTA(StdFile& File);

	const bool ReadSCD(StdFile& File);

	const bool ReadMSG(StdFile& File);

	const bool ReadIPIX(StdFile& File);

	const bool ReadSCRL(StdFile& File);

	const bool ReadModelItem(StdFile& File);

	const bool ReadModelObject(StdFile& File);

	const bool ReadESP(StdFile& File);

	const bool ReadRBJ(StdFile& File);

	const bool ReadEDT(StdFile& File);

	const bool ReadVAB(StdFile& File);

	const std::uintmax_t GetNextValidPointerBio1(StdFile& File, Room_Pointer_Type Start);

	const std::uintmax_t GetNextValidPointerBio2Nov96(StdFile& File, Room_Pointer_Type Start);

	const std::uintmax_t GetNextValidPointerBio2(StdFile& File, Room_Pointer_Type Start);

	const std::uintmax_t GetNextValidPointerBio3(StdFile& File, Room_Pointer_Type Start);

	void DebugPrint(StdFile& File);

public:

	using Resident_Evil_Common::m_Game;
	using Resident_Evil_Common::m_Stage;
	using Resident_Evil_Common::m_Room;
	using Resident_Evil_Common::m_Disk;
	using Resident_Evil_Common::GameType;
	using Resident_Evil_Common::GameStrW;

	// File Directory
	std::filesystem::path m_Path;

	// RID - Camera FOV, View and Projection
	std::shared_ptr<Resident_Evil_2_RID> Rid;

	// PRI - Camera Sprites
	std::vector<std::shared_ptr<Resident_Evil_2_PRI>> Pri;

	// PRI - Camera Sprite Textures
	std::vector<std::shared_ptr<Sony_PlayStation_Texture>> PriTex;

	// RVD - Camera View Frustum and Switches
	std::shared_ptr<Resident_Evil_2_RVD> Rvd;

	// LIT - Lighting
	std::shared_ptr<Resident_Evil_2_LIT> Lit;

	// SCA - Collision Mesh
	std::shared_ptr<Resident_Evil_2_SCA> Sca;

	// BLK - Enemy Navigation Mesh
	std::shared_ptr<Resident_Evil_2_BLK> Blk;

	// FLR - Floor Sound Mesh
	std::shared_ptr<Resident_Evil_2_FLR> Flr;

	// SCD - System Function (called only once when setting a room)
	std::vector<std::vector<std::uint8_t>> ScdX0;

	// SCD - System Function (called only once when setting a room)
	std::vector<std::vector<std::uint8_t>> ScdX1;

	// SCD - System Function (called every loop)
	std::vector<std::vector<std::uint8_t>> Scd0;

	// SCD - System Function (called every loop)
	std::vector<std::vector<std::uint8_t>> Scd1;

	// SCD - Event Function (called when a cutscene event is triggered)
	std::vector<std::vector<std::uint8_t>> ScdEv0;

	// SCD - Event Function (called when a cutscene event is triggered)
	std::vector<std::vector<std::uint8_t>> ScdEv1;

	// MSG - Main Text
	std::vector<std::vector<std::uint8_t>> Message;

	// MSG - Other Language Text
	std::vector<std::vector<std::uint8_t>> MessageSub;

	// PIX - Item Avatar Texture (40x30x8 headerless Sony PlayStation Texture)
	std::vector<std::shared_ptr<Sony_PlayStation_Texture>> Ipix;

	// PIX - Camera Scroll Texture (320x240x16 headerless Sony PlayStation Texture)
	std::shared_ptr<Sony_PlayStation_Texture> Scrl;

	// TMD/MD1/MD2 - Object Models
	std::vector<std::shared_ptr<Resident_Evil_Model>> Object;

	// TMD/MD1/MD2 - Item Models
	std::vector<std::shared_ptr<Resident_Evil_Model>> Item;

	// ESP - Effect Sprites
	std::shared_ptr<Resident_Evil_2_Effect> Esp;

	// RBJ - Player/SubPlayer/Enemy Model Animation
	std::shared_ptr<Resident_Evil_Animation> Rbj;

	// EDT - Soundbank Index and Attributes
	std::shared_ptr<Resident_Evil_2_EDT> Edt0;

	// EDT - Soundbank Index and Attributes
	std::shared_ptr<Resident_Evil_2_EDT> Edt1;

	// VAB - Sony PlayStation Soundbank
	std::shared_ptr<Sony_PlayStation_Soundbank> Vab0;

	// VAB - Sony PlayStation Soundbank
	std::shared_ptr<Sony_PlayStation_Soundbank> Vab1;

	// Editor Flags
	bool b_EditModel;

	// Item/Object Active In Editor?
	bool b_EditorItem, b_EditorObject;

	// Item Model Index Range
	std::uintmax_t iItem, iItemMin, iItemMax;

	// Object Model Index Range
	std::uintmax_t iObject, iObjectMin, iObjectMax;

	explicit Resident_Evil_Room(void) :
		m_Header{},
		Rid(std::make_shared<Resident_Evil_2_RID>()),
		Rvd(std::make_shared<Resident_Evil_2_RVD>()),
		Lit(std::make_shared<Resident_Evil_2_LIT>()),
		Sca(std::make_shared<Resident_Evil_2_SCA>()),
		Blk(std::make_shared<Resident_Evil_2_BLK>()),
		Flr(std::make_shared<Resident_Evil_2_FLR>()),
		Scrl(std::make_shared<Sony_PlayStation_Texture>()),
		Esp(std::make_shared<Resident_Evil_2_Effect>()),
		Rbj(std::make_shared<Resident_Evil_Animation>()),
		Edt0(std::make_shared<Resident_Evil_2_EDT>()),
		Edt1(std::make_shared<Resident_Evil_2_EDT>()),
		Vab0(std::make_shared<Sony_PlayStation_Soundbank>()),
		Vab1(std::make_shared<Sony_PlayStation_Soundbank>()),
		b_EditModel(false),
		b_EditorItem(false), b_EditorObject(false),
		iItem(0), iItemMin(0), iItemMax(0),
		iObject(0), iObjectMin(0), iObjectMax(0)
	{
		b_Open.store(false);
		SetGame(Video_Game::Resident_Evil_2);
		Rbj->SetType(Resident_Evil_Animation_Type::Room);
	}

	~Resident_Evil_Room(void) = default;

	// Set video game type
	virtual void SetGame(Video_Game Game) override
	{
		Resident_Evil_Common::SetGame(Game);

		Rbj->SetGame(Game);

		for (auto& Model : Object)
		{
			if (Model)
			{
				Model->SetGame(Game);
			}
		}

		for (auto& Model : Item)
		{
			if (Model)
			{
				Model->SetGame(Game);
			}
		}

		for (auto& Animation : Rbj->Data)
		{
			if (Animation)
			{
				Animation->SetGame(Game);
			}
		}
	}

#ifdef _WINDOWS
	// set window handle for message/debugging
	void SetWindow(HWND hWnd)
	{
		Str.hWnd = hWnd;

		Rid->Str.hWnd = hWnd;

		Rvd->Str.hWnd = hWnd;

		Lit->Str.hWnd = hWnd;

		Sca->Str.hWnd = hWnd;

		Blk->Str.hWnd = hWnd;

		Flr->Str.hWnd = hWnd;

		Scrl->Str.hWnd = hWnd;

		Esp->Str.hWnd = hWnd;

		Rbj->Str.hWnd = hWnd;

		Edt0->Str.hWnd = hWnd;

		Edt1->Str.hWnd = hWnd;

		Vab0->Str.hWnd = hWnd;

		Vab1->Str.hWnd = hWnd;

		for (auto& Texture : PriTex)
		{
			if (Texture)
			{
				Texture->Str.hWnd = hWnd;
			}
		}

		for (auto& Animation : Rbj->Data)
		{
			if (Animation)
			{
				Animation->Str.hWnd = hWnd;
			}
		}

		for (auto& Model : Item)
		{
			if (Model)
			{
				Model->SetWindow(hWnd);
			}
		}

		for (auto& Model : Object)
		{
			if (Model)
			{
				Model->SetWindow(hWnd);
			}
		}

		for (auto& Texture : Ipix)
		{
			if (Texture)
			{
				Texture->Str.hWnd = hWnd;
			}
		}
	}
#endif

#ifdef MSTD_DX9
	// initial setup
	void PlatformSetup(HWND hWnd, std::shared_ptr<Sony_PlayStation_GTE> _GTE, std::shared_ptr<Standard_DirectX_9> _Render, bool HorzFlip, bool VertFlip)
	{
		SetWindow(hWnd);
		SetGame(m_Game);
		GTE = _GTE;
		Render = _Render;
		b_HorzFlip = HorzFlip;
		b_VertFlip = VertFlip;
	}
#endif

	// Is the room open?
	[[nodiscard]] const bool IsOpen(void) const noexcept { return b_Open.load(); }

	// Header
	[[nodiscard]] const Resident_Evil_Room_Header& Header(void) const noexcept { return m_Header; }

	// Open
	const bool Open(std::filesystem::path Path);

	// Close
	void Close(void);

	// Get camera count
	const std::uintmax_t GetCameraCount(void) const noexcept { return Header().nCut; }

	// Set Editor
	void SetEditor(Room_Editor_Type Type);

	// Reset Editor
	void ResetEditor(void);

};