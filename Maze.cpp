#include "Maze.h"

Maze* Maze::instance = nullptr;

Maze::Maze(int y, int x) {
	gen = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
	dist = std::uniform_int_distribution<int>(0, 99);
	mat = std::vector<std::vector<unsigned char>>(y, std::vector<unsigned char>(x, 'O'));
	height = y;
	width = x;

	start[0] = 0;
	start[1] = 0;
	start[2] = 'O';
	goal[0] = y - 1;
	goal[1] = x - 1;
	goal[2] = 'O';

	addObstacles(OBSPERCENT);
}

int Maze::calculateH(int y, int x) {
	int heuristic = 0;

	y = abs(y - goal[0]);
	x = abs(x - goal[1]);

	heuristic = std::min(y, x);

	y -= heuristic;
	x -= heuristic;

	heuristic *= 14;

	if (y) {
		heuristic += y * 10;
	}

	if (x) {
		heuristic += x * 10;
	}

	return heuristic;
}

/*
-----------------------------------------------------------------------------------------
*/

// Node Subclass
Maze::Node::Node() {
	x = 0;
	y = 0;
	dist = 0;
	heur = 0;
	total = 0;
	total = INT_MAX;
	key = -1;
	prev = -1;
}

Maze::Node::Node(int yPos, int xPos, int distance, int heuristic, int currkey, int prevKey) {
	y = yPos;
	x = xPos;
	dist = distance;
	heur = heuristic;
	total = dist + heur;
	key = currkey;
	prev = prevKey;
}

/*
-----------------------------------------------------------------------------------------
*/

Maze* Maze::init() {
	if (!instance) {
		instance = new Maze(10, 10);
	}

	return instance;
}

Maze* Maze::init(int height, int width) {
	if (!instance) {
		height = std::min(height, MAXHEIGHT);
		width = std::min(width, MAXWIDTH);

		instance = new Maze(height, width);
	}

	return instance;
}

void Maze::uninit() {
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

void Maze::setStart(int y, int x) {
	start[2] = mat[y][x];
	mat[y][x] = '@';

	start[0] = y;
	start[1] = x;
}

void Maze::setGoal(int y, int x) {
	goal[2] = mat[y][x];
	mat[y][x] = '*';

	goal[0] = y;
	goal[1] = x;
}

int Maze::getHeight() {
	return height;
}

int Maze::getWidth() {
	return width;
}

int Maze::getMatHeight() {
	return mat.size();
}

int Maze::getMatWidth() {
	return mat[0].size();
}

int Maze::getStart1() {
	return start[0];
}

int Maze::getStart2() {
	return start[1];
}

int Maze::getGoal1() {
	return goal[0];
}

int Maze::getGoal2() {
	return goal[1];
}

void Maze::removeStart() {
	if (start[0] != -1) {
		mat[start[0]][start[1]] = start[2];
		start[0] = -1;
		start[1] = -1;
	}
}

void Maze::removeGoal() {
	if (goal[0] != -1) {
		mat[goal[0]][goal[1]] = goal[2];
		goal[0] = -1;
		goal[1] = -1;
	}
}

void Maze::removeColumn() {
	if (width == 1) {
		return;
	}

	if (start[1] + 1 == width) {
		removeStart();
	}

	if (goal[1] + 1 == width) {
		removeGoal();
	}

	for (int i = 0; mat.size() > i; i++) {
		mat[i].pop_back();
	}

	width--;
}

void Maze::removeRow() {
	if (height == 1) {
		return;
	}

	if (start[0] + 1 == height) {
		removeStart();
	}

	if (goal[0] + 1 == height) {
		removeGoal();
	}

	mat.pop_back();

	height--;
}

void Maze::removePath() {
	const int xDir[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const int yDir[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	int nextY;
	int nextX;
	int y;
	int x;

	std::queue<std::pair<int, int>> path;
	path.push({start[0],start[1]});
	
	while (!path.empty()) {
		y = path.front().first;
		x = path.front().second;
		path.pop();

		for (int i = 0; 8 > i; i++) {
			nextY = y + yDir[i];
			nextX = x + xDir[i];

			if (((nextY >= height || 0 > nextY) || (nextX >= width || 0 > nextX)) || (mat[nextY][nextX] != '!')) {
				continue;
			}

			mat[nextY][nextX] = 'O';
			path.push({nextY, nextX});
		}
	}
}

void Maze::addObstacles(int percent) {
	for (int y = 0; height > y; y++) {
		for (int x = 0; width > x; x++) {
			if (percent > dist(gen)) {
				mat[y][x] = 'X';
			}
			else {
				mat[y][x] = 'O';
			}
		}
	}

	if ((start[0] >= 0 && height > start[0]) && (start[1] >= 0 && width > start[1])) {
		mat[start[0]][start[1]] = '@';
	}

	if ((goal[0] >= 0 && height > goal[0]) && (goal[1] >= 0 && width > goal[1])) {
		mat[goal[0]][goal[1]] = '*';
	}
}

void Maze::addColumn() {
	if (width == MAXWIDTH) {
		return;
	}

	for (int i = 0; mat.size() > i; i++) {
		mat[i].emplace_back('O');
	}

	width++;
}

void Maze::addRow() {
	if (height == MAXHEIGHT) {
		return;
	}

	mat.emplace_back(std::vector<unsigned char>(mat[0].size(), 'O'));

	height++;
}

void Maze::flipCell(int y, int x) {
	switch (mat[y][x]) {
		case 'X':
			mat[y][x] = 'O';
			return;
		case 'O':
			mat[y][x] = 'X';
			return;
		default:
			return;
	}
}

char Maze::getCell(int y, int x) {
	return mat[y][x];
}

void Maze::printMaze() {
	for (int i = 0; mat.size() > i; i++) {
		for (int j = 0; mat[i].size() > j; j++) {
			switch (mat[i][j]) {
				case 'O':
					std::cout << "\033[1;34m";
					break;
				case 'X':
					std::cout << "\033[1;31m";
					break;
				case '@':
					std::cout << "\033[1;33m";
					break;
				case '*':
					std::cout << "\033[1;36m";
					break;
				case '!':
					std::cout << "\033[1;36;46mO \033[1;40m";
					continue;
				default:
					std::cout << "? ";
					continue;
			}

			std::cout << mat[i][j] << " ";
		}

		std::cout << "\n";
	}

	std::cout << std::endl;

	int temp = 0;
	std::cin >> temp;
}

void Maze::pathfind() {
	std::priority_queue<Node, std::vector<Node>, decltype([] (Node& left, Node& right) -> bool {return left.total > right.total;})> queue;
	std::vector<Node> nodeMap(width * height);

	const int dirCost[8] = { 10, 14, 10, 14, 10, 14, 10, 14 };
	const int xDir[8] = {0, 1, 1, 1, 0, -1, -1, -1};
	const int yDir[8] = {1, 1, 0, -1, -1, -1, 0, 1};
	
	queue.emplace(start[0], start[1], 0, calculateH(start[0], start[1]), (start[0] * width) + start[1], -1);
	nodeMap[queue.top().key] = queue.top();

	Node current;
	int newY;
	int newX;
	int dist;
	int heur;
	int key;

	while (!queue.empty()) {
		current = queue.top();

		if (current.y == goal[0] && current.x == goal[1]) {
			break;
		}

		queue.pop();

		for (int i = 0; 8 > i; i++) {
			newY = current.y + yDir[i];
			newX = current.x + xDir[i];
			
			if (((newY >= height || 0 > newY) || (newX >= width || 0 > newX)) || (mat[newY][newX] == 'X')) {
				continue;
			}

			key = (newY * width) + newX;
			dist = current.dist + dirCost[i];
			heur = calculateH(newY, newX);

			if (nodeMap[key].key == -1 || nodeMap[key].total > dist + heur) {
				nodeMap[key] = Node(newY, newX, dist, heur, key, current.key);
				queue.push(nodeMap[key]);
			}
		}
	}

	if (queue.empty()) {
		return;
	}

	while (current.prev != -1) {
		mat[current.y][current.x] = '!';
		current = nodeMap[current.prev];
	}

	mat[goal[0]][goal[1]] = '*';
}