// 第三回 PBL
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

typedef enum {
  MEASURE,
  SEARCH,
  APPROACH,
  PICKUP,
  MOVE,
  PLACE,
  FINISH
} TaskState;
TaskState task_state = MEASURE;

// 最小サイズの物体番号を返す関数
int minimum_object_number(Object *object) {
  int i;
  int min_num = 0;
  double min_value = object[0].radius;

  for (i = 1; i < OBJECT_NUM; i++) {
    if (min_value > object[i].radius) {
      min_value = object[i].radius;
      min_num = i;
    }
  }
  return min_num;
}

// object[n].positionにある物体をtarget_posに移動する関数
void object_move(Object *object, int *n, Position target_pos) {
  int i;

  // 課題の達成状況(task_state)で場合分け
  switch (task_state) {
    case MEASURE:  // 物体の位置情報の取得
      for (i = 0; i < OBJECT_NUM; i++) {
        object[i].index = i + 1;
        object[i].position = get_object_position(i);
        object[i].radius = get_object_radius(i);
      }
      task_state = SEARCH;
      break;

    case SEARCH:
      *n = minimum_object_number(object);
      printf("\n------- Minimum Size Object -------\n");
      printf("index = %d: x = %6.2f, y = %6.2f, radius = %6.3f\n",
             object[*n].index, object[*n].position.x, object[*n].position.y,
             object[*n].radius);
      task_state = APPROACH;
      break;

    case APPROACH:  // 物体へ移動
      set_command_move_arm_to(object[*n].position);
      task_state = PICKUP;
      break;

    case PICKUP:  // 物体を把持
      set_command_pick_up_object();
      task_state = MOVE;
      break;

    case MOVE:  // 物体を移動
      set_command_move_arm_to(target_pos);
      task_state = PLACE;
      break;

    case PLACE:  // 物体を設置
      set_command_release_object();
      object[*n].position = target_pos;  // 物体位置を更新
      task_state = FINISH;
      break;

    case FINISH:
      break;
  }
}

// 物体情報の表示
void object_info_display(Object *object) {
  int i;
  printf("\n------- Object Information ---------\n");
  for (i = 0; i < OBJECT_NUM; i++) {
    printf("index = %2d, x = %6.2f, y = %6.2f, radius = %6.3f\n",
           object[i].index, object[i].position.x, object[i].position.y,
           object[i].radius);
  }
}

int main(void) {
  int i;
  int min_num;                       // 最小サイズの物体番号
  Position front_pos = {0.7, 0.0};  // 物体の移動先

  // 物体情報を格納する構造体
  Object object[OBJECT_NUM];

  // ロボットシミュレータ初期化
  bool is_initialized = initialize_robot(OBJECT_NUM);
  if (!is_initialized) {
    printf("System Error: \%s\n", get_system_error_str(get_system_error()));
    finalize_robot();
    return -1;
  }

  // ロボット駆動ループ
  while (update_robot()) {
    // 最小サイズの物体の移動
    if (get_arm_state() == ARM_STATE_STOP) {
      if (task_state != FINISH) {
        object_move(object, &min_num, front_pos);

      } else {
        object_info_display(object);
        break;
      }
    }
  }

  finalize_robot();
  return 0;
}
