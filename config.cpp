// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "config.h"

#include <filesystem>
#include <fstream>
#include <configuru.hpp>
#include <cmrc/cmrc.hpp>

namespace fs = std::filesystem;
namespace cfg = configuru;

CMRC_DECLARE(rcfs);

cfg::Config get_default_config() {
  auto rcfs = cmrc::rcfs::get_filesystem();
  auto default_config_file = rcfs.open("default_config.json");
  std::string default_config_string(default_config_file.begin(), default_config_file.end());
  return cfg::parse_string(default_config_string.c_str(), cfg::JSON, "default_config.json");
}

ConfigRoot::ConfigRoot(const std::string &filename_config) : filename_config_(filename_config){
  fs::path f(filename_config);
  if (!fs::exists(f)) {
    read(get_default_config());
    save();
  } else {
    read(cfg::parse_file(filename_config, cfg::JSON));
  }
}

ConfigRoot::ConfigRoot() : ConfigRoot("config.json") {}

void ConfigRoot::read(const configuru::Config &src) {
  for (auto character : src["characters"].as_object()) {
    CharacterConfig c{};
    // This line won't cover because configuru wouldn't even call
    // "const ConversionError& on_error" in any case
    cfg::deserialize(&c, character.value(), [](std::string){});
    characters_.add_character(character.key(), c);
  }
}

CharactersConfig &ConfigRoot::characters() {
  return characters_;
}

void ConfigRoot::save() {
  auto config_root = cfg::Config::object();
  config_root.emplace("characters", characters_.state());
  cfg::dump_file(filename_config_, config_root, cfg::JSON);
}

ConfigRoot &Config::get() {
  if (conf_ == nullptr) {
    conf_ = std::make_shared<ConfigRoot>();
  }
  return *conf_;
}
void Config::init() {
  conf_.reset();
  conf_ = std::make_shared<ConfigRoot>();
}
