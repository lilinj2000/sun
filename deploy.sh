#! /bin/sh

home_app=~/app

home_sun=${home_app}/sun

./configure --prefix=${home_sun}

if test -d ${home_sun}; then
    rm -rf ${home_sun}
fi

make install

make distclean
