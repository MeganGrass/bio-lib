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
	Object = (1 << 0),
	Player = (1 << 1),
	SubPlayer = (1 << 2),
	Enemy = (1 << 3)
};


class Resident_Evil_Model :
	private Resident_Evil_Common {
private:

	// Standard String for messages/debugging
	Standard_String Str;

	// Animation Type Constants
	static constexpr AnimationIndex NORMAL = AnimationIndex::Normal;
	static constexpr AnimationIndex NORMAL_EX0 = AnimationIndex::NormalEx0;
	static constexpr AnimationIndex NORMAL_EX1 = AnimationIndex::NormalEx1;
	static constexpr AnimationIndex DAMAGE = AnimationIndex::Damage;
	static constexpr AnimationIndex WEAPON = AnimationIndex::Weapon;
	static constexpr AnimationIndex WEAPON_EX0 = AnimationIndex::WeaponEx0;
	static constexpr AnimationIndex WEAPON_EX1 = AnimationIndex::WeaponEx1;
	static constexpr AnimationIndex ROOM = AnimationIndex::Room;

	struct LIMB_INDEX
	{
		// add 0x08 (sizeof BINARY_HEADER) to very first pTriangleListA to get absolute offset to the first LIMB_DATA entry, then parse from there, ignoring the rest of the offsets
		std::uint16_t pTriangleListA{};					/*
															3-byte triangle indices (triplets) into a limb-local vertex pool; ends on an 0xFF triplet, or single 0xFF null terminator

															example:
															[00,12,00], [01,02,03], [04,05,06], [07,08,09], [0A,0B,0C], [0D,0E,0F], [10,11,12], [16,17,18], [19,1A,1B], [1C,1D,1E],
															[20,21,22], [23,25,26], [27,28,29], [2A,2B,2C], [2D,2E,2F], [30,31,32], [33,34,35], [37,38,3B], [3B,3B,FF]
														*/
		std::uint16_t pTriangleListB{};					/*
															same as pTriangleListA

															example:
															[02,0A,12], [01,02,0A], [12,34,35], [37,38,02], [01,22,23], [25,05,01], [1E,20,21], [08,04,16], [17,18,19], [1A,1B,1C], [1D,30,31], [FF,FF,FF]
														*/
		std::uint16_t pVertexBind{};					/*
															bytecode that assigns limb-local vertex IDs to joints for list A/B; stream terminates with 0xFF
															The stream is a sequence of opcodes and vertex IDs parsed statefully

															Opcodes:
															- A non-zero byte read while no joint is active sets the current JointId
															- 0x60: Selects list A for subsequent vertex assignments
															- 0x61: Selects list B for subsequent vertex assignments
															- 0x00: Resets the current JointId, requiring a new one to be set
															- 0xFF: Terminates the stream

															Any other byte is treated as a vertex ID to be assigned to the current joint and list

															example:
															02 13 60 00					- JointId = 0x02, assign vertex 0x13 to list A, reset joint
															02 13 60 39 0B 24 60 00		- JointId = 0x02, assign vertices 0x13, 0x39, 0x0B, 0x24 to list A, reset joint
															02 14 61 14 61 14 61 00		- JointId = 0x02, assign 0x14 to list A, then assign 0x14 to list B twice, reset joint
															05 15 60 3C 14 3A 0B 00		- JointId = 0x05, assign vertices 0x15, 0x3C, 0x14, 0x3A, 0x0B to list A, reset joint
															05 1F 60 36 60 1F 60 FF		- JointId = 0x05, assign vertices 0x1F, 0x36, 0x1F to list A, terminate stream
														*/
		std::uint16_t reserved{};						// padding, always zero (0)
	};

	struct BINARY_HEADER
	{
		std::uint8_t unk0{};							// total TriangleList Count?
		std::uint8_t unk1{};							// total TriangleList Count?
		std::uint16_t unk2{};							// 
		std::uint16_t pBindPose{};						/*
															absolute offset to SVECTOR2 array (count matches Animation(NORMAL)->Joint size)
															coordinate-space difference between the bind pose and animation joint space (opposite signage -/+)
														*/
		std::uint16_t LimbCount{};						// number of limbs/body groups
	};

	struct LIMB_DATA
	{
		std::vector<std::array<uint8_t, 3>> TriA{};
		std::vector<std::array<uint8_t, 3>> TriB{};
		std::array<int8_t, 256> JointA{};
		std::array<int8_t, 256> JointB{};
	};

	struct BINARY_DATA
	{
		std::vector<SVECTOR2> BindPose{};
		std::vector<LIMB_DATA> Limb{};
	};

	struct SHADOW
	{
		rect Rect{};
		sizevec Size{};
		size_t TexID{};
		std::vector<vec3t> Vec;
#ifdef MSTD_DX9
		std::unique_ptr<IDirect3DVertexBuffer9, IDirect3DDelete9<IDirect3DVertexBuffer9>> Vertices;
#endif
	};

	// Filename
	std::filesystem::path m_Filename, m_WeaponFilename;

	// Model Type
	ModelType m_ModelType;

	// Model Game Type
	Video_Game m_ModelGame;

	// Weapon Model Game Type
	Video_Game m_WeaponModelGame;

	// Animation Data
	std::array<std::shared_ptr<Resident_Evil_Animation>, std::to_underlying(AnimationIndex::Count)> m_Animations;

	// Animation Index ID
	std::atomic<AnimationIndex> m_AnimationIndex;

	// Controller Input / AI state machine
	std::unique_ptr<StateMachineType> m_State;

	// Matrix
	std::shared_ptr<Standard_Matrix> World;

	// Sony PlayStation (1994) GTE Matrix
	MATRIX2 m_SuperMatrix;

	// Transformation
	VECTOR2 m_Position, m_Rotation, m_Scale, m_EditorPosition, m_EditorRotation, m_EditorScale;

	// Interactive/Collision Size Vector
	SIZEVECTOR m_Hitbox;

	// Previous Offset
	SVECTOR2 m_Speed;

	// Model
	std::unique_ptr<Sony_PlayStation_Model> m_Model, m_WeaponModel;

	// Texture
	std::unique_ptr<Sony_PlayStation_Texture> m_Texture, m_WeaponTexture;

	// Shadow
	SHADOW m_Shadow;

	// Binary Chunk
	std::vector<std::uint8_t> m_Binary00, m_Binary01, m_WeaponBinary;

	// Frame Counter
	std::size_t m_FrameCounter;

	// Get data pointers from file archive (EMD/EMW/PLD/PLW)
	std::vector<std::uint32_t> GetDataPtr(StdFile& File, std::uintmax_t _FileBeginPtr);

	// Parse binary chunk
	void ParseBinary(const std::vector<std::uint8_t>& Binary, size_t JointCount, BINARY_DATA& Output);

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

#ifdef MSTD_DX9
	// Direct-X 9 Model
	std::unique_ptr<DX9_MODEL> m_DX9Model, m_DX9WeaponModel;

	// Clear DX9 model data
	void CloseModelDX9(void)
	{
		if (m_DX9Model)
		{
			m_DX9Model->Object.clear();
			m_DX9Model->Object.shrink_to_fit();
			m_DX9Model->Texture.clear();
			m_DX9Model->Texture.shrink_to_fit();
			m_DX9Model.reset(nullptr);
		}
	}

	// Clear DX9 weapon data
	void CloseWeaponDX9(void)
	{
		if (m_DX9WeaponModel)
		{
			m_DX9WeaponModel->Object.clear();
			m_DX9WeaponModel->Object.shrink_to_fit();
			m_DX9WeaponModel->Texture.clear();
			m_DX9WeaponModel->Texture.shrink_to_fit();
			m_DX9WeaponModel.reset(nullptr);
		}
	}
#endif

public:

	using Resident_Evil_Common::GameType;

	explicit Resident_Evil_Model(void) :
		World(std::make_shared<Standard_Matrix>()),
		m_Texture(std::make_unique<Sony_PlayStation_Texture>()),
		m_WeaponTexture(std::make_unique<Sony_PlayStation_Texture>()),
		m_Model(std::make_unique<Sony_PlayStation_Model>()),
		m_WeaponModel(std::make_unique<Sony_PlayStation_Model>()),
		m_ModelType(ModelType::None),
		m_ModelGame(Video_Game::Resident_Evil_2),
		m_WeaponModelGame(Video_Game::Resident_Evil_2),
		m_Filename{},
		m_WeaponFilename{},
		m_Position{ 0, 0, 0 },
		m_Rotation{ 0, 0, 0 },
		m_Scale{ ONE, ONE, ONE },
		m_EditorPosition{ 0, 0, 0 },
		m_EditorRotation{ 0, 0, 0 },
		m_EditorScale{ ONE, ONE, ONE },
		m_Hitbox{ 0, 0, 0 },
		m_Speed{ 0, 0, 0 },
		m_PlayerID(0),
		m_WeaponID(0),
		m_EnemyID(0),
		m_DiskID(0),
#ifdef MSTD_DX9
		m_TextureFilter(D3DTEXF_NONE),
		m_DX9Model(nullptr),
		m_DX9WeaponModel(nullptr),
#endif
		m_FrameCounter(0),
		b_HorzFlip(false),
		b_VertFlip(false),
		m_LerpValue(0.50f),
		b_ControllerMode(false),
		b_EditorMode(false),
		b_Dither(true),
		b_LockPosition(false),
		b_DrawWireframe(false),
		b_DrawTextured(true),
		b_DrawSolidColor(false),
		b_DrawSkeleton(false),
		b_DrawReference(false),
		b_DrawHitbox(false),
		b_DrawAllObjects(false),
		b_DrawSingleObject(false),
		b_DrawWeapon(false),
		b_DrawShadow(true),
		b_Bio1Enemy(false),
		b_IsAlive(true),
		Sce_flg(0),
		Sce_free0(0),
		Sce_free1(0),
		Sce_free2(0),
		Sce_free3(0),
		At_sce_no(0),
		Water(0),
		b_WeaponChange(false),
		iHealth(200), iHealthMin(0), iHealthMax(200),
		iObject(0), iObjectMin(0), iObjectMax(0),
		iWeaponObject(0), iWeaponObjectMin(0), iWeaponObjectMax(0),
		iRoom(0), iRoomMin(0), iRoomMax(0)
	{
		Routine = [&]() {};
		Controller = [&]() {};

		MATRIX2_SET_IDENTITY(&m_SuperMatrix);

		m_AnimationIndex.store(NORMAL);
		iClip.store(0);
		iFrame.store(0);
		b_Active.store(true);
		b_Drawing.store(false);
		b_StopDrawing.store(false);
		b_Play.store(true);
		b_Loop.store(true);
		b_PlayInReverse.store(false);
		b_PlayAllFrames.store(false);
		b_LerpKeyframes.store(true);
		b_IdleTurn.store(false);
		b_QuickTurn.store(false);
		m_QuickTurnRotation.store(0);
		b_WeaponKickback.store(false);
		b_WeaponKickbackComplete.store(false);

		m_Model->IgnoreMagic(true);
		m_WeaponModel->IgnoreMagic(true);

		for (size_t i = 0; i < m_Animations.size(); ++i)
		{
			m_Animations[i] = std::make_shared<Resident_Evil_Animation>();
			m_Animations[i]->SetType(Resident_Evil_Animation_Type(1 << i));
		}

		SetGame(Video_Game::Resident_Evil_2);

		m_Shadow.TexID = 0;
		m_Shadow.Vec.clear();
		m_Shadow.Vec.shrink_to_fit();

#ifdef MSTD_DX9
		m_Shadow.Vertices.reset(nullptr);
#endif
	}

	~Resident_Evil_Model(void) = default;

	// Sony PlayStation (1994) Geometry Transformation Engine
	std::shared_ptr<Sony_PlayStation_GTE> GTE;

#ifdef MSTD_DX9

	// Direct-X 9 Render Context
	std::shared_ptr<Standard_DirectX_9> Render;

	// Texture Filter
	D3DTEXTUREFILTERTYPE m_TextureFilter;

	// Export texture and standardized model to Direct-X 9 buffer
	std::unique_ptr<DX9_MODEL> ExportDX9(std::unique_ptr<FIXED_MODEL>& Model, std::unique_ptr<Sony_PlayStation_Texture>& Texture) const;

	// Direct-X 9 Model
	[[nodiscard]] auto& ModelDX9(void) noexcept { return m_DX9Model; }

	// Direct-X 9 Weapon Model
	[[nodiscard]] auto& WeaponModelDX9(void) noexcept { return m_DX9WeaponModel; }

#endif

	// Player, Weapon, Enemy and Disk IDs
	std::uint32_t m_PlayerID, m_WeaponID, m_EnemyID, m_DiskID, m_ObjectID;

	// Runtime Routine
	std::function<void()> Routine;

	// Gamepad Routine
	std::function<void()> Controller;

	// Animation clip index
	std::atomic<std::size_t> iClip;

	// Animation keyframe index
	std::atomic<std::size_t> iFrame;

	// Will the model be drawn?
	std::atomic<bool> b_Active;

	// Any model objects currently being drawn?
	std::atomic<bool> b_Drawing;

	// Immediately stop drawing model objects
	std::atomic<bool> b_StopDrawing;

	// Is keyframe processing active?
	std::atomic<bool> b_Play;

	// Will keyframe processing loop?
	std::atomic<bool> b_Loop;

	// Will keyframes process in reverse?
	std::atomic<bool> b_PlayInReverse;

	// All keyframes must be processed
	std::atomic<bool> b_PlayAllFrames;

	// Lerp keyframes
	std::atomic<bool> b_LerpKeyframes;

	// Lerp value
	float m_LerpValue;

	/*
		Idle-Turn Animation
		 - requires reset to idle before next animation
	*/
	std::atomic<bool> b_IdleTurn;

	/*
		Quick-Turn Animation
		 - for Bio1/Bio2, both don't have quick-turn animation
	*/
	std::atomic<bool> b_QuickTurn;

	/*
		Quick-Turn Rotation Counter
		 - set to 2048 on quick-turn start
		 - decrement by 64 on each frame
	*/
	std::atomic<std::int32_t> m_QuickTurnRotation;

	/*
		Perspective Flip
		 - requires camera to be flipped
	*/
	bool b_HorzFlip, b_VertFlip;

	/*
		Controller Mode
		 - allow animation state changes with controller input
	*/
	bool b_ControllerMode;

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
		 - skeleton will be drawn as lines
	*/
	bool b_DrawSkeleton;

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

	// Has the weapon id changed?
	bool b_WeaponChange;

	/*
		Does the weapon have kickback?
		 - position change ("push" backward) on weapon discharge
	*/
	std::atomic<bool> b_WeaponKickback;

	// Is the weapon kickback complete?
	std::atomic<bool> b_WeaponKickbackComplete;

	// Will the shadow be drawn?
	bool b_DrawShadow;

	// Is Bio1 EMD file enemy type?
	bool b_Bio1Enemy;

	// Is the character alive?
	bool b_IsAlive;

	// 
	std::uint8_t Routine_0;
	std::uint8_t Routine_1;
	std::uint8_t Routine_2;
	std::uint8_t Routine_3;
	std::int16_t Dest_x;
	std::int16_t Dest_z;
	std::uint8_t Set_flg_no;
	std::uint16_t Sce_flg;
	std::int16_t Sce_free0;
	std::int16_t Sce_free1;
	std::int16_t Sce_free2;
	std::int16_t Sce_free3;
	std::uint8_t At_sce_no;
	std::int16_t Water;

	// Health Power
	std::int32_t iHealth, iHealthMin, iHealthMax;

	// Model object index
	std::size_t iObject, iObjectMin, iObjectMax;

	// ID of model object to replace with weapon object
	std::size_t iWeaponObject, iWeaponObjectMin, iWeaponObjectMax;

	// Room animation clip index
	std::size_t iRoom, iRoomMin, iRoomMax;

	// Skeleton World Matrices
	std::vector<Standard_Matrix> m_BoneWorld;

	// set video game type
	virtual void SetGame(Video_Game Game) override
	{
		Resident_Evil_Common::SetGame(Game);
		for (size_t i = 0; i < m_Animations.size(); ++i) { m_Animations[i]->SetGame(Game); }
	}

#ifdef _WINDOWS
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
#endif

#ifdef MSTD_DX9
	// initial setup
	void PlatformSetup(HWND hWnd, std::shared_ptr<Sony_PlayStation_GTE> _GTE, std::shared_ptr<Standard_DirectX_9> _Render, bool HorzFlip, bool VertFlip)
	{
		SetWindow(hWnd);
		SetGame(m_Game);
		GTE = _GTE;
		Render = _Render;
		b_HorzFlip = HorzFlip;
		b_VertFlip = VertFlip;
	}
#endif

	// Filename
	[[nodiscard]] const auto& Filename(void) const noexcept { return m_Filename; }

	// Weapon Filename
	[[nodiscard]] const auto& WeaponFilename(void) const noexcept { return m_WeaponFilename; }

	// Model Type
	[[nodiscard]] const auto& ModelType(void) const noexcept { return m_ModelType; }

	// Model Game Type
	[[nodiscard]] const auto Game(void) const noexcept { return std::to_underlying(m_ModelGame); }

	// Weapon Model Game Type
	[[nodiscard]] const auto WeaponGame(void) const noexcept { return std::to_underlying(m_WeaponModelGame); }

	// Position
	[[nodiscard]] auto& Position(void) noexcept { return m_Position; }

	// Rotation
	[[nodiscard]] auto& Rotation(void) noexcept { return m_Rotation; }

	// Scale
	[[nodiscard]] auto& Scale(void) noexcept { return m_Scale; }

	// Editor Position
	[[nodiscard]] auto& EditorPosition(void) noexcept { return m_EditorPosition; }

	// Editor Rotation
	[[nodiscard]] auto& EditorRotation(void) noexcept { return m_EditorRotation; }

	// Editor Scale
	[[nodiscard]] auto& EditorScale(void) noexcept { return m_EditorScale; }

	// Interactive/Collision Size Vector
	[[nodiscard]] auto& Hitbox(void) noexcept { return m_Hitbox; }

	// Interactive/Collision Shape Vector
	[[nodiscard]] SHAPEVECTOR HitboxShape(void) noexcept;

	// Shadow
	[[nodiscard]] auto& Shadow(void) noexcept { return m_Shadow; }

	// Model
	[[nodiscard]] auto& Model(void) noexcept { return m_Model; }

	// Weapon Model
	[[nodiscard]] auto& WeaponModel(void) noexcept { return m_WeaponModel; }

	// Texture
	[[nodiscard]] auto& Texture(void) noexcept { return m_Texture; }

	// Weapon Texture
	[[nodiscard]] auto& WeaponTexture(void) noexcept { return m_WeaponTexture; }

	// Get Current Animation
	[[nodiscard]] auto& Animation(AnimationIndex Type) noexcept { return m_Animations[std::to_underlying(Type)]; }

	// Get Current Animation Frame
	[[nodiscard]] auto& Frame(void) noexcept { return m_Animations[std::to_underlying(m_AnimationIndex.load())]->Clip[iClip.load()][iFrame.load()]; }

	// Get Current Animation Index
	[[nodiscard]] const auto AnimIndex(void) const noexcept { return m_AnimationIndex.load(); }

	// Controller Input / AI state machine
	[[nodiscard]] auto& State(void) noexcept { return m_State; }

	// Previous Offset
	[[nodiscard]] auto& Speed(void) noexcept { return m_Speed; }

	// Is health in caution range?
	[[nodiscard]] const auto IsHealthCaution(void) const noexcept { return (iHealth <= ((iHealthMax / 3) * 2)); }

	// Is health in danger range?
	[[nodiscard]] const auto IsHealthDanger(void) const noexcept { return (iHealth <= (iHealthMax / 3)); }

	// Is health empty?
	[[nodiscard]] const auto IsHealthEmpty(void) const noexcept { return (iHealth <= iHealthMin); }

	// File ID
	[[nodiscard]] const uint32_t FileID(void) const noexcept
	{
		switch (m_ModelType)
		{
		case ModelType::Object:	return m_ObjectID;
		case ModelType::Player:	return m_PlayerID;
		case ModelType::SubPlayer: return m_EnemyID;
		case ModelType::Enemy:	return m_EnemyID;
		default: return 0;
		}
	}

	// Set File ID
	void SetFileID(uint32_t ID) noexcept
	{
		switch (m_ModelType)
		{
		case ModelType::Object:	m_ObjectID = ID; break;
		case ModelType::Player:	m_PlayerID = ID; break;
		case ModelType::SubPlayer: m_EnemyID = ID; break;
		case ModelType::Enemy: m_EnemyID = ID; break;
		}
	}

	// Weapon File ID
	[[nodiscard]] const uint32_t WeaponFileID(void) const noexcept
	{
		switch (m_ModelType)
		{
		case ModelType::Player:	return m_WeaponID;
		case ModelType::SubPlayer: return m_WeaponID;
		default: return 0;
		}
	}

	// Set Weapon File ID
	void SetWeaponFileID(uint32_t ID) noexcept
	{
		switch (m_ModelType)
		{
		case ModelType::Player:	m_WeaponID = ID; break;
		case ModelType::SubPlayer: m_WeaponID = ID; break;
		}
	}

	// nFloor
	[[nodiscard]] const std::uint8_t nFloor(void) const noexcept { return static_cast<uint8_t>(m_Position.y / -1800); }

	// Set nFloor
	void SetnFloor(std::uint8_t Floor) noexcept { m_Position.y = Floor * -1800; }

	// Set Current Animation Index
	void SetAnimIndex(AnimationIndex Index) noexcept { m_AnimationIndex.store(Index); }

	// Set Current Animation Clip Index
	void SetClip(std::size_t iValue)
	{
		const auto& m_ClipCount = Animation(AnimIndex())->GetClipCount();
		iClip.store(min(iValue, m_ClipCount ? m_ClipCount - 1 : 0));
	}

	// Set Current Animation Frame Index
	void SetFrame(std::size_t iValue)
	{
		const auto& m_FrameCount = Animation(AnimIndex())->GetFrameCount(iClip.load());
		iFrame.store(min(iValue, m_FrameCount ? m_FrameCount - 1 : 0));
	}

	// Reset Clip
	void ResetClip(void)
	{
		iClip.store(0);
		ResetFrame();
	}

	// Reset Frame
	void ResetFrame(void)
	{
		iFrame.store(0);
		ResetFrameCounter();
	}

	// Reset Frame Counter
	void ResetFrameCounter(std::size_t Value = 0)
	{
		m_FrameCounter = Value;
		m_Speed = { 0, 0, 0 };
	}

	// Clamp position between -32768 and 32768
	void ClampPosition(VECTOR2& Pos)
	{
		constexpr std::int32_t Threshold = 32768;
		constexpr std::int32_t Offset = Threshold * 2;

		if (Pos.x >= Threshold) { Pos.x -= Offset; }
		else if (Pos.x <= -Threshold) { Pos.x += Offset; }

		if (Pos.z >= Threshold) { Pos.z -= Offset; }
		else if (Pos.z <= -Threshold) { Pos.z += Offset; }
	}

	// Clamp rotation between -4096 and 4096
	void ClampRotation(VECTOR2& Rot)
	{
		constexpr std::int32_t Threshold = ONE;
		constexpr std::int32_t Offset = Threshold * 2;

		if (Rot.y >= Threshold) { Rot.y -= Offset; }
		else if (Rot.y <= -Threshold) { Rot.y += Offset; }
	}

	/*
		Stop drawing model
		 - stop drawing model objects immediately
	*/
	void StopDrawing(void)
	{
		b_Active.store(false);

		b_StopDrawing.store(true);

		if (b_Drawing.load())
		{
			auto StartTime = std::chrono::steady_clock::now();

			while (b_Drawing.load())
			{
				std::this_thread::yield();

				if (std::chrono::steady_clock::now() - StartTime > std::chrono::seconds(1))
				{
					break;
				}
			}
		}
	}

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
	bool Open(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	/*
		Open model texture
		 - TIM, TIM2, BMP, PNG and JPG
		 - automatically calls ExportDX9 when model is open
	*/
	bool OpenTexture(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	/*
		Open weapon texture
		 - TIM, TIM2, BMP, PNG and JPG
		 - automatically calls ExportDX9 when weapon model is open
	*/
	bool OpenWeaponTexture(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	/*
		Initialize shadow texture
		 - player model must be open
		 - hitbox must be set prior to this function
	*/
	void SetShadow(size_t PaletteID, std::uint16_t X, std::uint16_t Y, std::uint16_t Width, std::uint16_t Height);

	// Setup room data
	void SetRoomAnimations(std::shared_ptr<Resident_Evil_Animation>& Rbj);

	// Stop drawing and clear all data
	void Close(void)
	{
		StopDrawing();
		m_ModelType = ModelType::None;
		b_IdleTurn.store(false);
		b_QuickTurn.store(false);
		m_QuickTurnRotation.store(0);
		b_WeaponChange = false;
		ResetClip();
		CloseModel();
		CloseWeapon();
		b_Active.store(true);
	}

	// Clear model data
	void CloseModel(void);

	// Clear weapon data
	void CloseWeapon(void);

	// Clear room data
	void CloseRoom(void);

	// Shutdown model
	void Shutdown(void)
	{
		Routine = [&]() {};

		Controller = [&]() {};

		//StopDrawing();

		Close();
	}

	// Draw model at animation keyframe
	void DrawFrame(std::shared_ptr<Resident_Evil_Animation> Animation, size_t iClip, size_t iFrame, bool b_DrawRoot = false);

	// Draw object of model
	void DrawObject(std::size_t iObject, bool b_DrawAll = true, bool DrawWeapon = false);

	// Draw model
	void Draw(void);

	// Draw shadow
	void DrawShadow(void);

	// Translate model position with keyframe data
	void AddSpeedXZ(SVECTOR* Speed);

	void AddSpeedXZ_orig(int32_t muki);

	void AddSpeedXYZ(int32_t dir_y, int32_t dir_z);

	void AddSpeedXYZsuper(int32_t dir_y, int32_t dir_z);

};