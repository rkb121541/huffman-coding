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
};

void sortQueue(Node *node, std::deque<Node*> *priorityQueue) {
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

void printTree(Node* root) {
  std::cout << std::format("({} {})", root->name, root->frequency) << std::endl;
  if (root->left == nullptr && root->right == nullptr) {
    return;
  }
  printTree(root->left);
  printTree(root->right);
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
    sortQueue(node, &priorityQueue); // FUNCTION TO PUT IT IN THE RIGHT SPOT
  }
  printTree(priorityQueue.front());
  return 0;
}
