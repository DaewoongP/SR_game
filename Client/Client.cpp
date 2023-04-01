// Client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Client.h"
#include "MainApp.h"
#include"imgui.h"
#include"imgui_impl_dx9.h"
#include"imgui_impl_win32.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE g_hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
HWND		g_hWnd;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.
  
    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
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
  
    // imgui ��ü ����
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


    //imgui�� �� (����Ʈ : ����)
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

    // �⺻ �޽��� �����Դϴ�.
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

			// 60������ ������ �ɰ�ʹ�.
			if (Engine::IsPermit_Call(L"Frame60", fTimer_Immediate))
			{
				// 60���� 1�� �� ��������

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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.
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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
// 

// imgui�� ���ٸ� ����ϴµ� �ʿ��Ѱ����κ���
#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{//imgui Ŭ���� �ʿ�
   if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
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
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
