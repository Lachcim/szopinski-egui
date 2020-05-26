const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');

module.exports = {
	output: {
		path: path.resolve(__dirname, 'build'),
		filename: 'szopinski-calendar.bundle.js',
		publicPath: '/'
	},
	module: {
		rules: [
			{
				test: /\.jsx?$/,
				exclude: /node_modules/,
				use: ['babel-loader']
			}
		]
	},
	plugins: [
		new HtmlWebpackPlugin({
			template: path.resolve(__dirname, 'src/index.html'),
		}),
	],
}
