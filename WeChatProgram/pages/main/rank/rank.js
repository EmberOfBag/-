// pages/rank/rank.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    target: "rank_day",
    Ranks: {
      "rank_day": [],
      "rank_week": [],
      "rank_month": []
    }
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
    this.updataRankInfos();
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

  //切换排名榜单
  switchTab: function (e) {
    this.setData({
      target: e.target.id.split("-").join("_")
    });
  },

  //获取日排名信息
  getDayRankInfos: function () {
    let that = this;
    wx.request({
      url: "http://192.168.75.128:8000/order_7",
      method: 'POST',
      success(res) {
        let rankInfos = res.data;
        for (let i of rankInfos) {
          i.ReadTime_7 = (i.ReadTime_7 / 3600).toFixed(1);
        }
        that.setData({
          "Ranks.rank_day": res.data
        });
      }
    });
  },
  //获取周排名信息
  getWeekRankInfos: function () {
    let that = this;
    wx.request({
      url: "http://192.168.75.128:8000/order_7",
      method: 'POST',
      success(res) {
        let rankInfos = res.data;
        for (let i of rankInfos) {
          i.ReadTime_7 = (i.ReadTime_7 / 3600).toFixed(1);
        }
        that.setData({
          "Ranks.rank_week": res.data
        });
      }
    });
  },
  //获取月排名信息
  getMonthRankInfos: function () {
    let that = this;
    wx.request({
      url: "http://192.168.75.128:8000/order_7",
      method: 'POST',
      success(res) {
        let rankInfos = res.data;
        for (let i of rankInfos) {
          i.ReadTime_7 = (i.ReadTime_7 / 3600).toFixed(1);
        }
        that.setData({
          "Ranks.rank_month": res.data
        });
      }
    });
  },
  updataRankInfos: function () {
    this.getDayRankInfos();
    this.getWeekRankInfos();
    this.getMonthRankInfos();
  }
})