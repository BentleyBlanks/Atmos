# Atmos

A Simple Physically Based Renderer.(Not Finished Yet)

![](https://farm2.staticflickr.com/1549/26033000861_2fa780aaf5_o.png)

SPP = 256 width: 2048 height: 1536 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/26033000861/in/dateposted-public/)

## 构建说明

无


## 效果图

![](https://farm2.staticflickr.com/1532/25519761274_13b60a86a5_c.jpg)

SPP = 2000 width: 1024 height: 768 time: 3679.64s原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/25519761274/in/dateposted-public/)

![](https://farm2.staticflickr.com/1540/25493615434_59b96306f5_z.jpg)

SPP = 128 width:1000 height:500

![](https://farm2.staticflickr.com/1486/25742938521_7161856337_z.jpg)

SPP = 2048 Depth of Field: f = 180.0f，r = 20.0f width: 3200 height: 3200 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/25742938521/in/dateposted-public/)


## 使用说明

1.a3Log支持多种日志等级发放消息，可自行选择输出到终端或日志文件中，如

```cpp
    a3Log::debug("这一段是信息Log, 紧接着一个int变量: a = %d\n", a);

    a3LogFile::debug("What are you talking about. %d\n", a);

    a3LogHTML::begin();
    a3LogHTML::debug("What are you talking about. %d\n", a);
    a3LogHTML::end();
```
输出到文件部分目前```不支持```中文或者其他UTF8字符

2.```a3SamplerRenderer.cpp```中定义了```A3_RENDERING_NORMALMAP```和```A3_RENDERING_SINGLERAY```模式用于调试(全局渲染为法线, 单光线跟踪)，以及正常渲染模式```A3_RENDERING_REALISTICIMAGE```

3.目前Atoms支持Triangle(Mesh暂不支持), Sphere, Disk, Plane等基本几何形状

4.Atoms支持景深，透视等基本相机效果

5.Atoms目前仅支持显式的无限远区域光光照和点光源(其他基本光源暂未实现)

6.Atoms采用[Assimp](https://github.com/assimp/assimp), [loadpng](http://lodev.org/lodepng/)等第三方库完成模型导入以及贴图导入

7.渲染需要完成对```a3SamplerRenderer```中camera, integrator, sampler根据需求实际分配内存

##待办事项

1.基于物理的微表面模型的支持

2.更多光源模型的支持

3.全局重要性采样概念理解

4.C++11多线程的支持

5.BVH支持 可用Intel提供的BVH外部库 详情可见Tungsun

6.正交摄像机，环境摄像机支持

7.更多导出图像格式支持(目前仅支持```PPM```, ```PNG```)

8.参数方程形式的形状支持


## 关于作者

``` cpp
int 官某某 = "Bingo";

char[] 个人博客 = "http://bentleyblanks.github.io";
```

