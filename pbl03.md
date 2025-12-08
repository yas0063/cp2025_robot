# 第 3 回 物体を並べ替えよう

第 3 回では，物体を把持し (PICKUP)，目標位置までアームを移動させ，物体を設置 (PLACE)という動作を繰り返すことで，初期状態ではランダムに配置されている物体を並び替えることをやってみます．

なお，各物体の初期状態は`(-0.5, -0.5)`，`(-0.5, 0.5)`，`(0.5, 0.5)`，`(0.5, -0.5)`で囲まれる領域から，ランダムに選ばれています．

## 最小半径の物体の決定

[サンプルプログラム(main3.c)](src/main3.c)をダウンロードしてください．

ダウンロードしたmain3.cを，librobo.a のあるフォルダにコピーして，コンパイル・実行してください．

```
 gcc main3.c -o main3 -L. -lrobo -lm -lpthread -lfreeglut -lopengl32 -lglu32
```

このプログラムでは，アームが最小半径の物体を `(0.7, 0.0)` に移動させます．

今回，二つの新しい関数を作成しています．

```
 int minimum_object_number(Object *object);
```

この関数は，センシングした物体情報をもとに最小半径の物体番号を返します．

```
 void object_move(Object *object, int *n, Position target_pos);
```

この関数は object[n].position にある物体を target_pos へ移動させます．

## 最小半径の物体の移動

今回の object_move 関数における状態の場合分けは，

```
 typedef enum{
  MEASURE,
  SEARCH,
  APPROACH,
  PICKUP,
  MOVE,
  PLACE,
  FINISH
 } TaskState;
```

です．

この関数ではまず，位置を計測し，minimum_object_number 関数を用いて最小半径の物体番号 n を探します (SEARCH)．そして，その物体までアームを移動させ (APPROACH)，物体を把持し (PICKUP)，目標位置 target_position までアームを移動させ (MOVE)，物体を設置します (PLACE)．

メイン関数でこの関数を使うときは，配列名 object（先頭要素のアドレス）を渡します．

```
 object_move(object, &min_num, front_pos);
```

こうすることで，この関数内で object[i].position の値を更新することができます．また，移動させる物体番号 min_num もアドレスとして渡すことで，while ループで状態遷移をしたとしてもその数値を維持できます．

前回の approach_and_weigh 関数とは異なり，物体の把持後に，関数

```
 set_command_move_arm_to(target_pos);
```

でその物体を target_pos まで運びます．

そして，PLACE では，関数

```
 set_command_release_object();
```

で物体を設置しています．

また，

```
 object[*n].position = target_pos;
```

で，移動させた物体の位置を更新します．

最終的に，状態が FINISH になり，main 関数で物体情報を表示し，while ループを break します．

## 練習問題

### 問題

5 個すべての物体を上部の領域 `(y = 0.7)` に横一列に左から右に向かって，半径の小さい順に並べるプログラムを作成してください．

### ヒント

物体情報配列 object を半径でソートしましょう．x座標を変化させて並べます．

## 課題

5 個すべての物体を重量の**大きい**順に並べるプログラムを作成してください．まず，すべての物体の重量を計測し，その後，重量の大きい順に，横一列 `(y = 0.7)` に左から右に向かって物体を整列してください．

### ヒント

重量計測か整列かの課題達成状況で，場合分けしましょう．

#### update_robot()関数の使用について

update_robot()は，main関数内でのみ使用してください．自作関数内で呼び出すことは禁止します．ただし，main関数内では，複数のwhileループに分けて使用してもかまいません．

良い例：main関数内で複数のwhileループを使用
```
while(update_robot()) {
    // 重量計測処理
}
while(update_robot()) {
    // 物体配置処理
}
```

悪い例：自作関数内でupdate_robot()を使用
```
void my_function() {
    while(update_robot()) {
        // 処理
    }
}
```

Note: 適宜コメントを入れたり，関数化したりして，他人にもわかりやすいプログラムになるように心がけてください．全くコメントのないものは減点します．

Attention: 他人のプログラムを写したことが判明した場合は，両者とも 0 点になります．課題内容に対応していないプログラムにも点をつけません．

#### 提出物

- 作成したプログラムファイル（ソースコード）．

#### 提出先

CLE 上のレポート box

#### 提出日

締め切りは，一週間後．

- 1/29 13:30


