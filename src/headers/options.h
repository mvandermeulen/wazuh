/*
 * Copyright (C) 2015-2019, Wazuh Inc.
 * May 17, 2019.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#ifndef __OPTIONS_H
#define __OPTIONS_H

/* Options attributes */
typedef struct _option_t {
    int def;
    int min;
    int max;
} option_t;

/* Syscheck options structure */
typedef struct _syscheck_option_t {
    option_t sleep;
    option_t sleep_after;
    option_t rt_delay;
    option_t max_fd_win_rt;
    option_t max_audit_entries;
    option_t default_max_depth;
    option_t symlink_scan_interval;
    option_t file_max_size;
    option_t logging;
} syscheck_option_t;

/* Rootcheck options structure */
typedef struct _rootcheck_option_t {
    option_t sleep;
} rootcheck_option_t;

/* SCA options structure */
typedef struct _sca_option_t {
    option_t request_db_interval;
    option_t remote_commands;
    option_t commands_timeout;
} sca_option_t;

/* Remote options structure */
typedef struct _remote_option_t {
    option_t recv_counter_flush;
    option_t comp_average_printout;
    option_t verify_msg_id;
    option_t pass_empty_keyfile;
    option_t sender_pool;
    option_t request_pool;
    option_t request_timeout;
    option_t response_timeout;
    option_t request_rto_sec;
    option_t request_rto_msec;
    option_t max_attempts;
    option_t shared_reload;
    option_t rlimit_nofile;
    option_t recv_timeout;
    option_t send_timeout;
    option_t nocmerged;
    option_t keyupdate_interval;
    option_t worker_pool;
    option_t state_interval;
    option_t guess_agent_group;
    option_t group_data_flush;
    option_t receive_chunk;
    option_t buffer_relax;
    option_t tcp_keepidle;
    option_t tcp_keepintvl;
    option_t tcp_keepcnt;
    option_t logging;
} remote_option_t;

/* Mail options structure */
typedef struct _mail_option_t {
    option_t strict_checking;
    option_t grouping;
    option_t full_subject;
    option_t geoip;
} mail_option_t;

/* Auth options structure */
typedef struct _auth_option_t {
    option_t timeout_sec;
    option_t timeout_usec;
    option_t logging;
} auth_option_t;

/* Client buffer options structures */
typedef struct _client_buffer_option_t {
    option_t tolerance;
    option_t min_eps;
    option_t warn_level;
    option_t normal_level;
} client_buffer_option_t;

/* Internal options structure */
typedef struct _option_set_t {
    syscheck_option_t syscheck;
    rootcheck_option_t rootcheck;
    sca_option_t sca;
    remote_option_t remote;
    mail_option_t mail;
    auth_option_t auth;
    client_buffer_option_t client_buffer;
} option_set_t;

extern const option_set_t options;

#endif