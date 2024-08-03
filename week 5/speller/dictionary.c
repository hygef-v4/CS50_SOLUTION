// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int TABLE_SIZE = 50000;

// Hash table
node *table[TABLE_SIZE];

// Counter for the number of words loaded
unsigned int word_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Hash the word to get the index
    unsigned int index = hash(word);

    // Traverse the linked list at this index
    for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hashvalue = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        hashvalue += tolower(word[i]);
        hashvalue = (hashvalue * tolower(word[i])) % TABLE_SIZE;
    }
    return hashvalue;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open dictionary file.\n");
        return false;
    }

    // Read words from the file and insert into the hash table
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Allocate memory for new node
        node *newNode = malloc(sizeof(node));
        if (newNode == NULL)
        {
            fprintf(stderr, "Error: Could not allocate memory for node.\n");
            fclose(file);
            return false;
        }

        // Initialize the new node
        strcpy(newNode->word, word);
        newNode->next = NULL;

        // Get hash index
        unsigned int index = hash(word);

        // Insert node into the hash table
        newNode->next = table[index];
        table[index] = newNode;

        // Increment word count
        word_count++;
    }

    // Close the file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Free memory for each node in the hash table
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
