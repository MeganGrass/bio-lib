/*
*
*	Megan Grass
*	June 28, 2025
* 
*	CREDIT:
*
*		"CameraSwitch" function:
*		"Collision" function (original):
			https://github.com/XProger/OpenResident/blob/main/src/collision.h
*
*/


#include <bio_geometry.h>


void Resident_Evil_Geometry::Init(void)
{
	std::vector<uint32_t> Index4p = { 0, 1, 2, 3, 4, 5, 6, 7 };

	std::vector<uint32_t> IndexBoxWire = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7 };

	std::vector<uint32_t> IndexBox = { 0, 1, 5, 0, 5, 4, 3, 2, 6, 3, 6, 7, 0, 3, 7, 0, 7, 4, 1, 2, 6, 1, 6, 5, 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7 };

	std::vector<uint32_t> IndexPlaneWire = { 0, 1, 1, 3, 3, 5, 5, 7, 7, 0 };

	std::vector<uint32_t> IndexPlane = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7 };

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

	IndicesPlaneWire.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexPlaneWire.data(), IndexPlaneWire.size() * sizeof(uint32_t)));

	IndicesPlane.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexPlane.data(), IndexPlane.size() * sizeof(uint32_t)));

	IndicesTriWire.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexTriWire.data(), IndexTriWire.size() * sizeof(uint32_t)));

	IndicesTri.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexTri.data(), IndexTri.size() * sizeof(uint32_t)));

	IndicesRhombusWire.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexRhombusWire.data(), IndexRhombusWire.size() * sizeof(uint32_t)));

	IndicesRhombus.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexRhombus.data(), IndexRhombus.size() * sizeof(uint32_t)));

	IndicesCylinderWire.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexCylinderWire.data(), IndexCylinderWire.size() * sizeof(uint32_t)));

	IndicesCylinder.reset(Render->CreateIndexBuffer(D3DFMT_INDEX32, IndexCylinder.data(), IndexCylinder.size() * sizeof(uint32_t)));
#endif
}

void Resident_Evil_Geometry::Shutdown(void) noexcept
{
	iObject = 0;
	iObjectMin = 0;
	iObjectMax = 0;
	b_CollisionDetection = false;
	b_DrawCollision = false;
	b_SolidCollision = false;
	b_SolidCollisionAll = false;
	b_ShapeCollision = false;
	b_ShapeCollisionAll = false;
	b_HighlightCollision = false;
	b_DrawBlock = false;
	b_DrawFloor = false;
	b_HitSlopeX = false;
	b_HitSlopeZ = false;
	m_Slope = {};
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
		Render->AlphaTesting(TRUE, 0xFF, D3DCMP_GREATEREQUAL);
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
		Render->AlphaTesting(FALSE, 0x00, D3DCMP_NEVER);
	}
#endif
}

std::shared_ptr<Standard_Matrix> Resident_Evil_Geometry::SetWorldMatrix(const SHAPEVECTOR& Vec, const VECTOR2& Rotation, const vec3& Centroid) const
{
	float RX = GTE->ToFloat(std::clamp(Rotation.x, -ONE, ONE)) * 360.0f;
	float RY = GTE->ToFloat(std::clamp(Rotation.y, -ONE, ONE)) * 360.0f;
	float RZ = GTE->ToFloat(std::clamp(Rotation.z, -ONE, ONE)) * 360.0f;

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

void Resident_Evil_Geometry::DrawBox(SHAPEVECTOR Vec, VECTOR2 Rotation, DWORD Color, bool Solid) const
{
	float WW = GTE->ToFloat(Vec.w);
	float DD = GTE->ToFloat(Vec.d);
	float Height = -GTE->ToFloat(std::abs(Vec.h) - std::abs(Vec.y));

	std::vector<vec3> Shape = {
		vec3{ 0.0f, 0.0f, 0.0f },
		vec3{ WW, 0.0f, 0.0f },
		vec3{ WW, Height, 0.0f },
		vec3{ 0.0f, Height, 0.0f },
		vec3{ 0.0f, 0.0f, DD },
		vec3{ WW, 0.0f, DD },
		vec3{ WW, Height, DD },
		vec3{ 0.0f, Height, DD } };

	DrawShape({ SetWorldMatrix(Vec, Rotation, World->Centroid(Shape)), true, Shape, Color, Solid, IndicesBoxWire.get(), IndicesBox.get() });
}

void Resident_Evil_Geometry::DrawDiagonal(SHAPEVECTOR Vec, VECTOR2 Rotation, DWORD Color, bool Solid, Shape_Type Type) const
{
	float WW = GTE->ToFloat(Vec.w);
	float DD = GTE->ToFloat(Vec.d);
	float Height = -GTE->ToFloat(std::abs(Vec.h) - std::abs(Vec.y));

	std::vector<vec3> Shape;

	switch (Type)
	{
	case Shape_Type::Diagonal_B:
	case Shape_Type::Diagonal_C:
		Shape = {
			vec3{ 0.0f, 0.0f, 0.0f },
			vec3{ WW, 0.0f, DD },
			vec3{ WW, 0.0f, DD },
			vec3{ WW, Height, DD },
			vec3{ WW, Height, DD },
			vec3{ 0.0f, Height, 0.0f },
			vec3{ 0.0f, Height, 0.0f },
			vec3{ 0.0f, 0.0f, 0.0f }
		};
		break;
	case Shape_Type::Diagonal_A:
	case Shape_Type::Diagonal_D:
		Shape = {
			vec3{ 0.0f, 0.0f, DD },
			vec3{ WW, 0.0f, 0.0f },
			vec3{ WW, 0.0f, 0.0f },
			vec3{ WW, Height, 0.0f },
			vec3{ WW, Height, 0.0f },
			vec3{ 0.0f, Height, DD },
			vec3{ 0.0f, Height, DD },
			vec3{ 0.0f, 0.0f, DD }
		};
		break;
	}

	DrawShape({ SetWorldMatrix(Vec, Rotation, World->Centroid(Shape)), true, Shape, Color, Solid, IndicesPlaneWire.get(), IndicesPlane.get() });
}

void Resident_Evil_Geometry::DrawTriangle(SHAPEVECTOR Vec, VECTOR2 Rotation, DWORD Color, bool Solid, Shape_Type Type) const
{
	float WW = GTE->ToFloat(Vec.w);
	float DD = GTE->ToFloat(Vec.d);
	float Height = -GTE->ToFloat(std::abs(Vec.h) - std::abs(Vec.y));

	std::vector<vec3> Shape;

	switch (Type)
	{
		case Shape_Type::Slope_A:
			Shape = {
				vec3{ 0.0f, 0.0f, 0.0f },
				vec3{ WW, 0.0f, 0.0f },
				vec3{ WW, Height, 0.0f },
				vec3{ 0.0f, 0.0f, DD },
				vec3{ WW, 0.0f, DD },
				vec3{ WW, Height, DD } };
			break;
		case Shape_Type::Slope_B:
			Shape = {
				vec3{ WW, 0.0f, 0.0f },
				vec3{ 0.0f, Height, 0.0f },
				vec3{ 0.0f, 0.0f, 0.0f },
				vec3{ WW, 0.0f, DD },
				vec3{ 0.0f, Height, DD },
				vec3{ 0.0f, 0.0f, DD } };
			break;
		case Shape_Type::Slope_C:
			Shape = {
				vec3{ 0.0f, 0.0f, 0.0f },
				vec3{ 0.0f, Height, DD },
				vec3{ 0.0f, 0.0f, DD },
				vec3{ WW, 0.0f, 0.0f },
				vec3{ WW, Height, DD },
				vec3{ WW, 0.0f, DD } };
			break;
		case Shape_Type::Slope_D:
			Shape = {
				vec3{ 0.0f, 0.0f, DD },
				vec3{ 0.0f, Height, 0.0f },
				vec3{ 0.0f, 0.0f, 0.0f },
				vec3{ WW, 0.0f, DD },
				vec3{ WW, Height, 0.0f },
				vec3{ WW, 0.0f, 0.0f } };
			break;
	}

	DrawShape({ SetWorldMatrix(Vec, Rotation, World->Centroid(Shape)), true, Shape, Color, Solid, IndicesTriWire.get(), IndicesTri.get() });
}

void Resident_Evil_Geometry::DrawRhombus(SHAPEVECTOR Vec, VECTOR2 Rotation, DWORD Color, bool Solid) const
{
	float WW = GTE->ToFloat(Vec.w) * 0.5f;
	float DD = GTE->ToFloat(Vec.d) * 0.5f;
	float Height = -GTE->ToFloat(std::abs(Vec.h) - std::abs(Vec.y));

	std::vector<vec3> Shape = {
		vec3{ 0.0f, 0.0f, -DD },
		vec3{ -WW, 0.0f, 0.0f },
		vec3{ 0.0f, 0.0f, DD },
		vec3{ WW, 0.0f, 0.0f },
		vec3{ 0.0f, Height, -DD },
		vec3{ -WW, Height, 0.0f },
		vec3{ 0.0f, Height, DD },
		vec3{ WW, Height, 0.0f } };

	Vec.x += (Vec.w / 2);
	Vec.z += (Vec.d / 2);

	DrawShape({ SetWorldMatrix(Vec, Rotation, World->Centroid(Shape)), true, Shape, Color, Solid, IndicesRhombusWire.get(), IndicesRhombus.get() });
}

void Resident_Evil_Geometry::DrawCylinder(SHAPEVECTOR Vec, VECTOR2 Rotation, DWORD Color, bool Solid) const
{
	float Radius = min(GTE->ToFloat(Vec.w), GTE->ToFloat(Vec.d)) * 0.5f;
	float Offset = Radius * 0.75f;
	float WW = GTE->ToFloat(Vec.w) * 0.5f;
	float DD = GTE->ToFloat(Vec.d) * 0.5f;
	float XX = WW - Radius;
	float ZZ = DD - Radius;
	float Height = -GTE->ToFloat(std::abs(Vec.h) - std::abs(Vec.y));

	std::vector<vec3> Shape = {
		vec3{ WW - XX,			0.0f,	DD - ZZ - Radius },
		vec3{ WW + XX,			0.0f,	DD - ZZ - Radius },
		vec3{ WW + XX + Offset, 0.0f,	DD - ZZ - Offset },
		vec3{ WW + XX + Radius, 0.0f,	DD - ZZ			 },
		vec3{ WW + XX + Radius, 0.0f,	DD + ZZ			 },
		vec3{ WW + XX + Offset, 0.0f,	DD + ZZ + Offset },
		vec3{ WW + XX,			0.0f,	DD + ZZ + Radius },
		vec3{ WW - XX,			0.0f,	DD + ZZ + Radius },
		vec3{ WW - XX - Offset, 0.0f,	DD + ZZ + Offset },
		vec3{ WW - XX - Radius, 0.0f,	DD + ZZ			 },
		vec3{ WW - XX - Radius, 0.0f,	DD - ZZ			 },
		vec3{ WW - XX - Offset, 0.0f,	DD - ZZ - Offset },
		vec3{ WW - XX,			Height,	DD - ZZ - Radius },
		vec3{ WW + XX,			Height,	DD - ZZ - Radius },
		vec3{ WW + XX + Offset, Height,	DD - ZZ - Offset },
		vec3{ WW + XX + Radius, Height,	DD - ZZ			 },
		vec3{ WW + XX + Radius, Height,	DD + ZZ			 },
		vec3{ WW + XX + Offset, Height,	DD + ZZ + Offset },
		vec3{ WW + XX,			Height,	DD + ZZ + Radius },
		vec3{ WW - XX,			Height,	DD + ZZ + Radius },
		vec3{ WW - XX - Offset, Height,	DD + ZZ + Offset },
		vec3{ WW - XX - Radius, Height,	DD + ZZ			 },
		vec3{ WW - XX - Radius, Height,	DD - ZZ			 },
		vec3{ WW - XX - Offset, Height,	DD - ZZ - Offset },
		vec3{ WW,				0.0f,	DD				 },
		vec3{ WW,				Height,	DD				 } };

	DrawShape({ SetWorldMatrix(Vec, Rotation, World->Centroid(Shape)), true, Shape, Color, Solid, IndicesCylinderWire.get(), IndicesCylinder.get() });
}

bool Resident_Evil_Geometry::Collision(VECTOR2& Position, const SIZEVECTOR Hitbox, const SHAPEVECTOR Shape, const Shape_Type ShapeType)
{
	int32_t minX = Shape.x;
	int32_t maxX = Shape.x + Shape.w;
	int32_t minY = Shape.y;
	int32_t maxY = Shape.h;
	int32_t minZ = Shape.z;
	int32_t maxZ = Shape.z + Shape.d;

	int32_t& px = Position.x;
	int32_t& py = Position.y;
	int32_t& pz = Position.z;

	int32_t pw = Hitbox.w;
	int32_t ph = Hitbox.h;
	int32_t pd = Hitbox.d * 2;

	auto line = [](int32_t ax, int32_t az, int32_t bx, int32_t bz, int32_t r, int32_t& px, int32_t& pz) -> bool {
		int32_t dx = bx - ax;
		int32_t dz = bz - az;
		int32_t segLen2 = dx * dx + dz * dz;
		if (segLen2 == 0) return false;

		float t = ((px - ax) * dx + (pz - az) * dz) / static_cast<float>(segLen2);
		t = std::clamp(t, 0.0f, 1.0f);

		float closestX = ax + t * dx;
		float closestZ = az + t * dz;

		float distX = px - closestX;
		float distZ = pz - closestZ;
		float dist2 = distX * distX + distZ * distZ;

		if (dist2 > r * r) return false;

		float dist = std::sqrt(dist2);
		if (dist == 0.0f) return false;

		float push = (r - dist) / dist;
		px += static_cast<int32_t>(distX * push);
		pz += static_cast<int32_t>(distZ * push);

		return true;
		};

	auto circle = [](int32_t cr, int32_t cx, int32_t cz, int32_t r, int32_t& px, int32_t& pz) -> bool {
		int32_t dx = px - cx;
		int32_t dz = pz - cz;
		int32_t minR = r + cr;
		int32_t dist = dx * dx + dz * dz;

		if (dist > minR * minR) { return false; }

		dist = static_cast<int32_t>(sqrt(static_cast<uint32_t>(dist)));

		if (dist <= 0) { return false; }

		int32_t delta = minR - dist;

		px += (dx + 8) * delta / dist;
		pz += (dz + 8) * delta / dist;

		return true;
		};

	auto rect = [circle](int32_t minX, int32_t minZ, int32_t maxX, int32_t maxZ, int32_t r, int32_t& px, int32_t& pz) -> bool {
		auto x_clamp = [](int32_t x, int32_t a, int32_t b) { return (x < a) ? a : ((x > b) ? b : x); };
		int32_t closestX = x_clamp(px, minX, maxX);
		int32_t closestZ = x_clamp(pz, minZ, maxZ);
		return circle(0, closestX, closestZ, r, px, pz);
		};

	auto rhombus = [line](int32_t minX, int32_t minZ, int32_t maxX, int32_t maxZ, int32_t r, int32_t& px, int32_t& pz) -> bool {
		int32_t cx = (minX + maxX) >> 1;
		int32_t cz = (minZ + maxZ) >> 1;

		if (px < cx)
		{
			if (pz < cz) { return line(cx, minZ, minX, cz, r, px, pz); }
			else { return line(minX, cz, cx, maxZ, r, px, pz); }
		}
		else
		{
			if (pz < cz) { return line(maxX, cz, cx, minZ, r, px, pz); }
			else { return line(cx, maxZ, maxX, cz, r, px, pz); }
		}
		};

	if (!(std::to_underlying(ShapeType) & (SLOPE_A | SLOPE_B | SLOPE_C | SLOPE_D)))
	{
		if (py <= maxY && py < minY) { return false; }
	}

	if (b_HitSlopeX)
	{
		if (px - pw < minX - pw || px - pw > maxX - pw) { return true; }
		if (px - pw < m_Slope.minX - pw)
		{
			py = m_Slope.maxY;
			b_HitSlopeX = false;
		}
		if (px - pw > m_Slope.maxX - pw)
		{
			py = m_Slope.minY;
			b_HitSlopeX = false;
		}
	}

	else if (b_HitSlopeZ)
	{
		if (pz + pd < minZ + pd || pz - pd > maxZ + pd) { return true; }
		if (pz + pd < m_Slope.minZ + pd)
		{
			py = m_Slope.minY;
			b_HitSlopeZ = false;
		}
		if (pz - pd > m_Slope.maxZ + pd)
		{
			py = m_Slope.maxY;
			b_HitSlopeZ = false;
		}
	}

	else if (!(std::to_underlying(ShapeType) & (DIAGONAL_A | DIAGONAL_B | DIAGONAL_C | DIAGONAL_D)))
	{
		if (px + pw < minX || px - pw > maxX || pz + pd < minZ || pz - pd > maxZ) { return false; }
	}

	switch (ShapeType)
	{
		case Shape_Type::Rectangle: return rect(minX, minZ, maxX, maxZ, pw, px, pz);
		case Shape_Type::Diagonal_A: return line(maxX, minZ, minX, maxZ, pw, px, pz);
		case Shape_Type::Diagonal_B: return line(minX, minZ, maxX, maxZ, pw, px, pz);
		case Shape_Type::Diagonal_C: return line(minX, minZ, maxX, maxZ, pw, px, pz);
		case Shape_Type::Diagonal_D: return line(minX, maxZ, maxX, minZ, pw, px, pz);
		case Shape_Type::Rhombus: return rhombus(minX, minZ, maxX, maxZ, pw, px, pz);
		case Shape_Type::Circle:
		{
			int32_t cr = (maxX - minX) >> 1;
			return circle(cr, minX + cr, minZ + cr, pw, px, pz);
		}
		case Shape_Type::OblongX:
		{
			int32_t cr = (maxZ - minZ) >> 1;
			if (px < minX + cr) { return circle(cr, minX + cr, minZ + cr, pw, px, pz); }
			if (px > maxX - cr) { return circle(cr, maxX - cr, minZ + cr, pw, px, pz); }
			return rect(minX + cr, minZ, maxX - cr, maxZ, pw, px, pz);
		}
		case Shape_Type::OblongZ:
		{
			int32_t cr = (maxX - minX) >> 1;
			if (pz < minZ + cr) { return circle(cr, minX + cr, minZ + cr, pw, px, pz); }
			if (pz > maxZ - cr) { return circle(cr, minX + cr, maxZ - cr, pw, px, pz); }
			return rect(minX, minZ + cr, maxX, maxZ - cr, pw, px, pz);
		}
		case Shape_Type::Slope_A:
		{
			m_Slope = { minX, maxX, minY, maxY, minZ, maxZ };
			int32_t denom = maxX - minX;
			int32_t clamped_px = (px < minX) ? minX : (px > maxX ? maxX : px);
			int32_t num = (denom != 0) ? (clamped_px - minX) : 0;
			py = minY + ((num * (maxY - minY)) / (denom != 0 ? denom : 1));
			return b_HitSlopeX = true;
		}
		case Shape_Type::Slope_B:
		{
			m_Slope = { minX, maxX, minY, maxY, minZ, maxZ };
			int32_t denom = maxX - minX;
			int32_t clamped_px = (px < minX) ? minX : (px > maxX ? maxX : px);
			int32_t num = (denom != 0) ? (maxX - clamped_px) : 0;
			py = minY + ((num * (maxY - minY)) / (denom != 0 ? denom : 1));
			return b_HitSlopeX = true;
		}
		case Shape_Type::Slope_C:
		{
			m_Slope = { minX, maxX, minY, maxY, minZ, maxZ };
			int32_t denom = maxZ - minZ;
			int32_t clamped_pz = (pz < minZ) ? minZ : (pz > maxZ ? maxZ : pz);
			int32_t num = (denom != 0) ? (clamped_pz - minZ) : 0;
			py = minY + ((num * (maxY - minY)) / (denom != 0 ? denom : 1));
			return b_HitSlopeZ = true;
		}
		case Shape_Type::Slope_D:
		{
			m_Slope = { minX, maxX, minY, maxY, minZ, maxZ };
			int32_t denom = maxZ - minZ;
			int32_t clamped_pz = (pz < minZ) ? minZ : (pz > maxZ ? maxZ : pz);
			int32_t num = (denom != 0) ? (maxZ - clamped_pz) : 0;
			py = minY + ((num * (maxY - minY)) / (denom != 0 ? denom : 1));
			return b_HitSlopeZ = true;
		}
	}

	return false;
}

bool Resident_Evil_Geometry::CameraSwitch(VECTOR2& Position, const std::int16_t Xz[4][2])
{
	int32_t px, pz;

	px = Position.x - Xz[0][0];
	pz = Position.z - Xz[0][1];
	if ((Xz[1][1] - Xz[0][1]) * px < (Xz[1][0] - Xz[0][0]) * pz) { return false; }
	if ((Xz[3][0] - Xz[0][0]) * pz < (Xz[3][1] - Xz[0][1]) * px) { return false; }

	px = Position.x - Xz[2][0];
	pz = Position.z - Xz[2][1];
	if ((Xz[1][0] - Xz[2][0]) * pz < (Xz[1][1] - Xz[2][1]) * px) { return false; }
	if ((Xz[3][1] - Xz[2][1]) * px < (Xz[3][0] - Xz[2][0]) * pz) { return false; }

	return true;
}