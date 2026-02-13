#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <chrono>
#include <queue>
#include <unordered_map>

class Maze {
private:
	struct Node {
		int x, y, dist, heur, total, key, prev;

		Node();
		Node(int, int, int, int, int, int);
	};

	static const int THREADCOUNT = 6;
	static const int OBSPERCENT = 35;
	static const int MAXHEIGHT = 150;
	static const int MAXWIDTH = 255;

	static Maze* instance;

	std::vector<std::vector<unsigned char>> mat;
	std::default_random_engine gen;
	std::uniform_int_distribution<int> dist;

	short start[3];
	short goal[3];
	int height;
	int width;

	Maze();
	Maze(int, int);

	int calculateH(int, int);
public:
	static Maze* init();
	static Maze* init(int, int);
	static void uninit();

	void setStart(int, int);
	void setGoal(int, int);

	int getHeight();
	int getWidth();
	int getMatHeight();
	int getMatWidth();
	int getStart1();
	int getStart2();
	int getGoal1();
	int getGoal2();

	void removeStart();
	void removeGoal();
	void removeColumn();
	void removeRow();
	void removePath();

	void addObstacles(int);
	void addColumn();
	void addRow();

	void flipCell(int, int);
	char getCell(int, int);

	void printMaze();
	void pathfind();
};