# GoTestServer

#### 介绍
    该系统由三个核心部分组成：Go 服务器、Qt 客户端和 Web 后台管理页面。这个系统允许用户通过 Qt 客户端和 Web 后台管理页面之间进行实时消息传递和管理。
1. golang测试并发服务器
2. [配套QT客户端可传输消息/表情/文件](https://gitee.com/shijingying/GoTestServer/tree/master/GoTestClient)
3. [web管理页面](https://gitee.com/shijingying/vue-go/tree/master/WebManagement/web-management)
4. [语音聊天测试软件（带AAC和OPUS编解码、UDP传输、mqtt传输）](https://gitee.com/shijingying/GoTestServer/blob/IMServer(FileSeparation)/GoTestClient/FFMPEGAudioTest.7z)
5. [MQTT测试服务器下载](https://www.emqx.io/zh/downloads)
6. [FFMPEG x264 windows编译](https://gitee.com/shijingying/GoTestServer/blob/ClientCodeCollation/GoTestClient/utils/video/ffmpeg%EF%BC%8Ch264%E7%BC%96%E8%AF%91.txt)


#### go linux编译
- set GOARCH=amd64
- go env -w GOARCH=amd64
- set GOOS=linux
- go env -w GOOS=linux
- go build
#### 恢复windows
- go env -w GOARCH=amd64
- go env -w GOOS=windows

#### 开发历程
| 时间  | 内容  |  备注 |
|---|---|---|
|  历史 |  完成服务器，客户端，网页的基础架构的搭建 |   |
|  2023-11-7 |  修改客户端解包BUG，优化文件传输方式 |   |
|  2023-11-22 |  完成部分网页接口菜单列表等 |   |
|  2024-1-4 |  优化文件传输方式，加速一下 |   |
|  2024-1-8 |  增加头像控件，增加头像的弹窗，升级聊天窗口，增加人员组织架构页面人员详细信息显示 |   |
|  2024-1-9 |  新增聊天气泡，组织架构与主页面分离改为提升控件 |   |
|  2024-1-10 |  消息聊天页面与主页面分离改为提升控件，增加收到消息后的闪烁，增加未读消息提示 |   |
|  2024-1-11 |  修改人员在线状态显示错误问题，修改文件发送和接收问题 |   |
|  2024-1-16 |  新增用户头像上传功能，修复一个服务器在高并发下出现的数据覆盖问题 |   |
|  2024-1-17 |  合并代码 |   |
|  2024-1-18 |  增加图片的发送接收和显示功能，功能仿照微信 |   |
|  2024-1-19 |  升级更改发送文字加图片时，分开发送 |   |
|  2024-1-23 |  增加客户端窗口抖动功能，优化文件发送功能 |   |
|  2024-1-24 |  增加表情动画功能，新增抖动消息文字提示，修复文件传输进度显示bug |   |
|  2024-1-31 |  增加图片传输功能A->图片上传->完成后发消息->B接收消息->有图片获取图片->获取完成后显示消息 |   |
|  2024-2-1 |  测试图片传输效果，增加点击图片最大化显示功能 |   |
|  2024-2-24 |  增加一份语音编码解码及UDP传输的测试代码包含AAC及OPUS编解码 |   |
|  2024-2-25 |  增加MQTT传输语音的测试代码 "QT MQTT模块安装方法："https://www.cnblogs.com/ybqjymy/p/17620277.html|   |
|  2024-3-9 |  完成初版内网语音通话，后续视频类似处理（"MQTT server安装:"https://www.emqx.io/zh/downloads）|   |
|  2024-3-11 |  内测版本Qt/X264视频编码解码显示已完成，测试编码后的数据只需要将data数据发出即可重新解码，[ffmpeg及x264库编译](https://gitee.com/shijingying/GoTestServer/blob/IMServer(FileSeparation)/GoTestClient/utils/video/ffmpeg%EF%BC%8Ch264%E7%BC%96%E8%AF%91.txt) |   |
|  2024-3-12 |  完成视频通话逻辑以及UI，新增了视频及语音的接受和拒绝UI界面 |   |
|  2024-3-13 |  完成关闭语音视频等通知对方的功能，到此所有基本功能都已完成，开始整理代码，切换到CodeCollation分支 |   |
|  2024-3-13 |  完成网络模块代码优化，完成主界面UI模块代码优化，优雅 |   |
|  2024-3-14 |  修复一些代码优化后的BUG，准备开启群组之路 |   |
|  2024-3-15 |  修复一些代码优化后的BUG，优化文件传输功能 |   |


#### 框架及页面
![服务框架](show/%E6%9C%8D%E5%8A%A1%E6%A1%86%E6%9E%B6.jpg)
![软件框架](show/%E8%BD%AF%E4%BB%B6%E6%A1%86%E6%9E%B6.jpg)
![输入图片说明](show/%E6%96%87%E4%BB%B6%E5%8F%91%E9%80%81.jpg)
![输入图片说明](show/%E7%BB%84%E7%BB%87%E6%9E%B6%E6%9E%84.jpg)
![输入图片说明](show/%E8%A1%A8%E6%83%85.jpg)
![视频通话](show/%E8%A7%86%E9%A2%91%E9%80%9A%E8%AF%9D.jpg)
![输入图片说明](show/%E8%AF%AD%E9%9F%B3%E9%80%9A%E8%AF%9D.jpg)

#### 参与贡献

李惠涛，王科雷，孙磊，章学勇



