/*
*
*	Megan Grass
*	April 5, 2024
* 
*	Controller to change camera in debug mode
*
*/


#include "bio_global.h"


#include "std_box.h"


// Global Application
std::unique_ptr<Global_Application> G = std::make_unique<Global_Application>();

// Window Message Handler
extern LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

// Render Message Handler
extern LRESULT CALLBACK RenderProc(HWND, UINT, WPARAM, LPARAM);


/*
	Initialize
*/
int Global_Application::Main(int Width, int Height, HINSTANCE hInstance, LPWSTR lpCmdLine)
{
	{
		std::vector<TBBUTTON> ToolbarButtons(4);
		ToolbarButtons[0] = { MAKELONG(0, 0), IDC_TOOLBAR_OPEN, TBSTATE_ENABLED, BTNS_AUTOSIZE | BTNS_SHOWTEXT, {}, 0, 0 };	// (INT_PTR)L"Open\0\0"
		ToolbarButtons[1] = { MAKELONG(1, 0), IDC_TOOLBAR_CLOSE, TBSTATE_ENABLED, BTNS_AUTOSIZE, {}, 0, 0 };
		ToolbarButtons[2] = { MAKELONG(2, 0), IDC_TOOLBAR_SAVE, TBSTATE_ENABLED, BTNS_AUTOSIZE, {}, 0, 0 };
		ToolbarButtons[3] = { MAKELONG(3, 0), IDC_TOOLBAR_NEW, TBSTATE_ENABLED, BTNS_AUTOSIZE, {}, 0, 0 };

		Window->PresetStyle(WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
		Window->PresetStyleEx(WS_EX_ACCEPTFILES | WS_EX_APPWINDOW);
		Window->SetCaptionName(hInstance, IDS_APP_TITLE);
		Window->PresetClassName(hInstance, IDR_WINDOW);
		Window->SetIcon(hInstance, IDI_WINDOW);
		Window->SetIconSmall(hInstance, IDI_WINDOW);
		Window->SetMenu(hInstance, IDR_WINDOW);
		Window->SetAcceleratorTable(hInstance, IDR_WINDOW);
		Window->SetToolBar(32, 32, ToolbarButtons);
		Window->AddToolBarIcon(hInstance, IDI_TOOLBAR_OPEN);
		Window->AddToolBarIcon(hInstance, IDI_TOOLBAR_CLOSE);
		Window->AddToolBarIcon(hInstance, IDI_TOOLBAR_SAVE);
		Window->AddToolBarIcon(hInstance, IDI_TOOLBAR_NEW);
		Window->SetStatusBar(4);
		Window->SetColor(0, 0, 0);
		Window->Create(Width, Height, hInstance, SW_HIDE, WindowProc, WindowOptions::ToolBar | WindowOptions::StatusBar);

		Window->SetTimer(30);

		Device = Window->Device();

		Render->Initialize(Window->CreateChild(0, 0, Width, Height, hInstance, SW_SHOW, RenderProc, NULL, NULL), 320, 240, false);

		ShowWindow(Window->Get(), SW_SHOWDEFAULT);

		while (Render->DeviceState() != D3DDEVICE_STATE::NORMAL) { Window->SleepTimer(); }

		if (Render->NativeResolution())
		{
			Camera->SetOrtho(320.0f, 240.0f);
		}
		else
		{
			Camera->SetOrtho(static_cast<float>(Width), static_cast<float>(Height));
		}

		Render->CreateAxisGrid();
	}

	{
		Camera->BackgroundVert = Render->CreateVec4t(Camera->GetVert());

		Camera->b_Mirror = false;

		SetCamera(-1);
	}

	// temporary
	{
		Resident_Evil_Model* Model1 = new Resident_Evil_Model();
		Resident_Evil_2_Model* Model2 = new Resident_Evil_2_Model();
		Model1->Anim[ANIM_NORMAL]->NewAnimation();
		Model2->Anim[ANIM_NORMAL]->NewAnimation();
		Model1->Anim[ANIM_NORMAL]->AddFrame(0, Model2->GetAnimation(ANIM_NORMAL)->GetFrame(0, 0));
	}

	// temporary
	SetGame(Resident_Evil_Video_Game::Resident_Evil_2);

	// temporary
	{
		Cut = 10;
		Bio2->Rdt->Open(L"F:\\Source\\BioAssets\\Bio2\\ROOM10a0.RDT");
		SetCamera(Cut);
		Window->Status(1, "Stage: %X", Bio2->Rdt->Stage);
		Window->Status(2, "Room: %02X", Bio2->Rdt->Room);
	}

	// temporary
	std::unique_ptr<Standard_Box> Tmp = std::make_unique<Standard_Box>(GTE->ToFloat(10000 / ONE), GTE->ToFloat(8000 / ONE), GTE->ToFloat(18000 / ONE));
	//IDirect3DVertexBuffer9* Vertices = Render->CreateVec3t(Tmp->Vertices, Tmp->UV);
	IDirect3DVertexBuffer9* Vertices = Render->CreateVec3gt(Tmp->Vertices, 0xFFFF00FF, Tmp->UV);
	IDirect3DIndexBuffer9* Indices = Render->CreateIndexBuffer(D3DFMT_INDEX32, Tmp->Indices.data(), Tmp->Indices.size() * sizeof(uint32_t));

	// temporary
	float Rotation = 0.0f;

	// Add these variables to your class or as global variables
	std::chrono::steady_clock::time_point lastUpdateTime;
	float rotationSpeed = 2.0f; // Rotation speed in degrees per second

	// Initialize the last update time at the start of the Main function
	lastUpdateTime = std::chrono::steady_clock::now();

	D3DPRESENTSTATS PresentStats{};
	UINT SyncRefreshCount = 0;
	DWORD PresentFlags = 0;

	HACCEL hAccTable = Window->GetAcceleratorTable();

	MSG msg{};
	msg.message = NULL;
	msg.hwnd = Window->Get();
	while (msg.message != WM_QUIT)
	{
		while (::TranslateAccelerator(msg.hwnd, hAccTable, &msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				return (int)msg.wParam;
			}
		}

		Update();

		// Calculate the time difference
		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsedTime = currentTime - lastUpdateTime;
		lastUpdateTime = currentTime;

		// Update the rotation based on the elapsed time
		float deltaRotation = rotationSpeed * elapsedTime.count();
		Rotation += deltaRotation;

		Render->Device()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 128, 255), 1.0f, 0);
		Render->Device()->BeginScene();

		// Background
		if (Camera->Background)
		{
			Render->DrawVec4t(Camera->BackgroundVert, Camera->Background, Render->PS1DitherPixelShader);
		}

		// Axis Grid
		Render->DrawAxisGrid();

		// Box, textured
		Render->SetWorld(0.0f, 0.0f, 0.0f, 0.0f, Rotation, 0.0f);
		Render->DrawVec3gt(Vertices, Indices, Camera->Background);
		Render->DrawVec3gt(Vertices, Indices, nullptr, nullptr, D3DFILL_WIREFRAME, D3DPT_LINESTRIP);
		Render->SetWorld(Camera->World);

		Render->Device()->EndScene();
		Render->Device()->PresentEx(NULL, NULL, NULL, NULL, PresentFlags);

		Render->SwapChain()->GetPresentStats(&PresentStats);

		if ((PresentStats.SyncRefreshCount - SyncRefreshCount) > 1)
		{
			PresentFlags = D3DPRESENT_FORCEIMMEDIATE;	// | D3DPRESENT_DONOTFLIP;

		}
		else
		{
			PresentFlags = NULL;
		}

		SyncRefreshCount = PresentStats.SyncRefreshCount;

		Window->SleepTimer();
	}

	Vertices->Release();
	Indices->Release();

	return (int)msg.wParam;
}

/*
	Update global state
*/
void Global_Application::Update(void)
{
	if (Device->GetKeyDown(VK_ESCAPE))
	{
		PostMessage(Window->Get(), WM_CLOSE, 0, 0);
	}

	if (Device->GetKeyDown(VK_F11))
	{
		if (Window->IsMaximized())
		{
			ShowWindow(Window->Get(), SW_RESTORE);
		}
		else
		{
			ShowWindow(Window->Get(), SW_MAXIMIZE);
		}
	}

	if (Device->GetKeyDown(VK_LEFT))
	{
		SetCamera(--Cut);
	}

	if (Device->GetKeyDown(VK_RIGHT))
	{
		SetCamera(++Cut);
	}
}

/*
	Set active camera
*/
void Global_Application::SetCamera(std::uint8_t iCut)
{
	switch (std::to_underlying(Game) & (AUG95 | OCT95 | BIO1 | BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
	{
	case AUG95:
	case OCT95:
	case BIO1:
		Cut = 0xFF;
		break;
	case BIO2NOV96:
	case BIO2TRIAL:
	case BIO2:
	{
		if (Bio2->Rdt->IsOpen())
		{
			if (iCut > (Bio2->Rdt->GetCameraCount() - 1))
			{
				iCut = Bio2->Rdt->GetCameraCount() - 1;
			}

			Cut = iCut;

			Camera->Set_view(Bio2->Rdt->Rid->Get(Cut)->ViewR >> 7,
				{ Bio2->Rdt->Rid->Get(Cut)->View_p.x, Bio2->Rdt->Rid->Get(Cut)->View_p.y, Bio2->Rdt->Rid->Get(Cut)->View_p.z },
				{ Bio2->Rdt->Rid->Get(Cut)->View_r.x, Bio2->Rdt->Rid->Get(Cut)->View_r.y, Bio2->Rdt->Rid->Get(Cut)->View_r.z });
		}
		else
		{
			Cut = 0xFF;
		}
	}
	break;
	case BIO3:
		Cut = 0xFF;
		break;
	default:
		Cut = 0xFF;
		break;
	}

	if (Cut == 0xFF)
	{
		Camera->Set_view(0x6DD4 >> 7, { -16000, -7200, -16000 }, { 0, 7200, 0 });
	}

	Render->SetView(Camera->View);
	Render->SetProjection(Camera->Projection);

	if (Camera->Background)
	{
		Camera->Background->Release();
		Camera->Background = nullptr;
	}

	if (Cut != 0xFF)
	{
		std::unique_ptr<Standard_Image> Image = Bio2->OpenBSS(L"F:\\Games\\BioHazard\\Bio2\\BSS\\Dual Shock\\room10a.bss", Cut);
		Camera->Background = Render->CreateTexture(Image);


		//std::unique_ptr<Sony_PlayStation_Texture> Texture = std::make_unique<Sony_PlayStation_Texture>();// { L"C:\\Framework\\Visual Studio 2022\\reio\\Build\\Release\\x64" };
		//Texture->Open(L"C:\\Framework\\Visual Studio 2022\\reio\\Build\\Release\\x64\\esp0A.tim");
		//std::unique_ptr<Standard_Image> Image = Texture->GetBitmap();
		//Camera->Background = Render->CreateTexture(Image);
	}
	
	Window->Status(3, "Camera: %d", Cut);
}