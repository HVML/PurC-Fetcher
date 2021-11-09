#include "pcfetcher_msg_init_param.h"

struct pcfetcher_msg_init_param* pcfetcher_msg_init_param_create(void)
{
    return (struct pcfetcher_msg_init_param*)
            calloc(sizeof(struct pcfetcher_msg_init_param), 1);
}

void pcfetcher_msg_init_param_destroy(struct pcfetcher_msg_msg_init_param* msg)
{
    if (!msg) {
        return;
    }
    pcfetcher_string_destroy(msg->name);
    pcfetcher_url_destroy(msg->url);
    pcfetcher_string_array_destroy(msg->addrs);
}

void pcfetcher_msg_init_param_encode(pcfetcher_encoder* encode, void* v)
{
    struct pcfetcher_msg_init_param* msg = (struct pcfetcher_msg_init_param*)v;
    pcfetcher_msg_header_encode(encoder, &msg->header);
    pcfetcher_base_encode(encoder, msg->id);
    pcfetcher_string_encode(encoder, msg->name);
    pcfetcher_url_encode(encoder, msg->url);
    pcfetcher_string_array_encode(encoder, msg->addrs);
}

bool pcfetcher_msg_init_param_decode(pcfetcher_decoder* decoder, void** v)
{
    struct pcfetcher_msg_init_param* msg = pcfetcher_msg_init_param_create()
    pcfetcher_msg_header_decoder(decoder, &msg->header);
    pcfetcher_base_decode(decoder, msg->id);
    pcfetcher_string_decode(decoder, &msg->name);
    pcfetcher_url_decode(decoder, &msg->url);
    pcfetcher_string_array_decode(decoder, &msg->addrs);
}

