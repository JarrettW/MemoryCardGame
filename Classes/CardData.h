//保存卡片的数据
#ifndef __CARD_DATA_H__
#define __CARD_DATA_H__
struct CardData {
	int number;    //编号
	int row;	   //行
	int column;	   //列
	int flipCount; //翻转次数
};
#endif