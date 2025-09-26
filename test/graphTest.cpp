#include <gtest/gtest.h>
#include "graph.hpp"

using Edge = Basis::Edge;
using type = Basis::GraphType;

TEST(GraphTests, AddNonexistantEdge) {
    Basis::Graph<type::directed, int> graph {};
    const auto toAdd {Edge{.from = 0, .to = 1}};
    graph.addEdge(toAdd);

    ASSERT_TRUE(graph.doesVertexExist(toAdd.from));
    ASSERT_TRUE(graph.doesVertexExist(toAdd.to));
    ASSERT_TRUE(graph.doesEdgeExist(toAdd));
}

TEST(GraphTests, AddEdgeFromExistingVertex) {
    Basis::Graph<type::directed, int> graph {};
    const auto existingEdge {Edge{.from = 0, .to = 2}};
    graph.addEdge(existingEdge);
    ASSERT_TRUE(graph.doesVertexExist(existingEdge.from));
    ASSERT_TRUE(graph.doesVertexExist(existingEdge.to));
    ASSERT_TRUE(graph.doesEdgeExist(existingEdge));

    const auto toAdd {Edge{.from = 0, .to = 1}};
    graph.addEdge(toAdd);
    ASSERT_TRUE(graph.doesVertexExist(toAdd.from));
    ASSERT_TRUE(graph.doesVertexExist(toAdd.to));
    ASSERT_TRUE(graph.doesEdgeExist(toAdd));
}

TEST(GraphTests, AddEdgeToExistingVertex) {
    Basis::Graph<type::directed, int> graph {};
    const auto existingEdge {Edge{.from = 0, .to = 2}};
    graph.addEdge(existingEdge);
    ASSERT_TRUE(graph.doesVertexExist(existingEdge.from));
    ASSERT_TRUE(graph.doesVertexExist(existingEdge.to));
    ASSERT_TRUE(graph.doesEdgeExist(existingEdge));

    const auto toAdd {Edge{.from = 0, .to = 1}};
    graph.addEdge(toAdd);
    ASSERT_TRUE(graph.doesVertexExist(toAdd.from));
    ASSERT_TRUE(graph.doesVertexExist(toAdd.to));
    ASSERT_TRUE(graph.doesEdgeExist(toAdd));
}

TEST(GraphTests, AddExistingEdge) {
    Basis::Graph<type::directed, int> graph {};
    const auto toAdd {Edge{.from = 0, .to = 1}};
    graph.addEdge(toAdd);
    graph.addEdge(toAdd);

    ASSERT_TRUE(graph.doesVertexExist(toAdd.from));
    ASSERT_TRUE(graph.doesVertexExist(toAdd.to));
    ASSERT_TRUE(graph.doesEdgeExist(toAdd));
    ASSERT_EQ(graph.getVertexCount(), 2);
    ASSERT_EQ(graph.getEdgeCount(), 1);
}

TEST(GraphTests, VertexCountEmpty) {
    Basis::Graph<type::directed, int> graph {};
    ASSERT_EQ(graph.getVertexCount(), 0);
}

TEST(GraphTests, VertexCountNonEmpty) {
    Basis::Graph<type::directed, int> graph {};
    graph.addEdge(Edge{.from = 0, .to = 1});

    ASSERT_EQ(graph.getVertexCount(), 2);
}