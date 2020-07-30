/* Copyright (C) 2015-2020, Wazuh Inc.
 * Copyright (C) 2009 Trend Micro Inc.
 * All right reserved.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */

#include "list_log.h"

os_analysisd_list_log_msg_t* os_analysisd_create_list_log() {
    os_analysisd_list_log_msg_t* list;
    os_calloc(1, sizeof(os_analysisd_list_log_msg_t), list);
    return list;
}

void _os_analysisd_add_list_log(os_analysisd_list_log_msg_t* list, int level, int line,
                                const char* func, const char* file, char* msg, ...) {

    va_list args;
    os_analysisd_log_msg_t* new_msg;
    os_malloc(sizeof(os_analysisd_log_msg_t), new_msg);
    new_msg->next = NULL;

    /* Debug information */
    new_msg->line = line;
    os_strdup(func, new_msg->func);
    os_strdup(file, new_msg->file);

    /* Generic message */
    new_msg->level = level;
    os_malloc(OS_BUFFER_SIZE, new_msg->msg);
    va_start(args, msg);
    (void)vsnprintf(new_msg->msg, OS_BUFFER_SIZE, msg, args);
    va_end(args);
    os_realloc(new_msg->msg, strlen(new_msg->msg) + 1, new_msg->msg);

    if (list->head == NULL) {
        list->head = new_msg;
    } else {
        os_analysisd_log_msg_t* current_node = list->head;

        while (current_node->next) {
            current_node = current_node->next;
        }
        current_node->next = new_msg;
    }

    return;
}

os_analysisd_log_msg_t* os_analysisd_pop_list_log(os_analysisd_list_log_msg_t* list) {

    os_analysisd_log_msg_t* first_node = list->head;

    if (!first_node) {
        return NULL;
    }
    list->head = first_node->next;
    first_node->next = NULL; // Prevents memory frees errors

    return first_node;
}

char* os_analysisd_string_log_msg(os_analysisd_log_msg_t* log_msg) {
    char* str;

    if (isDebug()) {
        os_malloc(OS_BUFFER_SIZE, str);
        (void)snprintf(str, OS_BUFFER_SIZE, "%s:%d at %s(): %s", log_msg->file,
                       log_msg->line, log_msg->func, log_msg->msg);
        os_realloc(str, strlen(str) + 1, str);
    } else {
        os_strdup(log_msg->msg, str);
    }

    return str;
}

void os_analysisd_free_log_msg(os_analysisd_log_msg_t* log_msg) {
    if(!log_msg){
        return;
    }

    os_free(log_msg->file);
    os_free(log_msg->func);
    os_free(log_msg->msg);
}