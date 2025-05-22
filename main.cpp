#include <bits/stdc++.h>
#include <deque>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

class Node {
public:
  char character;
  int frequency;
  Node *left = nullptr;
  Node *right = nullptr;
  std::string encoding = "";
};

void addToQueue(Node *node, std::deque<Node *> *priorityQueue) {
  bool added = false; // to see if node was already added
  for (std::deque<Node *>::iterator it = priorityQueue->begin();
       it != priorityQueue->end();
       ++it) { // it is type iterator (basically like a ptr) / Node**
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
void traverseTree(
    Node *root,
    std::map<char, std::string>
        *encodings) { // makes sure that the tree is being made properly
  if (root->left == nullptr && root->right == nullptr) {
    // std::cout << std::format("({} {})", root->character, root->encoding) <<
    // std::endl;
    (*encodings)[root->character] = root->encoding;
    return;
  }
  root->left->encoding.append(root->encoding).append("0");
  traverseTree(root->left, encodings);
  root->right->encoding.append(root->encoding).append("1");
  traverseTree(root->right, encodings);
}

bool cmp(std::pair<char, int>& a, std::pair<char, int>& b) {
  return a.second < b.second; // sorting by frequency/ value
}

std::vector<std::pair<char, int>> sortedVector(std::map<char, int>& m) {
  std::vector<std::pair<char, int>> v;
  for (auto& it : m) {
    v.push_back(it);
  }
  std::sort(v.begin(), v.end(), cmp);
  return v;
}

std::pair<std::string, char> flip_pair(const std::pair<char, std::string> &p)
{
  return std::pair<std::string, char>(p.second, p.first);
}

std::map<std::string, char> flip_map(const std::map<char, std::string> &src)
{
  std::map<std::string, char> dst;
  std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()), 
                   flip_pair);
  return dst;
}

int main(int argc, char const *argv[]) {
  std::map<char, int> characterFrequencies;
  // reading a file
  std::ifstream inputFile("./examples/simple_paragraph.txt");
  // error checking to make sure file is open
  if (!inputFile) {
    std::cerr << "Unable to open file" << std::endl;
    return 1;
  }
  char ch;
  std::string inputString;
  while (inputFile.get(ch)) {
    characterFrequencies[ch]++; // add ch to map
    std::string charToString(1, ch);
    inputString.append(charToString); // add charToString to inputString
  }

  // I NEED TO SORT THE characterFrequencies -> make it into a vector
  std::vector<std::pair<char, int>> freqs = sortedVector(characterFrequencies);

  for (const auto &pair : freqs) { // printing out sorted vector
    std::cout << std::format("{}:{}, ", pair.first, pair.second);
  }
  std::cout << std::endl;
  std::cout << inputString << std::endl;

  // this is already sorted from min to max
  // LATER: SEE HOW TO SORT IT
  // std::map<char, int> frequencies = {
  //     {'a', 5},  {'b', 9},  {'c', 12},
  //     {'d', 13}, {'e', 16}, {'f', 45}}; // letter frequencies

  std::deque<Node*> priorityQueue; // priority queue of min to max
  // for each {letter, frequency} it gets made into a NEW node and pushed into
  // priority queue
  for (auto elem : freqs) {
    Node *node = new Node;
    node->character = elem.first;
    node->frequency = elem.second;
    priorityQueue.push_back(node);
  }

  Node *firstSmallest;
  Node *secondSmallest;
  while (priorityQueue.size() > 1) {
    firstSmallest = priorityQueue.front();
    priorityQueue.pop_front();
    secondSmallest = priorityQueue.front();
    priorityQueue.pop_front();
    Node *node = new Node;
    node->character = '\0';
    node->frequency = firstSmallest->frequency + secondSmallest->frequency;
    node->left = firstSmallest;
    node->right = secondSmallest;
    addToQueue(node, &priorityQueue); // FUNCTION TO PUT IT IN THE RIGHT SPOT
  }
  std::map<char, std::string> encodings;
  traverseTree(priorityQueue.front(), &encodings);
  for (const auto &pair : encodings) { // printing out encodings map
    std::cout << std::format("{}:{}, ", pair.first, pair.second);
  }
  std::cout << std::endl;

  // write to encoded string
  std::string encodedString = "";
  for (auto c : inputString) {
    encodedString.append(encodings[c]);
  }
  std::cout << encodedString << std::endl;
  
  std::map<std::string, char> flipped = flip_map(encodings);
  // time to decode the encoded string
  // encoded string ex: 001 011111011000010111110110000101111101
  std::string result = "";
  int leftptr = 0;
  int rightptr = 1;
  while (leftptr < rightptr && leftptr < encodedString.length()) {
    if (flipped.contains(encodedString.substr(leftptr, rightptr-leftptr))) {
      // substring exists in flipped map
      std::string charToString(1, flipped[encodedString.substr(leftptr, rightptr-leftptr)]);
      result.append(charToString);
      leftptr = rightptr;
      rightptr++;
    } else {
        // substring doesnt exist in flipped map
        rightptr++;
    }
  }

  // make the actual encode file
  // encode file will include the table of mappings and the encoded string
  std::ofstream encodedFile("./examples_encodings/simple_paragraph_encoding.txt");
  // for error checking
  if (!encodedFile) {
    std::cerr << "Unable to open file" << std::endl;
    return 1;
  }
    
  for (auto elem : encodings) {
    if (elem.first == '\n') {
      encodedFile << std::format("NEWLINE {}", elem.second) << "\n";
    } else if (elem.first == ' ') {
      encodedFile << std::format("SPACE {}", elem.second) << "\n";
    } else {
      encodedFile << std::format("{} {}", elem.first, elem.second) << "\n";
    }
  }
  encodedFile << "\n";
  encodedFile << encodedString;
  encodedFile.close();
  

  std::cout << result << std::endl;
  
  return 0;
}
