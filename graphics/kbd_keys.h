#pragma once
// export key codes
#define KEY_(name, code) const int Key::name = (code)

namespace Key {
  extern const int NOTHING;
  extern const int ARROW_UP;
  extern const int ARROW_DOWN;
  extern const int ARROW_LEFT;
  extern const int ARROW_RIGHT;
  extern const int THROW_UP;
  extern const int THROW_DOWN;
  extern const int THROW_LEFT;
  extern const int THROW_RIGHT;
}