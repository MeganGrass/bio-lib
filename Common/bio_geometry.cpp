/*
*
*	Megan Grass
*	June 28, 2025
*
*/


#include <bio_geometry.h>

void Resident_Evil_Geometry::Init(void)
{
	std::vector<uint32_t> Index4p = { 0, 1, 2, 3, 4, 5, 6, 7 };

	std::vector<uint32_t> IndexBoxWire = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7 };

	std::vector<uint32_t> IndexBox = { 4, 5, 6, 4, 6, 7, 1, 0, 3, 1, 3, 2, 0, 4, 7, 0, 7, 3, 5, 1, 2, 5, 2, 6, 3, 7, 6, 3, 6, 2, 0, 1, 5, 0, 5, 4 };

	std::vector<uint32_t> IndexTriWire = { 0, 1, 1, 2, 2, 0, 3, 4, 4, 5, 5, 3, 0, 3, 1, 4, 2, 5 };

	std::vector<uint32_t> IndexTri = { 0, 1, 2, 3, 4, 5, 0, 1, 4, 0, 4, 3, 1, 2, 5, 1, 5, 4, 2, 0, 3, 2, 3, 5 };

	std::vector<uint32_t> IndexRhombusWire = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7 };

	std::vector<uint32_t> IndexRhombus = { 0, 1, 2, 0, 2, 3, 4, 6, 5, 4, 7, 6, 0, 4, 1, 1, 4, 5, 1, 5, 2, 2, 5, 6, 2, 6, 3, 3, 6, 7, 3, 7, 0, 0, 7, 4 };

	std::vector<uint32_t> IndexCylinderWire = { 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 0, 12, 13, 13, 14, 14, 15, 15, 16, 16,
		17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 12, 0, 12, 1, 13, 2, 14, 3, 15, 4, 16, 5, 17, 6, 18, 7, 19, 8, 20, 9, 21, 10, 22, 11, 23 };

	std::vector<uint32_t> IndexCylinder = { 0, 1, 12, 1, 13, 12, 1, 2, 13, 2, 14, 13, 2, 3, 14, 3, 15, 14, 3, 4, 15, 4, 16, 15, 4, 5, 16, 5, 17, 16, 5, 6, 17, 6, 18, 17,
		6, 7, 18, 7, 19, 18, 7, 8, 19, 8, 20, 19, 8, 9, 20, 9, 21, 20, 9, 10, 21, 10, 22, 21, 10, 11, 22, 11, 23, 22, 11, 0, 23, 0, 12, 23,
		24, 0, 1, 24, 1, 2, 24, 2, 3, 24, 3, 4, 24, 4, 5, 24, 5, 6, 24, 6, 7, 24, 7, 8, 24, 8, 9, 24, 9, 10, 24, 10, 11, 24, 11, 0,
		25, 13, 12, 25, 14, 13, 25, 15, 14, 25, 16, 15, 25, 17, 16, 25, 18, 17, 25, 19, 18, 25, 20, 19, 25, 21, 20, 25, 22, 21, 25, 23, 22, 25, 12, 23 };

#if MSTD_DX9
	Indices4p.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, Index4p.data(), Index4p.size() * sizeof(uint32_t)));

	IndicesBoxWire.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexBoxWire.data(), IndexBoxWire.size() * sizeof(uint32_t)));

	IndicesBox.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexBox.data(), IndexBox.size() * sizeof(uint32_t)));

	IndicesTriWire.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexTriWire.data(), IndexTriWire.size() * sizeof(uint32_t)));

	IndicesTri.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexTri.data(), IndexTri.size() * sizeof(uint32_t)));

	IndicesRhombusWire.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexRhombusWire.data(), IndexRhombusWire.size() * sizeof(uint32_t)));

	IndicesRhombus.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexRhombus.data(), IndexRhombus.size() * sizeof(uint32_t)));

	IndicesCylinderWire.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexCylinderWire.data(), IndexCylinderWire.size() * sizeof(uint32_t)));

	IndicesCylinder.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexCylinder.data(), IndexCylinder.size() * sizeof(uint32_t)));
#endif
}

void Resident_Evil_Geometry::DrawShape(const DRAWSHAPE& Shape) const
{
#if MSTD_DX9
	DWORD CullMode = 0;

	if (Shape.Solid)
	{
		Render->Device()->GetRenderState(D3DRS_CULLMODE, &CullMode);
		Render->Device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		Render->AlphaBlending(TRUE, D3DBLEND_INVDESTCOLOR, D3DBLEND_INVSRCALPHA);
	}

	std::unique_ptr<IDirect3DVertexBuffer9, IDirect3DDelete9<IDirect3DVertexBuffer9>> Vertices;
	Vertices.reset(Render->CreateVec3c(Shape.Vertice, Shape.Solid ? (0x40 << 24) | (Shape.Color & 0xFFFFFF) : Shape.Color));

	Render->SetWorld(Shape.World);

	if (Shape.IndiceWire) { Render->DrawVec3c(Vertices.get(), (IDirect3DIndexBuffer9*)Shape.IndiceWire, nullptr, D3DFILL_WIREFRAME, D3DPT_LINELIST); }

	if (Shape.Indice && Shape.Solid) { Render->DrawVec3c(Vertices.get(), (IDirect3DIndexBuffer9*)Shape.Indice, nullptr, D3DFILL_SOLID, D3DPT_TRIANGLELIST, FALSE); }

	if (Shape.ResetWorld) { Render->ResetWorld(*Shape.World.get()); }

	if (Shape.Solid)
	{
		Render->Device()->SetRenderState(D3DRS_CULLMODE, CullMode);
		Render->AlphaBlending(FALSE, D3DBLEND_ZERO, D3DBLEND_ZERO);
	}
#endif
}

std::shared_ptr<Standard_Matrix> Resident_Evil_Geometry::SetWorldMatrix(const SHAPEVECTOR& Vec, const SVECTOR2& Rotation, const vec3& Centroid) const
{
	float RX = GTE->ToFloat(std::clamp(Rotation.x, (std::int16_t)-ONE, (std::int16_t)ONE)) * 360.0f;
	float RY = GTE->ToFloat(std::clamp(Rotation.y, (std::int16_t)-ONE, (std::int16_t)ONE)) * 360.0f;
	float RZ = GTE->ToFloat(std::clamp(Rotation.z, (std::int16_t)-ONE, (std::int16_t)ONE)) * 360.0f;

	Standard_Matrix Neg = Standard_Matrix().Translate(-Centroid);
	Standard_Matrix Pos = Standard_Matrix().Translate(Centroid);
	Standard_Matrix R = Standard_Matrix().YawPitchRoll(vec3{ World->Radian(RX), World->Radian(RY), World->Radian(RZ) });
	Standard_Matrix T = Standard_Matrix().Translate(vec3{ GTE->ToFloat(Vec.x), GTE->ToFloat(Vec.y), GTE->ToFloat(Vec.z) });

	*World = T * (Pos * R * Neg);

	return World;
}

void Resident_Evil_Geometry::Draw4p(const std::int16_t Xz[4][2], std::int32_t Y, DWORD Color) const
{
	float YY = GTE->ToFloat(Y);

	std::vector<vec3> Shape = {
		vec3{ GTE->ToFloat(Xz[0][0]), YY, GTE->ToFloat(Xz[0][1]) },
		vec3{ GTE->ToFloat(Xz[1][0]), YY, GTE->ToFloat(Xz[1][1]) },
		vec3{ GTE->ToFloat(Xz[1][0]), YY, GTE->ToFloat(Xz[1][1]) },
		vec3{ GTE->ToFloat(Xz[2][0]), YY, GTE->ToFloat(Xz[2][1]) },
		vec3{ GTE->ToFloat(Xz[2][0]), YY, GTE->ToFloat(Xz[2][1]) },
		vec3{ GTE->ToFloat(Xz[3][0]), YY, GTE->ToFloat(Xz[3][1]) },
		vec3{ GTE->ToFloat(Xz[3][0]), YY, GTE->ToFloat(Xz[3][1]) },
		vec3{ GTE->ToFloat(Xz[0][0]), YY, GTE->ToFloat(Xz[0][1]) } };

	World->SetWorld(vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 0.0f, 0.0f, 0.0f });

	DrawShape({ World, false, Shape, Color, false, Indices4p.get(), nullptr});
}

void Resident_Evil_Geometry::DrawBox(SHAPEVECTOR Vec, SVECTOR2 Rotation, DWORD Color, bool Solid) const
{
	float WW = GTE->ToFloat(Vec.w);
	float DD = GTE->ToFloat(Vec.d);
	float Low = GTE->ToFloat(Vec.y);
	float High = Low + GTE->ToFloat(Vec.h);

	std::vector<vec3> Shape = {
		vec3{ 0.0f, Low, 0.0f },
		vec3{ WW, Low, 0.0f },
		vec3{ WW, High, 0.0f },
		vec3{ 0.0f, High, 0.0f },
		vec3{ 0.0f, Low, DD },
		vec3{ WW, Low, DD },
		vec3{ WW, High, DD },
		vec3{ 0.0f, High, DD } };

	DrawShape({ SetWorldMatrix(Vec, Rotation, World->Centroid(Shape)), true, Shape, Color, Solid, IndicesBoxWire.get(), IndicesBox.get() });
}

void Resident_Evil_Geometry::DrawTriangle(SHAPEVECTOR Vec, SVECTOR2 Rotation, DWORD Color, bool Solid, Resident_Evil_Triangle_Type Type) const
{
	float WW = GTE->ToFloat(Vec.w);
	float DD = GTE->ToFloat(Vec.d);
	float Low = GTE->ToFloat(Vec.y);
	float High = Low + GTE->ToFloat(Vec.h);
	float SlopeHigh = Low + GTE->ToFloat(-1800);

	if (std::to_underlying(Type) & std::to_underlying(Resident_Evil_Triangle_Type::Stairs)) { SlopeHigh = High; }

	Type = static_cast<Resident_Evil_Triangle_Type>(std::to_underlying(Type) & ~std::to_underlying(Resident_Evil_Triangle_Type::Stairs));

	std::vector<vec3> Shape;

	switch (Type)
	{
		case Resident_Evil_Triangle_Type::Naname_A:
			Shape = {
				vec3{ 0.0f, Low, DD },
				vec3{ WW, Low, DD },
				vec3{ WW, Low, 0.0f },
				vec3{ 0.0f, High, DD },
				vec3{ WW, High, DD },
				vec3{ WW, High, 0.0f } };
			break;
		case Resident_Evil_Triangle_Type::Naname_B:
			Shape = {
				vec3{ 0.0f, Low, 0.0f },
				vec3{ 0.0f, Low, DD },
				vec3{ WW, Low, DD },
				vec3{ 0.0f, High, 0.0f },
				vec3{ 0.0f, High, DD },
				vec3{ WW, High, DD } };
			break;
		case Resident_Evil_Triangle_Type::Naname_C:
			Shape = {
				vec3{ WW, Low, DD },
				vec3{ WW, Low, 0.0f },
				vec3{ 0.0f, Low, 0.0f },
				vec3{ WW, High, DD },
				vec3{ WW, High, 0.0f },
				vec3{ 0.0f, High, 0.0f } };
			break;
		case Resident_Evil_Triangle_Type::Naname_D:
			Shape = {
				vec3{ WW, Low, 0.0f },
				vec3{ 0.0f, Low, 0.0f },
				vec3{ 0.0f, Low, DD },
				vec3{ WW, High, 0.0f },
				vec3{ 0.0f, High, 0.0f },
				vec3{ 0.0f, High, DD } };
			break;
		case Resident_Evil_Triangle_Type::Slope_A:
			Shape = {
				vec3{ 0.0f, Low, 0.0f },
				vec3{ WW, Low, 0.0f },
				vec3{ WW, SlopeHigh, 0.0f },
				vec3{ 0.0f, Low, DD },
				vec3{ WW, Low, DD },
				vec3{ WW, SlopeHigh, DD } };
			break;
		case Resident_Evil_Triangle_Type::Slope_B:
			Shape = {
				vec3{ WW, Low, 0.0f },
				vec3{ 0.0f, SlopeHigh, 0.0f },
				vec3{ 0.0f, Low, 0.0f },
				vec3{ WW, Low, DD },
				vec3{ 0.0f, SlopeHigh, DD },
				vec3{ 0.0f, Low, DD } };
			break;
		case Resident_Evil_Triangle_Type::Slope_C:
			Shape = {
				vec3{ 0.0f, Low, 0.0f },
				vec3{ 0.0f, SlopeHigh, DD },
				vec3{ 0.0f, Low, DD },
				vec3{ WW, Low, 0.0f },
				vec3{ WW, SlopeHigh, DD },
				vec3{ WW, Low, DD } };
			break;
		case Resident_Evil_Triangle_Type::Slope_D:
			Shape = {
				vec3{ 0.0f, Low, DD },
				vec3{ 0.0f, SlopeHigh, 0.0f },
				vec3{ 0.0f, Low, 0.0f },
				vec3{ WW, Low, DD },
				vec3{ WW, SlopeHigh, 0.0f },
				vec3{ WW, Low, 0.0f } };
			break;
	}

	DrawShape({ SetWorldMatrix(Vec, Rotation, World->Centroid(Shape)), true, Shape, Color, Solid, IndicesTriWire.get(), IndicesTri.get() });
}

void Resident_Evil_Geometry::DrawRhombus(SHAPEVECTOR Vec, SVECTOR2 Rotation, DWORD Color, bool Solid) const
{
	float WW = GTE->ToFloat(Vec.w) * 0.5f;
	float DD = GTE->ToFloat(Vec.d) * 0.5f;
	float Low = GTE->ToFloat(Vec.y);
	float High = Low + GTE->ToFloat(Vec.h);

	std::vector<vec3> Shape = {
		vec3{ 0.0f, Low, -DD },
		vec3{ -WW, Low, 0.0f },
		vec3{ 0.0f, Low, DD },
		vec3{ WW, Low, 0.0f },
		vec3{ 0.0f, High, -DD },
		vec3{ -WW, High, 0.0f },
		vec3{ 0.0f, High, DD },
		vec3{ WW, High, 0.0f } };

	Vec.x += (Vec.w * 2) / 3;
	Vec.z += (Vec.d / 2);

	DrawShape({ SetWorldMatrix(Vec, Rotation, World->Centroid(Shape)), true, Shape, Color, Solid, IndicesRhombusWire.get(), IndicesRhombus.get() });
}

void Resident_Evil_Geometry::DrawCylinder(SHAPEVECTOR Vec, SVECTOR2 Rotation, DWORD Color, bool Solid) const
{
	float Radius = min(GTE->ToFloat(Vec.w), GTE->ToFloat(Vec.d)) * 0.5f;
	float Offset = Radius * 0.75f;
	float WW = GTE->ToFloat(Vec.w) * 0.5f;
	float DD = GTE->ToFloat(Vec.d) * 0.5f;
	float XX = WW - Radius;
	float ZZ = DD - Radius;
	float Low = GTE->ToFloat(Vec.y);
	float High = Low + GTE->ToFloat(Vec.h);

	std::vector<vec3> Shape = {
		vec3{ WW - XX,			Low,	DD - ZZ - Radius },
		vec3{ WW + XX,			Low,	DD - ZZ - Radius },
		vec3{ WW + XX + Offset, Low,	DD - ZZ - Offset },
		vec3{ WW + XX + Radius, Low,	DD - ZZ			 },
		vec3{ WW + XX + Radius, Low,	DD + ZZ			 },
		vec3{ WW + XX + Offset, Low,	DD + ZZ + Offset },
		vec3{ WW + XX,			Low,	DD + ZZ + Radius },
		vec3{ WW - XX,			Low,	DD + ZZ + Radius },
		vec3{ WW - XX - Offset, Low,	DD + ZZ + Offset },
		vec3{ WW - XX - Radius, Low,	DD + ZZ			 },
		vec3{ WW - XX - Radius, Low,	DD - ZZ			 },
		vec3{ WW - XX - Offset, Low,	DD - ZZ - Offset },
		vec3{ WW - XX,			High,	DD - ZZ - Radius },
		vec3{ WW + XX,			High,	DD - ZZ - Radius },
		vec3{ WW + XX + Offset, High,	DD - ZZ - Offset },
		vec3{ WW + XX + Radius, High,	DD - ZZ			 },
		vec3{ WW + XX + Radius, High,	DD + ZZ			 },
		vec3{ WW + XX + Offset, High,	DD + ZZ + Offset },
		vec3{ WW + XX,			High,	DD + ZZ + Radius },
		vec3{ WW - XX,			High,	DD + ZZ + Radius },
		vec3{ WW - XX - Offset, High,	DD + ZZ + Offset },
		vec3{ WW - XX - Radius, High,	DD + ZZ			 },
		vec3{ WW - XX - Radius, High,	DD - ZZ			 },
		vec3{ WW - XX - Offset, High,	DD - ZZ - Offset },
		vec3{ WW,				Low,	DD				 },
		vec3{ WW,				High,	DD				 } };

	DrawShape({ SetWorldMatrix(Vec, Rotation, World->Centroid(Shape)), true, Shape, Color, Solid, IndicesCylinderWire.get(), IndicesCylinder.get() });
}