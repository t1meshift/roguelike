#pragma once
#include "../characters.h"

namespace characters {
namespace visitors {
class wall_visitor : public base_visitor {
 public:
  virtual bool collided() const;
  void visit(Knight &a, Character &b) override;
  void visit(Princess &a, Character &b) override;
  void visit(Wall&, Character&) override;
  void visit(Zombie&, Character&) override;
  void visit(Dragon&, Character&) override;
  void visit(AidKit&, Character&) override;
  void visit(Fireball&, Character&) override;

  void visit(Knight &a, Wall &b) override;
  void visit(Princess &a, Wall &b) override;
  void visit(Zombie &a, Wall &b) override;
  void visit(Dragon &a, Wall &b) override;
  void visit(Wall &a, Wall &b) override;
  void visit(Wall &a, Knight &b) override;
  void visit(Wall &a, Princess &b) override;
  void visit(Wall &a, Zombie &b) override;
  void visit(Wall &a, Dragon &b) override;
  void visit(Fireball &a, Wall &b) override;
  void visit(Wall &a, Fireball &b) override;
 private:
  bool collided_ = false;
};
}
}