// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    GridLocation s(cur.row-1, cur.col),
            n(cur.row+1, cur.col),
            w(cur.row, cur.col-1),
            e(cur.row, cur.col+1);
    if(maze.inBounds(s) && maze[s]==true) //第一个条件保障不超过迷宫边界。第二个条件保证不越过墙壁。
        neighbors.add(s);
    if(maze.inBounds(n) && maze[n]==true)
        neighbors.add(n);
    if(maze.inBounds(w) && maze[w]==true)
        neighbors.add(w);
    if(maze.inBounds(e) && maze[e]==true)
        neighbors.add(e);
    return neighbors;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void validatePath(Grid<bool>& maze, const Stack<GridLocation> path) {
    if(path.isEmpty())
        return;
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }

    /* TODO: Fill in the remainder of this function. */
    //1.起点必须在左上角
    GridLocation LeftUpper={0,0};
    //初始化一个path_tmp，把path_tmp排空直到最后一个元素，就得到了栈底
    Stack<GridLocation> path_tmp=path;
    while(path_tmp.size() != 1)
        path_tmp.pop();
    auto start = path_tmp.pop();
    if(start!=LeftUpper)
        error("起点必须在左上角!");
    //2.终点必须在右下角
        //已经被上面的代码写好了

    //3. 每一步移动都必须合规
    Stack<GridLocation> path_tmp2(path);
    while(!path_tmp2.isEmpty())
    {
        GridLocation cur = path_tmp2.pop();
        if(path_tmp2.isEmpty())
            break;
        GridLocation next = path_tmp2.peek();
        Set<GridLocation> const valMov =  generateValidMoves(maze, cur);
        if(!valMov.contains(next))
        {
            error("移动不合规！");
        }
    }

    //4.同一个位置不能走两次
    Set<GridLocation> unique;
    Stack<GridLocation> path_tmp3=path;
    while(!path_tmp3.isEmpty())
    {
        auto x = path_tmp3.pop();
        if(unique.contains(x))
        {
            error("同一个位置不能走两次!");
        }
        unique.add(x);
    }
    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without throwing any errors.
     */
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    /* TODO: Fill in the remainder of this function. */
    MazeGraphics::highlightPath(path,"yellow",1000);
    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}



void solutionToString(Stack<GridLocation> mySoln, Stack<string> & mySolnString)
{
    while(!mySoln.isEmpty())
    {
        auto loc = mySoln.pop();
        mySolnString.push(loc.toString());
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}





//-------------------------------------------
// TODO: add your test cases here
STUDENT_TEST("generateValidMoves 更多测试：正常")
{
    Grid<bool> maze = {{false, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation loc = {0, 1};
    Set<GridLocation> expected = {{1,1}, {0,2}};
    EXPECT_EQUAL(generateValidMoves(maze, loc), expected);
}

STUDENT_TEST("generateValidMoves 更多测试2:负数")
{
    Grid<bool> maze = {{false, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation loc = {0, -1};
    (generateValidMoves(maze, loc));
}
STUDENT_TEST("generateValidMoves 更多测试3:骑墙")
{
    Grid<bool> maze = {{false, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation loc = {0, 0};
    Set<GridLocation> expected = {{1,1}, {0,2}};
    (generateValidMoves(maze, loc));
}


STUDENT_TEST("validatePath 更多测试1: 17x37")
{
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/17x37.maze", maze);
    readSolutionFile("res/17x37.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}


PROVIDED_TEST("validatePath: 期待报错") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}


PROVIDED_TEST("迷宫求解 on file 5x7") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);
    Stack<GridLocation> mySoln = solveMaze(maze);
    Stack<string> mySolnString;
    while(!mySoln.isEmpty())
    {
        auto loc = mySoln.pop();
        mySolnString.push(loc.toString());
    }

    EXPECT_EQUAL(mySoln,soln);
}
