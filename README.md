# PKCS #11 Testing

This project is a fork of the [PKCS#11 Testing
Tools](https://github.com/opendnssec/pkcs11-testing). The main changes
are:
 - Updated build system. All GNU Autoconf and Automake related files
   were removed and replaced with a hardcoded [config.h](./config.h)
   and [GNUmakefile](./GNUmakefile) files.
 - Converted from C++ to C.
 - Updated coding style.
 - Added tests for:
   - Random number functions.

The original authors are listed in [AUTHORS](./AUTHORS) and original
[LICENSE](./LICENSE) applies.
