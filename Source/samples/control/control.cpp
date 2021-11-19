
#include "capi/fetcher.h"

#include <wtf/RunLoop.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    purc_instance_extra_info info = {0, 0};
    purc_init ("cn.fmsoft.hybridos.sample", "pcfetcher", &info);

    RunLoop::initializeMain();
    AtomString::init();
    WTF::RefCountedBase::enableThreadingChecksGlobally();

    pcfetcher_init(10, 1024);

    RunLoop::run();

    pcfetcher_term();
    purc_cleanup();

    return 0;
}
