#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

typedef struct node
{
    char* word;
    struct node* left, *right;
} node;

node* newnode(char* x)
{
    node* n = malloc(sizeof(node));
    n->left = n->right = NULL;
    n->word = strdup(x);
    return n;
}

node* insert(node* root, char* x)
{
    if (root == NULL)
    {
        node* n = newnode(x);
        root = n;
    }
     else if (strcasecmp(root->word, x) > 0)
    {
        root->left = insert(root->left, x);
    }
    else
    {
        root->right = insert(root->right, x);
    }

    return root;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int height(node* root) {
    if (root == NULL)
    {
        return -1;
    }
    else
    {
        return 1 + max(height(root->left), height(root->right));
    }
}

node* LoadDictionary()
{
    FILE* file;
    file = fopen("Dictionary.txt", "r");

    if (file == NULL)
    {
        printf("Error Opening The Dictionary.\n");
        return NULL;
    }
    else
    {
        printf("Dictionary Loaded Successfully...!\n");
        printf("..................................\n");
    }

    int count = 0;
    char str[30];
    node* root = NULL;
    while (fgets(str, sizeof(str), file) != NULL)
    {
        str[strcspn(str, "\n")] = 0;
        root = insert(root, str);
        count++;
    }
    printf("SIZE = %d\n", count);
    printf("..................................\n");
    printf("Height = %d\n", height(root));
    printf("..................................\n");
    fclose(file);
    return root;
}

node* search(node* root, char* x, node** lastReached)
 {
    node* current = root;
    *lastReached = NULL;

    while (current != NULL)
    {
        int cmp = strcasecmp(current->word, x);
        if (cmp == 0)
        {
            return current;
        }
        else
         {
            *lastReached = current;
            if (cmp > 0)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
    }

    return NULL;
}

node* findMax(node* root)
 {
    if (root == NULL) return NULL;

    while (root->right != NULL)
    {
        root = root->right;
    }
    return root;
}

node* findMin(node* root)
{
    if (root == NULL) return NULL;

    while (root->left != NULL)
    {
        root = root->left;
    }
    return root;
}

node* predecessor(node* root, node* current)
{
    if (current == NULL) return NULL;

    if (current->left != NULL)
    {
        return findMax(current->left);
    }
    else
     {
        node* pred = NULL;
        node* ancestor = root;
        while (ancestor != current)
        {
            if (strcasecmp(current->word, ancestor->word) > 0)
            {
                pred = ancestor;
                ancestor = ancestor->right;
            }
            else
             {
                ancestor = ancestor->left;
            }
        }
        return pred;
    }
}

node* successor(node* root, node* current)
 {
    if (current == NULL) return NULL;

    if (current->right != NULL)
    {
        return findMin(current->right);
    }
    else
     {
        node* succ = NULL;
        node* ancestor = root;
        while (ancestor != current) {
            if (strcasecmp(current->word, ancestor->word) < 0)
            {
                succ = ancestor;
                ancestor = ancestor->left;
            }
            else
            {
                ancestor = ancestor->right;
            }
        }
        return succ;
    }
}

int main() {
    node* root = LoadDictionary();
    if (root == NULL)
    {
        return 1;
    }

    char str[100];
    printf("Enter a sentence: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = 0;
    printf("\n..................................\n");

    char* token = strtok(str, " ");
    while (token != NULL)
    {
        node* lastReached = NULL;
        node* n = search(root, token, &lastReached);
        if (n == NULL)
        {
            printf("%s - INCORRECT, suggestions: ", token);
            printf("%s,",lastReached->word);
            node* pred = predecessor(root, lastReached);
            node* succ = successor(root, lastReached);
            if (pred != NULL) {
                printf("%s,", pred->word);
            } else
            {
                printf("No predecessor, ");
            }
            if (succ != NULL)
            {
                printf("%s.\n", succ->word);
            } else
            {
                printf("No successor.\n");
            }
           }
        else
        {
            printf("%s - CORRECT\n", token);
        }
        token = strtok(NULL, " ");
    }

    return 0;
}
