# Atmos

A Simple Physically Based Renderer.

![](https://farm2.staticflickr.com/1700/24819906013_c3ffe86c68_z.jpg)

SPP = 1024

## 构建说明

无



## 使用说明

1.a3Log支持多种日志等级发放消息，可自行选择输出到终端或日志文件中，如

```cpp
    a3Log::debug("这一段是信息Log, 紧接着一个int变量: a = %d\n", a);

    a3LogFile::debug("What are you talking about. %d\n", a);

    a3LogHTML::begin();
    a3LogHTML::debug("What are you talking about. %d\n", a);
    a3LogHTML::end();
```
但输出到文件部分目前不支持中文或者其他UTF8字符

2.```a3SamplerRenderer.cpp```中定义了跟踪深度，潜在BRDF(未封装)以及SPP, 尚未开放可自行更改。

## 关于作者

``` cpp
int 官某某 = "Bingo";

char[] 个人博客 = "http://bentleyblanks.github.io";
```

