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

#include <std_basic_fstream.h>

#include <std_text.h>

#include <lib_gte.h>

#include <bio_common.h>


#pragma pack(push, 1)


struct Resident_Evil_EDD_Header
{
	std::uint16_t nFrames;				// Frame count
	std::uint16_t pAttr;				// Attribute pointer
};


struct Resident_Evil_EDD_Header_Bio3
{
	std::uint16_t nFrames;				// Frame count
	std::uint16_t pAttr;				// Attribute pointer
	std::uint32_t Counter;				// Total unique frame counter
};


struct Resident_Evil_EMR_Header
{
	std::uint16_t pJoint;				// Joint pointer
	std::uint16_t pFrames;				// Frame pointer
	std::uint16_t nJoint;				// Joint count
	std::uint16_t FrameLen;				// Size of frame
};


struct Resident_Evil_RBJ_Header
{
	std::uint16_t Flag0;				// Unknown Flag
	std::uint16_t Flag1;				// Unknown Flag
};


struct Resident_Evil_Joint
{
	SVECTOR2 Pos{};						// Root position
	std::vector<std::uint8_t> Ref;		// Object references
};


struct Resident_Evil_Frame
{
	struct Attribute
	{
		std::uint32_t pEmr;				// EMR index // offset = (pFrames + (pEmr * FrameLen))
		std::uint32_t Speed;			// Speed multiplier (1 = 15fps delay, 2 = 30fps delay, ...)
		std::uint32_t Sound;			// Sound flag (Bio2 and Bio3, only)
	} Attr{};
	SVECTOR2 Origin{};					// Origin position
	SVECTOR2 Speed{};					// Speed position
	std::vector<SVECTOR2> Rotation;		// Rotation attributes
};


#pragma pack(pop)


enum class Resident_Evil_Animation_Type : int
{
//	Flag								// File			Joints?		Notes
	Normal = 1 << 0,					// EMD/PLD		Yes			Bio1/Bio2/Bio3 = first animation
	Normal_Ex = 1 << 1,					// EMD			Yes			Bio3 = second animation
	Extended = 1 << 2,					// EMD			No			Bio2 = second animation
	Damage = 1 << 3,					// EMD			No			Bio1 = second animation, Bio2/Bio3 = third animation
	Weapon = 1 << 4,					// EMW/PLW		No			Bio1/Bio2/Bio3 = first animation
	Weapon_Ex0 = 1 << 5,				// EMD/PLW		Yes			Bio3 = second animation (PLW), fourth animation (EMD)
	Weapon_Ex1 = 1 << 6,				// EMD/PLW		Yes			Bio3 = third animation (PLW), fifth animation (EMD)
	Room = 1 << 7,						// RDT/RBJ		No
};


#define ANIM_NORMAL			0
#define ANIM_NORMAL_EX		1
#define ANIM_EXTENDED		2
#define ANIM_DAMAGE			3
#define ANIM_WEAPON			4
#define ANIM_WEAPON_EX0		5
#define ANIM_WEAPON_EX1		6
#define ANIM_ROOM			7


class Resident_Evil_Animation :
	private Resident_Evil_Common {
private:

	// Resident Evil animation type constants
	static constexpr int NORMAL = std::to_underlying(Resident_Evil_Animation_Type::Normal);
	static constexpr int NORMAL_EX = std::to_underlying(Resident_Evil_Animation_Type::Normal_Ex);
	static constexpr int EXTENDED = std::to_underlying(Resident_Evil_Animation_Type::Extended);
	static constexpr int DAMAGE = std::to_underlying(Resident_Evil_Animation_Type::Damage);
	static constexpr int WEAPON = std::to_underlying(Resident_Evil_Animation_Type::Weapon);
	static constexpr int WEAPON_EX0 = std::to_underlying(Resident_Evil_Animation_Type::Weapon_Ex0);
	static constexpr int WEAPON_EX1 = std::to_underlying(Resident_Evil_Animation_Type::Weapon_Ex1);
	static constexpr int ROOM = std::to_underlying(Resident_Evil_Animation_Type::Room);

	// Type
	Resident_Evil_Animation_Type Type;

	// RBJ
	Resident_Evil_RBJ_Header Rbj;

	// Joint
	std::vector<Resident_Evil_Joint> Joint;

	// Animation
	std::vector<std::vector<Resident_Evil_Frame>> Anim;

	// Flag
	bool b_EddOpen;
	bool b_EmrOpen;

public:

	/*
		Construction
	*/
	explicit Resident_Evil_Animation(void) :
		Type(Resident_Evil_Animation_Type::Normal),
		Rbj(),
		Joint(),
		Anim(),
		b_EddOpen(false),
		b_EmrOpen(false)
	{
		SetGame(Resident_Evil_Video_Game::Resident_Evil);
	}
	virtual ~Resident_Evil_Animation(void)
	{
		CloseEDD();
		CloseEMR();
	}

	/*
		Friend
	*/
	friend class Resident_Evil_2_Animation;
	friend class Resident_Evil_3_Animation;

	/*
		Inheritance
	*/
	using Resident_Evil_Common::SetGame;

	/*
		Check if the animation is open
	*/
	bool operator !() { return !b_EddOpen && !b_EmrOpen; }

	/*
		Check if the animation is open
	*/
	bool IsOpen(void) const noexcept { return b_EddOpen && b_EmrOpen; }

	/*
		Force open if animation container is not empty
	*/
	bool ForceOpen(void) noexcept { if (!Anim.empty()) { b_EddOpen = b_EmrOpen = true; } return IsOpen(); }

	/*
		Set animation file type flag
	*/
	void SetType(Resident_Evil_Animation_Type FileType) noexcept { Type = FileType; }

	/*
		Open
	*/
	virtual std::uintmax_t OpenEDD(StdFile& File, std::uintmax_t _Ptr);

	/*
		Open
	*/
	virtual bool OpenEDD(std::filesystem::path Input, std::uintmax_t _Ptr = 0);

	/*
		Open
	*/
	virtual std::uintmax_t OpenEMR(StdFile& File, std::uintmax_t _Ptr);

	/*
		Open
	*/
	virtual bool OpenEMR(std::filesystem::path Input, std::uintmax_t _Ptr = 0);

	/*
		Save
	*/
	virtual std::uintmax_t SaveEDD(StdFile& File, std::uintmax_t _Ptr);

	/*
		Save
	*/
	virtual bool SaveEDD(std::filesystem::path Output, std::uintmax_t _Ptr = 0);

	/*
		Save
	*/
	virtual std::uintmax_t SaveEMR(StdFile& File, std::uintmax_t _Ptr);

	/*
		Save
	*/
	virtual bool SaveEMR(std::filesystem::path Output, std::uintmax_t _Ptr = 0);

	/*
		Get joint count
	*/
	std::size_t GetJointCount(void) const noexcept { return Joint.size(); }

	/*
		Get joint data
	*/
	Resident_Evil_Joint& GetJoint(std::size_t iJoint) { return Joint[iJoint]; }

	/*
		Push back empty joint
	*/
	std::size_t NewJoint(void);

	/*
		Push back joint from buffer
	*/
	void AddJoint(Resident_Evil_Joint& Input) { Joint.push_back(Input); }

	/*
		Push back joint reference
	*/
	void AddJointReference(std::size_t iJoint, std::uint8_t iObject) { Joint[iJoint].Ref.push_back(iObject); }

	/*
		Get animation count
	*/
	std::size_t GetAnimationCount(void) const noexcept { return Anim.size(); }

	/*
		Get animation data
	*/
	std::vector<Resident_Evil_Frame>& GetAnimation(std::size_t iAnim) { return Anim[iAnim]; }

	/*
		Push back empty animation
	*/
	std::size_t NewAnimation(void);

	/*
		Add animation from buffer
	*/
	void AddAnimation(std::vector<Resident_Evil_Frame>& Input) { Anim.push_back(Input); }

	/*
		Copy animation to buffer
	*/
	void CopyAnimation(std::size_t iAnim, std::vector<Resident_Evil_Frame>& Output) { Output = Anim[iAnim]; }

	/*
		Paste animation from buffer
	*/
	void PasteAnimation(std::size_t iAnim, std::vector<Resident_Evil_Frame> Input) { Anim[iAnim] = Input; }

	/*
		Insert animation
	*/
	void InsertAnimation(std::size_t iAnim, std::vector<Resident_Evil_Frame> Input) { Anim.insert(Anim.begin() + iAnim, Input); }

	/*
		Delete animation
	*/
	void DeleteAnimation(std::size_t iAnim) { Anim.erase(Anim.begin() + iAnim); }

	/*
		Get frame count
	*/
	std::size_t GetFrameCount(std::size_t iAnim) const { return Anim[iAnim].size(); }

	/*
		Get the total amount of unique frames
	*/
	virtual std::size_t GetUniqueFrameTotal(bool UpdateEDD = true);

	/*
		Get frame data
	*/
	Resident_Evil_Frame& GetFrame(std::size_t iAnim, std::size_t iFrame) { return Anim[iAnim][iFrame]; }

	/*
		Push back empty frame
	*/
	std::size_t NewFrame(std::size_t iAnim);

	/*
		Add frame from buffer
	*/
	void AddFrame(std::size_t iAnim, Resident_Evil_Frame& Input) { Anim[iAnim].push_back(Input); }

	/*
		Copy frame to buffer
	*/
	void CopyFrame(std::size_t iAnim, std::size_t iFrame, Resident_Evil_Frame& Output) { Output = Anim[iAnim][iFrame]; }

	/*
		Paste frame from buffer
	*/
	void PasteFrame(std::size_t iAnim, std::size_t iFrame, Resident_Evil_Frame Input) { Anim[iAnim][iFrame] = Input; }

	/*
		Insert frame
	*/
	void InsertFrame(std::size_t iAnim, std::size_t iFrame, Resident_Evil_Frame Input) { Anim[iAnim].insert(Anim[iAnim].begin() + iFrame, Input); }

	/*
		Delete frame
	*/
	void DeleteFrame(std::size_t iAnim, std::size_t iFrame) { Anim[iAnim].erase(Anim[iAnim].begin() + iFrame); }

	/*
		Get frame packet length
	*/
	virtual std::uint16_t GetFrameLength(void) const;

	/*
		Get EDD file size
	*/
	virtual std::uintmax_t SizeEDD(void);

	/*
		Get EMR file size
	*/
	virtual std::uintmax_t SizeEMR(void);

	/*
		Close EDD
	*/
	void CloseEDD(void);

	/*
		Close EMR
	*/
	void CloseEMR(void);

};