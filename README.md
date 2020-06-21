## 网络编程结课项目

# 前期准备:
在qt中使用c++进行界面开发。
网络搜集api,项目内使用的是网易云音乐的api(第三方)。
解码器使用lav filters。

# 实施方案:
1. 通过qnetworkaccessmanager、qnetworkrequest、qnetworkreply类进行网络通信调用搜集到的api。
2. 通过qjson相关类编写解析函数,提取网络请求返回的json数据中所需要的相关信息。
3. 通过qmediaplayer类播放音乐,音乐文件地址由json数据中解析得到。

# 成果:
达到预期的目标,可以搜索并播放音乐。
