#ifndef CHANGES_MANAGER_H
#define CHANGES_MANAGER_H

#define SIZE_INCREMENT 5

typedef enum {
  ACTION_EDIT,
  ACTION_DELETE,
  ACTION_INSERT,
  ACTION_MERGE,
  ACTION_SPLIT
} ChangeAction;

typedef struct {
  ChangeAction action;
  int y_start;
  int y_end;
  char *content;
} Change;

void init_changes_management();

void append_change(Change *change);

Change remove_last_change();

void clear_changes();

#endif
