/*
*
*	Megan Grass
*	July 13, 2025
*
*/


#pragma once

#include <sony_texture.h>

#include <sony_model.h>

#include <bio2_md1.h>

#include <bio3_md2.h>

#include <bio_animation.h>

#include <array>

#if MSTD_DX9
#include <std_dx9.h>


struct DX9_OBJECT
{
	std::size_t iTexture{};
	std::unique_ptr<IDirect3DVertexBuffer9, IDirect3DDelete9<IDirect3DVertexBuffer9>> Data;
};


struct DX9_MODEL
{
	float TextureWidth{};
	float TextureHeight{};
	std::vector<std::vector<DX9_OBJECT>> Object;
	std::vector<std::unique_ptr<IDirect3DTexture9, IDirect3DDelete9<IDirect3DTexture9>>> Texture;
};
#endif


enum class ModelType : std::int32_t
{
	None = (0 << 0),
	Object = (1 << 1),
	Player = (1 << 2),
	SubPlayer = (1 << 3),
	Enemy = (1 << 4)
};


class Resident_Evil_Model :
	private Resident_Evil_Common {
private:

	// Standard String for messages/debugging
	Standard_String Str;

	// Filename
	std::filesystem::path m_Filename, m_WeaponFilename;

	// Animation Type Constants
	static constexpr AnimationIndex NORMAL = AnimationIndex::Normal;
	static constexpr AnimationIndex NORMAL_EX0 = AnimationIndex::NormalEx0;
	static constexpr AnimationIndex NORMAL_EX1 = AnimationIndex::NormalEx1;
	static constexpr AnimationIndex DAMAGE = AnimationIndex::Damage;
	static constexpr AnimationIndex WEAPON = AnimationIndex::Weapon;
	static constexpr AnimationIndex WEAPON_EX0 = AnimationIndex::WeaponEx0;
	static constexpr AnimationIndex WEAPON_EX1 = AnimationIndex::WeaponEx1;
	static constexpr AnimationIndex ROOM = AnimationIndex::Room;

	// Matrix
	std::shared_ptr<Standard_Matrix> World;

	// Interactive/Collision Size Vector
	SIZEVECTOR m_Hitbox;

	// Transformation
	VECTOR2 m_Position, m_Rotation, m_Scale, m_EditorPosition, m_EditorRotation, m_EditorScale;

	// Texture
	std::unique_ptr<Sony_PlayStation_Texture> m_Texture, m_WeaponTexture;

	// Model
	std::unique_ptr<Sony_PlayStation_Model> m_Model, m_WeaponModel;

	// Binary Chunk (some EMDs in Bio2, every type in Bio3; currently unknown purpose, likely vertice blending/mesh deform...)
	std::vector<std::uint8_t> m_Binary00, m_Binary01, m_WeaponBinary;

	// Animation Data
	std::array<std::shared_ptr<Resident_Evil_Animation>, std::to_underlying(AnimationIndex::Count)> m_Animations;

	// Animation Index ID
	AnimationIndex m_AnimationIndex;

	// Get data pointers from file archive (EMD/EMW/PLD/PLW)
	std::vector<std::uint32_t> GetDataPtr(StdFile& File, std::uintmax_t _FileBeginPtr);

#if MSTD_DX9
	// Direct-X 9 Model
	std::unique_ptr<DX9_MODEL> m_DX9Model, m_DX9WeaponModel;

	// Clear DX9 model data
	void CloseModelDX9(void)
	{
		if (m_DX9Model)
		{
			m_DX9Model->Object.clear();
			m_DX9Model->Texture.clear();
			m_DX9Model.reset(nullptr);
		}
	}

	// Clear DX9 weapon data
	void CloseWeaponDX9(void)
	{
		if (m_DX9WeaponModel)
		{
			m_DX9WeaponModel->Object.clear();
			m_DX9WeaponModel->Texture.clear();
			m_DX9WeaponModel.reset(nullptr);
		}
	}
#endif

public:

	explicit Resident_Evil_Model(void) :
		m_Filename{},
		m_Position{ 0, 0, 0 },
		m_Rotation{ 0, 0, 0 },
		m_Scale{ ONE, ONE, ONE },
		m_EditorPosition{ 0, 0, 0 },
		m_EditorRotation{ 0, 0, 0 },
		m_EditorScale{ ONE, ONE, ONE },
		World(std::make_shared<Standard_Matrix>()),
		m_Texture(std::make_unique<Sony_PlayStation_Texture>()),
		m_WeaponTexture(std::make_unique<Sony_PlayStation_Texture>()),
		m_Model(std::make_unique<Sony_PlayStation_Model>()),
		m_WeaponModel(std::make_unique<Sony_PlayStation_Model>()),
		m_AnimationIndex(NORMAL),
		m_Hitbox{},
		b_Active(true),
		b_Drawing(false),
		b_Loop(true),
		b_Play(true),
		b_EditorMode(false),
		b_Dither(true),
		b_LockPosition(false),
		b_DrawWireframe(false),
		b_DrawTextured(true),
		b_DrawSolidColor(false),
		b_DrawSkeletonMesh(false),
		b_DrawReference(false),
		b_DrawHitbox(false),
		b_DrawAllObjects(false),
		b_DrawSingleObject(false),
		b_DrawWeapon(false),
		iObject(0),
		iObjectMin(0),
		iObjectMax(0),
		iWeaponObject(0),
		iWeaponObjectMin(0),
		iWeaponObjectMax(0),
		iClip(0),
		iFrame(0),
		iRoom(0),
		iRoomMin(0),
		iRoomMax(0)
#if MSTD_DX9
		,m_TextureFilter(D3DTEXF_NONE),
		m_DX9Model(nullptr),
		m_DX9WeaponModel(nullptr)
#endif
	{
		for (size_t i = 0; i < m_Animations.size(); ++i)
		{
			m_Animations[i] = std::make_shared<Resident_Evil_Animation>();
			m_Animations[i]->SetType(Resident_Evil_Animation_Type(1 << i));
		}
		m_Model->IgnoreMagic(true);
		m_WeaponModel->IgnoreMagic(true);
	}

	~Resident_Evil_Model(void) = default;

	using Resident_Evil_Common::GameType;

	// Sony PlayStation (1994) Geometry Transformation Engine
	std::shared_ptr<Sony_PlayStation_GTE> GTE;

#if MSTD_DX9

	// Direct-X 9 Render Context
	std::shared_ptr<Standard_DirectX_9> Render;

	// Texture Filter
	D3DTEXTUREFILTERTYPE m_TextureFilter;

	// Export texture and standardized model to Direct-X 9 buffer
	std::unique_ptr<DX9_MODEL> ExportDX9(std::unique_ptr<FIXED_MODEL>& Model, std::unique_ptr<Sony_PlayStation_Texture>& Texture) const;

	// Direct-X 9 Model
	std::unique_ptr<DX9_MODEL>& ModelDX9(void) noexcept { return m_DX9Model; }

	// Direct-X 9 Weapon Model
	std::unique_ptr<DX9_MODEL>& WeaponModelDX9(void) noexcept { return m_DX9WeaponModel; }

#endif

	// Will the model be drawn?
	bool b_Active;

	// Any model objects currently being drawn?
	bool b_Drawing;

	// Is keyframe processing active?
	bool b_Play;

	// Will keyframe processing loop?
	bool b_Loop;

	/*
		Editor Mode
		 - editor position, rotation and scale will be used
	*/
	bool b_EditorMode;

	/*
		Sony PlayStation (1994) Dithering Pixel Shader
		 - simple passthrough pixel shader will be used otherwise
	*/
	bool b_Dither;

	/*
		Lock model in position
		 - when processing keyframes, model will not move backward, forward, etc.
	*/
	bool b_LockPosition;

	/*
		Draw Wireframe
		 - polygons will be drawn as wireframe
	*/
	bool b_DrawWireframe;

	/*
		Draw Solid Color
		 - polygons will be drawn as solid color
	*/
	bool b_DrawSolidColor;

	/*
		Draw Textured
		 - polygons will be drawn as textured
	*/
	bool b_DrawTextured;

	/*
		Draw Skeleton
		 - skeleton mesh will be drawn
	*/
	bool b_DrawSkeletonMesh;

	/*
		Ignore keyframes in DrawFrame
		 - origin, speed and rotation are ignored
		 - reference skeleton is used instead
	*/
	bool b_DrawReference;

	/*
		Draw hitbox
		 - draw model's interactive/collision hitbox
	*/
	bool b_DrawHitbox;

	/*
		Draw all objects
		 - keyframes and skeleton are ignored
	*/
	bool b_DrawAllObjects;

	/*
		Draw iObject only
		 - keyframes and skeleton are ignored
	*/
	bool b_DrawSingleObject;

	/*
		Draw weapon model
		 - iWeaponObject ID of model object is replaced with weapon model
	*/
	bool b_DrawWeapon;

	// Model object index
	std::size_t iObject, iObjectMin, iObjectMax;

	// ID of model object to replace with weapon object
	std::size_t iWeaponObject, iWeaponObjectMin, iWeaponObjectMax;

	// Animation clip index
	std::size_t iClip;

	// Animation keyframe index
	std::size_t iFrame;

	// Room animation index
	std::size_t iRoom, iRoomMin, iRoomMax;

	// set window handle for message/debugging
	void SetWindow(HWND hWnd)
	{
		Str.hWnd = hWnd;
		m_Texture->Str.hWnd = hWnd;
		m_WeaponTexture->Str.hWnd = hWnd;
		m_Model->Str.hWnd = hWnd;
		m_WeaponModel->Str.hWnd = hWnd;
		for (size_t i = 0; i < m_Animations.size(); ++i) { m_Animations[i]->Str.hWnd = hWnd; }
	}

	// set video game type
	virtual void SetGame(Video_Game Game) override
	{
		Resident_Evil_Common::SetGame(Game);
		for (size_t i = 0; i < m_Animations.size(); ++i) { m_Animations[i]->SetGame(Game); }
	}

	// Filename
	const std::filesystem::path Filename(void) noexcept { return m_Filename; }

	// Weapon Filename
	const std::filesystem::path WeaponFilename(void) noexcept { return m_WeaponFilename; }

	// Position
	VECTOR2& Position(void) noexcept { return m_Position; }

	// Rotation
	VECTOR2& Rotation(void) noexcept { return m_Rotation; }

	// Scale
	VECTOR2& Scale(void) noexcept { return m_Scale; }

	// Editor Position
	VECTOR2& EditorPosition(void) noexcept { return m_EditorPosition; }

	// Editor Rotation
	VECTOR2& EditorRotation(void) noexcept { return m_EditorRotation; }

	// Editor Scale
	VECTOR2& EditorScale(void) noexcept { return m_EditorScale; }

	// Interactive/Collision Size Vector
	SIZEVECTOR& Hitbox(void) noexcept { return m_Hitbox; }

	// Interactive/Collision Shape Vector
	SHAPEVECTOR HitboxShape(void) noexcept;

	// Model
	std::unique_ptr<Sony_PlayStation_Model>& Model(void) noexcept { return m_Model; }

	// Weapon Model
	std::unique_ptr<Sony_PlayStation_Model>& WeaponModel(void) noexcept { return m_WeaponModel; }

	// Texture
	std::unique_ptr<Sony_PlayStation_Texture>& Texture(void) noexcept { return m_Texture; }

	// Weapon Texture
	std::unique_ptr<Sony_PlayStation_Texture>& WeaponTexture(void) noexcept { return m_WeaponTexture; }

	// Animation Data
	std::shared_ptr<Resident_Evil_Animation>& Animation(AnimationIndex Type) noexcept { return m_Animations[std::to_underlying(Type)]; }

	// Set Animation Index
	AnimationIndex& AnimIndex(void) noexcept { return m_AnimationIndex; }

	/*
		Set world matrix
		 - position, rotation and scale of model
	*/
	void SetWorld(const MATVECTOR& Vec) const;

	/*
		Open
		 - TMD, MD1, MD2, PLD, PLW, EMD, EMW
		 - call SetGame before this function
		 - automatically calls ExportDX9 when texture is open
	*/
	bool Open(std::filesystem::path Path, std::uintmax_t _Ptr = 0, bool b_Bio1Enemy = false);

	/*
		Open model file -- TMD (Bio1), MD1 (Bio2) or MD2 (Bio3)
		 - automatically calls ExportDX9 when texture is open
	*/
	bool OpenObject(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	/*
		Open player file - EMD (Bio1) or PLD (Bio2/Bio3)
		 - call SetGame before this function
		 - automatically calls ExportDX9
	*/
	bool OpenPlayer(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	/*
		Open enemy file - EMD (Bio1/Bio2/Bio3)
		 - call SetGame before this function
		 - automatically opens texture when Bio2/Bio3 EMD
		 - automatically calls ExportDX9
	*/
	bool OpenEnemy(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	/*
		Open weapon file - EMW (Bio1) or PLW (Bio2/Bio3)
		 - call SetGame before this function
		 - automatically calls ExportDX9
	*/
	bool OpenWeapon(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	/*
		Open model texture
		 - automatically calls ExportDX9 when model is open
	*/
	bool OpenTexture(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	/*
		Open weapon texture
		 - automatically calls ExportDX9 when weapon model is open
	*/
	bool OpenWeaponTexture(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	// Reset clip
	void ResetClip(void) { iClip = 0; iFrame = 0; }

	// Setup room data
	void SetRoomAnimations(std::shared_ptr<Resident_Evil_Animation>& Rbj);

	// Clear all data
	void Close(void) { ResetClip(); CloseModel(); CloseWeapon(); }

	// Clear model data
	void CloseModel(void);

	// Clear weapon data
	void CloseWeapon(void);

	// Clear room data
	void CloseRoom(void);

	// Draw model at animation keyframe
	void DrawFrame(std::shared_ptr<Resident_Evil_Animation> Animation, size_t iClip, size_t iFrame, bool b_DrawRoot = false);

	// Draw object of model
	void DrawObject(std::size_t iObject, bool b_DrawAll = true, bool DrawWeapon = false);

	// Draw model
	void Draw(void);

};