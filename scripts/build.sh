#!/bin/bash
set -ue

# The GCC binaries (gcc, g++, etc) can have their name prefixed
# by specifying a program-prefix option when configuring.
# https://gcc.gnu.org/install/configure.html
#   Search above page for "--program-prefix"
# The prefix allows multiple GCC to be installed and on system PATH.
# MINGW (Which we use for crosscompilation) uses this option.
#   x86_64-w64-mingw32- and i686-w64-mingw32-
# GCC as built by Linux distros usually uses this option.
#   however the prefixes vary

# Default prefix to "x86_64-linux-gnu-", but allow overriding.
#   (For example Fedora 25 is noted needing something like "x86_64-redhat-linux-")
# NOTE: ${x-y} is a bash-ism. differs from portable ${x:-y} if x undefined
PARAM_GCC_PROGRAM_PREFIX=${PARAM_GCC_PROGRAM_PREFIX-"x86_64-linux-gnu-"}
PARAM_GCC_PREFIX_RANLIB=${PARAM_GCC_PREFIX_RANLIB-"x86_64-linux-gnu-"}
PARAM_GCC_PREFIX_STRIP=${PARAM_GCC_PREFIX_STRIP-"x86_64-linux-gnu-"}

CONFIGS=3
BUILD_MATRIX_PREFIX=("x86_64-w64-mingw32-" "i686-w64-mingw32-" "${PARAM_GCC_PROGRAM_PREFIX}")
BUILD_MATRIX_PREFIX_RANLIB=("x86_64-w64-mingw32-" "i686-w64-mingw32-" "${PARAM_GCC_PREFIX_RANLIB}")
BUILD_MATRIX_PREFIX_STRIP=("x86_64-w64-mingw32-" "i686-w64-mingw32-" "${PARAM_GCC_PREFIX_STRIP}")
BUILD_MATRIX_ARTIFACT=("pscrypto.dll" "pscrypto.dll" "libpscrypto.so")
BUILD_MATRIX_JAVA_FOLDER=("win32-x86-64" "win32-x86" "linux-x86-64")

BUILD_DEST="pscrypto-lib"
read -p "What version: " VERSION

mkdir -p "$BUILD_DEST"

echo "Now building $CONFIGS pscrypto configs for v$VERSION"

for i in `seq 1 $CONFIGS`; do
   iter=$(( $i-1 ))
   javaFolder=${BUILD_MATRIX_JAVA_FOLDER[$iter]}

   export PREFIX=${BUILD_MATRIX_PREFIX[$iter]}
   export PREFIX_RANLIB=${BUILD_MATRIX_PREFIX_RANLIB[$iter]}
   export PREFIX_RANLIB_USE=t
   export PREFIX_STRIP=${BUILD_MATRIX_PREFIX_STRIP[$iter]}
   export PREFIX_STRIP_USE=t

   echo "Now building $javaFolder..."
   make clean > "build-log-clean-${javaFolder}.txt" 2>&1
   # -j with no number was LITERALLY causing a system crash (out of memory)
   make -j 2 > "build-log-${javaFolder}.txt" 2>&1

   mkdir -p "$BUILD_DEST/$javaFolder"
   cp "pscrypto/${BUILD_MATRIX_ARTIFACT[$iter]}" "$BUILD_DEST/$javaFolder/"
done

echo "Builds ${BUILD_MATRIX_JAVA_FOLDER[*]} complete"
echo "Build folder: $BUILD_DEST"

cat <<EOF > "$BUILD_DEST/README.txt"
This contains native PSCrypto libraries for use in the JVM.
See https://github.com/psforever/PSCrypto for more details.
EOF

cat <<EOF > "$BUILD_DEST/VERSION.txt"
$VERSION
EOF

echo "Build listing: "
ls -R $BUILD_DEST
