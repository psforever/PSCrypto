# PSCrypto
A PlanetSide specific wrapper around CryptoPP for use with Scala or Java. These functions are used for establishing an authenticated and confidential connection between a PlanetSide server and client.

To get the library, run

```shell
$ git clone --recursive https://github.com/psforever/PSCrypto.git
```

Or if you already cloned without getting the submodules

```shell
git submodule update --init --recursive
```

## Building
GNU Make and an `g++` (GNU C++) required. Builds tested in Debian, Cygwin with Mingw64, and GNUWin32.
In the top-level PSCrypto directory (not the subfolder), run

```shell
$ make -j4 # build for current system
```

This will build the CryptoPP dependency and then the `pscrypto` library, which links to CryptoPP. The pscrypto build artifact will be in pscrypto/ as `libpscrypto.so` or `pscrypto.dll` depending on your platform. Note that the library architecture must match that of the JVM or process you are loading it in to, otherwise you will see loader errors.

## Cross Compiling
Use the environment variable `PREFIX` to define a compiler tuple for building. For example

```shell
$ PREFIX=x86_64-w64-mingw32- make -j4
```

This will build a `pscrypto.dll` for 64-bit Windows. Platform and architecture quirks are handled by the file `Makefile.inc`.

You can also specify the `ARCH` variable manually (as `i686` or `x86_64`) in order to enable multilib building, if available.

## Releasing Code
You are able to create a release on a Linux 64-bit system with the MingW64 compiler toolchain installed. The prefixes required to build are

```
x86_64-w64-mingw32-
i686-w64-mingw32-
x86_64-linux-gnu-
```

Check out [the script to build a release](scripts/build.sh).

### Release Checklist

* Everything builds correctly
* Version has been bumped in [pscrypto/pscrypto.h](pscrypto/pscrypto.h) and in the Windows resource file [pscrypto/resource.rc](pscrypto/resource.rc)
* A release can be made with the build script
* Release notes have been updated
* All external references and this repository are tagged with attached release binaries
* Update links (travis, other readmes) to point to the latest release
