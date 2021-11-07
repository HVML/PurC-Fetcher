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

#if 0
    RunLoop::initializeMain();

    ProcessLauncherClient processClient;
    ProcessLauncher::LaunchOptions launchOptions;
    launchOptions.processType = ProcessLauncher::ProcessType::Network;
    RefPtr<ProcessLauncher> processLauncher = ProcessLauncher::create(&processClient, WTFMove(launchOptions));

    RunLoop::run();
#else
    struct pcfetcher_encoder* encoder = pcfetcher_encoder_create();
    size_t size = 0;
    const uint8_t* buffer = pcfetcher_encoder_get_buffer(encoder, &size);
    fprintf(stderr, "0...............................buffer=%p|size=%ld\n", buffer, size);
    bool b = true;
    encode_basic(encoder, b);

    float c = 1.2f;
    encode_basic(encoder, c);

    int d = 10010;
    encode_basic(encoder, d);

    buffer = pcfetcher_encoder_get_buffer(encoder, &size);
    fprintf(stderr, "1...............................buffer=%p|size=%ld\n", buffer, size);

    struct pcfetcher_decoder* decoder = pcfetcher_decoder_create(buffer, size, false);

    bool eb = false;
    decode_basic(decoder, eb);
    fprintf(stderr, "....decode eb=%d\n", eb);

    float ec;
    decode_basic(decoder, ec);
    fprintf(stderr, "....decode ec=%f\n", ec);

    int ed;
    decode_basic(decoder, ed);
    fprintf(stderr, "....decode ed=%d\n", ed);

    pcfetcher_decoder_destroy(decoder);

    pcfetcher_encoder_destroy(encoder);
#endif

    return 0;
}
