// Dear ImGui: standalone example application for DirectX 9
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>

/*SeekAnAnswer*/
#include "SeekAnAnswer/kernel/Fraction.h"
#include "SeekAnAnswer/kernel/monomial.h"
#include "SeekAnAnswer/kernel/polynomial.h"
#include "SeekAnAnswer/kernel/polynomial_exponential.h"
#include "SeekAnAnswer/kernel/polynomial_exponential_sum.h"
#include "SeekAnAnswer/kernel/equation.h"

#pragma comment(lib,"d3d9.lib")

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool Show_SeekAnAnswer_test = false;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("Test_Calculator");

            ImGui::Text("Please select the function that you want.");
            ImGui::Checkbox("SeekAnAnswer_test", &Show_SeekAnAnswer_test);

            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        //Show a test
        if (Show_SeekAnAnswer_test)
        { 
            ImGui::Begin("SeekAnAnswer test", &Show_SeekAnAnswer_test);
            if (ImGui::CollapsingHeader("Explain"))
            {
                ImGui::Text("This is a windows that is used to test SeekAnAnswer's functionality.");
                ImGui::Text("Here,you can easily test each feature of SeekAnAnser");
            }
            ImGui::Text("--TEST--");
            if (ImGui::CollapsingHeader("Monomial"))
            {
                // ‰»Î≤‚ ‘
                static bool Show_Input = false;
                //≈–∂œÕ¨¿‡œÓ≤‚ ‘
                static bool Show_SimilarItems = false;
                //≥À∑®‘ÀÀ„≤‚ ‘
                static bool Show_MUL = false;
                //≥˝∑®‘ÀÀ„≤‚ ‘
                static bool Show_division = false;

                ImGui::Text("This module is used to represent a monimial (ax^n)");
                ImGui::Checkbox("Input Test", &Show_Input);
                ImGui::Checkbox("Similar Items Test", &Show_SimilarItems);
                ImGui::Checkbox("MUL Test", &Show_MUL);
                ImGui::Checkbox("division Test", &Show_division);

                if(Show_Input)
                {
                    ImGui::Begin("Test Monomial Input", &Show_Input);
                    ImGui::Text("--Input--");

                    static char SetValueText[128] = "(1/1)a^(1/1)";
                    static char InputText[128] = "a";
                    static Monomial result("(1/1)a^(1/1)");

                    ImGui::InputText("SetValue", SetValueText, IM_ARRAYSIZE(SetValueText));
                    ImGui::SameLine();
                    if (ImGui::Button("update1"))
                        result.SetValue((std::string)SetValueText);

                    ImGui::InputText("Input", InputText, IM_ARRAYSIZE(InputText));
                    ImGui::SameLine();
                    if (ImGui::Button("update2"))
                        result.Input((std::string)InputText);

                    ImGui::Text("--Out--");
                    ImGui::Text("GetValue: %s", result.GetValue().c_str());
                    ImGui::Text("Out: %s", result.Out().c_str());
                    ImGui::End();
                }
                if (Show_SimilarItems)
                {
                    ImGui::Begin("Test Monomial Similar Items", &Show_SimilarItems);
                    ImGui::Text("--Input--");

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static std::string result = "wait";
                    static Monomial a;
                    static Monomial b;

                    ImGui::InputText("Input the a", InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText("Input the b", InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button("SimilarItems"))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = (a.SimilarItems(b)) ? "Yes" : "No";
                    }

                    ImGui::Text("--Result--");
                    ImGui::Text("Is Similar items : %s", result.c_str());
                    ImGui::End();
                }
                if (Show_MUL)
                {
                    ImGui::Begin("Test Monomial MUL", &Show_MUL);
                    ImGui::Text("--Input--");

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static Monomial a;
                    static Monomial b;
                    static Monomial result;

                    ImGui::InputText("Input the a", InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText("Input the b", InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button("result..."))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = a * b;
                    }

                    ImGui::Text("--Result--");
                    ImGui::Text("GetValue : %s", result.GetValue().c_str());
                    ImGui::Text("Out : %s", result.Out().c_str());
                    ImGui::End();
                }
                if (Show_division)
                {
                    ImGui::Begin("Test Monomial divition", &Show_division);
                    ImGui::Text("--Input--");

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static Monomial a;
                    static Monomial b;
                    static Monomial result;

                    ImGui::InputText("Input the a", InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText("Input the b", InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button("result..."))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = a / b;
                    }

                    ImGui::Text("--Result--");
                    ImGui::Text("GetValue : %s", result.GetValue().c_str());
                    ImGui::Text("Out : %s", result.Out().c_str());
                    ImGui::End();
                }

            }
            if (ImGui::CollapsingHeader("Polynomial"))
            {
                // ‰»Î≤‚ ‘
                static bool Show_Input = false;
                //≈–∂œÕ¨¿‡œÓ≤‚ ‘
                static bool Show_unite_like_terms = false;
                //º”∑®‘ÀÀ„≤‚ ‘
                static bool Show_add = false;
                //ºı∑®‘ÀÀ„≤‚ ‘
                static bool Show_subtraction = false;
                //≥À∑®‘ÀÀ„≤‚ ‘
                static bool Show_MUL = false;
                //≥˝∑®‘ÀÀ„≤‚ ‘
                static bool Show_division = false;

                ImGui::Text("This module is used to represent a Polynomial (ax^n + bx^m +...)");
                ImGui::Checkbox("Input Test", &Show_Input);
                ImGui::Checkbox("unite like terms Test", &Show_unite_like_terms);
                ImGui::Checkbox("MUL Test", &Show_MUL);
                ImGui::Checkbox("division Test", &Show_division);

                if (Show_Input)
                {
                    ImGui::Begin("Test Polynomial Input", &Show_Input);
                    ImGui::Text("--Input--");

                    static char SetValueText[128] = "(1/1)a^(1/1)";
                    static char InputText[128] = "a";
                    static Polynomial result("(1/1)a^(1/1)");

                    ImGui::InputText("SetValue", SetValueText, IM_ARRAYSIZE(SetValueText));
                    ImGui::SameLine();
                    if (ImGui::Button("update1"))
                        result.SetValue((std::string)SetValueText);

                    ImGui::InputText("Input", InputText, IM_ARRAYSIZE(InputText));
                    ImGui::SameLine();
                    if (ImGui::Button("update2"))
                        result.Input((std::string)InputText);

                    ImGui::Text("--Out--");
                    ImGui::Text("GetValue: %s", result.GetValue().c_str());
                    ImGui::Text("Out: %s", result.Out().c_str());
                    ImGui::End();
                }
                if (Show_unite_like_terms)
                {
                    ImGui::Begin("Test Polynomial Show_unite_like_terms", &Show_unite_like_terms);
                    ImGui::Text("--Input--");

                    static char InputText1[128] = "a";
                    static Polynomial a;

                    ImGui::InputText("Input the a", InputText1, IM_ARRAYSIZE(InputText1));
                    if (ImGui::Button("unite like terms"))
                    {
                        a.Input(InputText1);
                        a.Unite_like_terms();
                    }

                    ImGui::Text("--Result--");
                    ImGui::Text("result : %s", a.Out().c_str());
                    ImGui::End();
                }
                if (Show_add)
                {
                    ImGui::Begin("Test Polynomial add", &Show_add);
                    ImGui::Text("--Input--");

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static Polynomial a;
                    static Polynomial b;
                    static Polynomial result;

                    ImGui::InputText("Input the a", InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText("Input the b", InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button("result..."))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = a + b;
                    }

                    ImGui::Text("--Result--");
                    ImGui::Text("GetValue : %s", result.GetValue().c_str());
                    ImGui::Text("Out : %s", result.Out().c_str());
                    ImGui::End();
                }
                if (Show_subtraction)
                {
                    ImGui::Begin("Test Polynomial subtraction", &Show_subtraction);
                    ImGui::Text("--Input--");

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static Polynomial a;
                    static Polynomial b;
                    static Polynomial result;

                    ImGui::InputText("Input the a", InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText("Input the b", InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button("result..."))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = a - b;
                    }

                    ImGui::Text("--Result--");
                    ImGui::Text("GetValue : %s", result.GetValue().c_str());
                    ImGui::Text("Out : %s", result.Out().c_str());
                    ImGui::End();
                }
                if (Show_MUL)
                {
                    ImGui::Begin("Test Polynomial MUL", &Show_MUL);
                    ImGui::Text("--Input--");

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static Polynomial a;
                    static Polynomial b;
                    static Polynomial result;

                    ImGui::InputText("Input the a", InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText("Input the b", InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button("result..."))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = a * b;
                    }

                    ImGui::Text("--Result--");
                    ImGui::Text("GetValue : %s", result.GetValue().c_str());
                    ImGui::Text("Out : %s", result.Out().c_str());
                    ImGui::End();
                }
                if (Show_division)
                {
                    ImGui::Begin("Test Polynomial division", &Show_division);
                    ImGui::Text("--Input--");

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static Polynomial a;
                    static Polynomial b;
                    static Polynomial result;

                    ImGui::InputText("Input the a", InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText("Input the b", InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button("result..."))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = a / b;
                    }

                    ImGui::Text("--Result--");
                    ImGui::Text("GetValue : %s", result.GetValue().c_str());
                    ImGui::Text("Out : %s", result.Out().c_str());
                    ImGui::End();
                }

            }
            if (ImGui::CollapsingHeader("Equation"))
            {
                // ‰»Î≤‚ ‘
                static bool Show_Input = false;
                static bool Show_Solution_equation = false;

                ImGui::Text("This module is used to represent a Equation");
                ImGui::Checkbox("Input Test", &Show_Input);
                ImGui::Checkbox("Solution equation Test", &Show_Solution_equation);

                if (Show_Input)
                {
                    ImGui::Begin("Test Equation Input", &Show_Input);
                    ImGui::Text("--Input--");

                    static char SetValueText[128] = "((1/1)a^(1/1))*X^(1/1)=((1/1))*X^(0/1)";
                    static char InputText[128] = "aX=1";
                    static Equation result;

                    ImGui::InputText("SetValue", SetValueText, IM_ARRAYSIZE(SetValueText));
                    ImGui::SameLine();
                    if (ImGui::Button("update1"))
                    {
                        result.SetValue((std::string)SetValueText);
                        result.ShiftItem();
                    }

                    ImGui::InputText("Input", InputText, IM_ARRAYSIZE(InputText));
                    ImGui::SameLine();
                    if (ImGui::Button("update2"))
                    {
                        result.Input((std::string)InputText);
                        result.ShiftItem();
                    }

                    ImGui::Text("--Out--");
                    ImGui::Text("GetValue: %s", result.GetValue().c_str());
                    ImGui::Text("Out: %s", result.Out().c_str());
                    ImGui::End();
                }
                if (Show_Solution_equation)
                {
                    ImGui::Begin("Test solution equation ", &Show_Solution_equation);
                    ImGui::Text("--Input--");

                    static char InputText1[128] = "aX^2 + bX +c = 0";
                    static std::string result_text1;
                    static std::string result_text2;
                    static Equation a;
                    
                    Fraction<Polynomial> result1;
                    std::vector<Fraction<Polynomial_Exponential_Sum>> result2;

                    ImGui::InputText("Input the Equation", InputText1, IM_ARRAYSIZE(InputText1));
                    if (ImGui::Button("result..."))
                    {
                        a.Input(InputText1);
                        if (a.GetTheHighestDegreeTermOfTheUnknown() == Fraction<sint64>(1, 1))
                        {
                            result1 = a.linear_equation_with_one_unknown();
                            result_text1 = Out(result1);
                            result_text2 = "There is only one result to this equation";
                        }
                        else if (a.GetTheHighestDegreeTermOfTheUnknown() == Fraction<sint64>(2, 1))
                        {
                            result2 = a.quadratic_equation_in_one_unknown();
                            result_text1 = result2.at(0).Out();
                            result_text2 = result2.at(1).Out();
                        }
                    }

                    ImGui::Text("--Result--");
                    ImGui::Text("result 1 = %s", result_text1.c_str());
                    ImGui::Text("result 2 = %s", result_text2.c_str());
                    ImGui::End();
                }

            }
            ImGui::End();
        }


        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
