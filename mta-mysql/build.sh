#########################
# Remove old binary
#########################
if [ -e "mta_mysql.so" ]; then
    rm mta_mysql.so
fi

#########################
# Prepare build
#########################
autoreconf -ifv
./configure CXXFLAGS='-g -O2 -fPIC'
make clean

#########################
# Do build
#########################
echo Building...
make >_make.log

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
cp src/.libs/libmta_mysql.so ./mta_mysql.so
echo "Build completed "`pwd`"/mta_mysql.so"
exit 0
