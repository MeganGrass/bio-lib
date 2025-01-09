/*
*
*	Megan Grass
*	April 20, 2024
*
*
*	TODO: 
*	
*	support for effect sprites
* 
*	support for model animations
*
*/


#include "bio2_rdt.h"


/*
	Set game
*/
void Resident_Evil_2_RDT::SetGame(Resident_Evil_Video_Game _Game)
{
	switch(_Game)
	{
	case Resident_Evil_Video_Game::Resident_Evil_2_Oct_30_1997:
	case Resident_Evil_Video_Game::Resident_Evil_2_Trial:
	case Resident_Evil_Video_Game::Resident_Evil_2:
	case Resident_Evil_Video_Game::Resident_Evil_2_Dual_Shock:
		Game = _Game;
		break;
	default:
		Game = Resident_Evil_Video_Game::Resident_Evil_2;
		break;
	}
}


/*
	Open
*/
bool Resident_Evil_2_RDT::Open(std::filesystem::path Path)
{
	StdFile File { Path, FileAccessMode::Read, true, false };
	if (!File.IsOpen())
	{
		Str->Message("Resident Evil 2: RDT Error, could not open %s", File.GetPath().filename().string().c_str());
		return false;
	}

	// Close
	if (b_Open) { Close(); }

	// Meta
	GetStageRoom(Path.filename().string().c_str());

	// Header
	File.Read(0, &Header, sizeof(Resident_Evil_2_RDT_Header));

	// Fail-Safe
	{
		if (Header.nCut > BIO2_CAMERA_MAX)
		{
			Str->Message("Resident Evil 2: Aborting RDT, abnormal number of cameras detected in %s", File.GetPath().filename().string().c_str());
			return false;
		}

		std::uint32_t FileSize = static_cast<std::uint32_t>(File.Size());

		if ((Header.pEdt > FileSize) ||
			(Header.pVh > FileSize) ||
			(Header.pVb > FileSize) ||
			(Header.pZero0 > FileSize) ||
			(Header.pZero1 > FileSize) ||
			(Header.pRbj_end > FileSize) ||
			(Header.pSca > FileSize) ||
			(Header.pRcut > FileSize) ||
			(Header.pVcut > FileSize) ||
			(Header.pLight > FileSize) ||
			(Header.pOmodel > FileSize) ||
			(Header.pFloor > FileSize) ||
			(Header.pBlock > FileSize) ||
			(Header.pMessage_sub > FileSize) ||
			(Header.pScrl > FileSize) ||
			(Header.pScdx > FileSize) ||
			(Header.pScd > FileSize) ||
			(Header.pEsp_hed > FileSize) ||
			(Header.pEsp_end > FileSize) ||
			(Header.pEsp_tim > FileSize) ||
			(Header.pEsp_tim_end > FileSize) ||
			(Header.pRbj > FileSize))
		{
			Str->Message("Resident Evil 2: Aborting RDT, abnormal data offsets detected in %s", File.GetPath().filename().string().c_str());
			return false;
		}
	}

	// RID
	if (Header.pRcut)
	{
		Rid->Open(File, Header.nCut, Header.pRcut);

		// PRI
		for (std::uint8_t i = 0; i < Header.nCut; i++)
		{
			Pri.push_back(std::make_unique<Resident_Evil_2_PRI>());

			if (Rid->Get(i)->pSp || (Rid->Get(i)->pSp != 0xFFFFFFFF))
			{
				Pri[i]->Open(File, Rid->Get(i)->pSp);
			}
		}
	}

	// RVD
	if (Header.pVcut) { Rvd->Open(File, Header.pVcut); }

	// LIT
	if (Header.pLight) { Lit->Open(File, Header.nCut, Header.pLight); }

	// SCA
	if (Header.pSca) { Sca->Open(File, Header.pSca); }

	// BLK
	if (Header.pBlock) { Blk->Open(File, Header.pBlock); }

	// FLR
	if (Header.pFloor) { Flr->Open(File, Header.pFloor); }

	// SCD
	if (Header.pScdx)
	{
		std::uint16_t nData = 0;
		std::uint16_t pData = 0;

		File.Read(Header.pScdx, &nData, sizeof(std::uint16_t));

		for (std::uintmax_t i = 0; i < nData / sizeof(std::uint16_t); i++)
		{
			File.Read(Header.pScdx + (i * sizeof(std::uint16_t)), &pData, sizeof(std::uint16_t));

			if (!pData || (pData == 0xFFFF)) { break; }

			ScdX.push_back(std::make_unique<Resident_Evil_2_Bytecode>());

			ScdX[i]->Open(File, static_cast<uintmax_t>(Header.pScdx + pData));
		}

	}

	if (Header.pScd)
	{
		std::uint16_t nData = 0;
		std::uint16_t pData = 0;

		File.Read(Header.pScd, &nData, sizeof(std::uint16_t));

		for (std::uintmax_t i = 0; i < nData / sizeof(std::uint16_t); i++)
		{
			File.Read(Header.pScd + (i * sizeof(std::uint16_t)), &pData, sizeof(std::uint16_t));

			if (!pData || (pData == 0xFFFF)) { break; }

			Scd.push_back(std::make_unique<Resident_Evil_2_Bytecode>());

			Scd[i]->Open(File, static_cast<uintmax_t>(Header.pScd + pData));
		}

	}

	// MSG
	if (Header.pMessage)
	{
		std::uint16_t nData = 0;
		std::uint16_t pData = 0;

		File.Read(Header.pMessage, &nData, sizeof(std::uint16_t));

		for (std::uintmax_t i = 0; i < nData / sizeof(std::uint16_t); i++)
		{
			File.Read(Header.pMessage + (i * sizeof(std::uint16_t)), &pData, sizeof(std::uint16_t));

			if (!pData || (pData == 0xFFFF)) { break; }

			Message.push_back(std::make_unique<Resident_Evil_2_Message>());

			Message[i]->Open(File, static_cast<uintmax_t>(Header.pMessage + pData));
		}

	}

	if (Header.pMessage_sub)
	{
		std::uint16_t nData = 0;
		std::uint16_t pData = 0;

		File.Read(Header.pMessage_sub, &nData, sizeof(std::uint16_t));

		for (std::uintmax_t i = 0; i < nData / sizeof(std::uint16_t); i++)
		{
			File.Read(Header.pMessage_sub + (i * sizeof(std::uint16_t)), &pData, sizeof(std::uint16_t));

			if (!pData || (pData == 0xFFFF)) { break; }

			Message_sub.push_back(std::make_unique<Resident_Evil_2_Message>());

			Message_sub[i]->Open(File, static_cast<uintmax_t>(Header.pMessage_sub + pData));
		}

	}

	// Camera Scroll Texture
	if (Header.pScrl)
	{
		Scrl->Create(16, 320, 240, 0);
		Scrl->ImportPixels(File, Header.pScrl, 0x25800);
	}

	// Object Models
	if (Header.pOmodel)
	{
		struct Model_Link
		{
			std::uint32_t pTexture;
			std::uint32_t pModel;
		};

		std::vector<Model_Link> Link(Header.nOmodel);
		File.Read(Header.pOmodel, Link.data(), Link.size() * sizeof(Model_Link));

		for (std::size_t i = 0; i < Link.size(); i++)
		{
			Omodel.push_back(std::make_pair(std::make_shared<Sony_PlayStation_Texture>(), std::make_shared<Resident_Evil_2_MD1>()));

			if (Link[i].pTexture && (Link[i].pTexture != 0xFFFFFFFF))
			{
				if (i)
				{
					if (Link[i].pTexture == Link[i - 1].pTexture) { Omodel.back().first = Omodel.at(i - 1).first; }
				}
				else
				{
					Omodel.back().first->Open(File, Link[i].pTexture);
				}
			}
			if (Link[i].pModel && (Link[i].pModel != 0xFFFFFFFF))
			{
				if (i)
				{
					if (Link[i].pModel == Link[i - 1].pModel) { Omodel.back().second = Omodel.at(i - 1).second; }
				}
				else
				{
					Omodel.back().second->Open(File, Link[i].pModel);
				}
			}
		}
	}

	// Sony PlayStation Soundbank
	if (std::to_underlying(Game) & BIO2TRIAL)
	{
		Resident_Evil_2_RDT_Header_Trial_Ver Header_Trial{};
		File.Read(0, &Header_Trial, sizeof(Resident_Evil_2_RDT_Header_Trial_Ver));
		if (Header_Trial.pEdt0) { Edt0->Open(File, 48, Header_Trial.pEdt0); }
		if (Header_Trial.pEdt1) { Edt1->Open(File, 32, Header_Trial.pEdt1); }
		if (Header_Trial.pVh0) { Vab0->OpenVH(File, Header_Trial.pVh0); }
		if (Header_Trial.pVh1) { Vab1->OpenVH(File, Header_Trial.pVh1); }
		if (Header_Trial.pVb)
		{
			Vab0->OpenVB(File, Header_Trial.pVb);
			Vab1->OpenVB(File, Header_Trial.pVb);
		}
	}
	else
	{
		if (Header.pEdt) { Edt0->Open(File, 48, Header.pEdt); }
		if (Header.pVh) { Vab0->OpenVH(File, Header.pVh); }
		if (Header.pVb) { Vab0->OpenVB(File, Header.pVb); }
	}

	// Flag
	return b_Open = true;
}


/*
	Close
*/
void Resident_Evil_2_RDT::Close(void)
{
	// Flag
	b_Open = false;

	// Meta
	Stage = 0;
	Room = 0;

	// Header
	std::memset(&Header, 0, sizeof(Resident_Evil_2_RDT_Header));

	// RID
	Rid->Close();

	// PRI
	for (auto& Element : Pri)
	{
		Element->Close();
		Element.reset();
	}
	Pri.clear();

	// RVD
	Rvd->Close();

	// LIT
	Lit->Close();

	// SCA
	Sca->Close();

	// BLK
	Blk->Close();

	// FLR
	Flr->Close();

	// SCD
	for (auto& Element : ScdX)
	{
		Element->Close();
		Element.reset();
	}
	ScdX.clear();

	// SCD
	for (auto& Element : Scd)
	{
		Element->Close();
		Element.reset();
	}
	Scd.clear();

	// MSG
	for (auto& Element : Message)
	{
		Element->Close();
		Element.reset();
	}
	Message.clear();

	// MSG
	for (auto& Element : Message_sub)
	{
		Element->Close();
		Element.reset();
	}
	Message_sub.clear();

	// Camera Scroll Texture
	Scrl->Close();

	// Object Models
	for (auto& Element : Omodel)
	{
		Element.first->Close();
		Element.second->Close();
		Element.first.reset();
		Element.second.reset();
	}
	Omodel.clear();

	// Soundbank
	Edt0->Close();
	Edt1->Close();
	Vab0->CloseVAB();
	Vab1->CloseVAB();

	// Model Animation
	Rbj->CloseEDD();
	Rbj->CloseEMR();
}


/*
	Set camera count
*/
void Resident_Evil_2_RDT::SetCameraCount(std::uint8_t Count) noexcept
{
	if (Count > BIO2_CAMERA_MAX) { Count = BIO2_CAMERA_MAX; }
	Header.nCut = Count;
}


/*
	Set object model count
*/
void Resident_Evil_2_RDT::SetObjectModelCount(std::uint8_t Count) noexcept
{
	if (Count > BIO2_OMODEL_MAX) { Count = BIO2_OMODEL_MAX; }
	Header.nOmodel = Count;
	Omodel.resize(Count);
}