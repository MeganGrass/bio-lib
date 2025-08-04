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
	b_DrawLine = false;
	b_DrawSwitch = false;
#if MSTD_DX9
	if (m_Background)
	{
		m_Background->Release();
		m_Background.reset(nullptr);
	}

	if (m_BackgroundVert)
	{
		m_BackgroundVert->Release();
		m_BackgroundVert.reset(nullptr);
	}

	if (m_Sprite)
	{
		m_Sprite->Release();
		m_Sprite.reset(nullptr);
	}
#endif
}

void Resident_Evil_Camera::Reset(void)
{
	Stage = 0;
	Room = 0;
	m_Path.clear();
	Cut = 0;
	CutMax = 0;
	m_FOV = (0x6DD4 >> 7);
	m_Eye = { -16000, -7200, -16000 };
	m_At = { 0, 7200, 0 };
	m_TexWidth = 0.0f;
	m_TexHeight = 0.0f;
	m_Background.reset(nullptr);
	if (!b_ViewModelEdit) { Set(m_FOV, m_Eye, m_At); }
	else { Set(m_ModelFOV, m_ModelEye, m_ModelAt); }
}

void Resident_Evil_Camera::SetMeta(std::filesystem::path _Path, std::uint8_t _Stage, std::uint8_t _Room, std::uint8_t _CutMax) noexcept
{
	m_Path = _Path;
	Stage = _Stage;
	Room = _Room;
	CutMax = _CutMax;
}

void Resident_Evil_Camera::SetOrtho(float Width, float Height)
{
	m_OrthoWidth = Width;
	m_OrthoHeight = Height;
	m_OrthoScaleX = m_OrthoWidth / m_NativeWidth;
	m_OrthoScaleY = m_OrthoHeight / m_NativeHeight;
	Orthogonal->OrthogonalOffCenterLeft(0.0f, m_OrthoWidth, 0.0f, m_OrthoHeight, 0.0f, 1.0f);
}

std::uint8_t Resident_Evil_Camera::SetImage(std::uint8_t iCut)
{
	if (CutMax) { Cut = std::clamp(iCut, (uint8_t)0, (uint8_t)(CutMax - 1)); }
	else { Cut = 0; }

	m_TexWidth = 0;
	m_TexHeight = 0;

	m_TexSprWidth = 0;
	m_TexSprHeight = 0;

	std::unique_ptr<Standard_Image> Image = std::make_unique<Standard_Image>();
	Image->Str.hWnd = Str.hWnd;

	std::filesystem::path Background = Str.FormatCStyle(L"%ws\\ROOM%d%02x%02d.png", m_Path.wstring().c_str(), Stage, Room, Cut);

	std::filesystem::path Sprite = Str.FormatCStyle(L"%ws\\ROOM_%d%02x_%02d_mask.png", m_Path.wstring().c_str(), Stage, Room, Cut);

#ifdef LIB_PNG
	if (Standard_FileSystem().Exists(Background) && Image->OpenPNG(Background))
	{
		m_TexWidth = (float)Image->GetWidth();
		m_TexHeight = (float)Image->GetHeight();
	}
#endif

#if MSTD_DX9
	m_Background.reset(Render->CreateTexture(Image));
#endif

	Image->Close();

#ifdef LIB_PNG
	if (Standard_FileSystem().Exists(Sprite) && Image->OpenPNG(Sprite))
	{
		m_TexSprWidth = (float)Image->GetWidth();
		m_TexSprHeight = (float)Image->GetHeight();
	}
#endif

#if MSTD_DX9
	m_Sprite.reset(Render->CreateTexture(Image, false, 0, 0, true));
#endif

	return Cut;
}

std::vector<vec4t> Resident_Evil_Camera::GetImageVert(void) const
{
	std::vector<vec4t> Vert(4);

	// adjust for direct-x 9 half texel
	float l = -0.5f;
	float r = (m_OrthoWidth - 0.5f);
	float t = 0.5f;
	float b = (m_OrthoHeight + 0.5f);
	
	if (b_HorzFlipTex) std::swap(l, r);
	if (b_VertFlipTex) std::swap(t, b);

	Vert[0].vec.Set(l, b, 1.0f, 1.0f);	Vert[0].uv.Set(0.0f, 0.0f);
	Vert[1].vec.Set(r, b, 1.0f, 1.0f);	Vert[1].uv.Set(1.0f, 0.0f);
	Vert[2].vec.Set(l, t, 1.0f, 1.0f);	Vert[2].uv.Set(0.0f, 1.0f);
	Vert[3].vec.Set(r, t, 1.0f, 1.0f);	Vert[3].uv.Set(1.0f, 1.0f);

	return Vert;
}

void Resident_Evil_Camera::SetTopDownPerspective(void)
{
	b_ViewTopDown = true;

	View->Identity();
	View->RotateX(View->Radian(90.0f));

	Projection->OrthogonalOffCenterLeft(
		-((m_OrthoWidth / 2.0f) / m_Cy) + m_Cx, ((m_OrthoWidth / 2.0f) / m_Cy) + m_Cx,
		-((m_OrthoHeight / 2.0f) / m_Cy) - m_Cz, ((m_OrthoHeight / 2.0f) / m_Cy) - m_Cz,
		4096.0f, -1.0f
	);

	World->SetWorld(vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 1.0f, 1.0f, 1.0f });

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

	if (b_ViewModelEdit)
	{
		m_ModelFOV = FOV;
		m_ModelEye = Eye;
		m_ModelAt = At;
	}
	else
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
	Projection->m20 = Proj.OffsetX;	Projection->m21 = Proj.OffsetY;	Projection->m22 = Proj.OffsetZ;	Projection->m23 = -1.0f;
	Projection->m30 = 0.0f;			Projection->m31 = 0.0f;			Projection->m32 = Proj.ScaleZ;	Projection->m33 = 0.0f;

	World->SetWorld(vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 1.0f, 1.0f, 1.0f });

#if MSTD_DX9
	Render->SetWorld(World);
	Render->SetView(View);
	Render->SetProjection(Projection);
#endif
}