# Atmos

A Simple Physically Based Renderer.

![](https://farm2.staticflickr.com/1542/25389305206_85f8558be1_b.jpg)

## 构建说明

无



## 使用说明

1.a3Log支持多种日志等级发放消息，可自行选择输出到终端或日志文件中，如

```cpp
    a3Log::debug("这一段是信息Log, 紧接着一个int变量: a = %d\n", a);
    a3Log::debug("prefix", "suffix", "What are you talking about. %d\n", a);
```
但输出到文件部分目前不支持中文或者其他UTF8字符

## 关于作者

``` cpp
int 官某某 = "Bingo";

char[] 个人博客 = "http://bentleyblanks.github.io";
```

