//
// Created by zr on 23-6-2.
//

#ifndef TINYMQTT_MQTT_IO_CONTEXT_H
#define TINYMQTT_MQTT_IO_CONTEXT_H
#include "event/mqtt_event.h"
#include "mqtt_types.h"

#define MQTT_TCP_CHECKALIVE_INTERVAL    100
#define MQTT_CONNECT_MAX_PENDING        100
#define MQTT_TCP_MAX_IDLE               600

typedef void* tmq_mail_t;
typedef tmq_vec(tmq_mail_t) tmq_mail_list_t;
typedef struct tmq_mailbox_s tmq_mailbox_t;
typedef void(*mail_handler)(void* owner, tmq_mail_t mail);
typedef void(*mail_cleanup)(tmq_mail_t mail);

typedef struct tmq_mailbox_s {
    void* owner;
    mail_handler handler;
    tmq_notifier_t notifier;
    pthread_mutex_t lk;
    tmq_mail_list_t mailbox;
} tmq_mailbox_t;

void tmq_mailbox_init(tmq_mailbox_t* mailbox, tmq_event_loop_t* loop,
                      void* owner, mail_handler handler);
void tmq_mailbox_push(tmq_mailbox_t* mailbox, tmq_mail_t mail);

typedef tmq_map(char*, tmq_tcp_conn_t*) tcp_conn_map_t;
typedef tmq_vec(tmq_socket_t) tcp_conn_list_t;
typedef struct tmq_io_context_s
{
    tmq_broker_t* broker;
    pthread_t io_thread;
    int index;
    tmq_event_loop_t loop;
    tcp_conn_map_t tcp_conns;
    tmq_timerid_t tcp_checkalive_timer;
    tmq_timerid_t mqtt_keepalive_timer;

    tmq_mailbox_t pending_tcp_connections;
    tmq_mailbox_t mqtt_connect_responses;
    tmq_mailbox_t packet_sending_tasks;
    tmq_mailbox_t broadcast_tasks;

//    tcp_conn_list_t pending_tcp_conns;
//    connect_resp_list connect_resps;
//    packet_send_list sending_packets;
//
//    pthread_mutex_t pending_conns_lk;
//    pthread_mutex_t connect_resp_lk;
//    pthread_mutex_t sending_packets_lk;
//
//    tmq_notifier_t new_conn_notifier;
//    tmq_notifier_t connect_resp_notifier;
//    tmq_notifier_t sending_packets_notifier;
} tmq_io_context_t;

void tmq_io_context_init(tmq_io_context_t* context, tmq_broker_t* broker, int index);
void tmq_io_context_run(tmq_io_context_t* context);
void tmq_io_context_stop(tmq_io_context_t* context);

#endif //TINYMQTT_MQTT_IO_CONTEXT_H
