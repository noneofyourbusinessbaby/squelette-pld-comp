#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

// Placeholder for variable metadata
struct VarMetaData {
  // Add metadata fields here in the future
};

class VarIndex {
public:
  // Set or update a variable, assign index if new
  void setVariable(const std::string &name) {
    if (indices_.find(name) == indices_.end()) {
      int idx = nextIndex_;
      nextIndex_ += 4;
      indices_[name] = idx;
      std::cout << "Index: " << idx << " Name: " << name << std::endl;
    }
  }

  // Check if a variable exists
  bool hasVariable(const std::string &name) const {
    return indices_.find(name) != indices_.end();
  }

  // Remove a variable
  void removeVariable(const std::string &name) { indices_.erase(name); }

  // Get the index of a variable, returns -1 if not found
  int getIndex(const std::string &name) const {
    auto it = indices_.find(name);
    if (it != indices_.end()) {
      return it->second;
    }
    return -1;
  }

private:
  std::unordered_map<std::string, int> indices_;
  int nextIndex_ = 0;
};