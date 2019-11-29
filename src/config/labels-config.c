/*
 * Label Configuration
 * Copyright (C) 2015-2019, Wazuh Inc.
 * February 20, 2017.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#include "shared.h"
#include "config.h"

const char *xml_label = "label";
const char *xml_key = "key";
const char *xml_hidden = "hidden";

int Read_Labels(XML_NODE node, void *d1, __attribute__((unused)) void *d2, char **output) {
    int i;
    int j;
    const char *key;
    char message[OS_FLSIZE];
    size_t labels_z = 0;
    wlabel_t **labels = (wlabel_t **)d1;
    label_flags_t flags;

    if (!*labels) {
        os_calloc(1, sizeof(wlabel_t), *labels);
    }

    /* Get label size */

    while ((*labels)[labels_z].key) {
        labels_z++;
    }

    for (i = 0; node[i]; i++) {
        if (!node[i]->element) {
            if (output == NULL) {
                merror(XML_ELEMNULL);
            } else {
                wm_strcat(output, "Invalid NULL element in the configuration.", '\n');
            }
            goto error;
        } else if (!node[i]->content) {
            if (output == NULL) {
                merror(XML_VALUENULL, node[i]->element);
            } else {
                snprintf(message, OS_FLSIZE,
                    "Invalid NULL content for element: %s.",
                    node[i]->element);
                wm_strcat(output, message, '\n');
            }
            goto error;
        } else if (strcmp(node[i]->element, xml_label) == 0) {
            key = NULL;
            flags.hidden = flags.system = 0;

            for (j = 0; node[i]->attributes && node[i]->attributes[j]; j++) {
                if (strcmp(node[i]->attributes[j], xml_key) == 0) {
                    if (strlen(node[i]->values[j]) > 0) {
                        if (node[i]->values[j][0] == '_'){
                            if (output == NULL) {
                                mwarn("Label keys starting with \"_\"  are reserved for internal use. Skipping label '%s'.",
                                    node[i]->values[j]);
                            } else {
                                snprintf(message, OS_FLSIZE,
                                    "WARNING: Label keys starting with \"_\"  are reserved for internal use. Skipping label '%s'.",
                                    node[i]->values[j]);
                                wm_strcat(output, message, '\n');
                            }
                            flags.system = 1;
                        }
                        key = node[i]->values[j];
                    } else if (output == NULL) {
                        merror("Label with empty key.");
                        goto error;
                    } else {
                        wm_strcat(output, "Label with empty key.", '\n');
                        goto error;
                    }
                } else if (strcmp(node[i]->attributes[j], xml_hidden) == 0) {
                    if (strcmp(node[i]->values[j], "yes") == 0)
                        flags.hidden = 1;
                    else if (strcmp(node[i]->values[j], "no") == 0)
                        flags.hidden = 0;
                    else if (output == NULL) {
                        merror("Invalid content for attribute '%s'.", node[i]->attributes[j]);
                        goto error;
                    } else {
                        snprintf(message, OS_FLSIZE,
                            "Invalid content for attribute '%s'.",
                            node[i]->attributes[j]);
                        wm_strcat(output, message, '\n');
                        goto error;
                    }
                }
            }

            // Skip labels with "_"
            if (flags.system == 1)
                continue;

            if (!key) {
                if (output == NULL) {
                    merror("Expected 'key' attribute for label.");
                } else {
                    wm_strcat(output, "Expected 'key' attribute for label.", '\n');
                }
                goto error;
            }

            if (strlen(node[i]->content) == 0) {
                if (output == NULL) {
                    mwarn("Label '%s' is empty.", key);
                } else {
                    snprintf(message, OS_FLSIZE,
                        "WARNING: Label '%s' is empty.", key);
                    wm_strcat(output, message, '\n');
                }
            }

            *labels = labels_add(*labels, &labels_z, key, node[i]->content, flags, 1);
        } else if (output == NULL) {
            merror(XML_INVELEM, node[i]->element);
            goto error;
        } else {
            snprintf(message, OS_FLSIZE,
                "Invalid element in the configuration: '%s'.",
                node[i]->element);
            wm_strcat(output, message, '\n');
            goto error;
        }
    }

    return 0;

error:
    //labels_free(*labels);
    return OS_INVALID;
}

int Test_Labels(const char *path, int type, char **output) {
    wlabel_t *test_labels = NULL;
    int fail = 0;

    if (ReadConfig(CLABELS | type, path, &test_labels, NULL, output) < 0) {
        if (output == NULL){
            merror(CONF_READ_ERROR, "Labels");
        } else {
            wm_strcat(output, "ERROR: Invalid configuration in Labels", '\n');
        }
        fail = OS_INVALID;
    }

    labels_free(test_labels);
    return fail;
}