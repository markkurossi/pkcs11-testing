/*
 * Copyright (c) 2021 Markku Rossi
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
 random.cpp

 Functions for random number generation tests
 *****************************************************************************/

#include "random.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>

extern CK_FUNCTION_LIST_PTR p11;

static int
testRandom_Seed(CK_SLOT_ID slotID, CK_SESSION_HANDLE hSession)
{
  CK_RV rv;
  int retVal = 0;
  unsigned char seed[1024];

  printf("\nTesting random number seeding: ");
  memset(seed, '*', sizeof(seed));
  rv = p11->C_SeedRandom(hSession, seed, sizeof(seed));
  switch (rv)
    {
    case CKR_OK:
      printf("OK\n");
      break;

    case CKR_RANDOM_SEED_NOT_SUPPORTED:
      printf("not supported\n");
      break;

    default:
      printf("failed: rv=%s\n", rv2string(rv));
      retVal = 1;
      break;
    }

  return retVal;
}

static int
testRandom_Generate(CK_SLOT_ID slotID, CK_SESSION_HANDLE hSession)
{
  CK_RV rv;
  int retVal = 0;
  unsigned char random[1024];

  printf("Testing random number generation: ");
  rv = p11->C_GenerateRandom(hSession, random, sizeof(random));
  switch (rv)
    {
    case CKR_OK:
      printf("OK\n");
      break;

    default:
      printf("failed: rv=%s\n", rv2string(rv));
      retVal = 1;
      break;
    }

  return retVal;
}

int
testRandom(CK_SLOT_ID slotID, CK_SESSION_HANDLE hSession)
{
  CK_RV rv;
  int retVal = 0;

  printf("\n***********************************************\n");
  printf("* Testing random number generation algorithms *\n");
  printf("***********************************************\n");

  if (testRandom_Seed(slotID, hSession))
    retVal = 1;
  if (testRandom_Generate(slotID, hSession))
    retVal = 1;

  return retVal;
}
