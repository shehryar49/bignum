#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

using namespace std;

int main()
{
    /*bignum k("1");
    bignum l("3e8");
    while(k<=l)
    {
        cout<<k.val()<<endl;
        k.increment();
    }*/
    bignum n("3e8");
    bignum k("1");
    bignum ans("1");
    while(k <= n)
    {
        ans = ans * k;
        k.increment();
    }
    cout<<ans.val()<<endl;
}