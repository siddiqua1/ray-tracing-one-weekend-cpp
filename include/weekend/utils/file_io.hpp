#pragma once

#include <fstream>
#include <vector>
#include <format>

namespace weekend::utils
{
  struct Write;
  struct Fake;

  template <char mode>
  auto open(const char *file_name)
  {

    if constexpr (mode == 'w')
    {
      return Write{file_name};
    }
    else
    {
      return Fake{file_name};
    }
  }

  struct Write
  {
  private:
    std::fstream m_file;

  public:
    Write(const char *file_name) : m_file(file_name, std::ios::out)
    {
    }

    void write_line(const char *str)
    {
      if (m_file.is_open())
      {
        m_file << str << '\n';
      }
    }

    template <class... Args>
    void write_line(const char *str, Args &&...format_args)
    {
      std::string formatted = std::vformat(str, std::make_format_args(format_args...));
      write_line(formatted.c_str());
    }

    void write_lines(const std::vector<const char *> &strs)
    {
      for (const auto &s : strs)
      {
        write_line(s);
      }
    }

    ~Write()
    {
      if (m_file.is_open())
      {
        m_file.close();
        m_file = {};
      }
    }
  };

  struct Fake
  {
  };

}