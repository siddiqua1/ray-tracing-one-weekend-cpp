#pragma once

#include <fstream>
#include <iostream>

struct A
{
  void show() { std::cout << "I am A\n"; }
};
struct B
{
  void show() { std::cout << "I am B\n"; }
};

template <bool which>
auto get_obj()
{
  if constexpr (which)
    return A{};
  else
    return B{};
}

struct Read
{
  void read()
  {
    std::cout << "reading\n";
  }
};

struct Write
{
  void write()
  {
    std::cout << "writing\n";
  }
};

struct Append
{
  void append()
  {
    std::cout << "appending\n";
  }
};

struct Fake
{
};

template <char which>
auto get_type()
{
  if constexpr (which == 'r')
  {
    return Read{};
  }
  else if constexpr (which == 'w')
  {
    return Write{};
  }
  else if constexpr (which == 'a')
  {
    return Append{};
  }
  else
  {
    return Fake{};
  }
}
