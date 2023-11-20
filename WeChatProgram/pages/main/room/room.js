Page({

	/**
	 * 页面的初始数据
	 */
	data: {
		targetFunction: 0, //选择历史或者收藏座位
		isfold: true, //是否收起
		targetFloor:1,	//选择目标楼层
		functionRooms:[
			[
				{name:"一楼南自习室", seatId:"10", isOccupied:false},
				{name:"二楼平台自习室", seatId:"1", isOccupied:true},
				{name:"五楼自习室", seatId:"13", isOccupied:false},
				{name:"四楼实验楼自习室", seatId:"13", isOccupied:false}
			],
			[
				{name:"二楼平台自习室", seatId:"1", isOccupied:true}
			]
		],
		rooms:[
			[
				{name:"一楼南自习室",time:"全天",usage:10,amount:100},
				{name:"一楼东自习室",time:"全天",usage:20,amount:100},
				{name:"一楼西自习室",time:"全天",usage:30,amount:200},
			],
			[
				{name:"二楼平台自习区",time:"全天",usage:40,amount:100},
				{name:"二楼自习室",time:"全天",usage:50,amount:100},
				{name:"二楼环型阅读广场",time:"全天",usage:60,amount:200},
			],
			[
				{name:"三楼大厅自习室",time:"全天",usage:70,amount:100},
				{name:"三楼东自科书库",time:"全天",usage:80,amount:100},
				{name:"三楼西科书库",time:"全天",usage:100,amount:200},
			],
			[
				{name:"系统模型展示楼层",time:"全天",usage:0,amount:16}
			],
			[
				{name:"五楼自习室",time:"全天",usage:80,amount:300},
			]
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
	//切换历史选座/收藏座位
	switchFunction: function(e) {
		var n = e.currentTarget.id[4];
		if(n==this.data.targetFunction) {
			this.setData({isfold: !this.data.isfold});
		} else {
			this.setData({targetFunction:n});	this.setData({isfold: false});
		}
	},
	//切换楼层
	switchFloor: function(e) {
		var floornum = e.currentTarget.id[5]; //获取点击的楼层数
		this.setData({targetFloor: floornum});
	}
})