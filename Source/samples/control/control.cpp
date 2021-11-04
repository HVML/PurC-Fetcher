
#include "Connection.h"
#include "MessageNames.h"
#include <glib.h>
#include <wtf/glib/GLibUtilities.h>
#include <wtf/glib/GUniquePtr.h>
#include <wtf/RunLoop.h>

#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct _GSubprocessLauncher GSubprocessLauncher;

using namespace PurCFetcher;

static void childSetupFunction(gpointer userData)
{
    int socket = GPOINTER_TO_INT(userData);
    close(socket);
}

class ControlConnectClient : public IPC::Connection::Client {
    void didClose(IPC::Connection&) {}
    void didReceiveInvalidMessage(IPC::Connection&, IPC::MessageName) {}
};

int main(int argc, char** argv)
{
    fprintf(stderr, "argc=%d|argv[0]=%s\n", argc, argv[0]);
    char fetcher[] = "/tmp/fetcher";
    IPC::Connection::SocketPair socketPair = IPC::Connection::createPlatformConnection(IPC::Connection::ConnectionOptions::SetCloexecOnServer);
    UNUSED_PARAM(socketPair);

    GRefPtr<GSubprocessLauncher> launcher = adoptGRef(g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_INHERIT_FDS));
    g_subprocess_launcher_set_child_setup(launcher.get(), childSetupFunction, GINT_TO_POINTER(socketPair.server), nullptr);
    g_subprocess_launcher_take_fd(launcher.get(), socketPair.client, socketPair.client);

    GUniqueOutPtr<GError> error;
    GRefPtr<GSubprocess> process;
    pid_t processId  = getpid();
    GUniquePtr<gchar> processIdentifier(g_strdup_printf("%d" PRIu64, processId));
    GUniquePtr<gchar> webkitSocket(g_strdup_printf("%d", socketPair.client));
    char** pargv = g_newa(char*, 3);
    int i = 0;
    pargv[i++] = fetcher;
    pargv[i++] = processIdentifier.get();
    pargv[i++] = webkitSocket.get();

#if 1
    process = adoptGRef(g_subprocess_launcher_spawnv(launcher.get(), argv, &error.outPtr()));
    if (!process.get()) {
        fprintf(stderr, "................................................launcher process failed\n");
        return 1;
    }
#endif
    //fprintf(stderr, "...........................................fetcher=%s|socket=%s\n", pargv[0], pargv[2]);

    RunLoop::run();

    return 0;
}
