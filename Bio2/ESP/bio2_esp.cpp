/*
*
*	Megan Grass
*	September 4, 2025
*
*/


#include "bio2_esp.h"


const bool Resident_Evil_2_Effect::Open(StdFile& File, std::uintmax_t Count, std::uintmax_t pEsp_hed, std::uintmax_t pEsp_end, std::uintmax_t pEsp_tim, std::uintmax_t pEsp_tim_end)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil 2 Error: could not open ESP at 0x%llX in \"%ws\"", pEsp_hed, File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (b_Open.load()) { Close(); }

	EFF_MSEQUENCE_INDEX MSeqIdx{};

	EFF_MSEQUENCE_HEADER MSeqHeader{};

	std::uintmax_t Pointer = 0;

	std::uintmax_t pMSeq = 0;

	std::vector<std::uint8_t> ID(Count);

	File.Read(pEsp_hed, ID.data(), ID.size() * sizeof(std::uint8_t));

	for (std::size_t i = 0; i < Count; i++)
	{
		m_Data.push_back(Resident_Evil_2_EFF{});

		m_Data[i].Id = ID[i];
	}

	for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
	{
		File.Read(pEsp_end - (i * sizeof(std::uint32_t)), &Pointer, sizeof(std::uint32_t));
		Pointer += pEsp_hed;

		if (!IsValidPointer(File, Pointer)) { break; }

		File.Read(Pointer, &m_Data[i].Header, sizeof(EFF_HEADER));
		Pointer += sizeof(EFF_HEADER);

		m_Data[i].SSequence.resize(m_Data[i].Header.nSSequence);
		File.Read(Pointer, m_Data[i].SSequence.data(), m_Data[i].SSequence.size() * sizeof(EFF_SSEQUENCE));
		Pointer += m_Data[i].SSequence.size() * sizeof(EFF_SSEQUENCE);

		m_Data[i].SpriteGp.resize(m_Data[i].Header.nSpriteGp);
		File.Read(Pointer, m_Data[i].SpriteGp.data(), m_Data[i].SpriteGp.size() * sizeof(EFF_SPRITEGp));
		Pointer += m_Data[i].SpriteGp.size() * sizeof(EFF_SPRITEGp);

		m_Data[i].MSequence.resize(MSEQ_IDX_MAX);
		File.Read(Pointer, &MSeqIdx, sizeof(EFF_MSEQUENCE_INDEX));

		for (std::size_t x = 0; x < MSEQ_IDX_MAX; x++)
		{
			if (!MSeqIdx.pMSequence[x]) { break; }

			pMSeq = Pointer + static_cast<std::uintmax_t>(MSeqIdx.pMSequence[x] * 4);

			File.Read(pMSeq, &MSeqHeader, sizeof(EFF_MSEQUENCE_HEADER));
			pMSeq += sizeof(EFF_MSEQUENCE_HEADER);

			m_Data[i].MSequence[x].resize(MSeqHeader.nMSequence);

			File.Read(pMSeq, m_Data[i].MSequence[x].data(), m_Data[i].MSequence[x].size() * sizeof(EFF_MSEQUENCE));
		}
	}

	for (std::size_t i = 0; i < ID.size() && ID[i] != 0xFF; i++)
	{
		File.Read(pEsp_tim_end - (i * sizeof(std::uint32_t)) - sizeof(std::uint32_t), &Pointer, sizeof(std::uint32_t));
		Pointer += pEsp_tim;

		if (!IsValidPointer(File, Pointer)) { break; }

		m_Data[i].Tim = std::make_shared<Sony_PlayStation_Texture>();

		m_Data[i].Tim->Str.hWnd = Str.hWnd;

		m_Data[i].Tim->OpenTIM(File, Pointer);
	}

	return true;
}

void Resident_Evil_2_Effect::Close(void)
{
	for (auto& Element : m_Data)
	{
		Element.SSequence.clear();
		Element.SSequence.shrink_to_fit();

		Element.SpriteGp.clear();
		Element.SpriteGp.shrink_to_fit();

		for (auto& i : Element.MSequence)
		{
			i.clear();
			i.shrink_to_fit();
		}

		Element.MSequence.clear();
		Element.MSequence.shrink_to_fit();

		if (Element.Tim)
		{
			Element.Tim->Close();
			Element.Tim.reset();
		}
	}

	m_Data.clear();
	m_Data.shrink_to_fit();
}