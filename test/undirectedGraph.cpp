#include <gtest/gtest.h>
#include <stdexcept>
#include "graph.hpp"

using Edge = Basis::Edge;
using type = Basis::GraphType;
using GraphClass = Basis::Graph<type::undirected, int>;

struct CustomVertexAttribute {
    float weight;
    std::string label;
};


class UndirectedGraphTest: public testing::Test {
    public:
    GraphClass graph;

    UndirectedGraphTest(): graph() {}
};

class PopulatedUndirectedGraphTest: public testing::Test {
    public:
    Basis::Graph<type::undirected, int> graph {};

    PopulatedUndirectedGraphTest() {
        graph.addEdge(Edge{.from = 0, .to = 1});
        graph.addEdge(Edge{.from = 1, .to = 2});
        graph.addEdge(Edge{.from = 2, .to = 0});
    }
};

TEST_F(UndirectedGraphTest, isDirected) {
    ASSERT_FALSE(graph.is_directed());
}

TEST_F(UndirectedGraphTest, AddNonexistentEdge) {
    const auto toAdd {Edge{.from = 0, .to = 1}};
    const auto toAddReverse {Edge{.from = toAdd.to, .to = toAdd.from}};
    graph.addEdge(toAdd);


    ASSERT_TRUE(graph.doesVertexExist(toAdd.from));
    ASSERT_TRUE(graph.doesVertexExist(toAdd.to));
    ASSERT_TRUE(graph.doesEdgeExist(toAdd));
    ASSERT_TRUE(graph.doesEdgeExist(toAddReverse));
    ASSERT_EQ(graph.getVertexCount(), 2);
    ASSERT_EQ(graph.getEdgeCount(), 1);
}

TEST_F(UndirectedGraphTest, AddEdgeFromExistingVertex) {
    const auto existingEdge {Edge{.from = 0, .to = 2}};
    const auto existingEdgeReverse {Edge{.from = existingEdge.to, .to = existingEdge.from}};
    graph.addEdge(existingEdge);
    ASSERT_TRUE(graph.doesVertexExist(existingEdge.from));
    ASSERT_TRUE(graph.doesVertexExist(existingEdge.to));
    ASSERT_TRUE(graph.doesEdgeExist(existingEdge));
    ASSERT_TRUE(graph.doesEdgeExist(existingEdgeReverse));
    ASSERT_EQ(graph.getEdgeCount(), 1);

    const auto toAdd {Edge{.from = 0, .to = 1}};
    const auto toAddReverse {Edge{.from = toAdd.to, .to = toAdd.from}};
    graph.addEdge(toAdd);
    ASSERT_TRUE(graph.doesVertexExist(toAdd.from));
    ASSERT_TRUE(graph.doesVertexExist(toAdd.to));
    ASSERT_TRUE(graph.doesEdgeExist(toAdd));
    ASSERT_TRUE(graph.doesEdgeExist(toAddReverse));
    ASSERT_EQ(graph.getEdgeCount(), 2);
    ASSERT_EQ(graph.getVertexCount(), 3);
}

TEST_F(UndirectedGraphTest, AddExistingEdge) {
    const auto toAdd {Edge{.from = 0, .to = 1}};
    const auto toAddReverse {Edge{.from = toAdd.to, .to = toAdd.from}};
    graph.addEdge(toAdd);
    graph.addEdge(toAddReverse);
    graph.addEdge(toAdd);

    ASSERT_TRUE(graph.doesVertexExist(toAdd.from));
    ASSERT_TRUE(graph.doesVertexExist(toAdd.to));
    ASSERT_TRUE(graph.doesEdgeExist(toAdd));
    ASSERT_TRUE(graph.doesEdgeExist(toAddReverse));
    ASSERT_EQ(graph.getVertexCount(), 2);
    ASSERT_EQ(graph.getEdgeCount(), 1);
}

TEST_F(UndirectedGraphTest, VertexCountEmpty) {
    ASSERT_EQ(graph.getVertexCount(), 0);
}

TEST_F(UndirectedGraphTest, VertexCountNonEmpty) {
    graph.addEdge(Edge{.from = 0, .to = 1});
    ASSERT_EQ(graph.getVertexCount(), 2);
}

TEST_F(UndirectedGraphTest, DoesEdgeExist) {
    graph.addEdge(Edge{.from = 0, .to = 1});
    ASSERT_EQ(graph.getEdgeCount(), 1);
    ASSERT_TRUE(graph.doesEdgeExist(Edge{.from = 0, .to = 1}));
    ASSERT_TRUE(graph.doesEdgeExist(Edge{.from = 1, .to = 0}));
}

TEST_F(UndirectedGraphTest, DoesEdgeExistNonexistentEdge) {
    ASSERT_EQ(graph.getEdgeCount(), 0);
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 0, .to = 1}));
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 1, .to = 0}));
}

TEST_F(PopulatedUndirectedGraphTest, RemoveEdge) {
    const auto previousEdgeCount {graph.getEdgeCount()};
    const auto previousVertexCount {graph.getVertexCount()};
    ASSERT_TRUE(graph.removeEdge(Edge{.from = 2, .to = 0}));
    ASSERT_EQ(graph.getEdgeCount(), previousEdgeCount - 1);
    ASSERT_EQ(graph.getVertexCount(), previousVertexCount);
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 2, .to = 0}));
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 0, .to = 2}));
}

TEST_F(PopulatedUndirectedGraphTest, RemoveEdgeTwice) {
    const auto previousEdgeCount {graph.getEdgeCount()};
    ASSERT_TRUE(graph.removeEdge(Edge{.from = 2, .to = 0}));
    ASSERT_FALSE(graph.removeEdge(Edge{.from = 0, .to = 2}));
    ASSERT_EQ(graph.getEdgeCount(), previousEdgeCount - 1);
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 2, .to = 0}));
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 0, .to = 2}));
}

TEST_F(PopulatedUndirectedGraphTest, RemoveEdgeReversal) {
    const auto previousEdgeCount {graph.getEdgeCount()};
    ASSERT_TRUE(graph.removeEdge(Edge{.from = 0, .to = 2}));

    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 2, .to = 0}));
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 0, .to = 2}));
    ASSERT_EQ(graph.getEdgeCount(), previousEdgeCount - 1);
}

TEST_F(PopulatedUndirectedGraphTest, RemoveNonexistentEdge) {
    const auto previousEdgeCount {graph.getEdgeCount()};
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 0, .to = 3}));
    ASSERT_FALSE(graph.doesEdgeExist(Edge{.from = 3, .to = 0}));
    ASSERT_FALSE(graph.removeEdge(Edge{.from = 0, .to = 3}));
    ASSERT_EQ(graph.getEdgeCount(), previousEdgeCount);
}

TEST(GraphAttributeTest, SetAndGetAttributeDefaultType) {
    Basis::Graph<type::undirected, int> graph {};
    constexpr int coolValue {32};
    
    graph.addEdge(Edge{0, 1});
    graph.setVertexAttribute(0, coolValue);
    graph.setVertexAttribute(0, coolValue * 2);

    ASSERT_EQ(graph.getVertexAttribute(0), coolValue * 2);
    ASSERT_THROW(static_cast<void>(graph.getVertexAttribute(1)), std::out_of_range);
}

TEST(GraphAttributeTest, SetAndGetAttributeCustomType) {
    Basis::Graph<Basis::GraphType::undirected, CustomVertexAttribute> graph;
    const CustomVertexAttribute a1 {.weight = 1.0F, .label = "road"};
    const CustomVertexAttribute expected {.weight = 0.83F, .label = "dog"};

    graph.addEdge(Edge{0, 1});
    graph.setVertexAttribute(0, a1);
    graph.setVertexAttribute(0, expected);
    auto result {graph.getVertexAttribute(0)};

    ASSERT_EQ(result.weight, expected.weight);
    ASSERT_EQ(result.label, expected.label);
    ASSERT_THROW(static_cast<void>(graph.getVertexAttribute(1)), std::out_of_range);
}