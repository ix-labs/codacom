#!/bin/sh

set -e -x

URL_LOUVAIN="https://sites.google.com/site/findcommunities/newversion/community.tgz"
DIR_LOUVAIN="louvain"

URL_CONCLUDE="http://www.emilio.ferrara.name/wp-content/uploads/2011/11/CONCLUDE.zip"
DIR_CONCLUDE="conclude"

URL_SCD="https://github.com/DAMA-UPC/SCD/archive/master.zip"
DIR_SCD="scd"

#Louvain install
wget $URL_LOUVAIN -O $DIR_LOUVAIN/community.tgz
cd $DIR_LOUVAIN
tar -zxvf community.tgz
mv Community_latest/* .
rm -r community.tgz Community_latest
cp execute.sh.inst execute.sh
rm Makefile #The Makefile.am will handle the build
#Solve a bug, for recent gcc include unistd
echo '#include <unistd.h>' | cat - main_community.cpp > temp && mv temp main_community.cpp
cd -;
#Update of the build system to take into account the package
# if [ "$(grep '^SUBDIRS += louvain$' Makefile.am)" = "" ]
# then
# 		echo SUBDIRS += louvain >> Makefile.am
# fi

#Conclude install
wget $URL_CONCLUDE -O $DIR_CONCLUDE/CONCLUDE.zip
cd $DIR_CONCLUDE
unzip -o CONCLUDE.zip
cp execute.sh.inst execute.sh
cd -

#SCD install
wget $URL_SCD -O $DIR_SCD/master.zip
cd $DIR_SCD
unzip -o master.zip
echo "Build of SCD. If this part fail, check out https://github.com/DAMA-UPC/SCD"
mv SCD-master/* .
rm -r SCD-master
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cd ..
cp execute.sh.inst execute.sh
cd ..