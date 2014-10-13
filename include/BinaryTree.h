#ifndef __BINARY_TREE_H
#define __BINARY_TREE_H

#include <exception>
#include <iostream>

template <class T>
class LT {
 public:
    int operator()( const T &t1, const T &t2 ) const { return t1 < t2 ? -1 : t1 > t2 ? 1 : 0; }
 };

template <class KeyType, class ValueType, class Comparator=LT<KeyType> >
class BinaryTree {
 public:
    class binarytree_exception : public std::exception {};

 protected:
   class Node {
   public:
   Node( const KeyType &k, const ValueType &v )
   : key( k ), value( v ), parent( 0 ), left( 0 ), right( 0 )
   {

   }
   virtual ~Node() {}

   Node *grandparent() const { return parent ? parent->parent : NULL; }
   Node *sibling() const {
       if ( parent && parent->left == this ) {
           return parent->right;
       }
       return parent ? parent->left : NULL;
   }
   Node *uncle() const { return parent ? parent->sibling() : NULL; }

   virtual void assert() const {
       if ( left && left->parent != this ) {
           std::cerr << "parent( " << left->key << " ) != " << this->key << std::endl;
           throw binarytree_exception();
       }
       if ( right && right->parent != this ) {
           std::cerr << "parent( " << right->key << " ) != " << this->key << std::endl;
           throw binarytree_exception();
       }
   }
   virtual void assertTree() const {
       assert();
       if ( left ) {
           left->assertTree();
       }
       if ( right ) {
           right->assertTree();
       }
   }

   virtual void debug( const std::string &prefix = "" ) const {
       if ( parent ) {
           std::cout << prefix << key << " (" << parent->key << ")" << std::endl;
       } else {
           std::cout << prefix << key << " (NULL)" << std::endl;
       }
   }
   virtual void debugTree( const std::string &prefix = "" ) const {
       debug( prefix );
       std::string subPrefix = prefix + "  ";
       if ( left ) {
           left->debugTree( subPrefix );
       }
       if ( right ) {
           right->debugTree( subPrefix );
       }
   }

   int operator()( const KeyType &otherKey ) const { return cmp( key, otherKey ); }
   int operator()( const Node *other ) const { return cmp( key, other->key ); }

   void reap() {
       if ( left ) {
           left->reap();
           delete left;
           left = NULL;
       }
       if ( right ) {
           right->reap();
           delete right;
           right = NULL;
       }
   }

   unsigned int height() const { return max( left ? left->height() + 1 : 1, right ? right->height + 1 : 1 ); }

   Comparator cmp;
   KeyType key;
   ValueType value;
   Node *parent;
   Node *left;
   Node *right;
   };

 Node *root;

 void rotateLeft( Node *node ) {
     Node *parent = node->parent;
     Node *newNode = node->right;
     Node *newLeft = node;

     if ( parent ) {
         if ( node == parent->left ) {
             parent->left = newNode;
         } else {
             parent->right = newNode;
         }
     } else {
         root = newNode;
     }

     newNode->parent = parent;

     Node *oldLeft = NULL;

     oldLeft = newNode->left;

     newNode->left = newLeft;
     newLeft->parent = newNode;

     newLeft->right = oldLeft;
     if ( oldLeft ) {
         oldLeft->parent = newLeft;
     }
 }

 void rotateRight( Node *node ) {
     Node *parent = node->parent;
     Node *newNode = node->left;
     Node *newRight = node;

     if ( parent ) {
         if ( node == parent->left ) {
             parent->left = newNode;
         } else {
             parent->right = newNode;
         }
     } else {
         root = newNode;
     }

     newNode->parent = parent;

     Node *oldRight = NULL;

     oldRight = newNode->right;

     newNode->right = newRight;
     newRight->parent = newNode;

     newRight->left = oldRight;
     if ( oldRight ) {
         oldRight->parent = newRight;
     }
 }


 virtual Node *makeNode( const KeyType &key, const ValueType &value ) const {
     return new Node( key, value );
 }

 virtual void onInsert( Node *insertedNode ) {}
 virtual void onDeleteSwap( Node *toDelete, Node *replacement ) {}
 virtual void onDelete( Node *deleted, Node *replacement ) {}

 public:
 BinaryTree() : root( 0 ) { }
 virtual ~BinaryTree() {
     root->reap();
     delete root;
 }

 void insert( const KeyType &key, const ValueType &value ) {
     Node *toInsert = makeNode( key, value );
     if ( ! root ) {
         root = toInsert;
     } else {
         Node *cur = root;
         bool done = false;

         while ( ! done ) {
             int cmp = (*cur)(key);
             if ( cmp == 0 ) {
                 cur->value = value;
                 done = true;
             }
             if ( cmp < 0 ) {
                 if ( cur->right ) {
                     cur = cur->right;
                 } else {
                     cur->right = toInsert;
                     toInsert->parent = cur;
                     done = true;
                 }
             } else {
                 if ( cur->left ) {
                     cur = cur->left;
                 } else {
                     cur->left = toInsert;
                     toInsert->parent = cur;
                     done = true;
                 }
             }
         }
     }

     onInsert( toInsert );
 }

 void remove( const KeyType &key ) {
     Node *found = _findNode( key );

     if ( ! found )
         return;

     if ( found->left && found->right ) {
         Node *replace = found->right;
         while ( replace->left )
             replace = replace->left;
         _swap( found, replace );
         onDeleteSwap( found, replace );
     }

     Node *replace = found->left ? found->left : found->right;

     if ( ! found->parent ) {
         root = replace;
     } else {
         if ( found->parent->left == found ) {
             found->parent->left = replace;
         } else {
             found->parent->right = replace;
         }
     }

     if ( replace ) {
         replace->parent = found->parent;
     }

     onDelete( found, replace );

     delete found;
 }

 bool find( const KeyType &key, ValueType &valueDest ) const {
     Node *found = _findNode( key );
     if ( found ) {
         valueDest = found->value;
     }

     return found;
 }

    unsigned int height() { return root ? root->height() : 0; }

 private:
 void _swap( Node *node1, Node *node2 ) {
     if ( ! node1->parent ) {
         root = node2;
     } else if ( node1 == node1->parent->left ) {
         node1->parent->left = node2;
     } else {
         node1->parent->right = node2;
     }

     if ( ! node2->parent ) {
         root = node1;
     } else if ( node2 == node2->parent->left ) {
         node2->parent->left = node1;
     } else {
         node2->parent->right = node1;
     }

     std::swap( node1->parent, node2->parent );
     std::swap( node1->left, node2->left );
     std::swap( node1->right, node2->right );

     if ( node1->left )
         node1->left->parent = node1;
     if ( node2->left )
         node2->left->parent = node2;
     if ( node1->right )
         node1->right->parent = node1;
     if ( node2->left )
         node2->right->parent = node2;
 }
 Node *_findNode( const KeyType &key ) const {
     Node *cur = root;

     while ( cur ) {
         int cmp = (*cur)( key );
         if ( cmp == 0 ) {
             break;
         } else if ( cmp < 0 ) {
             cur = cur->right;
         } else {
             cur = cur->left;
         }
     }

     return cur;
 }
};

#endif
