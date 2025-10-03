#pragma once
#include <cstddef>
#include <algorithm>
#include <list>
#include <map>
#include <unordered_set>
#include <type_traits>

// TODO: consider overloading [] and [] = to retrieve and set the attribute values, respectively. 
// TODO: keep a hash table with all vertex attributes, If an attribute has not been set for a vertex, then
// return a default attribute (or use a std::optional to mark that it has no attribute). Avoid Nulls. 
// TODO: refactor so that we do not need to assume that the graph is directed.


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
            const auto didInsert {vertices.insert(vertex).second};
            if (didInsert) ++vertexCount;
        }

        /// @brief Add an edge from one vertex to another if the edge does 
        /// not already exist. The vertices are added if they do not already exist.
        /// @param edge Edge to add.  
        void addEdge(const Edge& edge) noexcept {
            const auto from {edge.from};
            const auto to   {edge.to};

            if (doesEdgeExist(edge)) return;

            if (doesVertexExist(from)) {
                (*edges.find(edge.from)).second.emplace_back(to);
            } else {
                edges.insert({from, {to}});
                addVertex(from);
            }
            addVertex(to);
            ++edgeCount;
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
            return vertices.contains(vertex);
        }

        /// @brief Check if the `edge` exists in the graph. 
        /// @param edge The edge to check existance of.
        /// @return True if the edge does exist, otherwise False. 
        [[nodiscard]] bool doesEdgeExist(Edge edge) const noexcept {
            const auto adjacentVertices {edges.find(edge.from)};
            if (adjacentVertices != edges.cend()) {
                const auto& adjacencyList {(*adjacentVertices).second};

                return std::find(adjacencyList.begin(), adjacencyList.end(), edge.to) != adjacencyList.end();
            } else {
                return false;
            }
        }

    };
}