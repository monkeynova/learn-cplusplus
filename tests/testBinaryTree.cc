#include "Test/More.h"
#include "BinaryTree.h"
#include <map>

using namespace Test::More;
using namespace std;

int main( void ) {
    BinaryTree<int,int> b;
    map<int,int> expect;
    map<int,int>::iterator it;
    int v;

    for ( int n = 0; n < 10000; n++ ) {
        int i = rand() % 100;
        it = expect.find( i );
        if ( it  != expect.end() ) {
            if ( ok( b.find( i, v ) ) )
                is( v, 2 * i );
            b.remove( i );
            ok( ! b.find( i, v ) );
            expect.erase( it );
        } else {
            ok( ! b.find( i, v ) );
            b.insert( i, 2 * i );
            if ( ok( b.find( i, v ) ) )
                is( v, 2 * i );
            expect[i] = 2 * i;
        }
    }
    
    int ret = done_testing();
    return min( ret, 253 );
}
