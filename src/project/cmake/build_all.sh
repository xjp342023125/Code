modules="lib
so
bin
"

for m in $modules
do
    cd $m
    sh build.sh || exit -1
    cd ..
done
