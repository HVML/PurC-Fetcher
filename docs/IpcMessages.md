# 进程间通讯消息


## 概述

在PurC Fetcher中进程间通讯的主要功能在类`Connection`中实现，它的主要代码位于目录`Source/PurCFetcher/ipc/`下Connection.h/cpp之中。
与平台相关的部分实现在Connection[Suffix].cpp，例如 ConnectionUnix.cpp 实现了对于Unix平台的支持。

其大概过程如下：

1. 使用 `Connection::SocketPair createPlatformConnection` 创建一对 socket，并将 SocketPair.client 传递给子进程
1. 父子进程使用相关接口 `send` 和 `didReceiveMessage` 相关接口完成消息的发送和接收。
1. 对于 Unix 平台的实现 ConnectionUnix.cpp，使用 `socketpair` 来实现函数 `createPlatformConnection`，并使用 `sendmsg` 和 `recvmsg` 来实现消息的发送和接收。

## 通讯协议

在PurC Fetcher中进程间的通讯未定义传统意义上的通讯协议(HTTP(文本), TCP/IP(二进制))，而是使用了类的序列化/反序列化来进程通记。
具体来说发送消息时，使用工具类Encoder将消息序列化，在接收消息时再通过工具类Decoder将消息反序化。

## 消息

### 消息类的结构

在PurC Fetcher中每个消息都使用一个C++类来表示，它们有相似的结构，下面以消息`InitializeNetworkProcess`为例，介绍一下消息类的结构:

```

class InitializeNetworkProcess {
public:
    using Arguments = std::tuple<const PurCFetcher::NetworkProcessCreationParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_InitializeNetworkProcess; }
    static const bool isSync = false;

    explicit InitializeNetworkProcess(const PurCFetcher::NetworkProcessCreationParameters& processCreationParameters)
    : m_arguments(processCreationParameters)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

    private:
        Arguments m_arguments;
};

```

* 每个消息类都有一个数据成员 `m_arguments`，它本质是上由 `std::tuple` 封装起来的一组对象
* `IPC::MessageName name()` : 返回消息的唯一标识
* `bool isSync` : 表示是否是同步消息
* `const Arguments& arguments()` :  返回 `m_arguments`

### 消息类的生成

消息类是使用脚本自动生成的，脚本位于 `tools/Scripts/generate-message-receiver.py`，它的输入是`Source/PurCFetcher/messages/`目录下的后缀为`messages.in`的文件。
下面还是以消息 `InitializeNetworkProcess` 为例，说明 `messages.in` 文件的结构:

```

messages -> NetworkProcess LegacyReceiver {
    # Initializes the network process.
    InitializeNetworkProcess(struct PurCFetcher::NetworkProcessCreationParameters processCreationParameters)

    ...

}

```

