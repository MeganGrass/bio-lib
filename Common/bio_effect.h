/*
*
*	Megan Grass
*	September 21, 2024
*
*/


#pragma once

#include <bio_common.h>

#include <sony_texture.h>

#include <gte/lib_gte.h>

#include <esp/bio1_esp.h>

#include <esp/bio2_esp.h>

#if MSTD_DX9
#include <std_dx9.h>
#endif


constexpr auto EFF_IDX_MAX = 8;

constexpr auto MSEQ_IDX_MAX = 8;

struct EFF_SSEQUENCE_CUSTOM
{
	std::uint8_t pGp;						// sprite group id
	std::uint8_t nSpr;						// total count of sprite groups in this sequence
	std::uint8_t Time;						// total count of frames to display this sprite (0xFF = terminator)
	std::uint8_t Width;						// sprite width
	std::uint8_t Height;					// sprite height
	std::int16_t Hotx;						// hotspot X
	std::int16_t Hoty;						// hotspot Y
};


struct Resident_Evil_EFF
{
	std::uint8_t Id;
	EFF_HEADER Header;
	std::vector<EFF_SSEQUENCE_CUSTOM> SSequence;
	std::vector<EFF_SPRITEGp> SpriteGp;
	std::vector<std::vector<EFF_MSEQUENCE>> MSequence;
	std::unique_ptr<Sony_PlayStation_Texture> Tim;
	VECTOR2 Position;
#if MSTD_DX9
	float TextureWidth{};
	float TextureHeight{};
	std::vector<std::unique_ptr<IDirect3DTexture9, IDirect3DDelete9<IDirect3DTexture9>>> Texture;
#endif
};


class Resident_Evil_Effect :
	protected Resident_Evil_Common {
private:

	std::vector<Resident_Evil_EFF> m_Data;

	std::shared_ptr<Standard_Matrix> World;

public:

	Standard_String Str;

	using Resident_Evil_Common::m_Game;
	using Resident_Evil_Common::GameType;

	Resident_Evil_Effect(void) :
		m_FrameCounter(0)
	{
		b_Play.store(true);
		b_Loop.store(true);
		b_PlayInReverse.store(false);
		iSSeq.store(0);
		iTime.store(0);
		SetGame(Video_Game::Resident_Evil_2);
	}

	~Resident_Evil_Effect(void) = default;

	// Is sequence processing active?
	std::atomic<bool> b_Play;

	// Will sequence processing loop?
	std::atomic<bool> b_Loop;

	// Will sequence process in reverse?
	std::atomic<bool> b_PlayInReverse;

	// Sprite Sequence Index
	std::atomic<std::size_t> iSSeq;

	// Timer
	std::atomic<std::size_t> iTime;

	// Frame Counter
	std::size_t m_FrameCounter;

	// Get data count
	constexpr std::size_t Count(void) const { return m_Data.size(); }

	// Get data
	[[nodiscard]] std::vector<Resident_Evil_EFF>& Data(void) { return m_Data; }

	// Get data element
	[[nodiscard]] Resident_Evil_EFF& Get(const std::size_t& iElement) { return m_Data[iElement]; }

	// Open ESP container
	const bool Open(StdFile& File, std::uintmax_t Count, std::uintmax_t pEsp_hed, std::uintmax_t pEsp_end, std::uintmax_t pEsp_tim, std::uintmax_t pEsp_tim_end);

	// Save ESP container
	const bool Save(StdFile& File, std::uintmax_t _Ptr);

	// Save ESP container
	const bool Save(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	// Close
	void Close(void);

#if MSTD_DX9
	std::shared_ptr<Standard_DirectX_9> Render;

	void ExportDX9(std::size_t iElement);
#endif

};