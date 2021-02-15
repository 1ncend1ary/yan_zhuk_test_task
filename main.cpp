/*************************
 *  Copyright (C) 2021
 *  Aleksei Seliverstov
 *************************/

/* FILE NAME   : main.cpp
 * PURPOSE     : Main program module.
 * PROGRAMMER  : CSC'2021.
 *               Aleksei Seliverstov.
 * LAST UPDATE : 15.02.21.
 * NOTE        : Namespace 'snake_game'.
 *               This project follows the following code style:
 *               https://google.github.io/styleguide/cppguide.html
 *
 * No part of this file may be changed without agreement of
 * the above-mentioned authors
 */

#include "lib/utils.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <random>

#ifdef WIN32
#include <conio.h>
#include <windows.h>
#endif

/* Project namespace */
namespace snake_game {
/* Snake character class */
class Snake {
 public:
  /* Snake constructor
   *
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  Snake() : dir_(Direction::kStop) {
    tail_x_.resize(1);
    tail_y_.resize(1);
  }

 protected:
// Naming according to https://google.github.io/styleguide/cppguide.html#Enumerator_Names
  enum struct Direction {
    kStop,  // If the first enumerator value has no value, it defaults to 0
    kLeft,
    kRight,
    kUp,
    kDown
  };

 public:
  /* x coordinates vector getter.
   *
   * ARGUMENTS:
   *   - vector index:
   *       const size_t idx;
   * RETURNS: coordinate by index
   */
  int GetTailX(const size_t idx) const {
    return tail_x_.at(idx);
  }
  /* y coordinates vector getter.
   *
   * ARGUMENTS:
   *   - vector index:
   *       const size_t idx;
   * RETURNS: coordinate by index
   */
  int GetTailY(const size_t idx) const {
    return tail_y_.at(idx);
  }

  /* x coordinates vector setter.
   *
   * ARGUMENTS:
   *   - vector index:
   *       const size_t idx;
   *   - write value:
   *       int value;
   * RETURNS: None.
   */
  void SetTailX(const size_t idx, int value) {
    if (idx < tail_x_.size()) {
      tail_x_[idx] = value;
    }
  }
  /* y coordinates vector setter.
   *
   * ARGUMENTS:
   *   - vector index:
   *       const size_t idx;
   *   - write value:
   *       int value;
   * RETURNS: None.
   */
  void SetTailY(const size_t idx, int value) {
    if (idx < tail_y_.size()) {
      tail_y_[idx] = value;
    }
  }

  /* Tail size getter.
   *
   * ARGUMENTS: None.
   * RETURNS: current tail size
   */
  size_t TailSize() const {
    return tail_x_.size() - 1;
  }
  /* Increase tail size function.
   *
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  void IncreaseTail() {
    tail_x_.push_back(0);
    tail_y_.push_back(0);
  }
  /* Decrease tail size function.
   *
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  void DecreaseTail() {
    if (tail_x_.size() > 1) {
      tail_x_.pop_back();
      tail_y_.pop_back();
    }
  }

  /* Direction getter.
   *
   * ARGUMENTS: None.
   * RETURNS: resulting direction
   */
  Direction GetDir() const {
    return dir_;
  }
  /* Direction setter.
   *
   * ARGUMENTS:
   *   - direction to set:
   *       Direction dir;
   * RETURNS: None.
   */
  void SetDir(Direction dir) {
    dir_ = dir;
  }

 private:
  // todo combine tail vectors
  std::vector<int> tail_x_;
  std::vector<int> tail_y_;

  Direction dir_;
};

/* Field class */
class Field : public Snake {
 public:
  /* Field constructor
   * Sets current game mode:
   * 1 - Classic snake
   * 2 - Grow (F) and shrink (X) mode
   *
   * ARGUMENTS:
   *   - mode specifier:
   *       unsigned mode;
   * RETURNS: None.
   */
  explicit Field(unsigned mode) : x_(width_ / 2), y_(height_ / 2), score_(0) {
    /* Random device setup */
    std::random_device rd;
    mt = std::mt19937(rd());
    dist_w = std::uniform_real_distribution<int> (0, width_);
    dist_h = std::uniform_real_distribution<int> (0, height_);

    fruit_x_ = dist_w(mt);
    fruit_y_ = dist_h(mt);

    switch (mode) {
      case 1:
      default:anti_fruit_x_ = anti_fruit_y_ = -1;
        break;
      case 2:anti_fruit_x_ = dist_w(mt);
        anti_fruit_y_ = dist_h(mt);
        break;
    }
  }

  /* Field render function
   *
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  void Render() {
    clear_background();
    std::cout << std::endl;
    std::cout << " ";
    for (int i = 0; i < width_ + 2; i++) {
      std::cout << "#";
    }
    std::cout << std::endl;

    for (int i = 0; i < height_; i++) {
      for (int j = 0; j < width_; j++) {
        if (j == 0) {
          std::cout << " #";
        }
        if (i == y_ && j == x_) {
          std::cout << "o";
        } else if (i == fruit_y_ && j == fruit_x_) {
          std::cout << "F";
        } else if (i == anti_fruit_y_ && j == anti_fruit_x_) {
          std::cout << "X";
        } else {
          bool print = false;
          for (size_t k = 0; k < s_.TailSize(); k++) {
            if (s_.GetTailX(k) == j && s_.GetTailY(k) == i) {
              std::cout << "o";
              print = true;
            }
          }
          if (!print) {
            std::cout << " ";
          }
        }

        if (j == width_ - 1) {
          std::cout << "#";
        }
      }
      std::cout << std::endl;
    }

    std::cout << " ";
    for (int i = 0; i < width_ + 2; i++) {
      std::cout << "#";
    }
    std::cout << std::endl;
    std::cout << " Score:" << score_ << "     " << "x: " << x_ << " y: " << y_ << std::endl;
  }

  /* Apply keyboard logic function
   *
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  void Keyboard(bool *gameOver) {
    if (_kbhit()) {
#ifndef WIN32
      int k = getchar();
#else
      int k = _getch();
#endif
      enum Arrows : int {
        kLeftArrow = 37,
        kUpArrow,
        kRightArrow,
        kDownArrow
      };
      switch (k) {
        case 'd':
        case kRightArrow:s_.SetDir(Direction::kRight);
          break;
        case 'a':
        case kLeftArrow:s_.SetDir(Direction::kLeft);
          break;
        case 'w':
        case kUpArrow:s_.SetDir(Direction::kUp);
          break;
        case 's':
        case kDownArrow:s_.SetDir(Direction::kDown);
          break;
        case 'q':s_.SetDir(Direction::kStop);
          *gameOver = true;
          break;
        default:break;
      }
    }
  }

  /* Apply game logic function
   *
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  void Logic(bool *gameOver) {
    int prevX = s_.GetTailX(0);
    int prevY = s_.GetTailY(0);
    s_.SetTailX(0, x_);
    s_.SetTailY(0, y_);

    for (size_t i = 1; i < s_.TailSize(); i++) {
      int prev2X = s_.GetTailX(i);
      int prev2Y = s_.GetTailY(i);

      s_.SetTailX(i, prevX);
      s_.SetTailY(i, prevY);

      prevX = prev2X;
      prevY = prev2Y;
    }
    switch (s_.GetDir()) {
      case Direction::kLeft:x_--;
        break;
      case Direction::kRight:x_++;
        break;
      case Direction::kUp:y_--;
        break;
      case Direction::kDown:y_++;
        break;
      default:break;
    }
    if (x_ > width_ || x_ < 0 || y_ > height_ || y_ < 0) {
      *gameOver = true;
    }

    for (size_t i = 0; i < s_.TailSize(); ++i) {
      if (s_.GetTailX(i) == x_ && s_.GetTailY(i) == y_) {
        *gameOver = true;
      }
    }

    if (x_ == fruit_x_ && y_ == fruit_y_) {
      /* Random seed value for rand based on time */
      score_ += 10;
      fruit_x_ = dist_w(mt);
      fruit_y_ = dist_h(mt);
      s_.IncreaseTail();
    }

    if (x_ == anti_fruit_x_ && y_ == anti_fruit_y_) {
      /* Random seed value for rand based on time */

      score_ += 20;
      anti_fruit_x_ = dist_w(mt);
      anti_fruit_y_ = dist_h(mt);
      s_.DecreaseTail();
    }
  }

 private:
  const static int width_ = 50;
  const static int height_ = 20;
  int x_, y_;
  int fruit_x_, fruit_y_;
  int anti_fruit_x_, anti_fruit_y_;
  int score_;
  Snake s_;
  std::mt19937 mt;
  std::uniform_real_distribution<int> dist_w;
  std::uniform_real_distribution<int> dist_h;
};

}

/* Main program fucntion */
int main() {
  /* Set up local variables */
  bool game_over = false;
  const int kSleepTime = 200;  /* Sleep time between frames in milliseconds */

  std::cout << "Main menu" << std::endl <<
            "WASD to move, q to quit" << std::endl <<
            "Modes:" << std::endl <<
            "1 - Classic snake" << std::endl <<
            "2 - Grow (F) and shrink (X) mode" << std::endl <<
            "Select mode:";

  unsigned mode;
  std::cin >> mode;

  snake_game::clear_background();

  snake_game::Field f(mode);

  /* Main program loop */
  while (!game_over) {
    f.Render();
    f.Keyboard(&game_over);
    f.Logic(&game_over);
    snake_game::sleepcp(kSleepTime);
  }

  std::cout << "===== GAME OVER =====" << std::endl;
#ifdef WIN32
  _getch();  /* Wait for ke press */
#endif
  return 0;
}