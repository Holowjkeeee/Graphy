#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>
#include <GLFW/glfw3.h>
#include <implot/implot.h>
#include <queue>
#include <iostream>
#include <string_view>
#include <random>
#include <string>
#include <math.h>
#include <stdio.h>


template <typename T>
class Optional {
private:
    T m_value;

public:
    bool IsValuePresented = false;
    void SetValue(T value) { IsValuePresented = true; m_value = value; }
    T GetValue() { return m_value; }
};



namespace Graphy 
{

long double getDistance(ImPlotPoint a, ImPlotPoint b)
{
    long double cathetusX = std::abs(a.x - b.x);
    long double cathetusY = std::abs(a.y - b.y);

    return std::sqrt(cathetusX * cathetusX + cathetusY * cathetusY);
}

using weight = long double;

class Vertex {
public:
    Vertex(){}
    Vertex(size_t id, ImPlotPoint position) : m_id(id), m_position(position){}

    size_t m_id;
    ImPlotPoint m_position;

    bool operator==(const Vertex& v) const { return m_position == v.m_position && m_id == v.m_id; }
};

class Edge
{
public:
    Edge(){}
    Edge(Vertex a, Vertex b, weight w) : m_a(a), m_b(b), m_weight(w){}

    Vertex m_a;
    Vertex m_b;
    weight m_weight;

    bool operator==(const Edge& e) const { return e.m_a == m_a && e.m_b == m_b && e.m_weight == m_weight; }
};

/// <summary>
/// Just a connected graph since the task requires exactly connected one
/// </summary>
class ConnectedGraph {
public:
    // Graph structure
    std::vector<Edge> m_adjacencyList;
    // Graph's MST
    std::vector<Edge> MST;
    // Verticies graph uses
    std::vector<Vertex> m_verticies;

    size_t getVerticiesQuantity() const
    {
        return m_verticies.size();
    }

    long double get_MST_cost()
    {
        long double output = 0;
        for (auto x : MST)
            output += x.m_weight;
        return output;
    }

    void addVertexNearest(ImPlotPoint point)
    {
        auto new_vertex = Vertex(m_verticies.size(), point);
        for (const auto& v : m_verticies)
        {
            weight ab_distance = getDistance(v.m_position, new_vertex.m_position);
            connectVerticies(v, new_vertex, ab_distance);
            connectVerticies(new_vertex, v, ab_distance);
        }
        m_verticies.emplace_back(new_vertex);

        if (m_verticies.size() == 3)
        {
            findMST();
        }
        else if (m_verticies.size() > 3)
        {
            Edge shortest = Edge(new_vertex, find_nearest_vertex(point), Graphy::getDistance(new_vertex.m_position, point));
            for (Edge edge : m_adjacencyList)
            {
                if (edge.m_a.m_id == shortest.m_a.m_id && edge.m_b.m_id == shortest.m_b.m_id)
                    MST.emplace_back(edge);
            }
        }
        else
        {
            MST.clear();
        }
    }

    Vertex find_nearest_vertex(ImPlotPoint point)
    {
        long double min = LONG_MAX;
        Vertex output;
        for (auto& item : m_verticies)
        {
            if (Graphy::getDistance(item.m_position, point) < min && item.m_position != point)
            {   
                min = Graphy::getDistance(item.m_position, point);
                output = item;
            }
        }
        return output;  
    }

    void addVertex(ImPlotPoint point)
    {
        auto new_vertex = Vertex( m_verticies.size(), point );
        for (const auto& v : m_verticies)
        {
            weight ab_distance = getDistance(v.m_position, new_vertex.m_position);
            connectVerticies(v, new_vertex, ab_distance);
            connectVerticies(new_vertex, v, ab_distance);
        }
        m_verticies.emplace_back(new_vertex);
        if (m_verticies.size() > 2)
        {
            findMST();
        }
        else
        {
            MST.clear();
        }
    }

    void connectVerticies(Vertex a, Vertex b, weight weight)
    {
        m_adjacencyList.emplace_back(a, b, weight);
    }

    void eraseEdge(Vertex a, Vertex b)
    {
        std::erase_if(m_adjacencyList,
            [&a, &b](Edge edge_value)
            { return edge_value.m_a == a && edge_value.m_b == b || edge_value.m_a == b && edge_value.m_b == a; }
        );
    };

    void eraseEdge(Edge const* edge)
    {
        std::erase_if(m_adjacencyList, [edge](Edge e) {return e == *edge; });
    }


    void eraseVertex(ImPlotPoint point)
    {
        auto found = *(std::ranges::find_if(m_verticies.begin(), m_verticies.end(),
            [&point](Vertex vertex) {return vertex.m_position == point; }));


        std::erase_if(m_adjacencyList, [&found](Edge edge)
            {return edge.m_a == found || edge.m_b == found; });

        std::erase(m_verticies, found);
        if (m_verticies.size() > 2)
        {
            findMST();
        }
        else
        {
            MST.clear();
        }
    }


    void findMST()
    {
        MST.clear();

        std::queue< Vertex > pq;

        // Create a vector for keys and initialize all
        // keys as infinite (INF)
        std::vector<int> key(m_verticies.size(), 999999);

        //TODO fix ids
        size_t max_id = 0;

        for (auto vertex : m_verticies)
        {
            if (vertex.m_id > max_id)
            {
                max_id = vertex.m_id;
            }
        }

        // Keep track of vertices included in MST
        std::vector<bool> inMST(max_id+1, false);

        // Insert source itself in priority queue and initialize its key as 0.
        pq.emplace(m_verticies.at(0));

        while (!pq.empty())
        {
            int u = pq.front().m_id;
            pq.pop();

            if (inMST.at(u) == true) {
                continue;
            }

            inMST.at(u) = true;

            Edge shortest_available;
            for (auto edge : m_adjacencyList)
            {
                if (edge.m_a.m_id == u && !inMST.at(edge.m_b.m_id)) pq.push(edge.m_b);
                if (edge.m_a.m_id == u && !inMST.at(edge.m_b.m_id) && edge.m_weight < key.at(u))
                {
                    key.at(u) = edge.m_weight;
                    shortest_available = edge;
                }
            }
            MST.emplace_back(shortest_available);
        }
    };
};


}


Graphy::ConnectedGraph graph = Graphy::ConnectedGraph();
static ImVector<ImPlotPoint> data;

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



Optional<ImPlotPoint> find_nearby_point(ImVector<ImPlotPoint>* vector, ImPlotPoint v)
{
    const double ALLOWED_MISTAKE = 0.1;
    Optional<ImPlotPoint> output;
    for (auto item : *vector)
    {
        if (Graphy::getDistance(item, v) <= ALLOWED_MISTAKE)
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


void drawGraph()
{
    for (auto x : graph.m_adjacencyList)
    {
        ImPlot::GetPlotDrawList()->AddLine(
            ImPlot::PlotToPixels(x.m_a.m_position),
            ImPlot::PlotToPixels(x.m_b.m_position),
            IM_COL32(128, 0, 255, 30), 0.2f
        );
    }
    if (!graph.MST.empty()) 
    {
        for (auto x : graph.MST)
        {
            ImPlot::GetPlotDrawList()->AddLine(
                ImPlot::PlotToPixels(x.m_a.m_position),
                ImPlot::PlotToPixels(x.m_b.m_position),
                IM_COL32(255, 255, 0, 255), 3.f
            );
        }
    }
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
                Optional<ImPlotPoint> found = find_nearby_point(&data, pt);
                if (!found.IsValuePresented)
                {
                    data.push_back(pt);
                    graph.addVertexNearest(pt);

                    if (data.Size > 1)
                        data.find_erase(ImPlotPoint(-2, -2)); // NOTE code smell
                }
            }

            else if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyShift) {
                ImPlotPoint pt = ImPlot::GetPlotMousePos();
                Optional<ImPlotPoint> found = find_nearby_point(&data, pt);
                if (found.IsValuePresented)
                {
                    data.find_erase(found.GetValue());
                    graph.eraseVertex(found.GetValue());

                    if (data.Size < 1)
                        data.push_back(ImPlotPoint(-2, -2)); // NOTE code smell
                }
            }

            ImPlot::PlotScatter("Points", &data[0].x, &data[0].y, data.Size,
                ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoFrame,
                0, 2 * sizeof(double));
                drawGraph();

            ImPlot::EndPlot();
        }

        ImGui::End();
    }

    if (ImGui::Begin("PlotSettings", nullptr))
    {
        if (auto cost = graph.get_MST_cost() != NULL) 
        {
            ImGui::Text("MST cost: %f", graph.get_MST_cost());
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