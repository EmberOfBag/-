var app = getApp();
Page({

	/**
	 * 页面的初始数据
	 */
	data: {
		isLoading: false,
		showViolation: false,
		userInfo:''
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
		this.setData({userInfo: app.globalData.userInfo});
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
	//显示展现违规次数的弹窗
	showModal: function() {
		this.setData({isLoading:true});
		setTimeout(()=>{
			this.setData({
				isLoading: false,
				showViolation: true
			});
		},500);
	},
	//关闭弹窗
	closeModal: function() {
		this.setData({showViolation: false});
	},
	//点击背景图上的三个点
	tapMoreBtn: function() {
		wx.showActionSheet({
			itemList: ['更换背景', '编辑个人资料'],
			success (res) {
				var i = res.tapIndex;
				if(i==1) {
					wx.navigateTo({url: '../info/info'});
				}
			},
			fail (res) {
			  console.log(res.errMsg)
			}
		})
	},
	tapExit: function(){
		wx.clearStorage();
		wx.reLaunch({
			url: '../../login/login',
		})
	}
})