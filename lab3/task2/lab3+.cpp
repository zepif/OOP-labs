#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

// Define GraphNode
class GraphNode {
private:
    std::string name;
    std::vector<std::shared_ptr<GraphNode>> connections;

public:
    explicit GraphNode(std::string nodeName) : name(std::move(nodeName)) {}
    
    void addConnection(const std::shared_ptr<GraphNode>& node) {
        if (std::find_if(connections.begin(), connections.end(),
            [&node](const auto& conn) { return conn.get() == node.get(); }) == connections.end()) {
            connections.push_back(node);
        }
    }
    
    void removeConnection(const std::shared_ptr<GraphNode>& node) {
        connections.erase(
            std::remove_if(connections.begin(), connections.end(),
                [&node](const auto& conn) { return conn.get() == node.get(); }),
            connections.end()
        );
    }
    
    const std::string& getName() const {
        return name;
    }
    
    const std::vector<std::shared_ptr<GraphNode>>& getConnections() const {
        return connections;
    }
};

// Define Graph
class Graph {
private:
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> nodes;
    std::string startNode;

public:
    Graph() = default;
    
    ~Graph() {
        nodes.clear();
    }
    
    Graph(const Graph& other) {
        for (const auto& [name, node] : other.nodes) {
            addNode(name);
        }
        
        for (const auto& [name, node] : other.nodes) {
            for (const auto& conn : node->getConnections()) {
                connect(name, conn->getName());
            }
        }
        
        if (!other.startNode.empty()) {
            startNode = other.startNode;
        }
    }
    
    Graph& operator=(const Graph& other) {
        if (this != &other) {
            nodes.clear();

            for (const auto& [name, node] : other.nodes) {
                addNode(name);
            }

            for (const auto& [name, node] : other.nodes) {
                for (const auto& conn : node->getConnections()) {
                    connect(name, conn->getName());
                }
            }

            startNode = other.startNode;
        }
        return *this;
    }
    
    Graph(Graph&& other) noexcept 
        : nodes(std::move(other.nodes))
        , startNode(std::move(other.startNode)) {
    }
    
    Graph& operator=(Graph&& other) noexcept {
        if (this != &other) {
            nodes = std::move(other.nodes);
            startNode = std::move(other.startNode);
        }
        return *this;
    }

    void addNode(const std::string& name) {
        if (nodes.empty()) {
            startNode = name;
        }
        nodes[name] = std::make_shared<GraphNode>(name);
    }
    
    void connect(const std::string& from, const std::string& to) {
        if (nodes.count(from) && nodes.count(to)) {
            nodes[from]->addConnection(nodes[to]);
            nodes[to]->addConnection(nodes[from]);
        }
    }
    
    void removeNode(const std::string& name) {
        if (nodes.count(name)) {
            auto node = nodes[name];
            for (auto& conn : node->getConnections()) {
                conn->removeConnection(node);
            }
            nodes.erase(name);
            
            if (name == startNode && !nodes.empty()) {
                startNode = nodes.begin()->first;
            }
        }
    }
    
    std::string getStartNode() const {
        return startNode;
    }

    void printGraph(const std::string& graphName = "Graph") const {
        std::cout << "\n=== " << graphName << " ===" << std::endl;
        std::cout << "Start node: " << startNode << std::endl;
        std::cout << "Nodes and their connections:" << std::endl;
        
        for (const auto& [name, node] : nodes) {
            std::cout << name << " -> ";
            const auto& connections = node->getConnections();
            if (connections.empty()) {
                std::cout << "no connections";
            } else {
                for (size_t i = 0; i < connections.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << connections[i]->getName();
                }
            }
            std::cout << std::endl;
        }
        std::cout << "====================\n" << std::endl;
    }
};

int main() {
    Graph A;
    A.addNode("1");
    A.addNode("2");
    A.addNode("3");
    A.connect("1", "2");
    A.connect("2", "3");
    A.printGraph("Graph A - Initial");

    Graph B = A;
    B.printGraph("Graph B - After copy from A");

    B.removeNode("2");
    B.printGraph("Graph B - After removing node 2");

    Graph C;
    C.addNode("4");
    C.addNode("5");
    C.printGraph("Graph C - Initial");

    C = B;
    C.printGraph("Graph C - After copy assignment from B");

    Graph newA;
    newA.addNode("6");
    newA.printGraph("newA - Initial");

    newA = std::move(C);
    newA.printGraph("newA - After move assignment from C");
        
    return 0;
}