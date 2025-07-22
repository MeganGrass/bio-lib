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

#include <bio_common.h>


class Resident_Evil_Camera final :
	private Resident_Evil_Common {
private:

	// Set View and Projection Matrix (GTE)
	MATRIX2 Set_view(VECTOR2 Eye, VECTOR2 At) const;

	// Move
	Resident_Evil_Camera(Resident_Evil_Camera&&) = delete;
	Resident_Evil_Camera& operator = (Resident_Evil_Camera&&) = delete;

	// Copy
	Resident_Evil_Camera(const Resident_Evil_Camera&) = delete;
	Resident_Evil_Camera& operator = (const Resident_Evil_Camera&) = delete;

	Standard_FileSystem FS;

	struct PROJECTION {
		constexpr static float Left = -160.0f;
		constexpr static float Right = 160.0f;
		constexpr static float Top = 120.0f;
		constexpr static float Bottom = -120.0f;
		constexpr static float Near = 1.0f;
		constexpr static float Far = 4096.0f;
		float FovY;
		float FovX;
		float ScaleX;
		float ScaleY;
		float ScaleZ;
		float OffsetX;
		float OffsetY;
		float OffsetZ;
		explicit PROJECTION(void) :
			FovY(2.0f * std::atan(120.0f / 0xDB)),
			FovX(2.0f * std::atan(160.0f / 0xDB)),
			ScaleX(1.0f / std::tan(FovX / 2.0f)),
			ScaleY(1.0f / std::tan(FovY / 2.0f)),
			ScaleZ(Far / (Near - Far)),
			OffsetX((Left + Right) / (Left - Right)),
			OffsetY((Top + Bottom) / (Top - Bottom)),
			OffsetZ(Near* Far / (Near - Far)) {}
		explicit PROJECTION(std::uint32_t FOV) :
			FovY(2.0f * std::atan(120.0f / FOV)),
			FovX(2.0f * std::atan(160.0f / FOV)),
			ScaleX(1.0f / std::tan(FovX / 2.0f)),
			ScaleY(1.0f / std::tan(FovY / 2.0f)),
			ScaleZ(Far / (Near - Far)),
			OffsetX((Left + Right) / (Left - Right)),
			OffsetY((Top + Bottom) / (Top - Bottom)),
			OffsetZ(Near * Far / (Near - Far)) {}
	};

public:

	Standard_String Str;

	// Sony PlayStation (1994) Geometry Transformation Engine
	std::shared_ptr<Sony_PlayStation_GTE> GTE;

	// Matrix
	std::shared_ptr<Standard_Matrix> Orthogonal, View, Projection, World;

	// Field of View
	std::uint32_t m_FOV;

	// Position
	VECTOR2 m_Eye;

	// Target
	VECTOR2 m_At;

	// Viewport Size
	float m_OrthoWidth, m_OrthoHeight;

	// Prerendered Background Texture Flip
	bool b_HorzFlipTex, b_VertFlipTex;

	// Prerendered Background File Directory
	std::filesystem::path m_Path;

	// Prerendered Background ID
	std::uint8_t m_Cut, m_CutMax;

	// Prerendered Background View On/Off
	bool b_ViewBackground;

	// Prerendered Background Texture Size
	float m_TexWidth, m_TexHeight;

	// Top-Down Perspective On/Off
	bool b_ViewTopDown;

	// Top-Down Perspective Camera Position
	float m_Cx, m_Cy, m_Cz;

	// Draw Switch Vectors On/Off
	bool b_DrawSwitch;

	// Model Editor Perspective
	bool b_ViewModelEdit;

#if MSTD_DX9

	// Direct-X 9 Render Context
	std::shared_ptr<Standard_DirectX_9> Render;

	// Prerendered Background Texture
	std::unique_ptr<IDirect3DTexture9, IDirect3DDelete9<IDirect3DTexture9>> m_Background;

	// Prerendered Background Texture Vertex Buffer (vec4t)
	std::unique_ptr<IDirect3DVertexBuffer9, IDirect3DDelete9<IDirect3DVertexBuffer9>> m_BackgroundVert;

	explicit Resident_Evil_Camera(std::shared_ptr<Standard_DirectX_9> render, std::shared_ptr<Sony_PlayStation_GTE> gte) :
		GTE(gte),
		Render(render),
		m_Background(nullptr),
		m_BackgroundVert(nullptr),
		Orthogonal(std::make_shared<Standard_Matrix>()),
		View(std::make_shared<Standard_Matrix>()),
		Projection(std::make_shared<Standard_Matrix>()),
		World(std::make_shared<Standard_Matrix>(vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 1.0f, 1.0f, 1.0f })),
		m_FOV(0x6DD4 >> 7),
		m_Eye{ -16000, -7200, -16000 },
		m_At{ 0, 7200, 0 },
		m_OrthoWidth(320.0f),
		m_OrthoHeight(240.0f),
		b_HorzFlipTex(false),
		b_VertFlipTex(false),
		m_Path(),
		m_Cut(0),
		m_CutMax(0),
		b_ViewBackground(false),
		m_TexWidth(0.0f),
		m_TexHeight(0.0f),
		b_ViewTopDown(false),
		m_Cx(0.0f),
		m_Cy(50.0f),
		m_Cz(0.0f),
		b_DrawSwitch(true),
		b_ViewModelEdit(false) {}
#else
#error "Resident_Evil_Camera does not have a render pipeline"
#endif

	~Resident_Evil_Camera(void) = default;

	// Shutdown
	void Shutdown(void) noexcept;

	/*
		Reset Camera
		- Meta data, background texture, fov and all matrices are reset
	*/
	void Reset(void);

	// Set Meta Data
	void SetMeta(std::filesystem::path _Path, std::uint8_t _Stage, std::uint8_t _Room, std::uint8_t CutMax) noexcept;

	// Set Orthographic Projection Matrix
	void SetOrtho(float Width, float Height);

	/*
		Set Prerendered Background Image
		- SetMeta must be called before this function
	*/
	std::uint8_t SetImage(std::uint8_t iCut);

	// Get Prerendered Background Vertices
	[[nodiscard]] std::vector<vec4t> GetImageVert(void) const;

	// Set View and Projection Matrix in Top-Down Perspective
	void SetTopDownPerspective(void);

	// Set View and Projection Matrix
	void Set(std::uint32_t FOV, VECTOR2 Eye, VECTOR2 At);

};