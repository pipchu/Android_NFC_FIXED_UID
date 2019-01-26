1. 简介

    通过注入到 __com.android.nfc__ 进程，调用libnfc-nci.so的函数来设置UID（NFCID1）。

2. 用到的库
    
    [TinyInjector](https://github.com/shunix/TinyInjector)，一个开源的Android注入工具。

    [xHook](https://github.com/iqiyi/xHook)，iqiyi开源的一个Android PLT Hook库。

    [android_system_nfc](https://github.com/LineageOS/android_system_nfc)，libnfc-nci.so的源代码，用到了里面的一些头文件。

3. 测试方法

* 将项目克隆到本地，在项目根目录使用ndk-build进行编译。
* 将编译好的injector与libuidctrl.so传输到手机/data/local/tmp/目录下，并授予injector可执行权限。
* 以root身份执行/data/local/tmp/injector com.android.nfc /data/local/tmp/libuidctrl.so。
    