/*
 * Copyright 2019-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef OPENSSL_OPENSSLCONF_H
# define OPENSSL_OPENSSLCONF_H
# pragma once


#ifdef __aarch64__
#include <openssl/configuration-aarch64.h>
#elif __arm__
#include <openssl/configuration-arm.h>
#elif __i386__
#include <openssl/configuration-i386.h>
#elif __x86_64__
#include <openssl/configuration-x86_64.h>
#else
#error "Unsupported architecture!"
#endif
# include <openssl/macros.h>

#endif  /* OPENSSL_OPENSSLCONF_H */
