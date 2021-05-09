/* $Id$ */

/*
 * Copyright (c) 2010 .SE (The Internet Infrastructure Foundation)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*****************************************************************************
 error.cpp

 Function for getting error string
*****************************************************************************/

#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>


const char *
rv2string(CK_RV rv)
{
  switch (rv)
    {
    case CKR_OK:
      return "CKR_OK";
    case CKR_CANCEL:
      return "CKR_CANCEL";
    case CKR_HOST_MEMORY:
      return "CKR_HOST_MEMORY";
    case CKR_SLOT_ID_INVALID:
      return "CKR_SLOT_ID_INVALID";

    case CKR_GENERAL_ERROR:
      return "CKR_GENERAL_ERROR";
    case CKR_FUNCTION_FAILED:
      return "CKR_FUNCTION_FAILED";

    case CKR_ARGUMENTS_BAD:
      return "CKR_ARGUMENTS_BAD";
    case CKR_NO_EVENT:
      return "CKR_NO_EVENT";
    case CKR_NEED_TO_CREATE_THREADS:
      return "CKR_NEED_TO_CREATE_THREADS";
    case CKR_CANT_LOCK:
      return "CKR_CANT_LOCK";

    case CKR_ATTRIBUTE_READ_ONLY:
      return "CKR_ATTRIBUTE_READ_ONLY";
    case CKR_ATTRIBUTE_SENSITIVE:
      return "CKR_ATTRIBUTE_SENSITIVE";
    case CKR_ATTRIBUTE_TYPE_INVALID:
      return "CKR_ATTRIBUTE_TYPE_INVALID";
    case CKR_ATTRIBUTE_VALUE_INVALID:
      return "CKR_ATTRIBUTE_VALUE_INVALID";

#if 0
    case CKR_ACTION_PROHIBITED:
      return "CKR_ACTION_PROHIBITED";
#endif

    case CKR_DATA_INVALID:
      return "CKR_DATA_INVALID";
    case CKR_DATA_LEN_RANGE:
      return "CKR_DATA_LEN_RANGE";
    case CKR_DEVICE_ERROR:
      return "CKR_DEVICE_ERROR";
    case CKR_DEVICE_MEMORY:
      return "CKR_DEVICE_MEMORY";
    case CKR_DEVICE_REMOVED:
      return "CKR_DEVICE_REMOVED";
    case CKR_ENCRYPTED_DATA_INVALID:
      return "CKR_ENCRYPTED_DATA_INVALID";
    case CKR_ENCRYPTED_DATA_LEN_RANGE:
      return "CKR_ENCRYPTED_DATA_LEN_RANGE";
#if 0
    case CKR_AEAD_DECRYPT_FAILED:
      return "CKR_AEAD_DECRYPT_FAILED";
#endif
    case CKR_FUNCTION_CANCELED:
      return "CKR_FUNCTION_CANCELED";
    case CKR_FUNCTION_NOT_PARALLEL:
      return "CKR_FUNCTION_NOT_PARALLEL";

    case CKR_FUNCTION_NOT_SUPPORTED:
      return "CKR_FUNCTION_NOT_SUPPORTED";

    case CKR_KEY_HANDLE_INVALID:
      return "CKR_KEY_HANDLE_INVALID";

    case CKR_KEY_SIZE_RANGE:
      return "CKR_KEY_SIZE_RANGE";
    case CKR_KEY_TYPE_INCONSISTENT:
      return "CKR_KEY_TYPE_INCONSISTENT";

    case CKR_KEY_NOT_NEEDED:
      return "CKR_KEY_NOT_NEEDED";
    case CKR_KEY_CHANGED:
      return "CKR_KEY_CHANGED";
    case CKR_KEY_NEEDED:
      return "CKR_KEY_NEEDED";
    case CKR_KEY_INDIGESTIBLE:
      return "CKR_KEY_INDIGESTIBLE";
    case CKR_KEY_FUNCTION_NOT_PERMITTED:
      return "CKR_KEY_FUNCTION_NOT_PERMITTED";
    case CKR_KEY_NOT_WRAPPABLE:
      return "CKR_KEY_NOT_WRAPPABLE";
    case CKR_KEY_UNEXTRACTABLE:
      return "CKR_KEY_UNEXTRACTABLE";

    case CKR_MECHANISM_INVALID:
      return "CKR_MECHANISM_INVALID";
    case CKR_MECHANISM_PARAM_INVALID:
      return "CKR_MECHANISM_PARAM_INVALID";

    case CKR_OBJECT_HANDLE_INVALID:
      return "CKR_OBJECT_HANDLE_INVALID";
    case CKR_OPERATION_ACTIVE:
      return "CKR_OPERATION_ACTIVE";
    case CKR_OPERATION_NOT_INITIALIZED:
      return "CKR_OPERATION_NOT_INITIALIZED";
    case CKR_PIN_INCORRECT:
      return "CKR_PIN_INCORRECT";
    case CKR_PIN_INVALID:
      return "CKR_PIN_INVALID";
    case CKR_PIN_LEN_RANGE:
      return "CKR_PIN_LEN_RANGE";

    case CKR_PIN_EXPIRED:
      return "CKR_PIN_EXPIRED";
    case CKR_PIN_LOCKED:
      return "CKR_PIN_LOCKED";

    case CKR_SESSION_CLOSED:
      return "CKR_SESSION_CLOSED";
    case CKR_SESSION_COUNT:
      return "CKR_SESSION_COUNT";
    case CKR_SESSION_HANDLE_INVALID:
      return "CKR_SESSION_HANDLE_INVALID";
    case CKR_SESSION_PARALLEL_NOT_SUPPORTED:
      return "CKR_SESSION_PARALLEL_NOT_SUPPORTED";
    case CKR_SESSION_READ_ONLY:
      return "CKR_SESSION_READ_ONLY";
    case CKR_SESSION_EXISTS:
      return "CKR_SESSION_EXISTS";

    case CKR_SESSION_READ_ONLY_EXISTS:
      return "CKR_SESSION_READ_ONLY_EXISTS";
    case CKR_SESSION_READ_WRITE_SO_EXISTS:
      return "CKR_SESSION_READ_WRITE_SO_EXISTS";

    case CKR_SIGNATURE_INVALID:
      return "CKR_SIGNATURE_INVALID";
    case CKR_SIGNATURE_LEN_RANGE:
      return "CKR_SIGNATURE_LEN_RANGE";
    case CKR_TEMPLATE_INCOMPLETE:
      return "CKR_TEMPLATE_INCOMPLETE";
    case CKR_TEMPLATE_INCONSISTENT:
      return "CKR_TEMPLATE_INCONSISTENT";
    case CKR_TOKEN_NOT_PRESENT:
      return "CKR_TOKEN_NOT_PRESENT";
    case CKR_TOKEN_NOT_RECOGNIZED:
      return "CKR_TOKEN_NOT_RECOGNIZED";
    case CKR_TOKEN_WRITE_PROTECTED:
      return "CKR_TOKEN_WRITE_PROTECTED";
    case CKR_UNWRAPPING_KEY_HANDLE_INVALID:
      return "CKR_UNWRAPPING_KEY_HANDLE_INVALID";
    case CKR_UNWRAPPING_KEY_SIZE_RANGE:
      return "CKR_UNWRAPPING_KEY_SIZE_RANGE";
    case CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT:
      return "CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT";
    case CKR_USER_ALREADY_LOGGED_IN:
      return "CKR_USER_ALREADY_LOGGED_IN";
    case CKR_USER_NOT_LOGGED_IN:
      return "CKR_USER_NOT_LOGGED_IN";
    case CKR_USER_PIN_NOT_INITIALIZED:
      return "CKR_USER_PIN_NOT_INITIALIZED";
    case CKR_USER_TYPE_INVALID:
      return "CKR_USER_TYPE_INVALID";

    case CKR_USER_ANOTHER_ALREADY_LOGGED_IN:
      return "CKR_USER_ANOTHER_ALREADY_LOGGED_IN";
    case CKR_USER_TOO_MANY_TYPES:
      return "CKR_USER_TOO_MANY_TYPES";

    case CKR_WRAPPED_KEY_INVALID:
      return "CKR_WRAPPED_KEY_INVALID";
    case CKR_WRAPPED_KEY_LEN_RANGE:
      return "CKR_WRAPPED_KEY_LEN_RANGE";
    case CKR_WRAPPING_KEY_HANDLE_INVALID:
      return "CKR_WRAPPING_KEY_HANDLE_INVALID";
    case CKR_WRAPPING_KEY_SIZE_RANGE:
      return "CKR_WRAPPING_KEY_SIZE_RANGE";
    case CKR_WRAPPING_KEY_TYPE_INCONSISTENT:
      return "CKR_WRAPPING_KEY_TYPE_INCONSISTENT";
    case CKR_RANDOM_SEED_NOT_SUPPORTED:
      return "CKR_RANDOM_SEED_NOT_SUPPORTED";

    case CKR_RANDOM_NO_RNG:
      return "CKR_RANDOM_NO_RNG";

    case CKR_DOMAIN_PARAMS_INVALID:
      return "CKR_DOMAIN_PARAMS_INVALID";

#if 0
    case CKR_CURVE_NOT_SUPPORTED:
      return "CKR_CURVE_NOT_SUPPORTED";
#endif

    case CKR_BUFFER_TOO_SMALL:
      return "CKR_BUFFER_TOO_SMALL";
    case CKR_SAVED_STATE_INVALID:
      return "CKR_SAVED_STATE_INVALID";
    case CKR_INFORMATION_SENSITIVE:
      return "CKR_INFORMATION_SENSITIVE";
    case CKR_STATE_UNSAVEABLE:
      return "CKR_STATE_UNSAVEABLE";

    case CKR_CRYPTOKI_NOT_INITIALIZED:
      return "CKR_CRYPTOKI_NOT_INITIALIZED";
    case CKR_CRYPTOKI_ALREADY_INITIALIZED:
      return "CKR_CRYPTOKI_ALREADY_INITIALIZED";
    case CKR_MUTEX_BAD:
      return "CKR_MUTEX_BAD";
    case CKR_MUTEX_NOT_LOCKED:
      return "CKR_MUTEX_NOT_LOCKED";

#if 0
    case CKR_NEW_PIN_MODE:
      return "CKR_NEW_PIN_MODE";
    case CKR_NEXT_OTP:
      return "CKR_NEXT_OTP";
#endif

#if 0
    case CKR_EXCEEDED_MAX_ITERATIONS:
      return "CKR_EXCEEDED_MAX_ITERATIONS";
    case CKR_FIPS_SELF_TEST_FAILED:
      return "CKR_FIPS_SELF_TEST_FAILED";
    case CKR_LIBRARY_LOAD_FAILED:
      return "CKR_LIBRARY_LOAD_FAILED";
    case CKR_PIN_TOO_WEAK:
      return "CKR_PIN_TOO_WEAK";
    case CKR_PUBLIC_KEY_INVALID:
      return "CKR_PUBLIC_KEY_INVALID";
#endif

    case CKR_FUNCTION_REJECTED:
      return "CKR_FUNCTION_REJECTED";
#if 0
    case CKR_TOKEN_RESOURCE_EXCEEDED:
      return "CKR_TOKEN_RESOURCE_EXCEEDED";
#endif

    case CKR_VENDOR_DEFINED:
      return "CKR_VENDOR_DEFINED";

    default:
      return "Unknown error";
    }
}
