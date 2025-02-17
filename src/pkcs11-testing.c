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
 pkcs11-testing.c

 This program can be used for testing HSMs using PKCS#11.
*****************************************************************************/

#include <config.h>
#include "pkcs11-testing.h"
#include "session.h"
#include "library.h"
#include "mechanisms.h"
#include "random.h"
#include "showslots.h"
#include "publickey.h"
#include "import.h"
#include "error.h"
#include "stability.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

// Display the usage
void usage()
{
  printf("Program for testing HSMs using PKCS#11\n");
  printf("Usage: pkcs11-testing [ACTION] [OPTIONS]\n");
  printf("[ACTIONS]\n");
  printf("  -h                    Shows this help screen.\n");
  printf("  --help                Shows this help screen.\n");
  printf("  --show-mechanisms     Display the available mechanisms.\n");
  printf("  --show-slots          Display the available slots.\n");
  printf("  --test-all            Run all tests (except stability test)\n");
  printf("  --test-dnssec         Test if the DNSSEC algorithms are available.\n");
  printf("  --test-rsaimport      Test if RSA keys can be imported.\n");
  printf("  --test-rsapub         Test if the public information is available in the private key.\n");
  printf("  --test-stability      Test if the HSM is stable. Creating keys and signing.\n");
  printf("  --test-suiteb         Test if the NSA Suite B algorithms are available.\n");
  printf("  --test-random         Test random number generation.\n");
  printf("  -v                    Show version info.\n");
  printf("  --version             Show version info.\n");
  printf("[OPTIONS]\n");
  printf("  --batchjobs <number>  The number of batchjobs in the stability test.\n");
  printf("  --module <path>       The path to the PKCS#11 library.\n");
  printf("  --pin <PIN>           The PIN for the normal user.\n");
  printf("  --rollovers <number>  The number of rollovers in the stability test.\n");
  printf("  --signatures <number> The number of signatures in the stability test.\n");
  printf("  --sleep <seconds>     The time between each batchjob.\n");
  printf("  --slot <number>       The slot where the token is located.\n");
}

// Enumeration of the long options
enum
  {
    OPT_BATCHJOBS = 0x100,
    OPT_HELP,
    OPT_MODULE,
    OPT_PIN,
    OPT_ROLLOVERS,
    OPT_SHOW_MECHANISMS,
    OPT_SHOW_SLOTS,
    OPT_SIGNATURES,
    OPT_SLEEP,
    OPT_SLOT,
    OPT_TEST_ALL,
    OPT_TEST_DNSSEC,
    OPT_TEST_RSAIMPORT,
    OPT_TEST_RSAPUB,
    OPT_TEST_STABILITY,
    OPT_TEST_SUITEB,
    OPT_TEST_RANDOM,
    OPT_VERSION
  };

// Text representation of the long options
static const struct option long_options[] =
  {
    { "batchjobs",       1, NULL, OPT_BATCHJOBS },
    { "help",            0, NULL, OPT_HELP },
    { "module",          1, NULL, OPT_MODULE },
    { "pin",             1, NULL, OPT_PIN },
    { "rollovers",       1, NULL, OPT_ROLLOVERS },
    { "show-mechanisms", 0, NULL, OPT_SHOW_MECHANISMS },
    { "show-slots",      0, NULL, OPT_SHOW_SLOTS },
    { "signatures",      1, NULL, OPT_SIGNATURES },
    { "sleep",           1, NULL, OPT_SLEEP },
    { "slot",            1, NULL, OPT_SLOT },
    { "test-all",        0, NULL, OPT_TEST_ALL },
    { "test-dnssec",     0, NULL, OPT_TEST_DNSSEC },
    { "test-rsaimport",  0, NULL, OPT_TEST_RSAIMPORT },
    { "test-rsapub",     0, NULL, OPT_TEST_RSAPUB },
    { "test-stability",  0, NULL, OPT_TEST_STABILITY },
    { "test-suiteb",     0, NULL, OPT_TEST_SUITEB },
    { "test-random",     0, NULL, OPT_TEST_RANDOM },
    { "version",         0, NULL, OPT_VERSION },
    { NULL,              0, NULL, 0 }
  };

CK_FUNCTION_LIST_PTR p11;

// The main function
int
main(int argc, char *argv[])
{
  int option_index = 0, opt, retVal = 0, action = 0, needSession = 0;

  int rollovers = 2;
  int batchjobs = 2;
  int signatures = 100;
  int sleepTime = 15;
  char *userPIN = NULL;
  char *module = NULL;
  char *slot = NULL;
  CK_SLOT_ID slotID;
  CK_SESSION_HANDLE hSession;

  CK_C_GetFunctionList pGetFunctionList;
  CK_RV rv;

  moduleHandle = NULL;
  p11 = NULL;

  while ((opt = getopt_long(argc, argv, "hv", long_options, &option_index)) != -1)
    {
      switch (opt)
        {
        case OPT_SHOW_MECHANISMS:
        case OPT_SHOW_SLOTS:
          action = opt;
          break;
        case OPT_TEST_ALL:
        case OPT_TEST_DNSSEC:
        case OPT_TEST_RSAIMPORT:
        case OPT_TEST_RSAPUB:
        case OPT_TEST_STABILITY:
        case OPT_TEST_SUITEB:
        case OPT_TEST_RANDOM:
          needSession = 1;
          action = opt;
          break;
        case OPT_BATCHJOBS:
          batchjobs = atoi(optarg);
          break;
        case OPT_ROLLOVERS:
          rollovers = atoi(optarg);
          break;
        case OPT_SIGNATURES:
          signatures = atoi(optarg);
          break;
        case OPT_SLEEP:
          sleepTime = atoi(optarg);
          break;
        case OPT_SLOT:
          slot = optarg;
          break;
        case OPT_MODULE:
          module = optarg;
          break;
        case OPT_PIN:
          userPIN = optarg;
          break;
        case OPT_VERSION:
        case 'v':
          printf("%s\n", PACKAGE_VERSION);
          exit(0);
          break;
        case OPT_HELP:
        case 'h':
        default:
          usage();
          exit(0);
          break;
        }
    }

  // No action given, display the usage.
  if (!action)
    {
      usage();
      exit(0);
    }

  if (module == NULL)
    {
      fprintf(stderr, "Please provide the path to the PKCS#11 library by using --module\n");
      exit(1);
    }

  // Get a pointer to the function list for PKCS#11 library
  pGetFunctionList = loadLibrary(module, &moduleHandle);
  if (pGetFunctionList == NULL)
    {
      fprintf(stderr, "ERROR: Could not load the library.\n");
      exit(1);
    }

  // Load the function list
  (*pGetFunctionList)(&p11);

  // Initialize the library
  rv = p11->C_Initialize(NULL_PTR);
  if (rv != CKR_OK)
    {
      fprintf(stderr, "ERROR: Could not initialize the library. rv=%s\n", rv2string(rv));
      exit(1);
    }

  if (needSession)
    {
      retVal = openLogin(slot, userPIN, &slotID, &hSession);
      if (retVal)
        {
          // Finalize the library
          p11->C_Finalize(NULL_PTR);
          unloadLibrary(moduleHandle);

          return retVal;
        }
    }

  switch (action)
    {
    case OPT_SHOW_MECHANISMS:
      retVal = showMechs(slot);
      break;
    case OPT_SHOW_SLOTS:
      retVal = showSlots();
      break;
    case OPT_TEST_ALL:
      if (testDNSSEC(slotID, hSession)) retVal = 1;
      if (testSuiteB(slotID, hSession)) retVal = 1;
      if (testRandom(slotID, hSession)) retVal = 1;
      if (testRSAPub(hSession)) retVal = 1;
      if (testRSAImport(hSession)) retVal = 1;
      break;
    case OPT_TEST_DNSSEC:
      retVal = testDNSSEC(slotID, hSession);
      break;
    case OPT_TEST_RSAIMPORT:
      retVal = testRSAImport(hSession);
      break;
    case OPT_TEST_RSAPUB:
      retVal = testRSAPub(hSession);
      break;
    case OPT_TEST_STABILITY:
      retVal = testStability(slotID, hSession, rollovers, batchjobs, signatures, sleepTime);
      break;
    case OPT_TEST_SUITEB:
      retVal = testSuiteB(slotID, hSession);
      break;
    case OPT_TEST_RANDOM:
      retVal = testRandom(slotID, hSession);
      break;
    default:
      break;
    }

  // Finalize the library
  p11->C_Finalize(NULL_PTR);
  unloadLibrary(moduleHandle);

  return retVal;
}
