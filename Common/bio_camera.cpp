/*
*
*	Megan Grass
*	April 16, 2024
*
*/


#include <bio_camera.h>


void Resident_Evil_Camera::Shutdown(void) noexcept
{
	b_ViewBackground = false;
	b_ViewSprite = false;
	b_ViewTopDown = false;
	b_DrawLine = false;
	b_DrawSwitch = false;
	b_ViewEditor = false;

	m_Background.reset();
	m_Sprite.reset();
	m_BackgroundVert.reset();
}

void Resident_Evil_Camera::Reset(void)
{
	m_Cut = 0;
	m_CutMax = 0;
	m_Path.clear();
	m_FOV = (0x6DD4 >> 7);
	m_Eye = { -16000, -7200, -16000 };
	m_At = { 0, 7200, 0 };
	m_TexWidth = 0.0f;
	m_TexHeight = 0.0f;
	m_TexSprWidth = 0.0f;
	m_TexSprHeight = 0.0f;
	m_Cx = 0.0f;
	m_Cy = 50.0f;
	m_Cz = 0.0f;
	m_Background.reset(nullptr);
	m_Sprite.reset(nullptr);
	Set(m_FOV, m_Eye, m_At);
}

void Resident_Evil_Camera::SetMeta(std::filesystem::path Path, std::uintmax_t Stage, std::uintmax_t Room, std::uintmax_t CutMax, Video_Game Game) noexcept
{
	m_Path = Path;
	m_Game = Game;
	m_Stage = Stage;
	m_Room = Room;
	m_CutMax = CutMax;
}

void Resident_Evil_Camera::SetOrtho(float Width, float Height)
{
	m_OrthoWidth = Width;
	m_OrthoHeight = Height;
	m_OrthoScaleX = m_OrthoWidth / m_NativeWidth;
	m_OrthoScaleY = m_OrthoHeight / m_NativeHeight;
	Orthogonal->OrthogonalOffCenterLeft(0.0f, m_OrthoWidth, 0.0f, m_OrthoHeight, 0.0f, 1.0f);
}

std::uintmax_t Resident_Evil_Camera::SetImage(std::uintmax_t iCut)
{
	if (m_CutMax) { m_Cut = std::clamp(iCut, static_cast<uintmax_t>(0), static_cast<uintmax_t>(m_CutMax - 1)); }
	else { m_Cut = 0; }

	m_TexWidth = 0;
	m_TexHeight = 0;

	m_TexSprWidth = 0;
	m_TexSprHeight = 0;

	std::unique_ptr<Sony_PlayStation_Texture> TIM = std::make_unique<Sony_PlayStation_Texture>();
	std::unique_ptr<Standard_Image> Image = std::make_unique<Standard_Image>();
	Image->Str.hWnd = Str.hWnd;

	std::filesystem::path BackgroundTIM = Str.FormatCStyle(L"%ws\\ROOM%d%02x%02d.tim", m_Path.wstring().c_str(), m_Stage, m_Room, m_Cut);
	std::filesystem::path BackgroundBMP = Str.FormatCStyle(L"%ws\\ROOM%d%02x%02d.bmp", m_Path.wstring().c_str(), m_Stage, m_Room, m_Cut);
	std::filesystem::path BackgroundPNG = Str.FormatCStyle(L"%ws\\ROOM%d%02x%02d.png", m_Path.wstring().c_str(), m_Stage, m_Room, m_Cut);
	std::filesystem::path BackgroundJPG = Str.FormatCStyle(L"%ws\\ROOM%d%02x%02d.jpg", m_Path.wstring().c_str(), m_Stage, m_Room, m_Cut);

	std::filesystem::path SpriteTIM = Str.FormatCStyle(L"%ws\\ROOM_%d%02x_%02d_mask.tim", m_Path.wstring().c_str(), m_Stage, m_Room, m_Cut);
	std::filesystem::path SpriteBMP = Str.FormatCStyle(L"%ws\\ROOM_%d%02x_%02d_mask.bmp", m_Path.wstring().c_str(), m_Stage, m_Room, m_Cut);
	std::filesystem::path SpritePNG = Str.FormatCStyle(L"%ws\\ROOM_%d%02x_%02d_mask.png", m_Path.wstring().c_str(), m_Stage, m_Room, m_Cut);
	std::filesystem::path SpriteJPG = Str.FormatCStyle(L"%ws\\ROOM_%d%02x_%02d_mask.jpg", m_Path.wstring().c_str(), m_Stage, m_Room, m_Cut);

	if (Standard_FileSystem().Exists(BackgroundTIM) && TIM->OpenTIM(BackgroundTIM))
	{
		m_TexWidth = static_cast<float>(TIM->GetWidth());
		m_TexHeight = static_cast<float>(TIM->GetHeight());
		Image = TIM->ExportImage();
		TIM->Close();
#if MSTD_DX9
		m_Background.reset(Render->CreateTexture(Image));
#endif
		Image->Close();
	}

	if (Standard_FileSystem().Exists(BackgroundBMP) && Image->OpenBMP(BackgroundBMP))
	{
		m_TexWidth = static_cast<float>(Image->GetWidth());
		m_TexHeight = static_cast<float>(Image->GetHeight());
#if MSTD_DX9
		m_Background.reset(Render->CreateTexture(Image));
#endif
		Image->Close();
	}

#ifdef LIB_PNG
	else if (Standard_FileSystem().Exists(BackgroundPNG) && Image->OpenPNG(BackgroundPNG))
	{
		m_TexWidth = static_cast<float>(Image->GetWidth());
		m_TexHeight = static_cast<float>(Image->GetHeight());
#if MSTD_DX9
		m_Background.reset(Render->CreateTexture(Image));
#endif
		Image->Close();
	}
#endif

#ifdef LIB_JPEG
	else if (Standard_FileSystem().Exists(BackgroundJPG) && Image->OpenJPEG(BackgroundJPG))
	{
		m_TexWidth = static_cast<float>(Image->GetWidth());
		m_TexHeight = static_cast<float>(Image->GetHeight());
#if MSTD_DX9
		m_Background.reset(Render->CreateTexture(Image));
#endif
		Image->Close();
	}
#endif

	if (Standard_FileSystem().Exists(SpriteTIM) && TIM->OpenTIM(SpriteTIM))
	{
		m_TexSprWidth = static_cast<float>(TIM->GetWidth());
		m_TexSprHeight = static_cast<float>(TIM->GetHeight());
		Image = TIM->ExportImage();
		TIM->Close();
#if MSTD_DX9
		m_Sprite.reset(Render->CreateTexture(Image, true, 0, 0, true));
#endif
		Image->Close();
	}

	if (Standard_FileSystem().Exists(SpriteBMP) && Image->OpenBMP(SpriteBMP))
	{
		m_TexSprWidth = static_cast<float>(Image->GetWidth());
		m_TexSprHeight = static_cast<float>(Image->GetHeight());
#if MSTD_DX9
		m_Sprite.reset(Render->CreateTexture(Image, false, 0, 0, true));
#endif
		Image->Close();
	}

#ifdef LIB_PNG
	else if (Standard_FileSystem().Exists(SpritePNG) && Image->OpenPNG(SpritePNG))
	{
		m_TexSprWidth = static_cast<float>(Image->GetWidth());
		m_TexSprHeight = static_cast<float>(Image->GetHeight());
#if MSTD_DX9
		m_Sprite.reset(Render->CreateTexture(Image, false, 0, 0, true));
#endif
		Image->Close();
	}
#endif

#ifdef LIB_JPEG
	else if (Standard_FileSystem().Exists(SpriteJPG) && Image->OpenJPEG(SpriteJPG))
	{
		m_TexSprWidth = static_cast<float>(Image->GetWidth());
		m_TexSprHeight = static_cast<float>(Image->GetHeight());
#if MSTD_DX9
		m_Sprite.reset(Render->CreateTexture(Image, false, 0, 0, true));
#endif
		Image->Close();
	}
#endif

	return m_Cut;
}

std::vector<vec4t> Resident_Evil_Camera::GetImageVert(void) const
{
	std::vector<vec4t> Vert(4);

#if MSTD_DX9
	float l = -0.5f;
	float r = (m_OrthoWidth - 0.5f);
	float t = 0.5f;
	float b = (m_OrthoHeight + 0.5f);
#else
	float l = 0.0f;
	float r = m_OrthoWidth;
	float t = 0.0f;
	float b = m_OrthoHeight;
#endif
	
	if (b_HorzFlip) { std::swap(l, r); }
	if (b_VertFlip) { std::swap(t, b); }

	Vert[0].vec.Set(l, b, 1.0f, 1.0f);	Vert[0].uv.Set(0.0f, 0.0f);
	Vert[1].vec.Set(r, b, 1.0f, 1.0f);	Vert[1].uv.Set(1.0f, 0.0f);
	Vert[2].vec.Set(l, t, 1.0f, 1.0f);	Vert[2].uv.Set(0.0f, 1.0f);
	Vert[3].vec.Set(r, t, 1.0f, 1.0f);	Vert[3].uv.Set(1.0f, 1.0f);

	return Vert;
}

void Resident_Evil_Camera::SetTopDown(void)
{
	b_ViewTopDown = true;

	View->Identity();
	View->RotateX(View->Radian(90.0f));

	Projection->OrthogonalOffCenterLeft(
		-((m_OrthoWidth / 2.0f) / m_Cy) + m_Cx, ((m_OrthoWidth / 2.0f) / m_Cy) + m_Cx,
		-((m_OrthoHeight / 2.0f) / m_Cy) - m_Cz, ((m_OrthoHeight / 2.0f) / m_Cy) - m_Cz,
		4096.0f, -1.0f
	);

#if MSTD_DX9
	Render->SetWorld(World);
	Render->SetView(View);
	Render->SetProjection(Projection);
#endif
}

MATRIX2 Resident_Evil_Camera::Set_view(VECTOR2 Eye, VECTOR2 At) const
{
	MATRIX2 Mat MATRIX_IDENTITY;
	MATRIX2 World MATRIX_IDENTITY;

	long DeltaX = At.x - Eye.x;
	long DeltaY = At.y - Eye.y;
	long DeltaZ = At.z - Eye.z;

	long Length = GTE->SquareRoot0(DeltaX * DeltaX + DeltaY * DeltaY + DeltaZ * DeltaZ);
	long LengthXZ = GTE->SquareRoot0(DeltaX * DeltaX + DeltaZ * DeltaZ);

	Mat.m22 = (std::uint16_t)((LengthXZ * ONE) / Length);
	Mat.m21 = (std::uint16_t)((DeltaY * ONE) / Length);
	Mat.m12 = -Mat.m21;
	Mat.m11 = Mat.m22;

	GTE->MulMatrix((MATRIX*)&World, (MATRIX*)&Mat);

	if (LengthXZ != 0)
	{
		MATRIX2_SET_IDENTITY(&Mat);

		Mat.m00 = (std::uint16_t)((DeltaZ * ONE) / LengthXZ);
		Mat.m22 = Mat.m00;
		Mat.m20 = (std::uint16_t)((DeltaX * ONE) / LengthXZ);
		Mat.m02 = -Mat.m20;

		GTE->MulMatrix((MATRIX*)&World, (MATRIX*)&Mat);
	}

	VECTOR Vec0{ -Eye.x, -Eye.y, -Eye.z };
	VECTOR Vec1{ 0, 0, 0 };

	GTE->ApplyMatrixLV((MATRIX*)&World, &Vec0, &Vec1);

	World.tx = Vec1.vx;
	World.ty = Vec1.vy;
	World.tz = Vec1.vz;

	return World;
}

void Resident_Evil_Camera::Set(std::uint32_t FOV, VECTOR2 Eye, VECTOR2 At)
{
	b_ViewTopDown = false;

	if (!b_ViewEditor)
	{
		m_FOV = FOV;
		m_Eye = Eye;
		m_At = At;
	}

	GTE->SetGeomScreen(FOV);

	MATRIX2 Mat = Set_view(Eye, At);

	View->m00 =  GTE->ToFloat(Mat.m00); View->m01 =  GTE->ToFloat(Mat.m01); View->m02 =  GTE->ToFloat(Mat.m02); View->m03 =  GTE->ToFloat(Mat.tx);
	View->m10 =  GTE->ToFloat(Mat.m10); View->m11 =  GTE->ToFloat(Mat.m11); View->m12 =  GTE->ToFloat(Mat.m12); View->m13 =  GTE->ToFloat(Mat.ty);
	View->m20 = -GTE->ToFloat(Mat.m20); View->m21 = -GTE->ToFloat(Mat.m21); View->m22 = -GTE->ToFloat(Mat.m22); View->m23 = -GTE->ToFloat(Mat.tz);
	View->m30 = 0.0f; View->m31 = 0.0f; View->m32 = 0.0f; View->m33 = 1.0f;

	PROJECTION Proj(FOV);

    Projection->m00 = Proj.ScaleX;	Projection->m01 = 0.0f;			Projection->m02 = 0.0f;			Projection->m03 = 0.0f;
	Projection->m10 = 0.0f;			Projection->m11 = Proj.ScaleY;	Projection->m12 = 0.0f;			Projection->m13 = 0.0f;
	Projection->m20 = Proj.OffsetX;	Projection->m21 = Proj.OffsetY;	Projection->m22 = Proj.OffsetZ;	Projection->m23 = -0.1f;
	Projection->m30 = 0.0f;			Projection->m31 = 0.0f;			Projection->m32 = Proj.ScaleZ;	Projection->m33 = 0.0f;

	if (b_HorzFlip)
	{
		Projection->m00 = -Projection->m00;
		Projection->m20 = -Projection->m20;
	}

	if (b_VertFlip)
	{
		Projection->m11 = -Projection->m11;
		Projection->m21 = -Projection->m21;
	}

#if MSTD_DX9
	Render->SetWorld(World);
	Render->SetView(View);
	Render->SetProjection(Projection);
#endif
}

void Resident_Evil_Camera::GetBillboard(vec3 Position, vec3& Side, vec3& Billboard) const
{
	vec3 Eye{};
	vec3 At{};

	if (b_ViewEditor)
	{
		Eye = vec3(GTE->ToFloat(m_EditorEye.x), GTE->ToFloat(m_EditorEye.y), GTE->ToFloat(m_EditorEye.z));
		At = vec3(GTE->ToFloat(m_EditorAt.x), GTE->ToFloat(m_EditorAt.y), GTE->ToFloat(m_EditorAt.z));
	}
	else
	{
		Eye = vec3(GTE->ToFloat(m_Eye.x), GTE->ToFloat(m_Eye.y), GTE->ToFloat(m_Eye.z));
		At = vec3(GTE->ToFloat(m_At.x), GTE->ToFloat(m_At.y), GTE->ToFloat(m_At.z));
	}

	vec3 Up = vec3(0.0f, 1.0f, 0.0f);

	vec3 ToEye = (Eye - Position).Normalize();

	Side = Up.Cross(ToEye).Normalize();

	if (Side.Length() < 0.001f)
	{
		Side = vec3(1.0f, 0.0f, 0.0f);
	}

	Billboard = ToEye.Cross(Side).Normalize();
}