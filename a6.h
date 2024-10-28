#ifndef a6_h
#define a6_h 
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>
#include <math.h>


typedef struct Node
{ 
    char node_name; // print out the ASCII representation of the numbers and characters
    int coord_x; 
    int coord_y;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct Stack
{ 
    struct Node * node;
    struct Stack * next;
} Stack;

void buildTree(char * filename, Stack ** stack_head);
void print_from_stack(char * filename, Stack ** stack_head);
void free_stack(Stack * stack_head);
#endif