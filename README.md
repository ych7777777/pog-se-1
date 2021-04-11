<<<<<<< HEAD
# pog-se
现场v反对v
李子任尝试使用fork以及git
=======
## pog-se

- 项目名称：斗地主
- 项目平台：Windows 10
- 开发语言：C++

## 分工

目前我的主要重心在项目整体框架，设计接口，底层框架上面，以及一些调试方面(遇到问题可以找我来解决)。

其他部分我将分为：

- 游戏逻辑部分：这一部分主要是处理游戏业务的代码，比如说整个游戏流程的流程顺序，判断输赢，牌出的对不对，能不能这样出等等。这一部分分工为2人。
- 游戏UI部分：这一部分主要是处理游戏与用户交互的代码，比如说游戏的界面是什么样子，按钮，牌它要在哪里等等。这一部分分工为2人。
- 网络部分：这一部分主要是考虑网络通信，如何联机的代码。这一部分分工为1人进行一个初步设计，之后会增加人手。

## 设计想法

首先我们要解耦，就是说这三个部分不能揉在一起。对于游戏逻辑部分来说，它只是从UI部分和网络部分得到玩家的输入，然后进行游戏的逻辑判断，然后告诉UI或者网络到底发生了什么。这一部分我们也叫做服务端。

而对于游戏UI部分，它主要负责的是接收UI和网络的信息，然后将对应的游戏内容表现出来，以及接收用户的输入。这一部分我们也叫做客户端。

而对于网络部分，其实严格来说属于服务端，但是其稍微有些不同，因此拿出来单独考虑。

## 目前任务

- 对于逻辑部分的，可以考虑一下游戏流程，整个逻辑什么样子的。最好有一个思维导图或者图示。然后整理逻辑，设计接口。
- 对于UI部分的，熟悉ImGui，使用提供的项目测试和熟悉ImGui。
- 对于网络部分的，调研这一部分内容，做好准备(emmm不要到时候没方案啥的)。
- 我会把整个主程序的代码框架写出来，然后大家在这个框架里面进行添加和修改。
- 对于所有人，需要自己写一个规划或者说描述性文档。
    - 准确来说对于某个部分需要一个，所以是两人合写。
    - 比如说逻辑部分可以列出支持的功能，以及需要解决的功能，在大方向上要做什么。
    - 比如说UI部分可以大概规划下我们要的界面的样子，软件使用流程等等。
    - 同时也可以说明需要其他部分提供一些什么样子的需求和功能。
    - 做好后发给我，我需要根据这个设计我们的代码接口，因此如果设计不当可能会导致大家都得重新干。
    - 这一部分细化一些后就可以作为大家开发过程中的日志记录，用于在Timelines文件夹里面提交。
- 对于所有任务，当所有人完成一阶段后的任务后，我们才会继续推进。不设时间DDL，大家自觉。

>>>>>>> 14c4ac859f0642ed43f3eba2841c580f05ebd0d0
