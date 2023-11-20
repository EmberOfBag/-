from typing import Union
from fastapi import FastAPI
from pydantic import BaseModel
from datetime import datetime as dt
import pymysql


app = FastAPI()


def connectDB(host, username, password, database):
    db = pymysql.connect(host=host, user=username, passwd=password, db=database)
    return db


def updateReadTime(UserID):
    db = connectDB("127.0.0.1", "root", "123456", "UserInfo")
    cursor = db.cursor()
    sql = f"SELECT LastSelectTime, UseTime FROM BasicInfo WHERE UserID={UserID}"
    cursor.execute(sql)
    useInfo = cursor.fetchone()
    lastSelectTime = useInfo[0]
    UseTime = useInfo[1]
    nowTimeInt = dt.now()
    useTimeThis = nowTimeInt - lastSelectTime
    sql = f"UPDATE BasicInfo SET NotSitTime={UseTime+useTimeThis.seconds} WHERE UserID={UserID}"


@app.get("/")
def read_root():
    return {"Hello": "World"}


class LoginItem(BaseModel):
    UserID: str
    Password: str


@app.post("/login")
def signin(item: LoginItem):
    db = connectDB("127.0.0.1", "root", "123456", "UserInfo")
    cursor = db.cursor()  # 使用cursor()方法获取操作游标
    sql = f"SELECT Password FROM BasicInfo WHERE UserID = {item.UserID}"
    cursor.execute(sql)
    _pwd = cursor.fetchone()
    if _pwd is None or item.Password != _pwd[0]:
        return -1  # 可能为没有用户或密码错误
    else:
        sql = f"SELECT * FROM BasicInfo WHERE UserID = {item.UserID}"
        cursor.execute(sql)
        res = cursor.fetchone()
    db.close()
    return {
        "UserID": res[0],
        "UserName": res[1],
        "Gender": res[3],
        "Faculty": res[4],
        "Major": res[5],
        "PhoneNumber": res[6],
        "BornDate": res[7],
        "Password": res[2],
        "LastLogin": res[8],
        "SeatID": res[9],
        "RestTime": 60 - res[10],
        "Violation": res[11],
        "ReadTime": res[12],
        "UserStatus": res[13],
    }


@app.get("/get_map")
def get_map():
    db = connectDB("127.0.0.1", "root", "123456", "SeatInfo")
    cursor = db.cursor()  # 使用cursor()方法获取操作游标
    sql = "SELECT * FROM Room1"
    cursor.execute(sql)
    res = cursor.fetchall()  # 0-255
    data = [[None for j in range(16)] for i in range(16)]
    for idx, r in enumerate(res):
        data[idx // 16][idx % 16] = {
            "PositionType": r[1],
            "PositionID": r[2],
            "PositionValue": r[3],
        }
    db.close()
    return data


class SelectData(BaseModel):
    SeatID: str
    UserID: str


@app.post("/select_chair")
def select_chair(item: SelectData):
    # 改学生数据
    db = connectDB("127.0.0.1", "root", "123456", "UserInfo")
    cursor = db.cursor()
    sql = f"SELECT SeatID FROM BasicInfo WHERE UserID = {item.UserID}"
    cursor.execute(sql)
    SeatID = cursor.fetchone()[0]
    if SeatID != -1:  # 不为-1说明选过座，直接返回-1
        return -1
    sql = (
        f"UPDATE BasicInfo SET SeatID={item.SeatID}, UserStatus='NotOnSeat' "
        f"WHERE UserID={item.UserID}"
    )
    cursor.execute(sql)
    now_time = dt.now().strftime("%Y-%m-%d %H:%M:%S")
    sql = f"UPDATE BasicInfo SET LastSelectTime='{now_time}' WHERE UserID={item.UserID}"
    cursor.execute(sql)
    db.commit()
    db.close()
    # 改座位数据
    db = connectDB("127.0.0.1", "root", "123456", "SeatInfo")
    cursor = db.cursor()
    sql = f"UPDATE Room1 SET PositionValue='Y' WHERE SeatID={item.SeatID}"
    cursor.execute(sql)
    db.commit()
    db.close()
    return 1


class QuitData(BaseModel):
    UserID: str


@app.post("/quit")
def quit(item: QuitData):
    # 改学生数据
    db = connectDB("127.0.0.1", "root", "123456", "UserInfo")
    cursor = db.cursor()
    sql0 = f"SELECT SeatID FROM BasicInfo WHERE UserID = {item.UserID}"
    cursor.execute(sql0)
    SeatID = cursor.fetchone()[0]
    sql = f"UPDATE BasicInfo SET SeatID='-1', UserStatus='NotSelect' WHERE SeatID={SeatID}"
    cursor.execute(sql)
    db.commit()
    db.close()
    # 先改座位数据
    db = connectDB("127.0.0.1", "root", "123456", "SeatInfo")
    cursor = db.cursor()
    sql = f"UPDATE Room1 SET PositionValue='G' WHERE SeatID={SeatID}"
    cursor.execute(sql)
    db.commit()
    db.close()
    updateReadTime(item.UserID)

    return 1


class ChairData(BaseModel):
    chairlist: list


@app.post("/chair_check")
def chair_check(item: ChairData):
    # 先改椅子信息
    db = connectDB("127.0.0.1", "root", "123456", "SeatInfo")
    cursor = db.cursor()
    for idx, chair_status in enumerate(item.chairlist):
        if chair_status == "1":
            sql = f"UPDATE Room1 SET PositionValue='R' WHERE SeatID={idx}"
            cursor.execute(sql)
        else:
            sql = f"UPDATE Room1 SET PositionValue='Y' WHERE PositionValue='R' AND SeatID={idx}"
            cursor.execute(sql)
    db.commit()
    db.close()
    return "Server Received and Changed!"
