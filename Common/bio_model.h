/*
*
*	Megan Grass
*	March 07, 2024
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


class Resident_Evil_Model :
	private Resident_Evil_Common {
private:

	// Animation Type Constants
	static constexpr AnimationIndex NORMAL = AnimationIndex::Normal;
	static constexpr AnimationIndex NORMAL_EX0 = AnimationIndex::NormalEx0;
	static constexpr AnimationIndex NORMAL_EX1 = AnimationIndex::NormalEx1;
	static constexpr AnimationIndex DAMAGE = AnimationIndex::Damage;
	static constexpr AnimationIndex WEAPON = AnimationIndex::Weapon;
	static constexpr AnimationIndex WEAPON_EX0 = AnimationIndex::WeaponEx0;
	static constexpr AnimationIndex WEAPON_EX1 = AnimationIndex::WeaponEx1;
	static constexpr AnimationIndex ROOM = AnimationIndex::Room;

	// Transformation
	VECTOR2 m_Position, m_Rotation;

	// Matrix
	std::shared_ptr<Standard_Matrix> World;

	// Texture
	std::unique_ptr<Sony_PlayStation_Texture> m_Texture, m_WeaponTexture;

	// Model
	std::unique_ptr<Sony_PlayStation_Model> m_Model, m_WeaponModel;

	// Binary Chunk (some EMDs in Bio2, every type in Bio3; currently unknown purpose, likely vertice blending/mesh deform...)
	std::vector<std::uint8_t> m_Binary00, m_Binary01, m_WeaponBinary;

	// Animation Data
	std::array<std::shared_ptr<Resident_Evil_Animation>, std::to_underlying(AnimationIndex::Count)> m_Animations;

	// Get data pointers from file archive (EMD/EMW/PLD/PLW)
	std::vector<std::uint32_t> GetDataPtr(StdFile& File, std::uintmax_t _FileBeginPtr);

	// Standard String for messages/debugging
	Standard_String Str;

#if MSTD_DX9
	// Direct-X 9 Model
	std::unique_ptr<DX9_MODEL> m_DX9Model, m_DX9WeaponModel;
#endif

public:

	explicit Resident_Evil_Model(void) :
		iClip(0),
		iFrame(0),
		iObject(0),
		m_Position({ 0, 0, 0}),
		m_Rotation({ 0, 0, 0 }),
		b_DrawRootOnly(false),
		World(std::make_shared<Standard_Matrix>()),
		m_Texture(std::make_unique<Sony_PlayStation_Texture>()),
		m_WeaponTexture(std::make_unique<Sony_PlayStation_Texture>()),
		m_Model(std::make_unique<Sony_PlayStation_Model>()),
		m_WeaponModel(std::make_unique<Sony_PlayStation_Model>())
#if MSTD_DX9
		,m_DX9Model(nullptr),
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

	/*
		Ignore keyframes in DrawFrame
		 - origin, speed and rotation are ignored
		 - root skeleton position is used
	*/
	bool b_DrawRootOnly;

	// Model object index
	std::size_t iObject;

	// Animation clip index
	std::size_t iClip;

	// Animation keyframe index
	std::size_t iFrame;

	// set window handle for message/debugging
	void SetWindow(HWND hWnd)
	{
		Str.hWnd = m_Texture->Str.hWnd = m_WeaponTexture->Str.hWnd = m_Model->Str.hWnd = m_WeaponModel->Str.hWnd = hWnd;
		for (size_t i = 0; i < m_Animations.size(); ++i) { m_Animations[i]->Str.hWnd = hWnd; }
	}

	// set video game type
	virtual void SetGame(Video_Game Game) override
	{
		Resident_Evil_Common::SetGame(Game);
		for (size_t i = 0; i < m_Animations.size(); ++i) { m_Animations[i]->SetGame(Game); }
	}

#if MSTD_DX9

	// Export texture and standardized model to Direct-X 9 buffer
	std::unique_ptr<DX9_MODEL> ExportDX9(std::unique_ptr<FIXED_MODEL>& Model, std::unique_ptr<Sony_PlayStation_Texture>& Texture) const;

	// Direct-X 9 Model
	std::unique_ptr<DX9_MODEL>& ModelDX9(void) noexcept { return m_DX9Model; }

	// Direct-X 9 Weapon Model
	std::unique_ptr<DX9_MODEL>& WeaponModelDX9(void) noexcept { return m_DX9WeaponModel; }

	// Direct-X 9 Render Context
	std::shared_ptr<Standard_DirectX_9> Render;

#endif

	// Sony PlayStation (1994) Geometry Transformation Engine
	std::shared_ptr<Sony_PlayStation_GTE> GTE;

	// Position
	VECTOR2& Position(void) noexcept { return m_Position; }

	// Rotation
	VECTOR2& Rotation(void) noexcept { return m_Rotation; }

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

	/*
		Set world matrix
		 - position and rotation of entire model
	*/
	void SetWorld(const MATVECTOR& Vec) const;

	/*
		Open model file -- TMD (Bio1), MD1 (Bio2) or MD2 (Bio3)
		 - call SetGame before this function
		 - ExportDX9 must be called after this function
	*/
	bool OpenObject(std::filesystem::path Path, std::uintmax_t _Ptr = 0, bool b_ReplaceModel = false);

	/*
		Open player file - EMD (Bio1) or PLD (Bio2/Bio3)
		 - call SetGame before this function
		 - automatically calls ExportDX9
	*/
	bool OpenPlayer(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	/*
		Open enemy file - EMD (Bio1/Bio2/Bio3)
		 - call SetGame before this function
		 - automatically calls ExportDX9
	*/
	bool OpenEnemy(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	/*
		Open weapon file - EMW (Bio1) or PLW (Bio2/Bio3)
		 - call SetGame before this function
		 - automatically calls ExportDX9
	*/
	bool OpenWeapon(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	// Open model texture
	bool OpenTexture(std::filesystem::path Path, std::uintmax_t _Ptr = 0) { if (m_Texture->IsOpen()) { m_Texture->Close(); } return m_Texture->OpenTIM(Path, _Ptr); }

	// Open weapon texture
	bool OpenWeaponTexture(std::filesystem::path Path, std::uintmax_t _Ptr = 0) { if (m_WeaponTexture->IsOpen()) { m_WeaponTexture->Close(); } return m_WeaponTexture->OpenTIM(Path, _Ptr); }

	// Clear all data
	void Close(void);

	// Clear weapon data
	void CloseWeapon(void);

	// Draw model at animation keyframe
	void DrawFrame(std::shared_ptr<Resident_Evil_Animation> Animation, size_t iClip, size_t iFrame);

	// Draw object of model
	void DrawObject(std::size_t iObject, bool b_DrawAll = true);

	// Draw model
	void Draw(void);

};