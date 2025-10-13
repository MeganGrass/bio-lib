/*
*
*	Megan Grass
*	October 10, 2025
*
*/


#include "bio_message.h"


void Resident_Evil_Message::ResetCanvas(void)
{
#ifdef MSTD_DX9
	m_Canvas.reset(Render->CreateTexture(512, 256, D3DUSAGE_DYNAMIC));
#endif

	b_BlankCanvas.store(true);

	b_Complete.store(false);
}

const bool Resident_Evil_Message::OpenFont(std::filesystem::path Path, std::uintmax_t Stage)
{
	std::unique_ptr<Sony_PlayStation_Texture> Texture = std::make_unique<Sony_PlayStation_Texture>();

	Texture->Str.hWnd = Str.hWnd;

	Texture->OpenTIM(Path);

	if (!Texture->IsOpen())
	{
		Str.Message(L"Resident Evil Stage Font Error: could not open \"%ws\"", Path.filename().wstring().c_str());
		return false;
	}

#ifdef MSTD_DX9
	if (Stage == 0)
	{
		if (Texture->GetPaletteCount())
		{
			m_Stage0.resize(Texture->GetPaletteCount());
			for (uint16_t i = 0; i < Texture->GetPaletteCount(); i++)
			{
				m_Stage0[i].reset(Render->CreateTexture(Texture, i, Sony_Texture_Transparency::Superblack, 0xFF00FF, true));
			}
		}
		else
		{
			m_Stage0.resize(1);
			m_Stage0[0].reset(Render->CreateTexture(Texture, 0, Sony_Texture_Transparency::Superblack, 0xFF00FF, true));
		}

		b_Static.store(true);
	}
	else
	{
		if (Texture->GetPaletteCount())
		{
			m_StageX.resize(Texture->GetPaletteCount());
			for (uint16_t i = 0; i < Texture->GetPaletteCount(); i++)
			{
				m_StageX[i].reset(Render->CreateTexture(Texture, i, Sony_Texture_Transparency::Superblack, 0xFF00FF, true));
			}
		}
		else
		{
			m_StageX.resize(1);
			m_StageX[0].reset(Render->CreateTexture(Texture, 0, Sony_Texture_Transparency::Superblack, 0xFF00FF, true));
		}

		b_Dynamic.store(true);
	}
#endif

	return true;
}

void Resident_Evil_Message::Print(std::int32_t Pos_x, std::int32_t Pos_y, std::uint32_t Attr, std::vector<std::uint8_t>& Message, size_t pStart)
{
	constexpr int32_t WH = 14;
	constexpr int32_t GPR = 18;
	constexpr int32_t RowY_Page0 = -46;
	constexpr int32_t RowY_Page1 = -60;
	constexpr int32_t RowY_English = 28;

	m_PrintCount.store(0);

	if (Message.empty() || m_Stage0.empty() || m_StageX.empty()) { b_Complete.store(true); return; }

	uint32_t TexturePage = 0;

	uint8_t Color = WHITE;

	int32_t StartX = Pos_x, StartY = Pos_y;

	int32_t SrcX = 0, SrcY = 0;

	uint8_t* pMess = &Message.data()[pStart];

	uint8_t Op = *pMess;

	auto PrintGlyph = [&]()
		{
#ifdef MSTD_DX9
			if (TexturePage == 0)
			{
				Render->BlitTexture2(m_Stage0[Color].get(), m_Canvas.get(), SrcX, SrcY, WH, WH, Pos_x, Pos_y, false);
			}
			else
			{
				Render->BlitTexture2(m_StageX[Color].get(), m_Canvas.get(), SrcX, SrcY, WH, WH, Pos_x, Pos_y, false);
			}
#endif
			b_BlankCanvas.store(false);
		};

	while (pMess < Message.data() + Message.size())
	{
		int32_t Size = WH;

		TexturePage = 0;

		if (Op == 0x00)
		{
			if (Attr & 0x4000)
			{
				Pos_x -= static_cast<int32_t>(Resident_Evil_2_Glyph_Metric[0]);

				Size = static_cast<int32_t>(Resident_Evil_2_Glyph_Metric[1]) + static_cast<int32_t>(Resident_Evil_2_Glyph_Metric[0]);
			}

			Pos_x += Size;

			Op = *++pMess;

			continue;
		}

		switch (Op)
		{

		case 0xEE:	// Japanese Characters, Texture Page 0
		{
			Op = *++pMess;

			int32_t Row = static_cast<int32_t>(Op) / GPR;

			SrcX = (Op % GPR) * WH;

			SrcY = Row * WH + RowY_Page0;

			PrintGlyph();

			Pos_x += Size;

			Op = *++pMess;

			continue;
		}

		case 0xEF:	// Japanese Characters, Texture Page 1
		{
			TexturePage = 1;

			Op = *++pMess;

			int32_t Row = static_cast<int32_t>(Op) / GPR;

			SrcX = (Op % GPR) * WH;

			SrcY = Row * WH;

			PrintGlyph();

			Pos_x += Size;

			Op = *++pMess;

			continue;
		}

		case 0xF0:	// Japanese Characters, Texture Page 1
		{
			TexturePage = 1;

			Op = *++pMess;

			int32_t Row = static_cast<int32_t>(Op) / GPR;

			SrcX = (Op % GPR) * WH;

			SrcY = Row * WH + RowY_Page1;

			PrintGlyph();

			Pos_x += Size;

			Op = *++pMess;

			continue;
		}

		case 0xF5:	// Empty Space (Reserved)
		{
			Op = *++pMess;
			continue;
		}

		case 0xF6:	// Space
		{
			Pos_x += WH / 2;

			Op = *++pMess;

			continue;
		}

		case 0xF7:	// End of String
		{
			m_PrintCount = static_cast<size_t>(++pMess - &Message.data()[0]);

			b_Complete.store(true);

			return;
		}

		case 0xF8:	// Item Name
		{
			Op = *++pMess;
			Op = *++pMess;
			{
				std::vector<std::uint8_t> NullMsg(Resident_Evil_2_Message_Null, Resident_Evil_2_Message_Null + 5);
				//Print(Pos_x, Pos_y, Attr, NullMsg, 0);
			}
			continue;
		}

		case 0xF9:	// Color Change
		{
			Op = *++pMess;

			Color = Op;

			Op = *++pMess;

			continue;
		}

		case 0xFA:	// Message Begin
		{
			Op = *++pMess;
			Op = *++pMess;	// non-vanilla
			continue;
		}

		case 0xFB:	// Branch Message (Yes, No, etc.)
		{
			Op = *++pMess;
			Op = *++pMess;
			continue;
		}

		case 0xFC:	// Newline
		{
			Pos_y += 16;

			Pos_x = StartX;

			Op = *++pMess;

			continue;
		}

		case 0xFD:	// Pause for Controller Input
		{
			Pos_y = StartY;

			Op = *++pMess;

			m_PrintCount = static_cast<size_t>(++pMess - &Message.data()[0]);

			if (b_Controller.load())
			{
				return;
			}
			else
			{
				continue;
			}
		}

		case 0xFE:	// End of Message
		{
			m_PrintCount = static_cast<size_t>(++pMess - &Message.data()[0]);

			b_Complete.store(true);

			return;
		}

		default:
		{
			int32_t Row = static_cast<int32_t>(Op) / GPR;

			SrcX = (Op % GPR) * WH;

			SrcY = Row * WH + RowY_English;

			if (Attr & 0x4000)
			{
				if (Op < 0x57)
				{
					uint32_t idx = static_cast<uint32_t>(Op) * 2;

					Pos_x -= static_cast<int32_t>(Resident_Evil_2_Glyph_Metric[idx]);

					Size = static_cast<int32_t>(Resident_Evil_2_Glyph_Metric[idx + 1]) + static_cast<int32_t>(Resident_Evil_2_Glyph_Metric[idx]);
				}
			}

			PrintGlyph();

			Pos_x += Size;

			Op = *++pMess;

			continue;
		}

		}

	}
}

void Resident_Evil_Message::Shutdown(void)
{
#ifdef MSTD_DX9
	for (auto& Tex : m_Stage0)
	{
		Tex.reset();
	}
	for (auto& Tex : m_StageX)
	{
		Tex.reset();
	}
	m_Canvas.reset();
#endif
}