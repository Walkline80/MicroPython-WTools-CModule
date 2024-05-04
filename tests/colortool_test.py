"""
Copyright © 2024 Walkline Wang (https://walkline.wang)
Gitee: https://gitee.com/walkline/micropython-wtools-cmodule
"""
import time
from random import randint
from wtools import ColorTool


def timed_function(f, *args, **kwargs):
	myname = str(f).split(' ')[1]

	def new_func(*args, **kwargs):
		t = time.ticks_us()
		result = f(*args, **kwargs)
		delta = time.ticks_diff(time.ticks_us(), t)
		print('Function {} Time = {:6.3f}ms'.format(myname, delta/1000))
		return result
	return new_func

def random_generator(max):
	while True:
		yield randint(0, max), randint(0, max), randint(0, max)


class ColorToolPythonTest(object):
	def __init__(self):
		self.__color  = random_generator(255)
		self.__bright = random_generator(100)

	def run(self):
		color  = next(self.__color)
		bright = next(self.__bright)[0]

		if isinstance(color, tuple) and len(color) == 3:
			h, s, v = self.rgb2hsv(*color)
			v *= bright / 100
			r, g, b = self.hsv2rgb(h, s, v)

	def rgb2hsv(self, r:int, g:int, b:int) -> tuple:
		r, g, b = r / 255.0, g / 255.0, b / 255.0
		max_value = max(r, g, b)
		min_value = min(r, g, b)
		delta = max_value - min_value

		if delta == 0:
			return 0, 0, max_value
		elif max_value == r:
			h = (g - b) / delta % 6
		elif max_value == g:
			h = (b - r) / delta + 2
		elif max_value == b:
			h = (r - g) / delta + 4

		h = h * 60

		if h < 0:
			h += 360

		s = delta / max_value
		v = max_value

		return h, s, v

	def hsv2rgb(self, h:float, s:float, v:float) -> tuple:
		if h >= 360:
			h = 0

		if s == 0:
			r, g, b = v, v, v
		else:
			h = h / 60.0
			i = int(h)
			f = h - i
			a = v * (1 - s)
			b = v * (1 - s * f)
			c = v * (1 - s * (1 - f))

			if i == 0:
				r, g, b = v, c, a
			elif i == 1:
				r, g, b = b, v, a
			elif i == 2:
				r, g, b = a, v, c
			elif i == 3:
				r, g, b = a, b, v
			elif i == 4:
				r, g, b = c, a, v
			else:
				r, g, b = v, a, b

		return int(r * 255), int(g * 255), int(b * 255)


class ColorToolCModuleTest(object):
	def __init__(self):
		self.__color  = random_generator(255)
		self.__bright = random_generator(100)

	def run(self):
		color  = next(self.__color)
		bright = next(self.__bright)[0]

		if isinstance(color, tuple) and len(color) == 3:
			h, s, v = ColorTool.rgb2hsv(*color)
			v *= bright / 100
			r, g, b = ColorTool.hsv2rgb(h, s, v)


def comapre_values():
	python = ColorToolPythonTest()

	for _ in range(5):
		color = next(python.__color)
		bright = next(python.__bright)[0]
		print(f'-- color: {color}, bright: {bright}')

		h_p, s_p, v_p = python.rgb2hsv(*color)
		h_c, s_c, v_c = ColorTool.rgb2hsv(*color)
		print(f' python(hsv): ({h_p}, {s_p}, {v_p})')
		print(f'cmodule(hsv): ({h_c}, {s_c}, {v_c})')

		v_p *= bright / 100
		v_c *= bright / 100

		r_p, g_p, b_p = python.hsv2rgb(h_p, s_p, v_p)
		r_c, g_c, b_c = ColorTool.hsv2rgb(h_c, s_c, v_c)
		print(f' python(rgb): ({r_p}, {g_p}, {b_p})')
		print(f'cmodule(rgb): ({r_c}, {g_c}, {b_c})\n')

@timed_function
def python_testing(times=1000):
	testing = ColorToolPythonTest()

	for _ in range(times):
		testing.run()

@timed_function
def cmodule_testing(times=1000):
	testing = ColorToolCModuleTest()

	for _ in range(times):
		testing.run()

def run_test():
	times = 10000

	comapre_values()

	print(f'Testing...Python {times} times')
	python_testing(times)

	print(f'\nTesting...CModule {times} times')
	cmodule_testing(times)


if __name__ == '__main__':
	run_test()
