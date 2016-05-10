# Atmos

A Physically Based Renderer.(Not Finished Yet)


## 构建说明

所有第三方库都为Visual Studio 2013/Visual Studio 2015下编译所得，暂不支持[CMake](https://cmake.org/)形式构建。其他版本VS需自行编译三方库

## 第三方库

1.[Assimp](https://github.com/assimp/assimp)用于Obj模型导入
2.[LodePNG](http://lodev.org/lodepng/)用于PNG图片解码编码
3.[t3DataStructures](https://github.com/BentleyBlanks/t3DataStructures)Tatty3d的数据结构模块，用于计时器等基础库使用
4.[t3Math](https://github.com/BentleyBlanks/t3Math)完成基本数学库支持

## 效果图

![](https://farm8.staticflickr.com/7672/26337553403_756357c59c_o.png)

SPP = 256 width: 700 height 700 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/26337553403/in/dateposted-public/)

![](https://farm8.staticflickr.com/7097/26336584694_5dc7a0f34d_o.png)

SPP = 256 width: 700 height 700 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/26336584694/in/dateposted-public/)

![](https://farm8.staticflickr.com/7168/26337551743_534b420f62_o.png)

SPP = 256 width: 700 height 700 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/26337551743/in/dateposted-public/)

<a data-flickr-embed="true"  href="https://www.flickr.com/photos/134486032@N03/26942251805/in/dateposted-public/" title="spotlight path"><img src="https://farm8.staticflickr.com/7375/26942251805_de9dfcb41d_b.jpg" width="700" height="700" alt="spotlight path"></a><script async src="//embedr.flickr.com/assets/client-code.js" charset="utf-8"></script>

![](https://farm2.staticflickr.com/1549/26033000861_130b76788b_z.jpg)

SPP = 256 width: 2048 height: 1536 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/26033000861/in/dateposted-public/)

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

5.Atoms目前支持显式的无限远区域光光照，聚光灯和点光源(其他基本光源暂未实现)

6.Atoms采用[Assimp](https://github.com/assimp/assimp), [loadpng](http://lodev.org/lodepng/)等第三方库完成模型导入以及贴图导入

7.渲染需要完成对```a3SamplerRenderer```中camera, integrator, sampler根据需求实际分配内存

##待办事项

1.基于物理的微表面模型的支持

2.更多光源模型的支持(Area Light)

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

