#include "config.h"
#include "Connection.h"
#include "MessageNames.h"
#include "ProcessLauncher.h"
#include "NetworkProcessCreationParameters.h"
#include "NetworkProcessMessages.h"
#include "Encoder.h"

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


    // test compare with encode
#if 1
    IPC::Encoder* ipc_encoder = new IPC::Encoder(IPC::MessageName(0x12), 0x5678);

    Vector<String> vec;
    vec.append("abcdef");
    vec.append("123456");
    ipc_encoder->encode(vec);

    const uint8_t* buf = ipc_encoder->buffer();
    size_t len = ipc_encoder->bufferSize();
    for (size_t i = 0; i < len; i++) {
        fprintf(stderr, "i=%ld|buf[i]=%d|buf[i]=%c\n", i, buf[i], buf[i]);
    }

    struct pcfetcher_decoder* decoder = pcfetcher_decoder_create(
            ipc_encoder->buffer(), ipc_encoder->bufferSize(), false);

    struct pcutils_arrlist* array = NULL;
    struct pcfetcher_msg_header msg;
    pcfetcher_msg_header_decode(decoder, &msg);
    pcfetcher_string_array_decode(decoder, &array);

    uint64_t size = pcutils_arrlist_length(array);
    for (uint64_t i = 0; i < size; i++) {
        void* item = pcutils_arrlist_get_idx(array, i);
        struct pcfetcher_string* str = (struct pcfetcher_string*) item;
        fprintf(stderr, "..................i=%ld|str=%s\n", i, str->buffer);
    }

    delete ipc_encoder;
    pcfetcher_decoder_destroy(decoder);
    pcfetcher_string_array_destroy(array);
#endif

#endif

    return 0;
}
