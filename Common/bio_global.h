/*
*
*	Megan Grass
*	April 5, 2024
*
*/


#pragma once

#include <resource.h>

#include <std_window.h>

#if MSTD_DX9
#include <std_dx9.h>
#endif

#include <lib_gte.h>

#include <bio_camera.h>

#include <bio1_model.h>

#include <bio2_model.h>

#include <bio3_model.h>

#include <bio1.h>

#include <bio2.h>

#include <bio2_nov96.h>

#include <bio3.h>

#include <bio_cdx.h>

#include <bio_disk.h>


typedef class Global_Application Global;

/*
	Global Application
*/
extern std::unique_ptr<Global_Application> G;

class Global_Application final :
	private Resident_Evil_Common {
private:

	/*
		Move
	*/
	Global_Application(Global_Application&&) = delete;
	Global_Application& operator = (Global_Application&&) = delete;


	/*
		Copy
	*/
	Global_Application(const Global_Application&) = delete;
	Global_Application& operator = (const Global_Application&) = delete;

public:

	/*
		Standard Window
	*/
	std::unique_ptr<Standard_Window> Window;

	/*
		Windows Devices
	*/
	std::shared_ptr<Windows_Devices> Device;

	/*
		Direct-X 9
	*/
	std::unique_ptr<Standard_DirectX_9> Render;

	/*
		View and Projection Matrices
	*/
	std::unique_ptr<Resident_Evil_Camera> Camera;

	/*
		Sony PlayStation (1994) Geometry Transformation Engine
	*/
	std::unique_ptr<Sony_PlayStation_GTE> GTE;

	/*
		Resident Evil
	*/
	std::unique_ptr<Resident_Evil> Bio1;

	/*
		Resident Evil 2 (November 1996)
	*/
	std::unique_ptr<Resident_Evil_2_Nov96> Bio2Nov96;

	/*
		Resident Evil 2
	*/
	std::unique_ptr<Resident_Evil_2> Bio2;

	/*
		Resident Evil 3
	*/
	std::unique_ptr<Resident_Evil_3> Bio3;

	/*
		CDX File Container
	*/
	std::unique_ptr<CDX_File_Container> Cdx;

	/*
		Sony PlayStation (1994) Executable [CAPCOM]
	*/
	std::unique_ptr<Capcom_Disk> Exe;

	/*
		Current Camera ID		// maybe import some official bio2 structs to proj instead...
	*/
	std::uint8_t Cut = 0;

	/*
		Construction
	*/
	explicit Global_Application(void) :
		Window{ std::make_unique<Standard_Window>() },
		Device{ nullptr },
		Render{ std::make_unique<Standard_DirectX_9>() },
		Camera{ std::make_unique<Resident_Evil_Camera>() },
		GTE{ std::make_unique<Sony_PlayStation_GTE>() },
		Bio1{ std::make_unique<Resident_Evil>() },
		Bio2Nov96{ std::make_unique<Resident_Evil_2_Nov96>() },
		Bio2{ std::make_unique<Resident_Evil_2>() },
		Bio3{ std::make_unique<Resident_Evil_3>() },
		Cdx{ std::make_unique<CDX_File_Container>() },
		Exe{ std::make_unique<Capcom_Disk>() },
		Cut(0)
	{
		//Render->SetNativeResolution(false);
		Render->AntiAliasing(true);
		Render->TextureFiltering(D3DTEXF_POINT);
	}

	/*
		Deconstruction
	*/
	virtual ~Global_Application(void)
	{
	}

	/*
		Initialize
	*/
	int Main(int Width, int Height, HINSTANCE hInstance, LPWSTR lpCmdLine);

	/*
		Update global state
	*/
	void Update(void);

	/*
		Set active camera
	*/
	void SetCamera(std::uint8_t iCut);

};