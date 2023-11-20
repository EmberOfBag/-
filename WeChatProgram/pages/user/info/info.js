// pages/user/info/info.js
const app = getApp();
Page({
  /**
   * 页面的初始数据
   */
  data: {
    userInfo: null,
    isChangepwdModalShow: false,
    oldpwd: '',
    newpwd: '',
    confirmpwd: ''
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    app.updateUserInfo(); //更新用户信息
    this.setData({
      userInfo: app.globalData.userInfo
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
  //点击修改密码
  bindEditBtn() {
    this.setData({
      isChangepwdModalShow: true
    });
  },
  //关闭弹窗
  closeModal: function () {
    this.setData({
      isChangepwdModalShow: false
    });
  },
  //绑定修改密码的那几个框
  bindOldPwdInput: function (e) {
    this.setData({
      oldpwd: e.detail.value
    });
  },
  bindNewPwdInput: function (e) {
    this.setData({
      newpwd: e.detail.value
    });
  },
  bindConfirmPwdInput: function (e) {
    this.setData({
      confirmpwd: e.detail.value
    });
  },
  handleTapConfirmBtn: function () {
    let opwd = this.data.oldpwd;
    let npwd = this.data.newpwd;
    let cpwd = this.data.confirmpwd;
    if (opwd.length == 0 || npwd.length == 0 || cpwd.length == 0) {
      wx.showToast({
        title: '请输入有效值',
        icon: 'none'
      })
    } else if (opwd == npwd) {
      wx.showToast({
        title: '新旧密码不能一致',
        icon: 'none'
      })
    } else if (npwd != cpwd) {
      wx.showToast({
        title: '确认密码错误',
        icon: 'none'
      })
    } else {
      wx.request({
        url: "http://192.168.75.128:8000/change_password",

        data: {
          "StuID": app.globalData.userInfo.StuID,
          "Password": opwd,
          "NewPassword": npwd
        },
        method: 'POST',
        success(res) {
          console.log(res);
          wx.reLaunch({
            url: '../../login/login',
          })
        }
      });
    }
  }
})