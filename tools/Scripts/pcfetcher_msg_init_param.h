#include "pcfetcher_msg.h"

#include "pcfetcher_url.h"

struct pcfetcher_msg_init_param {
    struct pcfetcher_msg_header header;

    uint32_t id;
    struct pcfetcher_string* name;
    struct pcfetcher_url* url;

    // pcfetcher_string array
    struct pcutils_arrlist* addrs;
}

struct pcfetcher_msg_init_param* pcfetcher_msg_init_param_create(void);

void pcfetcher_msg_init_param_destroy(struct pcfetcher_msg_msg_init_param*);

void pcfetcher_msg_init_param_encode(pcfetcher_encoder*, void*);

bool pcfetcher_msg_init_param_decode(pcfetcher_decoder*, void**);


static inline void pcfetcher_msg_init_param_array_free_fn(void* v)
{
    pcfetcher_msg_init_param_destroy((struct pcfetcher_msg_init_param*)v);
}

static inline struct pcutils_arrlist* pcfetcher_msg_init_param_array_create(void)
{
    return pcfetcher_array_create(pcfetcher_msg_init_param_array_free_fn);
}

static inline void pcfetcher_%s_array_destory(struct pcutils_arrlist* array)
{
    pcfetcher_array_destroy(array);
}

static inline void pcfetcher_msg_init_param_array_encode(
        struct pcfetcher_encoder* encoder, struct pcutils_arrlist* array)
{
    pcfetcher_array_encode(encoder, array, pcfetcher_msg_init_param_encode);
}

static inline void pcfetcher_msg_init_param_array_decode(
        struct pcfetcher_decoder* decoder, struct pcutils_arrlist** array)
{
    pcfetcher_array_decode(decoder, array,
         pcfetcher_msg_init_param_array_create,
         pcfetcher_msg_init_param_decode);
}
