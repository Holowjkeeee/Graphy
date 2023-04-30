#include "nodesoup.hpp"
#include <random>

namespace nodesoup
{


void SetRadiuses(const adj_list_t& aAdjList, std::vector<Vertex>& Vertexes, float aMinRadius, float aK)
{
	assert(Vertexes.size() == aAdjList.size());

	for (vertex_id_t v_id = 0; v_id < aAdjList.size(); v_id++)
	{
		float delta = log2f(aK * aAdjList[v_id].size() / aAdjList.size());
		float radius = aMinRadius + 2.0f * std::max(0.0f, delta);
		Vertexes[v_id].Radius = radius;
	}
}


void SetInitPositions(bool CircleMode, std::vector<Vertex>& Vertexes)
{
	constexpr float PI = 3.14159265358979323846f;

	if(CircleMode)
	{
		float angle = 2.0f * PI / Vertexes.size();

		for (vertex_id_t v_id = 0; v_id<Vertexes.size(); v_id++)
		{
			Vertexes[v_id].Position.x = cosf(v_id * angle);
			Vertexes[v_id].Position.y = sinf(v_id * angle);
			Vertexes[v_id].isFixed    = false;
		}
	}
	else
	{
		std::minstd_rand rand;
		rand.seed(42);

		for (Vertex vertex : Vertexes)
		{
			vertex.Position.x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
			vertex.Position.y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
			vertex.isFixed    = false;
		}
	}
}


}
