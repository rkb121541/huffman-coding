#include <bits/stdc++.h>
#include <deque>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

struct Node {
  char character;
  int frequency;
  Node *left = nullptr;
  Node *right = nullptr;
  std::string encoding = "";
};


void addToQueue(Node *node, std::deque<Node *> *priorityQueue) {
  bool added = false;
  for (std::deque<Node *>::iterator it = priorityQueue->begin(); it != priorityQueue->end(); ++it) {
    if (node->frequency < (*it)->frequency || (node->frequency == (*it)->frequency && node->character < (*it)->character)) {
      (*priorityQueue).insert(it, node);
      added = true;
      break;
    } 
  }
  if (!added) {
    (*priorityQueue).push_back(node);
  }
}

// used to make the encodings
void traverseTree(Node *root, std::map<char, std::string> *encodings) { 
  if (root->left == nullptr && root->right == nullptr) {
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

std::pair<std::string, char> flip_pair(const std::pair<char, std::string> &p) {
  return std::pair<std::string, char>(p.second, p.first);
}

std::map<std::string, char> flip_map(const std::map<char, std::string> &src) {
  std::map<std::string, char> dst;
  std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()), 
                   flip_pair);
  return dst;
}

int main(int argc, char const *argv[]) {
  std::cout << "Please put the file you want to encode into the `examples_og` directory before running the program" << std::endl;
  std::string userInput;
  std::cout << "Enter file name: ";
  std:: cin >> userInput;
  // userInput = "simple_paragraph.txt";
  std::string inputDirectory = "./examples_og/"; 
  std::ifstream inputFile(inputDirectory + userInput);
  if (!inputFile) {
    std::cerr << "Error! Unable to open file" << std::endl;
    return 1;
  }

  std::map<char, int> characterFrequencies;
  char ch;
  std::string inputString;
  while (inputFile.get(ch)) {
    characterFrequencies[ch]++; // add ch to map
    std::string charToString(1, ch);
    inputString.append(charToString); // add charToString to inputString
  }

  // I NEED TO SORT THE characterFrequencies so I had to make it into a vector
  std::vector<std::pair<char, int>> freqs = sortedVector(characterFrequencies);

  // printing out sorted vector
  // for (const auto &pair : freqs) {
  //   std::cout << std::format("{}:{}, ", pair.first, pair.second);
  // }
  // std::cout << std::endl;
  // std::cout << inputString << std::endl;

  std::deque<Node*> priorityQueue; // priority queue of min to max
  // for each {letter, frequency} it gets made into a NEW node and pushed into
  // priority queue
  for (auto elem : freqs) { // std::vector<std::pair<char, int>> freqs
    Node *node = new Node;
    node->character = elem.first;
    node->frequency = elem.second;
    priorityQueue.push_back(node); // adding it to the priority queue
  }

  Node *firstSmallest;
  Node *secondSmallest;
  // does the huffman tree building
  while (priorityQueue.size() > 1) {
    firstSmallest = priorityQueue.front();
    priorityQueue.pop_front();
    secondSmallest = priorityQueue.front();
    priorityQueue.pop_front();
    Node *node = new Node;
    node->character = '\0'; // for a node that is not og
    node->frequency = firstSmallest->frequency + secondSmallest->frequency;
    node->left = firstSmallest;
    node->right = secondSmallest;
    addToQueue(node, &priorityQueue); // FUNCTION TO PUT IT IN THE RIGHT SPOT
  }

  std::map<char, std::string> encodings; // recursive function so better to initialize here
  traverseTree(priorityQueue.front(), &encodings);
  // printing out encodings map
  // for (const auto &pair : encodings) { 
  //   std::cout << std::format("{}:{}, ", pair.first, pair.second);
  // }
  // std::cout << std::endl;

  // make encoded string
  std::string encodedString = "";
  for (auto c : inputString) {
    encodedString.append(encodings[c]);
  }
  // std::cout << encodedString << std::endl;


  // make the actual encode file
  // encoded file will include the table of mappings and the encoded string
  std::string encodedDirectory = "./examples_encodings/";
  
  size_t dotPosition = userInput.rfind('.'); // reverse find to find the last '.' smart
  std::string encodedFileName; // THIS SHOULD BE .hzip
  if (dotPosition != std::string::npos) {
    encodedFileName = userInput.substr(0, dotPosition) + "_encoding.hzip";
  } else {
    std::cerr << "Error! Make sure its a txt file" << std::endl;
    return 1;
  }

  std::ofstream encodedFile(encodedDirectory + encodedFileName);
  if (!encodedFile) {
    std::cerr << "Error! Unable to open file" << std::endl;
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
  
  std::map<std::string, char> flipped = flip_map(encodings); // {encoding: char}

  // time to decode the encoded string
  std::string decodedString = "";
  int leftptr = 0;
  int rightptr = 1;
  while (leftptr < rightptr && leftptr < encodedString.length()) {
    if (flipped.contains(encodedString.substr(leftptr, rightptr-leftptr))) {
      // substring exists in flipped map
      std::string charToString(1, flipped[encodedString.substr(leftptr, rightptr-leftptr)]);
      decodedString.append(charToString);
      leftptr = rightptr;
      rightptr++;
    } else {
        // substring doesnt exist in flipped map
        rightptr++;
    }
  }

  std::string decodedDirectory = "./examples_decodings/";
  
  dotPosition = userInput.rfind('.'); // reverse find to find the last '.' smart
  std::string decodedFileName; // THIS SHOULD BE .hzip
  if (dotPosition != std::string::npos) {
    decodedFileName = userInput.substr(0, dotPosition) + "_decoding.txt";
  } 

  std::ofstream decodedFile(decodedDirectory + decodedFileName);

  decodedFile << decodedString;
  decodedFile.close();

  if (inputString == decodedString) {
    std::cout << "Encoded and decoded successfully!" << std::endl;
  } else {
    std::cerr << "Error! Issue with encoding and decoding." << std::endl;
    return 1;
  }



  return 0;
}
