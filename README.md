# Atmos

A Physically Based Renderer.(Not Finished Yet)


## 构建说明

所有第三方库都为Visual Studio 2013/Visual Studio 2015下编译所得，暂不支持[CMake](https://cmake.org/)形式构建。其他版本VS需自行编译三方库

## 第三方库

1.[Assimp](https://github.com/assimp/assimp)用于Obj模型导入

2.[LodePNG](http://lodev.org/lodepng/)用于PNG图片解码编码

3.[t3DataStructures](https://github.com/BentleyBlanks/t3DataStructures)
Tatty3d的数据结构模块，用于计时器等基础库使用

4.[t3Math](https://github.com/BentleyBlanks/t3Math)完成基本数学库支持

5.[Tinyexr](https://github.com/syoyo/tinyexr)用于EXR图片解码编码

## 效果图

![](https://farm8.staticflickr.com/7249/26683877470_3b0a728e81_o.png)

SPP = 256, width: 700 height: 700 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/26683877470/in/dateposted-public/)

![](https://farm8.staticflickr.com/7672/26337553403_756357c59c_o.png)

SPP = 256 width: 700 height 700 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/26337553403/in/dateposted-public/)

![](https://farm8.staticflickr.com/7531/26495924673_9cf7cebd2f_b.jpg)

SPP = 256 width: 700 height 700 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/26336584694/in/dateposted-public/)

![](https://farm8.staticflickr.com/7168/26337551743_534b420f62_o.png)

SPP = 256 width: 700 height 700 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/26337551743/in/dateposted-public/)

<a data-flickr-embed="true"  href="https://www.flickr.com/photos/134486032@N03/26942251805/in/dateposted-public/" title="spotlight path"><img src="https://farm8.staticflickr.com/7375/26942251805_de9dfcb41d_b.jpg" width="700" height="700" alt="spotlight path"></a><script async src="//embedr.flickr.com/assets/client-code.js" charset="utf-8"></script>

SPP = 256 width: 900 height 900 原图可见[Flickr](https://www.flickr.com/photos/134486032@N03/26942251805/in/dateposted-public/)

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

2.```Renderer```目前提供了多种调试模式渲染，同时支持网格渲染，正常的采样渲染，以及Debug专用的单光线跟踪，法线/菲涅尔贴图渲染等

3.目前Atmos支持Triangle(Mesh暂不支持), Sphere, Disk, Plane等基本几何形状

4.Atmos支持```景深```，```透视```等基本相机效果

5.Atmos目前支持无限远区域光光照，区域光光照，聚光灯和点光源(其他基本光源暂未实现)

6.Atmos中BVH实现为简易```EqualCounts```(暂不支持SAH分割)

7.Atmos目前实现的几何体都支持基本```纹理映射```

##待办事项

1.修正Conductor的错误菲涅尔反射问题

2.基于物理的BRDF模型，微表面模型的支持

3.更多光源模型的支持(Distant Light)

4.全局重要性采样概念理解

5.C++11多线程的支持

6.正交摄像机，环境摄像机支持

7.参数方程形式的形状支持


## 关于作者

``` cpp
int 官某某 = "Bingo";

char[] 个人博客 = "http://bentleyblanks.github.io";
```

