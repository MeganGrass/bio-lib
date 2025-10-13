/*
*
*	Megan Grass
*	October 10, 2025
*
*/


#pragma once

#include <sony_texture.h>

#include <bio_common.h>

#if MSTD_DX9
#include <std_dx9.h>
#endif


#pragma pack(push, 1)

static constexpr int8_t Resident_Evil_2_Glyph_Metric[264] =
{
	0, 8, 2, 5, 4, 7, 4, 6,
	5, 6, 5, 7, 3, 7, 3, 8,
	4, 7, 6, 8, 1, 8, 2, 10,
	1, 12, 1, 12, 1, 12, 1, 12,
	1, 12, 1, 12, 1, 12, 1, 12,
	1, 12, 1, 12, 4, 7, 0, 14,
	2, 5, 2, 11, 4, 6, 2, 10,
	1, 13, 0, 14, 1, 13, 1, 12,
	1, 12, 1, 12, 2, 11, 1, 13,
	0, 14, 4, 7, 1, 13, 1, 13,
	1, 13, 0, 15, 0, 14, 1, 12,
	1, 12, 0, 13, 0, 14, 1, 12,
	1, 13, 1, 13, 1, 13, 0, 15,
	1, 13, 1, 13, 1, 13, 3, 8,
	0, 15, 3, 8, 4, 7, 0, 14,
	0, 14, 2, 10, 2, 10, 3, 9,
	2, 10, 3, 9, 3, 9, 2, 10,
	2, 11, 4, 6, 4, 6, 2, 11,
	4, 6, 0, 14, 2, 11, 2, 10,
	2, 10, 2, 10, 3, 8, 3, 9,
	3, 8, 2, 11, 2, 11, 0, 14,
	2, 11, 2, 11, 3, 9, 0, 0,
	2, -68, 70, 0, 0, 0, 3, -110,
	42, 0, 2, 0, 6, -124, 28, 0,
	5, 0, 2, -54, 42, 0, 11, 0,
	3, -124, 42, 0, 13, 0, 3, 90,
	70, 0, 16, 0, 2, 90, 98, 0,
	19, 0, 0, 0, 2, -46, 50, 0,
	0, 0, 3, 126, 70, 0, 2, 0,
	6, -124, 28, 0, 5, 0, 2, -60,
	42, 0, 11, 0, 3, 90, 70, 0,
	13, 0, 3, 112, 56, 0, 16, 0,
	2, 48, -112, 0, 19, 0, 0, 0
};

static constexpr uint8_t Resident_Evil_2_Message_Null[5] =
{
	0x4A, 0x51, 0x48, 0x48, 0xF7
};

#pragma pack(pop)


enum class FontColor : std::uint8_t
{
	White, Green, Red, Gray, Blue
};


class Resident_Evil_Message :
	private Resident_Evil_Common {
private:

	// Color Constants
	static constexpr uint8_t WHITE = std::to_underlying(FontColor::White);
	static constexpr uint8_t GREEN = std::to_underlying(FontColor::Green);
	static constexpr uint8_t RED = std::to_underlying(FontColor::Red);
	static constexpr uint8_t GRAY = std::to_underlying(FontColor::Gray);
	static constexpr uint8_t BLUE = std::to_underlying(FontColor::Blue);

#ifdef MSTD_DX9

	// Font Glyph Texture (static)
	std::vector<std::unique_ptr<IDirect3DTexture9, IDirect3DDelete9<IDirect3DTexture9>>> m_Stage0;

	// Font Glyph Texture (dynamic)
	std::vector<std::unique_ptr<IDirect3DTexture9, IDirect3DDelete9<IDirect3DTexture9>>> m_StageX;

#endif

	// Move
	Resident_Evil_Message(Resident_Evil_Message&&) = delete;
	Resident_Evil_Message& operator = (Resident_Evil_Message&&) = delete;

	// Copy
	Resident_Evil_Message(const Resident_Evil_Message&) = delete;
	Resident_Evil_Message& operator = (const Resident_Evil_Message&) = delete;

	// Is the canvas blank?
	std::atomic<bool> b_BlankCanvas;

	// Static font glyph texture loaded?
	std::atomic<bool> b_Static;

	// Dynamic font glyph texture loaded?
	std::atomic<bool> b_Dynamic;

	// Have all characters been printed?
	std::atomic<bool> b_Complete;

	// Pause for Controller Input?
	std::atomic<bool> b_Controller;

	// Total Count of Printed Characters
	std::atomic<size_t> m_PrintCount;

public:

	explicit Resident_Evil_Message(void)
	{
		b_BlankCanvas.store(true);
		b_Static.store(false);
		b_Dynamic.store(false);
		b_Complete.store(false);
		b_Controller.store(false);
		m_PrintCount.store(0);
	}

	~Resident_Evil_Message(void) = default;

#ifdef MSTD_DX9
	explicit Resident_Evil_Message(std::shared_ptr<Standard_DirectX_9> render) : Render(render)
	{
		b_BlankCanvas.store(true);
		b_Static.store(false);
		b_Dynamic.store(false);
		b_Complete.store(false);
		b_Controller.store(false);
		m_PrintCount.store(0);
	}

	// Direct-X 9 Render Context
	std::shared_ptr<Standard_DirectX_9> Render;

	// Render Texture
	std::unique_ptr<IDirect3DTexture9, IDirect3DDelete9<IDirect3DTexture9>> m_Canvas;
#endif

	// Messages/Debugging
	Standard_String Str;

	// Create Render Texture
	void ResetCanvas(void);

	// Reset Print Count
	void ResetPrintCount(void) noexcept { m_PrintCount.store(0); }

	// Is the canvas blank?
	const bool IsBlank(void) const noexcept { return b_BlankCanvas.load(); }

	// Have all characters been printed?
	const bool IsComplete(void) const noexcept { return b_Complete.load(); }

	// Is the static font glyph texture loaded?
	const bool IsStatic(void) const noexcept { return b_Static.load(); }

	// Is the dynamic font glyph texture loaded?
	const bool IsDynamic(void) const noexcept { return b_Dynamic.load(); }

	// Total Count of Printed Characters
	const size_t PrintCount(void) const noexcept { return m_PrintCount.load(); }

	// Pause for Controller Input?
	const bool ControllerPause(void) const noexcept { return b_Controller.load(); }

	/*
		Pause for Controller Input?
		- halt message parsing when 0xFD opcode is encountered
	*/
	void SetControllerPause(const bool Pause) noexcept { b_Controller.store(Pause); }

	/*
		Open Stage Font Texture
		 - two textures (max)
		 - Stage 0, static
		 - Stage x, dynamic (stage specific)
	*/
	const bool OpenFont(std::filesystem::path Path, std::uintmax_t Stage);

	/*
		Print Message (Resident Evil 2)
		 - glyphs are printed to render texture (canvas)
	*/
	void Print(std::int32_t Pos_x, std::int32_t Pos_y, std::uint32_t Attr, std::vector<std::uint8_t>& Message, size_t pStart = 0);

	// Shutdown
	void Shutdown(void);

};