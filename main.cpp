#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/ioctl.h>
#include <termios.h>
#include <cstdio>

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirecton dir;

/// http://www.flipcode.com/archives/_kbhit_for_Linux.shtml
/// Check KeyBoard Pressed Or Not
int _kbhit() {
  static const int STDIN = 0;
  static bool initialized = false;
  int bytesWaiting;

  if (!initialized) {
    // Use termios to turn off line buffering
    termios term;
    tcgetattr(STDIN, &term);
    term.c_lflag &= ~ICANON;
    tcsetattr(STDIN, TCSANOW, &term);
    setbuf(stdin, NULL);
    initialized = true;
  }

  ioctl(STDIN, FIONREAD, &bytesWaiting);
  return bytesWaiting;
}

void Setup() {
  gameOver = false;
  dir = STOP;
  x = width / 2;
  y = height / 2;
  fruitX = rand() % width;
  fruitY = rand() % height;
  score = 0;
}

/// Clear Background
void clear_background() {
  const char *clearcommand = "clear";
  system(clearcommand);
}

void Draw() {
  clear_background();
//  system("cls"); //system("clear");
  std::cout << std::endl;
  std::cout << " ";
  for (int i = 0; i < width + 2; i++)
    std::cout << "#";
  std::cout << std::endl;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (j == 0)
        std::cout << " #";
      if (i == y && j == x)
        std::cout << "O";
      else if (i == fruitY && j == fruitX)
        std::cout << "F";
      else {
        bool print = false;
        for (int k = 0; k < nTail; k++) {
          if (tailX[k] == j && tailY[k] == i) {
            std::cout << "o";
            print = true;
          }
        }
        if (!print)
          std::cout << " ";
      }

      if (j == width - 1)
        std::cout << "#";
    }
    std::cout << std::endl;
  }

  std::cout << " ";
  for (int i = 0; i < width + 2; i++)
    std::cout << "#";
  std::cout << std::endl;
  std::cout << " Score:" << score << std::endl;
}

/// Reaction On Press Button Of Keyboard
void reaction_on_keyboard(const char k) {
  if (k == 'd' || k == '6') {
    // Right Turn
    dir = RIGHT;

  } else if (k == 'a' || k == '4') {
    // Left Turn
    dir = LEFT;
  } else if (k == 'w' || k == '8') {

    // Turn UP
    dir = UP;
  } else if (k == 's' || k == '2') {
    // Turn Down
    dir = DOWN;
  } else if (k == 'q' || k == 'z' || k == 'c') {
    gameOver = true;
  }

}

void Input() {
  if (_kbhit()) {
    char k;
    std::cin >> k; /// Character
    reaction_on_keyboard(k);
  }
}
void Logic() {
  int prevX = tailX[0];
  int prevY = tailY[0];
  int prev2X, prev2Y;
  tailX[0] = x;
  tailY[0] = y;
  for (int i = 1; i < nTail; i++) {
    prev2X = tailX[i];
    prev2Y = tailY[i];
    tailX[i] = prevX;
    tailY[i] = prevY;
    prevX = prev2X;
    prevY = prev2Y;
  }
  switch (dir) {
    case LEFT:x--;
      break;
    case RIGHT:x++;
      break;
    case UP:y--;
      break;
    case DOWN:y++;
      break;
    default:break;
  }
  if (x > width || x < 0 || y > height || y < 0)
    gameOver = true;

  for (int i = 0; i < nTail; i++)
    if (tailX[i] == x && tailY[i] == y)
      gameOver = true;

  if (x == fruitX && y == fruitY) {
    // Random seed value for rand based on time
    srand(time(0));
    score += 10;
    fruitX = rand() % width;
    fruitY = rand() % height;
    nTail++;
  }
}

// Cross-platform sleep function
void sleepcp(int milliseconds)
{
  clock_t time_end;
  time_end = clock() + milliseconds * CLOCKS_PER_SEC / 1000;
  while (clock() < time_end) {
  }
}


int main() {
  Setup();
  int lap = 200;
  while (!gameOver) {
    Draw();
    Input();
    Logic();
    sleepcp(lap);
  }
  return 0;
}