#ifndef COMMON_H
#define COMMON_H

enum ItemType {
	BLOCK_ITEM,
	LINE_ITEM,
	EDGE_ITEM,
	BALL_ITEM
};
#define WIDTH 510
#define HEIGHT 780
#define MOVE_STEP 3.0
class GameRule {//游戏规则，单例模式
private:
	int maxLife;
	int startStage;
	int breakRule;
	static GameRule* ss;

	GameRule();
public:
	int getMaxLife();
	int getStartStage();
	int getBreakRule();
	void setMaxLife(int x);
	void setStartStage(int x);
	void setBreakRule(int x);
	static GameRule* getInstance();
};

#endif
