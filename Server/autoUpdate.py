import pymysql  # 导入模块
import time
from datetime import datetime as dt

runFirstTimeFlag = True


def connectDB(host, username, password, database):
    db = pymysql.connect(host=host, user=username, passwd=password, db=database)
    return db


def updateUserStatus():
    global runFirstTimeFlag
    global SeatsInfoPre
    global SeatsInfoNow
    db = connectDB("127.0.0.1", "root", "123456", "SeatInfo")
    sql = """SELECT SeatID, PositionValue FROM Room1 WHERE PositionType = 'C' """
    cursor = db.cursor()
    cursor.execute(sql)
    SeatsInfoNow = cursor.fetchall()
    if runFirstTimeFlag == True:
        runFirstTimeFlag = False
        SeatsInfoPre = SeatsInfoNow
    db.commit()
    db.close()

    db = connectDB("127.0.0.1", "root", "123456", "StudentInfo")
    for idx, nowSeat in enumerate(SeatsInfoNow):  # 修改StudentInfo的UserStatus
        preSeat = SeatsInfoPre[idx]
        if preSeat != nowSeat:
            SeatID = nowSeat[0]
            if nowSeat[1] == "R":
                sql = (
                    f"UPDATE BasicInfo SET UserStatus='Studying' WHERE SeatID={SeatID}"
                )
                cursor.execute(sql)
            if nowSeat[1] == "Y":
                sql = (
                    f"UPDATE BasicInfo SET UserStatus='NotOnSeat' WHERE SeatID={SeatID}"
                )
                cursor.execute(sql)
                now_time = dt.now().strftime("%Y-%m-%d %H:%M:%S")
                sql = f"UPDATE BasicInfo SET LastSelectTime='{now_time}' WHERE SeatID={SeatID}"
                cursor.execute(sql)
    db.commit   ()
    db.close    ()


def updateRestTime():
    db = connectDB("127.0.0.1", "root", "123456", "StudentInfo")
    cursor = db.cursor()
    sql = f"UPDATE BasicInfo SET NotSitTime=-1 WHERE UserStatus='NotSelect' AND NotSitTime!=-1"
    cursor.execute(sql)  # 先将没选坐的NotSitTime置为-1
    sql = f"SELECT StuID, LastSelectTime FROM BasicInfo WHERE UserStatus='NotOnSeat'"
    cursor.execute(sql)
    NotSitInfo = cursor.fetchall()
    for student in NotSitInfo:
        StuID = student[0]
        lastSelectTime = student[1]
        nowTimeInt = dt.now()
        NotSitTime = nowTimeInt - lastSelectTime
        sql = (
            f"UPDATE BasicInfo SET NotSitTime={NotSitTime.seconds} WHERE StuID={StuID}"
        )
        cursor.execute(sql)
    db.commit()
    db.close()


def updateViolation(maxVacantTime):
    db = connectDB("127.0.0.1", "root", "123456", "StudentInfo")
    cursor = db.cursor()
    sql = f"SELECT StuID, SeatID, NotSitTime, Violation FROM BasicInfo WHERE UserStatus='NotOnSeat'"
    cursor.execute(sql)
    NotSitTimeInfoList = cursor.fetchall()
    db.commit()
    db.close()
    for StuID, SeatID, NotSitTime, Violation in NotSitTimeInfoList:
        if NotSitTime > maxVacantTime:
            db = connectDB("127.0.0.1", "root", "123456", "StudentInfo")
            cursor = db.cursor()
            sql = f"UPDATE BasicInfo SET Violation={Violation+1} WHERE StuID={StuID}"
            cursor.execute(sql)
            sql = f"UPDATE BasicInfo SET NotSitTime={0} WHERE StuID={StuID}"
            cursor.execute(sql)
            sql = f"UPDATE BasicInfo SET UserStatus='NotSelect' WHERE StuID={StuID}"
            cursor.execute(sql)
            sql = f"UPDATE BasicInfo SET SeatID=-1 WHERE StuID={StuID}"
            cursor.execute(sql)
            db.commit()
            db.close()
            db = connectDB("127.0.0.1", "root", "123456", "SeatInfo")
            cursor = db.cursor()
            sql = f"UPDATE Room1 SET PositionValue='G' WHERE SeatID={SeatID}"
            cursor.execute(sql)
            db.commit()
            db.close()


while True:
    updateUserStatus()
    updateRestTime()
    updateViolation(30)
    time.sleep(0.5)
