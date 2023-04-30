#pragma once
#include <vector>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"


// NodeSoup position?

/// <summary>
/// Vertex struct information
/// </summary>
struct Vertex
{
  ImVec2 Position;
  float  Radius;
  bool   isFixed;
};


constexpr float INVALID_POSITION = -1000000.0f;

inline double norm(const ImVec2& aImVec2) noexcept
{
  return sqrt(aImVec2.x * aImVec2.x + aImVec2.y * aImVec2.y);
}

namespace nodesoup
{

/// <summary>
/// Vertex id (UInt64)
/// </summary>
using vertex_id_t = std::size_t;

/// <summary>
/// Simple adjacency list structure
/// </summary>
using adj_list_t = std::vector<std::vector<vertex_id_t>>;


/// <summary>
/// Assigns diameters to vertices based on their degree
/// </summary>
/// <param name="aAdjList"></param>
/// <param name="Vertexes"></param>
/// <param name="aMinRadius"></param>
/// <param name="aK"></param>
void SetRadiuses(const adj_list_t& aAdjList, std::vector<Vertex>& Vertexes, float aMinRadius = 4.0f, float aK = 300.0f);


/// <summary>
/// Distribute vertices equally on a 1.0 radius circle (aCircleMode==true) or randomly in 
/// unit square (aCircleMode == false)
/// </summary>
/// <param name="CircleMode"></param>
/// <param name="Vertexes"></param>
void SetInitPositions(bool CircleMode,std::vector<Vertex>& Vertexes);

}

