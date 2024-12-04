/*Copyright (c) 2016 CareerMonk Publications and others.
#E-Mail            : info@careermonk.com 
#Creation Date     : 2008-01-10 06:15:46 
#Created by        : Narasimha Karumanchi 
#Book Title        : Data Structures And Algorithms Made Easy
#Warranty          : This software is provided "as is" without any 
#                    warranty; without even the implied warranty of 
#                    merchantability or fitness for a particular purpose.*/

#include <stdio.h>
#include <stdlib.h>

struct BinaryTreeNode{
    int data;
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
};

// Insert a new node in the tree
struct BinaryTreeNode* createNewNode(int data){
    struct BinaryTreeNode* node = malloc(sizeof(struct BinaryTreeNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Traverse the tree - in order (left-root-right))
void inOrder(struct BinaryTreeNode* root){
     if (root == NULL) // or if (!root)
          return;
     
     inOrder(root->left);  
     printf("%d ", root->data);  
     inOrder(root->right);
}

// Traverse the tree - pre order (root-left-right))
void preOrder(struct BinaryTreeNode* root){
     if (root == NULL) // or if (!root)
          return;
     printf("%d ", root->data);  
     preOrder(root->left);  
     preOrder(root->right);
}

// Traverse the tree - post order (left-right-root))
void postOrder(struct BinaryTreeNode* root){
     if (root == NULL) // or if (!root)
          return;
       
     postOrder(root->left);  
     postOrder(root->right);
     printf("%d ", root->data);
}


// Test the code
int main() {
	/* Sample binary tree is
            9
          /   \
        19      1
         \    /
          3  6
    */
	printf("...Creating the tree\n");
    struct BinaryTreeNode* root = createNewNode(9);
    root->left        = createNewNode(19);
    root->right       = createNewNode(1);
    struct BinaryTreeNode* temp = root->left;
    temp->right  = createNewNode(3);
    temp = root->right;
    temp->left = createNewNode(6);
	
    // display the tree
    printf("Tree is:\n");
    printf("        9\n");
    printf("      /   \\\n");
    printf("    19     1\n");
    printf("     \\    /\n");
    printf("      3  6\n");
    printf("\n");
	printf("...Performing a recursive in-order traversal\n");
	inOrder(root);
	printf("\n...Performing a recursive pre-order traversal\n");
	preOrder(root);
	printf("\n...Performing a recursive post-order traversal\n");
	postOrder(root);
	return 0;
}