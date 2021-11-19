
#include "capi/fetcher.h"

#include <wtf/RunLoop.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    RunLoop::initializeMain();
    AtomString::init();
    WTF::RefCountedBase::enableThreadingChecksGlobally();

    pcfetcher_init(10, 1024);

    RunLoop::run();

    pcfetcher_term();

    return 0;
}
