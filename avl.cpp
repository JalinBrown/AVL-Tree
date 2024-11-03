/*!*****************************************************************************
*\file     avl.cpp
*\author   Jalin A. Brown
*\brief Description:
	Self balancing binary search tree (AVL tree) implementation.

	API is exactly the same. You just add rebalancing.

	Implement:
	1) rotations - both left and right
	2) balance calculation
	3) AVL logic - this step will require modification of the BST insert and erase
******************************************************************************/

#include "avl-map.h"
#include <iostream>

/*!****************************************************************************
// Class AVLmap->Node Public Methods
******************************************************************************/

//-----------------------------------------------------------------------------
// CTOR
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::Node
(
	KEY_TYPE k,
	VALUE_TYPE val,
	Node* p,
	int h,
	int b,
	Node* l,
	Node* r
) : key(k), value(val), parent(p), height(h), balance(b), left(l), right(r)
{}

//-----------------------------------------------------------------------------
// Key Getter
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
KEY_TYPE const& CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::Key() const {
	  return key;
}

//-----------------------------------------------------------------------------
// Value Getter
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
VALUE_TYPE& CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::Value() {
	  return value;
}

//-----------------------------------------------------------------------------
// First Node
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::first() {
  // Traverse to the furthest left leaf node and return it  
	Node* N = this;
	while (N->left) 
	{
    N = N->left;
  }
  return N;
}

//-----------------------------------------------------------------------------
// Last Node
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::last() {
	// Traverse to the furthest right leaf node and return it
	Node* N = this;
	while (N->right) 
	{
    N = N->right;
  }
  return N;
}

//-----------------------------------------------------------------------------
// Increment Node
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::increment() {
  // If the right child exists, return the leftmost node of the right subtree
	Node* N = this;
	if (N->right) {
    return N->right->first();
  }

	// Otherwise, return the first ancestor whose left child is also an ancestor of N
	while ((N->parent) && (N == N->parent->right))
	{
    N = N->parent;
  }

  return N->parent; // Return the parent node
}

//-----------------------------------------------------------------------------
// Decrement Node
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::decrement() {
  // If the left child exists, return the rightmost node of the left subtree
	Node* N = this;
	if (N->left) {
    return N->left->last();
  }

	// Otherwise, return the first ancestor whose right child is also an ancestor of N
	while (N->parent && N == N->parent->left) {
    N = N->parent;
  }

  return N->parent; // Return the parent node
}

//-----------------------------------------------------------------------------
// Print Node
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::print(std::ostream& os) const {
	// Print the key-value 
	os << key << " -> " << value << std::endl;
}

//-----------------------------------------------------------------------------
// Check if Node has Key
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
bool CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::hasKey(KEY_TYPE const& k)
{
	// If the key is found, return true, otherwise return false
	if (k == key) return true;
  return false;
}

//-----------------------------------------------------------------------------
// Get Node Height
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
int CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::getHeight() const {
	// If the node is null, return -1, otherwise return the height of the node
	int leftHeight = (left) ? left->height : -1;
	int rightHeight = (right) ? right->height : -1;

	// Return the max height of the node whether left heavy or right heavy
	return 1 + std::max(leftHeight, rightHeight);
}

//-----------------------------------------------------------------------------
// Get Node Balance
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
int CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::getBalanceFactor() const {
	// If the node is null, return 0, otherwise return the balance factor of the node
	int leftHeight = (left) ? left->height : -1;
	int rightHeight = (right) ? right->height : -1;
	return leftHeight - rightHeight;
}

//-----------------------------------------------------------------------------
// Update Node Height
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::updateHeight() {
	int leftHeight = (left) ? left->height : -1;
	int rightHeight = (right) ? right->height : -1;
	height = 1 + std::max(leftHeight, rightHeight);
}

//-----------------------------------------------------------------------------
// Key & Value Setter methods (just in case)
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::setKey(const KEY_TYPE& newKey){
  key = newKey;
}

template<typename KEY_TYPE, typename VALUE_TYPE>
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node::setValue(const VALUE_TYPE& newValue){
  value = newValue;
}

/*!****************************************************************************
//  Static data members
******************************************************************************/

//-----------------------------------------------------------------------------
// end_it , initialized to nullptr
//-----------------------------------------------------------------------------
template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator
CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::end_it = CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator(nullptr);

//-----------------------------------------------------------------------------
// const_end_it , initialized to nullptr
//-----------------------------------------------------------------------------
template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const
CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::const_end_it = CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const(nullptr);


/*!****************************************************************************
// Struct AVLmap->AVLmap_iterator Private Methods
******************************************************************************/

//-----------------------------------------------------------------------------
// CTOR
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator::AVLmap_iterator(Node* p) : p_node(p) {
}

//-----------------------------------------------------------------------------
// Copy CTOR
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator::AVLmap_iterator(const AVLmap_iterator& rhs) {
  p_node = rhs.p_node;
}

//-----------------------------------------------------------------------------
// Operator=
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator& CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator::operator=(const AVLmap_iterator& rhs) {
	if (this != &rhs) {
		p_node = rhs.p_node;
	}
	return *this;
}

//-----------------------------------------------------------------------------
// Operator++
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator& CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator::operator++() {
	p_node = p_node->increment();
	return *this;
}

//-----------------------------------------------------------------------------
// Operator++ int
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator::operator++(int) {
	AVLmap_iterator tmp = *this;
	p_node = p_node->increment();
	return tmp;
}

//-----------------------------------------------------------------------------
// Operator*
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node& CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator::operator*() {
	return *p_node;
}

//-----------------------------------------------------------------------------
// Operator->
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator::operator->() {
	return p_node;
}

//-----------------------------------------------------------------------------
// Operator!=
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
bool CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator::operator!=(const AVLmap_iterator& rhs) {
  return p_node != rhs.p_node;
}

//-----------------------------------------------------------------------------
// Operator==
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
bool CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator::operator==(const AVLmap_iterator& rhs) {
  return p_node == rhs.p_node;
}

//-----------------------------------------------------------------------------
// Find and return node with given key or end_it if not found
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::find(KEY_TYPE const& key) {
	// Traverse the tree to find the node with the given key
	Node* N = pRoot;
	while (N) {
		if (key < N->key) {
			N = N->left;
		}
		else if (key > N->key) {
			N = N->right;
		}
		else {
			return AVLmap_iterator(N); // Return the non-const iterator to the node
		}
	}
	return end_it; // Return the end const iterator if the key is not found
}

/*!****************************************************************************
// Struct AVLmap->AVLmap_iterator_const Private Methods
******************************************************************************/

//-----------------------------------------------------------------------------
// CTOR
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const::AVLmap_iterator_const(Node* p) : p_node(p) {
}

//-----------------------------------------------------------------------------
// Operator=
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const& CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const::operator=(const AVLmap_iterator_const& rhs) {
	p_node = rhs.p_node;
	return *this;
}

//-----------------------------------------------------------------------------
// Operator++
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const& CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const::operator++() {
	p_node = p_node->increment();
	return *this;
}

//-----------------------------------------------------------------------------
// Operator++ int
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const::operator++(int) {
	AVLmap_iterator_const tmp = *this;
	p_node = p_node->increment();
	return tmp;
}

//-----------------------------------------------------------------------------
// Operator*
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node const& CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const::operator*() {
  return *p_node;
}

//-----------------------------------------------------------------------------
// Operator->
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node const* CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const::operator->() {
	return p_node;
}

//-----------------------------------------------------------------------------
// Operator!=
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
bool CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const::operator!=(const AVLmap_iterator_const& rhs) {
  return p_node != rhs.p_node;
}

//-----------------------------------------------------------------------------
// Operator==
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
bool CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const::operator==(const AVLmap_iterator_const& rhs) {
  return p_node == rhs.p_node;
}

/*!****************************************************************************
// Class AVLmap Public Methods
******************************************************************************/

//-----------------------------------------------------------------------------
// CTOR
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap() {
}

//-----------------------------------------------------------------------------
// Copy CTOR
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap(const AVLmap& rhs) {
	// If the source tree is not empty, copy the tree, starting from the root
	if (rhs.pRoot) {
		pRoot = new Node
		(
			rhs.pRoot->key,
			rhs.pRoot->value,
			nullptr,
			0,  // Height and balance are updated during copy
			0,
			nullptr,
			nullptr
		);
		copyTree(pRoot, rhs.pRoot); // Copy the tree recursively
	}

	size_ = rhs.size_; // Copy the size of the source tree
}

//-----------------------------------------------------------------------------
// Operator=
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
CS280::AVLmap<KEY_TYPE, VALUE_TYPE>& CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::operator=(const AVLmap& rhs) {
    // Check for self-assignment
    if (this != &rhs) {
        // Clear the current tree
        clear();

        // If the source tree is not empty, copy the tree, starting from the root
        if (rhs.pRoot) {
            pRoot = new Node
            (
                rhs.pRoot->key,
                rhs.pRoot->value,
                nullptr,
                rhs.pRoot->height,
                rhs.pRoot->balance,
                nullptr,
                nullptr
            );

            copyTree(pRoot, rhs.pRoot);
        }

        size_ = rhs.size_;
    }

    return *this; // Return the current tree
}

//-----------------------------------------------------------------------------
// ~DTOR
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::~AVLmap() {
	clear(); // Clear the tree
}

//-----------------------------------------------------------------------------
// Size
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
unsigned int CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::size() {
  return size_;
}

//-----------------------------------------------------------------------------
// Copy the tree recursively
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::copyTree(Node* dest, const Node* src) {
	if (!src) return; // Base case for recursion

	// Delete existing nodes in the destination tree
	if (dest->left) delete dest->left;
	if (dest->right) delete dest->right;

	// Copy the node and recursively copy left and right subtrees
	dest->key = src->key;
	dest->value = src->value;
	if (src->left) {
		// Create node with parent pointer
		dest->left = new Node(src->left->key, src->left->value, dest, 1, 0, nullptr, nullptr); 
		copyTree(dest->left, src->left);
	}
	if (src->right) {
		// Create node with parent pointer
		dest->right = new Node(src->right->key, src->right->value, dest, 1, 0, nullptr, nullptr); 
		copyTree(dest->right, src->right);
	}

	// Update balance and height of the copied nodes after copying
	dest->balance = dest->getBalanceFactor();
	dest->height = dest->getHeight();
}

//-----------------------------------------------------------------------------
// Operator[]
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
VALUE_TYPE& CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::operator[](KEY_TYPE const& key) {
	// Find the node with the given key
	Node* N = pRoot;
	while (N) {
		if (key < N->key) {
			N = N->left;
		}
		else if (key > N->key) {
			N = N->right;
		}
		else {
			return N->value; // Return the value of the node
		}
	}

	// If the key is not found, insert the key-value pair and return the value
	insert(key, VALUE_TYPE());
	return find(key)->value;
}

//-----------------------------------------------------------------------------
// Insert AVL Node
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::insert(KEY_TYPE const& key, VALUE_TYPE const& value) {
	Node* N = pRoot;
	Node* P = nullptr;

	// Traverse the tree to find the appropriate position to insert the new node
	while (N) {
		P = N;
		if (key < N->key) {
			N = N->left;
		}
		else if (key > N->key) {
			N = N->right;
		}
		else {
			// Key already exists, update the value and return
			N->setValue(value); // Use the setter function for value
			return;
		}
	}

	// Create a new node to insert
	Node* newNode = new Node(key, value, nullptr, 0, 0, nullptr, nullptr);
	if (!P) {
		pRoot = newNode; // Tree is empty, set the new node as the root
	}
	else if (key < P->key) {
		P->left = newNode;
		newNode->parent = P;
	}
	else {
		P->right = newNode;
		newNode->parent = P;
	}

	++size_; // Increment the size of the tree

	// Update balance, height, and perform AVL balancing starting from the parent node
	updateBalanceAfterInsert(P);

	// Update pRoot if a new root node was inserted
	if (pRoot && pRoot->parent != nullptr) {
		pRoot = pRoot->parent;
	}
}

//-----------------------------------------------------------------------------
// Update Tree Balance After Insertion
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::updateBalanceAfterInsert(Node* node) {
	while (node) {
		node->balance = node->getBalanceFactor(); // Update balance factor
		node->height = node->getHeight();         // Update height

		// Check if the balance factor is greater than 1 (left-heavy subtree)
		if (node->balance > 1) {
			// Check if the left subtree of the node is right-heavy
			if (node->left && node->left->balance < 0) {
				node->left = leftRotate(node->left); // Perform left-right rotation
			}
			node = rightRotate(node); // Perform right rotation
		}
		// Check if the balance factor is less than -1 (right-heavy subtree)
		else if (node->balance < -1) {
			// Check if the right subtree of the node is left-heavy
			if (node->right && node->right->balance > 0) {
				node->right = rightRotate(node->right); // Perform right-left rotation
			}
			node = leftRotate(node); // Perform left rotation
		}

		if (node) {
			node = node->parent; // Move to the parent for further balancing
		}
	}
}

//-----------------------------------------------------------------------------
//AVLmap begin() method dealing with non-const iterator 
//-----------------------------------------------------------------------------
template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::begin() {
	if (pRoot)
		return AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator(pRoot->first());
	else
		return end_it;
}

//-----------------------------------------------------------------------------
//AVLmap end() method dealing with non-const iterator 
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::end() {
	return end_it;
}

//-----------------------------------------------------------------------------
//AVLmap begin() method dealing with CONST iterator 
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::begin() const {
  if (pRoot) 
		return AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const(pRoot->first());
  else       
		return const_end_it;
}

//-----------------------------------------------------------------------------
//AVLmap end() method dealing with CONST iterator
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::end() const {
	return const_end_it;
}

//-----------------------------------------------------------------------------
// Find and return node with given key or const_end_it if not found
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::find(KEY_TYPE const& key) const {
	Node* N = pRoot;
	while (N) {
		if (key < N->key) {
			N = N->left;
		}
		else if (key > N->key) {
			N = N->right;
		}
		else {
			return AVLmap_iterator_const(N);
		}
	}

	return const_end_it;
}

//-----------------------------------------------------------------------------
// Erase
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::erase(AVLmap_iterator it) {
	Node* N = it.p_node;
	if (!N) {
		return; // Check for null pointer
	}

	// Case 1: Node has no children
	if (!N->left && !N->right) {
		if (N->parent) {
			if (N->parent->left == N) {
				N->parent->left = nullptr;
			}
			else {
				N->parent->right = nullptr;
			}
		}
		else {
			pRoot = nullptr; // Update root if deleting the root node
		}
		delete N; // Free the memory allocated for the node
	}
	// Case 2: Node has one child
	else if (!N->left || !N->right) {
		Node* child = (N->left) ? N->left : N->right;
		if (N->parent) {
			if (N->parent->left == N) {
				N->parent->left = child;
			}
			else {
				N->parent->right = child;
			}
		}
		else {
			pRoot = child; // Update root if deleting the root node
		}
		if (child) {
			child->parent = N->parent;
		}
		delete N; // Free the memory allocated for the node
	}
	// Case 3: Node has two children
	else {
		Node* successor = N->right->first(); // Find the successor node
		N->key = successor->key;						 // Replace key with successor key
		N->value = successor->value;				 // Replace value with successor value
		erase(successor);										 // Recursively delete the successor node
	}

	--size_; // Decrement the size of the tree
}

//-----------------------------------------------------------------------------
// Update Tree Balance after deleting (AVL Balancing)
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::updateBalanceAfterDelete(Node* node) {
	while (node) {
		node->balance = node->getBalanceFactor();
		node->height = node->getHeight();

		if (node->balance > 1) {
			if (node->right && node->right->getBalanceFactor() < 0) {
				// Double rotation: right-left case
				rightRotate(node->right);
			}
			// Single rotation: left case
			leftRotate(node);
		}
		else if (node->balance < -1) {
			if (node->left && node->left->getBalanceFactor() > 0) {
				// Double rotation: left-right case
				leftRotate(node->left);
			}
			// Single rotation: right case
			rightRotate(node);
		}

		node = node->parent; // Move to the parent for further balancing
	}
}

//-----------------------------------------------------------------------------
// AVLmap Rotation Methods - (Y) (U)se (V)olks (W)agon ?!?!
// y - Node being rotated
// u - Left child of y (if it exists)
// v - Right child of y (if it exists)
// w - Right child of v (if it exists)
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Left Rotation
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::leftRotate(Node* y) {
	if (!y || !y->right) {
		return nullptr; // Check for null pointers
	}

	Node* subTreeNewRoot = y->right;
	Node* v = subTreeNewRoot->left;

	// Perform rotation
	subTreeNewRoot->left = y;
	y->right = v;

	// Update parent pointers
	subTreeNewRoot->parent = y->parent;
	if (y->parent) {
		if (y->parent->left == y) {
			y->parent->left = subTreeNewRoot;
		}
		else {
			y->parent->right = subTreeNewRoot;
		}
	}
	y->parent = subTreeNewRoot;
	if (v) {
		v->parent = y;
	}

	// Update heights
	// For node y, update its height by taking the maximum height between its 
	// left and right children (if they exist) and adding 1.
	y->height = std::max(y->left ? y->left->height : -1, y->right ? y->right->height : -1) + 1;

	// For the new root of the rotated subtree (subTreeNewRoot), update its height
	//  by considering the max height between its left and right children (if they exist) and adding 1.
	subTreeNewRoot->height = 
		std::max(subTreeNewRoot->left ? subTreeNewRoot->left->height : -1,
			       subTreeNewRoot->right ? subTreeNewRoot->right->height : -1) + 1;

	return subTreeNewRoot;
}

//-----------------------------------------------------------------------------
// Right Rotation
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::rightRotate(Node* y) {
	if (!y || !y->left) {
		return nullptr; // Check for null pointers
	}

	Node* subTreeNewRoot = y->left;  // left child of y as the new root of the subtree
	Node* v = subTreeNewRoot->right; // right child of the new root (if it exists)

	// Right rotation
	subTreeNewRoot->right = y; // Make y the right child of the new root
	y->left = v; // Make v the left child of y 

	// Update parent pointer
	subTreeNewRoot->parent = y->parent; // Update the parent of new root

	// If y has a parent
	if (y->parent) { 
		if (y->parent->left == y) {
			y->parent->left = subTreeNewRoot; // Update left child of y's parent to the new root
		}
		else {
			y->parent->right = subTreeNewRoot; // Update right child of y's parent to the new root
		}
	}

	// Update y's parent to be the new root
	y->parent = subTreeNewRoot; 
	if (v) {
		v->parent = y; // If v exists, update its parent to be y
	}

	// Update heights
	y->height = std::max(y->left ? y->left->height : -1, y->right ? y->right->height : -1) + 1;
	
	subTreeNewRoot->height = 
		std::max(subTreeNewRoot->left ? subTreeNewRoot->left->height : -1,
					  subTreeNewRoot->right ? subTreeNewRoot->right->height : -1) + 1;

	return subTreeNewRoot;
}

//-----------------------------------------------------------------------------
// Clear Tree
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::clear() {
	while (pRoot) {
		erase(begin());
	}
}

//-----------------------------------------------------------------------------
// Get Depth
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
int CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::getdepth(Node* b) const {
	int depth = 0;
	while (b->parent) {
		++depth;
		b = b->parent;
	}
	return depth;
}

//-----------------------------------------------------------------------------
// Update Heights
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::updateHeights(Node* node) {
	while (node) {
		node->height = std::max(node->left ? node->left->height : -1, 
													  node->right ? node->right->height : -1) + 1;
		node = node->parent;
	}
}

//-----------------------------------------------------------------------------
// Update Tree Balance
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::updateBalance(Node* node) {
	// Update balance and height of the node and its children
	if (node) {
		node->balance = node->getBalanceFactor(); // Update balance
		node->height = node->getHeight(); // Update height after balance
		updateBalance(node->left); // Update balance of left child
		updateBalance(node->right); // Update balance of right child
	}
}

//-----------------------------------------------------------------------------
// Return Height of Node
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
int CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::getHeight(Node* node) {
	return node->height;
}

//-----------------------------------------------------------------------------
// Move Constructor (noexcept)
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap(AVLmap&& other) noexcept
	: pRoot(other.pRoot), size_(other.size_) {	
	other.pRoot = nullptr; // Transfer ownership, set source to null
	other.size_ = 0;       // Reset the size of the source tree
}

//-----------------------------------------------------------------------------
// Move Assignment Operator (noexcept)
//-----------------------------------------------------------------------------
template<typename KEY_TYPE, typename VALUE_TYPE>
CS280::AVLmap<KEY_TYPE, VALUE_TYPE>& CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::operator=(AVLmap&& other) noexcept {
	if (this != &other) {		 // Check for self-assignment
		clear();							 // Clear current tree
		pRoot = other.pRoot;	 // Transfer ownership of root
		size_ = other.size_;	 // Transfer ownership of size
		other.pRoot = nullptr; // Reset source tree
		other.size_ = 0;       // Reset source size
	}
	return *this;
}