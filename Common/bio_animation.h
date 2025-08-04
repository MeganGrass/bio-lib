/*
*
*	Megan Grass
*	July 11, 2025
*
*/


#pragma once

#include "gte/lib_gte.h"

#include "std_matrix.h"

#include <bio_common.h>


enum class Resident_Evil_Animation_Type : std::uint32_t
{
	//	Flag								// File			Joints?		Notes
	None = (0 << 0),						// TMD/MD1/MD2	No			standard model, no animation data
	Normal = (1 << 0),						// EMD/PLD		Yes			Bio1/Bio2/Bio3 = first animation
	NormalEx0 = (1 << 1),					// EMD			No			Bio2 = second animation (use joints from Normal animation)
	NormalEx1 = (1 << 2),					// EMD			Yes			Bio3 = second animation
	Damage = (1 << 3),						// EMD			No			Bio1 = second animation, Bio2/Bio3 = third animation (for use with PLD)
	Weapon = (1 << 4),						// EMW/PLW		No			Bio1/Bio2/Bio3 = first animation (for use with PLD)
	WeaponEx0 = (1 << 5),					// EMD/PLW		Yes			Bio3 = second animation (PLW) / fourth animation (EMD) -- unknown purpose
	WeaponEx1 = (1 << 6),					// EMD/PLW		Yes			Bio3 = third animation (PLW) / fifth animation (EMD)
	Room = (1 << 7),						// RDT/RBJ		No			Bio1/Bio2/Bio3 (for use with either PLD or EMD)
};

enum class AnimationIndex : std::size_t
{
	Normal,
	NormalEx0,
	NormalEx1,
	Damage,
	Weapon,
	WeaponEx0,
	WeaponEx1,
	Room,
	Count
};

enum class Bio1PlayerState : std::size_t
{
	Idle,
	Damage_Front,
	Walk_Backward_Startled,
	Walk_Backward,
	Death,
	Aim,
	Aim_Upward,
	Aim_Downward,
	Aim_Handgun,
	Aim_Handgun_Upward,
	Aim_Handgun_Downward,
	Aim_Shotgun,
	Aim_Shotgun_Upward,
	Aim_Shotgun_Downward,
	Aim_Revolver,
	Aim_Revolver_Upward,
	Aim_Revolver_Downward,
	Aim_DumDum,
	Aim_DumDum_Upward,
	Aim_DumDum_Downward,
	Aim_Flamethrower,
	Aim_Flamethrower_Upward,
	Aim_Flamethrower_Downward,
	Aim_Bazooka_Explosive,
	Aim_Bazooka_Explosive_Upward,
	Aim_Bazooka_Explosive_Downward,
	Aim_Bazooka_Acid,
	Aim_Bazooka_Acid_Upward,
	Aim_Bazooka_Acid_Downward,
	Aim_Bazooka_Flame,
	Aim_Bazooka_Flame_Upward,
	Aim_Bazooka_Flame_Downward,
	Aim_Rocket_Launcher,
	Aim_Rocket_Launcher_Upward,
	Aim_Rocket_Launcher_Downward
};

enum class Bio2Nov96PlayerState : std::size_t
{
	Walk_Backward = 0,
	Walk_Backward_Startled = 1,
	Climb_Up = 2,
	Climb_Down = 3,
	Falling = 4,
	Landing = 5,
	Damage_Above = 6,
	Death = 7,
	Damage_Front = 8,
	Damage_Back = 9,
	Damage_Front_Minor = 10,
	Inspect_Kneel = 11,
	Damage_Laying = 12,
	Knock_Backward_Begin = 13,
	Knock_Backward_End = 14,
	Knock_Forward_Begin = 15,
	Knock_Forward_End = 16,
	Push_Begin = 17,
	Push = 18,
	Step_Forward = 19,
	Stairs_Ascend = 20,
	Stairs_Descend = 21,
	Idle_Caution = 22,
	Idle_Danger = 23,

	Run = 0,
	Idle_Begin = 1,
	Idle = 2,
	Idle_Pose_Begin = 3,
	Idle_Pose = 4,
	Walk_Forward = 5,
	Aim_Begin = 6,
	Fire = 7,
	Aim = 8,
	Fire_Upward = 9,
	Aim_Upward = 10,
	Fire_Downward = 11,
	Aim_Downward = 12,
	Reload = 13
};

enum class Bio2PlayerState : std::size_t
{
	Walk_Backward = 0,
	Walk_Backward_Startled = 1,
	Death = 2,
	Damage_Front_Minor = 3,
	Damage_Back = 4,
	Damage_Front = 5,
	Inspect_Kneel = 6,
	Push_Begin = 7,
	Push = 8,
	Walk_Backward_Caution = 9,

	Walk_Forward = 0,
	Run = 1,
	Idle = 2,
	Walk_Forward_Caution = 3,
	Run_Caution = 4,
	Idle_Caution = 5,
	Walk_Forward_Danger = 6,
	Run_Danger = 7,
	Idle_Danger = 8,
	Aim_Begin = 9,
	Fire = 10,
	Aim = 11,
	Fire_Upward = 12,
	Aim_Upward = 13,
	Fire_Downward = 14,
	Aim_Downward = 15,
	Reload = 16
};

enum class Bio3PlayerState : std::size_t
{
	Walk_Backward = 0,
	Death = 1,
	Damage_Front_Minor = 2,
	Damage_Back = 3,
	Damage_Front = 4,
	Inspect_Kneel = 5,
	Push_Begin = 6,
	Push = 7,
	Knock_Forward_Begin = 8,
	Knock_Forward_End = 9,
	Knock_Backward_Major_Begin = 10,
	Knock_Backward_Major_End = 11,
	Knock_Backward = 12,
	Dodge_Backward_Begin = 13,
	Dodge_Backward = 14,
	Dodge_Rightward = 15,
	Dodge_Leftward = 16,
	Dodge_Push_Begin = 17,
	Dodge_Push = 18,
	Dodge_Rightward_Roll = 19,
	Dodge_Leftward_Roll = 20,
	Idle_Pose = 21,

	Walk_Forward = 0,
	Run = 1,
	Idle = 2,
	Walk_Forward_Caution = 3,
	Run_Caution = 4,
	Idle_Caution = 5,
	Walk_Forward_Danger = 6,
	Run_Danger = 7,
	Idle_Danger = 8,
	Walk_Backward_Startled = 9,
	Walk_Backward_Caution = 10,
	Idle_Pose_Alt_Begin = 11,
	Idle_Pose_Alt = 12,
	Dodge_Aim_Begin = 13,
	Dodge_Fire = 14,
	Dodge_Aim = 15,
	Dodge_Aim_End = 16,
	Dodge_End = 17,
	Aim_Begin = 18,
	Fire = 19,
	Aim = 20,
	Fire_Upward = 21,
	Aim_Upward = 22,
	Fire_Downward = 23,
	Aim_Downward = 24,
	Reload = 25
};


class Resident_Evil_Animation :
	protected Resident_Evil_Common {
private:

#pragma pack(push, 1)


	struct Entity
	{
		/*
			Player = 1
			Sub-Player = 2
			extra bit per enemy (0x10, 0x20, 0x40, etc.)
		*/
		std::uint16_t Flag0;
		std::uint16_t Flag1;
	};


	struct Joint
	{
		SVECTOR2 Pos{};						// Root Position
		std::vector<std::uint8_t> Ref;		// Model Object IDs
	};


	struct Frame
	{
		struct Attribute
		{
			std::uint32_t pEmr;				// EMR File Index // offset = (pFrames + (pEmr * FrameLen))
			std::uint32_t Speed;			// Speed Multiplier (1 = 15fps delay, 2 = 30fps delay, ...)
			std::uint32_t Sound;			// Sound Flag (Bio2 and Bio3, only)
		} Attr{};
		SVECTOR2 Origin{};					// Origin Position
		SVECTOR2 Speed{};					// Speed Position
		std::vector<SVECTOR2> Rotation;		// Rotation Positions (x1 per Joint)
	};


#pragma pack(pop)

	// Animation Type Constants
	static constexpr std::uint32_t NONE = std::to_underlying(Resident_Evil_Animation_Type::None);
	static constexpr std::uint32_t NORMAL = std::to_underlying(Resident_Evil_Animation_Type::Normal);
	static constexpr std::uint32_t NORMAL_EX0 = std::to_underlying(Resident_Evil_Animation_Type::NormalEx0);
	static constexpr std::uint32_t NORMAL_EX1 = std::to_underlying(Resident_Evil_Animation_Type::NormalEx1);
	static constexpr std::uint32_t DAMAGE = std::to_underlying(Resident_Evil_Animation_Type::Damage);
	static constexpr std::uint32_t WEAPON = std::to_underlying(Resident_Evil_Animation_Type::Weapon);
	static constexpr std::uint32_t WEAPON_EX0 = std::to_underlying(Resident_Evil_Animation_Type::WeaponEx0);
	static constexpr std::uint32_t WEAPON_EX1 = std::to_underlying(Resident_Evil_Animation_Type::WeaponEx1);
	static constexpr std::uint32_t ROOM = std::to_underlying(Resident_Evil_Animation_Type::Room);

	// Get Animation Type
	std::uint32_t AnimType(void) const { return std::to_underlying(m_Type); }

	// Animation Type
	Resident_Evil_Animation_Type m_Type;

	// Flag
	bool b_EddOpen;
	bool b_EmrOpen;

	bool b_IsContainer;

	bool b_EraseDuplicates;

	Resident_Evil_Animation(const Resident_Evil_Animation&) = delete;
	Resident_Evil_Animation& operator=(const Resident_Evil_Animation&) = delete;

public:

	Resident_Evil_Animation(Resident_Evil_Animation&& other) noexcept :
		m_Type(other.m_Type),
		b_EddOpen(other.b_EddOpen),
		b_EmrOpen(other.b_EmrOpen),
		b_IsContainer(other.b_IsContainer),
		b_EraseDuplicates(other.b_EraseDuplicates),
		Joints(std::move(other.Joints)),
		Clip(std::move(other.Clip)),
		Skeleton(std::move(other.Skeleton)),
		EntityList(std::move(other.EntityList)),
		Data(std::move(other.Data))
	{
		Str.hWnd = other.Str.hWnd;
	}

	Resident_Evil_Animation& operator=(Resident_Evil_Animation&& other) noexcept
	{
		if (this != &other)
		{
			m_Type = other.m_Type;
			b_EddOpen = other.b_EddOpen;
			b_EmrOpen = other.b_EmrOpen;
			b_IsContainer = other.b_IsContainer;
			b_EraseDuplicates = other.b_EraseDuplicates;
			Joints = std::move(other.Joints);
			Clip = std::move(other.Clip);
			Skeleton = std::move(other.Skeleton);
			EntityList = std::move(other.EntityList);
			Data = std::move(other.Data);
			Str.hWnd = other.Str.hWnd;
		}
		return *this;
	}

	Resident_Evil_Animation(void) :
		Skeleton(std::make_shared<Bone>()),
		m_Type(Resident_Evil_Animation_Type::None),
		b_EddOpen(false),
		b_EmrOpen(false),
		b_IsContainer(false),
		b_EraseDuplicates(false),
		EntityList{}
	{
		SetGame(Video_Game::Resident_Evil);
	}

	~Resident_Evil_Animation(void) = default;

	class Bone : public std::enable_shared_from_this<Bone> {
	private:

		bool b_Initialized = false;

		Bone(const Bone&) = delete;
		Bone& operator=(const Bone&) = delete;

		Bone(Bone&&) = delete;
		Bone& operator=(Bone&&) = delete;

		void AddChildren(const std::vector<Joint>& Joints, std::size_t Index)
		{
			if (Index >= Joints.size()) { return; }

			ID = Index;
			Pos = Joints[Index].Pos;
			World = Standard_Matrix().Translate(vec3{ (float)(Pos.x) / ONE, (float)(Pos.y) / ONE, (float)(Pos.z) / ONE });

			for (const auto& Joint : Joints[Index].Ref)
			{
				if (Joint >= Joints.size()) { continue; }
				auto Child = std::make_shared<Bone>();
				Child->Parent = weak_from_this();
				Child->AddChildren(Joints, Joint);
				Children.emplace_back(std::move(Child));
			}
		}

	public:
		Bone() { Reset(); }
		virtual ~Bone() = default;

		std::size_t ID = 0;
		SVECTOR2 Pos{};
		Standard_Matrix World{};
		Standard_Matrix Local{};
		std::weak_ptr<Bone> Parent;
		std::vector<std::shared_ptr<Bone>> Children;

		// Has the skeleton been initialized?
		const bool& IsOpen(void) const { return b_Initialized; }

		std::shared_ptr<Bone> Clone(void) const
		{
			auto Copy = std::make_shared<Bone>();

			Copy->ID = this->ID;
			Copy->Pos = this->Pos;
			Copy->World = this->World;
			Copy->Local = this->Local;
			Copy->Parent.reset();

			for (const auto& Child : this->Children)
			{
				auto ChildCopy = Child->Clone();
				ChildCopy->Parent = Copy;
				Copy->Children.push_back(ChildCopy);
			}

			Copy->b_Initialized = this->b_Initialized;

			return Copy;
		}

		bool Create(const std::vector<Joint>& Joints)
		{
			Reset();

			if (Joints.empty()) { return false; }

			if (std::none_of(Joints.begin(), Joints.end(), [](const Joint& j) { return !j.Ref.empty(); })) { return false; }

			Pos = Joints[0].Pos;
			World = Standard_Matrix().Translate(vec3{ (float)(Pos.x) / ONE, (float)(Pos.y) / ONE, (float)(Pos.z) / ONE });

			for (const auto& Joint : Joints[0].Ref)
			{
				if (Joint >= Joints.size()) { continue; }
				auto Child = std::make_shared<Bone>();
				Child->Parent = weak_from_this();
				Child->AddChildren(Joints, Joint);
				Children.emplace_back(std::move(Child));
			}

			return b_Initialized = true;
		}

		void Reset(void)
		{
			b_Initialized = false;
			ID = 0;
			Pos = { 0, 0, 0 };
			World = Standard_Matrix();
			Local = Standard_Matrix();
			Parent.reset();
			Children.clear();
		}
	};

	Standard_String Str;

	// Armature Joints
	std::vector<Joint> Joints;

	// Keyframe Collection
	std::vector<std::vector<Frame>> Clip;

	// Armature Skeleton
	std::shared_ptr<Bone> Skeleton;

	/*
		Entity List
		 - available only in RBJ files
	*/
	Entity EntityList;

	/*
		Multi Container
		 - typical usage is for RBJ files
	*/
	std::vector<Resident_Evil_Animation> Data;

	// Set video game type
	using Resident_Evil_Common::SetGame;

	// Is EDD data opened?
	bool IsEDDOpen(void) const { return b_EddOpen; }

	// Is EDD and EMR data opened?
	bool IsOpen(void) const { return b_EddOpen && b_EmrOpen; }

	// Set Animation Type
	void SetType(Resident_Evil_Animation_Type Type) { m_Type = Type; }

	/*
		Open EDD file data
		 - EDD file data must be opened before EMR file data
	*/
	std::uintmax_t OpenEDD(StdFile& File, std::uintmax_t _Ptr);

	/*
		Open EMR file data
		 - EDD file data must be opened before EMR file data
	*/
	std::uintmax_t OpenEMR(StdFile& File, std::uintmax_t _Ptr);

	/*
		Open EDD file
		 - EDD file data must be opened before EMR file data
	*/
	bool OpenEDD(std::filesystem::path Path, std::uintmax_t _Ptr)
	{
		StdFile m_File;
		m_File.SetPath(Path);
		OpenEDD(m_File, _Ptr);
		return b_EddOpen;
	}

	/*
		Open EMR file
		 - EDD file data must be opened before EMR file data
	*/
	bool OpenEMR(std::filesystem::path Path, std::uintmax_t _Ptr)
	{
		StdFile m_File;
		m_File.SetPath(Path);
		OpenEMR(m_File, _Ptr);
		return b_EmrOpen;
	}

	// Save EDD data to file
	std::uintmax_t SaveEDD(StdFile& File, std::uintmax_t _Ptr);

	// Save EMR data to file
	std::uintmax_t SaveEMR(StdFile& File, std::uintmax_t _Ptr);

	// Save EDD to file
	bool SaveEDD(std::filesystem::path Path, std::uintmax_t _Ptr, bool b_Truncate = false)
	{
		StdFile m_File;

		if (b_Truncate)
		{
			m_File.Open(Path, FileAccessMode::Write_Ex, true, true);
		}
		else
		{
			m_File.Open(Path, FileAccessMode::Read_Ex, true, false);
		}

		std::uintmax_t m_Ptr = _Ptr;

		_Ptr = SaveEDD(m_File, _Ptr);

		return m_Ptr != _Ptr;
	}

	// Save EMR to file
	bool SaveEMR(std::filesystem::path Path, std::uintmax_t _Ptr, bool b_Truncate = false)
	{
		StdFile m_File;

		if (b_Truncate)
		{
			m_File.Open(Path, FileAccessMode::Write_Ex, true, true);
		}
		else
		{
			m_File.Open(Path, FileAccessMode::Read_Ex, true, false);
		}

		std::uintmax_t m_Ptr = _Ptr;

		_Ptr = SaveEMR(m_File, _Ptr);

		return m_Ptr != _Ptr;
	}

	// Open RBJ
	bool OpenRBJ(StdFile& File, std::uintmax_t _Ptr);

	// Open RBJ
	bool OpenRBJ(std::filesystem::path Path, std::uintmax_t _Ptr)
	{
		StdFile m_File;
		m_File.SetPath(Path);
		return OpenRBJ(m_File, _Ptr);
	}

	// 
	void Close(void);

	// Calculate total EDD file size
	std::uintmax_t SizeEDD(void);

	// Calculate total EMR file size
	std::uintmax_t SizeEMR(void);

	// Calculate individual keyframe byte size
	std::uint16_t GetFrameLength(void) const;

	// Get clip count
	std::size_t GetClipCount(void) const { return Clip.size(); }

	// Get keyframe count of individual clip
	std::size_t GetFrameCount(std::size_t iClip) const { return Clip.empty() ? 0 : Clip[iClip].size(); }

	/*
		Calculate total number of unique keyframes in individual clip
		 - for use with Bio3 only
	*/
	std::size_t GetUniqueFrameCount(std::size_t iClip, bool UpdateEDD = true);

	/*
		Calculate total number of unique keyframes from all clips
		 - for use with Bio1 and Bio2 only
	*/
	std::size_t GetUniqueFrameCountTotal(bool UpdateEDD = true);

	/*
		Create aramture skeleton
		 - must be called after both EDD and EMR data is opened
	*/
	bool CreateSkeleton(void) const { return Skeleton->Create(Joints); }

};