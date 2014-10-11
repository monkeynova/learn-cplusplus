#include "Test/More.h"

using namespace Test::More;

int main( void )
{
    is( 1, 1 );
    is( 1 + 1, 2, "is" );
    isnt( 1 + 1, 3, "isnt" );
    is( 1 + 1, 3, "is/should fail" );
    isnt( 1 + 1, 2, "isnt/should fail" );

    int failedTests = done_testing();
    return min( failedTests, 254 );
}
