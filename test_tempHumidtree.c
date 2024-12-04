/**
 * test_tempHumidtree.c - File used for testing tempHumidtree.c file
 *
 * @file:		test_tempHumidtree.c
 * @author:		Brandon Duong (duon@pdx.edu)
 * @date:		04-Dec-2024
 * @version:	1.0
 *
 * This code verifies and uses the variosu functions of tempHumidtree to
 * test the abilities and functions of the file which includes creating,
 * populating, searching, and deleting in/the binary tree
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <errno.h>
#include "tempHumidtree.h"

// Helper function to print a readable timestamp
void printTimestamp(time_t timestamp) {
    char buffer[26];
    ctime_r(&timestamp, buffer);
    buffer[24] = '\0'; // Remove newline
    printf("%s", buffer);
}

int main() {
	
	 // Working Directory
    printf("HW5 Temperature/Humidity Binary tree - Brandon Duong (duon@pdx.edu)\n");
    errno = 0;
    char *buf = getcwd(NULL, 0);    // allocates a buffer large enough to hold the path
	
    if (buf == NULL) {
        perror("getcwd");
        printf("Could not display the path\n");
    }
    else {
        printf("Current working directory: %s\n", buf);
        free(buf);
    }
    printf("\n");
	
    printf("Testing the Temp/Humidity BST...\n");

    // Create the BST
    TempHumidTreePtr_t tree = createTempHumidTree();
    if (!tree) {
        printf("Failed to create BST.\n");
        return 1;
    }

    // Insert test data
    printf("Inserting test data...\n");
    struct tm test_date = {0};
    test_date.tm_year = 2023 - 1900; // Year 2023
    test_date.tm_mon = 10 - 1; // October
    test_date.tm_mday = 1; // Start on October 1

    for (int i = 0; i < 5; i++) {
        time_t timestamp = mktime(&test_date);
        DataItem_t data = {timestamp, 0x80000 + i * 0x1000, 0xA0000 + i * 0x2000};
        if (insert(tree, data)) {
            printf("Inserted: ");
            printTimestamp(data.timestamp);
            printf(" - Temp: %.2f°C, Humid: %.2f%%\n",
                   (data.temp / (double)(1 << 20)) * 200 - 50,
                   (data.humid / (double)(1 << 20)) * 100);
        } else {
            printf("Failed to insert: ");
            printTimestamp(data.timestamp);
            printf("\n");
        }
        test_date.tm_mday++;
    }

    // Search for specific timestamps
    printf("\nSearching for specific timestamps...\n");
    test_date.tm_mday = 2; // October 2, 2023
    time_t search_time = mktime(&test_date);
    BSTNodePtr_t result = search(tree, search_time);
    if (result) {
        printf("Found: ");
        printTimestamp(result->data.timestamp);
        printf(" - Temp: %.2f°C, Humid: %.2f%%\n",
               (result->data.temp / (double)(1 << 20)) * 200 - 50,
               (result->data.humid / (double)(1 << 20)) * 100);
    } else {
        printf("Data not found for: ");
        printTimestamp(search_time);
        printf("\n");
    }

    // Perform inorder traversal
    printf("\nInorder traversal of the BST:\n");
    inorder(tree);

    // Free the tree
    printf("\nFreeing the BST...\n");
    freeTree(tree);

    return 0;
}
