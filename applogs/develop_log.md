# 开发日志

***

*2019-4-29*：程序数据正常，但扬声器时钟没有声音。经过排查，发现写入扬声器的数据一直是0（更准确的说是buffer的初始值），进一步发现buffer的值，其实一直没有更改，这是因为memcpy函数的len参数一直是0。最终到给len赋值的地方，发现那个函数的len参数是按值传递的，应该改为按引用传递。

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