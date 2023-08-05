#include <stdlib.h>
#include <stdio.h>

#include "pKernel/pKernel.h"
#include "tests/all.h"

int main()
{
    time_init(4500);
    pk_init(NULL);
    test_clock();
    pk_run();

    return 0;
}
