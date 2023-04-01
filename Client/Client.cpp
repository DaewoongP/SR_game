// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"
#include "MainApp.h"
#include"imgui.h"
#include"imgui_impl_dx9.h"
#include"imgui_impl_win32.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND		g_hWnd;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
  
    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
  
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));
    
    MSG msg;
	msg.message = WM_NULL;
 
	CMainApp*		pMainApp = CMainApp::Create();

	if (nullptr == pMainApp)
		return FALSE;

  // Setup Platform/Renderer backends

    //imgui
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&wc);
    //HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);
 
    ::ShowWindow(g_hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(g_hWnd);
  
    // imgui 객체 생성
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


    //imgui의 색 (디폴트 : 검정)
    ImGui::StyleColorsDark();
    //// Setup Platform/Renderer backends
    LPDIRECT3DDEVICE9 pd3dDevice = Engine::Get_GraphicDev();
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX9_Init(pd3dDevice);
    bool show_demo_window = true;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  
	FAILED_CHECK_RETURN(Engine::Ready_Timer(L"Timer_Immediate"), FALSE);
	FAILED_CHECK_RETURN(Engine::Ready_Timer(L"Timer_FPS60"), FALSE);

	FAILED_CHECK_RETURN(Engine::Ready_Frame(L"Frame60", 60.f), FALSE);

    // 기본 메시지 루프입니다.
    while (true)
    { 
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			Engine::Set_Timer(L"Timer_Immediate");
			_float fTimer_Immediate = Engine::Get_Timer(L"Timer_Immediate");

			// 60프레임 제한을 걸고싶다.
			if (Engine::IsPermit_Call(L"Frame60", fTimer_Immediate))
			{
				// 60분의 1초 에 들어오겠죠

				Engine::Set_Timer(L"Timer_FPS60");
				_float fTimer_FPS60 = Engine::Get_Timer(L"Timer_FPS60");

				pMainApp->Update_MainApp(fTimer_FPS60);
				pMainApp->LateUpdate_MainApp();
                pMainApp->Render_MainApp();

               //
               // ImGui_ImplDX9_NewFrame();
               // ImGui_ImplWin32_NewFrame();
               // ImGui::NewFrame();

               // //// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
               // if (show_demo_window)
               //     ImGui::ShowDemoWindow(&show_demo_window);

               //// //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
               // {
               //     static float f = 0.0f;
               //     static int counter = 0;
               //     ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
               //
               //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
               //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
               //    ImGui::Checkbox("Another Window", &show_another_window);

               //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
               //    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

               //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
               //        counter++;
               //    ImGui::SameLine();
               //    ImGui::Text("counter = %d", counter);
               //  
               //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
               //    ImGui::End();
               //}

               ////// 3. Show another simple window.
               //if (show_another_window)
               //{
               //     ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
               //     ImGui::Text("Hello from another window!");
               //     if (ImGui::Button("Close Me"))
               //         show_another_window = false;
               //     ImGui::End();
               // }
               
                //// Rendering
                //ImGui::EndFrame();
                //pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
                //pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
                //pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
                //D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
                ////pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
                //if (pd3dDevice->BeginScene() >= 0)
                //{
                //    ImGui::Render();
                //    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
                //    pd3dDevice->EndScene();
                //}

                //HRESULT result = pd3dDevice->Present(NULL, NULL, NULL, NULL);


               
			} 
			
		}        
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
	_ulong dwRefCnt = 0;

	if (dwRefCnt = Safe_Release(pMainApp))
	{
		MSG_BOX("MainApp Release Failed");
		return FALSE;
	}

    
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_CROSS);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   RECT rc{ 0, 0, WINCX, WINCY };

   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 
	  rc.right - rc.left, 
	  rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

 
   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;
 
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   ::ShowWindow(g_hWnd, SW_SHOWDEFAULT);
   ::UpdateWindow(g_hWnd); 
   //IMGUI_CHECKVERSION();
   //ImGui::CreateContext();
   //ImGuiIO& io = ImGui::GetIO(); (void)io;
   //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
   //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

   ////// Setup Dear ImGui style
   //ImGui::StyleColorsDark();
   //ImGui::StyleColorsLight();

   //// Setup Platform/Renderer backends
  /* LPDIRECT3DDEVICE9 pd3dDevice = Engine::Get_GraphicDev();
   ImGui_ImplWin32_Init(g_hWnd);
   ImGui_ImplWin32_Init(hWnd);
   ImGui_ImplDX9_Init(pd3dDevice);*/

   
  
   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
// 

// imgui의 툴바를 사용하는데 필요한것으로보임
#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{//imgui 클릭시 필요
   if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
