# PSCrypto
A PlanetSide specific wrapper around CryptoPP for use with Scala, Java, C++ or C. These functions are used for establishing an authenticated and confidential connection between a PlanetSide server and client.

To get the library, run

```shell
$ git clone --recursive https://github.com/psforever/PSCrypto.git
```

Or if you already cloned without getting the submodules

```shell
git submodule update --init --recursive
```

## Why is this library necessary?
PlanetSide uses an older version of a cryptographic library called CryptoPP (Crypto++). It uses this for encrypting, decrypting, and authenticating _all_ login and world packets. Some of the algorithms used by PlanetSide are depreciated and no longer used for modern crypto. This means that implementations of the algorithms are hard to find.

What we have done is figured out the last known version of CryptoPP that used some of these algorithms, forked it, made it compile, and wrapped it so it could be easily called from server code, which is in Scala. It's possible that these old algorithms could be recreated for new modern code, but this was the easiest option and it closely matched what PlanetSide was doing for crypto.

## Building
GNU Make and `g++` (GNU C++) required. Builds tested in Debian Jessie, Cygwin with Mingw64, and GNUWin32.
In the top-level PSCrypto directory (not the subfolder), run

```shell
$ make -j4 # will build for current system's GCC
```

This will build the CryptoPP dependency and then the `pscrypto` library, which links to CryptoPP. The pscrypto build artifact will be in pscrypto/ as `libpscrypto.so` or `pscrypto.dll` depending on your platform. Note that the library architecture must match that of the JVM or process you are loading it in to, otherwise you will see loader errors.

Keep in mind that if you are building with Cygwin's GCC (not a cross compiler like MinGW), then your binaries _will not work_ on systems without Cygwin installed!

## Cross Compiling
Use the environment variable `PREFIX` to define a compiler tuple for building. For example

```shell
$ PREFIX=x86_64-w64-mingw32- make -j4
```

This will build a `pscrypto.dll` for 64-bit Windows. Platform and architecture quirks are handled by the file `Makefile.inc`.

You can also specify the `ARCH` variable manually (as `i686` or `x86_64`) in order to enable multilib building, if available.

## Releasing Code
You are able to create a release on a Linux 64-bit system with the MingW64 compiler toolchain installed. The prefixes required to build in order to support Windows 32,64 and Linux 64-bit are

```
x86_64-w64-mingw32-
i686-w64-mingw32-
x86_64-linux-gnu-
```

Check out [the script to build a release](scripts/build.sh) for more information.

### Developer Release Checklist

* Everything builds correctly
* System testing has passed (i.e it works on Windows)
* Version has been bumped in [pscrypto/pscrypto.h](pscrypto/pscrypto.h) and in the Windows resource file [pscrypto/resource.rc](pscrypto/resource.rc)
* A release can be made with the build script
* Release notes have been updated
* All external references and this repository are tagged with attached release binaries
* Update links (travis, other readmes) to point to the latest release
