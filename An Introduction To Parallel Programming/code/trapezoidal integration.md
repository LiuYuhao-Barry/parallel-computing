# 并行梯形积分算法

对于一个连续可微函数$f(x)$，求其$(a,b)$的定积分。梯形积分算法把$(a,b)$分为$n$等份，近似视为$n$个小梯形，分别计算每个的面积并求和。当$n$足够大时，求和的结果收敛于实际的定积分结果。



## 并行化

计算梯形积分的串行程序如下：

```c++
// Calculate a single trapezoid's area
double calTrapezoidArea(double leftBound, double rightBound, int n) {
    double area = (function(leftBound) + function(rightBound)) / 2.0;
    double x = leftBound;
    double h = (rightBound - leftBound) / n;

    for (int i = 1; i < n; i++) {
        x += h;
        area += function(x);
    }

    area = area * h;
    return area;
}
```

可以发现，计算$n$个小梯形面积的部分相互独立。如果用$p$个进程一起计算，可以把$(a,b)$分为$p$等份，每个进程负责计算其中一份，最后把计算结果累加即可。



## 实验结果

当进程数为1时，程序为串行版本；当进程数大于等于2时，有1个进程作为master负责接收来自其他进程的计算结果，其余进程负责计算。也就是说，当进程数为2时，实际上只有一个进程负责计算；当进程数为12时，有11个进程共同完成计算任务。

| 进程数 | 并行部分运行时间（秒） |
| :----: | :--------------------: |
|   1    |        11.1468         |
|   2    |        11.1506         |
|   3    |         5.9946         |
|   4    |         3.9898         |
|   5    |         3.2637         |
|   6    |         2.7497         |
|   7    |         2.3101         |
|   8    |         1.9115         |
|   9    |         1.7187         |
|   10   |         1.5393         |
|   11   |         1.4935         |
|   12   |         1.3728         |

