/**
 * tempHumidtree.c - module file for binary tree functions to hold data from iom361
 *
 * @file:		tempHumidtree.c
 * @author:		Brandon Duong (duon@pdx.edu)
 * @date:		04-Dec-2024
 * @version:	1.0
 *
 * This code implements functions for creating and filling up a binary tree with
 * temperature data that can then be searched and deleted when not needed.
 *
 */
#include "tempHumidtree.h"
#include "iom361_r2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Include for strlen()

// Helper function to create a new BST node
static BSTNodePtr_t createNode(DataItem_t data) {
    BSTNodePtr_t node = (BSTNodePtr_t)malloc(sizeof(BSTNode_t));
    if (!node) {
        perror("Failed to allocate memory for BST node");
        return NULL;
    }
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

// Create a new temperature/humidity tree
TempHumidTreePtr_t createTempHumidTree(void) {
    TempHumidTreePtr_t tree = (TempHumidTreePtr_t)malloc(sizeof(TempHumidTree_t));
    if (!tree) {
        perror("Failed to allocate memory for the tree");
        return NULL;
    }
    tree->root = NULL; // Initialize the root to NULL
    return tree;
}

// Insert a new node into the BST
BSTNodePtr_t insert(TempHumidTreePtr_t tree, DataItem_t data) {
    if (!tree) return NULL;

    BSTNodePtr_t node = createNode(data);
    if (!node) return NULL;

    if (!tree->root) {
        tree->root = node;
        return node;
    }

    BSTNodePtr_t current = tree->root, parent = NULL;
    while (current) {
        parent = current;
        if (data.timestamp < current->data.timestamp) {
            current = current->left;
        } else if (data.timestamp > current->data.timestamp) {
            current = current->right;
        } else {
            free(node); // Duplicate timestamps are not allowed
            return NULL;
        }
    }

    if (data.timestamp < parent->data.timestamp) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    return node;
}

// Search for a node in the BST
BSTNodePtr_t search(TempHumidTreePtr_t tree, time_t timestamp) {
    if (!tree || !tree->root) return NULL;

    BSTNodePtr_t current = tree->root;
    while (current) {
        if (timestamp < current->data.timestamp) {
            current = current->left;
        } else if (timestamp > current->data.timestamp) {
            current = current->right;
        } else {
            return current; // Found the node
        }
    }
    return NULL; // Not found
}

// Helper function for inorder traversal
static void inorderTraversal(BSTNodePtr_t node) {
    if (!node) return;

    inorderTraversal(node->left);

    char timeStr[26];
    ctime_r(&node->data.timestamp, timeStr); // Format timestamp into a string
    timeStr[strlen(timeStr) - 1] = '\0'; // Remove newline character

    printf("%s - Temp: %.2f°C, Humid: %.2f%%\n",
           timeStr,
           (node->data.temp / (double)(1 << 20)) * 200 - 50,
           (node->data.humid / (double)(1 << 20)) * 100);

    inorderTraversal(node->right);
}

// Perform an inorder traversal of the tree
void inorder(TempHumidTreePtr_t tree) {
    if (!tree || !tree->root) {
        printf("Tree is empty.\n");
        return;
    }
    inorderTraversal(tree->root);
}

// Helper function to free nodes recursively
void freeNodes(BSTNodePtr_t node) {
    if (!node) return;

    freeNodes(node->left);
    freeNodes(node->right);

    free(node);
}

// Free the entire tree
void freeTree(TempHumidTreePtr_t tree) {
    if (!tree) return;

    freeNodes(tree->root);
    free(tree);
}

// Helper function to print a readable timestamp
void printReadableTimestamp(time_t timestamp) {
    char buffer[26];
    ctime_r(&timestamp, buffer);
    buffer[24] = '\0'; // Remove newline
    printf("%s", buffer);
}

// Populate the BST with random temperature and humidity readings
void populateBST(TempHumidTreePtr_t tree, int start_month, int start_day, int num_days) {
    if (!tree) {
        printf("Error: Tree is NULL.\n");
        return;
    }

    printf("Initializing population...\n");

    struct tm start_date = {0};
    start_date.tm_year = 2023 - 1900;
    start_date.tm_mon = start_month - 1;
    start_date.tm_mday = start_day;

    // Array to store timestamps
    time_t *timestamps = malloc(num_days * sizeof(time_t));
    if (!timestamps) {
        perror("Failed to allocate memory for timestamps.");
        return;
    }

    // Generate timestamps
    for (int i = 0; i < num_days; i++) {
        timestamps[i] = mktime(&start_date);
        if (timestamps[i] == -1) {
            printf("Error generating timestamp for day %d\n", i + 1);
        } else {
            printf("Generated timestamp: %ld -> ", timestamps[i]);
            printReadableTimestamp(timestamps[i]);
            printf("\n");
        }
        start_date.tm_mday++;
    }

    // Shuffle timestamps
    srand((unsigned)time(NULL));
    for (int i = 0; i < num_days; i++) {
        int j = rand() % num_days;
        time_t temp = timestamps[i];
        timestamps[i] = timestamps[j];
        timestamps[j] = temp;
    }

    printf("Shuffling complete. Populating BST...\n");

    // Populate BST with random temperature and humidity data
    for (int i = 0; i < num_days; i++) {
        printf("Setting sensor values for timestamp: ");
        printReadableTimestamp(timestamps[i]);
        printf("\n");

        _iom361_setSensor1_rndm(0, 50, 50, 100); // Generate random sensor data

        printf("Reading temperature and humidity...\n");
        uint32_t temp = iom361_readReg(NULL, TEMP_REG, NULL);
        uint32_t humid = iom361_readReg(NULL, HUMID_REG, NULL);

        printf("Temp = %u, Humid = %u\n", temp, humid);

        printf("Inserting into BST...\n");
        DataItem_t data = {timestamps[i], temp, humid};
        if (!insert(tree, data)) {
            printf("Failed to insert timestamp: ");
            printReadableTimestamp(timestamps[i]);
            printf("\n");
        }
    }

    free(timestamps);
    printf("BST populated successfully.\n");
}