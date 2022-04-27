
#include "capi/fetcher.h"

#include <wtf/RunLoop.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    const char* def_url = "https://hybridos.fmsoft.cn";
    purc_instance_extra_info info = {};
    purc_init ("cn.fmsoft.hybridos.sample", "pcfetcher", &info);

    const char* url = argv[1] ? argv[1] : def_url;

    struct pcfetcher_resp_header resp_header;
    purc_rwstream_t resp = pcfetcher_request_sync(
        url,
        PCFETCHER_REQUEST_METHOD_GET,
        NULL,
        10,
        &resp_header);

    fprintf(stderr, "....................................\n");
    fprintf(stderr, "%s\n", url);
    fprintf(stderr, ".................head begin\n");
    fprintf(stderr, "ret_code=%d\n", resp_header.ret_code);
    fprintf(stderr, "mime_type=%s\n", resp_header.mime_type);
    fprintf(stderr, "sz_resp=%ld\n", resp_header.sz_resp);
    fprintf(stderr, ".................head end\n");
    fprintf(stderr, ".................body begin\n");
    if (resp) {
        size_t sz_content = 0;
        size_t sz_buffer = 0;
        char* buf = (char*)purc_rwstream_get_mem_buffer_ex(resp, &sz_content,
                &sz_buffer, false);
        fprintf(stderr, "buffer size=%ld\n", sz_buffer);
        fprintf(stderr, "body size=%ld|buflen=%ld|buf=%s\n", sz_content,
                buf ? strlen(buf) : 0, buf);
        fprintf(stderr, "buf p=%s\n", buf);
        fprintf(stderr, "%s\n", buf ? buf : NULL);
        purc_rwstream_destroy(resp);
    }

    if (resp_header.mime_type) {
        free(resp_header.mime_type);
    }
    fprintf(stderr, ".................body end\n");
    fprintf(stderr, "....................................\n");

    purc_cleanup();

    return 0;
}
