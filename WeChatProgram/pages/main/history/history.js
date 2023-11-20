import * as echarts from '../../../ec-canvas/echarts';

//折线图数据(获取过后直接给全局变量吧，不想再弄异步加载了，乏了||555，居然还是用了)
var x = [
	['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'],
	['1', '2', '3', '4', '5', '6', '7', '8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26','27','28','29','30'],
	['1','2','3','4','5','6','7','8','9','10','11','12']
];
var y = [
	[4,7,5,7,2,13,10],
	[5,8,22,8,20,0,12,18,16,7,5,1,23,16,16,21,19,16,23,7,10,3,13,18,16,6,20,18],
	[214,317,215,547,722,143,170,51,170,200,123,610]
];

Page({
	/**
	 * 页面的初始数据
	 */
	data: {
		index: -1,
		timePeriod: ["周历史","月度历史","年度历史"],
		ec: {
			lazyLoad: true // 延迟加载
		}
	},

	/**
	 * 生命周期函数--监听页面加载
	 */
	onLoad: function (options) {
		this.echartsComponnet = this.selectComponent('#mychart-dom-line');
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
	//切换picker的项
	bindPickerChange: function(e) {
		this.setData({
		  index: e.detail.value
		});
		this.initChart();
	}, 
	//初始化折线图
	initChart: function() {
		var index = this.data.index;
		this.echartsComponnet.init((canvas, width, height) => {
			// 初始化图表
			const Chart = echarts.init(canvas, null, {
			  width: width,
			  height: height
			});
			var option = {
				xAxis: {
					name: index=='0'?'星期':index==1?'天':'月',
					type: 'category',
					axisTick: {
						alignWithLabel:true
					},
					data: x[index]
				},
				yAxis: {
					name: '自习时长/h',
					type: 'value',
					min: '0',
					max: index=='0'?'24':index==1?'24':'744',
				},
				series: [{
					data: y[index],
					type: 'line'
				}]
			};
			Chart.setOption(option);
			return Chart;
		});
	},
})