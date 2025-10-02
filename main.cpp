#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include <iostream>

const int OUTER_WIDTH = 532;
const int OUTER_HEIGHT = 522;
const int INNER_WIDTH = 530;
const int INNER_HEIGHT = 520;

bool isDragging = false;
float dragOffsetX = 0.0f;
float dragOffsetY = 0.0f;

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(OUTER_WIDTH, OUTER_HEIGHT, "Lunarys", nullptr, nullptr);
    if (!window) return -1;

    // --- Posiciona a janela centralizada + 50px pra direita ---
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    int centerX = (mode->width - OUTER_WIDTH) / 2 + 50; // 50 pixels pra direita
    int centerY = (mode->height - OUTER_HEIGHT) / 2;
    glfwSetWindowPos(window, centerX, centerY);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Variáveis para os widgets
    static bool cb1_window1 = false;
    static bool cb2_window2 = false;
    static bool cb3_window1 = false;
    static bool cb4_window2 = false;
    static float slider1_window1 = 0.0f;
    static float slider2_window2 = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- StartPos no centro da janela (agora só pra UI) ---
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        ImVec2 startPos = ImVec2((float)(fbWidth - INNER_WIDTH) * 0.5f, (float)(fbHeight - INNER_HEIGHT) * 0.5f);

        // Janela principal dark sem arredondamento usando startPos
        ImVec2 mainSize = ImVec2((float)INNER_WIDTH, (float)INNER_HEIGHT);
        ImVec2 mainPos = startPos;

        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

        // --- Glow externo roxo sem arredondamento ---
        ImU32 glowColor = IM_COL32(180, 0, 255, 200);
        for (int i = 0; i < 2; i++) {
            draw_list->AddRect(
                ImVec2(i, i),
                ImVec2((float)OUTER_WIDTH - i, (float)OUTER_HEIGHT - i),
                glowColor, 0.0f
            );
        }

        // --- Fundo da janela principal ---
        ImU32 bgColor = IM_COL32(20, 20, 20, 255);
        draw_list->AddRectFilled(mainPos, ImVec2(mainPos.x + mainSize.x, mainPos.y + mainSize.y), bgColor, 0.0f);

        // Janela ImGui invisível para interação
        ImGui::SetNextWindowPos(mainPos);
        ImGui::SetNextWindowSize(mainSize);
        ImGui::Begin("MainWindow", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBackground
        );

        // Barra de mover
        float barHeight = 30.0f;
        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::InvisibleButton("TitleBar", ImVec2(INNER_WIDTH, barHeight));
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            if (!isDragging) {
                int winX, winY;
                glfwGetWindowPos(window, &winX, &winY);
                dragOffsetX = (float)mouseX;
                dragOffsetY = (float)mouseY;
                isDragging = true;
            }
            int winX, winY;
            glfwGetWindowPos(window, &winX, &winY);
            int newX = (int)(mouseX - dragOffsetX + winX);
            int newY = (int)(mouseY - dragOffsetY + winY);
            glfwSetWindowPos(window, newX, newY);
        }
        else {
            isDragging = false;
        }

        // Logo e nome
        ImGui::SetCursorPos(ImVec2(10, 5));
        ImGui::TextColored(ImVec4(0.7f, 0.0f, 1.0f, 1.0f), "Lunarys");

        // --- Janelas internas ---
        float padding = mainSize.x * 0.02f;
        float borderThickness = 1.5f;
        float tabHeight = 20.0f;

        float innerWidth = (mainSize.x - 3 * padding) * 0.5f;
        float innerHeight = mainSize.y - barHeight - tabHeight - 2 * padding;

        ImVec2 innerPos1 = ImVec2(padding, barHeight + padding + tabHeight);
        ImVec2 innerPos2 = ImVec2(padding * 2 + innerWidth, barHeight + padding + tabHeight);

        ImU32 innerBorderColor = IM_COL32(100, 100, 100, 255);

        draw_list->AddRect(ImVec2(mainPos.x + innerPos1.x, mainPos.y + innerPos1.y),
            ImVec2(mainPos.x + innerPos1.x + innerWidth, mainPos.y + innerPos1.y + innerHeight),
            innerBorderColor, 0.0f, 0, borderThickness);

        draw_list->AddRect(ImVec2(mainPos.x + innerPos2.x, mainPos.y + innerPos2.y),
            ImVec2(mainPos.x + innerPos2.x + innerWidth, mainPos.y + innerPos2.y + innerHeight),
            innerBorderColor, 0.0f, 0, borderThickness);

        // --- Estilo roxo ---
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.7f, 0.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.0f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.0f, 0.35f, 1.0f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.3f, 0.0f, 0.4f, 1.0f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.7f, 0.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.0f, 1.0f, 1.0f);
        style.FrameRounding = 0.0f;
        style.ItemSpacing = ImVec2(8, 8);

        style.Colors[ImGuiCol_Tab] = ImVec4(0, 0, 0, 0);
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0, 0, 0, 0);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0, 0, 0, 0);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0, 0, 0, 0);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0, 0, 0, 0);
        style.Colors[ImGuiCol_Text] = ImVec4(0.7f, 0.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.7f, 0.0f, 1.0f, 1.0f);
        style.TabBorderSize = 1.5f;
        style.TabRounding = 0.0f;

        // --- Tabs ---
        ImGui::SetCursorPosY(barHeight + 5);
        if (ImGui::BeginTabBar("MainTabBar", ImGuiTabBarFlags_None)) {

            if (ImGui::BeginTabItem("TestTab1")) {
                ImVec2 tabMin = ImGui::GetItemRectMin();
                ImVec2 tabMax = ImGui::GetItemRectMax();
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                drawList->AddLine(ImVec2(tabMin.x, tabMax.y), ImVec2(tabMax.x, tabMax.y), IM_COL32(180, 0, 255, 255), 2.0f);

                ImGui::SetCursorPos(ImVec2(innerPos1.x + 5, innerPos1.y + 5));
                ImGui::BeginChild("Window1_Tab1", ImVec2(innerWidth - 10, innerHeight - 10), false, ImGuiWindowFlags_NoDecoration);
                ImGui::Checkbox("Checkbox 1", &cb1_window1);
                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(innerPos2.x + 5, innerPos2.y + 5));
                ImGui::BeginChild("Window2_Tab1", ImVec2(innerWidth - 10, innerHeight - 10), false, ImGuiWindowFlags_NoDecoration);
                ImGui::Checkbox("Checkbox 2", &cb2_window2);
                ImGui::EndChild();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("TestTab2")) {
                ImVec2 tabMin = ImGui::GetItemRectMin();
                ImVec2 tabMax = ImGui::GetItemRectMax();
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                drawList->AddLine(ImVec2(tabMin.x, tabMax.y), ImVec2(tabMax.x, tabMax.y), IM_COL32(180, 0, 255, 255), 2.0f);

                ImGui::SetCursorPos(ImVec2(innerPos1.x + 5, innerPos1.y + 5));
                ImGui::BeginChild("Window1_Tab2", ImVec2(innerWidth - 10, innerHeight - 10), false, ImGuiWindowFlags_NoDecoration);
                ImGui::SliderFloat("Slider 1", &slider1_window1, 0.0f, 100.0f, "%.1f");
                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(innerPos2.x + 5, innerPos2.y + 5));
                ImGui::BeginChild("Window2_Tab2", ImVec2(innerWidth - 10, innerHeight - 10), false, ImGuiWindowFlags_NoDecoration);
                ImGui::Checkbox("Checkbox 3", &cb4_window2);
                ImGui::EndChild();

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
