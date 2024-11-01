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
    Node * popped_node = temp->node;
    (*stack_head)->next = temp->next;
    free(temp);
    return popped_node;
}

struct Node * init_node(char * buffer) {
                // creating the node itself
            struct Node * new_node = malloc(sizeof(Node));
            if (new_node == NULL) {
                return NULL;
            }
            int cn, cx, cy;
            sscanf(buffer, "%d(%d,%d)", &cn, &cx, &cy);
            new_node->node_name = cn;
            new_node->width = cx;
            new_node->height = cy;
            new_node->org_x = 0;
            new_node->org_y = 0;
            new_node->left = NULL;
            new_node->right = NULL;
            return new_node;
}

void add_to_stack(Stack ** stack_head, Node * new_node) {
    struct Stack * new_stack = malloc(sizeof(Stack));
    if (new_stack == NULL) {
        return;
    }
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

    if (int_node == NULL) {
        return NULL;
    }

    int_node->node_name = l_h; 
    int_node->left = left_child;
    int_node->right = right_child;
    return int_node;
}

void preorder_print(Node * tree_node, FILE * fol) { // assuming that we dont care ab v and h
    if (tree_node == NULL) return;    
    if (tree_node->left == NULL && tree_node->right == NULL) {
        fprintf(fol, "%d(%d,%d)\n", tree_node->node_name, tree_node->width, tree_node->height);
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
        fprintf(fol, "%c(%d,%d)\n", tree_node->node_name, tree_node->width, tree_node->height);
    } else {
        fprintf(fol, "%d(%d,%d)\n", tree_node->node_name, tree_node->width, tree_node->height);
    }

    return;
}

int max(int num1, int num2)
{
    return (num1 > num2 ) ? num1 : num2;
}

int min(int num1, int num2)
{
    return (num1 > num2 ) ? num2 : num1;
}

struct Node * calculate_room_edges(Node * tree_node) { // O(3)
    if (tree_node->left == NULL && tree_node->right == NULL) {
        return tree_node; 
    }
     
    struct Node * node_left = calculate_room_edges(tree_node->left); 
    struct Node * node_right = calculate_room_edges(tree_node->right);

    if (tree_node->node_name == 'V') {
        tree_node->height = max(node_left->height, node_right->height);
        tree_node->width = (node_left->width + node_right->width);
    } else if (tree_node->node_name == 'H') {
        
        tree_node->width = max(node_left->width, node_right->width);
        tree_node->height = (node_left->height + node_right->height);
    } 

    return tree_node;
}

void print_dim_and_corner(Node * tree_node, FILE * fol) {
    if (tree_node->node_name != 'V' && tree_node->node_name != 'H') {
        fprintf(fol, "%d((%d,%d)(%d,%d))\n", tree_node->node_name, tree_node->width, tree_node->height, tree_node->org_x, tree_node->org_y);
    }
}

void box_and_room_coords(Node * tree_node, FILE * fol) {
    if(tree_node->left == NULL && tree_node->right == NULL) {
        return;
    }

    if (tree_node->node_name == 'V') {
    tree_node->left->org_y = tree_node->org_y;
    tree_node->right->org_x = tree_node->org_x + tree_node->left->width;

    } else if (tree_node->node_name == 'H') {
    tree_node->left->org_y = tree_node->org_y + tree_node->right->height;
    tree_node->right->org_x = tree_node->org_x;
    }

    tree_node->left->org_x = tree_node->org_x;
    tree_node->right->org_y = tree_node->org_y;

    box_and_room_coords(tree_node->left, fol);
    print_dim_and_corner(tree_node->left, fol);
    box_and_room_coords(tree_node->right, fol);
    print_dim_and_corner(tree_node->right, fol);

    return;
}

void print_from_stack(char * filename, Stack ** stack_head) {

    if ((*stack_head) == NULL || (*stack_head)->next == NULL) {
        return;
    }

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
    } else if (strcmp(ext, ".pck") == 0) {
        box_and_room_coords(tree_root, fol);
    }

    fclose(fol);
    return;
}


void buildTree(char * filename, Stack ** stack_head) {
    FILE * fol;
    fol = fopen(filename, "r");
    if (fol == NULL){
        return;
    }

    char buffer[10];
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
