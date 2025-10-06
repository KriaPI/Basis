#include <gtest/gtest.h>
#include "graph.hpp"

using Edge = Basis::Edge;
using type = Basis::GraphType;

class DirectedGraphTest: public testing::Test {
    protected:
    Basis::Graph<type::directed, int> graph;

    DirectedGraphTest(): graph() {}
};

class PopulatedDirectedGraphTest: public testing::Test {
    protected:
    Basis::Graph<type::directed, int> graph {};

    PopulatedDirectedGraphTest() {
        graph.addEdge(Edge{.from = 0, .to = 1});
        graph.addEdge(Edge{.from = 1, .to = 2});
        graph.addEdge(Edge{.from = 2, .to = 0});
    }
};


TEST_F(DirectedGraphTest, AddNonexistentEdge) {
    const auto toAdd {Edge{.from = 0, .to = 1}};
    const auto toAddToo {Edge{.from = 1, .to = 2}};
    graph.addEdge(toAdd);
    graph.addEdge(toAddToo);

    ASSERT_TRUE(graph.doesVertexExist(toAdd.from));
    ASSERT_TRUE(graph.doesVertexExist(toAddToo.from));
    ASSERT_TRUE(graph.doesVertexExist(toAdd.to));
    ASSERT_TRUE(graph.doesVertexExist(toAddToo.to));
    ASSERT_TRUE(graph.doesEdgeExist(toAdd));
    ASSERT_TRUE(graph.doesEdgeExist(toAddToo));
}

TEST_F(DirectedGraphTest, AddEdgeFromExistingVertex) {
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

TEST_F(DirectedGraphTest, AddEdgeToExistingVertex) {
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

TEST_F(DirectedGraphTest, AddExistingEdge) {
    const auto toAdd {Edge{.from = 0, .to = 1}};
    graph.addEdge(toAdd);
    graph.addEdge(toAdd);

    ASSERT_TRUE(graph.doesVertexExist(toAdd.from));
    ASSERT_TRUE(graph.doesVertexExist(toAdd.to));
    ASSERT_TRUE(graph.doesEdgeExist(toAdd));
    ASSERT_EQ(graph.getVertexCount(), 2);
    ASSERT_EQ(graph.getEdgeCount(), 1);
}

TEST_F(DirectedGraphTest, VertexCountEmpty) {
    ASSERT_EQ(graph.getVertexCount(), 0);
}

TEST_F(DirectedGraphTest, VertexCountNonEmpty) {
    graph.addEdge(Edge{.from = 0, .to = 1});
    ASSERT_EQ(graph.getVertexCount(), 2);
}

TEST_F(DirectedGraphTest, RemoveEdge) {
    graph.addEdge(Edge{.from = 0, .to = 1});
        graph.addEdge(Edge{.from = 1, .to = 2});
        graph.addEdge(Edge{.from = 2, .to = 0});

    const auto previousEdgeCount {graph.getEdgeCount()};
    const auto previousVertexCount {graph.getVertexCount()};
    ASSERT_TRUE(graph.removeEdge(Edge{.from = 2, .to = 0}));
    ASSERT_EQ(graph.getEdgeCount(), previousEdgeCount - 1);
    ASSERT_EQ(graph.getVertexCount(), previousVertexCount);
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 2, .to = 0}));
}

TEST_F(PopulatedDirectedGraphTest, RemoveEdgeTwice) {
    const auto previousEdgeCount {graph.getEdgeCount()};
    ASSERT_TRUE(graph.removeEdge(Edge{.from = 2, .to = 0}));
    ASSERT_FALSE(graph.removeEdge(Edge{.from = 2, .to = 0}));
    ASSERT_EQ(graph.getEdgeCount(), previousEdgeCount - 1);
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 2, .to = 0}));
}

TEST_F(PopulatedDirectedGraphTest, RemoveEdgeReversal) {
    graph.addEdge(Edge{.from = 0, .to = 2});
    const auto previousEdgeCount {graph.getEdgeCount()};
    ASSERT_TRUE(graph.removeEdge(Edge{.from = 2, .to = 0}));
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 2, .to = 0}));
    ASSERT_TRUE(graph.doesEdgeExist(Edge{.from = 0, .to = 2}));
    ASSERT_EQ(graph.getEdgeCount(), previousEdgeCount - 1);
}

TEST_F(PopulatedDirectedGraphTest, RemoveNonexistentEdge) {
    const auto previousEdgeCount {graph.getEdgeCount()};
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 0, .to = 3}));
    ASSERT_FALSE(graph.removeEdge(Edge{.from = 0, .to = 3}));
    ASSERT_EQ(graph.getEdgeCount(), previousEdgeCount);
}