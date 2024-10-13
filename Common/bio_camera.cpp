/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#include "bio_camera.h"


/*
	Set
*/
void Resident_Evil_Camera::Set(std::uint32_t ViewR, VECTOR2 Eye, VECTOR2 At)
{
	GTE->SetGeomScreen(ViewR);

	MATRIX Mat = GTE->Set_view(Eye, At);
	View.m00 = static_cast<float>(Mat.m[0][0]) / ONE; View.m01 = static_cast<float>(Mat.m[0][1]) / ONE; View.m02 = static_cast<float>(Mat.m[0][2]) / ONE; View.m03 = -static_cast<float>(Mat.t[0]) / ONE;
	View.m10 = static_cast<float>(Mat.m[1][0]) / ONE; View.m11 = static_cast<float>(Mat.m[1][1]) / ONE; View.m12 = static_cast<float>(Mat.m[1][2]) / ONE; View.m13 = -static_cast<float>(Mat.t[1]) / ONE;
	View.m20 = static_cast<float>(Mat.m[2][0]) / ONE; View.m21 = static_cast<float>(Mat.m[2][1]) / ONE; View.m22 = static_cast<float>(Mat.m[2][2]) / ONE; View.m23 = -static_cast<float>(Mat.t[2]) / ONE;
	View.m30 = 0.0f; View.m31 = 0.0f; View.m32 = 0.0f; View.m33 = 1.0f;

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