#ifndef __TEST_MORE_H
#define __TEST_MORE_H

#include <iostream>
#include <sstream>

namespace Test
{
    namespace More
    {
        using namespace std;

        int testCount = 0;
        int failureCount = 0;

        bool ok( string message = "" )
        {
            cout << "ok " << testCount++;
            if ( message != "" )
                cout << " " << message;
            cout << endl;
            return true;
        }
        bool fail( string message = "" )
        {
            cout << "not ok " << testCount++;
            if ( message != "" )
                cout << " " << message;
            cout << endl;
            failureCount++;
            return false;
        }
        void plan( int plannedTestCount )
        {
            cout << "1.." << plannedTestCount << endl;
        }
        void no_plan()
        {
            cout << "1.." << endl;
        }
        int done_testing()
        {
            cout << "1.." << testCount << endl;
            return failureCount;
        }

        void note( string message )
        {
            cout << "# " << message << endl;
        }
        void diag( string message )
        {
            cerr << "# " << message << endl;
        }

        template <class T> static bool is( const T &got, const T &expected, string message = "" )
            {
                if ( got == expected )
                    {
                        return ok( message );
                    }
                else
                    {
                        fail( message );
                        stringstream gotDiag;
                        gotDiag << "       got = " << got;
                        stringstream expectDiag;
                        expectDiag << " expected = " << expected;
                        diag( gotDiag.str() );
                        diag( expectDiag.str() );
                        return false;
                    }
            }

        template <class T> static bool isnt( const T &got, const T &expected, string message = "" )
            {
                if ( got != expected )
                    {
                        return ok( message );
                    }
                else
                    {
                        fail( message );
                        stringstream gotDiag;
                        gotDiag << "       got = " << got;
                        stringstream expectDiag;
                        expectDiag << " expected = anything else";
                        diag( gotDiag.str() );
                        diag( expectDiag.str() );
                        return false;
                    }
            }


    }
}

#endif
