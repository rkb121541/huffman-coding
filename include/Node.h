#include <string>

class Node {
public:
  char character;
  int frequency;
  Node *left = nullptr;
  Node *right = nullptr;
  std::string encoding = "";
};
