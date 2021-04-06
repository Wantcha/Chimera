#include "cmpch.h"
#include "Chimera/Core/Core.h"
#include "ImGuiLayer.h"

#include "imgui.h"

#define IMGUI_IMPL_API
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include "Chimera/Core/Application.h"

#include <glad/glad.h>

#include "ImGuizmo.h"
#include "FontAwesome.h"

Chimera::ImGuiLayer::ImGuiLayer()
	:Layer("ImGuiLayer")
{
}

Chimera::ImGuiLayer::~ImGuiLayer()
{
}

void Chimera::ImGuiLayer::OnAttach()
{
    CM_PROFILE_FUNCTION();

    //Setup Dear ImGui context
    IMGUI_CHECKVERSION();
	ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //Enable keyboard controls
    //io.ConfigFlags |= ImGuiConfigFLags_NavEnableGamepad; //Enable gamepad controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; //Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; //Enable multi-viewport
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNotTaskBarIcons;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    static const ImWchar icons_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };

    io.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Bold.ttf", 15.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Bold.ttf", 20.0f);

    ImFontConfig config;
    config.MergeMode = true;
    io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Regular.ttf", 15.0f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/fontawesome-webfont.ttf", 12.0f, &config, icons_ranges);

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    SetDarkThemeColors();

    Application& app = Application::Get();
    GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void Chimera::ImGuiLayer::OnDetach()
{
    CM_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Chimera::ImGuiLayer::OnEvent(Event& e)
{
    if (m_BlockEvents)
    {
        ImGuiIO& io = ImGui::GetIO();
        e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
    
}

void Chimera::ImGuiLayer::OnImGuiRender()
{
    //static bool show = true;
    //ImGui::ShowDemoWindow(&show);
}

void Chimera::ImGuiLayer::Begin()
{
    CM_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void Chimera::ImGuiLayer::End()
{
    CM_PROFILE_FUNCTION();

    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

    //Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Chimera::ImGuiLayer::SetDarkThemeColors()
{
    auto& colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_WindowBg] = ImVec4{ 0.11f, 0.135, 0.135f, 1.0f };

    //Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.21f, 0.225, 0.235f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.31f, 0.33f, 0.335f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.16f, 0.17f, 0.185f, 1.0f };

    //Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.21f, 0.22f, 0.235f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.31f, 0.325, 0.33f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.16f, 0.17f, 0.17f, 1.0f };

    //Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.21f, 0.225f, 0.235f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.31f, 0.325f, 0.33f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.16f, 0.172f, 0.171f, 1.0f };

    //Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.16f, 0.17f, 0.18f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.39f, 0.40f, 0.40f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.32f, 0.33f, 0.335f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.16f, 0.17f, 0.175f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.21f, 0.225, 0.235f, 1.0f };

    //Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.16f, 0.1705f, 0.175f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.16f, 0.1705f, 0.175f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.16f, 0.18f, 0.16f, 1.0f };
}
