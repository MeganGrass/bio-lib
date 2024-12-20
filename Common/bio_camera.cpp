/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#include "bio_global.h"


/*
	Resident Evil 2 Dual Shock (SLUS_007.48)
*/
MATRIX Resident_Evil_Camera::Set_view(VECTOR2 Eye, VECTOR2 At)
{
	MATRIX Mat{};
	MATRIX World{};	// Gs world-to-screen matrix

	G->GTE->SetIdentity(&Mat);
	G->GTE->SetIdentity(&World);

	long DeltaX = At.x - Eye.x;
	long DeltaY = At.y - Eye.y;
	long DeltaZ = At.z - Eye.z;

	long Length = G->GTE->SquareRoot0(DeltaX * DeltaX + DeltaY * DeltaY + DeltaZ * DeltaZ);
	long LengthXZ = G->GTE->SquareRoot0(DeltaX * DeltaX + DeltaZ * DeltaZ);

	Mat.m[2][2] = static_cast<short>((LengthXZ * ONE) / Length);
	Mat.m[2][1] = static_cast<short>((DeltaY * ONE) / Length);
	Mat.m[1][2] = -Mat.m[2][1];
	Mat.m[1][1] = Mat.m[2][2];

	G->GTE->MulMatrix(&World, &Mat);

	if (LengthXZ != 0)
	{
		G->GTE->SetIdentity(&Mat);

		Mat.m[0][0] = static_cast<short>((DeltaZ * ONE) / LengthXZ);
		Mat.m[2][2] = Mat.m[0][0];
		Mat.m[2][0] = static_cast<short>((DeltaX * ONE) / LengthXZ);
		Mat.m[0][2] = -Mat.m[2][0];

		G->GTE->MulMatrix(&World, &Mat);
	}

	VECTOR Up{ -Eye.x, -Eye.y, -Eye.z };

	G->GTE->ApplyMatrixLV(&World, &Up, (VECTOR*)World.t);

	return World;
}


/*
	Set
*/
void Resident_Evil_Camera::Set_view(std::uint32_t ViewR, VECTOR2 Eye, VECTOR2 At)
{
	G->GTE->SetGeomScreen(ViewR);

	MATRIX Mat = Set_view(Eye, At);

	View.m00 = static_cast<float>(Mat.m[0][0]) / ONE; View.m01 = static_cast<float>(Mat.m[0][1]) / ONE; View.m02 = static_cast<float>(Mat.m[0][2]) / ONE; View.m03 = -static_cast<float>(Mat.t[0]) / ONE;
	View.m10 = static_cast<float>(Mat.m[1][0]) / ONE; View.m11 = static_cast<float>(Mat.m[1][1]) / ONE; View.m12 = static_cast<float>(Mat.m[1][2]) / ONE; View.m13 = -static_cast<float>(Mat.t[1]) / ONE;
	View.m20 = static_cast<float>(Mat.m[2][0]) / ONE; View.m21 = static_cast<float>(Mat.m[2][1]) / ONE; View.m22 = static_cast<float>(Mat.m[2][2]) / ONE; View.m23 = -static_cast<float>(Mat.t[2]) / ONE;
	View.m30 = 0.0f; View.m31 = 0.0f; View.m32 = 0.0f; View.m33 = 1.0f;

	if (!b_Mirror)
	{
		View.m00 = -View.m00;
		View.m01 = -View.m01;
		View.m02 = -View.m02;
		View.m03 = -View.m03;
	}

    float Right = 160.0f;
    float Left = -160.0f;
    float Top = 120.0f;
    float Bottom = -120.0f;
    float Near = 1.0f;
    float Far = 1000.0f;
    float FovY = 2.0f * std::atan(120.0f / ViewR);
    float FovX = 2.0f * std::atan(160.0f / ViewR);
    float ScaleX = 1.0f / std::tan(FovX / 2.0f);
    float ScaleY = 1.0f / std::tan(FovY / 2.0f);
    float ScaleZ = Far / (Near - Far);
    float OffsetX = (Left + Right) / (Left - Right);
    float OffsetY = (Top + Bottom) / (Top - Bottom);
    float OffsetZ = Near * Far / (Near - Far);
    Projection.m00 = ScaleX; Projection.m01 = 0.0f; Projection.m02 = 0.0f; Projection.m03 = 0.0f;
    Projection.m10 = 0.0f; Projection.m11 = ScaleY; Projection.m12 = 0.0f; Projection.m13 = 0.0f;
    Projection.m20 = OffsetX; Projection.m21 = OffsetY; Projection.m22 = OffsetZ; Projection .m23 = -1.0f;
    Projection.m30 = 0.0f; Projection.m31 = 0.0f; Projection.m32 = ScaleZ; Projection.m33 = 0.0f;
}


/*
	Set ortho
*/
void Resident_Evil_Camera::SetOrtho(float Width, float Height)
{
	m_Width = Width;
	m_Height = Height;
	Orthogonal.OrthogonalOffCenterRight(0.0f, m_Width, m_Height, 0.0f, 0.0f, 1.0f);
}


/*
	Get background vertices
*/
std::vector<vec4t> Resident_Evil_Camera::GetVert(void) const
{
	std::vector<vec4t> Vert(4);

	float l = -0.5f;
	float r = (m_Width - 0.5f);
	float t = -0.5f;
	float b = (m_Height - 0.5f);

	if (b_Mirror)
	{
		float tmp = l;
		l = r;
		r = tmp;
	}

	Vert[0].vec.Set(l, b, 1.0f, 1.0f);	Vert[0].uv.Set(0.0f, 0.0f);
	Vert[1].vec.Set(r, b, 1.0f, 1.0f);	Vert[1].uv.Set(1.0f, 0.0f);
	Vert[2].vec.Set(l, t, 1.0f, 1.0f);	Vert[2].uv.Set(0.0f, 1.0f);
	Vert[3].vec.Set(r, t, 1.0f, 1.0f);	Vert[3].uv.Set(1.0f, 1.0f);

	return Vert;
}