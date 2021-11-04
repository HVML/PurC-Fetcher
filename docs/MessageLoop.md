# 消息循环

- [基础接口](#基础接口)
- [现状](#现状)
- [将来](#将来)

## 基础接口

消息循环的主要代码 `RunLoop.h/cpp` 位于目录 `Source/WTF/wtf/`，它实现了通用消息循环的基础封装。
它的主要接口如下:

- `static void initializeMain()`: 初始化主消息循环
- `static RunLoop& main()`: 获取主消息循环
- `static bool isMain()`: 判断是否是主消息循环
- `void dispatch(Function<void()>&&) final`: 将函数加入消息循环
- `static void run()`: 执行消息循环(不同平台需要单独实现)
- `void stop()`: 停止(不同平台需要单独实现)
- `void wakeUp()`: 唤醒(不同平台需要单独实现)

## 现状

目前使用的是使用glib实现的`RunLoop`，其代码位于 `Source/WTF/wtf/glib/RunLoopGLib.cpp`。

## 将来

考虑将来使用 MiniGUI 消息循环来实现 `RunLoop`。
