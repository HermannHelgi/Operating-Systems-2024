#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct ListItem {
    struct ListItem *next;
    int value;
};

struct ListItem *listHead = NULL;

void appendItem(int valie) {
     // ... implement this
     // append at the end of the list
}

int removeFirstItem() {
    // implement this
    // removes the first item from the list and returns its value; returns -1 if list is empty
    return -1;
}

int containsItem(int value) {
    // implement this
    // return true (1) if list contains value, false (0) if not
    return 0;
}

int isEmpty() {
    // implement this
    // return true (1) if list is empty, false (0) otherweise
    return 0;
}

int main() {
    appendItem(42);
    appendItem(4711);
    removeFirstItem();
    appendItem(42);
    appendItem(4);
    for(int i=0; i<5; i++) printf("%d\n", removeFirstItem());
}
