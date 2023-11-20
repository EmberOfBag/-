import * as echarts from '../../../ec-canvas/echarts';
Page({
	/**
	 * 页面的初始数据
	 */
	data: {
		ec: {
			lazyLoad: true // 延迟加载
		}
	},
	/**
	 * 生命周期函数--监听页面加载
	 */
	onLoad: function (options) {
		this.echartsComponent = this.selectComponent('#mychart-dom-radar');
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
		this.initChart();
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
	//初始化折线图
	initChart: function() {
		this.echartsComponent.init((canvas, width, height) => {
			// 初始化图表
			const Chart = echarts.init(canvas, null, {
			  width: width,
			  height: height
			});
			var option = {
				radar: {
					shape: 'circle',
					center: ['50%', '40%'],
					name: {
						textStyle: {
							color: '#fff',
							backgroundColor: 'orange',
							borderRadius: 3,
							padding: [3, 5]
						}
					},
					indicator: [
						{ name: '空调需求', max: 1},
						{ name: '近门', max: 1},
						{ name: '靠窗', max: 1},
						{ name: '走道', max: 1},
						{ name: '厕所', max: 1}
					]
				},
				series: {
					type: 'radar',
					data: [{value:[0.5, 0.8 ,0.4 ,0.2, 0.8]}]
				}
			};
			Chart.setOption(option);
			return Chart;
		});
	},
})