# Atmos

A Physically Based Renderer.(Not Finished Yet)

<a data-flickr-embed="true"  href="https://www.flickr.com/photos/134486032@N03/29702466352/in/dateposted-public/" title="Test_000027"><img src="https://c1.staticflickr.com/9/8459/29702466352_4639ce4d29_c.jpg" width="800" height="450" alt="Test_000027"></a>

Video's([Youtube](https://www.youtube.com/watch?v=d6pLWDCjvSk)  ||  [Youku](http://v.youku.com/v_show/id_XMTczMjI3NjY1Mg==.html#paction)) rendering was done by the [Atmos Renderer](https://github.com/BentleyBlanks/Atmos), water and other physics simulation were exported from [Blender](https://www.blender.org/). The [AtmosMovie](https://github.com/BentleyBlanks/AtmosMovie) was responsible for the auto-rendering. 120 frames in all, cost 25.3h.

> In 2016, this demo and the Atmos Renderer, were as my graduation project from JiangNan University.

Also developed an [Atmos Editor](https://github.com/BentleyBlanks/AtmosVisualizer/tree/master/AtmosVisualizer/AtmosEditor), supports multiple camera to set up the matrials, shapes, lights...And preview the final result in real-time camera.

<a data-flickr-embed="true"  href="https://www.flickr.com/photos/134486032@N03/26185532778/in/dateposted-public/" title="QQ图片20180203211712"><img src="https://farm5.staticflickr.com/4749/26185532778_100b0788c9_c.jpg" width="800" height="438" alt="QQ图片20180203211712"></a>

## Build

All thirdyparty libs are compiled by Visual Studio 2015. 

> If you're using other versions of VS, please compiled them manualy. 

## Thirdyparty

1.[tinyobjloader](https://github.com/syoyo/tinyobjloader) Importing obj models.

2.[loadpng](http://lodev.org/lodepng/) encode / decode png image.

3.[t3DataStructures](https://github.com/BentleyBlanks/t3DataStructures)
 from Tatty3d, provide some fundamental modules.

4.[t3Math](https://github.com/BentleyBlanks/t3Math) from Tatty3d, for mathematical calculation.

5.[tinyexr](https://github.com/syoyo/tinyexr) encode / decode exr image.

## ScreenShots

<a data-flickr-embed="true"  href="https://www.flickr.com/photos/134486032@N03/26683877470/in/dateposted-public/" title="path dieletric texture"><img src="https://farm8.staticflickr.com/7249/26683877470_b2f5d1ab20_b.jpg" width="700" height="700" alt="path dieletric texture"></a>

<a data-flickr-embed="true"  href="https://www.flickr.com/photos/134486032@N03/26495924673/in/dateposted-public/" title="hello"><img src="https://farm8.staticflickr.com/7531/26495924673_9cf7cebd2f_b.jpg" width="700" height="700" alt="hello"></a>

<a data-flickr-embed="true"  href="https://www.flickr.com/photos/134486032@N03/26942251805/in/dateposted-public/" title="spotlight path"><img src="https://farm8.staticflickr.com/7375/26942251805_de9dfcb41d_b.jpg" width="700" height="700" alt="spotlight path"></a>

<a data-flickr-embed="true"  href="https://www.flickr.com/photos/134486032@N03/26185801198/in/dateposted-public/" title="atmosPath"><img src="https://farm5.staticflickr.com/4768/26185801198_d38d7cc821_b.jpg" width="700" height="700" alt="atmosPath"></a>

## Features

2. Visualizer Renderer for debug and visualization.

3. Triangle, Sphere, Disk, Plane shapes supported(with texture mapping).

4. Perspective(with DOF) Camera.

5. Environment, Point, Spot, Area Light supported.

6. BVH for faster intersection.

9. Easy to use log system, levels of urgency supported

```cpp
    a3Log::debug("Whos Your Daddy? %d\n", a);

    a3LogFile::debug("Whos Your Daddy? %d\n", a);

    a3LogHTML::begin();
    a3LogHTML::debug("Whos Your Daddy? %d\n", a);
    a3LogHTML::end();
```
> UTF-8 characters are not supported

## Author

``` cpp
const char* 官某某 = "Bingo";

std::string 个人博客 = "http://bentleyblanks.github.io";
```

