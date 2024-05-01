<h1 align="center">MicroPython WTools CModule</h1>

<p align="center"><img src="https://img.shields.io/badge/Licence-MIT-green.svg?style=for-the-badge" /></p>

## 项目介绍

为`MicroPython`提供计算类相关功能，试图将大量需要计算的代码封装为 C 模块，以提高效率。

## 功能需求列表

目前的功能需求是对颜色亮度进行调节。

## 如何添加模块

> 以`ESP`系列芯片为例

* 在`MicroPython`项目同级目录克隆或粘贴本项目文件夹，并将文件夹重命名为`wtools`

	```bash
	git clone https://gitee.com/walkline/micropython-wtools-cmodule.git wtools
	```

* 根据`MicroPython`项目固件编译说明做好前期准备

* 使用如下命令编译固件：

	```bash
	cd micropython/ports/esp32
	make USER_C_MODULES=../../../../wtools/cmodules/micropython.cmake
	```

## 模块使用说明

参考项目目录下`tests`文件中的代码。

查看 [模块方法列表](./DOCS.md)

## 参考资料

* [Adafruit NeoPixel Library](https://github.com/adafruit/Adafruit_NeoPixel)
* [Arduino](https://www.arduino.cc/)

## 合作交流

* 联系邮箱：<walkline@163.com>
* QQ 交流群：
	* 走线物联：[163271910](https://jq.qq.com/?_wv=1027&k=xtPoHgwL)
	* 扇贝物联：[31324057](https://jq.qq.com/?_wv=1027&k=yp4FrpWh)

<p align="center"><img src="https://gitee.com/walkline/WeatherStation/raw/docs/images/qrcode_walkline.png" width="300px" alt="走线物联"><img src="https://gitee.com/walkline/WeatherStation/raw/docs/images/qrcode_bigiot.png" width="300px" alt="扇贝物联"></p>
