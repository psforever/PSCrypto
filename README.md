# PSCrypto
A wrapper around CryptoPP for use with Scala or Java.

```shell
$ git clone --recursive https://github.com/psforever/psf-cryptopp.git
```

Or if you already cloned without getting the submodules

```shell
git submodule update --init --recursive
```

## Building
GNU Make and a c++ compiler required.

```shell
$ make -j4
```

## Cross Compiling
Use the environment variable `PREFIX` to define a compiler tuple for building. For example

```shell
$ PREFIX=x86_64-w64-mingw32- make -j4
```

This will build a `pscrypto.dll` for 64-bit Windows.
