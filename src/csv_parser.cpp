#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include "csv_parser.h"

std::string CSVRow::operator[](std::size_t index) const
{
  return std::string(&line[data[index] + 1], data[index + 1] - (data[index] + 1));
}

std::size_t CSVRow::size() const
{
  return data.size() - 1;
}

void CSVRow::read_next_row(std::istream& str)
{
  std::getline(str, line);
  data.clear();
  data.push_back(-1);
  std::string::size_type pos(0);
  while((pos = line.find(',', pos)) != std::string::npos)
  {
    data.push_back(pos);
    ++pos;
  }
  pos = line.size();
  data.push_back(pos);
}

std::string CSVRow::get_line() const
{
  return line;
}

std::istream& operator>>(std::istream& str, CSVRow& data)
{
  data.read_next_row(str);
  return str;
}

CSVIterator::CSVIterator(std::istream& str)
  : stream(str.good() ? &str : nullptr) {++(*this);}

CSVIterator::CSVIterator()
  : stream(nullptr) {}

CSVIterator& CSVIterator::operator++()
{
  if(stream)
  {
    if(!((*stream) >> row)) stream = nullptr;
  }
  return *this;
}

CSVIterator CSVIterator::operator++(int)
{
  CSVIterator tmp(*this);
  ++(*this);
  return tmp;
}

CSVRow const& CSVIterator::operator*() const {return row;}

CSVRow const* CSVIterator::operator->() const {return &row;}

bool CSVIterator::operator==(CSVIterator const& rhs)
{
  return ((this == &rhs)
	  || ((this->stream == nullptr) && (rhs.stream == nullptr)));
}

bool CSVIterator::operator!=(CSVIterator const& rhs) {return !((*this) == rhs);}

CSVRange::CSVRange(std::istream& str)
  : stream(str) {}

CSVIterator CSVRange::begin() const {return CSVIterator(stream);}
CSVIterator CSVRange::end() const {return CSVIterator();}
