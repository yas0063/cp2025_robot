// 第四回 PBL
// サンプルプログラム

#include <stdio.h>

#include "robot_simulator.h"
#define OBJECT_NUM 7

typedef struct {
  int index;
  Position position;
  ColorRGB color;
  double radius;
  double weight;
} Object;

typedef enum {
  MEASURE,
  SORT,
  APPROACH,
  PICKUP,
  MOVE,
  PLACE,
  FINISH,
} TaskState;
TaskState task_state = MEASURE;

// 色のR値の小さい順に物体番号をバブルソート
void color_r_bubble_sort(Object *object) {
  int i, j;
  Object temp;

  for (i = 0; i < OBJECT_NUM; i++) {
    for (j = OBJECT_NUM - 1; j > i; j--) {
      if (object[j - 1].color.r > object[j].color.r) {
        temp = object[j];
        object[j] = object[j - 1];
        object[j - 1] = temp;
      }
    }
  }
  return;
}

// object[n].positionにある物体をtarget_posに移動する関数
void object_move(Object *object, int n, Position target_pos) {
  int i;

  // 課題の達成状況(task_state)で場合分け
  switch (task_state) {
    case MEASURE:  // 物体の位置と色情報の取得
      for (i = 0; i < OBJECT_NUM; i++) {
        object[i].index = i + 1;
        object[i].position = get_object_position(i);
        object[i].color = get_object_color_rgb(i);
        object[i].radius = get_object_radius(i);
      }
      task_state = SORT;
      break;

    case SORT:
      color_r_bubble_sort(object);
      task_state = APPROACH;

    case APPROACH:  // 物体へ移動
      set_command_move_arm_to(object[n].position);
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
      object[n].position = target_pos;  // 物体位置の更新
      task_state = FINISH;
      break;

    case FINISH:
      break;
  }
}

// 物体情報の表示
void object_info_display(Object *object) {
  int i;
  printf("\n------- Sorted Object Information ---------\n");
  for (i = 0; i < OBJECT_NUM; i++) {
    printf("index = %2d, x = %6.2f, y = %6.2f, R = %3d, G = %3d, B = %3d\n",
           object[i].index, object[i].position.x, object[i].position.y,
           object[i].color.r, object[i].color.g, object[i].color.b);
  }
}

int main(void) {
  int i;
  int n = 0;  // 何個目の物体を扱っているかを示す変数
  Position target_pos = {-0.6, 0.0};  // 物体の移動先（中央に横並び）

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
    if (get_arm_state() == ARM_STATE_STOP) {
      // 物体の整列
      if (n < OBJECT_NUM) {
        if (task_state != FINISH) {
          object_move(object, n, target_pos);
        } else {
          // 物体の設置に完了したら次の物体へ
          target_pos.x += object[n].radius + object[n + 1].radius + 0.05;
          task_state = APPROACH;
          n++;
        }

      } else {
        object_info_display(object);
        break;
      }
    }
  }

  finalize_robot();
  return 0;
}
