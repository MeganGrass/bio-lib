/*
*
*	Megan Grass
*	September 21, 2024
*
*/


#include "bio_effect.h"


const bool Resident_Evil_Effect::Open(StdFile& File, std::uintmax_t Count, std::uintmax_t pEsp_hed, std::uintmax_t pEsp_end, std::uintmax_t pEsp_tim, std::uintmax_t pEsp_tim_end)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil Effect Error: could not open ESP at 0x%llX in \"%ws\"", pEsp_hed, File.GetPath().filename().wstring().c_str());
		return false;
	}

	Close();

	EFF_HEADER_BIO1 Header{};

	std::vector<EFF_SSEQUENCE> SSeq{};

	std::vector<EFF_SSEQUENCE_BIO1> SSeqBio1{};

	EFF_MSEQUENCE_INDEX MSeqIdx{};

	EFF_MSEQUENCE_INDEX_BIO1 MSeqIdxBio1{};

	EFF_MSEQUENCE_HEADER MSeqHeader{};

	std::uintmax_t Pointer = 0;

	std::uintmax_t pMSeq = 0;

	std::vector<std::uint8_t> ID(Count);

	File.Read(pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

	for (std::size_t i = 0; i < Count; i++)
	{
		m_Data.push_back(Resident_Evil_EFF{});

		m_Data[i].Id = ID[i];
	}

	for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
	{
		if (GameType() & (AUG95 | OCT95 | BIO1))
		{
			File.Read(pEsp_end - (i * sizeof(std::uint32_t)), &Pointer, sizeof(std::uint32_t));

			if (!IsValidPointer(File, Pointer)) { break; }

			File.Read(Pointer, &Header, sizeof(EFF_HEADER_BIO1));
			Pointer += sizeof(EFF_HEADER_BIO1);

			m_Data[i].Header.nSSequence = Header.nSSequence;
			m_Data[i].Header.nSpriteGp = Header.nSpriteGp;
			m_Data[i].Header.CBA = Header.CBA;
			m_Data[i].Header.reserved = Header.reserved;

			SSeqBio1.resize(m_Data[i].Header.nSSequence);
			File.Read(Pointer, SSeqBio1.data(), SSeqBio1.size() * sizeof(EFF_SSEQUENCE_BIO1));
			Pointer += SSeqBio1.size() * sizeof(EFF_SSEQUENCE_BIO1);

			m_Data[i].SSequence.resize(m_Data[i].Header.nSSequence);
			for (std::size_t x = 0; x < SSeqBio1.size(); x++)
			{
				m_Data[i].SSequence[x].pGp = SSeqBio1[x].pGp;
				m_Data[i].SSequence[x].nSpr = 1;
				m_Data[i].SSequence[x].Time = SSeqBio1[x].Time;
				m_Data[i].SSequence[x].Width = SSeqBio1[x].Width;
				m_Data[i].SSequence[x].Height = SSeqBio1[x].Height;
				m_Data[i].SSequence[x].Hotx = 0;
				m_Data[i].SSequence[x].Hoty = 0;
			}

			m_Data[i].SpriteGp.resize(m_Data[i].Header.nSpriteGp);
			File.Read(Pointer, m_Data[i].SpriteGp.data(), m_Data[i].SpriteGp.size() * sizeof(EFF_SPRITEGp));
			Pointer += m_Data[i].SpriteGp.size() * sizeof(EFF_SPRITEGp);

			m_Data[i].MSequence.resize(MSEQ_IDX_MAX);
			File.Read(Pointer, &MSeqIdxBio1, sizeof(EFF_MSEQUENCE_INDEX_BIO1));
		}
		else
		{
			File.Read(pEsp_end - (i * sizeof(std::uint32_t)), &Pointer, sizeof(std::uint32_t));
			Pointer += pEsp_hed;

			if (!IsValidPointer(File, Pointer)) { break; }

			File.Read(Pointer, &m_Data[i].Header, sizeof(EFF_HEADER));
			Pointer += sizeof(EFF_HEADER);

			SSeq.resize(m_Data[i].Header.nSSequence);
			File.Read(Pointer, SSeq.data(), SSeq.size() * sizeof(EFF_SSEQUENCE));
			Pointer += SSeq.size() * sizeof(EFF_SSEQUENCE);

			m_Data[i].SSequence.resize(m_Data[i].Header.nSSequence);
			for (std::size_t x = 0; x < SSeq.size(); x++)
			{
				m_Data[i].SSequence[x].pGp = SSeq[x].pGp;
				m_Data[i].SSequence[x].nSpr = SSeq[x].nSpr;
				m_Data[i].SSequence[x].Time = SSeq[x].Time;
				m_Data[i].SSequence[x].Width = SSeq[x].Twh;
				m_Data[i].SSequence[x].Height = SSeq[x].Twh;
				m_Data[i].SSequence[x].Hotx = SSeq[x].Hotx;
				m_Data[i].SSequence[x].Hoty = SSeq[x].Hoty;
			}

			m_Data[i].SpriteGp.resize(m_Data[i].Header.nSpriteGp);
			File.Read(Pointer, m_Data[i].SpriteGp.data(), m_Data[i].SpriteGp.size() * sizeof(EFF_SPRITEGp));
			Pointer += m_Data[i].SpriteGp.size() * sizeof(EFF_SPRITEGp);

			m_Data[i].MSequence.resize(MSEQ_IDX_MAX);
			File.Read(Pointer, &MSeqIdx, sizeof(EFF_MSEQUENCE_INDEX));
		}

		for (std::size_t x = 0; x < MSEQ_IDX_MAX; x++)
		{
			if (!MSeqIdx.pMSequence[x]) { break; }

			pMSeq = Pointer + static_cast<std::uintmax_t>(MSeqIdx.pMSequence[x] * sizeof(std::uint32_t));

			File.Read(pMSeq, &MSeqHeader, sizeof(EFF_MSEQUENCE_HEADER));
			pMSeq += sizeof(EFF_MSEQUENCE_HEADER);

			m_Data[i].MSequence[x].resize(MSeqHeader.nMSequence);

			File.Read(pMSeq, m_Data[i].MSequence[x].data(), m_Data[i].MSequence[x].size() * sizeof(EFF_MSEQUENCE));
		}
	}

	for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
	{
		if (GameType() & (AUG95 | OCT95 | BIO1))
		{
			File.Read(pEsp_tim_end - (i * sizeof(std::uint32_t)), &Pointer, sizeof(std::uint32_t));
		}
		else
		{
			File.Read(pEsp_tim_end - (i * sizeof(std::uint32_t)) - sizeof(std::uint32_t), &Pointer, sizeof(std::uint32_t));
			Pointer += pEsp_tim;
		}

		if (!IsValidPointer(File, Pointer)) { break; }

		m_Data[i].Tim = std::make_unique<Sony_PlayStation_Texture>();

		m_Data[i].Tim->Str.hWnd = Str.hWnd;

		m_Data[i].Tim->OpenTIM(File, Pointer);

#if MSTD_DX9
		ExportDX9(i);
#endif
	}

	return true;
}

void Resident_Evil_Effect::Close(void)
{
	for (auto& Element : m_Data)
	{
		Element.Position = { 0, 0, 0 };

		Element.SSequence.clear();
		Element.SSequence.shrink_to_fit();

		Element.SpriteGp.clear();
		Element.SpriteGp.shrink_to_fit();

		for (auto& MSequence : Element.MSequence)
		{
			MSequence.clear();
			MSequence.shrink_to_fit();
		}

		Element.MSequence.clear();
		Element.MSequence.shrink_to_fit();

		if (Element.Tim)
		{
			Element.Tim->Close();
			Element.Tim.reset();
		}

		Element.TextureWidth = 0.0f;
		Element.TextureHeight = 0.0f;

#if MSTD_DX9
		for (auto& Texture : Element.Texture)
		{
			Texture.reset();
			Texture = nullptr;
		}
#endif
	}

	b_Play.store(true);
	b_Loop.store(true);
	b_PlayInReverse.store(false);
	iSSeq.store(0);
	iTime.store(0);
	m_FrameCounter = 0;

	m_Data.clear();
	m_Data.shrink_to_fit();
}

#if MSTD_DX9
void Resident_Evil_Effect::ExportDX9(std::size_t iElement)
{
	if (iElement >= m_Data.size()) { return; }

	auto& Effect = m_Data[iElement];

	if (Effect.Id == 0xFF || !Effect.Tim || !Effect.Tim->IsOpen()) { return; }

	if (Effect.Tim->GetPaletteCount())
	{
		Effect.Texture.resize(Effect.Tim->GetPaletteCount());
		for (uint16_t i = 0; i < Effect.Tim->GetPaletteCount(); ++i)
		{
			Effect.Texture[i].reset(Render->CreateTexture(Effect.Tim, i, Sony_Texture_Transparency::Superimposed, 0xFF00FF, true));
		}
	}
	else
	{
		Effect.Texture.resize(1);
		Effect.Texture[0].reset(Render->CreateTexture(Effect.Tim, 0, Sony_Texture_Transparency::Superimposed, 0xFF00FF, true));
	}

	auto NextPowerOfTwo = [](int32_t x) {
		if (x <= 0) { return 1; }
		x--;
		for (int Shift = 1; Shift < 32; Shift <<= 1) { x |= x >> Shift; }
		return x + 1;
		};

	Effect.TextureWidth = static_cast<float>(NextPowerOfTwo(static_cast<int>(Effect.Tim->GetWidth())));
	Effect.TextureHeight = static_cast<float>(NextPowerOfTwo(static_cast<int>(Effect.Tim->GetHeight())));
}
#endif