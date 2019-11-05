/*
**
** Copyright 2017, The Android Open Source Project
**
** This file is dual licensed.  It may be redistributed and/or modified
** under the terms of the Apache 2.0 License OR version 2 of the GNU
** General Public License.
*/

#pragma once

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// Replace this macro with android_log() invocation since we can't access 
// liblog internals ( __android_log_error_write ) from the outside of AOSP.
#define android_errorWriteLog(tag, subTag) \
{ \
    char tag_s[16]; \
    sprintf(tag_s, "0x%08x", (int32_t)tag); \
    __android_log_write(ANDROID_LOG_ERROR, tag_s, subTag); \
}

/*
#define android_errorWriteLog(tag, subTag) \
  __android_log_error_write(tag, subTag, -1, NULL, 0)

#define android_errorWriteWithInfoLog(tag, subTag, uid, data, dataLen) \
  __android_log_error_write(tag, subTag, uid, data, dataLen)

int __android_log_error_write(int tag, const char* subTag, int32_t uid,
                              const char* data, uint32_t dataLen);
*/
#ifdef __cplusplus
}
#endif
