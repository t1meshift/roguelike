// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#define CATCH_CONFIG_RUNNER
#define CONFIGURU_IMPLEMENTATION 1
#include <catch2/catch.hpp>
#include <configuru.hpp>
#include "../graphics.h"

int main( int argc, char* argv[] ) {
  graphics::init(); // This line is needed because scene tests require graphics module

  int result = Catch::Session().run( argc, argv );

  return result;
}