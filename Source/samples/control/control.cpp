#include "config.h"
#include "Connection.h"
#include "MessageNames.h"
#include "ProcessLauncher.h"
#include "NetworkProcessCreationParameters.h"
#include "NetworkProcessMessages.h"
#include "Encoder.h"

#include "capi/fetcher.h"
#include "capi/fetcher-msg.h"

#include <glib.h>
#include <wtf/glib/GLibUtilities.h>
#include <wtf/glib/GUniquePtr.h>
#include <wtf/RunLoop.h>

#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

using namespace PurCFetcher;

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
