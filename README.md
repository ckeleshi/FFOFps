﻿<a href="REVERSE.md">逆向分析内容点这里</a>

# 介绍

此插件支持修改QQ自由幻想帧数，获得流畅的游戏体验。</br>
目前插件支持任意图形方式启动游戏。强烈推荐使用OpenGL模式。</br>
**请自行承担使用本插件的风险！**</br>

# 安装方法
使用方法:将附件解压到qqffo.exe同一目录下。</br>
通过修改FFOFPS.ini中FPS数值调节帧数。默认值为60，取值范围是[33,9999]。</br>

# FAQ
### 1.为什么无论改成什么数值，帧数都没有变化？
ini被文本编辑器加上了UTF8签名的原因，在开头空一行即可。

### 2.为什么游戏不能达到我设定的帧数？
插件不能凭空变出性能来。

### 3.为什么帧数不能超过显示器刷新率？
请关闭垂直同步。

### 4.为什么多开之后帧数变低了/提升帧数之后干啥都卡？
**游戏在同一个循环内处理渲染和逻辑，意味着两者用同一个线程处理**</br>
所以帧数变高，显卡和CPU占用会一起变高。需要自行确定是CPU还是显卡瓶颈。如果是CPU瓶颈，尝试手动将游戏进程分散到其他CPU核心。</br>
Windows7的CPU调度貌似不如Windows10智能，更加需要人工干预。

### 5.为什么我的帧数波动很大？
电脑性能不能一直满足所设定的帧数，建议降低设置。
