# 用途
学校服务器部署的VSCode不允许直接从剪贴板粘贴内容，此软件通过调用Windows API的虚拟键盘来间接实现粘贴

![使用示例](intro.gif "使用示例")
[视频介绍（已过时）](https://www.bilibili.com/video/BV1goYwzZEN3)

# 使用方法

在应用程序内打开文件，点击“准备写入”。在此之后，如果切出窗口，则在等待0.5秒后立刻进行文件的写入。

- 可以设置输入字符的时间间隔（默认2ms）

- 切回窗口可以暂停输出，点击“中止输出”可以直接停止输出

- **使用前尽量删除文本内非ASCII字符集的内容，否则会影响输入效果**

>注意：本软件的命令行版本已经弃用并归档，请务必使用Windows版本

# 特别提示
关于此项目的屎山代码分析：参见此[comment](https://github.com/Done-0/fuck-u-code/issues/2#issuecomment-3271347973)（已过时）

>屎山代码分析[项目地址](https://github.com/Done-0/fuck-u-code)