<p align="center">
<a href="https://sm.ms/image/QHKGWyR8DJ9f3LX" target="_blank"><img src="https://s2.loli.net/2022/08/18/QHKGWyR8DJ9f3LX.jpg" alt="头像.jpg" style="width:100px; height:100px"></a></p>
</p>

<div align="center">

# 后端接口的README
易燃书包 
最后更新:2023/3/5

</div>

## 简述
项目原为大创项目"基于薄膜压力传感器的图书馆智能座位管理系统"，由我接手后作为毕业设计项目"智能座位管理系统"施行。

由于后端部分文件丢失，由我进行一定的重构。

此文件主要用于记录后端接口。

## 关键信息

### 系统信息
重构后使用虚拟机运行Ubuntu系统，使用MySQL数据库，使用[FastAPI](https://fastapi.tiangolo.com/zh/)框架(python)编写接口。

### 请求地址
由于没有上线需求，故通过请求本地IP获取数据

虚拟机Ubuntu系统的IP地址-2023/3/3有效

```
192.168.75.128
```
## 接口实现
- ### 登录接口

    #### 接口链接：
    http://192.168.75.128:8000/login

    #### 传输方式：
    POST

    #### 输入：
    ```javascript
    data = {
        "StuID":"xxxxxxxx",	\\账号
        "Password":"xxxxxxxx"	\\密码
        }
    ```

    #### 输出：
    （登录成功则输出学生个人信息，登陆失败则输出-1，均为json格式）
    ```json
    \\成功
    {
        "StuID":"xxxx", 		\\ 学号(string)
        "StuName":"xxxx",		\\ 姓名(string)
        "Gender":"xxxx",		\\ 性别(string)
        "Faculty":"xxxx",		\\ 学院(string)
        "Major":"xxxx",		    \\ 专业(string)
        "PhoneNumber":xxxx,	    \\ 电话号码(int)
        "BornDate":xxxx,		\\ 出生日期(data)
        "Password":"xxxx",		\\ 密码(string)
        "LastSelectTime":xxxx,	\\ 最后选坐日期(data)
        "SeatID":xxxx,			\\ 座位号(int)没有座位则为-1
        "RestTime":xxx - xxx,   \\ 座位剩余时间，为最大时间减去空置时间
        "Violation":xxx,	    \\ 用户违规次数
        "UserStatus":"xxxxx"    \\ 用户状态(string)，三种:NotSelect, NotOnSeat, Studying
        "ReadTime":xxx          \\ 阅读时间(int)，单位秒，记录用户总阅读时长
    }
    \\失败
    -1
    ```
    #### 备注：
    虽然名字是登录接口login，但实际返回的是用户的绝大多数(实际现在就是所有)信息，所以所有对用户数据信息的获取都是通过请求这个接口然后解析后得到的

- ### 选座接口
- 
    #### 接口链接：
    http://192.168.75.128:8000/select_chair

    #### 传输方式：
    POST

    #### 输入：
    
    ```javascript
    data = {
    "SeatID" = xxxx,	\\座位号(int)
    "StuID" = xxxx		\\学号(int)
	}
    ```

   ####  输出：
    
    ```json
    1	\\选座成功(int)
    -1	\\选座失败(int)
    ```

- ### 签退接口quit
- 
    #### 接口链接：
    http://192.168.75.128:8000/quit

    #### 传输方式：
    POST

    #### 输入：
    
    ```javascript
    data = {
        "StuID":"xxxxxxxx"	\\账号
	}
    ```

   ####  输出：
    
    ```json
    1	\\离开座位成功(int)
    -1	\\离开座位失败(int)
    ```

- ### 硬件接口
- 
    #### 接口链接：
    http://192.168.75.128:8000/chair_check

    #### 传输方式：
    POST

    #### 输入：
    [
        0=x,
        1=x,
        ...
        15=x
    ]

    #### 输出：
    ```
    "Server Received and Changed!"
    ```

- ### 获取地图接口
- 
    #### 接口链接：
    http://192.168.75.128:8000/get_map

    #### 传输方式：
    GET

    #### 输入：
    无

    #### 输出：
    ```
    data = [
    {"PositionType":"xxx","PositionID":"xxx","PositionValue":"xxx"},
    {"PositionType":"xxx","PositionID":"xxx","PositionValue":"xxx"},
    ···
    {"PositionType":"xxx","PositionID":"xxx","PositionValue":"xxx"}
    ]
    //参数说明：
    //在本接口返回有16×16个对象的一个数组，其中每一个对象包含三个键与值：
    //键PositionType为当前坐标上物体的类型，它有四种值：GND(地面)、D(桌子)、W(窗户)、C(椅子)，当且仅当变量为C时需要对PositionID与PositionValue进行判断
    //键PositionID为SeatID的关联，其值与SeatID的值一一对应，表示座位号
	//键PositionID为SeatStatus的关联，其值与SeatStatus的值一一对应，表示座位状态
    ```

## TODO
1. 由于前身为图书馆管理系统，现在作为座位管理系统在命名及数据管理等方面有一些不足及不合适的地方，记录在此：
    
    1. 登录及注册使用的是学号，可以改为手机号
    2. 用户信息里留有学院、专业等，虽然实际并未使用但可以去掉
    3. 数据库中ReadTime，为使用时长，名称不太对

2. 选坐接口的重构

3. 修改密码接口
   
    修改密码

    接口链接：http://www.tsgglxt.ltd/change_password.php

    传输方式：POST

    输入：

    ```js
    data = {
        "StuID":"xxxxxxxxxxx",		\\账号
        "Password":"xxxxxxxx",		\\原密码
        "NewPassword":"xxxxx"		\\新密码
  	}
    ```

    输出：

    ```json
    1	\\修改成功
    -1	\\修改失败，可能是密码或者账号错
    ```

4. 排名接口

## 已知问题
暂无

## 版本
### V0.0.3  2023/5/19
- [+]补充了接口信息

### V0.0.2  2023/3/6 
- [#]对登录接口完成了重构


### V0.0.1  2023/3/3 
- [#]对登录接口完成了重构
- [+]增加了UserStatus，ReadTime的返回，是由于之前的接口文件"接口实现1.8.md"可能为旧版本的接口文档，缺失了部分接口及接口内的信息


