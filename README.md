# SDRCloud
本程序为一个SDR（软件定义无线电程序），主要用途为接收射频广播信号、接收NOAA气象卫星云图信号。
——基于2016级国科大电子系统设计课程作业项目
***
## 1.项目文件说明:
- SDRCloud文件夹下为废弃内容
- SDRCloudv2为SDRCloud项目的第2个版本，目前正在紧急开发中
- dlls文件下为必要的动态链接库：libusb1.0.dll为USB驱动，rtlsdr.dll为RTL电视棒的驱动（x64版），qcustomplotd2.dll为一个Qt图形绘制库
- includes文件下为动态链接库引入头文件
- libs下为导入库

## 2.项目目前的结构
- mainwindow.h中的MainWindow类主要负责界面的显示、用户的交互逻辑、数据的可视化
- sdrdevice.h里的SDRDevice类为SDR硬件设备的抽象类
- rtldevice.h里的RTLDevice类为RTL电视棒类
- readrtlthread.h下的ReadRTLThread类为读取RTL数据的worker线程类

## 其他
欢迎大家fork本仓库！如果有什么意见或好的想法，欢迎Push给我，也欢迎提出issues。
