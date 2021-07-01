modules="so1
so2_ref
so2
bin
"

for m in $modules
do
    cd $m
    sh build.sh || exit -1
    cd ..
done
