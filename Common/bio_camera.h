/*
*
*	Megan Grass
*	April 16, 2024
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
public:

	// Sony PlayStation (1994) Native Screen Resolution
	constexpr static float m_NativeWidth = 320.0f, m_NativeHeight = 240.0f;

	// Sony PlayStation (1994) Geometry Transformation Engine
	std::shared_ptr<Sony_PlayStation_GTE> GTE;

private:

	// Set View Matrix (GTE)
	MATRIX2 Set_view(VECTOR2 Eye, VECTOR2 At) const;

	// Move
	Resident_Evil_Camera(Resident_Evil_Camera&&) = delete;
	Resident_Evil_Camera& operator = (Resident_Evil_Camera&&) = delete;

	// Copy
	Resident_Evil_Camera(const Resident_Evil_Camera&) = delete;
	Resident_Evil_Camera& operator = (const Resident_Evil_Camera&) = delete;

	struct PROJECTION {
		constexpr static float Left = -(m_NativeWidth * 0.5f);
		constexpr static float Right = (m_NativeWidth * 0.5f);
		constexpr static float Top = (m_NativeHeight * 0.5f);
		constexpr static float Bottom = -(m_NativeHeight * 0.5f);
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
			FovY(2.0f * std::atan((m_NativeHeight * 0.5f) / 0xDB)),
			FovX(2.0f * std::atan((m_NativeWidth * 0.5f) / 0xDB)),
			ScaleX(1.0f / std::tan(FovX / 2.0f)),
			ScaleY(1.0f / std::tan(FovY / 2.0f)),
			ScaleZ(Far / (Near - Far)),
			OffsetX((Left + Right) / (Left - Right)),
			OffsetY((Top + Bottom) / (Top - Bottom)),
			OffsetZ(Near * Far / (Near - Far)) {}
		explicit PROJECTION(std::uint32_t FOV) :
			FovY(2.0f * std::atan((m_NativeHeight * 0.5f) / FOV)),
			FovX(2.0f * std::atan((m_NativeWidth * 0.5f) / FOV)),
			ScaleX(1.0f / std::tan(FovX / 2.0f)),
			ScaleY(1.0f / std::tan(FovY / 2.0f)),
			ScaleZ(Far / (Near - Far)),
			OffsetX((Left + Right) / (Left - Right)),
			OffsetY((Top + Bottom) / (Top - Bottom)),
			OffsetZ(Near * Far / (Near - Far)) {}
	};

public:

	Standard_String Str;

	// Matrix
	std::shared_ptr<Standard_Matrix> Orthogonal, View, Projection, World;

	// Field of View
	std::uint32_t m_FOV, m_ModelFOV;

	// Position
	VECTOR2 m_Eye, m_ModelEye;

	// Target
	VECTOR2 m_At, m_ModelAt;

	// Viewport Size
	float m_OrthoWidth, m_OrthoHeight;

	// Viewport Scale
	float m_OrthoScaleX, m_OrthoScaleY;

	// Perspective Flip
	bool b_HorzFlip, b_VertFlip;

	// Prerendered Background File Directory
	std::filesystem::path m_Path;

	// Prerendered Background ID
	using Resident_Evil_Common::Cut;
	using Resident_Evil_Common::CutMax;

	// Prerendered Background View On/Off
	bool b_ViewBackground;

	// Prerendered Background Texture Size
	float m_TexWidth, m_TexHeight;

	// Sprite View On/Off
	bool b_ViewSprite;

	// Sprite Texture Size
	float m_TexSprWidth, m_TexSprHeight;

	// Top-Down Perspective On/Off
	bool b_ViewTopDown;

	// Top-Down Perspective Camera Position
	float m_Cx, m_Cy, m_Cz;

	// Draw Line (Eye, At)
	bool b_DrawLine;

	// Draw Switch Vectors On/Off
	bool b_DrawSwitch;

	// Model Editor Perspective
	bool b_ViewModelEdit;

#if MSTD_DX9

	// Direct-X 9 Render Context
	std::shared_ptr<Standard_DirectX_9> Render;

	// Prerendered Background Texture
	std::unique_ptr<IDirect3DTexture9, IDirect3DDelete9<IDirect3DTexture9>> m_Background;

	// Sprite Texture
	std::unique_ptr<IDirect3DTexture9, IDirect3DDelete9<IDirect3DTexture9>> m_Sprite;

	// Prerendered Background Texture Vertex Buffer (vec4t)
	std::unique_ptr<IDirect3DVertexBuffer9, IDirect3DDelete9<IDirect3DVertexBuffer9>> m_BackgroundVert;

	explicit Resident_Evil_Camera(std::shared_ptr<Standard_DirectX_9> render, std::shared_ptr<Sony_PlayStation_GTE> gte) :
		GTE(gte),
		Render(render),
		m_Background(nullptr),
		m_BackgroundVert(nullptr),
		m_Sprite(nullptr),
		Orthogonal(std::make_shared<Standard_Matrix>()),
		View(std::make_shared<Standard_Matrix>()),
		Projection(std::make_shared<Standard_Matrix>()),
		World(std::make_shared<Standard_Matrix>(vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 1.0f, 1.0f, 1.0f })),
		m_FOV(0x6DD4 >> 7),
		m_Eye{ -16000, -7200, -16000 },
		m_At{ 0, 7200, 0 },
		m_ModelFOV(0x6DD4 >> 7),
		m_ModelEye{ 5400, -1800, 0 },
		m_ModelAt{ 0, -1800 , 0 },
		m_OrthoWidth(m_NativeWidth),
		m_OrthoHeight(m_NativeHeight),
		m_OrthoScaleX(m_OrthoWidth / m_NativeWidth),
		m_OrthoScaleY(m_OrthoHeight / m_NativeHeight),
		b_HorzFlip(false),
		b_VertFlip(false),
		m_Path(),
		b_ViewBackground(true),
		m_TexWidth(0.0f),
		m_TexHeight(0.0f),
		b_ViewSprite(true),
		m_TexSprWidth(0.0f),
		m_TexSprHeight(0.0f),
		b_ViewTopDown(false),
		m_Cx(0.0f),
		m_Cy(50.0f),
		m_Cz(0.0f),
		b_DrawLine(true),
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
	void SetMeta(std::filesystem::path _Path, std::uint8_t _Stage, std::uint8_t _Room, std::uint8_t _CutMax) noexcept;

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