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

#include <bio1_animation.h>


class Resident_Evil_3_Animation :
	public Resident_Evil_Animation {
private:

public:

	/*
		Construction
	*/
	explicit Resident_Evil_3_Animation(void)
	{
		SetGame(Resident_Evil_Video_Game::Resident_Evil_3);
	}
	virtual ~Resident_Evil_3_Animation(void)
	{
	}

	/*
		Open
	*/
	virtual std::uintmax_t OpenEDD(StdFile& File, std::uintmax_t _Ptr) override;

	/*
		Open
	*/
	virtual bool OpenEDD(std::filesystem::path Input, std::uintmax_t _Ptr = 0) override;

	/*
		Open
	*/
	virtual std::uintmax_t OpenEMR(StdFile& File, std::uintmax_t _Ptr) override;

	/*
		Open
	*/
	virtual bool OpenEMR(std::filesystem::path Input, std::uintmax_t _Ptr = 0) override;

	/*
		Save
	*/
	virtual std::uintmax_t SaveEDD(StdFile& File, std::uintmax_t _Ptr) override;

	/*
		Save
	*/
	virtual bool SaveEDD(std::filesystem::path Output, std::uintmax_t _Ptr = 0) override;

	/*
		Save
	*/
	virtual std::uintmax_t SaveEMR(StdFile& File, std::uintmax_t _Ptr) override;

	/*
		Save
	*/
	virtual bool SaveEMR(std::filesystem::path Output, std::uintmax_t _Ptr = 0) override;

	/*
		Get the total amount of unique frames
	*/
	virtual std::size_t GetUniqueFrameTotal(bool UpdateEDD = true) override;

	/*
		Get the amount of unique frames in a single animation
	*/
	std::size_t GetUniqueFrameCount(std::size_t iAnim);

	/*
		Get frame packet length
	*/
	virtual std::uint16_t GetFrameLength(void) const override;

	/*
		Get EDD file size
	*/
	virtual std::uintmax_t SizeEDD(void) override;

	/*
		Get EMR file size
	*/
	virtual std::uintmax_t SizeEMR(void) override;

};