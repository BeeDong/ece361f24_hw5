#include <stdio.h>
#include <time.h>
#include "tempHumidtree.h"

int main() {
    printf("Testing populateBST function...\n");

    // Create the BST
    TempHumidTreePtr_t tree = createTempHumidTree();
    if (!tree) {
        printf("Failed to create BST.\n");
        return 1;
    }

    // Populate the BST with random data
    int start_month = 10; // October
    int start_day = 1;    // Start from October 1
    int num_days = 10;    // 10 days of data

    populateBST(tree, start_month, start_day, num_days);

    // Perform inorder traversal
    printf("\nInorder traversal of BST:\n");
    inorder(tree);

    // Search for specific timestamps
    printf("\nSearching for specific timestamps...\n");
    struct tm search_date = {0};
    search_date.tm_year = 2023 - 1900; // Example year
    search_date.tm_mon = start_month - 1;
    search_date.tm_mday = start_day + 5; // Search for 6th day (Oct 6)

    time_t search_time = mktime(&search_date);
    if (search_time == -1) {
        printf("Error generating search timestamp.\n");
    } else {
        BSTNodePtr_t result = search(tree, search_time);
        if (result) {
            char buffer[26];
            ctime_r(&result->data.timestamp, buffer);
            buffer[24] = '\0'; // Remove newline
            printf("Found: %s - Temp: %.2f°C, Humid: %.2f%%\n",
                   buffer,
                   (result->data.temp / (double)(1 << 20)) * 200 - 50,
                   (result->data.humid / (double)(1 << 20)) * 100);
        } else {
            char buffer[26];
            ctime_r(&search_time, buffer);
            buffer[24] = '\0'; // Remove newline
            printf("Data not found for date: %s\n", buffer);
        }
    }

    // Free the tree
    printf("\nFreeing the BST...\n");
    freeTree(tree);

    return 0;
}
