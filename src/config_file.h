/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef __CONFIG_FILE_H
#define __CONFIG_FILE_H

#include "detox.h"

extern int config_file_lineno;

extern struct detox_parse_results *parse_config_file(char *filename, struct detox_parse_results *previous_list, struct detox_options *main_options);

#endif				/* __CONFIG_FILE_H */
