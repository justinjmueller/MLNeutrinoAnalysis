#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <iostream>
#include <string>
#include <vector>

class CSVRow
{
 public:
  std::string operator[](std::size_t index) const;
  std::size_t size() const;
  void read_next_row(std::istream& str);
  std::string get_line() const;
 private:
  std::string line;
  std::vector<int> data;
};

class CSVIterator
{   
 public:
  CSVIterator(std::istream& str);
  CSVIterator();
  CSVIterator& operator++();
  CSVIterator operator++(int);
  CSVRow const& operator*() const;
  CSVRow const* operator->() const;
  bool operator==(CSVIterator const& rhs);
  bool operator!=(CSVIterator const& rhs);
 private:
  std::istream* stream;
  CSVRow row;
};

class CSVRange
{
  std::istream& stream;
 public:
  CSVRange(std::istream& str);
  CSVIterator begin() const;
  CSVIterator end() const;
};
#endif
