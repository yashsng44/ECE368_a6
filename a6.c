#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <regex.h>
#include "a6.h"


struct Node * pop_from_stack(Stack ** stack_head) {
    if (*stack_head == NULL) {
        return NULL;
    }
    Stack * temp = (*stack_head)->next;
    Node * popped_node = temp->node; // the next to the next value..
    // udpate the stack head 
    (*stack_head)->next = temp->next;
    free(temp);
    return popped_node;
}

struct Node * init_node(char * buffer) {
                // creating the node itself
            struct Node * new_node = malloc(sizeof(Node));
            int cn, cx, cy;
            sscanf(buffer, "%d(%d,%d)", &cn, &cx, &cy);
            new_node->node_name = cn;
            new_node->coord_x = cx;
            new_node->coord_y = cy;
            new_node->left = NULL;
            new_node->right = NULL;
            return new_node;
}

void add_to_stack(Stack ** stack_head, Node * new_node) {
            struct Stack * new_stack = malloc(sizeof(Stack));
            new_stack->node = new_node;
            new_stack->next = (*stack_head)->next;
            (*stack_head)->next = new_stack;
            return;
}

// Free the binary tree recursively
void free_tree(Node *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void free_stack(Stack * stack_head) {
    Stack * current = stack_head->next;
    while (current != NULL) {
        Stack *next_free = current->next;
        if (current->node != NULL) {
            free_tree(current->node);
        }
        free(current);
        current = next_free;
    }
}

struct Node * build_trio(Stack ** stack_head, char * buffer) {
            char l_h;
            sscanf(buffer, "%c", &l_h);
            struct Node * right_child = pop_from_stack(stack_head);
            struct Node * left_child = pop_from_stack(stack_head);
            struct Node * int_node = malloc(sizeof(Node));
            int_node->node_name = l_h;
            int_node->left = left_child;
            int_node->right = right_child;
            return int_node;
}

void buildTree(char * filename, Stack ** stack_head) {
    
    FILE * fol;
    fol = fopen(filename, "r");
    if (fol == NULL){
        return;
    }

    char buffer[20];

    while (fgets(buffer, sizeof(buffer), fol))
    {   
        if(buffer[0] == 'V' || buffer[0] == 'H') {
            Node * int_node = build_trio(stack_head, buffer);
            add_to_stack(stack_head, int_node);
        }
        else {
            Node * new_node = init_node(buffer);
            add_to_stack(stack_head, new_node);
        }
    }

    fclose(fol);
    return;
}

void preorder_print(Node * tree_node, FILE * fol) { // assuming that we dont care ab v and h
    if (tree_node == NULL) return;    
    if (tree_node->left == NULL && tree_node->right == NULL) {
        fprintf(fol, "%d(%d,%d)\n", tree_node->node_name, tree_node->coord_x, tree_node->coord_y);
    } else {
        fprintf(fol, "%c\n", tree_node->node_name);
    }

    preorder_print(tree_node->left, fol);
    preorder_print(tree_node->right, fol);
    return;
}

void postorder_print(Node * tree_node, FILE * fol) {
    
    if (tree_node == NULL) return;    
    postorder_print(tree_node->left, fol);
    postorder_print(tree_node->right, fol);
    if (tree_node->node_name == 'V' || tree_node->node_name == 'H') {
        fprintf(fol, "%c(%d,%d)\n", tree_node->node_name, tree_node->coord_x, tree_node->coord_y);
    } else {
        fprintf(fol, "%d(%d,%d)\n", tree_node->node_name, tree_node->coord_x, tree_node->coord_y);
    }

    return;
}

int max(int num1, int num2)
{
    return (num1 > num2 ) ? num1 : num2;
}

struct Node * calculate_room_edges(Node * tree_node) { // O(3)

    if (tree_node->left == NULL && tree_node->right == NULL) {
        return tree_node; // return the same node
    }
     
    struct Node * node_left = calculate_room_edges(tree_node->left); 
    struct Node * node_right = calculate_room_edges(tree_node->right);

    int x, y;

    if (tree_node->node_name == 'V') {
        y = max(node_left->coord_y, node_right->coord_y);
        x = (node_left->coord_x + node_right->coord_x);
    } else if (tree_node->node_name == 'H') {
        x = max(node_left->coord_x, node_right->coord_x);
        y = (node_left->coord_y + node_right->coord_y);
    } 

    tree_node->coord_x = x;
    tree_node->coord_y = y;

    return tree_node;
}

void print_from_stack(char * filename, Stack ** stack_head) {

    FILE * fol;
    fol = fopen(filename, "w");
    if (fol == NULL) {
        return;
    }

    Node * tree_root = (*stack_head)->next->node;
    char *ext = strrchr(filename, '.');  // Find the last '.' in the filename
    if (strcmp(ext, ".pr") == 0) {
    preorder_print(tree_root, fol);
    } else if (strcmp(ext, ".dim") == 0) {
    calculate_room_edges(tree_root); // fills in the edge values..
    postorder_print(tree_root, fol);
    } 

    fclose(fol);
    return;
}

