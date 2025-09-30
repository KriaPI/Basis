#pragma once
#include <cstddef>
#include <list>
#include <map>
#include <unordered_set>
#include <type_traits>

// TODO: consider overloading [] and [] = to retrieve and set the attribute values, respectively. 
// TODO: implement function stubs.
// TODO: keep a hash table with all vertex attributes, If an attribute has not been set for a vertex, then
// return a default attribute (or use a std::optional to mark that it has no attribute). Avoid Nulls. 

namespace Basis {
    enum class GraphType {
        directed,
        undirected
    };

    using VertexIndice = std::size_t;
    struct Edge {
        VertexIndice from;
        VertexIndice to;
    };

    template <GraphType typeOfGraph, typename attributes>
    class Graph {
        std::size_t vertexCount {0};
        std::size_t edgeCount {0};
        std::unordered_set<VertexIndice> vertices;
        std::unordered_map<VertexIndice, std::list<VertexIndice>> edges;
        
    public:
    

    void addVertex(VertexIndice vertex) {
        
    }

    /// @brief Add an edge from one vertex to another if the edge does 
    /// not already exist. The vertices are added if they do not already exist.
    /// @param edge Edge to add.  
    void addEdge(const Edge& edge) noexcept {
        // edge exists          => do nothing (the vertices must exist)
        // No vertices exist    => Add vertices and edge
        // from vertex exists   => Add "to" vertex and add edge 
        // to vertex exists     => Add "from" vertex and add edge
        // both exist           => Add edge

        auto from {edge.from};
        auto to   {edge.to};

        

        if (std::is_same(typeOfGraph, GraphType::directed)::value) {
            
        } else {

        }
    }

    /// @brief Remove the edge from the graph if it exists.  
    /// @param edge Edge to remove. 
    /// @return True if the edge existed and was removed, otherwise False. 
    bool removeEdge(Edge edge) noexcept {
        return false;
    }

    /// @brief Retrieve the number of vertices in the graph.
    /// @return The number of vertices.
    [[nodiscard]] std::size_t getVertexCount() const noexcept {
        return vertexCount;
    } 

    /// @brief Retrieve the number of vertices in the graph.
    /// @return The number of vertices.
    [[nodiscard]] std::size_t getEdgeCount() const noexcept {
        return edgeCount;
    } 

    /// @brief Check if the `vertex` exists in the graph. 
    /// @param vertex The vertex to check existance of.
    /// @return True if the vertex does exist, otherwise False. 
    [[nodiscard]] bool doesVertexExist(VertexIndice vertex) const noexcept {
        return false;
    }

    /// @brief Check if the `edge` exists in the graph. 
    /// @param edge The edge to check existance of.
    /// @return True if the edge does exist, otherwise False. 
    [[nodiscard]] bool doesEdgeExist(Edge edge) const noexcept {
        return false;
    }

    };
}