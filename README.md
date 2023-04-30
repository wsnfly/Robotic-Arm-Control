# Robotic-Arm-Control

Robotic arm motion control algorithm.

Today is March 27, 2023. I want to make a set of control algorithms for the robotic arm, and let it run on a single-chip microcomputer with relatively low computing power. Here, it is mainly to study the algorithm. There is no requirement for what type of microcontroller to use. I will use the STM32 series for testing.

Task goal, read G code, or input a position coordinates through external interface (currently, it is mainly designed with coordinates input by external system in real time). A complete set of control system that controls the robotic arm to reach the designated target along the predetermined route and speed. The control system is divided into 3 chapters, and each chapter corresponds to a different controller, (I don't want to concentrate all operations on a single-chip microcomputer, but I hope to divide them into different functions and complete their own calculations)

Chapter 1: Coordinate controller, which reads the absolute coordinate data from the outside and sends out the calculated data

1.0.1 Read the external coordinates, run speed information, and calculate the position where the robot arm should be in each time period. Usually 3-axis (xyz) or 6-axis (xyzabc). This is the calculation of the absolute position in space, where the input position parameter is necessary, and the velocity parameter is secondary. The path from the current position to the target position is generally a straight line between two points. Of course, it can also be an arc or other splines. In the first stage of the program, I will still use linear interpolation. First of all, different types of robotic arms have their own characteristics, at least the maximum operating speed and acceleration. The single-chip microcomputer needs to know its current position, running direction and running speed in real time. After reading the new coordinate position, the next running direction and speed of the robotic arm can be obtained through calculation. If the robot arm is at rest when the coordinate position is received, it will draw a straight line from the current position to the target position, and calculate the position where the robot arm should be in each time period, and send this position coordinate to the next The position coordinates of the processing module need to ensure interpolation accuracy and control of acceleration and deceleration.

The above is the working content of this module.

Chapter 2: In this chapter, the inverse solution controller will use different inverse solution algorithms for different types of manipulators. The inverse solution controller receives the data from the coordinate controller, and after inverse solution calculation, outputs the corresponding angle of each motor, and transmits it to each motor controller through the interface.

Chapter 3: Motor pulse controller here mainly receives data from the inverse controller, converts the data into the actual position and speed of the motor to control the rotation of the motor, but this is not the part that actually controls the motor MOS tube, the motor pulse controller Just output pulse and direction signal

Chapter 4: Motor Driver The driver receives the signal from the pulse controller, converts the signal and controls the motor directly. (This part does not need to be manufactured, and the corresponding driver should be matched when purchasing a servo motor or a stepper motor)

机械臂运动控制算法。

今天是2023年3月27日
我想要制作一整套机械臂的控制算法，并且让他能够运行在算力比较低的单片机中。
在这，主要是研究算法，对于使用什么型号的单片机并没有要求，我会使用STM32系列进行测试。

任务目标，读取G代码，或者外部接口输入一个位置坐标（目前主要以外部系统实时输入的坐标来设计）。控制机械臂沿预定路线和速度到达指定目标的一整套控系统。
控制系统分为3个章节，每个章节对应的不同的控制器，（我并不希望将所有的运算都集中在一个单片机上,而是希望将他们分成不同的职能，各自完成自己的计算）


第一章：
坐标控制器，读取来自外部的绝对坐标数据，并将计算完成的数据发送出去


1.0.1
读取外部坐标，运行速度信息，并通过计算得到每个时间段机械臂应该所处的位置。一般是3轴（xyz）或者6轴（xyzabc）。
这是对空间绝对位置的计算，其中输入位置参数是必须的，速度参数是次要的。从当前位置到达目标位置的路径一般是两点之间的直线。当然也可以是圆弧或者其他样条。
在第一阶段的程序中，我依然会使用直线插补。
首先不同类型的机械臂拥有属于自己的一些特征，至少拥有最大运行速度，加速度。
单片机需要实时知道自己当前所处的位置，运行方向和运行速度。
在读取到新的坐标位置之后，通过计算，得到机械臂接下来的运行方向和速度。
如果接收到坐标位置时机械臂处于静止状态，那么它会在当前位置到目标位置之间画出一条直线，并计算每个时间段机械臂应该所处的位置，将这个位置坐标发送到下一个处理模块
这个位置坐标需要保证插补精度和加减速的控制。

以上是这个模块的工作内容。

第二章：
这一章逆解控制器，对于不同类型的机械臂会使用不同的逆解算法。
逆解控制器接收来自坐标控制器的数据，经过逆解计算之后，输出各个电机应该对应的角度，并通过接口传送到各个电机控制器。

第三章：
电机脉冲控制器
这里主要接收来自逆解控制器的数据，将数据转换成电机实际位置和速度之后去控制电机的转动
但是这里还不是真正控制电机MOS管的部分，电机脉冲控制器只是输出脉冲和方向信号



第四章：
电机驱动器
驱动器接收来自脉冲控制器的信号，将信号转换之后直接控制电机。（该部分不需要制造，购买伺服电机或者步进电机时应该配套相应的驱动器）


截至2023年4月1日，第三部分的电机脉冲控制器已经完成。
接下来的工作是位置控制器。

直线插补算法：
1：计算目标位置与当前位置，每个轴需要移动的距离，反方向用负数表示
2：计算得到6个轴sqrt（x*x+y*y+z*z+a*a+b*b+c*c) 得到一个向量t
3：t所对应的数值就类似单轴运动的距离，带入加减速和最大速度。
4：t在运行过程中，可以计算得到各个轴的位置。x（当时）= t（当时）*x/t（总数）
5：将每个轴的位置发送输出


这个程序中初步先使用梯形加减速算法，后期可以带入正选波速度算法，或者s曲线速度算法。都是可以的
注意，这部分模块的输入时来自外部的坐标点，如果在运行过程中被输入了新的位置，会立即停止（直接停止，不带减速），然后立即加速想新的位置移动。这里立即停止并不会导致丢步，因为目标位置突然改变，每个关节轴上的模块并不会突然改变速度。
但是这种情况会导致路径过冲。一瞬间的位置可能不准确。
