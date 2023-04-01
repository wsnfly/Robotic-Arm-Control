/*
 * abujindianjipwm.h
 *
 *  Created on: Mar 22, 2023
 *      Author: HuaWei
 */

#ifndef ABUJINDIANJIPWM_H_
#define ABUJINDIANJIPWM_H_

typedef struct
{

//	__IO 	 int64_t jiasudu;  //加速度 步每秒
//	__IO 	 int64_t jiansudu;//减速度 步每秒

	__IO int64_t dangqiansudu;//当前运行速度
	__IO int64_t yudingsudu;//预定的运行速度
		 int64_t sudukongzhi;//速度控制器，控制加速或者减速

	__IO int64_t tingzhiweizhi;//最小停止位置20190702


	__IO int64_t zuidasudu ;//最大运行速度 步每秒
	__IO int64_t zuixiaosudu;//最小运行速度（启动速度） 步每秒

	int64_t jiluweizhi;//预定将要到达的步位置
	__IO int64_t dangqianweizhi;//当前步位置
	     int64_t yudingweizhi;//预定将要到达的步位置
	     int shijifangxiang;

} bujinpwm;




void jiasuleibiaojisuan(uint64_t,uint64_t,uint64_t,uint64_t);
void pwmzhongduankongzhi(void);
uint32_t s2b(uint32_t ,uint32_t );
uint64_t s2b2(uint64_t ,uint64_t  ,uint64_t );
#endif /* ABUJINDIANJIPWM_H_ */
