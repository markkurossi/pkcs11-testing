/*
 * Copyright (c) 2021 Markku Rossi.
 *
 * All rights reserved.
 */

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
 import.c

 Functions for testing key import
*****************************************************************************/

#include "import.h"
#include "error.h"
#include "publickey.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


extern CK_FUNCTION_LIST_PTR p11;

int
testRSAImport(CK_SESSION_HANDLE hSession)
{
  CK_RV rv;
  int retVal = 0;
  CK_BYTE id[] = { 123 };
  CK_BBOOL ckTrue = CK_TRUE, ckFalse = CK_FALSE;
  CK_BYTE label[] = "label";
  CK_OBJECT_CLASS pubClass = CKO_PUBLIC_KEY, privClass = CKO_PRIVATE_KEY;
  CK_KEY_TYPE keyType = CKK_RSA;
  CK_OBJECT_HANDLE hPublicKey, hPrivateKey;

  CK_BYTE n_1024[] =
    {
      0xB8, 0xA4, 0xAF, 0x0A, 0xE1, 0x26, 0xD2, 0x04,
      0x72, 0xCD, 0xC3, 0xF0, 0x00, 0x83, 0x1F, 0x02,
      0x00, 0xC8, 0x5F, 0x0B, 0xCC, 0xE5, 0x87, 0xCC,
      0xF1, 0x82, 0x91, 0xE4, 0x65, 0x81, 0x2E, 0x8C,
      0xAF, 0x87, 0xF1, 0x00, 0x92, 0x96, 0x81, 0x1E,
      0xB6, 0x52, 0xA4, 0x28, 0x35, 0x16, 0xBB, 0xA2,
      0x51, 0xA1, 0xB8, 0xDE, 0xB0, 0x6F, 0x2A, 0x37,
      0x97, 0xF4, 0x8C, 0x56, 0x60, 0x7A, 0xC4, 0x0A,
      0x20, 0x75, 0xED, 0x3D, 0x9B, 0xE3, 0xCF, 0x87,
      0x96, 0x5C, 0xF6, 0x0C, 0xB3, 0xB4, 0x17, 0xF3,
      0xAA, 0x13, 0x4D, 0x3E, 0x6F, 0xDD, 0xA7, 0xF9,
      0x0B, 0xD3, 0x6E, 0xFD, 0xA5, 0x5C, 0x59, 0x93,
      0x7D, 0xFC, 0x51, 0xFE, 0x61, 0x0C, 0xEB, 0xDE,
      0x94, 0x21, 0x0F, 0x98, 0xE8, 0x86, 0x03, 0x13,
      0x0E, 0x33, 0xAF, 0x16, 0x32, 0x47, 0xA0, 0xC7,
      0x24, 0x51, 0xFB, 0x3F, 0x0C, 0x6B, 0x99, 0x79,
    };
  CK_BYTE e_1024[] =
    {
      0x01, 0x00, 0x01
    };
  CK_BYTE d_1024[] =
    {
      0x82, 0xB8, 0xA4, 0xEC, 0x70, 0xE6, 0x88, 0xFD,
      0x79, 0x41, 0xD0, 0x1C, 0x54, 0x60, 0x80, 0x4D,
      0x6C, 0xBC, 0x6E, 0xFD, 0xED, 0xBB, 0xFA, 0xDE,
      0xCF, 0x84, 0xFF, 0x40, 0xD1, 0xD4, 0x19, 0x5E,
      0xA4, 0xCE, 0xFB, 0x82, 0xCA, 0x45, 0x1F, 0x78,
      0xDC, 0xDF, 0xB9, 0x34, 0x76, 0x11, 0x78, 0x19,
      0xBC, 0xED, 0x5F, 0xF2, 0xD8, 0xBA, 0x7B, 0x0B,
      0x0C, 0xDB, 0xA7, 0x97, 0x67, 0x8A, 0xC2, 0xCA,
      0xAC, 0x6E, 0xFA, 0x9F, 0x79, 0x8A, 0xE4, 0x7F,
      0x3A, 0x44, 0xFA, 0xC0, 0x03, 0xF2, 0x7A, 0xD9,
      0x0E, 0xB1, 0x2A, 0x0C, 0xCA, 0x61, 0x9F, 0x09,
      0xF1, 0x71, 0x4D, 0x8E, 0xAE, 0x6C, 0x5F, 0x8C,
      0x12, 0x4F, 0x64, 0x9A, 0xFC, 0x14, 0x3F, 0xCB,
      0x2B, 0xB8, 0x65, 0x68, 0x7B, 0xCD, 0xF7, 0x25,
      0x67, 0xD5, 0xC4, 0x8C, 0x77, 0xD5, 0x76, 0xC1,
      0xA7, 0xC5, 0xF1, 0x81, 0xE2, 0xCE, 0x7D, 0x01
  };
  CK_BYTE p_1024[] =
    {
      0xE3, 0xB8, 0x61, 0xA8, 0x68, 0x93, 0x01, 0xAA,
      0x65, 0xF5, 0x94, 0x17, 0x7A, 0x34, 0xAC, 0xCD,
      0x8B, 0xFC, 0x1B, 0xD1, 0xB8, 0xF4, 0x4E, 0xCF,
      0xA1, 0xB3, 0xD7, 0xFE, 0x29, 0xC2, 0xC4, 0x28,
      0x23, 0x3D, 0xDB, 0x52, 0xB6, 0xCA, 0x30, 0x2D,
      0x7B, 0xDD, 0x71, 0xFC, 0x9D, 0x23, 0xFE, 0x01,
      0x01, 0x47, 0x6E, 0xCD, 0xA3, 0x22, 0xD5, 0x9F,
      0x23, 0xB9, 0xB6, 0xB5, 0xF8, 0x40, 0x25, 0x49,
    };
  CK_BYTE q_1024[] =
    {
      0xCF, 0x92, 0xCF, 0xE2, 0xFE, 0xF9, 0x8C, 0xEF,
      0x04, 0x70, 0x7D, 0xED, 0x10, 0xE0, 0x14, 0x95,
      0x0B, 0x21, 0x9D, 0xF4, 0x27, 0xBB, 0x0C, 0x7E,
      0x48, 0x20, 0xFC, 0x5A, 0xAA, 0x2F, 0x07, 0xC0,
      0x06, 0xC8, 0x2E, 0x28, 0x63, 0xB2, 0x43, 0xA2,
      0x77, 0x11, 0x5E, 0x05, 0xB1, 0x96, 0x11, 0x03,
      0x88, 0xBA, 0x86, 0x85, 0x07, 0x53, 0x69, 0xBB,
      0xC4, 0xBE, 0x4F, 0x26, 0xEB, 0xCD, 0x42, 0xB1,
    };
  CK_BYTE dp_1024[] =
    {
      0xE1, 0xA5, 0x56, 0xBF, 0xB5, 0xBF, 0xD5, 0xAA,
      0xAF, 0x95, 0x84, 0xC5, 0x2E, 0xC6, 0xFB, 0x64,
      0x42, 0x0D, 0xF2, 0xD3, 0x01, 0xDE, 0xB4, 0x29,
      0x72, 0x25, 0xEF, 0x25, 0x5E, 0xE4, 0x2B, 0xD3,
      0x49, 0xF8, 0x85, 0x57, 0x94, 0xE1, 0x0C, 0x94,
      0x45, 0x64, 0xCD, 0x0A, 0x6F, 0xFB, 0xB6, 0x39,
      0xF0, 0xB6, 0x83, 0xC9, 0x28, 0x99, 0xDF, 0xEB,
      0xED, 0x4E, 0x9D, 0x09, 0xFB, 0x90, 0x34, 0x61,
    };
  CK_BYTE dq_1024[] =
    {
      0x20, 0x94, 0xEB, 0xDA, 0xB6, 0xA9, 0x17, 0x2D,
      0x21, 0x66, 0x42, 0xED, 0xE6, 0x3B, 0x36, 0x1F,
      0xDA, 0xFA, 0x70, 0xDF, 0xB5, 0x28, 0xD0, 0x22,
      0x36, 0x52, 0x2F, 0x2A, 0x50, 0x12, 0x47, 0xC6,
      0x25, 0x8A, 0x60, 0x6C, 0x68, 0x91, 0xBF, 0xBA,
      0x0D, 0xB4, 0x85, 0x2C, 0xA1, 0x9E, 0x6B, 0x97,
      0x41, 0x6B, 0xFD, 0xD7, 0x7D, 0xB9, 0xE3, 0x1B,
      0xDD, 0x1A, 0xFE, 0x33, 0x9E, 0xC9, 0xBE, 0x51,
    };
  CK_BYTE iqmp_1024[] =
    {
      0xD9, 0x17, 0xBF, 0x9B, 0x88, 0xF1, 0x39, 0x97,
      0xE6, 0xA8, 0xC1, 0x69, 0xC1, 0xEA, 0xD8, 0x71,
      0xC2, 0x71, 0xED, 0x96, 0x7E, 0xBA, 0x8B, 0x83,
      0x52, 0x96, 0x1D, 0x1C, 0x61, 0x8F, 0xC7, 0x84,
      0x4D, 0xF8, 0xEC, 0xCD, 0x4D, 0x6F, 0x6C, 0xFD,
      0xA3, 0x41, 0x8F, 0xEC, 0x1F, 0xA0, 0xA7, 0xE2,
      0xD2, 0x78, 0x17, 0x17, 0xEF, 0x91, 0x22, 0x11,
      0xD3, 0x60, 0x1B, 0xEA, 0x58, 0xBA, 0x7F, 0xE4,
    };
  CK_ATTRIBUTE pubTemplate_1024[] =
    {
      { CKA_CLASS,		&pubClass,	sizeof(pubClass) },
      { CKA_KEY_TYPE,		&keyType,	sizeof(keyType) },
      { CKA_LABEL,		label,		sizeof(label) },
      { CKA_ID,			id,		sizeof(id) },
      { CKA_TOKEN,		&ckTrue,	sizeof(ckTrue) },
      { CKA_VERIFY,		&ckTrue,	sizeof(ckTrue) },
      { CKA_ENCRYPT,		&ckFalse,	sizeof(ckFalse) },
      { CKA_WRAP,		&ckFalse,	sizeof(ckFalse) },
      { CKA_PUBLIC_EXPONENT,	e_1024,		sizeof(e_1024) },
      { CKA_MODULUS,		n_1024,		sizeof(n_1024) }
    };
  CK_ATTRIBUTE privTemplate_1024[] =
    {
      { CKA_CLASS,		&privClass,	sizeof(privClass) },
      { CKA_KEY_TYPE,		&keyType,	sizeof(keyType) },
      { CKA_LABEL,		label,		sizeof(label) },
      { CKA_ID,			id,		sizeof(id) },
      { CKA_SIGN,		&ckTrue,	sizeof(ckTrue) },
      { CKA_DECRYPT,		&ckFalse,	sizeof(ckFalse) },
      { CKA_UNWRAP,		&ckFalse,	sizeof(ckFalse) },
      { CKA_SENSITIVE,		&ckTrue,	sizeof(ckTrue) },
      { CKA_TOKEN,		&ckTrue,	sizeof(ckTrue) },
      { CKA_PRIVATE,		&ckTrue,	sizeof(ckTrue) },
      { CKA_EXTRACTABLE,	&ckFalse,	sizeof(ckFalse) },
      { CKA_PUBLIC_EXPONENT,	e_1024,		sizeof(e_1024) },
      { CKA_MODULUS,		n_1024,		sizeof(n_1024) },
      { CKA_PRIVATE_EXPONENT,	d_1024,		sizeof(d_1024) },
      { CKA_PRIME_1,		p_1024,		sizeof(p_1024) },
      { CKA_PRIME_2,		q_1024,		sizeof(q_1024) },
      { CKA_EXPONENT_1,		dp_1024,	sizeof(dp_1024) },
      { CKA_EXPONENT_2,		dq_1024,	sizeof(dq_1024) },
      { CKA_COEFFICIENT,	iqmp_1024,	sizeof(iqmp_1024) }
    };

  CK_BYTE n_1025[] =
    {
      0x01, 0x97, 0xDC, 0xA8, 0x7E, 0xBA, 0xA7, 0x5E,
      0xB3, 0xEE, 0x4B, 0x83, 0x54, 0x91, 0x29, 0x33,
      0xFC, 0x9D, 0x04, 0x6E, 0x12, 0xCF, 0xDF, 0x8B,
      0xA8, 0x05, 0x6B, 0xBF, 0x83, 0x8C, 0x22, 0xA3,
      0xED, 0x06, 0xAE, 0x9E, 0x6D, 0x1B, 0x6B, 0x7A,
      0x8B, 0xEA, 0x57, 0xD0, 0xB9, 0x9B, 0x57, 0x33,
      0xFA, 0xD7, 0x56, 0x2C, 0x1E, 0x82, 0x77, 0x3B,
      0x01, 0x07, 0x6D, 0xB0, 0x86, 0xC1, 0x85, 0x2B,
      0x18, 0x6F, 0x96, 0x01, 0xF9, 0xB1, 0x13, 0x76,
      0x90, 0x2F, 0xD0, 0x02, 0xA2, 0xE4, 0x58, 0xA7,
      0x19, 0xB5, 0xC3, 0x66, 0x7D, 0xE6, 0x68, 0x97,
      0x30, 0x16, 0xE9, 0x96, 0x1E, 0x6B, 0xFF, 0x77,
      0x7E, 0xEE, 0x92, 0x6E, 0x1A, 0xEE, 0x34, 0x77,
      0x2A, 0xDE, 0xF7, 0xD0, 0x48, 0x9B, 0x04, 0x9E,
      0x57, 0x74, 0x3E, 0x9E, 0x41, 0xBD, 0x5A, 0xCC,
      0xD6, 0xC7, 0x32, 0x31, 0x2D, 0x8E, 0x99, 0x0D,
      0x45,
    };
  CK_BYTE e_1025[] =
    {
      0x01, 0x00, 0x01
    };
  CK_BYTE d_1025[] =
    {
      0x01, 0x87, 0x73, 0x6C, 0xF4, 0x54, 0xC9, 0x16,
      0x87, 0xB3, 0x0A, 0x1E, 0xBE, 0x27, 0xA4, 0x58,
      0x14, 0xF7, 0xCE, 0xAC, 0xA6, 0xB9, 0x84, 0x60,
      0x51, 0x9F, 0x02, 0x4F, 0x09, 0x3E, 0x92, 0x70,
      0xAA, 0xA9, 0x63, 0x27, 0x02, 0xA8, 0xF2, 0x7A,
      0xE7, 0x96, 0xBF, 0x39, 0xC1, 0x2A, 0x6A, 0x83,
      0xA5, 0x18, 0xCF, 0xC8, 0x00, 0x70, 0x4F, 0x66,
      0xFE, 0x11, 0xD7, 0x21, 0x39, 0xF4, 0xBA, 0x8F,
      0x23, 0x83, 0x3A, 0x39, 0xB2, 0x07, 0x63, 0x76,
      0x8D, 0x6D, 0x57, 0xCD, 0x3F, 0x34, 0x73, 0x0E,
      0xFC, 0x17, 0xEC, 0x09, 0xCA, 0x7F, 0x6E, 0x64,
      0x9C, 0x38, 0x0E, 0x56, 0xCF, 0xF2, 0xA0, 0x4B,
      0x78, 0xD0, 0x0E, 0xD9, 0xFC, 0x8A, 0xEE, 0x5E,
      0x71, 0xA9, 0x53, 0x1B, 0x1A, 0xE2, 0xC7, 0xD9,
      0x81, 0xEB, 0x9A, 0x34, 0xC0, 0x1B, 0x87, 0xDE,
      0x62, 0x68, 0xF6, 0x35, 0xA2, 0x13, 0xE7, 0xC9,
      0x81,
    };
  CK_BYTE p_1025[] =
    {
      0x01, 0xE0, 0x42, 0x6B, 0x29, 0x30, 0x21, 0x4C,
      0xB1, 0x86, 0xBF, 0x1C, 0x7F, 0x75, 0x9D, 0x55,
      0x8A, 0x67, 0x33, 0x56, 0x44, 0xCD, 0x2D, 0xDF,
      0x0E, 0x5A, 0x98, 0x58, 0x22, 0xC7, 0x8C, 0x40,
      0x74, 0x78, 0x67, 0xE1, 0x17, 0x80, 0x62, 0x57,
      0xBA, 0xA3, 0x84, 0x6C, 0x18, 0xCC, 0xCE, 0x41,
      0x74, 0x92, 0xA5, 0xC1, 0x01, 0xB1, 0xB8, 0x72,
      0xFC, 0x16, 0xF6, 0xAF, 0x16, 0xB9, 0x0D, 0xB1,
      0x1D,
    };
  CK_BYTE q_1025[] =
    {
      0xD9, 0x68, 0xAA, 0xEF, 0x6E, 0x50, 0xD6, 0xCA,
      0x4F, 0xA2, 0xD2, 0x15, 0xF2, 0x2A, 0x6A, 0x10,
      0x79, 0xAF, 0xFB, 0x29, 0xB5, 0x8B, 0x62, 0xB3,
      0x74, 0x16, 0xF7, 0x0A, 0xE4, 0x05, 0x8F, 0xDB,
      0xE7, 0x3B, 0x19, 0x5D, 0x35, 0x59, 0xBE, 0x8A,
      0x75, 0xBE, 0xE1, 0x0B, 0x76, 0x18, 0x4A, 0xF8,
      0xA4, 0x90, 0x48, 0x8A, 0x7D, 0xEB, 0x25, 0x81,
      0x61, 0x4A, 0x04, 0xE9, 0x9E, 0x6D, 0xFC, 0x49,
    };
  CK_BYTE dp_1025[] =
    {
      0x62, 0x57, 0xB7, 0x71, 0xDB, 0xB5, 0x35, 0xEB,
      0x34, 0x58, 0x76, 0x11, 0x73, 0x98, 0x20, 0x28,
      0x13, 0x31, 0xE3, 0xFC, 0x7A, 0xB7, 0x65, 0xF1,
      0x9E, 0x83, 0x8B, 0xA8, 0xB3, 0x8B, 0xF8, 0xB6,
      0xE0, 0xE1, 0x04, 0xAA, 0xB7, 0x53, 0x6B, 0xFB,
      0x7F, 0xF8, 0x61, 0xDA, 0x42, 0x36, 0x0A, 0x19,
      0x40, 0xDE, 0x48, 0x43, 0x7C, 0x8C, 0xF4, 0x9E,
      0xC2, 0x65, 0x09, 0x3D, 0x71, 0xEF, 0x67, 0xD1,
    };
  CK_BYTE dq_1025[] =
    {
      0x1C, 0xD2, 0x33, 0x56, 0xEA, 0x58, 0xED, 0x20,
      0x84, 0xCE, 0x20, 0xEB, 0xA7, 0xDE, 0xD7, 0x90,
      0xB7, 0x91, 0x0C, 0xCA, 0xCA, 0xB6, 0x5F, 0xAF,
      0x4A, 0x84, 0x80, 0xDB, 0x80, 0xBE, 0xFC, 0x03,
      0x3A, 0xF4, 0x2A, 0xB8, 0xA0, 0x89, 0xF5, 0x28,
      0x85, 0xD3, 0x78, 0x21, 0xFC, 0xD5, 0xDB, 0x9A,
      0x7A, 0xB6, 0x65, 0xE7, 0x76, 0x29, 0xE6, 0x1E,
      0xF0, 0xB6, 0xCF, 0x4F, 0xE9, 0x20, 0xCB, 0x81,
    };
  CK_BYTE iqmp_1025[] =
    {
      0x01, 0x6D, 0x82, 0xC6, 0x60, 0xF1, 0xA2, 0xAA,
      0xC2, 0xFA, 0x72, 0xF2, 0x23, 0x7D, 0x95, 0xBD,
      0xCD, 0xE6, 0x4D, 0x52, 0x2E, 0x80, 0xDD, 0x44,
      0x10, 0x1E, 0x6C, 0xE8, 0x20, 0x96, 0xA4, 0x51,
      0x36, 0x95, 0x17, 0x99, 0x8D, 0x32, 0x77, 0xD5,
      0xF3, 0x38, 0x86, 0x96, 0xF8, 0xD5, 0x73, 0xD0,
      0x8B, 0xFA, 0x70, 0xA2, 0x57, 0x0D, 0xF8, 0x0D,
      0x61, 0x9E, 0x30, 0x45, 0x88, 0x1A, 0xF1, 0x5E,
      0x8E,
    };
  CK_ATTRIBUTE pubTemplate_1025[] =
    {
      { CKA_CLASS,		&pubClass,	sizeof(pubClass) },
      { CKA_KEY_TYPE,		&keyType,	sizeof(keyType) },
      { CKA_LABEL,		label,		sizeof(label) },
      { CKA_ID,			id,		sizeof(id) },
      { CKA_TOKEN,		&ckTrue,	sizeof(ckTrue) },
      { CKA_VERIFY,		&ckTrue,	sizeof(ckTrue) },
      { CKA_ENCRYPT,		&ckFalse,	sizeof(ckFalse) },
      { CKA_WRAP,		&ckFalse,	sizeof(ckFalse) },
      { CKA_PUBLIC_EXPONENT,	e_1025,		sizeof(e_1025) },
      { CKA_MODULUS,		n_1025,		sizeof(n_1025) }
    };
  CK_ATTRIBUTE privTemplate_1025[] =
    {
      { CKA_CLASS,		&privClass,	sizeof(privClass) },
      { CKA_KEY_TYPE,		&keyType,	sizeof(keyType) },
      { CKA_LABEL,		label,		sizeof(label) },
      { CKA_ID,			id,		sizeof(id) },
      { CKA_SIGN,		&ckTrue,	sizeof(ckTrue) },
      { CKA_DECRYPT,		&ckFalse,	sizeof(ckFalse) },
      { CKA_UNWRAP,		&ckFalse,	sizeof(ckFalse) },
      { CKA_SENSITIVE,		&ckTrue,	sizeof(ckTrue) },
      { CKA_TOKEN,		&ckTrue,	sizeof(ckTrue) },
      { CKA_PRIVATE,		&ckTrue,	sizeof(ckTrue) },
      { CKA_EXTRACTABLE,	&ckFalse,	sizeof(ckFalse) },
      { CKA_PUBLIC_EXPONENT,	e_1025,		sizeof(e_1025) },
      { CKA_MODULUS,		n_1025,		sizeof(n_1025) },
      { CKA_PRIVATE_EXPONENT,	d_1025,		sizeof(d_1025) },
      { CKA_PRIME_1,		p_1025,		sizeof(p_1025) },
      { CKA_PRIME_2,		q_1025,		sizeof(q_1025) },
      { CKA_EXPONENT_1,		dp_1025,	sizeof(dp_1025) },
      { CKA_EXPONENT_2,		dq_1025,	sizeof(dq_1025) },
      { CKA_COEFFICIENT,	iqmp_1025,	sizeof(iqmp_1025) }
    };

  printf("\n************************************\n");
  printf("* Test for importing RSA key pairs *\n");
  printf("************************************\n\n");
  printf("This test will try to import 1024 bit and 1025 bit\n");
  printf("key pairs. Some HSM:s might not accept the odd size 1025.\n");
  printf("Also verify that the HSM set the key lengths correctly.\n\n");

  printf("Importing 1024 bit public RSA key: ");
  rv = p11->C_CreateObject(hSession, pubTemplate_1024, 10, &hPublicKey);
  if (rv != CKR_OK)
    {
      printf("Failed to import. rv=%s\n", rv2string(rv));
      return 1;
    }
  printf("OK\n");

  printf("Importing 1024 bit private RSA key: ");
  rv = p11->C_CreateObject(hSession, privTemplate_1024, 19, &hPrivateKey);
  if (rv != CKR_OK)
    {
      printf("Failed to import. rv=%s\n", rv2string(rv));
      p11->C_DestroyObject(hSession, hPublicKey);
      return 1;
    }
  printf("OK\n");

  if (testRSAImport_size(hSession, hPublicKey)) retVal = 1;
  if (testRSAImport_signverify(hSession, hPublicKey, hPrivateKey)) retVal = 1;

  p11->C_DestroyObject(hSession, hPublicKey);
  p11->C_DestroyObject(hSession, hPrivateKey);

  printf("Importing 1025 bit public RSA key: ");
  rv = p11->C_CreateObject(hSession, pubTemplate_1025, 10, &hPublicKey);
  if (rv != CKR_OK)
    {
      printf("Failed to import. rv=%s\n", rv2string(rv));
      return 1;
    }
  printf("OK\n");

  printf("Importing 1025 bit private RSA key: ");
  rv = p11->C_CreateObject(hSession, privTemplate_1025, 19, &hPrivateKey);
  if (rv != CKR_OK)
    {
      printf("Failed to import. rv=%s\n", rv2string(rv));
      p11->C_DestroyObject(hSession, hPublicKey);
      return 1;
    }
  printf("OK\n");

  if (testRSAImport_size(hSession, hPublicKey)) retVal = 1;
  if (testRSAImport_signverify(hSession, hPublicKey, hPrivateKey)) retVal = 1;

  p11->C_DestroyObject(hSession, hPublicKey);
  p11->C_DestroyObject(hSession, hPrivateKey);

  return retVal;
}

int
testRSAImport_size(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hPublicKey)
{
  CK_RV rv;
  CK_ULONG modulus_bits, bits;
  int mask;
  CK_BYTE_PTR modulus = NULL;
  CK_ATTRIBUTE template1[] =
    {
      { CKA_MODULUS_BITS, &modulus_bits, sizeof(CK_ULONG) }
    };
  CK_ATTRIBUTE template2[] =
    {
      { CKA_MODULUS, NULL_PTR, 0 }
    };

  int retVal = 0;

  printf("Key size from CKA_MODULUS_BITS in public key: ");

  // Get value
  rv = p11->C_GetAttributeValue(hSession, hPublicKey, template1,  1);
  if (rv != CKR_OK)
    {
      printf("Failed to get attribute. rv=%s\n", rv2string(rv));
      modulus_bits = 0;
      retVal = 1;
    }
  else
    {
      printf("%lu bits\n", modulus_bits);
    }

  printf("Key size from CKA_MODULUS in public key: ");

  // Get buffer sizes
  rv = p11->C_GetAttributeValue(hSession, hPublicKey, template2,  1);
  if (rv != CKR_OK)
    {
      printf("Failed to get the size of the attribute. rv=%s\n", rv2string(rv));
      return 1;
    }

  // Allocate memory
  modulus = (CK_BYTE_PTR)malloc(template2[0].ulValueLen);
  template2[0].pValue = modulus;
  if (modulus == NULL)
    {
      printf("Failed to allocate memory\n");
      return 1;
    }

  // Get the attribute
  rv = p11->C_GetAttributeValue(hSession, hPublicKey, template2,  1);
  if (rv != CKR_OK)
    {
      printf("Failed to get the attribute. rv=%s\n", rv2string(rv));
      free(modulus);
      return 1;
    }

  // Calculate size
  bits = template2[0].ulValueLen * 8;
  mask = 0x80;
  for (int i = 0; bits && (modulus[i] & mask) == 0; bits--)
    {
      mask >>= 1;
      if (mask == 0)
        {
          i++;
          mask = 0x80;
        }
    }
  free(modulus);

  printf("%lu bits\n", bits);

  if (bits == modulus_bits)
    {
      printf("Equal bit length: Yes\n");
    }
  else
    {
      printf("Equal bit length: No\n");
      retVal = 1;
    }

  return retVal;
}

int
testRSAImport_signverify(CK_SESSION_HANDLE hSession,
                         CK_OBJECT_HANDLE hPublicKey,
                         CK_OBJECT_HANDLE hPrivateKey)
{
  CK_RV rv;
  CK_MECHANISM mechanism = { CKM_RSA_PKCS, NULL_PTR, 0 };
  CK_BYTE_PTR pSignature;
  CK_ULONG pSignature_len;
  CK_BYTE data[] = {"Text"};
  int retVal = 0;

  printf("Create signature: ");

  rv = p11->C_SignInit(hSession, &mechanism, hPrivateKey);
  if (rv != CKR_OK)
    {
      printf("Failed to initialize signing. rv=%s\n", rv2string(rv));
      return 1;
    }

  rv = p11->C_Sign(hSession, data, sizeof(data)-1, NULL_PTR, &pSignature_len);
  if (rv != CKR_OK)
    {
      printf("Failed to get the length of the signature. rv=%s\n",
             rv2string(rv));
      return 1;
    }

  pSignature = (CK_BYTE_PTR)malloc(pSignature_len);
  if (pSignature == NULL)
    {
      printf("Failed to allocate memory\n");
      return 1;
    }

  rv = p11->C_Sign(hSession, data, sizeof(data)-1, pSignature, &pSignature_len);
  if (rv != CKR_OK)
    {
      printf("Failed to sign data. rv=%s\n", rv2string(rv));
      free (pSignature);
      return 1;
    }

  printf("OK\n");
  printf("Verify signature: ");

  rv = p11->C_VerifyInit(hSession, &mechanism, hPublicKey);
  if (rv != CKR_OK)
    {
      printf("Failed to initialize verification. rv=%s\n", rv2string(rv));
      free (pSignature);
      return 1;
    }

  rv = p11->C_Verify(hSession, data, sizeof(data) - 1,
                     pSignature, pSignature_len);
  if (rv != CKR_OK)
    {
      printf("Failed to verify signature. rv=%s\n", rv2string(rv));
      printf("Signature: ");
      printBinBuffer(pSignature, pSignature_len);
      retVal = 1;
    }
  else
    {
      printf("OK\n");
    }

  free (pSignature);
  return retVal;
}
