#include "changes_manager.h"
#include <stdlib.h>

long capacity = 5;
long size = 0;

Change *changes;

void init_changes_management() {
  changes = malloc(capacity * sizeof(Change));
  if (changes == NULL) {
    exit(1);
  }
}

void append_change(Change *change) {
  if (size == capacity || size + 1 == capacity) {
    long new_capacity = capacity + SIZE_INCREMENT;

    Change *temp = realloc(changes, new_capacity * sizeof(Change));
    if (temp == NULL) {
      exit(1);
    }
    changes = temp;
    capacity = new_capacity;
  }

  changes[size++] = *change;
}

Change remove_last_change() {
  if (size > 0) {
    size--;
    return changes[size];
  }

  Change empty;
  empty.content = NULL;
  empty.y_start = 0;
  empty.y_end = 0;
  return empty;
}

void clear_changes() {
  free(changes);
  changes = NULL;
  size = 0;
  capacity = 5;
}
