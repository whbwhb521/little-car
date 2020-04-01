# little-car
directx learning
用directx做的一个小车移动的小项目

本任务主要参考资料有如下：
1.龙书
2. X_Jun96的博客，https://blog.csdn.net/X_Jun96/article/details/80293670。
3. 部分带带大师兄的博客， https://blog.csdn.net/qq_29523119。

1.操作说明：
1）运行exe文件。
2）数字键 1 切换到小车的第一人称视角。
3）数字键 2 切换到自由视角。
4）数字键 3 切换到第三人称相机。
5）在第一人称下， w s a d 控制小车移动。 鼠标无效。
6）在自由视角下， w s a d 控制相机平移， 鼠标控制相机方向。
7）在第三人称相机下， w s a d控制小车移动， 相机始终看向小车，可以使用鼠标进行相机的转动，鼠标滚轮改变小车和相机之间的距离。


2.说明：
1）绘制了一个小车，小车主题为立方体，利用texturecube进行立方体纹理贴图。 
2）轮子使用网上找的宝马图标进行贴图，可以进行前后转动。
3）小车上方为椅子和小车操纵杆以及电脑（操纵杆为小车操纵杆，电脑不是游戏机，在科技发达的现在，使用智能电脑可以更加精确的进行挖掘工作）。 电脑屏幕使用RenderToTexture来获取相机所捕获的纹理（方便起见，没有在新建一个投影相机进行纹理的捕获，而直接使用了主相机）。
4）地面是一个平面，使用了网上寻找的草地纹理，纹理的寻址模式设为wrap。
5）提示板使用公告板效果，我把它当作现实之外的物体，所以没有被RTT捕获。电脑屏幕中不会出现。
6）光效有点光源和平行光。
7）物体材质分为阴影材质和正常材质，没有根据每个物体进行细分。
8）绘制车子阴影，利用了简单的平面阴影。
经测试，暂无bug。
