// 第一回 総合演習
// 状態遷移を用いたアームの移動

#include <stdio.h>

#include "robot_simulator.h"
#define OBJECT_NUM 10

typedef enum {
  MOVE_TO_A,
  MOVE_TO_B,
  FINISH,
} PositionState;

int main(void) {
  // アームの目標座標
  Position posA = {0.5, 0.2};
  Position posB = {-0.5, 0.2};

  // Aへの移動から開始
  PositionState position_state = MOVE_TO_A;

  // ロボットシミュレータ初期化
  bool is_initialized = initialize_robot(OBJECT_NUM);
  if (!is_initialized) {
    printf("System Error: %s\n", get_system_error_str(get_system_error()));
    finalize_robot();
    return -1;
  }

  // まずはposAへ動き始める．
  set_command_move_arm_to(posA);

  // ロボット駆動ループ
  while (update_robot()) {
    // 移動完了してアームが停止していれば、次の移動を開始
    if (get_arm_state() == ARM_STATE_STOP) {
      switch (position_state) {
        case MOVE_TO_A:
          set_command_move_arm_to(posB);
          position_state = MOVE_TO_B;
          break;
        case MOVE_TO_B:
          position_state = FINISH;
          break;
        default:
          break;
      }
    }
  }

  finalize_robot();
  return 0;
}
