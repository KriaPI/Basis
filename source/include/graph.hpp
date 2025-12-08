#pragma once
#include <algorithm>
#include <cstddef>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <format>
#include <queue>
#include <stack>

// TODO: de-duplicate bfs and dfs code
// TODO: think about if custom iterators are a good idea to implement

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

    Edge getEdgeReversal(const Edge& edge) {
        return Edge{.from = edge.to, .to = edge.from};
    }

    template <typename VertexAttribute = int, typename EdgeAttribute = char>
    class GraphBase {
        private:
        using EdgeEnd = EdgeEndWithAttribute<EdgeAttribute>;

        std::size_t vertexCount {0};
        std::size_t edgeCount {0};
        std::unordered_set<VertexIndice> vertices;
        std::unordered_map<VertexIndice, VertexAttribute> vertexAttributes;
        std::unordered_map<VertexIndice, std::list<EdgeEnd>> edges;
    
        public:
        /// @brief Add a vertex to a the graph.
        /// @param vertex The vertex to add. 
        void addVertex(VertexIndice vertex) {
            const auto didInsert {vertices.insert(vertex).second};
            if (didInsert) {++vertexCount;}
        }

        /// @brief Add an edge from one vertex to another if the edge does 
        /// not already exist. The vertices are added if they do not already exist.
        /// @param edge Edge to add.  
        void addEdge(const Edge& edge) noexcept {
            if (doesEdgeExist(edge)) {return;}

            const auto from {edge.from};
            const auto to   {edge.to};
            auto fromVertex {edges.find(edge.from)};

            if (fromVertex != edges.end()) {
                (*edges.find(edge.from)).second.emplace_back(EdgeEnd {.to = to, .attribute = {}});
            } else {
                edges.insert({from, std::list{EdgeEnd {.to = to, .attribute = {}}} });
                addVertex(from);
            }
            addVertex(to);
            ++edgeCount;
        }

        /// @brief Add an edge from one vertex to another if the edge does 
        /// not already exist. The vertices are added if they do not already exist.
        /// @param edge Edge to add. 
        /// @param attributeValue value to set for the attribute of `edge`.
        template <typename T> 
        void addEdge(const Edge& edge, T&& attributeValue) noexcept {
            addEdge(edge);
            setEdgeAttribute(edge, std::forward<T>(attributeValue));
        }

        /// @brief Remove the edge from the graph if it exists.  
        /// @param edge Edge to remove. 
        /// @return True if the edge existed and was removed, otherwise False. 
        bool removeEdge(const Edge& edge) noexcept {
            if (doesEdgeExist(edge)) {
                std::list<EdgeEnd>& adjacencyList {(*edges.find(edge.from)).second};
                adjacencyList.remove_if([&edge] (EdgeEnd& edgeEnd) {return edgeEnd.to == edge.to;});
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

                return std::ranges::find(adjacencyList, edge.to, [] (const EdgeEnd& edgeEnd) {return edgeEnd.to;}) != adjacencyList.end();
            } else {
                return false;
            }
        }

        /// @brief Set or update an attribute value of a vertex.
        /// @param vertex Vertex whose attribute should be set or updated.
        /// @param value Value to set or update to. 
        template <typename T>
        void setVertexAttribute(const VertexIndice vertex, T&& value) {
            vertexAttributes.insert_or_assign(vertex, std::forward<T>(value));
        }

        /// @brief Retrieve the attribute value of a vertex. 
        /// @param vertex Index of vertex to retrieve attribute value of. 
        /// @return The attribute value of a `vertex`.
        /// @throws `std::out_of_range` if no attribute value has not been set for `vertex` or if `vertex` does not exist. 
        [[nodiscard]] const auto& getVertexAttribute(const VertexIndice vertex) const {
            return vertexAttributes.at(vertex);
        }

        /// @brief Set or update the attribute value of an edge. 
        /// @param edge The edge whose attribute value should be set or updated.
        /// @param value Value to set or update to.
        /// @note Edge attribute values are already default-initialized.
        /// @throws `std::out_of_range` if `edge` does not exist in the graph. 
        template <typename T>
        void setEdgeAttribute(const Edge& edge, T&& value) {
            if (!doesEdgeExist(edge)) {
                throw std::out_of_range(std::format("Edge ({}, {}) does not exist.", edge.from, edge.to));
            }
            
            const auto adjacentVertices {edges.find(edge.from)};
            auto& adjacencyList {(*adjacentVertices).second};
            const auto edgeEnd {std::ranges::find(adjacencyList, edge.to, [] (EdgeEnd& edgeEnd) {return edgeEnd.to;})};
            edgeEnd->attribute = std::forward<T>(value);
        }

        /// @brief Retrieve the attribute value of an edge. 
        /// @param edge Edge whose value should be retrieved.
        /// @return The attribute value of `edge`.
        /// @throws `std::out_of_range` if `edge` does not exist in the graph. 
        [[nodiscard]] const auto& getEdgeAttribute(const Edge& edge) const {
            if (!doesEdgeExist(edge)) {
                throw std::out_of_range(std::format("Edge ({}, {}) does not exist.", edge.from, edge.to));
            }

            const auto adjacentVertices {edges.find(edge.from)};
            const auto& adjacencyList {(*adjacentVertices).second};
            const auto& edgeEnd {std::ranges::find(adjacencyList, edge.to, [] (const EdgeEnd& edgeEnd) {return edgeEnd.to;})};
            return edgeEnd->attribute;
        }

        /// @brief Retrieve a const reference to the list containing neighboring vertices (along with their attributes). 
        /// @param vertex A vertex within the graph.
        /// @return The neighbors of `vertex`.
        /// @throws `std::out_of_range` if `vertex` does not exist in the graph.
        [[nodiscard]] const auto& getNeighbors(const VertexIndice vertex) {
            if (!doesVertexExist(vertex)) {
                throw std::out_of_range(std::format("No vertex with index {} exists.", vertex));
            } else {
                return edges[vertex];
            }
        }

        /// @brief Get the order that vertices are discovered in a breadth-first search.
        /// @param source Vertex to start search from. 
        /// @return The order of a breadth-first search starting from `source`.
        /// @throws `std::out_of_range` if `source` does not exist in the graph.
        [[nodiscard]] const std::list<VertexIndice> getBreadthFirstOrder(VertexIndice source) {
            if (!doesVertexExist(source)) {
                throw std::out_of_range(std::format("No vertex with index {} exists.", source));
            }

            std::list<VertexIndice> discoveryOrder;
            discoveryOrder.emplace_back(source);
            std::unordered_set<VertexIndice> discovered {source}; 
            std::queue<VertexIndice> toVisit;
            toVisit.push(source);

            while (!toVisit.empty()) {
                auto current {toVisit.front()};
                auto neighbors {getNeighbors(current)};
                
                if (!discovered.contains(current)) {
                    discovered.insert(current);
                    discoveryOrder.emplace_back(current);
                }

                for (EdgeEnd& neighborWithAttribute: neighbors) {
                    auto neighbor {neighborWithAttribute.to};
                    if (!discovered.contains(neighbor)) {
                        toVisit.push(neighbor);
                        
                    }
                }
                toVisit.pop();
            }

            return discoveryOrder;
        }

        /// @brief Get the order that vertices are discovered in a depth-first search.
        /// @param source Vertex to start search from. 
        /// @return The order of a depth-first search starting from `source`.
        /// @throws `std::out_of_range` if `source` does not exist in the graph.
        [[nodiscard]] const std::list<VertexIndice> getDepthFirstOrder(VertexIndice source) {
            if (!doesVertexExist(source)) {
                throw std::out_of_range(std::format("No vertex with index {} exists.", source));
            }

            std::list<VertexIndice> discoveryOrder;
            discoveryOrder.emplace_back(source);
            std::unordered_set<VertexIndice> discovered {source}; 
            std::stack<VertexIndice> toVisit;
            toVisit.push(source);

            while (!toVisit.empty()) {
                auto current {toVisit.top()};
                toVisit.pop();
                auto neighbors {getNeighbors(current)};
                
                if (!discovered.contains(current)) {
                    discovered.insert(current);
                    discoveryOrder.emplace_back(current);
                }

                for (EdgeEnd& neighborWithAttribute: neighbors) {
                    auto neighbor {neighborWithAttribute.to};
                    if (!discovered.contains(neighbor)) {
                        toVisit.push(neighbor);
                    }
                }
            }

            return discoveryOrder;
        }

    };

    /// @brief A type generic graph class with edge and vertex attributes. 
    /// @tparam TypeOfGraph Specify whether the graph should behave as an undirected graph or a directed graph. 
    /// @tparam VertexAttributes Attribute type for vertices.
    /// @tparam EdgeAttributes Attribute type for edges. 
    /// @note Edges always have an initialized edge attribute of 
    /// the type `EdgeAttributes`. Consider using a pointer to the attribute type if the type is expensive (memory-wise).
    template <GraphType TypeOfGraph, typename VertexAttributes = int, typename EdgeAttributes = char>
    class Graph: public GraphBase<VertexAttributes, EdgeAttributes> {
        public:
        [[nodiscard]] bool is_directed() const noexcept {
            return true;
        }
    };


    /// @brief A type generic graph class with edge and vertex attributes. 
    /// @tparam TypeOfGraph Specify whether the graph should behave as an undirected graph or a directed graph. 
    /// @tparam VertexAttributes Attribute type for vertices.
    /// @tparam EdgeAttributes Attribute type for edges. 
    /// @note Edges always have an initialized edge attribute of 
    /// the type `EdgeAttributes`. Consider using a pointer to the attribute type if the type is expensive (memory-wise).
    template <typename VertexAttributes, typename  EdgeAttributes>
    class Graph<GraphType::undirected, VertexAttributes, EdgeAttributes>: public GraphBase<VertexAttributes, EdgeAttributes> {
        public:
        [[nodiscard]] bool is_directed() const noexcept {
            return false;
        }

        /// @brief Retrieve the number of vertices in the graph.
        /// @return The number of vertices.
        [[nodiscard]] std::size_t getEdgeCount() const noexcept {
            return GraphBase<>::getEdgeCount() / 2;
        } 

        /// @brief Add an edge between two vertices if it does not already exist.
        /// The vertices are added if they do not already exist.
        /// @param edge Edge to add.  
        void addEdge(const Edge& edge) noexcept {
            auto reverseEdge {getEdgeReversal(edge)};
            GraphBase<VertexAttributes, EdgeAttributes>::addEdge(edge);
            GraphBase<VertexAttributes, EdgeAttributes>::addEdge(reverseEdge);
        }

        /// @brief Add an edge from one vertex to another if the edge does 
        /// not already exist. The vertices are added if they do not already exist.
        /// @param edge Edge to add. 
        /// @param attributeValue value to set for the attribute of `edge`.
        template <typename T> 
        void addEdge(const Edge& edge, T&& attributeValue) noexcept {
            GraphBase<VertexAttributes, EdgeAttributes>::addEdge(edge, attributeValue);
            GraphBase<VertexAttributes, EdgeAttributes>::addEdge(getEdgeReversal(edge), std::forward<T>(attributeValue));
        }

        /// @brief Remove the edge from the graph if it exists.  
        /// @param edge Edge to remove. 
        /// @return True if the edge existed and was removed, otherwise False. 
        bool removeEdge(const Edge& edge) noexcept {
            auto reverseEdge {getEdgeReversal(edge)};
            return GraphBase<>::removeEdge(edge) && GraphBase<>::removeEdge(reverseEdge);
        }

        /// @brief Set or update the attribute value of an edge. 
        /// @param edge The edge whose attribute value should be set or updated.
        /// @param value Value to set or update to.
        /// @note Edge attribute values are already default-initialized.
        /// @throws `std::out_of_range` if `edge` does not exist in the graph. 
        template <typename T>
        void setEdgeAttribute(const Edge& edge, T&& value) {
            GraphBase<VertexAttributes, EdgeAttributes>::setEdgeAttribute(edge, std::forward<T>(value));
            GraphBase<VertexAttributes, EdgeAttributes>::setEdgeAttribute(getEdgeReversal(edge), std::forward<T>(value));
        }
    };
}