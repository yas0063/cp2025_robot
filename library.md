# ロボットライブラリ

##  本演習で想定するロボットシミュレータ

- グリッパ（ロボットの手）により物体を掴んでその位置を移動させるガントリー（直行）型ロボット
- 物体把持に失敗することもある．
- 物体の位置・形状・色を読み取る視覚センサ
- 把持中の物体の重さを計測する重量センサ
- 基本動作・センシングのライブラリ


## 準備

### サンプルプログラム
[サンプルプログラム(robot_sample.zip)](src/robot_sample.zip)をダウンロードしてください．

これには

- main1.c

- robot_simulator.h

- librobo.a

- librobo2.a

が含まれています．

## コンパイルと実行

メインのファイルmain1.c，ヘッダーファイル（robot_simulator.h），ライブラリ（librobo.a）を同じフォルダにおき，そのディレクトリーにて，
```
gcc main1.c -o main1 -L. -lrobo -lm -lpthread -lfreeglut -lopengl32 -lglu32
```
でコンパイルします．


そして，
```
 ./main1.exe
```
で実行します．

キー入力で簡単な表示の操作ができます．
```
a：左，d：右，w：拡大，s：縮小，q：終了
```



## コンパイルオプションとライブラリー

- コンパイルコマンドの"-l{ライブラリ名}"は，プログラム中で使用するライブラリを指定しています．例えば，mは算術演算ライブラリ(libm.aあるいはlibm.so)で，GL，glut，GLUは3Dグラフィックのライブラリ（libfreeglut.a, libopengl32.a, libglu32.aあるいはibfreeglut.so, libopengl32.so, libglu32.so）です．pthreadは，スレッドプログラミングをするときに使用するライブラリ(libpthread.a)です．roboライブラリ（librobo.a）は独自に開発したロボットライブラリです
指定するときは，libと.a（か.so）を取ったものを，"-l"の後に書きます．例えば，libm.aなら，"-lm"，libGL.aなら，"-lGL"になります．

- openglが，[OpenGL](https://www.opengl.org/)のコアとなるライブライリー，OpenGLライブラリをより簡単に使えるようにしたものがGLU（OpenGL Utility Library），さらに拡張したものがGLUT（OpenGL Utility Toolkit）です．

- ライブラリの".a"と".so“の違いは，静的リンク用のライブラリ（".a"）か動的リンク用のライブラリ（".so"）の違いです．
静的リンクは，プログラムの実行に必要なライブラリやモジュールなどを，プログラム本体にコンパイル時に連結（リンク）されて，使用されます．一方，動的リンクは，プログラム本体に連結されるのではなく，プログラム実行時に（動的に）呼び出すという形で使われます．

- "-L." は同じフォルダ内にライブラリがあることを示します．コンパイラは，デフォルトで設定されているフォルダーや環境変数 LD_LIBRARY_PATHで指定されたフォルダーにて，ライブラリーを検索します．それ以外の場所におかれたライブラリを使う場合は，"-L"にて明示的に，そのありかを指定する必要があります．今回は，librobo.aがプログラム（ソースコード）と一緒のフォルダに置かれているため，同じフォルダを意味する"."を使って， "-L."と指定する必要があります．


- roboライブラリ（librobo.a）は，ロボットの駆動やセンシングに関する関数や構造体を定義しています．なお，このライブラリ自体を変更することはできません．ソースコードがあればライブラリの変更が可能ですが，この授業の範囲外とします．

- ライブラリの構築については，四年生の授業「アドバンスドプログラミング演習（C言語）」で扱います．）

- librobo2.aは第2回の課題で使います．


## ヘッダファイル

ロボットライブラリの関数と構造体，列挙型はヘッダファイルrobot_simulator.hで説明されています．
  
例えば，位置を表すPosition構造体にはxとyの座標が定義されています．

```
 struct Position_{
     double x;
     double y;
 };
 typedef struct Position_ Position;
```

また，色を表すColorRGB構造体にはr（赤），g（緑），b（青）の各色の変数が定義されています．

```
 struct ColorRGB_{
      int r;
      int g;
      int b;
 };
 typedef struct ColorRGB_ ColorRGB;
```

例えば，関数
```
 bool set_command_move_arm_to( Position target_position );
```
へPosition構造体を渡すことで，アームがその座標まで移動します．bool型（論理型）関数はtrue（真）かfalse（偽）を返します．
  
この関数の場合，ARM_STATE_HANDLING_OBJECTのとき，あるいは，指定した座標が可動範囲外であるときにfalseを返し，それ以外だとtrueを返します．このARM_STATE_HANDLING_OBJECTはenum型（列挙型）で定義されています．
```
 enum ARM_STATE{
      ARM_STATE_STOP,
      ARM_STATE_MOVING,
      ARM_STATE_HANDLING_OBJECT,
 };
 typedef enum ARM_STATE ArmState;
```

列挙型は一般に，選択肢を表す整数の定数を定義するときに用いられます．
```
 enum タグ名{
      列挙変数1,
      列挙変数2,
      列挙変数3, ...
 };
```
で定義すると，列挙変数には自動的に先頭から0, 1, 2, ...と整数が割り当てられます．Arm_State列挙型はアームの状態を表し，各列挙変数はそれぞれ，アームが止まっている，動いている，物体を把持してるを意味しています．

今アームがどの状態にあるかは，関数
```
 ArmState get_arm_state();
```
で取得できます．この関数はアームの状態に応じたArmState型を返します．すなわち，ARM_STATE_STOP，ARM_STATE_MOVING，および，ARM_STATE_HANDLING_OBJECTのいずれかを返します．この関数がARM_STATE_HANDLING_OBJECT（つまり，2）を返せば，アームが物体を把持していることになります．

set_command_move_arm_to関数では，このアーム状態の情報を用いて，返り値を決めています．ただし，上記の処理はライブラリのソースコードに書かれており，ライブラリユーザには見えません．

このヘッダファイルには，この他にも課題の遂行に必要な関数・列挙型が説明されています．プログラミングを始める前に，各自で読んでおいてください．

## 画面表示について

VScode上のターミナルで，プログラムを実行した際，シミュレータがVScodeの後ろに表示されてしまい，結果がわかりづらくなってしまう場合があります．

Windowsだけですが，シミュレータの画面を最前面に表示するよう修正したライブラリーを準備しました．

- [librobo_w.a](src/librobo_w.a)
- [librobo2_w.a](src/librobo2_w.a)

これらをダウンロードし，librobo.a, librobo2.aとリネームして，それまで使っていた（robot_sample.zipに含まれていた）librobo.a, librobo2.aと差し替えて使用して下さい．


