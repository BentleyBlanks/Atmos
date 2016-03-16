# Atmos

A Simple Physically Based Renderer.(Not Finished Yet)

![](https://farm2.staticflickr.com/1463/25207503294_b30be85aa8_z.jpg)

SPP = 2048 width: 3200 height: 3200

> 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/25151015169/in/dateposted-public/)

----------

## 构建说明

无

----------

## 效果图

![](https://farm2.staticflickr.com/1486/25742938521_7161856337_z.jpg)

SPP = 2048 Depth of Field: f = 180.0f，r = 20.0f width: 3200 height: 3200

> 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/25742938521/in/dateposted-public/)

![](https://farm2.staticflickr.com/1648/25539708580_9dd04535fd_z.jpg)

SPP = 256 width: 1000 height: 1000

> 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/25539708580/in/dateposted-public/)

![](https://farm2.staticflickr.com/1680/25539770910_482c4da26f_z.jpg)

Scene's Normal Map

----------

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

3.目前Atoms支持Triangle Mesh, Sphere, Disk, Plane等基本几何形状

----------

##待办事项

1.Triangle Mesh的渲染(仍有BUG)

2.基于物理的BRDF的支持

3.基于物理的微表面模型的支持

4.多种光源模型的支持

5.C++11多线程的支持

6.BVH支持 可用Intel提供的BVH外部库 详情可见Tungsun

7.正交摄像机，环境摄像机支持

8.更多导出图像格式支持(目前仅支持```PPM```, ```PNG```)

9.渲染队列支持

10.重要性采样，分层采样等采样技术支持

11.不同随机分布采样的支持

12.参数方程形式的形状支持

----------

## 关于作者

``` cpp
int 官某某 = "Bingo";

char[] 个人博客 = "http://bentleyblanks.github.io";
```

