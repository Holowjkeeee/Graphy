#include "fruchterman_reingold.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace nodesoup
{


using std::vector;

FruchtermanReingold::FruchtermanReingold(const adj_list_t& aAdjacencyList, double aK) :
    m_AdjacencyList(aAdjacencyList),
    m_K(aK),
    m_KSquared(aK * aK),
    m_Temp(10 * sqrt(aAdjacencyList.size())),
    m_Movements(m_AdjacencyList.size()) 
{}



void FruchtermanReingold::Start(bool aStartCircle)
{
    m_Movements.resize(m_AdjacencyList.size());
    m_Positions.resize(m_AdjacencyList.size());

    m_CurrIter=0;
    m_MaxIter=0;

    m_StartCircle = aStartCircle;
}


void FruchtermanReingold::DoStep()
{
    ImVec2 zero = {0.0f, 0.0f};
    fill(m_Movements.begin(), m_Movements.end(), zero);

    // Repulsion force between vertice pairs
    for(vertex_id_t v_id = 0; v_id < m_AdjacencyList.size(); v_id++)
    {
        for(vertex_id_t other_id = v_id + 1; other_id < m_AdjacencyList.size(); other_id++)
        {
            if(v_id == other_id) continue;

            ImVec2 delta = m_Positions[v_id].Position - m_Positions[other_id].Position;
            double distance = norm(delta);
            // TODO: handle distance == 0.0

            // > 1000.0: not worth computing
            if(distance > 1000.0) continue;

            double repulsion = m_KSquared / distance;

            m_Movements[v_id]     += delta / distance * repulsion;
            m_Movements[other_id] -= delta / distance * repulsion;
        }

        // Attraction force between edges
        for(vertex_id_t adj_id : m_AdjacencyList[v_id])
        {
            if(adj_id > v_id) continue;

            ImVec2 delta = m_Positions[v_id].Position-m_Positions[adj_id].Position;
            float distance = norm(delta);
            if(distance==0.0f) continue;

            double attraction = distance * distance / m_K;

            m_Movements[v_id]   -= delta / distance * attraction;
            m_Movements[adj_id] += delta / distance * attraction;
        }
    }

    // Max movement capped by current temperature
    for(vertex_id_t v_id = 0; v_id < m_AdjacencyList.size(); v_id++)
    {
        if(!m_Positions[v_id].isFixed)
        {
            double movement_norm = norm(m_Movements[v_id]);
            // < 1.0: not worth computing
            if (movement_norm < 1.0) continue;

            double capped_mvmt_norm = std::min(movement_norm, m_Temp);
            ImVec2 capped_mvmt = m_Movements[v_id] / movement_norm * capped_mvmt_norm;

            m_Positions[v_id].Position = m_Positions[v_id].Position + capped_mvmt;
        }
    }

    // Cool down fast until we reach 1.5, then stay at low temperature
    if(m_Temp > 0.1) m_Temp *= 0.85;
    else m_Temp=0.1;
}


void FruchtermanReingold::Step(int aStepSize, int aMaxStep, vector<Vertex>& Vertexes)
{
    if(m_CurrIter >= aMaxStep && aMaxStep > 0)
    {
        for(int k = 0; k < Vertexes.size(); ++k)
        {
            Vertexes[k].isFixed  = m_Positions[k].isFixed;
            Vertexes[k].Position = m_Positions[k].Position;
        }
        return;
    }

    if (!m_CurrIter)
    {
        SetInitPositions();
    }

    for(int k = 0; k < aStepSize && m_CurrIter < m_MaxIter; ++k)
    {
        DoStep();
        m_CurrIter++;
    }


    if(m_CurrIter >= m_MaxIter)
    {
        m_CurrIter = 1;
        m_MaxIter++;

        for(int k = 0; k < Vertexes.size(); ++k)
        {
            Vertexes[k].isFixed  = m_Positions[k].isFixed;
            Vertexes[k].Position = m_Positions[k].Position;
        }
    }
}


void FruchtermanReingold::SetInitPositions()
{
    nodesoup::SetInitPositions(m_StartCircle, m_Positions);
}


void FruchtermanReingold::SetK(double aK) noexcept
{
    m_K = aK;
    m_KSquared = aK * aK;
}


void FruchtermanReingold::MovePos(vertex_id_t aVertexId, const ImVec2& aDisp, bool aRecalculate)
{
    // TODO: assert aVertexId en rango
    if(aRecalculate)
    {
        if((aDisp.x == INVALID_POSITION && aDisp.y == INVALID_POSITION) && 
            m_Positions[aVertexId].isFixed)
        {
            m_Positions[aVertexId].isFixed=false;
        }

        m_CurrIter=1;
        m_MaxIter=0;
        return;
    }

    m_Positions[aVertexId].Position += aDisp;
    m_Positions[aVertexId].isFixed = true;
}




}
