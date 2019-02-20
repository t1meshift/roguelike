// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "wall_visitor.h"

namespace characters {
namespace visitors {
void wall_visitor::visit(Knight &a, Character &b) {
  b.accept(*this, a);
}
void wall_visitor::visit(Princess &a, Character &b) {
  b.accept(*this, a);
}
void wall_visitor::visit(Wall &a, Character &b) {
  collided_ = true;
}
void wall_visitor::visit(Zombie &a, Character &b) {
  b.accept(*this, a);
}
void wall_visitor::visit(Dragon &a, Character &b) {
  b.accept(*this, a);
}
void wall_visitor::visit(AidKit &a, Character &b) {
  b.accept(*this, a);
}
void wall_visitor::visit(Fireball &a, Character &b) {
  b.accept(*this, a);
}
void wall_visitor::visit(Knight &a, Wall &b) {
  collided_ = true;
}
void wall_visitor::visit(Princess &a, Wall &b) {
  collided_ = true;
}
void wall_visitor::visit(Zombie &a, Wall &b) {
  collided_ = true;
}
void wall_visitor::visit(Dragon &a, Wall &b) {
  collided_ = true;
}
void wall_visitor::visit(Wall &a, Wall &b) {
  collided_ = true;
}
void wall_visitor::visit(Wall &a, Knight &b) {
  collided_ = true;
}
void wall_visitor::visit(Wall &a, Princess &b) {
  collided_ = true;
}
void wall_visitor::visit(Wall &a, Zombie &b) {
  collided_ = true;
}
void wall_visitor::visit(Wall &a, Dragon &b) {
  collided_ = true;
}
void wall_visitor::visit(Fireball &a, Wall &b) {
  collided_ = true;
  a.hp(0);
}
void wall_visitor::visit(Wall &a, Fireball &b) {
  collided_ = true;
  b.hp(0);
}
bool wall_visitor::collided() const {
  return collided_;
}
}
}