// 第二回 PBL
// サンプルプログラム

#include <stdio.h>

#include "robot_simulator.h"
#define OBJECT_NUM 5

typedef struct {
  int index;
  Position position;
  ColorRGB color;
  double radius;
  double weight;
} Object;

typedef enum { MEASURE, APPROACH, PICKUP, RELEASE, FINISH } TaskState;
TaskState task_state = MEASURE;

// n番目の物体の重量を計測する
void approach_and_weigh(Object *object, int n) {
  // 課題の達成状況(task_state)で場合分け
  switch (task_state) {
    case MEASURE:
      object[n].index = n + 1;
      object[n].position = get_object_position(n);
      task_state = APPROACH;
      break;

    case APPROACH:  // 物体へ移動
      set_command_move_arm_to(object[n].position);
      task_state = PICKUP;
      break;

    case PICKUP:  // 物体を把持
      set_command_pick_up_object();
      task_state = RELEASE;
      break;

    case RELEASE:  // 重量を計測して、物体を離す
      object[n].weight = get_held_object_weight();
      set_command_release_object();
      task_state = FINISH;
      break;

    case FINISH:
      break;
  }
}

// 物体情報の表示
void object_info_display(Object *object) {
  printf("\n------- Object Information ---------\n");
  printf("index = %2d, x = %6.2f, y = %6.2f, weight = %6.2f\n", object[0].index,
         object[0].position.x, object[0].position.y, object[0].weight);
}

int main(void) {
  Object object[OBJECT_NUM];  // 物体情報を格納する構造体

  // ロボットシミュレータ初期化
  bool is_initialized = initialize_robot(OBJECT_NUM);
  if (!is_initialized) {
    printf("System Error: \%s\n", get_system_error_str(get_system_error()));
    finalize_robot();
    return -1;
  }

  // ロボット駆動ループ
  while (update_robot()) {
    // 物体の重量を計測
    if (get_arm_state() == ARM_STATE_STOP) {
      if (task_state != FINISH) {
        approach_and_weigh(object, 0);
      } else {
        // 計測が終わったら表示
        object_info_display(object);
        break;
      }
    }
  }

  finalize_robot();
  return 0;
}
