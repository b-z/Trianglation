# 开发技术文档

**软硕163 周伯威 2016213588**

[TOC]

## 1. 程序的框架结构

### 1.1 模块划分

#### 1.1.1 Delaunay三角化计算

在`class Delaunay`内实现了三角化相关的函数。其中重要的函数包括:

``` c++
void computeEdges();
// 转换多边形类的数据结构，生成边表
void computeTriangulation();
// 计算三角化
bool isPointOnTheLeftOfVector(int p, Edge e);
// 判断顶点p是否在边e的左侧
bool isOtherEdgesIntersectWithVectors(Edge e1, Edge e2);
// 判断边e1、e2是否与其他边相交
bool isOtherPointsInTriangle(int p1, int p2, int p3);
// 判断p1、p2、p3构成的三角形内是否有其他顶点
bool isIn(Point p, Point p1, Point p2, Point p3);
// 判断点p是否位于p1、p2、p3构成的三角形内
int getMinimumCirclePointIDFromCandidates(int p1, int p2);
// 求候选顶点与点p1、p2构成三角形外接圆半径最小的顶点
int isPolygonEdge(int p1, int p2);
// 判断点p1、p2连线是否是多边形的边
void removeEdge(int idx);
// 从边表中删除一条边
bool isIntersection(Edge e1, Edge e2);
// 判断两边是否相交
double getCircumscribedCircleRadius(Point a, Point b, Point c);
// 求三角形外接圆半径
void reduceTriangle(int p);
// 确定剖分一个三角形，并移除
```

#### 1.1.2 Point类相关计算函数

``` c++
Point operator +(Point a, Point b);		// 两个点坐标相加
Point operator -(Point a, Point b);		// 两个点坐标相减
double operator ^(Point a, Point b);	// 两个点距离
double operator >>(Point a, Point b);	// 一个点到另一个点的幅角差(有正负)
double operator !(Point a);				// 一个点的幅角
```

#### 1.1.3 三角化结果绘制

``` c++
void CPolygonView::drawTriangleMesh(CDC* pDC, double scale, Point translation, int right, int bottom, TriangleMesh *mesh);
// 绘制多个三角形
void CPolygonView::drawTriangle(CDC* pDC, double scale, Point translation, int right, int bottom, Point u, Point v, Point w, int fill, int r, int g, int b);
// 绘制一个三角形，三角形在其构造函数中会随机分配一个颜色值
```

### 1.2 程序设计的整体思路

论文中给出了较详细的算法实现过程，共有5个步骤，我的实现也是按照这五个步骤进行的。

但是实际运行后发现效率较低，于是修改了函数结构与执行顺序，详见**2.1 效率优化**。

## 2. 编程用到的一些技巧

### 2.1 效率优化

#### 2.1.1 减少vector相关函数的使用

调试发现程序最内层循环中有较多的时间花在vector的`[]`和`size()`操作上，我减少了这些操作的使用，从而在常数上减少了代码运行时间。

#### 2.1.2 递归改写为循环

论文中的伪代码使用了递归方法，但实际上这个递归是没有必要的。我将递归写法改写成了循环，减少了程序对栈空间的消耗。

#### 2.1.3 剪枝

==论文中，先筛选候选顶点，再从候选顶点中找到最优三角形的方法效率很低。==

下面回顾一下论文的方法:

* 每轮迭代中，取出符合以下两点条件的顶点，放入候选顶点:
  * 顶点位于第一条边的左侧($O(1)$)
  * **顶点与第一条边构成的三角形可以作为三角剖分的三角形**($O(N)$)==(※)==
* 从候选顶点中选择与第一条边构成三角形的外接圆半径最小的顶点($O(1)$)
* 将这个顶点、第一条边连接成三角形，作为一个剖分三角形($O(1)$)。进入下一轮迭代。

==※ 注==: 论文中此步骤有误，详见**原理文档**。

上述论文方法中，加粗的部分的时间复杂度为$O(N)$，其中$N$为边数。对于一般的多边形，候选顶点的第一个条件很容易满足，因而很容易进入第二个条件的判断，这使得总的复杂度高达$O(N^3)$。

我更改了程序执行的顺序，首先判断外接圆半径是否比当前的最小半径小，若不是，则直接判断下一个顶点。

从运行结果上看，当边数较多的时候，该方法大幅度降低了程序运行时间。详见**结果文档**。

#### 2.1.4 双缓存绘图

默认给出的绘图方式会导致绘图区闪烁，使用双缓存绘图可解决。

#### 2.1.5 浮点数误差的处理

在修正算法中，可能有浮点数误差的问题。例如，正多边形的各个顶点是共圆的，而误差的存在使得顶点在圆上、圆内、圆外的判定会出现问题。

实际判断点在圆内时，将圆半径减去$10^{-8}$以消除影响。

### 2.2 测试技巧

* 如果三角化函数执行时间小于一秒，则使执行次数翻倍，反复进行这个过程直到总时间超过一秒。使用该方法可得到单次执行函数的精确时间。
* 在函数的关键节点设置计数器，用于记录总执行次数。
* 多边形边数的选取是指数增加的，初始为3，每次在上次的基础上乘以1.1，直到边数超过2000。
* 将多边形边数、执行时间、总执行次数输出至文件。进行数据分析时，对这些数均取对数，因为边数是指数增长的，所以取对数之后取样点是均匀分布的，可进行更精准的直线拟合。

## 3. 如何编译、运行或配置程序

### 3.1 三角剖分程序

* 运行环境:
  * Visual Studio 2012 with MFC


* 编译选项:
  * `PolygonView.cpp`中，修改
    ```c++
    #define ZBW_IS_DEBUGGING 0
    ```

    当参数为0时表示使用`try-catch`对多边形的错误进行捕获，如果输入多边形有误则会提示用户;

    当参数为1时，如果输入多边形有误则程序会停止运行。
  * `PolygonView.cpp`中，修改
    ```c++
    #define DYNAMIC 1
    ```

    当参数为0时表示实时刷新多边形的三角剖分;

    当参数为1时，点击Trianglation才会重新进行三角剖分。
  * `PolygonView.cpp`中，修改
    ```c++
    #define DOUBLE_BUFFER 1
    ```

    当参数为0时表示使用双缓存绘图;

    当参数为1时不使用双缓存绘图，界面会闪烁。

* 运行方法

  * 切换至Release模式，直接编译、运行;
  * 添加好多边形后，点击"Trianglation"按钮即可进行三角剖分;
  * 可分别剖分多边形A、多边形B。


### 3.2 测试程序

* 运行环境:
  * Visual Studio 2015 (注: 因VS2015提供了更高级的测试工具而VS2012没有)


* 编译选项:

  * 程序使用了递归方法，所以当多边形边数过多时，会报stack overflow错误，需要调整栈空间;

  * 修改**项目属性-链接器-系统-堆栈保留大小(/STACK: reserve)**为100000000(100MB)以上即可。

  * `Delaunay.h`中，修改

    ```c++
    #define REWRITE
    ```

    该行被注释时，使用论文算法(较慢);

    取消注释则使用新算法(较快)。

* 运行方法:

  * `main()`函数位于`test.cpp`中;

  * 程序内置了两种测试多边形:

    * `POLYGON_REGULAR`表示使用正多边形
    * `POLYGON_BAR`表示使用一个长条形的多边形

  * `Test::test()`函数的使用方法:

    ```c++
    void test(int max_size, int type, std::string file);
    /*
    Parameters:
    * max_size - 多边形的最大边数，程序会测试从三条边逐渐增加至不超过max_size的边数(使用指数增长);
    * type - POLYGON_REGULAR 或者 POLYGON_BAR;
    * file - 测试结果文件输出的路径。
    */
    ```

  * 输出: 

    * 输出是一个文本文件，存为`*.tsv`格式可用Microsoft Excel打开;
    * 第一行是表头，其余每一行是一组数据，数据各项由制表符分隔;
    * 每组数据包括:
      1. 多边形边数
      2. 运行耗时(秒)
      3. $\log_2{边数}$
      4. $\log_2{{耗时}}$
      5. 函数执行计数
      6. $\log_2{{函数执行计数}}$