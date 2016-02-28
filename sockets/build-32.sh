#########################
# Remove old binary
#########################
if [ -e "./x32/ml_sockets.so" ]; then
    rm ./x32/ml_sockets.so
fi

#########################
# Prepare build
#########################
autoreconf -ifv
./configure CXXFLAGS='-m32 -g -O2 -fPIC' CFLAGS='-m32' LDFLAGS='-m32'
make clean

#########################
# Do build
#########################
echo Building...
make >_make-32.log

#########################
# Check for error
#########################
rc=$?
if [ $rc -ne 0 ]; then
    echo "Stopping: make returned error code $rc"
    exit 1
fi

#########################
# Copy binary file
#########################
mkdir x32
cp src/.libs/libml_sockets.so ./x32/ml_sockets.so
echo "Build completed "`pwd`"/x32/ml_sockets.so"
exit 0
