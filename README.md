# Atmos

A Physically Based Renderer.(Not Finished Yet)

<a data-flickr-embed="true"  href="https://www.flickr.com/photos/134486032@N03/29702466352/in/dateposted-public/" title="Test_000027"><img src="https://c1.staticflickr.com/9/8459/29702466352_4639ce4d29_c.jpg" width="800" height="450" alt="Test_000027"></a><!-- <script async src="//embedr.flickr.com/assets/client-code.js" charset="utf-8"></script> -->


Video's([Youtube](https://www.youtube.com/watch?v=d6pLWDCjvSk)  ||  [Youku](http://v.youku.com/v_show/id_XMTczMjI3NjY1Mg==.html#paction)) rendering was done by the [Atmos Renderer](https://github.com/BentleyBlanks/Atmos), water and other physics simulation were exported from [Blender](https://www.blender.org/). The [AtmosMovie](https://github.com/BentleyBlanks/AtmosMovie) was responsible for the auto-rendering. 120 frames in all, cost 25.3h.

> In 2016, this demo and the Atmos Renderer, were as my graduation project from JiangNan University.

## Build

All thirdyparty libs are compiled by Visual Studio 2015. 

> If you're using other versions of VS, please compiled them manualy. 

## Thirdyparty

1.[tinyobjloader](https://github.com/syoyo/tinyobjloader) Iimporting obj models.

2.[loadpng](http://lodev.org/lodepng/) encode / decode png image.

3.[t3DataStructures](https://github.com/BentleyBlanks/t3DataStructures)
 from Tatty3d, provide some fundamental modules.

4.[t3Math](https://github.com/BentleyBlanks/t3Math) from Tatty3d, for mathematical calculation.

5.[tinyexr](https://github.com/syoyo/tinyexr) encode / decode exr image.

## ScreenShots

![](https://farm8.staticflickr.com/7249/26683877470_3b0a728e81_o.png)

SPP = 256, width: 700 height: 700  [Flickr](https://www.flickr.com/photos/134486032@N03/26683877470/in/dateposted-public/)

![](https://farm8.staticflickr.com/7672/26337553403_756357c59c_o.png)

SPP = 256 width: 700 height 700  [Flickr](https://www.flickr.com/photos/134486032@N03/26337553403/in/dateposted-public/)

![](https://farm8.staticflickr.com/7531/26495924673_9cf7cebd2f_b.jpg)

SPP = 256 width: 700 height 700  [Flickr](https://www.flickr.com/photos/134486032@N03/26336584694/in/dateposted-public/)

![](https://farm8.staticflickr.com/7168/26337551743_534b420f62_o.png)

SPP = 256 width: 700 height 700  [Flickr](https://www.flickr.com/photos/134486032@N03/26337551743/in/dateposted-public/)

<a data-flickr-embed="true"  href="https://www.flickr.com/photos/134486032@N03/26942251805/in/dateposted-public/" title="spotlight path"><img src="https://farm8.staticflickr.com/7375/26942251805_de9dfcb41d_b.jpg" width="700" height="700" alt="spotlight path"></a><!-- <script async src="//embedr.flickr.com/assets/client-code.js" charset="utf-8"></script> -->

SPP = 256 width: 900 height 900  [Flickr](https://www.flickr.com/photos/134486032@N03/26942251805/in/dateposted-public/)

## Features

2. Visualizer Renderer for debug and visualization.

3. Triangle, Sphere, Disk, Plane shapes supported(with texture mapping).

4. Perspective(with DOF) Camera.

5. Environment, Point, Spot, Area Light supported.

6. BVH for faster intersection.

9. Easy to use log system, levels of urgency supported

> UTF-8 characters are not supported

```cpp
    a3Log::debug("Whos Your Daddy? %d\n", a);

    a3LogFile::debug("Whos Your Daddy? %d\n", a);

    a3LogHTML::begin();
    a3LogHTML::debug("Whos Your Daddy? %d\n", a);
    a3LogHTML::end();
```

## Author

``` cpp
const char* 官某某 = "Bingo";

std::string 个人博客 = "http://bentleyblanks.github.io";
```

