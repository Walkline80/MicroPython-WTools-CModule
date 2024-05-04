"""
Copyright © 2024 Walkline Wang (https://walkline.wang)
Gitee: https://gitee.com/walkline/micropython-wtools-cmodule
"""
import time
from random import randint
from wtools import ArrayTool


def timed_function(f, *args, **kwargs):
	myname = str(f).split(' ')[1]

	def new_func(*args, **kwargs):
		t = time.ticks_us()
		result = f(*args, **kwargs)
		delta = time.ticks_diff(time.ticks_us(), t)
		print('Function {} Time = {:6.3f}ms'.format(myname, delta/1000))
		return result
	return new_func

def random_generator(self, max):
	while True:
		yield randint(0, max)


class ArrayToolPythonTest(object):
	UP    = 0
	DOWN  = 1
	LEFT  = 2
	RIGHT = 3

	def __init__(self, width, height):
		self.__width  = width
		self.__height = height

		self.__direction = random_generator(3)

		self.__shift = {
			self.UP   : self.__shift_up,
			self.DOWN : self.__shift_down,
			self.LEFT : self.__shift_left,
			self.RIGHT: self.__shift_right
		}

		self.__replace = {
			self.UP   : self.__replace_down,
			self.DOWN : self.__replace_up,
			self.LEFT : self.__replace_right,
			self.RIGHT: self.__replace_left
		}

	def run(self):
		self.__current_frame = [[0] * self.__width for _ in range(self.__height)]
		self.__in_frame      = [[0] * self.__width for _ in range(self.__height)]

		in_dots  = f'{0x5ebd:0>15b}'
		out_dots = f'{0x47e1:0>15b}'

		direction = next(self.__direction)

		for row in range(self.__height):
			for col in range(self.__width):
				self.__in_frame[row][col]  = int(in_dots[col * self.__height + row])
				self.__current_frame[row][col] = int(out_dots[col * self.__height + row])

		for count in range(5 if direction in (self.LEFT, self.RIGHT) else 7):
			if count in (0, 1):
				self.__shift[direction]()
			else:
				self.__shift[direction]()
				self.__replace[direction](count)

	def __shift_up(self):
		self.__current_frame.pop(0)
		self.__current_frame.append([0] * self.__width)

	def __shift_down(self):
		self.__current_frame.pop(-1)
		self.__current_frame.insert(0, [0] * self.__width)

	def __shift_left(self):
		for row in self.__current_frame:
			row.pop(0)
			row.append(0)

	def __shift_right(self):
		for row in self.__current_frame:
			row.pop(-1)
			row.insert(0, 0)

	def __replace_up(self, times):
		self.__current_frame[0] = self.__in_frame[-times + 1]

	def __replace_down(self, times):
		self.__current_frame[-1] = self.__in_frame[times - 2]

	def __replace_left(self, times):
		for index, row in enumerate(self.__current_frame):
			row[0] = self.__in_frame[index][-times + 1]

	def __replace_right(self, times):
		for index, row in enumerate(self.__current_frame):
			row[-1] = self.__in_frame[index][times - 2]


class ArrayToolCModuleTest(object):
	UP    = ArrayTool.SHIFT_UP
	DOWN  = ArrayTool.SHIFT_DOWN
	LEFT  = ArrayTool.SHIFT_LEFT
	RIGHT = ArrayTool.SHIFT_RIGHT

	def __init__(self, width, height):
		self.__width  = width
		self.__height = height

		self.__array_in  = ArrayTool(self.__width, self.__height)
		self.__array_out = ArrayTool(self.__width, self.__height)

		self.__direction = random_generator(3)

	def run(self):
		self.__array_in.init(0x47e1)
		self.__array_out.init(0x5ebd)

		direction = next(self.__direction)

		for count in range(5 if direction in (self.LEFT, self.RIGHT) else 7):
			if count in (0, 1):
				self.__array_out.shift(direction)
			else:
				self.__array_out.shift(direction)

				if direction in (self.UP, self.DOWN):
					row_index_1 = self.__height - 1 if direction == self.UP else 0
					row_index_2 = count - 2 if direction == self.UP else -count + 1
					self.__array_out.replace_row(row_index_1, self.__array_in.get_row(row_index_2))
				else:
					col_index_1 = self.__width - 1 if direction == self.LEFT else 0
					col_index_2 = count - 2 if direction == self.LEFT else -count + 1
					self.__array_out.replace_col(col_index_1, self.__array_in.get_col(col_index_2))


@timed_function
def python_testing(times=1000):
	testing = ArrayToolPythonTest(3, 5)

	for _ in range(times):
		testing.run()

@timed_function
def cmodule_testing(times=1000):
	testing = ArrayToolCModuleTest(3, 5)

	for _ in range(times):
		testing.run()

def run_test():
	times = 1000

	print(f'Testing...Python {times} times')
	python_testing(times)

	print(f'\nTesting...CModule {times} times')
	cmodule_testing(times)


if __name__ == '__main__':
	run_test()
