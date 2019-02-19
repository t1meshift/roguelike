// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "win_cond_visitor.h"
namespace characters {
namespace visitors {
void win_cond_visitor::visit(Knight &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(Princess &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(Wall &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(Zombie &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(Dragon &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(AidKit &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(Fireball &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(Knight &a, Princess &b) {
  if (!a.is_dead()) won_ = true;
}
void win_cond_visitor::visit(Princess &a, Knight &b) {
  if (!a.is_dead()) won_ = true;
}
}
}