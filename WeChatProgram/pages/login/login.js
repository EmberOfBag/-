var app = getApp();
Page({
  /**
   * 页面的初始数据
   */
  data: {
    stuID: "",
    password: "",
    signIn: { //注册相关组件
      isShow: false,
      stuID: "",
      password: "",
      confirmPwd: "",
    }
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    //当页面加载时，获取缓存里的学号密码
    var id = wx.getStorageSync('stuID');
    var psd = wx.getStorageSync('password');
    if (id.length==0)
      return
    wx.request({
      url: "http://192.168.75.128:8000/login",
      data: {
        "StuID": id,
        "Password": psd
      },
      method: 'POST',
      success(res) {
        var d = res.data;
        console.log(d);
        if (d == -1) {
          console.log("本地信息过期或者不存在");
        } else {
          //登录成功时，获取数据
          console.log(res)
          app.globalData.userInfo = res.data;
          //进入主界面
          wx.switchTab({
            url: '/pages/main/main/main',
          })
        }
      }
    });
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  },

  //获取输入的学号
  getUserName(e) {
    var value = e.detail.value; //获取输入的内容
    this.setData({
      stuID: value,
    });
    wx.setStorageSync('stuID', value); //将获取到的username值存入本地缓存空间
  },

  //获取输入的密码
  getPassword(e) {
    var value = e.detail.value;
    this.setData({
      password: value
    });
    wx.setStorageSync('password', value);
  },

  //登录
  dologin(e) {
    let that = this;
    if (this.data.stuID.length == 0 || this.data.password.length == 0) { //如果没有输入
      wx.showToast({
        title: "请输入用户名或密码",
        icon: "none"
      });
    } else {
      that.setData({
        loadStatus: true
      });
      //发送登陆请求
      wx.request({
        url: "http://192.168.75.128:8000/login",
        data: {
          "StuID": that.data.stuID,
          "Password": that.data.password
        },
        method: 'POST',
        success(res) {
          var d = res.data;
          that.setData({
            loadStatus: false
          });
          if (d == -1) { //返回-1说明用户名密码错误
            wx.showToast({
              title: "用户名或密码输入错误",
              icon: "none"
            });
          } else {
            //登录成功时，获取数据
            console.log(res)
            app.globalData.userInfo = res.data;
            //进入主界面
            wx.switchTab({
              url: '/pages/main/main/main',
            })
          }
        }
      });
    }
  },
  //点击注册按钮
  handleTapSignin: function () {
    this.setData({
      'signIn.isShow': true
    });
  },
  handleInputSignUsername(e) {
    var value = e.detail.value; //获取输入的内容
    this.setData({
      'signIn.stuID': value
    });
  },
  handleInputSignPwd(e) {
    var value = e.detail.value; //获取输入的内容
    this.setData({
      'signIn.password': value
    });
  },
  handleInputSignConfirmPwd(e) {
    var value = e.detail.value; //获取输入的内容
    this.setData({
      'signIn.confirmPwd': value
    });
  },
  handleTapConfirmSign() {
    console.log(this.data.signIn.stuID);
    if (this.data.signIn.stuID.length == 0 ||
      this.data.signIn.password.length == 0) {
      wx.showToast({
        title: '学号或密码不能为空',
        icon: 'none',
        duration: 2000
      })
    } else if (this.data.signIn.password == this.data.signIn.confirmPwd) {
      //向服务器发送请求
    } else {
      wx.showToast({
        title: '两次输入密码不一致',
        icon: 'none',
        duration: 2000
      })
    }
  },
  handleTapX: function () {
    this.setData({
      'signIn.isShow': false
    });
  }
})