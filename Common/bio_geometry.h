/*
*
*	Megan Grass
*	June 28, 2025
*
*/


#pragma once

#include <lib_gte.h>

#if MSTD_DX9
#include <std_dx9.h>
#endif


enum class Shape_Type : std::int32_t
{
	Rectangle = (1 << 0),
	Diagonal_A = (1 << 1),	//  \  // Plane
	Diagonal_B = (1 << 2),	//  /  // Plane
	Diagonal_C = (1 << 3),	//  /  // Plane
	Diagonal_D = (1 << 4),	//  \  // Plane
	Rhombus = (1 << 5),
	Circle = (1 << 6),
	OblongX = (1 << 7),
	OblongZ = (1 << 8),
	Slope_A = (1 << 9),		// Adjacent = WEST, Opposite = EAST
	Slope_B = (1 << 10),	// Adjacent = EAST, Opposite = WEST
	Slope_C = (1 << 11),	// Adjacent = SOUTH, Opposite = NORTH
	Slope_D = (1 << 12),	// Adjacent = NORTH, Opposite = SOUTH
	Stairs = (1 << 13),
	Curve = (1 << 14)
};


class Resident_Evil_Geometry final {
private:

	// Shape Type Constants
	static constexpr std::uint32_t RECT = std::to_underlying(Shape_Type::Rectangle);
	static constexpr std::uint32_t DIAGONAL_A = std::to_underlying(Shape_Type::Diagonal_A);
	static constexpr std::uint32_t DIAGONAL_B = std::to_underlying(Shape_Type::Diagonal_B);
	static constexpr std::uint32_t DIAGONAL_C = std::to_underlying(Shape_Type::Diagonal_C);
	static constexpr std::uint32_t DIAGONAL_D = std::to_underlying(Shape_Type::Diagonal_D);
	static constexpr std::uint32_t RHOMBUS = std::to_underlying(Shape_Type::Rhombus);
	static constexpr std::uint32_t CIRCLE = std::to_underlying(Shape_Type::Circle);
	static constexpr std::uint32_t OBLONG_X = std::to_underlying(Shape_Type::OblongX);
	static constexpr std::uint32_t OBLONG_Z = std::to_underlying(Shape_Type::OblongZ);
	static constexpr std::uint32_t SLOPE_A = std::to_underlying(Shape_Type::Slope_A);
	static constexpr std::uint32_t SLOPE_B = std::to_underlying(Shape_Type::Slope_B);
	static constexpr std::uint32_t SLOPE_C = std::to_underlying(Shape_Type::Slope_C);
	static constexpr std::uint32_t SLOPE_D = std::to_underlying(Shape_Type::Slope_D);

	std::shared_ptr<Standard_Matrix> World;

	struct DRAWSHAPE
	{
		std::shared_ptr<Standard_Matrix> World;
		bool ResetWorld;
		const std::vector<vec3>& Vertice;
		DWORD Color;
		bool Solid;
		void* IndiceWire;
		void* Indice;
	};

	struct COLLISIONVECTOR
	{
		int32_t minX, maxX, minY, maxY, minZ, maxZ;
	};

	COLLISIONVECTOR m_Slope;

	bool b_HitSlopeX, b_HitSlopeZ;

	std::shared_ptr<Standard_Matrix> SetWorldMatrix(const SHAPEVECTOR& Vec, const VECTOR2& Rotation, const vec3& Centroid) const;

	void DrawShape(const DRAWSHAPE& Shape) const;

#if MSTD_DX9

	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> Indices4p;

	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> IndicesBoxWire;
	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> IndicesBox;

	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> IndicesPlaneWire;
	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> IndicesPlane;

	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> IndicesTriWire;
	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> IndicesTri;

	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> IndicesRhombusWire;
	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> IndicesRhombus;

	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> IndicesCylinderWire;
	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> IndicesCylinder;

#endif

public:

	// Sony PlayStation (1994) Geometry Transformation Engine
	std::shared_ptr<Sony_PlayStation_GTE> GTE;

	// Collision Index
	std::size_t iObject, iObjectMin, iObjectMax;

	// Entity Collision Detection On/Off
	bool b_CollisionDetection;

	// Automatic Camera Switching On/Off
	bool b_SwitchDetection;

	// Draw Collision Polygons On/Off
	bool b_DrawCollision;

	// Draw Collision as Solid Color On/Off
	bool b_SolidCollision, b_SolidCollisionAll;

	/*
		Draw Collision as Solid Shape On/Off
		 - when false, collision polygons are drawn as 2D (no height)
	*/
	bool b_ShapeCollision, b_ShapeCollisionAll;

	// Highlight Collision Index On/Off
	bool b_HighlightCollision;

	// Draw Navigational Polygons On/Off
	bool b_DrawBlock;

	// Draw Floor Sound Polygons On/Off
	bool b_DrawFloor;

#if MSTD_DX9

	// Direct-X 9 Render Context
	std::shared_ptr<Standard_DirectX_9> Render;

	explicit Resident_Evil_Geometry(std::shared_ptr<Standard_DirectX_9> render, std::shared_ptr<Sony_PlayStation_GTE> gte) :
		GTE(gte),
		Render(render),
		World(std::make_shared<Standard_Matrix>()),
		m_Slope{},
		b_HitSlopeX(false),
		b_HitSlopeZ(false),
		iObject(0),
		iObjectMin(0),
		iObjectMax(0),
		b_CollisionDetection(true),
		b_SwitchDetection(true),
		b_DrawCollision(true),
		b_SolidCollision(true),
		b_SolidCollisionAll(false),
		b_ShapeCollision(true),
		b_ShapeCollisionAll(false),
		b_HighlightCollision(true),
		b_DrawBlock(true),
		b_DrawFloor(true)
	{
	}
#else
#error "Resident_Evil_Geometry does not have a render pipeline"
#endif

	// Initialize Geometry
	void Init(void);

	// Shutdown
	void Shutdown(void) noexcept;

	// Draw 4-point Rectangle
	void Draw4p(const std::int16_t Xz[4][2], std::int32_t Y, DWORD Color, bool Solid = false) const;

	// Draw Box
	void DrawBox(SHAPEVECTOR Vec, VECTOR2 Rotation, DWORD Color, bool Solid = false) const;

	// Draw Diagonal
	void DrawDiagonal(SHAPEVECTOR Vec, VECTOR2 Rotation, DWORD Color, bool Solid = false, Shape_Type Type = Shape_Type::Diagonal_A) const;

	// Draw Triangle
	void DrawTriangle(SHAPEVECTOR Vec, VECTOR2 Rotation, DWORD Color, bool Solid = false, Shape_Type Type = Shape_Type::Slope_A) const;

	// Draw Rhombus
	void DrawRhombus(SHAPEVECTOR Vec, VECTOR2 Rotation, DWORD Color, bool Solid = false) const;

	// Draw Cylinder
	void DrawCylinder(SHAPEVECTOR Vec, VECTOR2 Rotation, DWORD Color, bool Solid = false) const;

	// Collision Detection
	bool Collision(VECTOR2& Position, const SIZEVECTOR Hitbox, const SHAPEVECTOR Shape, const Shape_Type ShapeType);

	// Collision 4P Detection
	bool Collision4P(VECTOR2& Position, const std::int16_t Xz[4][2]);

	// Collision Box Detection
	bool CollisionBox(VECTOR2& Position, const SIZEVECTOR Hitbox, const std::int16_t X, const std::int16_t Z, const std::uint16_t W, const std::uint16_t D);

	// Collision Hitbox Detection
	bool CollisionHitbox(VECTOR2& Position, const SIZEVECTOR Hitbox0, const VECTOR2& Position1, const SIZEVECTOR Hitbox1);

};