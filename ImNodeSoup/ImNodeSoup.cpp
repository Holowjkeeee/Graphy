#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <ImNodeSoup/nodesoup.hpp>
#include <ImNodeSoup/fruchterman_reingold.hpp>
#include <Graphy/presets.hpp>


static ImVec2 graphDisplay {0.0f, 0.0f};
static float  graphScale = 1.0f;


/// <summary>
/// Read simple dot files
/// </summary>
/// <param name="aDotData"></param>
/// <returns></returns>
nodesoup::adj_list_t read_from_dot(const char* aDotData)
{
    nodesoup::adj_list_t adjacent_list;

    std::istringstream ifs(aDotData);
    if (!ifs.good()) return adjacent_list;


    std::unordered_map<std::string, nodesoup::vertex_id_t> names;

    auto name_to_vertex_id = [&adjacent_list,&names](std::string name)
    {
        if(name[name.size() - 1] == ';')
        {
            name.erase(name.end() - 1, name.end());
        }

        nodesoup::vertex_id_t v_id;
        auto it = names.find(name);
        if (it != names.end())
        {
            return (*it).second;
        }

        v_id = adjacent_list.size();
        names.insert({ name, v_id });
        adjacent_list.resize(v_id + 1);
        return v_id;
    };

    std::string line;
    // skip first line
    std::getline(ifs, line);

    while (std::getline(ifs, line))
    {
        if (line[0] == '}') break;

        std::istringstream iss(line);
        std::string name;
        std::string edge_sign;
        std::string adj_name;
        iss >> name >> edge_sign >> adj_name;

        // add vertex if new
        nodesoup::vertex_id_t v_id = name_to_vertex_id(name);

        assert(edge_sign == "--" || edge_sign.size() == 0);
        if (edge_sign != "--") continue;

        // add adjacent vertex if new
        nodesoup::vertex_id_t adj_id = name_to_vertex_id(adj_name);

        // add edge if new
        if (find(adjacent_list[v_id].begin(),adjacent_list[v_id].end(), adj_id) == adjacent_list[v_id].end())
        {
            adjacent_list[v_id].push_back(adj_id);
            adjacent_list[adj_id].push_back(v_id);
        }
    }

    return adjacent_list;
}

/// <summary>
/// Get square distance between positions
/// </summary>
static inline float squaredDistance(const ImVec2& a, const ImVec2& b) noexcept
{
  ImVec2 delta = a - b;
  return delta.x * delta.x + delta.y * delta.y;
}


static ImVec2 GetStartPos() noexcept
{
  ImGuiWindow const* window = ImGui::GetCurrentWindowRead();
  return window->Pos + graphDisplay;
}


static void DrawData(
    const nodesoup::adj_list_t& aAdjList,
    const std::vector<Vertex>& aPositions,
    bool isDebugEnabled
)
{
    ImGuiWindow const* window = ImGui::GetCurrentWindow();
    ImGuiIO const& io = ImGui::GetIO();

    // TODO https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-how-can-i-tell-whether-to-dispatch-mousekeyboard-to-dear-imgui-or-my-application

    // Adjust graph position on screen
    if (io.MouseDownDuration[1] > 0 &&             // if mouse left click
        window->InnerClipRect.Contains(io.MousePos)) // and the mouse pointer still on window
        graphDisplay += io.MouseDelta;
  
    // Adjust scale
    if (window->InnerClipRect.Contains(io.MousePos)) {
        if(io.MouseWheel > 0) graphScale *= 1.1f;
            else if(io.MouseWheel < 0) graphScale *= 0.9f;
    }
    
     
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 cursor_pos = GetStartPos();

    ImVec2 origin(window->Size.x / 2.0, window->Size.y / 2.0);

    const ImU32 node_col     = ImGui::GetColorU32(ImGuiCol_ScrollbarGrabActive);
    const ImU32 node_fix_col = ImGui::GetColorU32(ImGuiCol_NavHighlight);
    const ImU32 arc_col      = ImGui::GetColorU32(ImGuiCol_ScrollbarGrab);
    const ImU32 txt_col      = ImGui::GetColorU32(ImGuiCol_Text);


    for(nodesoup::vertex_id_t v_id = 0; v_id < aAdjList.size(); v_id++)
    {
        const Vertex& currentPosition = aPositions[v_id];
        ImVec2 v_pos = currentPosition.Position * graphScale + origin;

        for(auto adj_id : aAdjList[v_id])
        {
            if(adj_id < v_id)continue;

            ImVec2 adj_pos = aPositions[adj_id].Position * graphScale + origin;
            draw_list -> AddLine(cursor_pos + v_pos, cursor_pos + adj_pos,arc_col, 5.F);
        }


        draw_list -> AddCircleFilled(cursor_pos + ImVec2(v_pos.x, v_pos.y), currentPosition.Radius, node_col);

        if(isDebugEnabled)
        {
            // Graph info
            draw_list->
                AddText(cursor_pos+ImVec2(v_pos.x,v_pos.y),       txt_col, std::to_string(v_id)   .c_str());
            draw_list->
                AddText(cursor_pos+ImVec2(v_pos.x,v_pos.y+20.0f), txt_col, std::to_string(v_pos.x).c_str());
            draw_list->
                AddText(cursor_pos+ImVec2(v_pos.x,v_pos.y+40.0f), txt_col, std::to_string(v_pos.y).c_str());
        
            // Canvas info
            ImGuiContext const& g = *GImGui;
            ImGui::SetCursorPos({ 20.0f,window->InnerClipRect.GetHeight() - g.FontSize - 20 });
            ImGui::Text("x:%.3f  y:%.3f  scale:%.3f", graphDisplay.x, graphDisplay.y, graphScale);
        }
    }

    
}


static void solve() 
{
    
}

/// <summary>
/// Show graph canvas and settings pane for it
/// </summary>
void ShowNodeSoup()
{
    static std::vector<Vertex> positions; // vertexes' positions
    static std::vector<float> radiuses;   // vertexes' radiuses

    float k = 15.0; // a constant used to compute attractive and repulsive forces between vertexes

    static nodesoup::adj_list_t adjacency_list;
    static nodesoup::FruchtermanReingold fr(adjacency_list, k);

    constexpr int kCircle = 0;
    static int init_mode = kCircle;

    static bool draw_debug = false;

    const char* items[] = { "None", "example", "K6", "K6-2", "Small dense", "Bin tree", "Quad tree" };
    const char* items_data[] = { "", example, k6_dot, k6_2_dot, small_dense_dot, bin_tree_dot, quad_tree_dot };
    static int item_current = 0;
    bool isDataChanged = false;
    bool isSolvePressed = false;


    if (ImGui::Begin("NodeSoupSettings", nullptr))
    {
        isDataChanged = ImGui::Combo("Data", &item_current, items, IM_ARRAYSIZE(items));
        ImGui::Checkbox("Show debug info", &draw_debug);

        isSolvePressed = ImGui::Button("Solve");

        ImGui::End();
    }

    if (ImGui::Begin("NodeSoup", nullptr))
    {
        if(draw_debug)
        {
            ImGui::Text("Energy: %.3f",static_cast<float>(fr.GetEnergy()));
        }
    
        if(isDataChanged)
        {
            adjacency_list = read_from_dot(items_data[item_current]);
            positions.resize(adjacency_list.size());
            nodesoup::SetRadiuses(adjacency_list,positions);
            fr.Start(init_mode == kCircle);
        }

        if(!adjacency_list.empty())
        {
            fr.Step(15, 0, positions);
        }

        DrawData(adjacency_list, positions, draw_debug);

        if (isSolvePressed)
        {
            solve();
        }

        ImGui::End();
    }
}