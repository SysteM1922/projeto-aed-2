//
// AED, January 2022
//
// Solution of the second practical assignement (multi-ordered tree)
//
// Place your student numbers and names here
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AED_2021_A02.h"
#include "elapsed_time.h"
//
// the custom tree node structure
//

// we want to maintain three ordered trees (using the same nodes!), so we need three left and three right pointers
// so, when inserting a new node we need to do it three times (one for each index), so we will end upo with 3 three roots
//

typedef struct tree_node_s
{
    char name[MAX_NAME_SIZE + 1];                         // index 0 data item
    char zip_code[MAX_ZIP_CODE_SIZE + 1];                 // index 1 data item
    char telephone_number[MAX_TELEPHONE_NUMBER_SIZE + 1]; // index 2 data item
    char social_number[MAX_SOCIAL_SECURITY_NUMBER + 1];   // index 3 data item
    struct tree_node_s *left[4];                          // left pointers (one for each index) ---- left means smaller
    struct tree_node_s *right[4];                         // right pointers (one for each index) --- right means larger
} tree_node_t;

//
// the node comparison function (do not change this)
//

int compare_tree_nodes(tree_node_t *node1, tree_node_t *node2, int main_idx)
{
    int i, c;

    for (i = 0; i < 4; i++)
    {
        if (main_idx == 0)
            c = strcmp(node1->name, node2->name);
        else if (main_idx == 1)
            c = strcmp(node1->zip_code, node2->zip_code);
        else if (main_idx == 2)
            c = strcmp(node1->telephone_number, node2->telephone_number);
        else
            c = strcmp(node1->social_number, node2->social_number);
        if (c != 0)
            return c;                                  // different on this index, so return
        main_idx = (main_idx == 3) ? 0 : main_idx + 1; // advance to the next index
    }
    return 0;
}

//
// tree insertion routine (place your code here)
//

void tree_insert(tree_node_t *node, int ind, tree_node_t **roots)
{
    //if roots are null (tree is empty), then the node is the root
    if (roots[ind] == NULL)
    {
        roots[ind] = node;
        return;
    }
    if (compare_tree_nodes(node, roots[ind], ind) < 0) //recurs down the tree if tree isn't empty
        tree_insert(node, ind, roots[ind]->left);
    else if (compare_tree_nodes(node, roots[ind], ind) > 0)
        tree_insert(node, ind, roots[ind]->right);
    return;
}

//
// tree search routine (place your code here)
//

tree_node_t *find(tree_node_t node, int ind, tree_node_t **roots)
{
    //if roots are null (tree is empty) || node to find is root (ig)
    if (roots[ind] == NULL || (compare_tree_nodes(&node, roots[ind], ind) == 0))
    {
        return roots[ind];
    }

    //left
    else if (compare_tree_nodes(&node, roots[ind], ind) < 0) //recurs down the tree if tree isn't empty
        return find(node, ind, roots[ind]->left);

    //right -> (compare_tree_nodes(&node, roots[ind], ind) > 0)
    else
        return find(node, ind, roots[ind]->right);
}

//
// tree depdth
//

int tree_depth(tree_node_t **roots, int idx)
{
    //tree non existent
    if (roots[idx] == NULL)
        return -1;
    else
    {
        //calcular rec para a esq e para a direita da root
        int left = tree_depth(roots[idx]->left, idx);
        int right = tree_depth(roots[idx]->right, idx);

        //ver qual é maior, e somar 1, para calcular a depth total
        if (left > right)
            return left + 1;
        else
            return right + 1;
    }
}

// main program
//

void calcuDepth(int main_index, int n_persons, tree_node_t **roots)
{
    int depth = tree_depth(roots, main_index); // place your code here to compute the depth of the tree with number main_index
    printf("%d %d\n", n_persons, depth);
}

void calcuFind(int main_index, int n_persons, tree_node_t **roots, tree_node_t *persons)
{

    double dt = cpu_time();
    for (int i = 0; i < n_persons; i++)
    {
        tree_node_t n = persons[i];                      // make a copy of the node data
        if (find(n, main_index, roots) != &(persons[i])) // place your code here to find a given person, searching for it using the tree with number main_index
        {
            fprintf(stderr, "person %d not found using index %d\n", i, main_index);
            return;
        }
    }
    dt = cpu_time() - dt;
    printf("%d %.3f\n", n_persons, dt);
}

int main(int argc, char **argv)
{
    double dt;
    int arryPeople[] = {10, 21, 46, 100, 215, 464, 1000, 2154, 4641, 10000, 21544, 46415, 100000, 215443, 464158, 1000000, 2154434, 4641588, 10000000};
    int nP = 19;

    // process the command line arguments
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s student_number number_of_persons [options ...]\n", argv[0]);
        fprintf(stderr, "Recognized options:\n");
        fprintf(stderr, "  -list[N]              # list the tree contents, sorted by key index N (the default is index 0)\n");
        fprintf(stderr, "  -search[N]            # search the tree contents, sorted by key index N (the default is index 0)\n");
        fprintf(stderr, "\t  N = 0 -> Name\n\t  N = 1 -> Zip Code\n\t  N = 2 -> Phone Number\n\t  N = 3 -> Security Number\n");
        // place a description of your own options here
        return 1;
    }
    int student_number = atoi(argv[1]);
    if (student_number < 1 || student_number >= 1000000)
    {
        fprintf(stderr, "Bad student number (%d) --- must be an integer belonging to [1,1000000{\n", student_number);
        return 1;
    }

    for (int n = 0; n < nP; n++)
    {

        int n_persons = arryPeople[n];
        if (n_persons < 3 || n_persons > 10000000)
        {
            fprintf(stderr, "Bad number of persons (%d) --- must be an integer belonging to [3,10000000]\n", n_persons);
            return 1;
        }

        // generate all data
        tree_node_t *persons = (tree_node_t *)calloc((size_t)n_persons, sizeof(tree_node_t));
        if (persons == NULL)
        {
            fprintf(stderr, "Output memory!\n");
            return 1;
        }
        aed_srandom(student_number);
        for (int i = 0; i < n_persons; i++)
        {
            random_name(&(persons[i].name[0]));
            random_zip_code(&(persons[i].zip_code[0]));
            random_telephone_number(&(persons[i].telephone_number[0]));
            random_social_number(&(persons[i].social_number[0]));
            for (int j = 0; j < 3; j++)
                persons[i].left[j] = persons[i].right[j] = NULL; // make sure the pointers are initially NULL
        }

        // create the ordered binary trees
        dt = cpu_time();
        tree_node_t *roots[4]; // four indices, four roots
        for (int main_index = 0; main_index < 4; main_index++)
            roots[main_index] = NULL;
        for (int i = 0; i < n_persons; i++)
            for (int main_index = 0; main_index < 4; main_index++)
                tree_insert(&(persons[i]), main_index, roots); // place your code here to insert &(persons[i]) in the tree with number main_index
        dt = cpu_time() - dt;
        //printf("%d %.3es\n", n_persons, dt);

        //

    
        // search the tree
        for (int main_index = 0; main_index < 4; main_index++)
        {
            //printf("Indice %d\n", main_index);
            calcuFind(main_index, n_persons, roots, persons);
        }

        //

        // compute the largest tree depdth
        for (int main_index = 0; main_index < 4; main_index++)
        {
            //printf("Indice %d\n", main_index);
            //calcuDepth(main_index, n_persons, roots);
        }

        printf("\n");
        // clean up --- don't forget to test your program with valgrind, we don't want any memory leaks
        free(persons);
    }
    return 0;
}
