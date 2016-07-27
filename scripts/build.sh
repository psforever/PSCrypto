#!/bin/bash
set -ue

CONFIGS=3
BUILD_MATRIX_PREFIX=("x86_64-w64-mingw32-" "i686-w64-mingw32-" "x86_64-linux-gnu-")
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

   echo "Now building $javaFolder..."
   make clean > /dev/null 2>&1
   make -j > "build-log-${javaFolder}.txt" 2>&1

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
