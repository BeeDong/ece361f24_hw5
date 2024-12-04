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
