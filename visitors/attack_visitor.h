#pragma once
#include "../characters.h"

namespace characters {
namespace visitors {
class attack_visitor : public base_visitor {
 public:
  void visit(Knight &a, Character &b) override;
  void visit(Princess &a, Character &b) override;
  void visit(Wall&, Character&) override;
  void visit(Zombie&, Character&) override;
  void visit(Dragon&, Character&) override;
  void visit(AidKit&, Character&) override;
  void visit(Fireball&, Character&) override;

  void visit(Knight &a, AidKit &b) override;
  void visit(Princess &a, AidKit &b) override;
  void visit(Zombie &a, AidKit &b) override;
  void visit(Dragon &a, AidKit &b) override;
  void visit(AidKit &a, Knight &b) override;
  void visit(AidKit &a, Princess &b) override;
  void visit(AidKit &a, Zombie &b) override;
  void visit(AidKit &a, Dragon &b) override;

  void visit(Fireball &a, Knight &b) override;
  void visit(Fireball &a, Princess &b) override;
  void visit(Fireball &a, Zombie &b) override;
  void visit(Fireball &a, Dragon &b) override;
  void visit(Fireball &a, AidKit &b) override;
  void visit(Fireball &a, Fireball &b) override;
  void visit(Knight &a, Fireball &b) override;
  void visit(Princess &a, Fireball &b) override;
  void visit(Zombie &a, Fireball &b) override;
  void visit(Dragon &a, Fireball &b) override;
  void visit(AidKit &a, Fireball &b) override;
 private:
  bool done_ = false;
};
}
}