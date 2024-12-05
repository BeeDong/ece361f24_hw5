/**
 * @file tempHumidtree.h
 * @brief 
 * This is header file for a binary search tree ADT targeted to the
 * Temp/Humidity BST from ECE 361 HW #5
 *
 * @version:	1.0.0	
 * @author:		Roy Kravitz (roy.kravitz@pdx.edu)
 * @date: 		18-Nov-2023
 *
 * @note code is based on the BinarySearchTree example used during
 * the ECE 361 lectures which is based on Karumanchi's example code
 */
 
 #ifndef _TEMPHUMIDTREE_H
 #define _TEMPHUMIDTREE_H
 
 #include <stdbool.h>
 #include <stdint.h>
 #include <time.h>
 
/***** Debug constants *****/
//#define _DEBUG 2	
#define _DEBUG 0

/***** typedefs, structs, etc *****/
// define data item struct
typedef struct _temp_humid_data_s  {
		time_t		timestamp;
		uint32_t	temp;
		uint32_t	humid;
} DataItem_t, *DataItemPtr_t;

// define the binary search tree node
typedef struct _bstNode_s {
	DataItem_t			data;
	struct _bstNode_s	*left;
	struct _bstNode_s	*right;
} BSTNode_t, *BSTNodePtr_t;

// define the temp/humidity binary search tree
typedef struct _bstTree_s {
	BSTNodePtr_t	root;
	int				n;
} TempHumidTree_t, *TempHumidTreePtr_t;


/***** API FUNCTIONS *****/

/**
 * createTempHumidTree() - creates a Temperature/Humidity tree
 *
 * @return	a pointer to the new Temp/Humidity tree if succeeds.  Null if it fails
 * root node will start as NULL because the tree starts with 0 nodes
 *
 */
 TempHumidTreePtr_t createTempHumidTree(void);
 
/**
 * insert() - inserts a temp/humid data record into the tree
 *
 * @param	tree	pointer to the TempHumidtree to add the node to
 * @param	info	Temp/Humid data node to add to tree
 * @return			pointer to the new BST node

 * @note Not a good idea to expose the data node but w/o a pointer to
 * root I don't see much harm and it could be useful for debug
 */
BSTNodePtr_t insert(TempHumidTreePtr_t tree, DataItem_t info);


/**
 * search() - searches for a temp/humid data record into the tree w/ the specified timestamp
 *
 * @param	tree		pointer to the TempHumidtree to search
 * @param	timestamp	timestamp of the Temp/Humid data node we are seeking
 * @return				pointer to the BST node with that timestamp or NULL if not found

 * @note Not a good idea to expose the data node but w/o a pointer to
 * root I don't see much harm and it could be useful for debug
 */
 BSTNodePtr_t search(TempHumidTreePtr_t tree, time_t timestamp);
 
 
/**
 * inorder() - performs inorder traversal of tree
 *
 * @param	tree		pointer to the TempHumidtree to add the node to
 *
 * @brief
 * Performs an inorder traversal of the BST.  The data in the nodes are
 * displayed using the helper (non-API) function _displayDataNode(). This function
 * is a wrapper for _inorder() (non-AIPI) which does the work.
 */ 
void inorder(TempHumidTreePtr_t tree);

/**
 * freeNodes() - frees up nodes from the binary tree
 *
 * @param	node		pointer to a node in the binary tree to be removed
 *
 * @brief
 * Performs a removal of a single node and any nodes below it in the tree
 *
 */ 
void freeNodes(BSTNodePtr_t node);

/**
 * freeTree() - frees up the space that the binary tree took
 *
 * @param	tree		pointer to the generated binary tree
 *
 * @brief
 * deletes the binary tree and clears the allocation that was made
 *
 */ 
void freeTree(TempHumidTreePtr_t tree);

/*
 * printReadableTimestamp() - takes variable time_t 
 * 
 * @param	timestamp	
 *
 */ 
void printReadableTimestamp(time_t timestamp);

/*
 * populateBST() - fills binary tree with random temperature and humidity readings
 * 
 * @param	tree	pointer to generated binary tree
 * @param	start_month		initial month to populate with
 * @param	start_day	initial day to populate with
 * @param	num_days	number of days to populate
 *
 */ 
void populateBST(TempHumidTreePtr_t tree, int start_month, int start_day, int num_days);

 
#endif

