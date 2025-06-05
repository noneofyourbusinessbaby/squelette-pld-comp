#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

// Placeholder for variable metadata
struct VarMetaData
{
  // Add metadata fields here in the future
  int occurrences = 0; // Example field for tracking occurrences
};

class VarIndex
{
public:
  // Set or update a variable, assign index if new
  void setVariable(const std::string &name)
  {
    if (indices_.find(name) == indices_.end())
    {
      int idx = nextIndex_;
      nextIndex_ += 1;
      indices_[name] = idx;
      // create metadata entry
      metadata_[name] = VarMetaData();
      // std::cout << "Index: " << idx << " Name: " << name << std::endl;
    }
  }

  // Check if a variable exists
  bool hasVariable(const std::string &name) const
  {
    return indices_.find(name) != indices_.end();
  }

  // Remove a variable
  void removeVariable(const std::string &name) { indices_.erase(name); }

  // Get the index of a variable, returns -1 if not found
  int getIndex(const std::string &name) const
  {
    auto it = affected_variables_indices_.find(name);
    if (it != affected_variables_indices_.end())
    {
      return it->second;
    }
    return -1;
  }

  void incrementOccurrences(const std::string &name)
  {
    if (metadata_.find(name) == metadata_.end())
    {
      metadata_[name] = VarMetaData();
    }
    metadata_[name].occurrences++;
  }

  void associateDownwardGrowingIndexForEachVariableInAffectedVariableIndex()
  {
    // get the size of the indices map
    int max_index = affected_variables_indices_.size() * 4;

    // iterate through the indices map each index = max_index - current_index * 4
    for (const auto &pair : affected_variables_indices_)
    {
      const std::string &name = pair.first;
      int current_index = pair.second;
      int new_index = max_index - current_index * 4;
      affected_variables_indices_[name] = new_index;
      // std::cout << "Updated Index: " << new_index << " Name: " << name << std::endl;
    }
  }

  void setVariableAffectedVaribleIndex(const std::string &name)
  {
    if (affected_variables_indices_.find(name) == affected_variables_indices_.end())
    {
      int idx = affected_variables_nextIndex_;
      affected_variables_nextIndex_ += 1;
      affected_variables_indices_[name] = idx;
      // create metadata entry
      metadata_[name] = VarMetaData();
      // std::cout << "Index for stack variables: " << idx << " Name: " << name << std::endl;
    }
  }

private:
  std::unordered_map<std::string, int> indices_;
  std::unordered_map<std::string, int> affected_variables_indices_;
  std::unordered_map<std::string, VarMetaData> metadata_;

  int nextIndex_ = 0;
  int affected_variables_nextIndex_ = 0;
};