## Skynet

Skynet is a lightweight online game framework, and it can be used in many other fields.

## ǰ��
#### ���ֿ�skynet֧��windows�����У�ֻ֧��visual studio 2013����ȷ����ı������Ѿ����SP4����
#### ��Ϊ����Ҫ������ǿ��һ�飬������vs2013����SP4�����������������

```
�˰汾�޸��Թٷ���skynet���Ķ��������£�
1��sproto�޸ģ������real��˫���ȸ�����double����֧�֣��Լ�variant���ͣ�������real/int/string/bool����֧��
2��windows�²�֧��epoll���ʲ���event-select����ģ��ģ��epoll����֤��С�Ķ�skynetԴ�������£�ʵ������ͨѶ
3��windowsƽ̨��û��pipe���ݵĽӿڣ�������socket api��ģ����һ����
4������̨���룬hack�޸���read������ģ���ȡfd 0(stdin)
```

## ����
```
windows��
ʹ��visual studio 2013ֱ�Ӵ�build/vs2013/skynet.sln���ɣ�Ŀǰ��ʱֻ֧����һ���汾�ı�����

linux/macos��
�ٷ���һ��
```

## ����
```
windows��
1������Ŀ¼����Ϊskynet.exe����Ŀ¼��Ĭ��Ϊ $(ProjectDir)..\..\
2�������������Ϊconfig�ļ������·������ examples/config

linux/macos��
�͹ٷ���һ��
```

## Build

For windows, open build/vs2013/skynet.sln and build all
You can use vs ide to debugging skynet

```
## Difference between offical skynet
1.sproto support real(double)/variant(real/int/string) field type
2.used event-select to simulate epoll
3.use socket api to simulate pipe()
4.hack read fd(0) for console input
```

For linux, install autoconf first for jemalloc:

```
git clone https://github.com/cloudwu/skynet.git
cd skynet
make 'PLATFORM'  # PLATFORM can be linux, macosx, freebsd now
```

Or you can:

```
export PLAT=linux
make
```

For FreeBSD , use gmake instead of make.

## Test

Run these in different consoles:

```
./skynet examples/config	# Launch first skynet node  (Gate server) and a skynet-master (see config for standalone option)
./3rd/lua/lua examples/client.lua 	# Launch a client, and try to input hello.
```

## About Lua version

Skynet now uses a modified version of lua 5.3.3 ( https://github.com/ejoy/lua/tree/skynet ) for multiple lua states.

You can also use official Lua versions, just edit the Makefile by yourself.

## How To Use (Sorry, Only in Chinese now)

* Read Wiki for documents https://github.com/cloudwu/skynet/wiki
* The FAQ in wiki https://github.com/cloudwu/skynet/wiki/FAQ
