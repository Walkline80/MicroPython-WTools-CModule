<h1 align="center">MicroPython WTools CModule</h1>

<p align="center"><img src="https://img.shields.io/badge/Licence-MIT-green.svg?style=for-the-badge" /></p>

## 代码格式化及拼写检查

### 格式化代码

要格式化代码需要安装`uncrustify`工具，根据官方文档说明，只能使用`v0.72`版本进行格式化操作：

```
sudo apt install uncrustify
```

使用如下命令进行代码格式化操作：

```bash
cd/to/micropython
tools/codeformat.py -c ../wtools/cmodules/wtools/modwtools.c
tools/codeformat.py -c ../wtools/cmodules/wtools/modwtools.h
```

### 拼写检查

要进行拼写检查，需要安装`codespell`工具：

```bash
pip install codespell tomli
```

使用如下命令进行拼写检查操作：

```bash
cd/to/micropython
codespell ../wtools/
```

## 参考资料

[Git commit conventions](https://github.com/micropython/micropython/blob/master/CODECONVENTIONS.md)
