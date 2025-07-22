/*
*
*	Megan Grass
*	March 07, 2024
*
*/


#pragma once

#include <lib_gte.h>

#if MSTD_DX9
#include <std_dx9.h>
#endif


enum class Resident_Evil_Triangle_Type : std::uint32_t {
	Naname_A,	// \| SouthEast
	Naname_B,	// |/ SouthWest
	Naname_C,	// /| NorthWest
	Naname_D,	// |\ NorthEast
	Slope_A,	// Adjacent = EAST, Opposite = WEST
	Slope_B,	// Adjacent = WEST, Opposite = EAST
	Slope_C,	// Adjacent = NORTH, Opposite = SOUTH
	Slope_D,	// Adjacent = SOUTH, Opposite = NORTH
	Stairs		// Same appearance as Slope, but with non-fixed height
};

static Resident_Evil_Triangle_Type operator | (Resident_Evil_Triangle_Type _Mode0, Resident_Evil_Triangle_Type _Mode1)
{
	return static_cast<Resident_Evil_Triangle_Type>(std::to_underlying(_Mode0) | std::to_underlying(_Mode1));
}


class Resident_Evil_Geometry final {
private:

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

	std::shared_ptr<Standard_Matrix> SetWorldMatrix(const SHAPEVECTOR& Vec, const SVECTOR2& Rotation, const vec3& Centroid) const;

	void DrawShape(const DRAWSHAPE& Shape) const;

#if MSTD_DX9

	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> Indices4p;

	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> IndicesBoxWire;
	std::unique_ptr<IDirect3DIndexBuffer9, IDirect3DDelete9<IDirect3DIndexBuffer9>> IndicesBox;

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

	// Draw Collision Polygons On/Off
	bool b_DrawCollision;

#if MSTD_DX9

	// Direct-X 9 Render Context
	std::shared_ptr<Standard_DirectX_9> Render;

	explicit Resident_Evil_Geometry(std::shared_ptr<Standard_DirectX_9> render, std::shared_ptr<Sony_PlayStation_GTE> gte) :
		GTE(gte),
		Render(render),
		World(std::make_shared<Standard_Matrix>()),
		b_DrawCollision(true)
	{
	}
#else
#error "Resident_Evil_Geometry does not have a render pipeline"
#endif

	// Initialize Geometry
	void Init(void);

	// Draw 4-point Rectangle
	void Draw4p(const std::int16_t Xz[4][2], std::int32_t Y, DWORD Color) const;

	// Draw Box
	void DrawBox(SHAPEVECTOR Vec, SVECTOR2 Rotation, DWORD Color, bool Solid = false) const;

	// Draw Triangle
	void DrawTriangle(SHAPEVECTOR Vec, SVECTOR2 Rotation, DWORD Color, bool Solid = false, Resident_Evil_Triangle_Type Type = Resident_Evil_Triangle_Type::Naname_A) const;

	// Draw Rhombus
	void DrawRhombus(SHAPEVECTOR Vec, SVECTOR2 Rotation, DWORD Color, bool Solid = false) const;

	// Draw Cylinder
	void DrawCylinder(SHAPEVECTOR Vec, SVECTOR2 Rotation, DWORD Color, bool Solid = false) const;

};