/*!*****************************************************************************
*\file     avl.h
*\author   Jalin A. Brown
*\brief Description:
	Self balancing binary search tree (AVL tree) implementation.

	API is exactly the same. You just add rebalancing.

	Implement:
	1) rotations - both left and right
	2) balance calculation
	3) AVL logic - this step will require modification of the BST insert and erase
******************************************************************************/

#ifndef AVLMAP_H
#define AVLMAP_H

//-----------------------------------------------------------------------------
// Includes:
//-----------------------------------------------------------------------------

#include <utility> // std::move()
#include <stack>	 // std::stack

namespace CS280 {
		//-----------------------------------------------------------------------------
		// AVLmap class declarations
		//-----------------------------------------------------------------------------
    template< typename KEY_TYPE, typename VALUE_TYPE >
    class AVLmap {
		public:

			//-----------------------------------------------------------------------------
			// Node class declarations
			//-----------------------------------------------------------------------------
			class Node {
				public:
					// Big Four
					Node( KEY_TYPE k, VALUE_TYPE val, Node* p, int h, int b, Node* l, Node* r);
					Node(const Node&)               = delete;
					Node* operator=(const Node&)    = delete;
					
					// Gettters
          KEY_TYPE const & Key() const;   // return a const reference
          VALUE_TYPE  &    Value();       // return a reference
					Node*  first(); // minimum - follow left as far as possible
					Node*  last(); // maximum - follow right as far as possible
					Node*  increment(); // successor
					Node*  decrement(); // predecessor

					// Helper functions
					void print(std::ostream& os ) const;
					bool hasKey(KEY_TYPE const& k);
					int  getHeight() const;
					int  getBalanceFactor() const;
					void updateHeight();
					void setKey(const KEY_TYPE& newKey);
					void setValue(const VALUE_TYPE& newValue);

				private:
          KEY_TYPE    key;
					VALUE_TYPE  value;
					Node        *parent;
					Node        *left;
					Node        *right;
					int         height,balance; // optional

					friend class AVLmap;
			};

		// AVLmap_iterator class declarations
    private:

			//-----------------------------------------------------------------------------
			// AVLmap_iterator class declarations
			//-----------------------------------------------------------------------------
			class AVLmap_iterator {
				private:
					Node* p_node;
				public:
					AVLmap_iterator(Node* p=nullptr);
					AVLmap_iterator(const AVLmap_iterator& rhs);
					AVLmap_iterator& operator=(const AVLmap_iterator& rhs);
					AVLmap_iterator& operator++();
					AVLmap_iterator operator++(int);
					Node & operator*();
					Node * operator->();
					Node* getnode() {return p_node;}
					bool operator!=(const AVLmap_iterator& rhs);
          bool operator==(const AVLmap_iterator& rhs);

					friend class AVLmap;
			};

			//-----------------------------------------------------------------------------
			// AVLmap_iterator_const class declarations
			//-----------------------------------------------------------------------------
			class AVLmap_iterator_const {
				private:
					Node* p_node;
				public:
					AVLmap_iterator_const(Node* p=nullptr);
					AVLmap_iterator_const& operator=(const AVLmap_iterator_const& rhs);
					AVLmap_iterator_const& operator++();
					AVLmap_iterator_const operator++(int);
					Node const& operator*();
					Node const* operator->();
					Node* getnode() { return p_node; }
					bool operator!=(const AVLmap_iterator_const& rhs);
					bool operator==(const AVLmap_iterator_const& rhs);

					friend class AVLmap;
			};

		//-----------------------------------------------------------------------------
		// AVLmap class implementations
		//-----------------------------------------------------------------------------
		Node* pRoot = nullptr;
    unsigned int size_ = 0;
		static AVLmap_iterator end_it;
		static AVLmap_iterator_const const_end_it;

		public:
			// BIG FOUR
			AVLmap();
			AVLmap(const AVLmap& rhs);
			AVLmap& operator=(const AVLmap& rhs);
			virtual ~AVLmap();

			// Operator[]
			VALUE_TYPE& operator[](KEY_TYPE const& key);

			// Move constructor and move assignment
			AVLmap(AVLmap&& other) noexcept;
			AVLmap& operator=(AVLmap&& other) noexcept;

			// Getters
      unsigned int size();
			int getdepth(Node* b) const;

			// Helper functions
			void erase(AVLmap_iterator it);
			void clear();
			int getHeight(Node* node);
			void insert(KEY_TYPE const& key, VALUE_TYPE const& value);
			void copyTree(Node* dest, const Node* src);// Method to copy a subtree recursively

			//standard names for iterator types
			typedef AVLmap_iterator       iterator;
			typedef AVLmap_iterator_const const_iterator;

			//-----------------------------------------------------------------------------
			// AVLmap methods dealing with non-const iterator 
			//-----------------------------------------------------------------------------
			AVLmap_iterator begin();
			AVLmap_iterator end();
			AVLmap_iterator find(KEY_TYPE const& key);

			//-----------------------------------------------------------------------------
			// AVLmap methods dealing with const iterator 
			//-----------------------------------------------------------------------------
			AVLmap_iterator_const begin() const;
			AVLmap_iterator_const end() const;
			AVLmap_iterator_const find(KEY_TYPE const& key) const;

			//-----------------------------------------------------------------------------
			// AVLmap Rotation Methods
			//-----------------------------------------------------------------------------

			Node* leftRotate(Node* y); // Left Rotation			
			Node* rightRotate(Node* y); // Right Rotation
			void updateBalanceAfterInsert(Node* node);
			void updateBalanceAfterDelete(Node* node);
			void updateBalance(Node* node);
			void updateHeights(Node* node);

			friend class AVLmap_iterator;
			friend class AVLmap_iterator_const;
		private:
	};

	// Operator<<
  template< typename KEY_TYPE, typename VALUE_TYPE >
	std::ostream& operator<<(std::ostream& os, AVLmap<KEY_TYPE, VALUE_TYPE> const& map);

}

#include "avl-map.cpp"
#endif
