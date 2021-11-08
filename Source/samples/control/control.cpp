#include "config.h"
#include "Connection.h"
#include "MessageNames.h"
#include "ProcessLauncher.h"
#include "NetworkProcessCreationParameters.h"
#include "NetworkProcessMessages.h"
#include "Encoder.h"

#include "capi/fetcher-msg.h"
#include "purc/purc-arraylist.h"

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
    struct pcutils_arrlist* array = pcutils_arrlist_new(NULL);

    uint8_t c1[] = "abcdef";
    struct pcfetcher_string s1 = {6, 1, c1};
    uint8_t c2[] = "123456";
    struct pcfetcher_string s2 = {6, 1, c2};
 //   uint8_t c3[] = "789abc";
//    struct pcfetcher_string s3 = {6, 1, c3};

    pcutils_arrlist_add(array, &s1);
    pcutils_arrlist_add(array, &s2);
//    pcutils_arrlist_add(array, &s3);

    struct pcfetcher_encoder* encoder = pcfetcher_encoder_create();

    struct pcfetcher_msg_header msg = {0, 0x12, 0x5678};
    pcfetcher_encode_msg_header(encoder, &msg);
    pcfetcher_encode_array(encoder, array, pcfetcher_encode_string);

    size_t len = 0;
    const uint8_t* buf = pcfetcher_encoder_get_buffer(encoder, &len);
    for (size_t i = 0; i < len; i++) {
        fprintf(stderr, "i=%ld|buf[i]=%d|buf[i]=%c\n", i, buf[i], buf[i]);
    }

#if 0
    IPC::Encoder* ipc_encoder = new IPC::Encoder(IPC::MessageName(0x12), 0x5678);

    Vector<String> vec;
    vec.append("abcdef");
    vec.append("123456");
    ipc_encoder->encode(vec);

    uint8_t* buf = ipc_encoder->buffer();
    size_t len = ipc_encoder->bufferSize();
    for (size_t i = 0; i < len; i++) {
        fprintf(stderr, "i=%ld|buf[i]=%d|buf[i]=%c\n", i, buf[i], buf[i]);
    }

    struct pcfetcher_decoder* decoder = pcfetcher_decoder_create(
            ipc_encoder->buffer(), ipc_encoder->bufferSize(), false);

    struct pcfetcher_msg_header msg;
    pcfetcher_decode_msg_header(decoder, &msg);

    struct pcfetcher_string* s;

    pcfetcher_decode_string(decoder, &s);
    fprintf(stderr, "s=%s\n", s->buffer);

    pcfetcher_destory_string(s);

    delete ipc_encoder;

    pcfetcher_decoder_destroy(decoder);
#endif

    pcutils_arrlist_free(array);
#endif

    return 0;
}
