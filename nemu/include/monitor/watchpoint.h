#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char expr[256];
  bool empty;
  int key;
} WP;
void init_wp_pool();
void print_wp();
WP* new_wp();
void free_wp(WP *wp);
WP* wphead();
WP* wpfree_();
#endif
