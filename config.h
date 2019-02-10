#pragma once
#include "config/characters.h"

class ConfigRoot {
 public:
  explicit ConfigRoot(const std::string &filename_config);
  ConfigRoot();
  void save();
  CharactersConfig &characters();
 private:
  CharactersConfig characters_;
  // TODO other config values

  std::string filename_config_;
  void read(const configuru::Config &src);
};

class Config {
 public:
  static ConfigRoot &get();
  static void init();
 private:
  inline static std::shared_ptr<ConfigRoot> conf_ = nullptr;
};