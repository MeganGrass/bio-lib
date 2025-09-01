/*
*
*	Megan Grass
*	July 11, 2025
*
*/


#pragma once

#include <gte/lib_gte.h>

#include <std_matrix.h>

#include <bio_common.h>

#include <animation/bio1_anim_state.h>

#include <animation/bio2_anim_state.h>

#include <animation/bio2_anim_state_nov96.h>

#include <animation/bio3_anim_state.h>


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
	const std::uint32_t AnimType(void) const { return std::to_underlying(m_Type); }

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
	std::vector<std::shared_ptr<Resident_Evil_Animation>> Data;

	// Set video game type
	using Resident_Evil_Common::SetGame;

	// Is EDD data opened?
	bool IsEDDOpen(void) const { return b_EddOpen; }

	// Is EDD and EMR data opened?
	bool IsOpen(void) const { return b_EddOpen && b_EmrOpen; }

	// Set Animation Type
	void SetType(Resident_Evil_Animation_Type Type) { m_Type = Type; }

	// Get Animation Type
	const AnimationIndex GetType(void) const
	{
		switch (m_Type)
		{
		case Resident_Evil_Animation_Type::Normal: return AnimationIndex::Normal;
		case Resident_Evil_Animation_Type::NormalEx0: return AnimationIndex::NormalEx0;
		case Resident_Evil_Animation_Type::NormalEx1: return AnimationIndex::NormalEx1;
		case Resident_Evil_Animation_Type::Damage: return AnimationIndex::Damage;
		case Resident_Evil_Animation_Type::Weapon: return AnimationIndex::Weapon;
		case Resident_Evil_Animation_Type::WeaponEx0: return AnimationIndex::WeaponEx0;
		case Resident_Evil_Animation_Type::WeaponEx1: return AnimationIndex::WeaponEx1;
		case Resident_Evil_Animation_Type::Room: return AnimationIndex::Room;
		default: return AnimationIndex::Count; // Invalid type
		}
	}

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