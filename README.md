# Atmos

A Physically Based Renderer.(Not Finished Yet)

<a data-flickr-embed="true"  href="https://www.flickr.com/photos/134486032@N03/29702466352/in/dateposted-public/" title="Test_000027"><img src="https://c1.staticflickr.com/9/8459/29702466352_4639ce4d29_c.jpg" width="800" height="450" alt="Test_000027"></a><script async src="//embedr.flickr.com/assets/client-code.js" charset="utf-8"></script>

Atmos with Blender

[Youtube](https://www.youtube.com/watch?v=d6pLWDCjvSk)  ||  [Youku](http://v.youku.com/v_show/id_XMTczMjI3NjY1Mg==.html#paction)

该Demo渲染部分由Atmos完成，水以及物理碰撞部分得Blender加持完成。渲染工作[AtmosMovie](https://github.com/BentleyBlanks/AtmosMovie)工具自动化渲染，120帧画面，总渲染时长25.3h。

> 该Demo以及渲染器本身作为个人江南大学2016年本科毕业设计展示中提交。

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

> 输出到文件部分目前```不支持```中文或者其他UTF8字符

```cpp
    a3Log::debug("这一段是信息Log, 紧接着一个int变量: a = %d\n", a);

    a3LogFile::debug("What are you talking about. %d\n", a);

    a3LogHTML::begin();
    a3LogHTML::debug("What are you talking about. %d\n", a);
    a3LogHTML::end();
```

2.```Renderer```目前提供了多种调试模式渲染，同时支持网格渲染，正常的采样渲染，以及Debug专用的单光线跟踪，法线/菲涅尔贴图渲染等

3.Atmos支持Triangle(Mesh暂不支持), Sphere, Disk, Plane等基本几何形状

4.Atmos支持```景深```，```透视```等基本相机效果

5.Atmos目前支持无限远区域光光照，区域光光照，聚光灯和点光源(其他基本光源暂未实现)

6.Atmos中BVH实现为简易```EqualCounts```(暂不支持SAH分割)

7.Atmos目前实现的几何体都支持基本```纹理映射```

##待办事项

1.重构Integrator中错误Path Tracing重要性采样实现问题

2.修复原错误菲涅尔反射问题

3.修复原错误棋盘贴图场景在Mitsuba同一画面配置下结果不同的错误

4.实现BVH中未实现的更高效写法

5.完成若干个基本采样的单元测试项目

6.重新实现非狄拉克分布的BSDF与光源

7.全局重要性采样支持

8.正交摄像机，环境摄像机支持

9.更多的基于物理的BRDF模型，以及Microfacet材质支持


## 关于作者

``` cpp
int 官某某 = "Bingo";

char[] 个人博客 = "http://bentleyblanks.github.io";
```

