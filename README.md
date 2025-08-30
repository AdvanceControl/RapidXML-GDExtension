# RapidXML GDExtension
## RapidXML GDExtension是什么
RapidXML GDExtension是一个为 Godot 创建的 XML 解析器。


## 特性
 - 基于高性能的 RapidXML C++ 库。
 - 简单的 API，用于解析 XML 文件。
 - 通过路径 (`root/child`) 查询节点和属性。

## 从源码构建
请打开终端或CMD，导航到根目录并执行
`scons platform= target=template_<release或debug> arch=`
如
`scons platform=windows target=template_<release> arch=x86_64`
等待构建完成

## 许可证
本项目同Godot和RapidXML使用 MIT 许可证。