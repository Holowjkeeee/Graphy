#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>
#include <GLFW/glfw3.h>
#include <implot/implot.h>
#include <iostream>
#include <random>
#include <string>
#include <math.h>
#include <stdio.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

const char* initGlfwSettings()
{
    // glfw: initialize and configure
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        std::cout << "gltwInit() failed\n";
        exit(EXIT_FAILURE);
    }

    // Decide GL+GLSL versions
    const char* glsl_version;

#if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
        // GL 3.0 + GLSL 130
        glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    return glsl_version;
}

/// <summary>
/// glad: load all OpenGL function pointers
/// </summary>
void initGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        exit(EXIT_FAILURE);
    }
}

long double getDistance(ImPlotPoint a, ImPlotPoint b)
{
    long double cathetusX = std::abs(a.x - b.x);
    long double cathetusY = std::abs(a.y - b.y);

    return std::sqrt(cathetusX * cathetusX + cathetusY * cathetusY);
}

ImVector<ImPlotPoint> data;
double solutionCost;

template <typename T>
class Optional {
private:
    T m_value;

public:
    bool IsValuePresented = false;
    void SetValue(T value) { IsValuePresented = true; m_value = value; }
    T GetValue() { return m_value; }
};

Optional<ImPlotPoint> find_nearby_point(ImVector<ImPlotPoint>* vector, const ImPlotPoint* v)
{
    const double ALLOWED_MISTAKE = 0.1;
    Optional<ImPlotPoint> output;
    for (auto item : *vector)
    {
        if (getDistance(item, *v) <= ALLOWED_MISTAKE)
        {
            output.SetValue(item);
            break;
        }
    }
    return output;
}

void StylePlot() {
    ImPlotStyle& style = ImPlot::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImPlotCol_MarkerFill] = ImVec4(0.92f, 0.92f, 0.2f, 1.00f);
    colors[ImPlotCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
    colors[ImPlotCol_TitleText] = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);

    ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, LONG_MAX);
    ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, LONG_MAX);
    ImPlot::SetupAxesLimits(0, 10, 0, 10);

    ImPlot::GetStyle().MarkerSize = 6;
}

float GetWindowContentRegionHeight() 
{ 
    return ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y; 
}

double solve()
{
    std::minstd_rand rand;
    rand.seed(42);
    return rand();
}

void ShowUI() {
    if (data.Size < 1)
        data.push_back(ImPlotPoint(-2, -2)); // NOTE code smell

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    if (ImGui::Begin("Plot", nullptr))
    {
        if (ImPlot::BeginPlot("Graph", ImVec2(ImGui::GetWindowContentRegionWidth(), GetWindowContentRegionHeight())))
        {
            StylePlot();

            if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyCtrl) {
                ImPlotPoint pt = ImPlot::GetPlotMousePos();
                Optional<ImPlotPoint> found = find_nearby_point(&data, &pt);
                if (!found.IsValuePresented) 
                {
                    data.push_back(pt);

                    if (data.Size > 1)
                        data.find_erase(ImPlotPoint(-2, -2)); // NOTE code smell
                }
            }

            else if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyShift) {
                ImPlotPoint pt = ImPlot::GetPlotMousePos();
                Optional<ImPlotPoint> found = find_nearby_point(&data, &pt);
                if (found.IsValuePresented)
                {
                    data.find_erase(found.GetValue());

                    if (data.Size < 1)
                        data.push_back(ImPlotPoint(-2, -2)); // NOTE code smell
                }
            }

            ImPlot::PlotScatter("Points", &data[0].x, &data[0].y, data.Size,
                ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoFrame,
                0, 2 * sizeof(double));
            ImPlot::EndPlot();
        }

        ImGui::End();
    }

    if (ImGui::Begin("PlotSettings", nullptr))
    {
        bool isSolutionRequsted = ImGui::Button("Solve");

        if (isSolutionRequsted)
        {
            solutionCost = solve();
        }
        if (solutionCost != NULL) 
        {
            ImGui::Text("%f", solutionCost);
        }

        ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        if (ImGui::BeginTable("table1", 2, flags))
        {
            
            ImGui::TableSetupColumn("X");
            ImGui::TableSetupColumn("Y");
            ImGui::TableHeadersRow();

            for (auto item : data)
            {
                ImGui::TableNextRow();
                if (item.x > 0 && item.y > 0)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%f", item.x);
                    ImGui::TableNextColumn();
                    ImGui::Text("%f", item.y);
                }
            }
            ImGui::EndTable();
        }

        ImGui::End();
    }
}


int main() {

    const char* glsl_version = initGlfwSettings();

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(1300, 800, "Graphy", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window/n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    initGlad();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.ConfigViewportsNoAutoMerge = true;
    io.ConfigViewportsNoTaskBarIcon = true;
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform 
    // windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        //ImPlot::ShowDemoWindow();
        ShowUI();
        //ImGui::ShowDemoWindow();


        // Rendering
        ImGui::Render();
        int display_w;
        int display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we 
        // save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc)
        glfwSwapBuffers(window);
        glfwPollEvents();
        ImGui::EndFrame();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


/// <summary>
/// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
/// </summary>
/// <param name="window"></param>
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/// <summary>
/// glfw: whenever the window size changed (by OS or user) this callback function executes
/// </summary>
/// <param name="window"></param>
/// <param name="width"></param>
/// <param name="height"></param>
void framebuffer_size_callback([[maybe_unused]] GLFWwindow const* window, int width, int height)
{
    // tell OpenGL the size of the rendering window so OpenGL knows how we 
    // want to display the data and coordinates with respect to the window.
    glViewport(0, 0, width, height);
}