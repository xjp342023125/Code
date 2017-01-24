mkdir out
cd out
rm -rf bin
rm -rf include
rm -rf lib
mkdir bin
mkdir include
mkdir lib

cd ..
copy .\openssl\libcrypto-1_1.dll .\out\bin
copy .\openssl\libssl-1_1.dll .\out\bin

xcopy .\curl\builds\libcurl-vc14-x86-debug-dll-ssl-dll\*.* .\out\ /E

copy .\libevent\build\bin\Debug\event.dll .\out\bin
copy .\libevent\build\lib\Debug\event.lib .\out\lib

pause