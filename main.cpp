#include <deque>
#include <format>
#include <iostream>
#include <map>
#include <string>

class Node {
public:
  std::string name;
  int frequency;
  Node *left = nullptr;
  Node *right = nullptr;
  std::string encoding = "";
};

void addToQueue(Node *node, std::deque<Node*> *priorityQueue) {
  bool added = false; // to see if node was already added
  for (std::deque<Node*>::iterator it = priorityQueue->begin();
       it != priorityQueue->end(); ++it) { // it is type iterator (basically like a ptr) / Node**
    if (node->frequency < (*it)->frequency) { // deref it to make it a Node*
      (*priorityQueue).insert(it, node);
      added = true;
      break;
    }
  }
  if (!added) {
    (*priorityQueue).push_back(node);
  }
}

// initially used to print tree nodes; now i use to make the encodings
void traverseTree(Node* root, std::map<std::string, std::string>* encodings) { // makes sure that the tree is being made properly
  if (root->left == nullptr && root->right == nullptr) {
    // std::cout << std::format("({} {})", root->name, root->encoding) << std::endl;
    (*encodings)[root->name] = root->encoding; 
    return;
  }
  root->left->encoding.append(root->encoding).append("0");
  traverseTree(root->left, encodings);
  root->right->encoding.append(root->encoding).append("1");
  traverseTree(root->right, encodings);
}

int main(int argc, char const *argv[]) {
  // this is already sorted from min to max
  // LATER: SEE HOW TO SORT IT
  std::map<std::string, int> frequencies = {{"a", 5},  {"b", 9},  {"c", 12},
                                      {"d", 13}, {"e", 16}, {"f", 45}}; // name frequencies
  std::deque<Node*> priorityQueue; // priority queue of min to max
  // for each {name, frequency} it gets made into a NEW node and pushed into priority queue
  for (auto elem : frequencies) {
    Node* node = new Node;
    node->name = elem.first;
    node->frequency = elem.second;
    priorityQueue.push_back(node);
  }

  Node* firstSmallest;
  Node* secondSmallest;
  while (priorityQueue.size() > 1) {
    firstSmallest = priorityQueue.front();
    priorityQueue.pop_front();
    secondSmallest = priorityQueue.front();
    priorityQueue.pop_front();
    Node* node = new Node;
    node->name = "node";
    node->frequency = firstSmallest->frequency + secondSmallest->frequency;
    node->left = firstSmallest;
    node->right = secondSmallest;
    addToQueue(node, &priorityQueue); // FUNCTION TO PUT IT IN THE RIGHT SPOT
  }
  std::map<std::string, std::string> encodings;
  traverseTree(priorityQueue.front(), &encodings);
  for (const auto& pair : encodings) { // printing out encodings map
    std::cout << std::format("{}:{}, ", pair.first, pair.second);
  }
  std::cout << std::endl;

  return 0;
}
