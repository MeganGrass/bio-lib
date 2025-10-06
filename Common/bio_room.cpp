/*
*
*	Megan Grass
*	September 4, 2025
*
*/


#include "bio_room.h"

#include <clut/bio1_clut.h>

const bool Resident_Evil_Room::ReadHeader(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read header in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (GameType() & AUG95)
	{
		Resident_Evil_RDT_Header_Aug95 Header{};
		File.Read(0, &Header, sizeof(Resident_Evil_RDT_Header_Aug95));

		m_Header.nSprite = Header.nSprite;
		m_Header.nCut = Header.nCut;
		m_Header.nOmodel = Header.nOmodel;
		m_Header.nItem = Header.nItem;
		m_Header.nDoor = Header.nDoor;
		m_Header.nRoom_at = Header.nRoom_at;
		m_Header.Reverb_lv = 0;
		m_Header.pEdt0 = Header.pEdt;
		m_Header.pVh0 = Header.pVh;
		m_Header.pVb0 = Header.pVb;
		m_Header.pEdt1 = 0;
		m_Header.pVh1 = 0;
		m_Header.pVb1 = 0;
		m_Header.pRbj_end = 0;
		m_Header.pSca = Header.pSca;
		m_Header.pRcut = sizeof(Resident_Evil_RDT_Header_Aug95);
		m_Header.pVcut = Header.pVcut;
		m_Header.pLight = 0x06;
		m_Header.pOmodel = Header.pOmodel;
		m_Header.pImodel = Header.pImodel;
		m_Header.pFloor = Header.pFloor;
		m_Header.pBlock = Header.pBlock;
		m_Header.pMessage = Header.pMessage;
		m_Header.pMessage_sub = 0;
		m_Header.pItemPix = 0;
		m_Header.pScrl = 0;
		m_Header.pOta = 0;
		m_Header.pScdX0 = Header.pScdX0;
		m_Header.pScdX1 = Header.pScdX1;
		m_Header.pScd0 = Header.pScd0;
		m_Header.pScd1 = Header.pScd1;
		m_Header.pScdEv0 = 0;
		m_Header.pScdEv1 = 0;
		m_Header.pEsp_hed = Header.pEsp_hed;
		m_Header.pEsp_end = Header.pEsp_end;
		m_Header.pEsp_tim = 0;
		m_Header.pEsp_tim_end = Header.pEsp_tim_end;
		m_Header.pEmr = 0;
		m_Header.pEdd = 0;
		m_Header.pRbj = 0;

		return true;
	}

	else if (GameType() & OCT95)
	{
		Resident_Evil_RDT_Header_Oct95 Header{};
		File.Read(0, &Header, sizeof(Resident_Evil_RDT_Header_Oct95));

		m_Header.nSprite = Header.nSprite;
		m_Header.nCut = Header.nCut;
		m_Header.nOmodel = Header.nOmodel;
		m_Header.nItem = Header.nItem;
		m_Header.nDoor = Header.nDoor;
		m_Header.nRoom_at = Header.nRoom_at;
		m_Header.Reverb_lv = 0;
		m_Header.pEdt0 = Header.pEdt;
		m_Header.pVh0 = Header.pVh;
		m_Header.pVb0 = Header.pVb;
		m_Header.pEdt1 = 0;
		m_Header.pVh1 = 0;
		m_Header.pVb1 = 0;
		m_Header.pRbj_end = 0;
		m_Header.pSca = Header.pSca;
		m_Header.pRcut = sizeof(Resident_Evil_RDT_Header_Oct95);
		m_Header.pVcut = Header.pVcut;
		m_Header.pLight = 0x06;
		m_Header.pOmodel = Header.pOmodel;
		m_Header.pImodel = Header.pImodel;
		m_Header.pFloor = Header.pFloor;
		m_Header.pBlock = Header.pBlock;
		m_Header.pMessage = Header.pMessage;
		m_Header.pMessage_sub = 0;
		m_Header.pItemPix = Header.pItemPix;
		m_Header.pScrl = 0;
		m_Header.pOta = 0;
		m_Header.pScdX0 = Header.pScdX0;
		m_Header.pScdX1 = Header.pScdX1;
		m_Header.pScd0 = Header.pScd0;
		m_Header.pScd1 = Header.pScd1;
		m_Header.pScdEv0 = Header.pScdEv0;
		m_Header.pScdEv1 = Header.pScdEv1;
		m_Header.pEsp_hed = Header.pEsp_hed;
		m_Header.pEsp_end = Header.pEsp_end;
		m_Header.pEsp_tim = 0;
		m_Header.pEsp_tim_end = Header.pEsp_tim_end;
		m_Header.pEmr = 0;
		m_Header.pEdd = 0;
		m_Header.pRbj = 0;

		return true;
	}

	else if (GameType() & BIO1)
	{
		Resident_Evil_RDT_Header Header{};
		File.Read(0, &Header, sizeof(Resident_Evil_RDT_Header));

		m_Header.nSprite = Header.nSprite;
		m_Header.nCut = Header.nCut;
		m_Header.nOmodel = Header.nOmodel;
		m_Header.nItem = Header.nItem;
		m_Header.nDoor = Header.nDoor;
		m_Header.nRoom_at = Header.nRoom_at;
		m_Header.Reverb_lv = 0;
		m_Header.pEdt0 = Header.pEdt;
		m_Header.pVh0 = Header.pVh;
		m_Header.pVb0 = Header.pVb;
		m_Header.pEdt1 = 0;
		m_Header.pVh1 = 0;
		m_Header.pVb1 = 0;
		m_Header.pRbj_end = 0;
		m_Header.pSca = Header.pSca;
		m_Header.pRcut = sizeof(Resident_Evil_RDT_Header);
		m_Header.pVcut = Header.pVcut;
		m_Header.pLight = 0x06;
		m_Header.pOmodel = Header.pOmodel;
		m_Header.pImodel = Header.pImodel;
		m_Header.pFloor = Header.pFloor;
		m_Header.pBlock = Header.pBlock;
		m_Header.pMessage = Header.pMessage;
		m_Header.pMessage_sub = 0;
		m_Header.pItemPix = Header.pItemPix;
		m_Header.pScrl = 0;
		m_Header.pOta = 0;
		m_Header.pScdX0 = Header.pScdX;
		m_Header.pScdX1 = 0;
		m_Header.pScd0 = Header.pScd;
		m_Header.pScd1 = 0;
		m_Header.pScdEv0 = Header.pScdEv;
		m_Header.pScdEv1 = 0;
		m_Header.pEsp_hed = Header.pEsp_hed;
		m_Header.pEsp_end = Header.pEsp_end;
		m_Header.pEsp_tim = 0;
		m_Header.pEsp_tim_end = Header.pEsp_tim_end;
		m_Header.pEmr = Header.pEmr;
		m_Header.pEdd = Header.pEdd;
		m_Header.pRbj = 0;

		return true;
	}

	else if (GameType() & BIO2NOV96)
	{
		Resident_Evil_2_RDT_Nov96 Header{};
		File.Read(0, &Header, sizeof(Resident_Evil_2_RDT_Nov96));

		m_Header.nSprite = Header.nSprite;
		m_Header.nCut = Header.nCut;
		m_Header.nOmodel = Header.nOmodel;
		m_Header.nItem = Header.nItem;
		m_Header.nDoor = Header.nDoor;
		m_Header.nRoom_at = Header.nRoom_at;
		m_Header.Reverb_lv = Header.Reverb_lv;
		m_Header.pEdt0 = Header.pEdt0;
		m_Header.pVh0 = Header.pVh0;
		m_Header.pVb0 = Header.pVb0;
		m_Header.pEdt1 = Header.pEdt1;
		m_Header.pVh1 = Header.pVh1;
		m_Header.pVb1 = Header.pVb1;
		m_Header.pRbj_end = 0;
		m_Header.pSca = Header.pSca;
		m_Header.pRcut = Header.pRcut;
		m_Header.pVcut = Header.pVcut;
		m_Header.pLight = Header.pLight;
		m_Header.pOmodel = Header.pOmodel;
		m_Header.pImodel = 0;
		m_Header.pFloor = Header.pFloor;
		m_Header.pBlock = Header.pBlock;
		m_Header.pMessage = Header.pMessage;
		m_Header.pMessage_sub = 0;
		m_Header.pItemPix = 0;
		m_Header.pScrl = 0;
		m_Header.pOta = 0;
		m_Header.pScdX0 = Header.pScdx;
		m_Header.pScdX1 = 0;
		m_Header.pScd0 = Header.pScd0;
		m_Header.pScd1 = Header.pScd1;
		m_Header.pScdEv0 = 0;
		m_Header.pScdEv1 = 0;
		m_Header.pEsp_hed = Header.pEsp_hed;
		m_Header.pEsp_end = Header.pEsp_end;
		m_Header.pEsp_tim = Header.pEsp_tim;
		m_Header.pEsp_tim_end = Header.pEsp_tim_end;
		m_Header.pEmr = 0;
		m_Header.pEdd = 0;
		m_Header.pRbj = Header.pRbj;

		return true;
	}

	else if (GameType() & BIO2TRIAL)
	{
		Resident_Evil_2_RDT_Header_Trial_Ver Header{};
		File.Read(0, &Header, sizeof(Resident_Evil_2_RDT_Header_Trial_Ver));

		m_Header.nSprite = Header.nSprite;
		m_Header.nCut = Header.nCut;
		m_Header.nOmodel = Header.nOmodel;
		m_Header.nItem = Header.nItem;
		m_Header.nDoor = Header.nDoor;
		m_Header.nRoom_at = Header.nRoom_at;
		m_Header.Reverb_lv = Header.Reverb_lv;
		m_Header.pEdt0 = Header.pEdt0;
		m_Header.pVh0 = Header.pVh0;
		m_Header.pVb0 = Header.pVb;
		m_Header.pEdt1 = Header.pEdt1;
		m_Header.pVh1 = Header.pVh1;
		m_Header.pVb1 = 0;
		m_Header.pRbj_end = Header.pRbj_end;
		m_Header.pSca = Header.pSca;
		m_Header.pRcut = Header.pRcut;
		m_Header.pVcut = Header.pVcut;
		m_Header.pLight = Header.pLight;
		m_Header.pOmodel = Header.pOmodel;
		m_Header.pImodel = 0;
		m_Header.pFloor = Header.pFloor;
		m_Header.pBlock = Header.pBlock;
		m_Header.pMessage = Header.pMessage;
		m_Header.pMessage_sub = Header.pMessage_sub;
		m_Header.pItemPix = 0;
		m_Header.pScrl = Header.pScrl;
		m_Header.pOta = 0;
		m_Header.pScdX0 = Header.pScdx;
		m_Header.pScdX1 = 0;
		m_Header.pScd0 = Header.pScd;
		m_Header.pScd1 = 0;
		m_Header.pScdEv0 = 0;
		m_Header.pScdEv1 = 0;
		m_Header.pEsp_hed = Header.pEsp_hed;
		m_Header.pEsp_end = Header.pEsp_end;
		m_Header.pEsp_tim = Header.pEsp_tim;
		m_Header.pEsp_tim_end = Header.pEsp_tim_end;
		m_Header.pEmr = 0;
		m_Header.pEdd = 0;
		m_Header.pRbj = Header.pRbj;

		return true;
	}

	else if (GameType() & BIO2)
	{
		Resident_Evil_2_RDT_Header Header{};
		File.Read(0, &Header, sizeof(Resident_Evil_2_RDT_Header));

		m_Header.nSprite = Header.nSprite;
		m_Header.nCut = Header.nCut;
		m_Header.nOmodel = Header.nOmodel;
		m_Header.nItem = Header.nItem;
		m_Header.nDoor = Header.nDoor;
		m_Header.nRoom_at = Header.nRoom_at;
		m_Header.Reverb_lv = Header.Reverb_lv;
		m_Header.pEdt0 = Header.pEdt;
		m_Header.pVh0 = Header.pVh;
		m_Header.pVb0 = Header.pVb;
		m_Header.pEdt1 = 0;
		m_Header.pVh1 = 0;
		m_Header.pVb1 = 0;
		m_Header.pRbj_end = Header.pRbj_end;
		m_Header.pSca = Header.pSca;
		m_Header.pRcut = Header.pRcut;
		m_Header.pVcut = Header.pVcut;
		m_Header.pLight = Header.pLight;
		m_Header.pOmodel = Header.pOmodel;
		m_Header.pImodel = 0;
		m_Header.pFloor = Header.pFloor;
		m_Header.pBlock = Header.pBlock;
		m_Header.pMessage = Header.pMessage;
		m_Header.pMessage_sub = Header.pMessage_sub;
		m_Header.pItemPix = 0;
		m_Header.pScrl = Header.pScrl;
		m_Header.pOta = 0;
		m_Header.pScdX0 = Header.pScdx;
		m_Header.pScdX1 = 0;
		m_Header.pScd0 = Header.pScd;
		m_Header.pScd1 = 0;
		m_Header.pScdEv0 = 0;
		m_Header.pScdEv1 = 0;
		m_Header.pEsp_hed = Header.pEsp_hed;
		m_Header.pEsp_end = Header.pEsp_end;
		m_Header.pEsp_tim = Header.pEsp_tim;
		m_Header.pEsp_tim_end = Header.pEsp_tim_end;
		m_Header.pEmr = 0;
		m_Header.pEdd = 0;
		m_Header.pRbj = Header.pRbj;

		return true;
	}

	else if (GameType() & BIO3)
	{
		Resident_Evil_3_RDT_Header Header{};
		File.Read(0, &Header, sizeof(Resident_Evil_3_RDT_Header));

		m_Header.nSprite = Header.nSprite;
		m_Header.nCut = Header.nCut;
		m_Header.nOmodel = Header.nOmodel;
		m_Header.nItem = Header.nItem;
		m_Header.nDoor = Header.nDoor;
		m_Header.nRoom_at = Header.nRoom_at;
		m_Header.Reverb_lv = Header.Reverb_lv;
		m_Header.pEdt0 = Header.pEdt;
		m_Header.pVh0 = Header.pVh;
		m_Header.pVb0 = Header.pVb;
		m_Header.pEdt1 = 0;
		m_Header.pVh1 = 0;
		m_Header.pVb1 = 0;
		m_Header.pRbj_end = 0;
		m_Header.pSca = Header.pSca;
		m_Header.pRcut = Header.pRcut;
		m_Header.pVcut = Header.pVcut;
		m_Header.pLight = Header.pLight;
		m_Header.pOmodel = Header.pOmodel;
		m_Header.pImodel = 0;
		m_Header.pFloor = Header.pFloor;
		m_Header.pBlock = Header.pBlock;
		m_Header.pMessage = Header.pMessage;
		m_Header.pMessage_sub = 0;
		m_Header.pItemPix = 0;
		m_Header.pScrl = 0;
		m_Header.pOta = Header.pOta;
		m_Header.pScdX0 = Header.pScd;
		m_Header.pScdX1 = 0;
		m_Header.pScd0 = 0;
		m_Header.pScd1 = 0;
		m_Header.pScdEv0 = 0;
		m_Header.pScdEv1 = 0;
		m_Header.pEsp_hed = Header.pEsp_hed;
		m_Header.pEsp_end = Header.pEsp_end;
		m_Header.pEsp_tim = Header.pEsp_tim;
		m_Header.pEsp_tim_end = Header.pEsp_tim_end;
		m_Header.pEmr = 0;
		m_Header.pEdd = 0;
		m_Header.pRbj = 0;

		return true;
	}

	return false;
}

const bool Resident_Evil_Room::ReadRID(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read RID in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (!IsValidPointer(File, Header().pRcut)) { return true; }

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		std::vector<Resident_Evil_RID_Data> Data(Header().nCut);

		File.Read(Header().pRcut, Data.data(), Data.size() * sizeof(Resident_Evil_RID_Data));

		PriTex.resize(Header().nCut);

		Resident_Evil_2_RID_Data Temp{};

		for (std::size_t i = 0; i < Header().nCut; i++)
		{
			if ((i + 1) >= Header().nCut) { Temp.end_flg = 1; }
			else { Temp.end_flg = 0; }

			Temp.ViewR = static_cast<std::uint16_t>((Data[i].ViewR << 7));

			Temp.View_p.x = Data[i].View_p.x;
			Temp.View_p.y = Data[i].View_p.y;
			Temp.View_p.z = Data[i].View_p.z;

			Temp.View_r.x = Data[i].View_r.x;
			Temp.View_r.y = Data[i].View_r.y;
			Temp.View_r.z = Data[i].View_r.z;

			Temp.pSp = Data[i].pSp;

			Rid->Add(Temp);

			PriTex[i] = std::make_shared<Sony_PlayStation_Texture>();

			PriTex[i]->Str.hWnd = Str.hWnd;

			if (IsValidPointer(File, Data[i].pTim))
			{
				PriTex[i]->OpenTIM(File, Data[i].pTim);
			}
		}
	}

	else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
	{
		Rid->Open(File, Header().nCut, Header().pRcut);
	}

	for (std::size_t i = 0; i < Header().nCut; i++)
	{
		Pri.push_back(std::make_shared<Resident_Evil_2_PRI>());

		Pri[i]->Str.hWnd = Str.hWnd;

		if (IsValidPointer(File, Rid->Get(i)->pSp))
		{
			Pri[i]->Open(File, Rid->Get(i)->pSp, (GameType() & (AUG95 | OCT95 | BIO1 | BIO2NOV96)), (GameType() & (AUG95 | OCT95 | BIO1)), (GameType() & BIO2NOV96));
		}
	}

	return true;
}

const bool Resident_Evil_Room::ReadRVD(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read RVD in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (!IsValidPointer(File, Header().pVcut)) { return true; }

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		std::vector<Resident_Evil_RVD_Data> Data;

		Resident_Evil_RVD_Data Vcut{};

		std::uintmax_t pData = 0;

		std::uintmax_t _Ptr = Header().pVcut;

		do {

			pData = (_Ptr + (Data.size() * sizeof(Resident_Evil_RVD_Data)));

			File.Read(pData, &Vcut, sizeof(Resident_Evil_RVD_Data));

			if ((Vcut.Fcut != 0xFF) && (Vcut.Tcut != 0xFF) && (Vcut.Fcut != 0xFFFF) && (Vcut.Tcut != 0xFFFF))
			{
				Data.push_back(Vcut);
			}
			else
			{
				break;
			}

		} while ((Vcut.Fcut != 0xFF) && (Vcut.Tcut != 0xFF) && (Vcut.Fcut != 0xFFFF) && (Vcut.Tcut != 0xFFFF));

		Resident_Evil_2_RVD_Data Temp{};

		for (std::size_t i = 0; i < Data.size(); i++)
		{
			Temp.Be_flg = 1;
			Temp.nFloor = -1;
			Temp.Fcut = static_cast<std::uint8_t>(Data[i].Fcut);
			if (Data[i].Tcut == 9)
			{
				Temp.Tcut = 0;
			}
			else
			{
				Temp.Tcut = static_cast<std::uint8_t>(Data[i].Tcut);
			}
			Temp.Xz[0][0] = (Data[i].Xz[0][0] & 0x7FFF);
			Temp.Xz[0][1] = (Data[i].Xz[0][1] & 0x7FFF);
			Temp.Xz[1][0] = (Data[i].Xz[1][0] & 0x7FFF);
			Temp.Xz[1][1] = (Data[i].Xz[1][1] & 0x7FFF);
			Temp.Xz[2][0] = (Data[i].Xz[2][0] & 0x7FFF);
			Temp.Xz[2][1] = (Data[i].Xz[2][1] & 0x7FFF);
			Temp.Xz[3][0] = (Data[i].Xz[3][0] & 0x7FFF);
			Temp.Xz[3][1] = (Data[i].Xz[3][1] & 0x7FFF);
			Rvd->Add(Temp);
		}
	}

	else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
	{
		Rvd->Open(File, Header().pVcut);
	}

	Rvd->Sort();

	return true;
}

const bool Resident_Evil_Room::ReadLIT(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read LIT in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (!IsValidPointer(File, Header().pLight)) { return true; }

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		Resident_Evil_LIT_Data Data{};

		Resident_Evil_LIT_Data_Aug95 DataAug95{};

		File.Read(Header().pLight, &Data, sizeof(Resident_Evil_LIT_Data));

		if (GameType() & AUG95)
		{
			Resident_Evil_LIT_Data_Aug95 DataAug95{};
			File.Read(Header().pLight, &DataAug95, sizeof(Resident_Evil_LIT_Data_Aug95));
			std::memcpy(&Data.Ambient, &DataAug95.Ambient, sizeof(Data.Ambient));
			std::memcpy(&Data.Data, &DataAug95.Data, sizeof(Data.Data));
		}

		Resident_Evil_2_LIT_Data Temp{};

		for (std::size_t i = 0; i < Header().nCut; i++)
		{
			Temp.Mag = 1;
			Temp.Mode[0] = Data.Data[0].Mode[0];
			Temp.Mode[1] = Data.Data[0].Mode[1];
			Temp.Mode[2] = Data.Data[0].Mode[2];
			std::memcpy(&Temp.Col[0], &Data.Data[0].Color, sizeof(CVECTOR2));
			std::memcpy(&Temp.Col[1], &Data.Data[1].Color, sizeof(CVECTOR2));
			std::memcpy(&Temp.Col[2], &Data.Data[2].Color, sizeof(CVECTOR2));
			//Temp.Ambient.r = static_cast<std::uint8_t>((Data.Ambient[0] / 0x3FF) * 0x1F);
			//Temp.Ambient.g = static_cast<std::uint8_t>((Data.Ambient[1] / 0x3FF) * 0x1F);
			//Temp.Ambient.b = static_cast<std::uint8_t>((Data.Ambient[2] / 0x3FF) * 0x1F);
			Temp.Ambient.r = (Data.Ambient[0] >> 4);
			Temp.Ambient.g = (Data.Ambient[1] >> 4);
			Temp.Ambient.b = (Data.Ambient[2] >> 4);
			Temp.Pos[0].x = Data.Data[0].Pos.x;
			Temp.Pos[0].y = Data.Data[0].Pos.y;
			Temp.Pos[0].z = Data.Data[0].Pos.z;
			Temp.Pos[1].x = Data.Data[1].Pos.x;
			Temp.Pos[1].y = Data.Data[1].Pos.y;
			Temp.Pos[1].z = Data.Data[1].Pos.z;
			Temp.Pos[2].x = Data.Data[2].Pos.x;
			Temp.Pos[2].y = Data.Data[2].Pos.y;
			Temp.Pos[2].z = Data.Data[2].Pos.z;
			Temp.L[0] = (Data.Data[0].L >> 1);
			Temp.L[1] = (Data.Data[1].L >> 1);
			Temp.L[2] = (Data.Data[2].L >> 1);

			Lit->Add(Temp);
		}
	}

	else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
	{
		Lit->Open(File, Header().nCut, Header().pLight);
	}

	return true;
}

const bool Resident_Evil_Room::ReadSCA(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read SCA in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (!IsValidPointer(File, Header().pSca)) { return true; }

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		auto GetHeight = [&](const std::vector<Resident_Evil_SCA_Data>& Data) -> std::int16_t
			{
				std::int16_t High = 0;

				for (const auto& Entry : Data)
				{
					if (Entry.Type.Bits.High > High)
					{
						High = Entry.Type.Bits.High;
					}
				}

				return High;
			};

		auto Add = [&](const std::vector<Resident_Evil_SCA_Data>& Data, std::shared_ptr<Resident_Evil_2_SCA>& Out, std::uint32_t Id) -> void
			{
				Resident_Evil_2_SCA_Data Temp{};

				for (size_t i = 0; i < Data.size(); i++)
				{
					switch (Data[i].Id)
					{
					case 0: continue;
					case 1: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Box); break;
					case 2: continue;
					case 3: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Circle); break;
					case 4: continue;
					case 5: continue;
					}

					Temp.X = min(Data[i].Xz[1][0], Data[i].Xz[0][0]);
					Temp.Z = min(Data[i].Xz[1][1], Data[i].Xz[0][1]);
					Temp.W = std::abs(Data[i].Xz[1][0] - Data[i].Xz[0][0]);
					Temp.D = std::abs(Data[i].Xz[1][1] - Data[i].Xz[0][1]);

					Temp.Id.Bits.bit0 = 8;
					Temp.Id.Bits.bit1 = 0;
					Temp.Id.Bits.bit2 = 0;
					Temp.Id.Bits.bit3 = 1;
					Temp.Id.Bits.bit4 = 0;
					Temp.Id.Bits.bit5 = 0;
					Temp.Id.Bits.bit6 = 1;
					Temp.Id.Bits.bit7 = 1;
					Temp.Id.Bits.bit8 = 1;

					Temp.Type.Bits.Hypotenuse = 0;
					Temp.Type.Bits.Axis = 0;

					Temp.Type.Bits.nFloor = Data[i].Type.Bits.High;

					if (Temp.Type.Bits.nFloor)
					{
						Temp.Type.Bits.nFloor--;
					}

					Temp.Type.Bits.Height = 18;

					Out->SetArea(Temp);

					Out->SetFloor(Temp, 0);

					Out->Add(Temp);
				}
			};

		Resident_Evil_SCA_Header ScaHeader{};
		File.Read(Header().pSca, &ScaHeader, sizeof(Resident_Evil_SCA_Header));

		std::uintmax_t pData = Header().pSca + sizeof(Resident_Evil_SCA_Header);

		std::vector<Resident_Evil_SCA_Data> Data0(ScaHeader.nData0);
		File.Read(pData, Data0.data(), Data0.size() * sizeof(Resident_Evil_SCA_Data));
		pData += (ScaHeader.nData0 * sizeof(Resident_Evil_SCA_Data));

		std::vector<Resident_Evil_SCA_Data> Data1(ScaHeader.nData1);
		File.Read(pData, Data1.data(), Data1.size() * sizeof(Resident_Evil_SCA_Data));
		pData += (ScaHeader.nData1 * sizeof(Resident_Evil_SCA_Data));

		std::vector<Resident_Evil_SCA_Data> Data2(ScaHeader.nData2);
		File.Read(pData, Data2.data(), Data2.size() * sizeof(Resident_Evil_SCA_Data));
		pData += (ScaHeader.nData2 * sizeof(Resident_Evil_SCA_Data));

		std::vector<Resident_Evil_SCA_Data> Data3(ScaHeader.nData3);
		File.Read(pData, Data3.data(), Data3.size() * sizeof(Resident_Evil_SCA_Data));
		pData += (ScaHeader.nData3 * sizeof(Resident_Evil_SCA_Data));

		std::vector<Resident_Evil_SCA_Data> Data4(ScaHeader.nData4);
		File.Read(pData, Data4.data(), Data4.size() * sizeof(Resident_Evil_SCA_Data));
		pData += (ScaHeader.nData4 * sizeof(Resident_Evil_SCA_Data));

		std::int16_t Height = 0;

		if (GetHeight(Data0) > Height) { Height = GetHeight(Data0); }
		if (GetHeight(Data1) > Height) { Height = GetHeight(Data1); }
		if (GetHeight(Data2) > Height) { Height = GetHeight(Data2); }
		if (GetHeight(Data3) > Height) { Height = GetHeight(Data3); }
		if (GetHeight(Data4) > Height) { Height = GetHeight(Data4); }

		Sca->GetHeader()->Cx = (ScaHeader.Cx & 0x7FFF);
		Sca->GetHeader()->Cz = (ScaHeader.Cz & 0x7FFF);
		Sca->GetHeader()->Ceiling = (Height * -1800);
		Sca->GetHeader()->Color = 0xC5C5C5C5;

		Add(Data0, Sca, 0);
		Add(Data1, Sca, 1);
		Add(Data2, Sca, 2);
		Add(Data3, Sca, 3);
		Add(Data4, Sca, 4);

		Sca->GetHeader()->nData = static_cast<std::uint32_t>(Sca->Count()) + 1;

		Sca->RemoveDuplicates();

		Sca->CalcCxCz();
	}

	else if (GameType() & BIO2NOV96)
	{
		auto GetHeight = [&](const std::vector<Resident_Evil_2_Nov96_SCA_Data>& Data) -> std::int16_t
			{
				std::int16_t High = 0;

				for (const auto& Entry : Data)
				{
					if (Entry.Type.Bits.High > High)
					{
						High = Entry.Type.Bits.Low + Entry.Type.Bits.High;
					}
				}

				return High;
			};

		auto Add = [&](const std::vector<Resident_Evil_2_Nov96_SCA_Data>& Data, std::shared_ptr<Resident_Evil_2_SCA>& Out, std::uint32_t Id) -> void
			{
				Resident_Evil_2_SCA_Data Temp{};

				for (size_t i = 0; i < Data.size(); i++)
				{
					switch (Data[i].Id.Bits.Shape)
					{
					case 0: continue;
					case 1: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Box); break;
					case 2: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Hishi); break;
					case 3: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Circle); break;
					case 4: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Naname_a); break;
					case 5: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Naname_b); break;
					case 6: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Naname_c); break;
					case 7: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Naname_d); break;
					case 8: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Koban_x); break;
					case 9: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Koban_z); break;
					}

					Temp.X = Data[i].X;
					Temp.Z = Data[i].Z;
					Temp.W = Data[i].W;
					Temp.D = Data[i].D;

					Temp.Id.Bits.bit0 = 8;
					Temp.Id.Bits.bit1 = 0;
					Temp.Id.Bits.bit2 = 0;
					Temp.Id.Bits.bit3 = 1;
					Temp.Id.Bits.bit4 = 0;
					Temp.Id.Bits.bit5 = 0;
					Temp.Id.Bits.bit6 = 1;
					Temp.Id.Bits.bit7 = 1;
					Temp.Id.Bits.bit8 = 1;

					Temp.Type.Bits.Hypotenuse = 0;
					Temp.Type.Bits.Axis = 0;

					Temp.Type.Bits.nFloor = Data[i].Type.Bits.Low + Data[i].Type.Bits.High;

					if (Temp.Type.Bits.nFloor)
					{
						Temp.Type.Bits.nFloor--;
					}

					Temp.Type.Bits.Height = 18;

					Out->SetArea(Temp);

					Out->SetFloor(Temp, Data[i].Type.Bits.Low);

					Out->Add(Temp);
				}
			};

		Resident_Evil_2_Nov96_SCA_Header ScaHeader{};
		File.Read(Header().pSca, &ScaHeader, sizeof(Resident_Evil_2_Nov96_SCA_Header));

		std::uintmax_t pData = Header().pSca + sizeof(Resident_Evil_2_Nov96_SCA_Header);

		std::vector<Resident_Evil_2_Nov96_SCA_Data> Data0(ScaHeader.nData0);
		File.Read(pData, Data0.data(), Data0.size() * sizeof(Resident_Evil_2_Nov96_SCA_Data));
		pData += (ScaHeader.nData0 * sizeof(Resident_Evil_2_Nov96_SCA_Data));

		std::vector<Resident_Evil_2_Nov96_SCA_Data> Data1(ScaHeader.nData1);
		File.Read(pData, Data1.data(), Data1.size() * sizeof(Resident_Evil_2_Nov96_SCA_Data));
		pData += (ScaHeader.nData1 * sizeof(Resident_Evil_2_Nov96_SCA_Data));

		std::vector<Resident_Evil_2_Nov96_SCA_Data> Data2(ScaHeader.nData2);
		File.Read(pData, Data2.data(), Data2.size() * sizeof(Resident_Evil_2_Nov96_SCA_Data));
		pData += (ScaHeader.nData2 * sizeof(Resident_Evil_2_Nov96_SCA_Data));

		std::vector<Resident_Evil_2_Nov96_SCA_Data> Data3(ScaHeader.nData3);
		File.Read(pData, Data3.data(), Data3.size() * sizeof(Resident_Evil_2_Nov96_SCA_Data));
		pData += (ScaHeader.nData3 * sizeof(Resident_Evil_2_Nov96_SCA_Data));

		std::vector<Resident_Evil_2_Nov96_SCA_Data> Data4(ScaHeader.nData4);
		File.Read(pData, Data4.data(), Data4.size() * sizeof(Resident_Evil_2_Nov96_SCA_Data));
		pData += (ScaHeader.nData4 * sizeof(Resident_Evil_2_Nov96_SCA_Data));

		std::int16_t Height = 0;

		if (GetHeight(Data0) > Height) { Height = GetHeight(Data0); }
		if (GetHeight(Data1) > Height) { Height = GetHeight(Data1); }
		if (GetHeight(Data2) > Height) { Height = GetHeight(Data2); }
		if (GetHeight(Data3) > Height) { Height = GetHeight(Data3); }
		if (GetHeight(Data4) > Height) { Height = GetHeight(Data4); }

		Sca->GetHeader()->Cx = (ScaHeader.Cx & 0x7FFF);
		Sca->GetHeader()->Cz = (ScaHeader.Cz & 0x7FFF);
		Sca->GetHeader()->Ceiling = (Height * -1800);
		Sca->GetHeader()->Color = 0xC5C5C5C5;

		Add(Data0, Sca, 0);
		Add(Data1, Sca, 1);
		Add(Data2, Sca, 2);
		Add(Data3, Sca, 3);
		Add(Data4, Sca, 4);

		Sca->GetHeader()->nData = static_cast<std::uint32_t>(Sca->Count()) + 1;

		Sca->RemoveDuplicates();

		Sca->CalcCxCz();
	}

	else if (GameType() & (BIO2TRIAL | BIO2))
	{
		Sca->Open(File, Header().pSca);
	}

	else if (GameType() & (BIO3))
	{
		Resident_Evil_3_SCA_Header ScaHeader{};
		File.Read(Header().pSca, &ScaHeader, sizeof(Resident_Evil_3_SCA_Header));

		std::vector<Resident_Evil_3_SCA_Data> Data(ScaHeader.nData - 1);

		File.Read(Header().pSca + sizeof(Resident_Evil_3_SCA_Header), Data.data(), Data.size() * sizeof(Resident_Evil_3_SCA_Data));

		Sca->GetHeader()->Cx = ScaHeader.Cx;
		Sca->GetHeader()->Cz = ScaHeader.Cz;
		Sca->GetHeader()->Ceiling = 0;	// TODO
		Sca->GetHeader()->Color = 0xC5C5C5C5;

		Resident_Evil_2_SCA_Data Temp{};

		for (std::size_t i = 0; i < Data.size(); i++)
		{
			Temp.X = Data[i].Xz[0][0];
			Temp.Z = Data[i].Xz[0][1];
			Temp.W = Data[i].Xz[1][0] - Data[i].Xz[0][0];
			Temp.D = Data[i].Xz[1][1] - Data[i].Xz[0][1];

			switch (Data[i].Id.Bits.Shape)
			{
			case 0: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Circle); break;
			case 1: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Box); break;
			case 2: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Koban_x); break;
			case 3: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Koban_z); break;
			case 4: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Naname_c); break;
			case 5: std::cout << "Unknown Shape: " << (int)Data[i].Id.Bits.Shape << std::endl; continue;
			case 6: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Naname_d); break;
			case 7: std::cout << "Unknown Shape: " << (int)Data[i].Id.Bits.Shape << std::endl; continue;
			case 8: std::cout << "Unknown Shape: " << (int)Data[i].Id.Bits.Shape << std::endl; continue;
			case 9: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Slope); break;
			case 10: Temp.Id.Bits.Shape = std::to_underlying(Resident_Evil_2_Collision_Shape::Box_3); break;
			case 11: std::cout << "Unknown Shape: " << (int)Data[i].Id.Bits.Shape << std::endl; continue;
			case 12: std::cout << "Unknown Shape: " << (int)Data[i].Id.Bits.Shape << std::endl; continue;
			}
			std::cout << "Shape: " << (int)Data[i].Id.Bits.Shape << " New Shape: " << (int)Temp.Id.Bits.Shape << std::endl;

			Data[i].Type.Data = Data[i].Type.Data;

			Data[i].Floor = Data[i].Floor;

			Sca->Add(Temp);

			Sca->GetHeader()->nData = static_cast<std::uint32_t>(Sca->Count()) + 1;
		}
	}

	return true;
}

const bool Resident_Evil_Room::ReadBLK(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read BLK in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (!IsValidPointer(File, Header().pBlock)) { return true; }

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		std::uint16_t Count = 0;

		File.Read(Header().pBlock, &Count, sizeof(std::uint16_t));

		std::vector<Resident_Evil_2_BLK_Data> Data(Count);

		File.Read(Header().pBlock + sizeof(std::uint16_t), Data.data(), Data.size() * sizeof(Resident_Evil_2_BLK_Data));

		for (std::size_t i = 0; i < Data.size(); i++)
		{
			Blk->Add(Data[i]);
		}
	}

	else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
	{
		Blk->Open(File, Header().pBlock);
	}

	return true;
}

const bool Resident_Evil_Room::ReadFLR(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read FLR in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (!IsValidPointer(File, Header().pFloor)) { return true; }

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		std::uint16_t Count = 0;

		File.Read(Header().pFloor, &Count, sizeof(std::uint16_t));

		std::vector<Resident_Evil_FLR_Data> Data(Count);

		File.Read(Header().pFloor + sizeof(std::uint16_t), Data.data(), Data.size() * sizeof(Resident_Evil_FLR_Data));

		Resident_Evil_2_FLR_Data Temp{};

		for (std::size_t i = 0; i < Data.size(); i++)
		{
			Temp.X = Data[i].X;
			Temp.Z = Data[i].Z;
			Temp.W = Data[i].W;
			Temp.D = Data[i].D;
			Temp.Se_no = Data[i].Se_no;
			Temp.Floor_height = Data[i].unk;
			Flr->Add(Temp);
		}
	}

	else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
	{
		Flr->Open(File, Header().pFloor);
	}

	return true;
}

const bool Resident_Evil_Room::ReadOTA(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read OTA in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (!IsValidPointer(File, Header().pOta)) { return true; }

	if (GameType() & BIO3)
	{
	}

	return true;
}

const bool Resident_Evil_Room::ReadSCD(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read SCD in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	auto ReadBio1 = [&](const std::uintmax_t pIndex, std::vector<std::vector<std::uint8_t>>& Out) -> void
		{
			std::uint16_t Size = 0;

			File.Read(pIndex, &Size, sizeof(std::uint16_t));

			if (!Size || (Size == 0xFFFF)) { return; }

			Out.resize(1);

			Out[0].resize(Size);

			File.Read(pIndex + sizeof(std::uint16_t), Out[0].data(), Out[0].size());
		};

	auto Read = [&](const std::uintmax_t pIndex, const std::uintmax_t SizeOfPointer, Room_Pointer_Type Start, std::vector<std::vector<std::uint8_t>>& Out) -> void
		{
			// as requested, going back to the old method of calculating individual file size with pointers instead of reading opcodes... blah.
			// this method allows for loading of data with custom opcodes

			std::uintmax_t nData = 0;
			std::uintmax_t Pointer = 0;
			std::uintmax_t Size = 0;

			File.Read(pIndex, &nData, SizeOfPointer);
			if (!nData || (nData == 0xFFFF) || (nData == 0xFFFFFFFF)) { return; }

			Out.resize((nData / SizeOfPointer));

			std::vector<std::uintmax_t> Index(Out.size());

			if (SizeOfPointer == sizeof(std::uint16_t))
			{
				std::vector<std::uint16_t> pData(Out.size());

				File.Read(pIndex, pData.data(), pData.size() * sizeof(std::uint16_t));

				for (std::size_t i = 0; i < pData.size(); i++) { Index[i] = pData[i]; }
			}

			if (SizeOfPointer == sizeof(std::uint32_t))
			{
				std::vector<std::uint32_t> pData(Out.size());

				File.Read(pIndex, pData.data(), pData.size() * sizeof(std::uint32_t));

				for (std::size_t i = 0; i < pData.size(); i++) { Index[i] = pData[i]; }
			}

			for (std::size_t i = 0; i < Index.size(); i++)
			{
				if (!Index[i] || (Index[i] == 0xFFFF) || (Index[i] == 0xFFFFFFFF)) { break; }

				Pointer = pIndex + Index[i];

				if ((i + 1) >= Index.size() || !Index[i + 1] || (Index[i + 1] == 0xFFFF) || (Index[i + 1] == 0xFFFFFFFF))
				{
					if (GameType() & (AUG95 | OCT95 | BIO1))
					{
						Size = GetNextValidPointerBio1(File, Start) - Pointer;
					}
					else if (GameType() & BIO2NOV96)
					{
						Size = GetNextValidPointerBio2Nov96(File, Start) - Pointer;
					}
					else if (GameType() & (BIO2TRIAL | BIO2))
					{
						Size = GetNextValidPointerBio2(File, Start) - Pointer;
					}
					else if (GameType() & BIO3)
					{
						Size = GetNextValidPointerBio3(File, Start) - Pointer;
					}
				}
				else
				{
					Size = Index[i + 1] - Index[i];
				}

				Out[i].resize(Size);

				File.Read(Pointer, Out[i].data(), Size);
			}
		};

	if (IsValidPointer(File, Header().pScdX0))
	{
		if (GameType() & (AUG95 | OCT95 | BIO1))
		{
			ReadBio1(Header().pScdX0, ScdX0);
		}
		else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
		{
			Read(Header().pScdX0, sizeof(std::uint16_t), Room_Pointer_Type::SCDX0, ScdX0);
		}
	}

	if (IsValidPointer(File, Header().pScdX1))
	{
		if (GameType() & (AUG95 | OCT95 | BIO1))
		{
			ReadBio1(Header().pScdX1, ScdX1);
		}
		else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
		{
			Read(Header().pScdX1, sizeof(std::uint16_t), Room_Pointer_Type::SCDX1, ScdX1);
		}
	}

	if (IsValidPointer(File, Header().pScd0))
	{
		if (GameType() & (AUG95 | OCT95 | BIO1))
		{
			ReadBio1(Header().pScd0, Scd0);
		}
		else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
		{
			Read(Header().pScd0, sizeof(std::uint16_t), Room_Pointer_Type::SCD0, Scd0);
		}
	}

	if (IsValidPointer(File, Header().pScd1))
	{
		if (GameType() & (AUG95 | OCT95 | BIO1))
		{
			ReadBio1(Header().pScd1, Scd1);
		}
		else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
		{
			Read(Header().pScd1, sizeof(std::uint16_t), Room_Pointer_Type::SCD1, Scd1);
		}
	}

	if (IsValidPointer(File, Header().pScdEv0))
	{
		if (GameType() & (AUG95 | OCT95 | BIO1))
		{
			Read(Header().pScdEv0, sizeof(std::uint32_t), Room_Pointer_Type::SCDEV0, ScdEv0);
		}
		else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
		{
			Read(Header().pScdEv0, sizeof(std::uint16_t), Room_Pointer_Type::SCDEV0, ScdEv0);
		}
	}

	if (IsValidPointer(File, Header().pScdEv1))
	{
		if (GameType() & (AUG95 | OCT95 | BIO1))
		{
			Read(Header().pScdEv1, sizeof(std::uint32_t), Room_Pointer_Type::SCDEV1, ScdEv1);
		}
		else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
		{
			Read(Header().pScdEv1, sizeof(std::uint16_t), Room_Pointer_Type::SCDEV1, ScdEv1);
		}
	}

	if (GameType() & BIO2)
	{
		for (auto& SCD : ScdX0)
		{
			Script->Open(ScriptType::Init, SCD);
		}

		for (auto& SCD : Scd0)
		{
			Script->Open(ScriptType::Loop, SCD);
		}
	}

	Script->Scheduler();

	return true;
}

const bool Resident_Evil_Room::ReadMSG(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read MSG in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	auto Read = [&](const std::uintmax_t pIndex, Room_Pointer_Type Start, std::vector<std::vector<std::uint8_t>>& Out) -> void
		{
			// as requested, going back to the old method of calculating individual file size with pointers instead of reading opcodes... blah.
			// this method allows for loading of data with custom opcodes

			std::uintmax_t nData = 0;
			std::uintmax_t Pointer = 0;
			std::uintmax_t Size = 0;

			File.Read(pIndex, &nData, sizeof(std::uint16_t));
			if (!nData || (nData == 0xFFFF) || (nData == 0xFFFFFFFF)) { return; }

			Out.resize((nData / sizeof(std::uint16_t)));

			std::vector<std::uintmax_t> Index(Out.size());
			std::vector<std::uint16_t> pData(Out.size());

			File.Read(pIndex, pData.data(), pData.size() * sizeof(std::uint16_t));

			for (std::size_t i = 0; i < Index.size(); i++)
			{
				Index[i] = pData[i];

				if (!Index[i] || (Index[i] == 0xFFFF)) { break; }

				Pointer = pIndex + Index[i];

				if ((i + 1) >= Index.size() || !Index[i + 1] || (Index[i + 1] == 0xFFFF))
				{
					if (GameType() & (AUG95 | OCT95 | BIO1))
					{
						Size = GetNextValidPointerBio1(File, Start) - Pointer;
					}
					else if (GameType() & BIO2NOV96)
					{
						Size = GetNextValidPointerBio2Nov96(File, Start) - Pointer;
					}
					else if (GameType() & (BIO2TRIAL | BIO2))
					{
						Size = GetNextValidPointerBio2(File, Start) - Pointer;
					}
					else if (GameType() & BIO3)
					{
						Size = GetNextValidPointerBio3(File, Start) - Pointer;
					}
				}
				else
				{
					Size = Index[i + 1] - Index[i];
				}

				Out[i].resize(Size);

				File.Read(Pointer, Out[i].data(), Size);
			}
		};

	if (IsValidPointer(File, Header().pMessage))
	{
		Read(Header().pMessage, Room_Pointer_Type::MSG, Message);
	}

	if (IsValidPointer(File, Header().pMessage_sub))
	{
		Read(Header().pMessage_sub, Room_Pointer_Type::MSG_SUB, MessageSub);
	}

	return true;
}

const bool Resident_Evil_Room::ReadIPIX(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read item pix textures in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (!IsValidPointer(File, Header().pItemPix)) { return true; }

	if (GameType() & (OCT95 | BIO1))
	{
		std::vector<Sony_Pixel_16bpp> Palette(sizeof(bio1_status_clut02) / sizeof(Sony_Pixel_16bpp));

		std::memcpy(Palette.data(), bio1_status_clut02, sizeof(bio1_status_clut02));

		Ipix.resize(Header().nItem);

		for (std::size_t i = 0; i < Ipix.size(); i++)
		{
			Ipix[i] = std::make_shared<Sony_PlayStation_Texture>();

			Ipix[i]->Str.hWnd = Str.hWnd;

			Ipix[i]->Create(8, 40, 30, 1);

			Ipix[i]->PastePalette(Palette, 0);

			Ipix[i]->ReadPixels(File, Header().pScrl, 40, 30);
		}
	}

	return true;
}

const bool Resident_Evil_Room::ReadSCRL(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read camera scroll in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (!IsValidPointer(File, Header().pScrl)) { return true; }

	if (GameType() & BIO2)
	{
		Scrl->Str.hWnd = Str.hWnd;

		Scrl->Create(16, 320, 240, 0);

		Scrl->ReadPixels(File, Header().pScrl, 320, 240);
	}

	return true;
}

const bool Resident_Evil_Room::ReadModelItem(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read item model in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (!IsValidPointer(File, Header().pImodel)) { return true; }

	if ((GameType() & (AUG95 | OCT95 | BIO1)) == 0) { return true; }

	Item.resize(Header().nItem);

	struct MODEL_LINK
	{
		std::uint32_t pTexture;
		std::uint32_t pModel;
	};

	std::vector<MODEL_LINK> Link(Header().nItem);
	File.Read(Header().pImodel, Link.data(), Link.size() * sizeof(MODEL_LINK));

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		struct MODEL_LINK_BIO1
		{
			std::uint32_t pModel;
			std::uint32_t pTexture;
		};

		std::vector<MODEL_LINK_BIO1> LinkBio1(Header().nItem);
		File.Read(Header().pImodel, LinkBio1.data(), LinkBio1.size() * sizeof(MODEL_LINK_BIO1));

		for (std::size_t i = 0; i < Link.size(); i++)
		{
			Link[i].pModel = LinkBio1[i].pModel;
			Link[i].pTexture = LinkBio1[i].pTexture;
		}
	}

	for (std::size_t i = 0; i < Link.size(); i++)
	{
		Item[i] = std::make_shared<Resident_Evil_Model>();

		Item[i]->PlatformSetup(Str.hWnd, GTE, Render, b_HorzFlip, b_VertFlip);

		Item[i]->SetGame(m_Game);

		if (IsValidPointer(File, Link[i].pTexture))
		{
			Item[i]->OpenTexture(File, Link[i].pTexture);
		}

		if (IsValidPointer(File, Link[i].pModel))
		{
			Item[i]->Open(File, Link[i].pModel);
		}
	}

	return true;
}

const bool Resident_Evil_Room::ReadModelObject(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read object model in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (!IsValidPointer(File, Header().pOmodel)) { return true; }

	Object.resize(Header().nOmodel);

	struct MODEL_LINK
	{
		std::uint32_t pTexture;
		std::uint32_t pModel;
	};

	std::vector<MODEL_LINK> Link(Header().nOmodel);
	File.Read(Header().pOmodel, Link.data(), Link.size() * sizeof(MODEL_LINK));

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		struct MODEL_LINK_BIO1
		{
			std::uint32_t pModel;
			std::uint32_t pTexture;
		};

		std::vector<MODEL_LINK_BIO1> LinkBio1(Header().nOmodel);
		File.Read(Header().pOmodel, LinkBio1.data(), LinkBio1.size() * sizeof(MODEL_LINK_BIO1));

		for (std::size_t i = 0; i < Link.size(); i++)
		{
			Link[i].pModel = LinkBio1[i].pModel;
			Link[i].pTexture = LinkBio1[i].pTexture;
		}
	}

	for (std::size_t i = 0; i < Link.size(); i++)
	{
		Object[i] = std::make_shared<Resident_Evil_Model>();

		Object[i]->PlatformSetup(Str.hWnd, GTE, Render, b_HorzFlip, b_VertFlip);

		Object[i]->SetGame(m_Game);

		if (IsValidPointer(File, Link[i].pTexture))
		{
			Object[i]->OpenTexture(File, Link[i].pTexture);
		}

		if (IsValidPointer(File, Link[i].pModel))
		{
			if (GameType() & (BIO3))
			{
				Object[i]->Open(File, static_cast<std::uintmax_t>(Link[i].pModel + 0x18));	// TODO: add proper MD2 header support
			}
			else
			{
				Object[i]->Open(File, Link[i].pModel);
			}
		}
	}

	return true;
}

const bool Resident_Evil_Room::ReadESP(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read ESP in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (!IsValidPointer(File, Header().pEsp_hed)) { return true; }

	Esp->Open(File, EFF_IDX_MAX, Header().pEsp_hed, Header().pEsp_end, Header().pEsp_tim, Header().pEsp_tim_end);

	return true;
}

const bool Resident_Evil_Room::ReadRBJ(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read RBJ in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (GameType() & AUG95) { Rbj->SetGame(Video_Game::Resident_Evil_Aug_4_1995); }

	else if (GameType() & OCT95) { Rbj->SetGame(Video_Game::Resident_Evil_Oct_4_1995); }

	else if (GameType() & BIO1) { Rbj->SetGame(Video_Game::Resident_Evil); }

	else if (GameType() & BIO2NOV96) { Rbj->SetGame(Video_Game::Resident_Evil_2_Nov_6_1996); }

	else if (GameType() & (BIO2TRIAL | BIO2)) { Rbj->SetGame(Video_Game::Resident_Evil_2); }

	else if (GameType() & BIO3) { Rbj->SetGame(Video_Game::Resident_Evil_3); }

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		if (!IsValidPointer(File, Header().pEdd)) { return true; }
		if (!IsValidPointer(File, Header().pEmr)) { return true; }

		Rbj->Data.resize(1);
		Rbj->Data[0] = std::make_shared<Resident_Evil_Animation>();
		Rbj->Data[0]->Str.hWnd = Str.hWnd;
		Rbj->Data[0]->SetType(Resident_Evil_Animation_Type::Room);
		Rbj->Data[0]->SetGame(m_Game);
		Rbj->Data[0]->OpenEDD(File, m_Header.pEdd);
		Rbj->Data[0]->OpenEMR(File, m_Header.pEmr);
	}

	else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
	{
		if (!IsValidPointer(File, Header().pRbj)) { return true; }

		Rbj->OpenRBJ(File, m_Header.pRbj);
	}

	return true;
}

const bool Resident_Evil_Room::ReadEDT(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read EDT in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
	}

	else if (GameType() & BIO2NOV96)
	{
		if (IsValidPointer(File, Header().pEdt0)) { Edt0->Open(File, 32, Header().pEdt0); }
		if (IsValidPointer(File, Header().pEdt1)) { Edt1->Open(File, 24, Header().pEdt1); }
	}

	else if (GameType() & BIO2TRIAL)
	{
	}

	else if (GameType() & BIO2)
	{
		if (IsValidPointer(File, Header().pEdt0)) { Edt0->Open(File, 48, Header().pEdt0); }
	}

	else if (GameType() & BIO3)
	{
	}

	return true;
}

const bool Resident_Evil_Room::ReadVAB(StdFile& File)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil RDT Error: could not read VAB in \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (GameType() & (BIO3))
	{
	}
	else
	{
		if (IsValidPointer(File, Header().pVh0)) { Vab0->OpenVH(File, Header().pVh0); }
		if (IsValidPointer(File, Header().pVb0)) { Vab0->OpenVB(File, Header().pVb0); }

		if (IsValidPointer(File, Header().pVh1)) { Vab1->OpenVH(File, Header().pVh1); }
		if (IsValidPointer(File, Header().pVb1)) { Vab1->OpenVB(File, Header().pVb1); }
	}

	return true;
}

const std::uintmax_t Resident_Evil_Room::GetNextValidPointerBio1(StdFile& File, Room_Pointer_Type Start)
{
	struct MODEL_LINK_BIO1
	{
		std::uint32_t pModel;
		std::uint32_t pTexture;
	};

	if (Start == Room_Pointer_Type::LIT)
	{
		if (IsValidPointer(File, Header().pRcut)) { return Header().pRcut; }

		Start = Room_Pointer_Type::RCUT;
	}

	if (Start == Room_Pointer_Type::RCUT)
	{
		if (IsValidPointer(File, Header().pImodel)) { return Header().pImodel; }

		Start = Room_Pointer_Type::IMODEL;
	}

	if (Start == Room_Pointer_Type::IMODEL)
	{
		if (IsValidPointer(File, Header().pOmodel)) { return Header().pOmodel; }

		Start = Room_Pointer_Type::OMODEL;
	}

	if (Start == Room_Pointer_Type::OMODEL)
	{
		if (IsValidPointer(File, Header().pVcut)) { return Header().pVcut; }

		Start = Room_Pointer_Type::VCUT;
	}

	if (Start == Room_Pointer_Type::VCUT)
	{
		if (IsValidPointer(File, Header().pRcut))
		{
			std::vector<Resident_Evil_RID_Data> Data(Header().nCut);

			File.Get().clear();

			File.Read(Header().pRcut, Data.data(), Data.size() * sizeof(Resident_Evil_RID_Data));

			for (std::size_t i = 0; i < Data.size(); i++)
			{
				if (IsValidPointer(File, Data[i].pSp)) { return Data[i].pSp; }
			}

			for (std::size_t i = 0; i < Data.size(); i++)
			{
				if (IsValidPointer(File, Data[i].pTim)) { return Data[i].pTim; }
			}
		}

		Start = Room_Pointer_Type::PRI;
	}

	if (Start == Room_Pointer_Type::PRI)
	{
		if (IsValidPointer(File, Header().pSca)) { return Header().pSca; }

		Start = Room_Pointer_Type::SCA;
	}

	if (Start == Room_Pointer_Type::SCA)
	{
		if (IsValidPointer(File, Header().pBlock)) { return Header().pBlock; }

		Start = Room_Pointer_Type::BLK;
	}

	if (Start == Room_Pointer_Type::BLK)
	{
		if (IsValidPointer(File, Header().pFloor)) { return Header().pFloor; }

		Start = Room_Pointer_Type::FLR;
	}

	if (Start == Room_Pointer_Type::FLR)
	{
		if (IsValidPointer(File, Header().pScdX0)) { return Header().pScdX0; }

		Start = Room_Pointer_Type::SCDX0;
	}

	if (Start == Room_Pointer_Type::SCDX0)
	{
		if (IsValidPointer(File, Header().pScdX1)) { return Header().pScdX1; }

		Start = Room_Pointer_Type::SCDX1;
	}

	if (Start == Room_Pointer_Type::SCDX1)
	{
		if (IsValidPointer(File, Header().pScd0)) { return Header().pScd0; }

		Start = Room_Pointer_Type::SCD0;
	}

	if (Start == Room_Pointer_Type::SCD0)
	{
		if (IsValidPointer(File, Header().pScd1)) { return Header().pScd1; }

		Start = Room_Pointer_Type::SCD1;
	}

	if (Start == Room_Pointer_Type::SCD1)
	{
		if (IsValidPointer(File, Header().pScdEv0)) { return Header().pScdEv0; }

		Start = Room_Pointer_Type::SCDEV0;
	}

	if (Start == Room_Pointer_Type::SCDEV0)
	{
		if (IsValidPointer(File, Header().pScdEv1)) { return Header().pScdEv1; }

		Start = Room_Pointer_Type::SCDEV1;
	}

	if (Start == Room_Pointer_Type::SCDEV1)
	{
		if (IsValidPointer(File, Header().pEmr)) { return Header().pEmr; }

		Start = Room_Pointer_Type::EMR;
	}

	if (Start == Room_Pointer_Type::EMR)
	{
		if (IsValidPointer(File, Header().pEdd)) { return Header().pEdd; }

		Start = Room_Pointer_Type::EDD;
	}

	if (Start == Room_Pointer_Type::EDD)
	{
		if (IsValidPointer(File, Header().pMessage)) { return Header().pMessage; }

		Start = Room_Pointer_Type::MSG;
	}

	if (Start == Room_Pointer_Type::MSG)
	{
		if (IsValidPointer(File, Header().pItemPix)) { return Header().pItemPix; }

		Start = Room_Pointer_Type::IPIX;
	}

	if (Start == Room_Pointer_Type::IPIX)
	{
		if (IsValidPointer(File, Header().pEsp_hed)) { return Header().pEsp_hed; }

		Start = Room_Pointer_Type::ESP_HED;
	}

	if (Start == Room_Pointer_Type::ESP_HED)
	{
		if (IsValidPointer(File, Header().pEsp_hed) && IsValidPointer(File, Header().pEsp_end))
		{
			std::vector<std::uint8_t> ID(8);

			std::uint32_t Pointer = 0;

			File.Get().clear();

			File.Read(Header().pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

			for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
			{
				File.Read(Header().pEsp_end - (i * sizeof(std::uint32_t)), &Pointer, sizeof(std::uint32_t));

				if (IsValidPointer(File, Pointer)) { return Pointer; }
			}

			return Header().pEsp_end + sizeof(std::uint32_t);
		}

		if (IsValidPointer(File, Header().pEsp_end))
		{
			return Header().pEsp_end + sizeof(std::uint32_t);
		}

		Start = Room_Pointer_Type::ESP_END;
	}

	if (Start == Room_Pointer_Type::ESP_END)
	{
		if (IsValidPointer(File, Header().pImodel))
		{
			std::vector<MODEL_LINK_BIO1> ItemLink(Header().nItem);

			File.Get().clear();

			File.Read(Header().pImodel, ItemLink.data(), ItemLink.size() * sizeof(MODEL_LINK_BIO1));

			for (std::size_t i = 0; i < ItemLink.size(); i++)
			{
				if (IsValidPointer(File, ItemLink[i].pModel)) { return ItemLink[i].pModel; }
			}
		}

		Start = Room_Pointer_Type::IMODEL_MODEL;
	}

	if (Start == Room_Pointer_Type::IMODEL_MODEL)
	{
		if (IsValidPointer(File, Header().pOmodel))
		{
			std::vector<MODEL_LINK_BIO1> ObjectLink(Header().nOmodel);

			File.Get().clear();

			File.Read(Header().pOmodel, ObjectLink.data(), ObjectLink.size() * sizeof(MODEL_LINK_BIO1));

			for (std::size_t i = 0; i < ObjectLink.size(); i++)
			{
				if (IsValidPointer(File, ObjectLink[i].pModel)) { return ObjectLink[i].pModel; }
			}
		}

		Start = Room_Pointer_Type::OMODEL_MODEL;
	}

	if (Start == Room_Pointer_Type::OMODEL_MODEL)
	{
		if (IsValidPointer(File, Header().pEdt0)) { return Header().pEdt0; }

		Start = Room_Pointer_Type::EDT0;
	}

	if (Start == Room_Pointer_Type::EDT0)
	{
		if (IsValidPointer(File, Header().pVh0)) { return Header().pVh0; }

		Start = Room_Pointer_Type::VH0;
	}

	if (Start == Room_Pointer_Type::VH0)
	{
		if (IsValidPointer(File, Header().pVb0)) { return Header().pVb0; }

		Start = Room_Pointer_Type::VB0;
	}

	if (Start == Room_Pointer_Type::VB0)
	{
		if (IsValidPointer(File, Header().pImodel))
		{
			std::vector<MODEL_LINK_BIO1> ItemLink(Header().nItem);

			File.Get().clear();

			File.Read(Header().pImodel, ItemLink.data(), ItemLink.size() * sizeof(MODEL_LINK_BIO1));

			for (std::size_t i = 0; i < ItemLink.size(); i++)
			{
				if (IsValidPointer(File, ItemLink[i].pTexture)) { return ItemLink[i].pTexture; }
			}
		}

		Start = Room_Pointer_Type::IMODEL_TEX;
	}

	if (Start == Room_Pointer_Type::IMODEL_TEX)
	{
		if (IsValidPointer(File, Header().pOmodel))
		{
			std::vector<MODEL_LINK_BIO1> ObjectLink(Header().nOmodel);

			File.Get().clear();

			File.Read(Header().pOmodel, ObjectLink.data(), ObjectLink.size() * sizeof(MODEL_LINK_BIO1));

			for (std::size_t i = 0; i < ObjectLink.size(); i++)
			{
				if (IsValidPointer(File, ObjectLink[i].pTexture)) { return ObjectLink[i].pTexture; }
			}
		}

		Start = Room_Pointer_Type::OMODEL_TEX;
	}

	if (Start == Room_Pointer_Type::OMODEL_TEX)
	{
		Start = Room_Pointer_Type::ESP_TIM_END;
	}

	if (Start == Room_Pointer_Type::ESP_TIM_END)
	{
		if (IsValidPointer(File, Header().pEsp_hed) && IsValidPointer(File, Header().pEsp_tim_end))
		{
			std::vector<std::uint8_t> ID(8);

			std::uint32_t Pointer = 0;

			File.Get().clear();

			File.Read(Header().pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

			for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
			{
				File.Read(Header().pEsp_tim_end - (i * sizeof(std::uint32_t)), &Pointer, sizeof(std::uint32_t));

				if (IsValidPointer(File, Pointer)) { return Pointer; }
			}

			return Header().pEsp_tim_end + sizeof(std::uint32_t);
		}

		if (IsValidPointer(File, Header().pEsp_tim_end))
		{
			return Header().pEsp_tim_end + sizeof(std::uint32_t);
		}

		Start = Room_Pointer_Type::FILE_SIZE;
	}

	File.Get().clear();

	if (Start == Room_Pointer_Type::FILE_SIZE)
	{
		return File.Size();
	}

	return File.Size();
}

const std::uintmax_t Resident_Evil_Room::GetNextValidPointerBio2Nov96(StdFile& File, Room_Pointer_Type Start)
{
	struct MODEL_LINK
	{
		std::uint32_t pTexture;
		std::uint32_t pModel;
	};

	if (Start == Room_Pointer_Type::RCUT)
	{
		if (IsValidPointer(File, Header().pOmodel)) { return Header().pOmodel; }

		Start = Room_Pointer_Type::OMODEL;
	}

	if (Start == Room_Pointer_Type::OMODEL)
	{
		if (IsValidPointer(File, Header().pVcut)) { return Header().pVcut; }

		Start = Room_Pointer_Type::VCUT;
	}

	if (Start == Room_Pointer_Type::VCUT)
	{
		if (IsValidPointer(File, Header().pLight)) { return Header().pLight; }

		Start = Room_Pointer_Type::LIT;
	}

	if (Start == Room_Pointer_Type::LIT)
	{
		if (IsValidPointer(File, Header().pRcut))
		{
			std::vector<Resident_Evil_2_RID_Data> Data(Header().nCut);

			File.Get().clear();

			File.Read(Header().pRcut, Data.data(), Data.size() * sizeof(Resident_Evil_2_RID_Data));

			for (std::size_t i = 0; i < Data.size(); i++)
			{
				if (IsValidPointer(File, Data[i].pSp)) { return Data[i].pSp; }
			}
		}

		Start = Room_Pointer_Type::PRI;
	}

	if (Start == Room_Pointer_Type::PRI)
	{
		if (IsValidPointer(File, Header().pSca)) { return Header().pSca; }

		Start = Room_Pointer_Type::SCA;
	}

	if (Start == Room_Pointer_Type::SCA)
	{
		if (IsValidPointer(File, Header().pBlock)) { return Header().pBlock; }

		Start = Room_Pointer_Type::BLK;
	}

	if (Start == Room_Pointer_Type::BLK)
	{
		if (IsValidPointer(File, Header().pFloor)) { return Header().pFloor; }

		Start = Room_Pointer_Type::FLR;
	}

	if (Start == Room_Pointer_Type::FLR)
	{
		if (IsValidPointer(File, Header().pScdX0)) { return Header().pScdX0; }

		Start = Room_Pointer_Type::SCDX0;
	}

	if (Start == Room_Pointer_Type::SCDX0)
	{
		if (IsValidPointer(File, Header().pScd0)) { return Header().pScd0; }

		Start = Room_Pointer_Type::SCD0;
	}

	if (Start == Room_Pointer_Type::SCD0)
	{
		if (IsValidPointer(File, Header().pScd1)) { return Header().pScd1; }

		Start = Room_Pointer_Type::SCD1;
	}

	if (Start == Room_Pointer_Type::SCD1)
	{
		if (IsValidPointer(File, Header().pMessage)) { return Header().pMessage; }

		Start = Room_Pointer_Type::MSG;
	}

	if (Start == Room_Pointer_Type::MSG)
	{
		if (IsValidPointer(File, Header().pRbj)) { return Header().pRbj; }

		Start = Room_Pointer_Type::RBJ;
	}

	if (Start == Room_Pointer_Type::RBJ)
	{
		if (IsValidPointer(File, Header().pOmodel))
		{
			std::vector<MODEL_LINK> ObjectLink(Header().nOmodel);

			File.Get().clear();

			File.Read(Header().pOmodel, ObjectLink.data(), ObjectLink.size() * sizeof(MODEL_LINK));

			for (std::size_t i = 0; i < ObjectLink.size(); i++)
			{
				if (IsValidPointer(File, ObjectLink[i].pModel)) { return ObjectLink[i].pModel; }
			}
		}

		Start = Room_Pointer_Type::OMODEL_MODEL;
	}

	if (Start == Room_Pointer_Type::OMODEL_MODEL)
	{
		if (IsValidPointer(File, Header().pEsp_hed)) { return Header().pEsp_hed; }

		Start = Room_Pointer_Type::ESP_HED;
	}

	if (Start == Room_Pointer_Type::ESP_HED)
	{
		if (IsValidPointer(File, Header().pEsp_hed) && IsValidPointer(File, Header().pEsp_end))
		{
			std::vector<std::uint8_t> ID(8);

			std::uint32_t Pointer = 0;

			File.Get().clear();

			File.Read(Header().pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

			for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
			{
				File.Read(Header().pEsp_end - (i * sizeof(std::uint32_t)), &Pointer, sizeof(std::uint32_t));

				if (IsValidPointer(File, Header().pEsp_hed + Pointer)) { return Header().pEsp_hed + Pointer; }
			}

			return Header().pEsp_end + sizeof(std::uint32_t);
		}

		if (IsValidPointer(File, Header().pEsp_end))
		{
			return Header().pEsp_end + sizeof(std::uint32_t);
		}

		Start = Room_Pointer_Type::ESP_END;
	}

	if (Start == Room_Pointer_Type::ESP_END)
	{
		if (IsValidPointer(File, Header().pEdt0)) { return Header().pEdt0; }

		Start = Room_Pointer_Type::EDT0;
	}

	if (Start == Room_Pointer_Type::EDT0)
	{
		if (IsValidPointer(File, Header().pVh0)) { return Header().pVh0; }

		Start = Room_Pointer_Type::VH0;
	}

	if (Start == Room_Pointer_Type::VH0)
	{
		if (IsValidPointer(File, Header().pEdt1)) { return Header().pEdt1; }

		Start = Room_Pointer_Type::EDT1;
	}

	if (Start == Room_Pointer_Type::EDT1)
	{
		if (IsValidPointer(File, Header().pVh1)) { return Header().pVh1; }

		Start = Room_Pointer_Type::VH1;
	}

	if (Start == Room_Pointer_Type::VH1)
	{
		if (IsValidPointer(File, Header().pVb0)) { return Header().pVb0; }

		Start = Room_Pointer_Type::VB0;
	}

	if (Start == Room_Pointer_Type::VB0)
	{
		if (IsValidPointer(File, Header().pVb1)) { return Header().pVb1; }

		Start = Room_Pointer_Type::VB1;
	}

	if (Start == Room_Pointer_Type::VB1)
	{
		Start = Room_Pointer_Type::ESP_TIM;
	}

	if (Start == Room_Pointer_Type::ESP_TIM)
	{
		if (IsValidPointer(File, Header().pEsp_hed) && IsValidPointer(File, Header().pEsp_tim_end))
		{
			std::vector<std::uint8_t> ID(8);

			std::uint32_t Pointer = 0;

			File.Get().clear();

			File.Read(Header().pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

			for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
			{
				File.Read(Header().pEsp_tim_end - (i * sizeof(std::uint32_t)) - sizeof(std::uint32_t), &Pointer, sizeof(std::uint32_t));

				if (IsValidPointer(File, Header().pEsp_tim + Pointer)) { return Header().pEsp_tim + Pointer; }
			}

			return Header().pEsp_tim_end;
		}

		if (IsValidPointer(File, Header().pEsp_tim_end))
		{
			return Header().pEsp_tim_end;
		}

		Start = Room_Pointer_Type::ESP_TIM_END;
	}

	if (Start == Room_Pointer_Type::ESP_TIM_END)
	{
		if (IsValidPointer(File, Header().pOmodel))
		{
			std::vector<MODEL_LINK> ObjectLink(Header().nOmodel);

			File.Get().clear();

			File.Read(Header().pOmodel, ObjectLink.data(), ObjectLink.size() * sizeof(MODEL_LINK));

			for (std::size_t i = 0; i < ObjectLink.size(); i++)
			{
				if (IsValidPointer(File, ObjectLink[i].pTexture)) { return ObjectLink[i].pTexture; }
			}
		}

		Start = Room_Pointer_Type::FILE_SIZE;
	}

	File.Get().clear();

	if (Start == Room_Pointer_Type::FILE_SIZE)
	{
		return File.Size();
	}

	return File.Size();
}

const std::uintmax_t Resident_Evil_Room::GetNextValidPointerBio2(StdFile& File, Room_Pointer_Type Start)
{
	struct MODEL_LINK
	{
		std::uint32_t pTexture;
		std::uint32_t pModel;
	};

	if (Start == Room_Pointer_Type::RCUT)
	{
		if (IsValidPointer(File, Header().pOmodel)) { return Header().pOmodel; }

		Start = Room_Pointer_Type::OMODEL;
	}

	if (Start == Room_Pointer_Type::OMODEL)
	{
		if (IsValidPointer(File, Header().pVcut)) { return Header().pVcut; }

		Start = Room_Pointer_Type::VCUT;
	}

	if (Start == Room_Pointer_Type::VCUT)
	{
		if (IsValidPointer(File, Header().pLight)) { return Header().pLight; }

		Start = Room_Pointer_Type::LIT;
	}

	if (Start == Room_Pointer_Type::LIT)
	{
		if (IsValidPointer(File, Header().pRcut))
		{
			std::vector<Resident_Evil_2_RID_Data> Data(Header().nCut);

			File.Get().clear();

			File.Read(Header().pRcut, Data.data(), Data.size() * sizeof(Resident_Evil_2_RID_Data));

			for (std::size_t i = 0; i < Data.size(); i++)
			{
				if (IsValidPointer(File, Data[i].pSp)) { return Data[i].pSp; }
			}
		}

		Start = Room_Pointer_Type::PRI;
	}

	if (Start == Room_Pointer_Type::PRI)
	{
		if (IsValidPointer(File, Header().pSca)) { return Header().pSca; }

		Start = Room_Pointer_Type::SCA;
	}

	if (Start == Room_Pointer_Type::SCA)
	{
		if (IsValidPointer(File, Header().pBlock)) { return Header().pBlock; }

		Start = Room_Pointer_Type::BLK;
	}

	if (Start == Room_Pointer_Type::BLK)
	{
		if (IsValidPointer(File, Header().pFloor)) { return Header().pFloor; }

		Start = Room_Pointer_Type::FLR;
	}

	if (Start == Room_Pointer_Type::FLR)
	{
		if (IsValidPointer(File, Header().pScdX0)) { return Header().pScdX0; }

		Start = Room_Pointer_Type::SCDX0;
	}

	if (Start == Room_Pointer_Type::SCDX0)
	{
		if (IsValidPointer(File, Header().pScd0)) { return Header().pScd0; }

		Start = Room_Pointer_Type::SCD0;
	}

	if (Start == Room_Pointer_Type::SCD0)
	{
		if (IsValidPointer(File, Header().pScd1)) { return Header().pScd1; }

		Start = Room_Pointer_Type::SCD1;
	}

	if (Start == Room_Pointer_Type::SCD1)
	{
		if (IsValidPointer(File, Header().pMessage)) { return Header().pMessage; }

		Start = Room_Pointer_Type::MSG;
	}

	if (Start == Room_Pointer_Type::MSG)
	{
		if (IsValidPointer(File, Header().pMessage_sub)) { return Header().pMessage_sub; }

		Start = Room_Pointer_Type::MSG_SUB;
	}

	if (Start == Room_Pointer_Type::MSG_SUB)
	{
		if (IsValidPointer(File, Header().pScrl)) { return Header().pScrl; }

		Start = Room_Pointer_Type::SCRL;
	}

	if (Start == Room_Pointer_Type::SCRL)
	{
		if (IsValidPointer(File, Header().pOmodel))
		{
			std::vector<MODEL_LINK> ObjectLink(Header().nOmodel);

			File.Get().clear();

			File.Read(Header().pOmodel, ObjectLink.data(), ObjectLink.size() * sizeof(MODEL_LINK));

			for (std::size_t i = 0; i < ObjectLink.size(); i++)
			{
				if (IsValidPointer(File, ObjectLink[i].pModel)) { return ObjectLink[i].pModel; }
			}
		}

		if (IsValidPointer(File, Header().pEsp_hed)) { return Header().pEsp_hed; }

		Start = Room_Pointer_Type::OMODEL_MODEL;
	}

	if (Start == Room_Pointer_Type::OMODEL_MODEL)
	{
		if (IsValidPointer(File, Header().pEsp_hed)) { return Header().pEsp_hed; }

		Start = Room_Pointer_Type::ESP_HED;
	}

	if (Start == Room_Pointer_Type::ESP_HED)
	{
		if (IsValidPointer(File, Header().pEsp_hed) && IsValidPointer(File, Header().pEsp_end))
		{
			std::vector<std::uint8_t> ID(8);

			std::uint32_t Pointer = 0;

			File.Get().clear();

			File.Read(Header().pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

			for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
			{
				File.Read(Header().pEsp_end - (i * sizeof(std::uint32_t)), &Pointer, sizeof(std::uint32_t));

				if (IsValidPointer(File, Header().pEsp_hed + Pointer)) { return Header().pEsp_hed + Pointer; }
			}

			return Header().pEsp_end + sizeof(std::uint32_t);
		}

		if (IsValidPointer(File, Header().pEsp_end))
		{
			return Header().pEsp_end + sizeof(std::uint32_t);
		}

		Start = Room_Pointer_Type::ESP_END;
	}

	if (Start == Room_Pointer_Type::ESP_END)
	{
		if (IsValidPointer(File, Header().pRbj)) { return Header().pRbj; }

		Start = Room_Pointer_Type::RBJ;
	}

	if (Start == Room_Pointer_Type::RBJ)
	{
		if (IsValidPointer(File, Header().pRbj_end)) { return Header().pRbj_end; }

		Start = Room_Pointer_Type::RBJ_END;
	}

	if (Start == Room_Pointer_Type::RBJ_END)
	{
		if (IsValidPointer(File, Header().pEdt0)) { return Header().pEdt0; }

		Start = Room_Pointer_Type::EDT0;
	}

	if (Start == Room_Pointer_Type::EDT0)
	{
		if (IsValidPointer(File, Header().pEdt1)) { return Header().pEdt1; }

		Start = Room_Pointer_Type::EDT1;
	}

	if (Start == Room_Pointer_Type::EDT1)
	{
		if (IsValidPointer(File, Header().pVh0)) { return Header().pVh0; }

		Start = Room_Pointer_Type::VH0;
	}

	if (Start == Room_Pointer_Type::VH0)
	{
		if (IsValidPointer(File, Header().pVb0)) { return Header().pVb0; }

		Start = Room_Pointer_Type::VB0;
	}

	if (Start == Room_Pointer_Type::VB0)
	{
		if (IsValidPointer(File, Header().pVh1)) { return Header().pVh1; }

		Start = Room_Pointer_Type::VH1;
	}

	if (Start == Room_Pointer_Type::VH1)
	{
		Start = Room_Pointer_Type::ESP_TIM;
	}

	if (Start == Room_Pointer_Type::ESP_TIM)
	{
		if (IsValidPointer(File, Header().pEsp_hed) && IsValidPointer(File, Header().pEsp_tim_end))
		{
			std::vector<std::uint8_t> ID(8);

			std::uint32_t Pointer = 0;

			File.Get().clear();

			File.Read(Header().pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

			for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
			{
				File.Read(Header().pEsp_tim_end - (i * sizeof(std::uint32_t)) - sizeof(std::uint32_t), &Pointer, sizeof(std::uint32_t));

				if (IsValidPointer(File, Header().pEsp_tim + Pointer)) { return Header().pEsp_tim + Pointer; }
			}

			return Header().pEsp_tim_end;
		}

		if (IsValidPointer(File, Header().pEsp_tim_end))
		{
			return Header().pEsp_tim_end;
		}

		Start = Room_Pointer_Type::ESP_TIM_END;
	}

	if (Start == Room_Pointer_Type::ESP_TIM_END)
	{
		if (IsValidPointer(File, Header().pOmodel))
		{
			std::vector<MODEL_LINK> ObjectLink(Header().nOmodel);

			File.Get().clear();

			File.Read(Header().pOmodel, ObjectLink.data(), ObjectLink.size() * sizeof(MODEL_LINK));

			for (std::size_t i = 0; i < ObjectLink.size(); i++)
			{
				if (IsValidPointer(File, ObjectLink[i].pTexture)) { return ObjectLink[i].pTexture; }
			}
		}

		Start = Room_Pointer_Type::FILE_SIZE;
	}

	File.Get().clear();

	if (Start == Room_Pointer_Type::FILE_SIZE)
	{
		return File.Size();
	}

	return File.Size();
}

const std::uintmax_t Resident_Evil_Room::GetNextValidPointerBio3(StdFile& File, Room_Pointer_Type Start)
{
	File.Get().clear();

	if (Start == Room_Pointer_Type::FILE_SIZE)
	{
		return File.Size();
	}

	return File.Size();
}

void Resident_Evil_Room::DebugPrint(StdFile& File)
{
	std::cout << "RDT:" << std::endl;

	std::cout << std::left;
	std::cout << "  " << std::setw(20) << "Game:" << GameStr() << std::endl;

	auto PrintField = [&](const char* Label, std::uintmax_t Pointer, Room_Pointer_Type type)
		{
			std::cout << std::left;

			if (GameType() & (AUG95 | OCT95 | BIO1))
			{
				std::cout << "  " << std::setw(20) << Label
					<< std::setw(12) << Str.FormatCStyle("0x%x", Pointer).c_str()
					<< std::setw(6) << "Next:"
					<< std::setw(12) << Str.FormatCStyle("0x%x", GetNextValidPointerBio1(File, type))
					<< std::endl;
			}

			else if (GameType() & BIO2NOV96)
			{
				std::cout << "  " << std::setw(20) << Label
					<< std::setw(12) << Str.FormatCStyle("0x%x", Pointer).c_str()
					<< std::setw(6) << "Next:"
					<< std::setw(12) << Str.FormatCStyle("0x%x", GetNextValidPointerBio2Nov96(File, type))
					<< std::endl;
			}

			else if (GameType() & BIO2)
			{
				std::cout << "  " << std::setw(20) << Label
					<< std::setw(12) << Str.FormatCStyle("0x%x", Pointer).c_str()
					<< std::setw(6) << "Next:"
					<< std::setw(12) << Str.FormatCStyle("0x%x", GetNextValidPointerBio2(File, type))
					<< std::endl;
			}

			else if (GameType() & BIO3)
			{
				std::cout << "  " << std::setw(20) << Label
					<< std::setw(12) << Str.FormatCStyle("0x%x", Pointer).c_str()
					<< std::setw(6) << "Next:"
					<< std::setw(12) << Str.FormatCStyle("0x%x", GetNextValidPointerBio3(File, type))
					<< std::endl;
			}
		};

	struct MODEL_LINK
	{
		std::uint32_t pTexture;
		std::uint32_t pModel;
	};

	struct MODEL_LINK_BIO1
	{
		std::uint32_t pModel;
		std::uint32_t pTexture;
	};

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		std::vector<MODEL_LINK_BIO1> ItemLink(Header().nItem);

		std::vector<MODEL_LINK_BIO1> ObjectLink(Header().nOmodel);

		if (IsValidPointer(File, Header().pImodel))
		{
			File.Get().clear();

			File.Read(Header().pImodel, ItemLink.data(), ItemLink.size() * sizeof(MODEL_LINK_BIO1));
		}

		if (IsValidPointer(File, Header().pOmodel))
		{
			File.Get().clear();

			File.Read(Header().pOmodel, ObjectLink.data(), ObjectLink.size() * sizeof(MODEL_LINK_BIO1));
		}

		PrintField("LIT:", Header().pLight, Room_Pointer_Type::LIT);
		PrintField("RID:", Header().pRcut, Room_Pointer_Type::RCUT);
		PrintField("ITEM_PTR:", Header().pImodel, Room_Pointer_Type::IMODEL);
		PrintField("OBJECT_PTR:", Header().pOmodel, Room_Pointer_Type::OMODEL);
		PrintField("RVD:", Header().pVcut, Room_Pointer_Type::VCUT);

		if (IsValidPointer(File, Header().pRcut))
		{
			std::vector<Resident_Evil_RID_Data> Data(Header().nCut);

			File.Get().clear();

			File.Read(Header().pRcut, Data.data(), Data.size() * sizeof(Resident_Evil_RID_Data));

			for (std::size_t i = 0; i < Data.size(); i++)
			{
				PrintField(Str.FormatCStyle("PRI[%02d]_DAT:", i).c_str(), Data[i].pSp, Room_Pointer_Type::PRI);
			}

			for (std::size_t i = 0; i < Data.size(); i++)
			{
				PrintField(Str.FormatCStyle("PRI[%02d]_TEX:", i).c_str(), Data[i].pTim, Room_Pointer_Type::PRI);
			}
		}

		PrintField("SCA:", Header().pSca, Room_Pointer_Type::SCA);
		PrintField("BLK:", Header().pBlock, Room_Pointer_Type::BLK);
		PrintField("FLR:", Header().pFloor, Room_Pointer_Type::FLR);
		PrintField("SCDX0:", Header().pScdX0, Room_Pointer_Type::SCDX0);
		PrintField("SCDX1:", Header().pScdX1, Room_Pointer_Type::SCDX1);
		PrintField("SCD0:", Header().pScd0, Room_Pointer_Type::SCD0);
		PrintField("SCD1:", Header().pScd1, Room_Pointer_Type::SCD1);
		PrintField("SCDEV0:", Header().pScdEv0, Room_Pointer_Type::SCDEV0);
		PrintField("SCDEV1:", Header().pScdEv1, Room_Pointer_Type::SCDEV1);
		PrintField("EMR:", Header().pEmr, Room_Pointer_Type::EMR);
		PrintField("EDD:", Header().pEdd, Room_Pointer_Type::EDD);
		PrintField("MSG:", Header().pMessage, Room_Pointer_Type::MSG);
		PrintField("ITEM_PIX:", Header().pItemPix, Room_Pointer_Type::IPIX);
		PrintField("ESP_HED:", Header().pEsp_hed, Room_Pointer_Type::ESP_HED);

		if (IsValidPointer(File, Header().pEsp_hed) && IsValidPointer(File, Header().pEsp_end))
		{
			std::vector<std::uint8_t> ID(8);

			std::uint32_t Pointer = 0;

			File.Get().clear();

			File.Read(Header().pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

			for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
			{
				File.Read(Header().pEsp_end - (i * sizeof(std::uint32_t)), &Pointer, sizeof(std::uint32_t));

				if (IsValidPointer(File, Pointer)) { PrintField(Str.FormatCStyle("ESP[%02d]_DAT:", i).c_str(), Pointer, Room_Pointer_Type::ESP_END); }
			}
		}

		PrintField("ESP_END:", Header().pEsp_end, Room_Pointer_Type::ESP_END);

		for (std::size_t i = 0; i < ItemLink.size(); i++)
		{
			PrintField(Str.FormatCStyle("ITEM[%02d]_DAT:", i).c_str(), ItemLink[i].pModel, Room_Pointer_Type::IMODEL_MODEL);
		}

		for (std::size_t i = 0; i < ObjectLink.size(); i++)
		{
			PrintField(Str.FormatCStyle("OBJECT[%02d]_DAT:", i).c_str(), ObjectLink[i].pModel, Room_Pointer_Type::OMODEL_MODEL);
		}

		PrintField("EDT0:", Header().pEdt0, Room_Pointer_Type::EDT0);
		PrintField("VH0:", Header().pVh0, Room_Pointer_Type::VH0);
		PrintField("VB0:", Header().pVb0, Room_Pointer_Type::VB0);

		for (std::size_t i = 0; i < ItemLink.size(); i++)
		{
			PrintField(Str.FormatCStyle("ITEM[%02d]_TEX:", i).c_str(), ItemLink[i].pTexture, Room_Pointer_Type::IMODEL_TEX);
		}

		for (std::size_t i = 0; i < ObjectLink.size(); i++)
		{
			PrintField(Str.FormatCStyle("OBJECT[%02d]_TEX:", i).c_str(), ObjectLink[i].pTexture, Room_Pointer_Type::OMODEL_TEX);
		}

		if (IsValidPointer(File, Header().pEsp_hed) && IsValidPointer(File, Header().pEsp_tim_end))
		{
			std::vector<std::uint8_t> ID(8);

			std::uint32_t Pointer = 0;

			File.Get().clear();

			File.Read(Header().pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

			for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
			{
				File.Read(Header().pEsp_tim_end - (i * sizeof(std::uint32_t)), &Pointer, sizeof(std::uint32_t));

				if (IsValidPointer(File, Pointer)) { PrintField(Str.FormatCStyle("ESP[%02d]_TEX:", i).c_str(), Pointer, Room_Pointer_Type::FILE_SIZE); }
			}
		}

		PrintField("ESP_TIM_END:", Header().pEsp_tim_end, Room_Pointer_Type::FILE_SIZE);

		PrintField("FILE_SIZE:", File.Size(), Room_Pointer_Type::FILE_SIZE);
	}

	if (GameType() & BIO2NOV96)
	{
		std::vector<MODEL_LINK> ObjectLink(Header().nOmodel);

		if (IsValidPointer(File, Header().pOmodel))
		{
			File.Get().clear();

			File.Read(Header().pOmodel, ObjectLink.data(), ObjectLink.size() * sizeof(MODEL_LINK));
		}

		PrintField("RID:", Header().pRcut, Room_Pointer_Type::RCUT);
		PrintField("OBJECT_PTR:", Header().pOmodel, Room_Pointer_Type::OMODEL);
		PrintField("RVD:", Header().pVcut, Room_Pointer_Type::VCUT);
		PrintField("LIT:", Header().pLight, Room_Pointer_Type::LIT);

		if (IsValidPointer(File, Header().pRcut))
		{
			std::vector<Resident_Evil_2_RID_Data> Data(Header().nCut);

			File.Get().clear();

			File.Read(Header().pRcut, Data.data(), Data.size() * sizeof(Resident_Evil_2_RID_Data));

			for (std::size_t i = 0; i < Data.size(); i++)
			{
				PrintField(Str.FormatCStyle("PRI[%02d]_DAT:", i).c_str(), Data[i].pSp, Room_Pointer_Type::PRI);
			}
		}

		PrintField("SCA:", Header().pSca, Room_Pointer_Type::SCA);
		PrintField("BLK:", Header().pBlock, Room_Pointer_Type::BLK);
		PrintField("FLR:", Header().pFloor, Room_Pointer_Type::FLR);
		PrintField("SCDX0:", Header().pScdX0, Room_Pointer_Type::SCDX0);
		PrintField("SCD0:", Header().pScd0, Room_Pointer_Type::SCD0);
		PrintField("SCD1:", Header().pScd1, Room_Pointer_Type::SCD1);
		PrintField("MSG:", Header().pMessage, Room_Pointer_Type::MSG);
		PrintField("RBJ:", Header().pRbj, Room_Pointer_Type::RBJ);

		for (std::size_t i = 0; i < ObjectLink.size(); i++)
		{
			PrintField(Str.FormatCStyle("OBJECT[%02d]_DAT:", i).c_str(), ObjectLink[i].pModel, Room_Pointer_Type::OMODEL_MODEL);
		}

		PrintField("ESP_HED:", Header().pEsp_hed, Room_Pointer_Type::ESP_HED);

		if (IsValidPointer(File, Header().pEsp_hed) && IsValidPointer(File, Header().pEsp_end))
		{
			std::vector<std::uint8_t> ID(8);

			std::uint32_t Pointer = 0;

			File.Get().clear();

			File.Read(Header().pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

			for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
			{
				File.Read(Header().pEsp_end - (i * sizeof(std::uint32_t)), &Pointer, sizeof(std::uint32_t));

				if (IsValidPointer(File, Header().pEsp_hed + Pointer)) { PrintField(Str.FormatCStyle("ESP[%02d]_DAT:", i).c_str(), Header().pEsp_hed + Pointer, Room_Pointer_Type::ESP_END); }
			}
		}

		PrintField("ESP_END:", Header().pEsp_end, Room_Pointer_Type::ESP_END);

		PrintField("EDT0:", Header().pEdt0, Room_Pointer_Type::EDT0);
		PrintField("VH0:", Header().pVh0, Room_Pointer_Type::VH0);
		PrintField("EDT1:", Header().pEdt1, Room_Pointer_Type::EDT1);
		PrintField("VH1:", Header().pVh1, Room_Pointer_Type::VH1);
		PrintField("VB0:", Header().pVb0, Room_Pointer_Type::VB0);
		PrintField("VB1:", Header().pVb1, Room_Pointer_Type::VB1);

		PrintField("ESP_TIM:", Header().pEsp_tim, Room_Pointer_Type::ESP_TIM);

		if (IsValidPointer(File, Header().pEsp_hed) && IsValidPointer(File, Header().pEsp_tim_end))
		{
			std::vector<std::uint8_t> ID(8);

			std::uint32_t Pointer = 0;

			File.Get().clear();

			File.Read(Header().pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

			for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
			{
				File.Read(Header().pEsp_tim_end - (i * sizeof(std::uint32_t)) - sizeof(std::uint32_t), &Pointer, sizeof(std::uint32_t));

				if (IsValidPointer(File, Header().pEsp_tim + Pointer)) { PrintField(Str.FormatCStyle("ESP[%02d]_TEX:", i).c_str(), Header().pEsp_tim + Pointer, Room_Pointer_Type::ESP_TIM_END); }
			}
		}

		PrintField("ESP_TIM_END:", Header().pEsp_tim_end, Room_Pointer_Type::ESP_TIM_END);

		for (std::size_t i = 0; i < ObjectLink.size(); i++)
		{
			PrintField(Str.FormatCStyle("OBJECT[%02d]_TEX:", i).c_str(), ObjectLink[i].pTexture, Room_Pointer_Type::OMODEL_TEX);
		}

		PrintField("FILE_SIZE:", File.Size(), Room_Pointer_Type::FILE_SIZE);
	}

	if (GameType() & (BIO2TRIAL | BIO2))
	{
		std::vector<MODEL_LINK> ObjectLink(Header().nOmodel);

		if (IsValidPointer(File, Header().pOmodel))
		{
			File.Get().clear();

			File.Read(Header().pOmodel, ObjectLink.data(), ObjectLink.size() * sizeof(MODEL_LINK));
		}

		PrintField("RID:", Header().pRcut, Room_Pointer_Type::RCUT);
		PrintField("OBJECT_PTR:", Header().pOmodel, Room_Pointer_Type::OMODEL);
		PrintField("RVD:", Header().pVcut, Room_Pointer_Type::VCUT);
		PrintField("LIT:", Header().pLight, Room_Pointer_Type::LIT);

		if (IsValidPointer(File, Header().pRcut))
		{
			std::vector<Resident_Evil_2_RID_Data> Data(Header().nCut);

			File.Get().clear();

			File.Read(Header().pRcut, Data.data(), Data.size() * sizeof(Resident_Evil_2_RID_Data));

			for (std::size_t i = 0; i < Data.size(); i++)
			{
				PrintField(Str.FormatCStyle("PRI[%02d]_DAT:", i).c_str(), Data[i].pSp, Room_Pointer_Type::PRI);
			}
		}

		PrintField("SCA:", Header().pSca, Room_Pointer_Type::SCA);
		PrintField("BLK:", Header().pBlock, Room_Pointer_Type::BLK);
		PrintField("FLR:", Header().pFloor, Room_Pointer_Type::FLR);
		PrintField("SCDX0:", Header().pScdX0, Room_Pointer_Type::SCDX0);
		PrintField("SCD0:", Header().pScd0, Room_Pointer_Type::SCD0);
		PrintField("SCD1:", Header().pScd1, Room_Pointer_Type::SCD1);
		PrintField("MSG:", Header().pMessage, Room_Pointer_Type::MSG);
		PrintField("MSG_SUB:", Header().pMessage_sub, Room_Pointer_Type::MSG_SUB);
		PrintField("SCRL:", Header().pScrl, Room_Pointer_Type::SCRL);

		for (std::size_t i = 0; i < ObjectLink.size(); i++)
		{
			PrintField(Str.FormatCStyle("OBJECT[%02d]_DAT:", i).c_str(), ObjectLink[i].pModel, Room_Pointer_Type::OMODEL_MODEL);
		}

		PrintField("ESP_HED:", Header().pEsp_hed, Room_Pointer_Type::ESP_HED);

		if (IsValidPointer(File, Header().pEsp_hed) && IsValidPointer(File, Header().pEsp_end))
		{
			std::vector<std::uint8_t> ID(8);

			std::uint32_t Pointer = 0;

			File.Get().clear();

			File.Read(Header().pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

			for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
			{
				File.Read(Header().pEsp_end - (i * sizeof(std::uint32_t)), &Pointer, sizeof(std::uint32_t));

				if (IsValidPointer(File, Header().pEsp_hed + Pointer)) { PrintField(Str.FormatCStyle("ESP[%02d]_DAT:", i).c_str(), Header().pEsp_hed + Pointer, Room_Pointer_Type::ESP_END); }
			}
		}

		PrintField("ESP_END:", Header().pEsp_end, Room_Pointer_Type::ESP_END);
		PrintField("RBJ:", Header().pRbj, Room_Pointer_Type::RBJ);
		PrintField("RBJ_END:", Header().pRbj_end, Room_Pointer_Type::RBJ_END);
		PrintField("EDT0:", Header().pEdt0, Room_Pointer_Type::EDT0);
		PrintField("EDT1:", Header().pEdt1, Room_Pointer_Type::EDT1);
		PrintField("VH0:", Header().pVh0, Room_Pointer_Type::VH0);
		PrintField("VB0:", Header().pVb0, Room_Pointer_Type::VB0);
		PrintField("VH1:", Header().pVh1, Room_Pointer_Type::VH1);

		PrintField("ESP_TIM:", Header().pEsp_tim, Room_Pointer_Type::ESP_TIM);

		if (IsValidPointer(File, Header().pEsp_hed) && IsValidPointer(File, Header().pEsp_tim_end))
		{
			std::vector<std::uint8_t> ID(8);

			std::uint32_t Pointer = 0;

			File.Get().clear();

			File.Read(Header().pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

			for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
			{
				File.Read(Header().pEsp_tim_end - (i * sizeof(std::uint32_t)) - sizeof(std::uint32_t), &Pointer, sizeof(std::uint32_t));

				if (IsValidPointer(File, Header().pEsp_tim + Pointer)) { PrintField(Str.FormatCStyle("ESP[%02d]_TEX:", i).c_str(), Header().pEsp_tim + Pointer, Room_Pointer_Type::ESP_TIM_END); }
			}
		}

		PrintField("ESP_TIM_END:", Header().pEsp_tim_end, Room_Pointer_Type::ESP_TIM_END);

		for (std::size_t i = 0; i < ObjectLink.size(); i++)
		{
			PrintField(Str.FormatCStyle("OBJECT[%02d]_TEX:", i).c_str(), ObjectLink[i].pTexture, Room_Pointer_Type::OMODEL_TEX);
		}

		PrintField("FILE_SIZE:", File.Size(), Room_Pointer_Type::FILE_SIZE);
	}
}

const bool Resident_Evil_Room::Open(std::filesystem::path Path)
{
	StdFile m_File { Path, FileAccessMode::Read, true, false };

	if (!m_File.IsOpen())
	{
		Str.Message(L"Resident Evil RDT Error: could not open \"%ws\"", m_File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (b_Open.load()) { Close(); }

	m_RoomGame = m_Game;

	m_Path = Standard_FileSystem().GetDirectory(Path);

	GetStageRoom(Path.filename().string().c_str());

	if (!ReadHeader(m_File)) { return false; }

	if (!ReadRID(m_File)) { return false; }

	if (!ReadRVD(m_File)) { return false; }
	
	if (!ReadLIT(m_File)) { return false; }

	if (!ReadSCA(m_File)) { return false; }

	if (!ReadBLK(m_File)) { return false; }

	if (!ReadFLR(m_File)) { return false; }

	if (!ReadOTA(m_File)) { return false; }

	if (!ReadSCD(m_File)) { return false; }

	if (!ReadMSG(m_File)) { return false; }

	if (!ReadIPIX(m_File)) { return false; }

	if (!ReadSCRL(m_File)) { return false; }

	if (!ReadModelItem(m_File)) { return false; }

	if (!ReadModelObject(m_File)) { return false; }

	if (!ReadESP(m_File)) { return false; }

	if (!ReadRBJ(m_File)) { return false; }

	if (!ReadEDT(m_File)) { return false; }

	if (!ReadVAB(m_File)) { return false; }

#ifdef _DEBUG
	//DebugPrint(m_File);
#endif

	SetWindow(Str.hWnd);

	b_Open.store(true);

	return true;
}

void Resident_Evil_Room::Close(void)
{
	b_Open.store(false);

	ResetEditor();

	Script->Close();

	std::memset(&m_Header, 0, sizeof(Resident_Evil_Room_Header));

	m_Stage = 0;
	m_Room = 0;
	m_Disk = 0;

	m_Path.clear();

	Rid->Close();

	for (auto& Element : Pri)
	{
		if (Element)
		{
			Element->Close();
			Element.reset();
		}
	}
	Pri.clear();
	Pri.shrink_to_fit();

	for (auto& Element : PriTex)
	{
		if (Element)
		{
			Element->Close();
			Element.reset();
		}
	}
	PriTex.clear();
	PriTex.shrink_to_fit();

	Rvd->Close();

	Lit->Close();

	Sca->Close();

	Blk->Close();

	Flr->Close();

	for (auto& Element : ScdX0)
	{
		Element.clear();
		Element.shrink_to_fit();
	}
	ScdX0.clear();
	ScdX0.shrink_to_fit();

	for (auto& Element : ScdX1)
	{
		Element.clear();
		Element.shrink_to_fit();
	}
	ScdX1.clear();
	ScdX1.shrink_to_fit();

	for (auto& Element : Scd0)
	{
		Element.clear();
		Element.shrink_to_fit();
	}
	Scd0.clear();
	Scd0.shrink_to_fit();

	for (auto& Element : Scd1)
	{
		Element.clear();
		Element.shrink_to_fit();
	}
	Scd1.clear();
	Scd1.shrink_to_fit();

	for (auto& Element : ScdEv0)
	{
		Element.clear();
		Element.shrink_to_fit();
	}
	ScdEv0.clear();
	ScdEv0.shrink_to_fit();

	for (auto& Element : ScdEv1)
	{
		Element.clear();
		Element.shrink_to_fit();
	}
	ScdEv1.clear();
	ScdEv1.shrink_to_fit();

	for (auto& Element : Message)
	{
		Element.clear();
		Element.shrink_to_fit();
	}
	Message.clear();
	Message.shrink_to_fit();

	for (auto& Element : MessageSub)
	{
		Element.clear();
		Element.shrink_to_fit();
	}
	MessageSub.clear();
	MessageSub.shrink_to_fit();

	for (auto& Element : Ipix)
	{
		if (Element)
		{
			Element->Close();
			Element.reset();
		}
	}
	Ipix.clear();
	Ipix.shrink_to_fit();

	if (Scrl) { Scrl->Close(); }

	for (auto& Element : Object)
	{
		Element->Close();
		Element.reset();
	}
	Object.clear();
	Object.shrink_to_fit();

	for (auto& Element : Item)
	{
		Element->Close();
		Element.reset();
	}
	Item.clear();
	Item.shrink_to_fit();

	Esp->Close();

	Rbj->Close();

	Edt0->Close();
	Edt1->Close();
	Vab0->CloseVAB();
	Vab1->CloseVAB();
}

void Resident_Evil_Room::SetEditor(Room_Editor_Type Type)
{
	ResetEditor();

	if (Type == Room_Editor_Type::MODEL)
	{
		b_EditModel = true;

		for (size_t i = 0; i < Item.size(); i++) { Item[i]->b_EditorMode = true; }
		for (size_t i = 0; i < Object.size(); i++) { Object[i]->b_EditorMode = true; }
	}

	if (Type == Room_Editor_Type::ESP)
	{
		b_EditEffect = true;
		Esp->iTime.store(0);
		Esp->iSSeq.fetch_add(1);
		Esp->m_FrameCounter = 0;
	}
}

void Resident_Evil_Room::ResetEditor(void)
{
	b_EditModel = false;
	b_EditEffect = false;

	b_EditorItem = b_EditorObject = false;
	iItem = iItemMin = iItemMax = 0;
	iObject = iObjectMin = iObjectMax = 0;

	iEffect = iEffectMin = iEffectMax = 0;
	iEffectGp = iEffectGpMin = iEffectGpMax = 0;
	iEffectSSeq = iEffectSSeqMin = iEffectSSeqMax = 0;
	iEffectMSeq = iEffectMSeqMin = iEffectMSeqMax = 0;
	iEffectMSeqID = iEffectMSeqIDMin = iEffectMSeqIDMax = 0;
	iEffectClutID = iEffectClutIDMin = iEffectClutIDMax = 0;

	for (size_t i = 0; i < Item.size(); i++) { Item[i]->b_EditorMode = false; }
	for (size_t i = 0; i < Object.size(); i++) { Object[i]->b_EditorMode = false; }
}