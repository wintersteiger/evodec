#include <iostream>
#include <fstream>
#include <set>
#include <string>

#include "devices.h"
#include "decode.h"
#include "encode.h"

class Options {
  public:
  Options() : format("hgi80") {}
  std::string format;
  std::set<std::string> input_files;
};

static bool next_arg(const char *arg, int &i, int argc, const char **argv, std::string &value)
{
  if (std::string(argv[i]) == arg) {
    if (i + 1 >= argc) {
      std::cout << "parameter to '" << arg << "' missing" << std::endl;
      return false;
    }
    value = argv[++i];
    return true;
  }
  return false;
}

static bool next_arg(const char *arg, int &i, int argc, const char **argv, std::set<std::string> &values)
{
  std::string t;
  bool r = next_arg(arg, i, argc, argv, t);
  if (r)
    values.insert(t);
  return r;
}

static void recode(std::istream &instream, const std::string &fmt)
{
  std::string line;
  while (std::getline(instream, line))
  {
    Message msg;

    try {
      msg = decode(line);
    }
    catch (std::exception &ex) {
      std::cout << "Error decoding '" << line << "': " << ex.what() << std::endl;
      continue;
    }

    std::string (*encode_fun)(const Message &msg);
    if (fmt == "raw")
      encode_fun = encode_raw;
    else if (fmt == "hgi80")
      encode_fun = encode_hgi80;
    else if (fmt == "json")
      encode_fun = encode_json;
    else {
      std::cout << "unknown format '" << fmt << "'" << std::endl;
      break;
    }

    try {
      std::string encoded;
      encoded = encode_fun(msg);

      if (fmt == "raw") {
        std::cout << encoded;
        if (line != encoded)
          std::cout << " != " << line;
        std::cout << std::endl;
      }
      else
        std::cout << encoded << std::endl;
    }
    catch (std::exception &ex) {
      std::cout << "Error encoding message: " << ex.what() << std::endl;
      continue;
    }
  }
}

int main(int argc, const char **argv)
{
  Options options;

  for (int i=1; i < argc; i++) {
    if (next_arg("-f", i, argc, argv, options.format) ||
        next_arg("-i", i, argc, argv, options.input_files))
      continue;
    else
      options.input_files.insert(argv[i]);
  }

  if (options.input_files.empty())
    recode(std::cin, options.format);
  else
  {
    for (auto fn : options.input_files) {
      std::cout << fn << ":" << std::endl;
      std::ifstream is(fn);
      recode(is, options.format);
    }
  }

  return 0;
}