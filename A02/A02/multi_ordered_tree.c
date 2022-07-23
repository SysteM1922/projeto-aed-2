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

//
// list, i,e, traverse the tree (place your code here)
//

void list(tree_node_t **root, int ind, int *np)
{
    if (root[ind] != NULL)
    {
        list(root[ind]->left, ind, np);
        printf("%-13d%-20s %-20s %-25s %-10s\n", *np, root[ind]->name, root[ind]->telephone_number,
               root[ind]->social_number, root[ind]->zip_code);
        *np = *np + 1;
        list(root[ind]->right, ind, np);
    }
}

void searchF(tree_node_t **root, int ind, int *np, char *search, int flag)
{
    if (root[ind] != NULL)
    {
        searchF(root[ind]->left, ind, np, search, flag);

        //NAME
        if (flag == 0)
        {
            char tempName[MAX_NAME_SIZE];
            strcpy(tempName, root[ind]->name);
            //guarda o nome completo na var tempName
            char *token = strtok(tempName, " ");
            int i = 0;
            while (token[i])
            {
                if (token[i] == '\n')
                {
                    token[i] = '\0';
                    break;
                }
                i++;
            }

            if ((strcmp(search, token)) == 0 || (strcmp(search, root[ind]->name)) == 0)
            {
                printf("%-13d%-20s %-20s %-25s %-10s\n", *np, root[ind]->name, root[ind]->telephone_number,
                       root[ind]->social_number, root[ind]->zip_code);
                *np = *np + 1;
            }
        }

        //ZIP CODE
        else if (flag == 1)
        {
            if ((strcmp(search, root[ind]->zip_code)) == 0)
            {
                printf("%-13d%-20s %-20s %-25s %-10s\n", *np, root[ind]->name, root[ind]->telephone_number,
                       root[ind]->social_number, root[ind]->zip_code);
                *np = *np + 1;
            }
        }

        //Phone Number
        else if (flag == 2)
        {
            if ((strcmp(search, root[ind]->telephone_number)) == 0)
            {
                printf("%-13d%-20s %-20s %-25s %-10s\n", *np, root[ind]->name, root[ind]->telephone_number,
                       root[ind]->social_number, root[ind]->zip_code);
                *np = *np + 1;
            }
        }

        //Security Number
        else if (flag == 3)
        {
            if ((strcmp(search, root[ind]->social_number)) == 0)
            {
                printf("%-13d%-20s %-20s %-25s %-10s\n", *np, root[ind]->name, root[ind]->telephone_number,
                       root[ind]->social_number, root[ind]->zip_code);
                *np = *np + 1;
            }
        }

        searchF(root[ind]->right, ind, np, search, flag);
    }
}


int nodeCounter(tree_node_t **root, int cind, int lvl, int idx) {

    if (root[idx] == NULL)
    {
        return 0;
    }
    if (cind == lvl)
        return 1;

    return nodeCounter(root[idx] -> left, cind + 1, lvl, idx) +
    nodeCounter(root[idx] -> right, cind + 1, lvl, idx);

}



int main(int argc, char **argv)
{
    double dt;

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
    int n_persons = atoi(argv[2]);
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
    printf("Tree creation time (%d persons): %.3es\n", n_persons, dt);

    //

    // search the tree
    for (int main_index = 0; main_index < 4; main_index++)
    {
        dt = cpu_time();
        for (int i = 0; i < n_persons; i++)
        {
            tree_node_t n = persons[i];                      // make a copy of the node data
            if (find(n, main_index, roots) != &(persons[i])) // place your code here to find a given person, searching for it using the tree with number main_index
            {
                fprintf(stderr, "person %d not found using index %d\n", i, main_index);
                return 1;
            }
        }
        dt = cpu_time() - dt;
        printf("Tree search time (%d persons, index %d): %.3es\n", n_persons, main_index, dt);
    }

    //

    // compute the largest tree depdth
    for (int main_index = 0; main_index < 4; main_index++)
    {
        dt = cpu_time();

        int depth = tree_depth(roots, main_index); // place your code here to compute the depth of the tree with number main_index

        dt = cpu_time() - dt;
        printf("Tree depth for index %d: %d (done in %.3es)\n", main_index, depth, dt);
    }

    //
    
    //node counter
    int currLvl = 0;
    for (int main_index = 0; main_index < 4; main_index++)
    {
        int nNodes = 0;
        int lvl = 0;

        int depth = tree_depth(roots, main_index);
        printf("\nPara o índice %d:\n",main_index);

        for (int i = 0; i < depth; i++) {
            nNodes = nodeCounter(roots, currLvl, lvl, main_index);
            //printf("lvl = %d -> %d\n", lvl, nNodes);

            printf("\tNível (%d) -> %d nodes!\n", lvl, nNodes);

            lvl ++;
        }
        printf("\n");
    }

    // process the command line optional arguments
    for (int i = 3; i < argc; i++)
    {
        if (strncmp(argv[i], "-list", 5) == 0)
        { // list all (optional)
            int main_index = atoi(&(argv[i][5]));
            if (main_index < 0)
                main_index = 0;
            if (main_index >= 3)
                main_index = 3;
            int np = 1;
            printf("\n\nList of persons:\n\n");
            printf("%-13s %-20s %-20s %-25s %-10s\n", "Nº", "Name", "Telephone Number", "Social Sec. Number", "Zip Code");
            list(roots, main_index, &np); // place your code here to traverse, in order, the tree with number main_index
            printf("\n");
        }

        //search OPT
        if (strncmp(argv[i], "-search", 7) == 0)
        { // list all (optional)
            int main_index = atoi(&(argv[i][7]));
            if (main_index < 0)
                main_index = 0;
            if (main_index >= 3)
                main_index = 3;

            if (main_index == 0)
            {
                printf("A procurar por nome (1st name or full name)...\n");

                char nameSearch[MAX_NAME_SIZE];
                printf("Enter the name you want to search for: ");
                // read a string
                fgets(nameSearch, sizeof(nameSearch), stdin);
                int i = 0;
                while (nameSearch[i])
                {
                    if (nameSearch[i] == '\n')
                    {
                        nameSearch[i] = '\0';
                        break;
                    }
                    i++;
                }
                printf("\n\nList of persons with '%s' (name):\n\n", nameSearch);
                printf("%-13s %-20s %-20s %-25s %-10s\n", "Nº", "Name", "Telephone Number", "Social Sec. Number", "Zip Code");
                int np = 1;
                searchF(roots, main_index, &np, nameSearch, main_index);
            }

            if (main_index == 1)
            {
                printf("A procurar por zip code...\n");

                char zipCodeSearch[MAX_ZIP_CODE_SIZE];
                printf("Enter the zipCode you want to search for: ");
                // read a string
                fgets(zipCodeSearch, sizeof(zipCodeSearch), stdin);
                int i = 0;
                while (zipCodeSearch[i])
                {
                    if (zipCodeSearch[i] == '\n')
                    {
                        zipCodeSearch[i] = '\0';
                        break;
                    }
                    i++;
                }
                printf("\n\nList of persons with '%s' (zipCode):\n\n", zipCodeSearch);
                printf("%-13s %-20s %-20s %-25s %-10s\n", "Nº", "Name", "Telephone Number", "Social Sec. Number", "Zip Code");
                int np = 1;
                searchF(roots, main_index, &np, zipCodeSearch, main_index);
            }

            if (main_index == 2)
            {
                printf("A procurar por phone Number...\n");

                char phoneNumber[MAX_ZIP_CODE_SIZE];
                printf("Enter the Phone Number you want to search for: ");
                // read a string
                fgets(phoneNumber, sizeof(phoneNumber), stdin);
                int i = 0;
                while (phoneNumber[i])
                {
                    if (phoneNumber[i] == '\n')
                    {
                        phoneNumber[i] = '\0';
                        break;
                    }
                    i++;
                }
                printf("\n\nList of persons with '%s' phoneNumber:\n\n", phoneNumber);
                printf("%-13s %-20s %-20s %-25s %-10s\n", "Nº", "Name", "Telephone Number", "Social Sec. Number", "Zip Code");
                int np = 1;
                searchF(roots, main_index, &np, phoneNumber, main_index);
            }
            if (main_index == 3)
            {
                printf("A procurar por Security Number...\n");

                char securityNumber[MAX_ZIP_CODE_SIZE];
                printf("Enter the Security Number you want to search for: ");
                // read a string
                fgets(securityNumber, sizeof(securityNumber), stdin);
                int i = 0;
                while (securityNumber[i])
                {
                    if (securityNumber[i] == '\n')
                    {
                        securityNumber[i] = '\0';
                        break;
                    }
                    i++;
                }
                printf("\n\nList of persons with '%s' (securityNumber):\n\n", securityNumber);
                printf("%-13s %-20s %-20s %-25s %-10s\n", "Nº", "Name", "Telephone Number", "Social Sec. Number", "Zip Code");
                int np = 1;
                searchF(roots, main_index, &np, securityNumber, main_index);
            }
        }
        // place your own options here
    }

    //

    // clean up --- don't forget to test your program with valgrind, we don't want any memory leaks
    free(persons);

    return 0;
}
