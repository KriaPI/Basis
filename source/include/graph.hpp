#pragma once
#include <cstddef>
#include <list>
#include <map>

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

    template <GraphType type, typename attributes>
    class Graph {
        std::size_t vertexCount {0};
        std::size_t edgeCount {0};
        std::map<VertexIndice, std::list<VertexIndice>> vertices;
    
    public:
    Graph();

    /// @brief Add an edge from one vertex to another if the edge does 
    /// not already exist. The vertices are added if they do not already exist.
    /// @param edge Edge to add.  
    void addEdge(const Edge& edge) noexcept {
        return;        
    }

    /// @brief Remove the edge from the graph if it exists.  
    /// @param edge Edge to remove. 
    /// @return True if the edge existed and was removed, otherwise False. 
    bool removeEdge(Edge edge) noexcept {

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

    /// @brief Associate an attribute with a vertex. If the vertex is already associated with an attribute,
    /// then the associated attribute will be overwritten by the new attribute `values`. 
    /// @param values Any object.
    /// @param vertex 
    void associateAttributes(attributes values, std::size_t vertex) {

    }

    /// @brief Associate all existing vertices with the object `values`. Note that each vertex will be associated with its own
    /// copy of `values`. Any attribute already associated with some vertex will be overwritten by this. 
    /// @param values Any object.
    void associateAttributesToAll(attributes values) {

    }
    
    /// TODO: add logic for case when no attribute is associated with the vertex. Maybe use std::optional?  
    /// @brief Retrieve the attribute value(s) associated with the vertices if there are any.
    /// @param vertex 
    /// @return A reference to the attribute value(s) associated with the vertex. 
    const attributes& getAssociatedAttribute(VertexIndice vertex) {

    }

    };
    
}