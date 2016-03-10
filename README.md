# Atmos

A Simple Physically Based Renderer.

![](https://farm2.staticflickr.com/1672/25560276222_eea6d5b8a5_z.jpg)

SPP = 256 width: 4000 height: 4000

> 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/25560276222/in/dateposted-public/)

![](https://farm2.staticflickr.com/1474/25151015169_7efcfeaa0e_z.jpg)

SPP = 1024

> 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/25151015169/in/dateposted-public/)

![](https://farm2.staticflickr.com/1531/25509064516_8314deedf1_z.jpg)

SPP = 128，Depth of Field: f = 180.0f，r = 20.0f

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

3.目前Atoms支持Triangle Mesh, Sphere, Disk, Plane等基本几何形状

##待办事项

1.Triangle Mesh的渲染与obj模型导入支持

2.无限远区域光照模型支持

3.外部模型库assimp的ThirdParty并入

4.基于物理的BRDF的支持

5.基于物理的微表面模型的支持

6.多种光源模型的支持

7.C++11多线程的支持

8.BVH支持 可用Intel提供的BVH外部库 详情可见Tungsun

9.正交摄像机，环境摄像机支持

10.更多导出图像格式支持

11.渲染队列支持

12.重要性采样，分层采样等采样技术支持

13.不同随机分布采样的支持

14.参数方程形式的形状支持

## 关于作者

``` cpp
int 官某某 = "Bingo";

char[] 个人博客 = "http://bentleyblanks.github.io";
```

