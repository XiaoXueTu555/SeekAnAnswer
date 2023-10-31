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
#include "SeekAnAnswer/kernel/arithmetic_expression.h"

/*FileIO 用于实现多语言切换*/
#include "FileIO/file_io.h"

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

// Data
//语言的文本映射
TextMap language_text;
int language = 0;

inline const char* ST(std::string key);

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
    //io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 24.0f);
    io.Fonts->AddFontFromFileTTF("ImGui/misc/fonts/msyh.ttc", 24.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    //io.Fonts->AddFontFromFileTTF("ImGui/misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("ImGui/misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    io.FontGlobalScale = 1.0f;
    io.FontAllowUserScaling = true;

    /*加载语言的key*/
    language_text.ReadKeyFile("key_list.txt");

    // Our state
    bool Show_SeekAnAnswer_test = false;
    bool Show_Language_window = false;

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

        //说明语言设置已更改
        if (language != language_text.language)
        {
            //根据所设置的语言加载对应的语言文件
            switch (language_text.language)
            {
            /*case 0: 默认是英文，为了节省磁盘与内存空间，语言的key就是英文的，其他的语言
            文件都由key来映射对应的翻译*/
            case 1:
                language_text.ReadLanguageFile("Chinese.txt");
                break;
            default:
                break;
            }
            language = language_text.language;
        }

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin(ST("Test_Calculator"));

            ImGui::Text(ST("Please select the function that you want."));
            ImGui::Checkbox(ST("SeekAnAnswer_test"), &Show_SeekAnAnswer_test);
            ImGui::Checkbox(u8"Language / 语言", &Show_Language_window);

            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        //Show a test
        if (Show_SeekAnAnswer_test)
        { 
            ImGui::Begin(ST("SeekAnAnswer test"), &Show_SeekAnAnswer_test);
            if (ImGui::CollapsingHeader(ST("Explain")))
            {
                ImGui::Text(ST("This is a windows that is used to test SeekAnAnswer's functionality."));
                ImGui::Text(ST("Here,you can easily test each feature of SeekAnAnswer"));
            }
            ImGui::Text(ST("--TEST--"));
            if (ImGui::CollapsingHeader(ST("Monomial")))
            {
                //输入测试
                static bool Show_Input = false;
                //判断同类项测试
                static bool Show_SimilarItems = false;
                //乘法运算测试
                static bool Show_MUL = false;
                //除法运算测试
                static bool Show_division = false;
                //代入功能测试
                static bool Show_Substitute = false;

                ImGui::Text(ST("This module is used to represent a monomial (ax^n)"));
                ImGui::Checkbox(ST("Input Test"), &Show_Input);
                ImGui::Checkbox(ST("Similar Items Test"), &Show_SimilarItems);
                ImGui::Checkbox(ST("MUL Test"), &Show_MUL);
                ImGui::Checkbox(ST("division Test"), &Show_division);
                ImGui::Checkbox(ST("Substitute Test"), &Show_Substitute);

                if(Show_Input)
                {
                    ImGui::Begin(ST("Test Monomial Input"), &Show_Input);
                    ImGui::Text(ST("--Input--"));

                    static char SetValueText[128] = "(1/1)a^(1/1)";
                    static char InputText[128] = "a";
                    static Monomial result("(1/1)a^(1/1)");
                    static std::string Out1, Out2;

                    ImGui::InputText("SetValue", SetValueText, IM_ARRAYSIZE(SetValueText));
                    ImGui::SameLine();
                    if (ImGui::Button(ST("update1")))
                    {
                        if (result.IsValid((std::string)SetValueText))
                        {
                            result.SetValue((std::string)SetValueText);
                            Out1 = result.GetValue();
                            Out2 = result.Out();
                        }
                        else
                        {
                            Out1 = Out2 = ST("The input is error!");
                        }
                    }

                    ImGui::InputText("Input", InputText, IM_ARRAYSIZE(InputText));
                    ImGui::SameLine();
                    if (ImGui::Button(ST("update2")))
                    {
                        if (result.IsValid((std::string)InputText))
                        {
                            result.Input((std::string)InputText);
                            Out1 = result.GetValue();
                            Out2 = result.Out();
                        }
                        else
                        {
                            Out1 = Out2 = ST("The input is error!");
                        }
                    }

                    ImGui::Text(ST("--Out--"));

                    ImGui::Text("GetValue: %s", Out1.c_str()); 
                    ImGui::SameLine(); 
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(Out1.c_str());
                    }

                    ImGui::Text("Out: %s", Out2.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy2"))
                    {
                        ImGui::SetClipboardText(Out2.c_str());
                    }
                    ImGui::End();
                }
                if (Show_SimilarItems)
                {
                    ImGui::Begin(ST("Test Monomial Similar Items"), &Show_SimilarItems);
                    ImGui::Text(ST("--Input--"));

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static std::string result = "wait";
                    static Monomial a;
                    static Monomial b;

                    ImGui::InputText(ST("Input the a"), InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText(ST("Input the b"), InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button(ST("SimilarItems")))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = (a.SimilarItems(b)) ? "Yes" : "No";
                    }

                    ImGui::Text(ST("--Result--"));
                    ImGui::Text("Is Similar items : %s", result.c_str());

                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy"))
                    {
                        ImGui::SetClipboardText(result.c_str());
                    }

                    ImGui::End();
                }
                if (Show_MUL)
                {
                    ImGui::Begin(ST("Test Monomial MUL"), &Show_MUL);
                    ImGui::Text(ST("--Input--"));

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static Monomial a;
                    static Monomial b;
                    static Monomial result;

                    ImGui::InputText(ST("Input the a"), InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText(ST("Input the b"), InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button(ST("result...")))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = a * b;
                    }

                    ImGui::Text(ST("--Result--"));
                    ImGui::Text("GetValue : %s", result.GetValue().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(result.GetValue().c_str());
                    }

                    ImGui::Text("Out : %s", result.Out().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy2"))
                    {
                        ImGui::SetClipboardText(result.Out().c_str());
                    }

                    ImGui::End();
                }
                if (Show_division)
                {
                    ImGui::Begin(ST("Test Monomial division"), &Show_division);
                    ImGui::Text(ST("--Input--"));

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static Monomial a;
                    static Monomial b;
                    static Monomial result;

                    ImGui::InputText(ST("Input the a"), InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText(ST("Input the b"), InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button(ST("result...")))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = a / b;
                    }

                    ImGui::Text(ST("--Out--"));

                    ImGui::Text("GetValue: %s", result.GetValue().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(result.GetValue().c_str());
                    }

                    ImGui::Text("Out: %s", result.Out().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy2"))
                    {
                        ImGui::SetClipboardText(result.Out().c_str());
                    }
                    ImGui::End();
                }
                if (Show_Substitute)
                {
                    ImGui::Begin(ST("Monomial Substitute Test"), &Show_Substitute);
                    static char the_monomial_text[128] = "a^2";
                    static char character_text[128] = "a";
                    static char substitute_character[128] = "3b";
                    static Monomial result("(1/1)a^(2/1)");
                    static Monomial substitute_monomial;
                    static sint8 character = 'a';
                    static std::string Out1 = "please check \"update\" ";

                    ImGui::Text(ST("please input the monomial."));
                    ImGui::InputText(ST("monomial"), the_monomial_text, IM_ARRAYSIZE(the_monomial_text));

                    ImGui::Text(ST("please input the character."));
                    ImGui::InputText(ST("character"), character_text, IM_ARRAYSIZE(character_text));

                    ImGui::Text(ST("please input the substitute monomial."));
                    ImGui::InputText(ST("substitute monomial"), substitute_character, IM_ARRAYSIZE(substitute_character));

                    if (ImGui::Button(ST("update")))
                    {

                        //如果满足单项式的条件
                        if (result.IsValid((std::string)the_monomial_text))
                        {
                            result.Input((std::string)the_monomial_text);
                        }
                        else
                        {
                            Out1 = ST("The input is error!");
                            goto Out_The_Result;
                        }  

                        //如果代换字符输入有误，则报错
                        if (((std::string)character_text).size() != 1)
                        {
                            Out1 = ST("The input is error!");
                            goto Out_The_Result;
                        }

                        //输入代换字符
                        character = character_text[0];

                        //如果满足单项式的条件
                        if (substitute_monomial.IsValid((std::string)substitute_character))
                        {
                            substitute_monomial.Input((std::string)substitute_character);
                        }
                        else
                        {
                            Out1 = ST("The input is error!");
                            goto Out_The_Result;
                        }

                        //代换字符
                        if (!result.Substitute(character, substitute_monomial))
                        {
                            Out1 = ST("The input is error!");
                            goto Out_The_Result;
                        }
                        Out1 = result.Out();
                    }
                    //跳转标签，显示结果
                Out_The_Result:

                    ImGui::Text(ST("--Out--"));
                    ImGui::Text("The result: %s", Out1.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy"))
                    {
                        ImGui::SetClipboardText(Out1.c_str());
                    }

                    ImGui::End();
                }

            }
            if (ImGui::CollapsingHeader(ST("Polynomial")))
            {
                //输入测试
                static bool Show_Input = false;
                //判断同类项测试
                static bool Show_unite_like_terms = false;
                //加法运算测试
                static bool Show_add = false;
                //减法运算测试
                static bool Show_subtraction = false;
                //乘法运算测试
                static bool Show_MUL = false;
                //除法运算测试
                static bool Show_division = false;
                //代换功能测试
                static bool Show_Substitute = false;

                ImGui::Text(ST("This module is used to represent a Polynomial (ax^n + bx^m +...)"));
                ImGui::Checkbox(ST("Input Test"), &Show_Input);
                ImGui::Checkbox(ST("unite like terms Test"), &Show_unite_like_terms);
                ImGui::Checkbox(ST("MUL Test"), &Show_MUL);
                ImGui::Checkbox(ST("division Test"), &Show_division);
                ImGui::Checkbox(ST("Substitute Test"), &Show_Substitute);

                if (Show_Input)
                {
                    ImGui::Begin(ST("Test Polynomial Input"), &Show_Input);
                    ImGui::Text(ST("--Input--"));

                    static char SetValueText[128] = "(1/1)a^(1/1)";
                    static char InputText[128] = "a";
                    static Polynomial result("(1/1)a^(1/1)");
                    static std::string Out1, Out2;

                    ImGui::InputText("SetValue", SetValueText, IM_ARRAYSIZE(SetValueText));
                    ImGui::SameLine();
                    if (ImGui::Button(ST("update1")))
                    {
                        if (result.IsValid((std::string)SetValueText))
                        {
                            result.SetValue((std::string)SetValueText);
                            Out1 = result.GetValue();
                            Out2 = result.Out();
                        }
                        else
                        {
                            Out1 = Out2 = ST("The input is error!");
                        }
                    }

                    ImGui::InputText("Input", InputText, IM_ARRAYSIZE(InputText));
                    ImGui::SameLine();
                    if (ImGui::Button(ST("update2")))
                    {
                        if (result.IsValid((std::string)InputText))
                        {
                            result.Input((std::string)InputText);
                            Out1 = result.GetValue();
                            Out2 = result.Out();
                        }
                        else
                        {
                            Out1 = Out2 = ST("The input is error!");
                        }
                    }

                    ImGui::Text(ST("--Out--"));

                    ImGui::Text("GetValue: %s", Out1.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(Out1.c_str());
                    }

                    ImGui::Text("Out: %s", Out2.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy2"))
                    {
                        ImGui::SetClipboardText(Out2.c_str());
                    }
                    ImGui::End();
                }
                if (Show_unite_like_terms)
                {
                    ImGui::Begin(ST("Test Polynomial unite_like_terms"), &Show_unite_like_terms);
                    ImGui::Text(ST("--Input--"));

                    static char InputText1[128] = "a";
                    static Polynomial a;

                    ImGui::InputText(ST("Input the a"), InputText1, IM_ARRAYSIZE(InputText1));
                    if (ImGui::Button(ST("unite like terms")))
                    {
                        a.Input(InputText1);
                        a.Unite_like_terms();
                    }

                    ImGui::Text(ST("--Result--"));
                    ImGui::Text("result : %s", a.Out().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(a.Out().c_str());
                    }

                    ImGui::End();
                }
                if (Show_add)
                {
                    ImGui::Begin(ST("Test Polynomial add"), &Show_add);
                    ImGui::Text(ST("--Input--"));

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static Polynomial a;
                    static Polynomial b;
                    static Polynomial result;

                    ImGui::InputText(ST("Input the a"), InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText(ST("Input the b"), InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button(ST("result...")))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = a + b;
                    }

                    ImGui::Text(ST("--Out--"));

                    ImGui::Text("GetValue: %s", result.GetValue().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(result.GetValue().c_str());
                    }

                    ImGui::Text("Out: %s", result.Out().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy2"))
                    {
                        ImGui::SetClipboardText(result.Out().c_str());
                    }
                    ImGui::End();
                }
                if (Show_subtraction)
                {
                    ImGui::Begin(ST("Test Polynomial subtraction"), &Show_subtraction);
                    ImGui::Text(ST("--Input--"));

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static Polynomial a;
                    static Polynomial b;
                    static Polynomial result;

                    ImGui::InputText(ST("Input the a"), InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText(ST("Input the b"), InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button(ST("result...")))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = a - b;
                    }

                    ImGui::Text(ST("--Out--"));

                    ImGui::Text("GetValue: %s", result.GetValue().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(result.GetValue().c_str());
                    }

                    ImGui::Text("Out: %s", result.Out().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy2"))
                    {
                        ImGui::SetClipboardText(result.Out().c_str());
                    }
                    ImGui::End();
                }
                if (Show_MUL)
                {
                    ImGui::Begin(ST("Test Polynomial MUL"), &Show_MUL);
                    ImGui::Text(ST("--Input--"));

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static Polynomial a;
                    static Polynomial b;
                    static Polynomial result;

                    ImGui::InputText(ST("Input the a"), InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText(ST("Input the b"), InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button(ST("result...")))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = a * b;
                    }

                    ImGui::Text(ST("--Out--"));

                    ImGui::Text("GetValue: %s", result.GetValue().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(result.GetValue().c_str());
                    }

                    ImGui::Text("Out: %s", result.Out().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy2"))
                    {
                        ImGui::SetClipboardText(result.Out().c_str());
                    }
                    ImGui::End();
                }
                if (Show_division)
                {
                    ImGui::Begin(ST("Test Polynomial division"), &Show_division);
                    ImGui::Text(ST("--Input--"));

                    static char InputText1[128] = "a";
                    static char InputText2[128] = "b";
                    static Polynomial a;
                    static Polynomial b;
                    static Polynomial result;

                    ImGui::InputText(ST("Input the a"), InputText1, IM_ARRAYSIZE(InputText1));
                    ImGui::InputText(ST("Input the b"), InputText2, IM_ARRAYSIZE(InputText2));
                    if (ImGui::Button(ST("result...")))
                    {
                        a.Input(InputText1);
                        b.Input(InputText2);
                        result = a / b;
                    }

                    ImGui::Text(ST("--Out--"));

                    ImGui::Text("GetValue: %s", result.GetValue().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(result.GetValue().c_str());
                    }

                    ImGui::Text("Out: %s", result.Out().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy2"))
                    {
                        ImGui::SetClipboardText(result.Out().c_str());
                    }
                    ImGui::End();
                }
                if (Show_Substitute)
                {
                    ImGui::Begin(ST("Polynomial Substitute Test"), &Show_Substitute);
                    static char the_polynomial_text[128] = "x^2";
                    static char character_text[128] = "x";
                    static char substitute_character[128] = "a + b";
                    static Polynomial result("(1/1)x^(2/1)");
                    static Polynomial substitute_polynomial;
                    static sint8 character = 'x';
                    static std::string Out1 = "please check \"update\" ";

                    ImGui::Text(ST("please input the polynomial."));
                    ImGui::InputText(ST("polynomial"), the_polynomial_text, IM_ARRAYSIZE(the_polynomial_text));

                    ImGui::Text(ST("please input the character."));
                    ImGui::InputText(ST("character"), character_text, IM_ARRAYSIZE(character_text));

                    ImGui::Text(ST("please input the substitute polynomial."));
                    ImGui::InputText(ST("substitute polynomial"), substitute_character, IM_ARRAYSIZE(substitute_character));

                    if (ImGui::Button(ST("update")))
                    {

                        //如果满足多项式的条件
                        if (result.IsValid((std::string)the_polynomial_text))
                        {
                            result.Input((std::string)the_polynomial_text);
                        }
                        else
                        {
                            Out1 = ST("The input is error!");
                            goto Polynomial_Out_The_Result;
                        }

                        //如果代换字符输入有误，则报错
                        if (((std::string)character_text).size() != 1)
                        {
                            Out1 = "The input is error!";
                            goto Polynomial_Out_The_Result;
                        }

                        //输入代换字符
                        character = character_text[0];

                        //如果满足多项式的条件
                        if (substitute_polynomial.IsValid((std::string)substitute_character))
                        {
                            substitute_polynomial.Input((std::string)substitute_character);
                        }
                        else
                        {
                            Out1 = "The input is error!";
                            goto Polynomial_Out_The_Result;
                        }

                        //代换字符
                        if (!result.Substitute(character, substitute_polynomial))
                        {
                            Out1 = "The input is error!";
                            goto Polynomial_Out_The_Result;
                        }
                        Out1 = result.Out();
                    }
                    //跳转标签，显示结果
                Polynomial_Out_The_Result:

                    ImGui::Text(ST("--Out--"));
                    ImGui::Text("The result: %s", Out1.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(Out1.c_str());
                    }

                    ImGui::End();
                }

            }
            if (ImGui::CollapsingHeader(ST("Equation")))
            {
                //输入测试
                static bool Show_Input = false;
                static bool Show_Solution_equation = false;

                ImGui::Text(ST("This module is used to represent a Equation"));
                ImGui::Checkbox(ST("Input Test"), &Show_Input);
                ImGui::Checkbox(ST("Solution equation Test"), &Show_Solution_equation);

                if (Show_Input)
                {
                    ImGui::Begin(ST("Test Equation Input"), &Show_Input);
                    ImGui::Text(ST("--Input--"));

                    static char SetValueText[128] = "((1/1)a^(1/1))*X^(1/1)=((1/1))*X^(0/1)";
                    static char InputText[128] = "aX = 1";
                    static Equation result;
                    static std::string Out1, Out2;

                    ImGui::InputText("SetValue", SetValueText, IM_ARRAYSIZE(SetValueText));
                    ImGui::SameLine();
                    if (ImGui::Button(ST("update1")))
                    {
                        if (result.IsValid((std::string)SetValueText))
                        {
                            result.SetValue((std::string)SetValueText);
                            result.ShiftItem();
                            Out1 = result.GetValue();
                            Out2 = result.Out();
                        }
                        else
                        {
                            Out1 = Out2 = ST("the input is error!");
                        }
                    }

                    ImGui::InputText("Input", InputText, IM_ARRAYSIZE(InputText));
                    ImGui::SameLine();
                    if (ImGui::Button(ST("update2")))
                    {
                        if (result.IsValid((std::string)InputText))
                        {
                            result.Input((std::string)InputText);
                            result.ShiftItem();
                            Out1 = result.GetValue();
                            Out2 = result.Out();
                        }
                        else
                        {
                            Out1 = Out2 = ST("the input is error!");
                        }
                    }

                    ImGui::Text(ST("--Out--"));

                    ImGui::Text("GetValue: %s", Out1.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(Out1.c_str());
                    }

                    ImGui::Text("Out: %s", Out2.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy2"))
                    {
                        ImGui::SetClipboardText(Out2.c_str());
                    }

                    ImGui::End();
                }
                if (Show_Solution_equation)
                {
                    ImGui::Begin(ST("Test solution equation"), &Show_Solution_equation);
                    ImGui::Text(ST("--Input--"));

                    static char InputText1[128] = "aX^2 + bX + c = 0";
                    static std::string result_text1;
                    static std::string result_text2;
                    static Equation a;
                    
                    Fraction<Polynomial> result1;
                    std::vector<Fraction<Polynomial_Exponential_Sum>> result2;

                    ImGui::InputText(ST("Input the Equation"), InputText1, IM_ARRAYSIZE(InputText1));
                    if (ImGui::Button(ST("result...")))
                    {
                        //如果方程输入有效则赋值
                        if (a.IsValid(InputText1))
                        {
                            a.Input(InputText1);
                        }
                        //如果方程输入无效
                        else
                        {
                            result_text1 = result_text2 = ST("The input is error!");
                            goto out; //跳转到显示错误结果
                        }
                        if (a.GetTheHighestDegreeTermOfTheUnknown() == Fraction<sint64>(1, 1))
                        {
                            a.linear_equation_with_one_unknown();
                            result_text1 = Out(a.root1);
                            result_text2 = ST("There is only one result to this equation");
                        }
                        else if (a.GetTheHighestDegreeTermOfTheUnknown() == Fraction<sint64>(2, 1))
                        {
                            //如果该方程是纯数字方程
                            if (a.IsPurelyNumericalEquation())
                            {
                                if (a.NumberOfRoot() > 0)
                                {
                                    a.quadratic_equation_in_one_unknown();
                                    result_text1 = a.root2.at(0).Out();
                                    result_text2 = a.root2.at(1).Out();
                                }
                                else
                                {
                                    result_text1 = result_text2 = ST("This equation has no real roots");
                                }
                            }
                            else
                            {
                                a.quadratic_equation_in_one_unknown();
                                result_text1 = a.root2.at(0).Out();
                                result_text2 = a.root2.at(1).Out();
                            }
                        }
                        else
                        {
                            result_text1 = result_text2 =
                                ST("This equation cannot be solved using the SeekAnAnswer module");
                        }
                    }

                    out:
                    ImGui::Text(ST("--Result--"));
                    ImGui::Text("result 1 = %s", result_text1.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(result_text1.c_str());
                    }

                    ImGui::Text("result 2 = %s", result_text2.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy2"))
                    {
                        ImGui::SetClipboardText(result_text2.c_str());
                    }

                    ImGui::End();
                }

            }
            if (ImGui::CollapsingHeader(ST("Arithmetic")))
            {
                static bool show_arithmetic = false;
                ImGui::Text(ST("This module is used to calculate expression"));
                ImGui::Checkbox(ST("Arithmetic test"), &show_arithmetic);

                if (show_arithmetic)
                {
                    ImGui::Begin(ST("Arithmetic test"), &show_arithmetic);
                    ImGui::Text(ST("Please input your expression"));

                    static char InputText[128] = "(-1*2^(1/2)) + (2*2^(1/2))";
                    static std::string expression_out1, expression_out2;
                    Arithmetic_Expression expression;

                    ImGui::InputText(ST("input expression"), InputText, IM_ARRAYSIZE(InputText));
                    if (ImGui::Button(ST("result...")))
                    {
                        if (expression.IsValid((std::string)InputText))
                        {
                            expression.Input(InputText);
                            expression.Calculate();
                            expression_out1 = expression.Out();
                            expression_out2 = expression.Approximate_Out();
                        }
                        else
                        {
                            expression_out1 = expression_out2 = ST("The input is error!");
                        }
                    }

                    ImGui::Text(ST("--Result--"));

                    ImGui::Text("result = %s", expression_out1.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy1"))
                    {
                        ImGui::SetClipboardText(expression_out1.c_str());
                    }

                    ImGui::Text(((std::string)(char*)ST((std::string)"approximate result") + (std::string)" = %s").c_str(), expression_out2.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("copy2"))
                    {
                        ImGui::SetClipboardText(expression_out2.c_str());
                    }

                    ImGui::End();
                }
            }
            ImGui::End();
        }

        //Show language
        if (Show_Language_window)
        {
            ImGui::Begin(u8"Language / 语言", &Show_Language_window);
            ImGui::Text(u8"Please select your language / 请选择你的语言");

            const char* items[] = { u8"English", u8"简体中文"};
            ImGui::Combo(u8"Language / 语言", &language_text.language, items, IM_ARRAYSIZE(items));

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

inline const char* ST(std::string key)
{
    return language_text.ShowText(key);
}
