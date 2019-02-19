#pragma once
#include "../characters.h"

namespace characters {
namespace visitors {
class win_cond_visitor : public base_visitor {
 public:
  bool won() {
    return won_;
  }
  void visit(Knight &a, Character &b) override;
  void visit(Wall&, Character&) override;
  void visit(Zombie&, Character&) override;
  void visit(Dragon&, Character&) override;
  void visit(AidKit&, Character&) override;
  void visit(Fireball&, Character&) override;

  void visit(Princess &a, Character &b) override;
  void visit(Knight &a, Princess &b) override;
  void visit(Princess &a, Knight &b) override;
 private:
  bool won_ = false;
};
}
}