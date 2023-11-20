const app = getApp();
var intervalId = undefined;
Page({

  /**
   * 页面的初始数据
   */
  data: {
    userInfo: {},
    statusImg: {
      "Studying": "https://i.loli.net/2021/03/19/DbUHJEg3ud6GF4p.png",
      "NotOnSeat": "https://i.loli.net/2021/03/19/cf93yqkM5xF8Xpm.png",
      "NotSelect": "https://i.loli.net/2021/03/19/UJPbLxuBMZhtnGc.png"
    },
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

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
    intervalId = setInterval(() => {
      app.updateUserInfo();
      this.setData({
        userInfo: app.globalData.userInfo
      });
    }, 1000);
  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {
    clearInterval(intervalId);
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
  tapQuit: function () {
    var that = this;
    wx.showModal({
      title: '是否签退？',
      content: '注意：此动作不可逆',
      success(res) {
        if (res.confirm) {
          that.Quit();
          app.updateUserInfo();
        } else if (res.cancel) {
          // 用户点击取消
        }
      }
    })
  },
  //封装签退函数
  Quit: function () {
    wx.request({
      url: "http://192.168.75.128:8000/quit",
      method: 'POST',
      data: {
        StuID: app.globalData.userInfo.StuID
      },
      success(res) {
        console.log(res);
      }
    });
  },

})