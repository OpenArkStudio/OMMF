# OMMF(Object-Manager-Message-Function)
[![Build Status](https://travis-ci.org/freeeyes/OMMF.svg?branch=master)](https://travis-ci.org/freeeyes/OMMF)
##OMMF目标
通过XML，设计逻辑流程，并自动生成对应的类代码，数据库存储代码，缓冲层读写代码。
节省不必要的程序员编写类读写代码的时间。
并且通过配置文件，实现梳理业务流程的目的。
解决目前开发中，个人随意声明结构体给代码阅读和维护带来麻烦，逻辑不清的问题。
初步目标是机器生成的代码替代实际中的代码30%的工作量。

## OMMF 开发计划 2018
- [x] 1.开发支持mysql和类对象关系的xml配置方法。
- [x] 2.开发支持Mysql的数据读写代码自动生成的功能。
- [x] 3.开发支持Mysql数据库脚本自动化生成的代码。
- [x] 4.开发支持将mysql数据读函数和ObjectManager管理类互通的接口。
- [ ] 6.添加OMMF开发文档，使用文档。

![设计导图](image.jpg)