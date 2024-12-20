/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#pragma once


#include <lib_gte.h>

#include <std_matrix.h>

#include <std_image.h>


class Resident_Evil_Camera final {
private:

	/*
		Resident Evil 2 Dual Shock (SLUS_007.48)
	*/
	MATRIX Set_view(VECTOR2 Eye, VECTOR2 At);

	/*
		Move
	*/
	Resident_Evil_Camera(Resident_Evil_Camera&&) = delete;
	Resident_Evil_Camera& operator = (Resident_Evil_Camera&&) = delete;


	/*
		Copy
	*/
	Resident_Evil_Camera(const Resident_Evil_Camera&) = delete;
	Resident_Evil_Camera& operator = (const Resident_Evil_Camera&) = delete;

public:

	/*
		View Matrix
	*/
	Standard_Matrix View;

	/*
		Projection Matrix
	*/
	Standard_Matrix Projection;

	/*
		World Matrix
	*/
	Standard_Matrix World;

	/*
		Orthogonal Matrix
	*/
	Standard_Matrix Orthogonal;

#if MSTD_DX9
	/*
		Background Texture
	*/
	IDirect3DTexture9* Background;

	/*
		Background Vertices (vec4t)
	*/
	IDirect3DVertexBuffer9* BackgroundVert;
#else
	std::unique_ptr<Standard_Image> Background;
	std::vector<vec4t> BackgroundVert;
#endif

	/*
		Ortho Width
	*/
	float m_Width;

	/*
		Ortho 
	*/
	float m_Height;

	/*
		If true, entire scene is horizontally mirrored
	*/
	bool b_Mirror;

	/*
		Construction
	*/
	explicit Resident_Evil_Camera(void) :
		View(),
		Projection(),
		Orthogonal(),
		Background(nullptr),
		BackgroundVert(),
		m_Width(0.0f),
		m_Height(0.0f),
		b_Mirror(false)
	{
		//Set_view(0x6DD4 >> 7, { -16000, -7200, -16000 }, { 0, 7200, 0 });
		vec3 translation{ 0.0f, 0.0f, 0.0f };
		vec3 rotation{ 0.0f, 0.0f, 0.0f };
		vec3 scale{ 1.0f, 1.0f, 1.0f };
		World.SetWorld(translation, rotation, scale);
	}


	/*
		Deconstruction
	*/
	virtual ~Resident_Evil_Camera(void) noexcept
	{
#if MSTD_DX9
		if (Background) { Background->Release(); Background = nullptr; }
		if (BackgroundVert) { BackgroundVert->Release(); BackgroundVert = nullptr; }
#endif
	}

	/*
		Set view and projection matrices
	*/
	void Set_view(std::uint32_t ViewR, VECTOR2 Eye, VECTOR2 At);

	/*
		Set ortho
	*/
	void SetOrtho(float Width, float Height);

	/*
		Get background vertices
	*/
	[[nodiscard]] std::vector<vec4t> GetVert(void) const;

};