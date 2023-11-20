const app = getApp();
Page({

  /**
   * 页面的初始数据
   */
  data: {
    seats: [
      [],
      [],
      [],
      []
    ]
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
    this.getMap();
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

  //点绿色座
  tapGSeat: function (e) {
    if (app.globalData.userInfo.SeatID != -1) {
      wx.showToast({
        title: '你选过座位辣~',
        icon: 'none',
        duration: 2000
      });
    } else {
      var that = this;
      var id = e.currentTarget.id.replace("CG", ""); //获取点击的座位id
      wx.showModal({
        title: '选座提示',
        content: '确定选择' + id + "号座位？",
        success(res) {
          if (res.confirm) {
            that.chooseSeat(id);
            wx.reLaunch({
              url: '../main/main',
            })
          } else if (res.cancel) {
            // 用户取消了
          }
        }
      });
    }
  },
  //封装选座函数
  chooseSeat: function (id) {
    wx.request({
      url: "http://192.168.75.128:8000/select_chair",

      method: 'POST',
      data: {
        SeatID: id,
        StuID: app.globalData.userInfo.StuID,
        SeatLayer: 1
      },
      success(res) {
        console.log(res);
      }
    });
  },
  //点有人的座位
  tapRYSeat: function (e) {
    wx.showToast({
      title: '这个座位已经有人辣~',
      icon: 'none',
      duration: 2000
    });
  },
  //map对象转化为字符串
  mapobj2str: function (o) {
    return o.PositionType == 'C' ? ('C' + o.PositionValue + o.PositionID) : (o.PositionType);
  },
  //获取座位地图
  getMap: function () {
    var that = this;
    wx.request({
      url: "http://192.168.75.128:8000/get_map",
      method: 'GET',
      success(res) {
        var d = res.data;
        console.log(d);
        for (let i = 0; i < d.length; i++) {
          for (let j = 0; j < d[i].length; j++) {
            d[i][j] = that.mapobj2str(d[i][j]);
          }
        }
        console.log(d);
        that.setData({
          seats: d
        });
      }
    });
  }
})