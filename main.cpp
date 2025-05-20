#include <map>
#include <iostream>
#include <format>
#include <deque>
#include <string>

class Node {
public:
    std::string letter;
    int frequency;
    Node* left = nullptr;
    Node* right = nullptr;
};

std::deque<Node> sortQueue(Node node, std::deque<Node> priorityQueue) {
    for (std::deque<Node>::iterator it = priorityQueue.begin(); it != priorityQueue.end(); ++it) {
        auto next_it = std::next(it);
        if (next_it == priorityQueue.end()) {
            
        }
    }
}

int main(int argc, char const *argv[])
{
    // this is already sorted from min to max
    // LATER: SEE HOW TO SORT IT 
    std::map<std::string, int> pairs = { {"a", 5}, {"b", 9}, {"c", 12}, {"d", 13}, {"e", 16}, {"f", 45} };
    Node tempNode; // variable declaration
    std::deque<Node> priorityQueue; // put it into queue
    for (auto elem : pairs) {
        tempNode.letter = elem.first;
        tempNode.frequency = elem.second;
        priorityQueue.push_back(tempNode);
        // std::cout << std::format("Node(letter: {}, frequency: {}) ", tempNode.letter, tempNode.frequency);
    }
    // std::cout << std::endl;

    Node firstSmallest;
    Node secondSmallest;
    while (!priorityQueue.empty()) {
        firstSmallest = priorityQueue.front();
        priorityQueue.pop_back();
        secondSmallest = priorityQueue.front();
        priorityQueue.pop_back();
        // FUNCTION TO PUT IT IN THE RIGHT SPOT
    }
    

    return 0;
}
