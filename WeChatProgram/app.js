// app.js
App({
  onLaunch: function () {
    wx.getSystemInfo({
      success: e => {
        this.globalData.StatusBar = e.statusBarHeight;
        let custom = wx.getMenuButtonBoundingClientRect();
        this.globalData.Custom = custom;
        this.globalData.CustomBar = custom.bottom + custom.top - e.statusBarHeight;
      }
    });
  },
  //更新用户信息
  updateUserInfo: function () {
    var that = this;
    wx.request({
      url: "http://192.168.75.128:8000/login",
      data: {
        "StuID": this.globalData.userInfo.StuID,
        "Password": this.globalData.userInfo.Password
      },
      method: 'POST',
      success(res) {
        that.globalData.userInfo = res.data;
        that.globalData.userInfo.RestTime = Math.ceil(that.globalData.userInfo.RestTime / 60);
        that.globalData.userInfo.ReadTime = parseFloat(that.globalData.userInfo.ReadTime / 3600).toFixed(2);
        console.log('excute update!', that.globalData.userInfo)
      }
    });
  },
  globalData: {
    userInfo: null
  }
})