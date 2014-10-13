#ifndef __REDBLACK_TREE_H
#define __REDBLACK_TREE_H

#include "BinaryTree.h"

template <class K, class V, class C=LT<K> >
    class RedBlackTree : public BinaryTree<K,V,C> {
 public:
 class redblack_exception : public std::exception {};

 protected:
 typedef typename BinaryTree<K,V,C>::Node BTNode;

 class Node : public BTNode {
    public:
    enum COLOR { RED, BLACK };
    Node( const K &k, const V & v ) : BTNode( k, v ), color( RED ) {
    }

    virtual void assert() const {
        this->BTNode::assert();
        if ( color == RED ) {
            Node *left = static_cast<Node *>( BTNode::left );
            Node *right = static_cast<Node *>( BTNode::right );

            if ( left && left->color == RED ) {
                throw redblack_exception();
            }
            if ( right && right->color == RED ) {
                throw redblack_exception();
            }
        }
    }

    virtual void assertTree() const {
        int count;
        assertTreeWithCount( count );
    }

    void assertTreeWithCount( int &count ) const {
        int thisCount = color == Node::BLACK ? 1 : 0;
        assert();

        // Leaf cases if no recursion
        int countLeft = 1;
        int countRight = 1;

        Node *left = static_cast<Node *>( BTNode::left );
        Node *right = static_cast<Node *>( BTNode::right );

        if ( left ) {
            left->assertTreeWithCount( countLeft );
        }
        if ( right ) {
            right->assertTreeWithCount( countRight );
        }
        if ( countLeft != countRight ) {
            throw redblack_exception();
        }
        count = countLeft + thisCount;
    }

    COLOR color;
    };

 virtual BTNode *makeNode( const K &key, const V &value ) const {
     return static_cast<BTNode *>( new Node( key, value ) );
 }

 virtual void onInsert( BTNode *node ) {
     _onInsertFix( node );
     if ( BinaryTree<K,V,C>::root ) {
         BinaryTree<K,V,C>::root->assertTree();
     }
 }

 virtual void onDeleteSwap( BTNode *_toDelete, BTNode *_replacement ) {
     Node *toDelete = static_cast<Node *>( _toDelete );
     Node *replacement = static_cast<Node *>( _replacement );

     std::swap( toDelete->color, replacement->color );
 }

 virtual void onDelete( BTNode *deleted, BTNode *replacement ) {
     _onDeleteFix( deleted, replacement );
     if ( BinaryTree<K,V,C>::root ) {
         BinaryTree<K,V,C>::root->assertTree();
     }
 }

 private:
 typename Node::COLOR _getColor( const BTNode *node ) const {
     if ( node ) {
         return static_cast<const Node *>( node )->color;
     }
     return Node::BLACK; // Leaves are considered black
 }
 void _setColor( BTNode *node, typename Node::COLOR color ) const { static_cast<Node *>( node )->color = color; }

 void _onInsertFix( BTNode *node ) {
     if ( ! node->parent ) {
         _setColor( node, Node::BLACK );
         return;
     }

     if ( _getColor( node->parent ) == Node::BLACK ) {
         return;
     }

     BTNode *uncle = node->uncle();
     BTNode *grandparent = node->grandparent();

     if ( uncle && _getColor( uncle ) == Node::RED ) {
         _setColor( node->parent, Node::BLACK );
         _setColor( uncle, Node::BLACK );
         _setColor( grandparent, Node::RED );
         _onInsertFix( grandparent );
         return;
     }

     if ( node == node->parent->right && node->parent == grandparent->left ) {
         this->rotateLeft( node->parent );
         node = node->left;
     } else if ( node == node->parent->left && node->parent == grandparent->right ) {
         this->rotateRight( node->parent );
         node = node->right;
     }

     _setColor( node->parent, Node::BLACK );
     _setColor( grandparent, Node::RED );
     if ( node == node->parent->left ) {
         this->rotateRight( grandparent );
     } else {
         this->rotateLeft( grandparent );
     }
 }

 void _onDeleteFix( BTNode *deleted, BTNode *replacement ) {
     if ( _getColor( deleted ) == Node::RED ) {
         return;
     }

     if ( _getColor( replacement ) == Node::RED ) {
         _setColor( replacement, Node::BLACK );
         return;
     }

     BTNode *parent = replacement ? replacement->parent : deleted->parent;

     if ( ! parent ) {
         return;
     }

     BTNode *sibling = parent->left == replacement ? parent->right : parent->left;

     if ( _getColor( sibling ) == Node::RED ) {
         _setColor( parent, Node::RED );
         _setColor( sibling, Node::BLACK );
         if ( replacement == parent->left ) {
             this->rotateLeft( parent );
         } else {
             this->rotateRight( parent );
         }

         sibling = parent->left == replacement ? parent->right : parent->left;
     }

     if ( _getColor( sibling->left ) == Node::BLACK && _getColor( sibling->right ) == Node::BLACK ) {
         _setColor( sibling, Node::RED );
         if ( _getColor( parent ) == Node::BLACK ) {
             _onDeleteFix( deleted, parent );
         } else {
             _setColor( parent, Node::BLACK );
         }

         return;
     }

     // Sibling is black, exactly one red child
     if ( replacement == parent->left && _getColor( sibling->left ) == Node::RED ) {
         _setColor( sibling, Node::RED );
         _setColor( sibling->left, Node::BLACK );
         this->rotateRight( sibling );
         sibling = sibling->parent; // Got rotated out
     } else if ( replacement == parent->right && _getColor( sibling->right ) == Node::RED ) {
         _setColor( sibling, Node::RED );
         _setColor( sibling->right, Node::BLACK );
         this->rotateLeft( sibling );
         sibling = sibling->parent; // Got rotated out
     }

     _setColor( sibling, _getColor( parent ) );
     _setColor( parent, Node::BLACK );

     if ( replacement == parent->left ) {
         if ( sibling->right ) {
             _setColor( sibling->right, Node::BLACK );
         }
         this->rotateLeft( parent );
     } else {
         if ( sibling->left ) {
             _setColor( sibling->left, Node::BLACK );
         }
         this->rotateRight( parent );
     }
 }

};


#endif
