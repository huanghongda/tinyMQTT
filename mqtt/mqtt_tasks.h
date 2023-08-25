//
// Created by just do it on 2023/8/24.
//

#ifndef TINYMQTT_MQTT_TASKS_H
#define TINYMQTT_MQTT_TASKS_H
#include "mqtt_packet.h"
#include "mqtt_types.h"
#include "mqtt_task_executor.h"
#include "base/mqtt_util.h"
#include <stdlib.h>


/*************** session operation tasks *****************/
typedef enum session_req_op_e
{
    SESSION_CONNECT,
    SESSION_DISCONNECT,
    SESSION_FORCE_CLOSE
} session_req_op;

typedef struct session_connect_req
{
    tmq_connect_pkt connect_pkt;
    tmq_tcp_conn_t* conn;
} session_connect_req;

typedef struct session_req
{
    session_req_op op;
    union
    {
        session_connect_req connect_req;
        tmq_session_t* session;
    };
} session_req;

typedef struct session_task_ctx
{
    tmq_broker_t* broker;
    session_req req;
}session_task_ctx;

/*************** topic operation tasks *****************/
typedef enum topic_req_op_e
{
    TOPIC_SUBSCRIBE,
    TOPIC_UNSUBSCRIBE
} topic_req_op;

typedef struct topic_req
{
    topic_req_op op;
    tmq_str_t client_id;
    union
    {
        tmq_subscribe_pkt subscribe_pkt;
        tmq_unsubscribe_pkt unsubscribe_pkt;
    };
} topic_req;

typedef struct topic_task_ctx
{
    tmq_broker_t* broker;
    topic_req req;
}topic_task_ctx;

/*************** publish task *****************/
typedef struct publish_req
{
    tmq_str_t topic;
    tmq_message message;
    uint8_t retain;
} publish_req;

typedef struct publish_task_ctx
{
    tmq_broker_t* broker;
    publish_req req;
}publish_task_ctx;

#endif //TINYMQTT_MQTT_TASKS_H