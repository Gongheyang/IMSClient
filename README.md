# IMSProject
windows系统，即时通讯软件（包含客户端，服务端）


## 项目概况

即时通讯软件是通过即时通讯技术来实现在线聊天、交流的软件。有2种架构形式，一种是C/S架构，采用客户端/服务器形式，用户使用过程中需要下载安装客户端软件。
采用B/S架构，即浏览器/服务端形式，这种形式的即时通讯软件，直接借助互联网为媒介、客户端无需安装任何软件，既可以体验服务器端进行沟通对话，一般运用在电子商务网站的服务商。

本项目使用的是C/S架构。


## 目录结构

- IMSProject
  - build 工程文件
  - data 数据资源文件
  - doc：文档
  - src：源代码
    - Base 公共基础模块
	- APPServer 服务端
	- APPClient 客户端
  - config APP配置
  - include 公共头文件

## 开发环境搭建

开发环境：Window10
开发IDE：vs2015_update3


使用说明：

服务端配置： 需要配置./config/config.ini 文件中的 net项 下的port 

客户端配置： 需要根据服务端配置./config/config.ini 文件中的 net项 下的ip和port 

注意：使用时服务端必须先启动

