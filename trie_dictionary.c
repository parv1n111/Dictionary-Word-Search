//Task 14 - Dictionary Word Search
//Dictionary implemented using a Trie (prefix tree)
//Operations: insert, search, prefix search (autocomplete), display all, delete
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h> //to validate letters and convert uppercase characters to lowercase

#define ALPHABET_SIZE 26
#define MAX_WORD_LEN 100

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
} TrieNode;

/* Create a new node */
TrieNode *createNode(void) {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    node->isEndOfWord = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    return node;
}

// Check word has only letters, and convert to lowercase
int normalize(char *word) {
    if (word[0] == '\0') return 0;
    for (int i = 0; word[i]; i++) {
        if (!isalpha((unsigned char)word[i])) return 0;
        word[i] = (char)tolower((unsigned char)word[i]);
    }
    return 1;
}

// Insert a word into the trie 
void insert(TrieNode *root, const char *word) {
    TrieNode *curr = root;
    for (int i = 0; word[i]; i++) {
        int idx = word[i] - 'a';
        if (curr->children[idx] == NULL)
            curr->children[idx] = createNode();
        curr = curr->children[idx];
    }
    curr->isEndOfWord = 1;
}

// Search for a complete word 
int search(TrieNode *root, const char *word) {
    TrieNode *curr = root;
    for (int i = 0; word[i]; i++) {
        int idx = word[i] - 'a';
        if (curr->children[idx] == NULL) return 0;
        curr = curr->children[idx];
    }
    return curr->isEndOfWord;
}

// Return node at end of prefix, or NULL 
TrieNode *findPrefix(TrieNode *root, const char *prefix) {
    TrieNode *curr = root;
    for (int i = 0; prefix[i]; i++) {
        int idx = prefix[i] - 'a';
        if (curr->children[idx] == NULL) return NULL;
        curr = curr->children[idx];
    }
    return curr;
}

// Print all words below a node (DFS) 
void printWords(TrieNode *node, char *buffer, int depth, int *count) {
    if (node->isEndOfWord) {
        buffer[depth] = '\0';
        printf("  %s\n", buffer);
        (*count)++;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            buffer[depth] = (char)('a' + i);
            printWords(node->children[i], buffer, depth + 1, count);
        }
    }
}

// Check if node has any children 
int hasChildren(TrieNode *node) {
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (node->children[i] != NULL) return 1;
    return 0;
}

// Delete a word; returns 1 if the node can be freed by the caller 
int deleteWord(TrieNode *node, const char *word, int depth, int *deleted) {
    if (node == NULL) return 0;

    if (word[depth] == '\0') {
        if (!node->isEndOfWord) return 0;
        node->isEndOfWord = 0;
        *deleted = 1;
        return !hasChildren(node);
    }

    int idx = word[depth] - 'a';
    if (deleteWord(node->children[idx], word, depth + 1, deleted)) {
        free(node->children[idx]);
        node->children[idx] = NULL;
        return !node->isEndOfWord && !hasChildren(node);
    }
    return 0;
}

// Free the whole trie 
void freeTrie(TrieNode *node) {
    if (node == NULL) return;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        freeTrie(node->children[i]);
    free(node);
}

void printMenu(void) {
    printf("\n===== TRIE DICTIONARY =====\n");
    printf("1. Insert word\n");
    printf("2. Search word\n");
    printf("3. Words starting with prefix\n");
    printf("4. Display all words\n");
    printf("5. Delete word\n");
    printf("6. Exit\n");
    printf("Enter choice: ");
}

int main(void) {
    TrieNode *root = createNode();
    char word[MAX_WORD_LEN];
    char buffer[MAX_WORD_LEN];
    int choice;

    /* Preload some sample words */
    const char *samples[] = {
        "apple", "app", "application", "apply", "arrow", "ant",
        "ball", "bat", "batch", "book", "bridge", "brain",
        "cat", "car", "card", "care", "code", "computer",
        "data", "dog", "database", "desk", "doctor", "dream",
        "eagle", "earth", "egg", "engine", "elephant", "exam",
        "fan", "fish", "file", "fire", "flower", "function",
        "game", "garden", "graph", "green", "gate", "guitar",
        "house", "hat", "hash", "heap", "hill", "hospital",
        "ice", "idea", "image", "input", "insert", "island",
        "jar", "java", "jacket", "juice", "jungle", "journey",
        "key", "kite", "king", "kitchen", "knife", "keyboard",
        "lamp", "list", "loop", "learn", "light", "library",
        "map", "memory", "mouse", "music", "matrix", "mountain",
        "node", "note", "number", "night", "network", "nest",
        "orange", "owl", "output", "ocean", "object", "oven",
        "pen", "pointer", "python", "queue", "program", "planet",
        "quiz", "queen", "question", "quick", "quilt", "quote",
        "rain", "root", "river", "recursion", "road", "robot",
        "stack", "sort", "search", "string", "sun", "school",
        "tree", "trie", "table", "tiger", "train", "tower",
        "umbrella", "union", "unit", "update", "user", "uncle",
        "variable", "vector", "village", "violin", "virus", "voice",
        "water", "web", "window", "word", "wind", "world",
        "xray", "xylophone", "xenon", "xerox", "xmas", "xenial",
        "yak", "yarn", "year", "yellow", "yoga", "yield",
        "zebra", "zero", "zone", "zoo", "zip", "zigzag"
    };
    int n = sizeof(samples) / sizeof(samples[0]);
    for (int i = 0; i < n; i++) insert(root, samples[i]);
    printf("Loaded %d sample words into the dictionary.\n", n);

    while (1) {
        printMenu();
        int result = scanf("%d", &choice);
        if (result == EOF) {
            /* input stream closed - nothing more to read */
            break;
        }
        if (result != 1) {
            printf("Invalid input! Please enter a number from 1 to 6.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ; /* discard the bad input so the menu can be shown again */
            continue;
        }

        switch (choice) {
        case 1:
            printf("Enter word to insert: ");
            scanf("%99s", word);
            if (!normalize(word)) {
                printf("Only alphabetic words are allowed.\n");
            } else if (search(root, word)) {
                printf("'%s' already exists.\n", word);
            } else {
                insert(root, word);
                printf("'%s' inserted successfully.\n", word);
            }
            break;

        case 2:
            printf("Enter word to search: ");
            scanf("%99s", word);
            if (!normalize(word)) {
                printf("Only alphabetic words are allowed.\n");
            } else if (search(root, word)) {
                printf("'%s' FOUND in dictionary.\n", word);
            } else {
                printf("'%s' NOT found in dictionary.\n", word);
            }
            break;

        case 3: {
            printf("Enter prefix: ");
            scanf("%99s", word);
            if (!normalize(word)) {
                printf("Only alphabetic prefixes are allowed.\n");
                break;
            }
            TrieNode *node = findPrefix(root, word);
            if (node == NULL) {
                printf("No words start with '%s'.\n", word);
            } else {
                int count = 0;
                strcpy(buffer, word);
                printf("Words starting with '%s':\n", word);
                printWords(node, buffer, (int)strlen(word), &count);
                printf("Total: %d word(s)\n", count);
            }
            break;
        }

        case 4: {
            int count = 0;
            printf("All words in dictionary:\n");
            printWords(root, buffer, 0, &count);
            if (count == 0) printf("  (dictionary is empty)\n");
            else printf("Total: %d word(s)\n", count);
            break;
        }

        case 5: {
            printf("Enter word to delete: ");
            scanf("%99s", word);
            if (!normalize(word)) {
                printf("Only alphabetic words are allowed.\n");
                break;
            }
            int deleted = 0;
            deleteWord(root, word, 0, &deleted);
            if (deleted) printf("'%s' deleted.\n", word);
            else printf("'%s' not found.\n", word);
            break;
        }

        case 6:
            freeTrie(root);
            printf("Goodbye!\n");
            return 0;

        default:
            printf("Invalid choice. Try again.\n");
        }
    }

    freeTrie(root);
    return 0;
}