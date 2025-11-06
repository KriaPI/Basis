#pragma once
#include <algorithm>
#include <cstddef>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <utility>

// TODO: implement edge attributes
// TODO: implement breadth-first search and depth-first search


namespace Basis {
    using VertexIndice = std::size_t;
    
    enum class GraphType {
        directed,
        undirected
    };

    struct Edge {
        VertexIndice from;
        VertexIndice to;
    };

    template <typename Attribute>
    struct EdgeEndWithAttribute {
        VertexIndice to;
        Attribute attribute;
    };

    /// @brief Retrieve the reverse of `edge`. 
    Edge getEdgeReversal(const Edge& edge) {
        return Edge{.from = edge.to, .to = edge.from};
    }

    template <typename VertexAttribute = int, typename EdgeAttribute = int>
    class GraphBase {
        private:

        std::size_t vertexCount {0};
        std::size_t edgeCount {0};
        std::unordered_set<VertexIndice> vertices;
        std::unordered_map<VertexIndice, VertexAttribute> vertexAttributes;
        std::unordered_map<VertexIndice, std::list<VertexIndice>> edges;
    
        public:
        void addVertex(VertexIndice vertex) {
            const auto didInsert {vertices.insert(vertex).second};
            if (didInsert) {++vertexCount;}
        }

        /// @brief Add an edge from one vertex to another if the edge does 
        /// not already exist. The vertices are added if they do not already exist.
        /// @param edge Edge to add.  
        void addEdge(const Edge& edge) noexcept {
            const auto from {edge.from};
            const auto to   {edge.to};

            if (doesEdgeExist(edge)) {return;}

            auto fromVertex {edges.find(edge.from)};

            if (fromVertex != edges.end()) {
                (*edges.find(edge.from)).second.emplace_back(to);
            } else {
                edges.insert({from, std::list{to}});
                addVertex(from);
            }
            addVertex(to);
            ++edgeCount;
        }

        /// @brief Remove the edge from the graph if it exists.  
        /// @param edge Edge to remove. 
        /// @return True if the edge existed and was removed, otherwise False. 
        bool removeEdge(const Edge& edge) noexcept {
            if (doesEdgeExist(edge)) {
                (*edges.find(edge.from)).second.remove(edge.to);
                edgeCount--;
                return true;
            } else {
                return false;
            }
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
        [[nodiscard]] bool doesEdgeExist(const Edge& edge) const noexcept {
            const auto adjacentVertices {edges.find(edge.from)};
            if (adjacentVertices != edges.cend()) {
                const auto& adjacencyList {(*adjacentVertices).second};

                return std::ranges::find(adjacencyList, edge.to) != adjacencyList.end();
            } else {
                return false;
            }
        }

        [[nodiscard]] const auto& getVertexAttribute(const VertexIndice vertex) const {
            return vertexAttributes.at(vertex);
        }

        template <typename T>
        void setVertexAttribute(const VertexIndice vertex, T&& value) {
            vertexAttributes.insert_or_assign(vertex, std::forward<T>(value));
        }
    };


    template <GraphType TypeOfGraph, typename VertexAttributes = int, typename EdgeAttributes = int>
    class Graph: public GraphBase<VertexAttributes, EdgeAttributes> {
        public:
        [[nodiscard]] bool is_directed() const noexcept {
            return true;
        }
    };

    template <typename VertexAttributes>
    class Graph<GraphType::undirected, VertexAttributes>: public GraphBase<VertexAttributes> {
        public:
        [[nodiscard]] bool is_directed() const noexcept {
            return false;
        }

        /// @brief Retrieve the number of vertices in the graph.
        /// @return The number of vertices.
        [[nodiscard]] std::size_t getEdgeCount() const noexcept {
            return GraphBase<VertexAttributes>::getEdgeCount() / 2;
        } 

        /// @brief Add an edge between two vertices if it does not already exist.
        /// The vertices are added if they do not already exist.
        /// @param edge Edge to add.  
        void addEdge(const Edge& edge) noexcept {
            auto reverseEdge {getEdgeReversal(edge)};
            GraphBase<VertexAttributes>::addEdge(edge);
            GraphBase<VertexAttributes>::addEdge(reverseEdge);
        }

        /// @brief Remove the edge from the graph if it exists.  
        /// @param edge Edge to remove. 
        /// @return True if the edge existed and was removed, otherwise False. 
        bool removeEdge(const Edge& edge) noexcept {
            auto reverseEdge {getEdgeReversal(edge)};
            return GraphBase<VertexAttributes>::removeEdge(edge) && GraphBase<VertexAttributes>::removeEdge(reverseEdge);
        }
    };
}