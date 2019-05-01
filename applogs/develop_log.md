# 开发日志

***

- *2019-4-29*：程序数据正常，但扬声器时钟没有声音。经过排查，发现写入扬声器的数据一直是0（更准确的说是buffer的初始值），进一步发现buffer的值，其实一直没有更改，这是因为memcpy函数的len参数一直是0。最终到给len赋值的地方，发现那个函数的len参数是按值传递的，应该改为按引用传递。

```
void Demodulator::getDataByChar(char data[], /*quint32 len应该是*/quint32 &len)
{
	// 考虑这里是否需要一定的条件等待，避免重复读取数据
	m_signalLock.lockForRead();
	int i = 0;
	for (int i = 0; i < m_signalLen; i++)
	{
		data[i] = char(m_signal[i]);
	}
	len = i;
	m_signalLock.unlock();
}

```

而此时**另一个版本**的却能发出声音，这是因为，它用的是m_buffer的replace函数，即将m_buffer中的len=0个字节的数据替换为解调后的数据，这就相当于对数据进行了追加，所以即使len=0也在传递解调的数据到扬声器中。之所以说是追加，是因为通过vs的内存检测可以看到这个程序**所耗内存是一直在增加**的。

- *2019-4-30:*将VS2017中的Qt项目直接移植到另一台电脑时出现以下问题：

| 问题                          | 原因                         | 解决方法                                                     |
| :---------------------------- | ---------------------------- | ------------------------------------------------------------ |
| 提示no Qt version assigned... | 没有在vs中设置好Qt版本       | 在QtvsTools中的project settings里选择好Qt version为正确版本，例如msvc2017_x64 |
| 无法找到stdio.h头文件等       | vs的windows kit版本可能不对  | 在项目菜单中选择重定解决方案目标                             |
| 无法找到stdio.h头文件等       | VC++附加库目录不对           | 在调试>vc++>常规>附加库目录中添加（更改）到自己本地的Includes目录，也就是你**项目的额外头文件**位置。 |
| 无法打开QObject等源文件       | 未添加相应源文件所在的Qt模块 | 在QtvsTools>Project settings中选择需要的Qt Modules，如core GUI等 |
| 缺少lib，dll文件              | -                            | 在调试>链接器>常规下添加lib文件目录，在调试>常规>环境路径Path中添加dlls（动态链接库程序）所在的目录 |

- *2019-5-1*：当我自己构建一个Figure类并继承自QCustomPlot类（第三方类库）时，在vs编译里报错：

  ```
  "public: static struct QMetaObject const QCustomPlot::staticMetaObject" (?staticMetaObject@QCustomPlot@@2UQMetaObject@@B)
  ```

  注：QCustomPlot的头文件目录，Lib目录我都已设置，且可以正常使用QCustomPlot型成员变量，但如果我继承它就报上面的错。

  **网上类似的问题**：继承QwtPlot时报错，网友的解决办法是：

  > 在VS里，调试>项目属性>C/C++>预处理器>添加**QWT_DLL**定义

  说是因为*只有在预处理的时候定义了 QWT_DLL才能使用QWT的抛出类，否则就会出错！*

  QCustomPlot的问题就不知道怎么办了...







