# 实时儿童中英文混合
1. 修改main.cpp中的path的值指定为要识别的文件,进入目录build中执行命令`cmake .. -DCMAKE_BUILD_TYPE=Debug`生成Makefile文件
2. 执行命令make生产可执行文件main
3. 进入目录`/debug/bin/debug`中
4. 执行命令:`export LD_LIBRARY_PATH=../../../lib:$LD_LIBRARY_PATH``export LD_LIBRARY_PATH=../../../third_party:$LD_LIBRARY_PATH`添加环境变量
5. 执行命令:./main运行程序