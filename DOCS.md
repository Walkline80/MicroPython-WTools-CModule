<h1 align="center">MicroPython WTools CModule</h1>
<p align="center"><img src="https://img.shields.io/badge/Licence-MIT-green.svg?style=for-the-badge" /></p>


## ArrayTool 类

示例用法：

```python
from wtools import ArrayTool

array_a = ArrayTool(3, 5)
array_b = ArrayTool(3, 5)

array_a.init(0x47e1)
array_b.init(0x5ebd)

array_a.shift(ArrayTool.SHIFT_LEFT)

array_a.replace_col(0, array_b.get_col(0))
array_a.replace_row(1, array_b.get_row(1))

array_a.to_list()
array_a.print()
```

### 构造器

* **class wtools.ArrayTool(columns, rows)**

	构建`ArrayTool`数组对象，它是一个包含`columns`列和`rows`行的二维数组。

### 方法列表

* **ArrayTool.init(int)**

	输入一个整形数，根据它的二进制数值对二维数组进行填充。

	填充顺序为：从上到下，从左到右，即：

	```docs
	1 4 7
	2 5 8
	3 6 9
	```

* **ArrayTool.print()**

	打印二维数组用于预览数据。

* **ArrayTool.shift(direction)**

	根据`direction`指定的方向，将数组向上、下、左、右移动一位。

	移出的数据会被丢弃，新增的空位会被填充`0`。

* **ArrayTool.replace_col(index, data)**

	替换二维数组中指定列的数据。

	`data`是一个`list`型数据，长度必须与`columns`一致。

* **ArrayTool.replace_row(index, data)**

	替换二维数组中指定行的数据。

	`data`是一个`list`型数据，长度必须与`rows`一致。

* **ArrayTool.get_col(index)**

	获取二维数组中指定列的数据。

	`index`可以使用`-1`来获取最后一列。

* **ArrayTool.get_row(index)**

	获取二维数组中指定行的数据。

	`index`可以使用`-1`来获取最后一列。

* **ArrayTool.to_list()**

	将二维数组转换为一维数组列表。

	一维数组列表中数据的顺序与`ArrayTool.init()`的填充顺序一致。

### 常量列表

* **ArrayTool.SHIFT_UP**
* **ArrayTool.SHIFT_DOWN**
* **ArrayTool.SHIFT_LEFT**
* **ArrayTool.SHIFT_RIGHT**

	`ArrayTool.shift()`方法使用的常量。

## ColorTool 类

示例用法：

```python
from wtools import ColorTool

color = (128, 128, 128)

# h, s, v = ColorTool.rgb2hsv(color)
h, s, v = ColorTool.rgb2hsv(*color)

v *= 0.5

# color = ColorTool.hsv2rgb((h, s, v))
color = ColorTool.hsv2rgb(h, s, v)

print(ColorTool.gamma8(color))
```

### 构造器

* 无构造函数

### 静态方法列表

* **ColorTool.gamma8(int)**
* **ColorTool.gamma8(tuple(r, g, b))**

	输入一个整形数，或一个包含整形数的元祖，返回经过伽马矫正后的原类型数据。

	> 伽马矫正使用`Gamma 2.6`

* **ColorTool.rgb2hsv(r, g, b)**
* **ColorTool.rgb2hsv(tuple(r, g, b))**

	输入三个整形数，或一个包含整形数的元祖，返回对应的`HSV`元祖。

* **ColorTool.hsv2rgb(h, s, v)**
* **ColorTool.hsv2rgb(tuple(h, s, v))**

	输入三个整形数，或一个包含整形数的元祖，返回对应的`RGB`元祖。

## 合作交流

* 联系邮箱：<walkline@163.com>
* QQ 交流群：
	* 走线物联：[163271910](https://jq.qq.com/?_wv=1027&k=xtPoHgwL)
	* 扇贝物联：[31324057](https://jq.qq.com/?_wv=1027&k=yp4FrpWh)

<p align="center"><img src="https://gitee.com/walkline/WeatherStation/raw/docs/images/qrcode_walkline.png" width="300px" alt="走线物联"><img src="https://gitee.com/walkline/WeatherStation/raw/docs/images/qrcode_bigiot.png" width="300px" alt="扇贝物联"></p>
