#include "config.h"
#include "Connection.h"
#include "MessageNames.h"
#include "ProcessLauncher.h"
#include "NetworkProcessCreationParameters.h"
#include "NetworkProcessMessages.h"

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

class ProcessLauncherClient :  public ProcessLauncher::Client, public IPC::Connection::Client {
    void didFinishLaunching(ProcessLauncher*, IPC::Connection::Identifier identifier)
    {
        fprintf(stderr, "..................................finishing launching\n");
        this->identifier = identifier;

        this->conn = IPC::Connection::createServerConnection(identifier, *this);
        NetworkProcessCreationParameters parameters;
        this->conn->open();
        this->conn->send(Messages::NetworkProcess::InitializeNetworkProcess(parameters), 0);
    }
    void didClose(IPC::Connection&) {}
    void didReceiveInvalidMessage(IPC::Connection&, IPC::MessageName) {}
public:
    IPC::Connection::Identifier identifier;
    RefPtr<IPC::Connection> conn;
};

int main(int argc, char** argv)
{
    fprintf(stderr, "argc=%d|argv[0]=%s\n", argc, argv[0]);

    RunLoop::initializeMain();

    ProcessLauncherClient processClient;
    ProcessLauncher::LaunchOptions launchOptions;
    launchOptions.processType = ProcessLauncher::ProcessType::Network;
    RefPtr<ProcessLauncher> processLauncher = ProcessLauncher::create(&processClient, WTFMove(launchOptions));

    RunLoop::run();

    return 0;
}
