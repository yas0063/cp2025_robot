// 第一回 PBL
// サンプルプログラム

#include <stdio.h>

#include "robot_simulator.h"
#define OBJECT_NUM 10

int main(void) {
  // アームの目標座標
  Position pos = {0.2, 0.1};

  // ロボットシミュレータ初期化
  bool is_initialized = initialize_robot(OBJECT_NUM);
  if (!is_initialized) {
    printf("System Error: \%s\n", get_system_error_str(get_system_error()));
    finalize_robot();
    return -1;
  }

  // ロボット駆動ループ
  while (update_robot()) {
    if (get_update_robot_step() == 500) set_command_move_arm_to(pos);
  }

  finalize_robot();
  return 0;
}
