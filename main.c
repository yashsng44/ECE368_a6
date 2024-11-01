#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>
#include <math.h>
#include "a6.h"

int main(int argc, char ** argv) {

if (argc != 5)
    {
      return EXIT_FAILURE;
}

Stack * stack_head = malloc(sizeof(Stack));
buildTree(argv[1], &stack_head); // builds tree from the input tree in post order...
print_from_stack(argv[2], &stack_head);
print_from_stack(argv[3], &stack_head);
print_from_stack(argv[4], &stack_head);
// give me a second

free_stack(stack_head);
free(stack_head);
return EXIT_SUCCESS;
}
    
