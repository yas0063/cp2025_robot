#ifndef ROBOT_H
#define ROBOT_H

#include <stdbool.h>
#ifndef __APPLE__
#include <windef.h>
#endif

/* === Summary ===

enum SYSTEM_ERROR{								// システムのエラー状態．
	SYSTEM_ERROR_NO_ERROR,						//  正常．
	SYSTEM_ERROR_THE_INPUT_IS_OUT_OF_RANGE,		//  入力値が範囲外．
	SYSTEM_ERROR_FAILED_TO_INITIALIZE_DISPLAY,	//  モニタ初期化失敗．
	SYSTEM_ERROR_MAXIMUM_UPDATE_STEPS,			//  シミュレーション実行時間ステップ超過．
};

enum ARM_STATE{					// アームの状態遷移．
	ARM_STATE_STOP,				//  停止状態．
	ARM_STATE_MOVING,			//  移動状態．
	ARM_STATE_HANDLING_OBJECT,	//  グリッパ稼働状態．
};

enum ARM_ERROR{					// アームのエラー状態．
	ARM_ERROR_NO_ERROR,			//  正常．
	ARM_ERROR_OUT_OF_RANGE,		//  入力値がアームの可動範囲外．
	ARM_ERROR_GRIPPER_IS_BUSY,	//  グリッパが稼働中につき，アームが移動できない．
};

enum GRIPPER_STATE{				// グリッパの状態遷移．
	GRIPPER_STATE_EMPTY,		//  何も把持していない状態．
	GRIPPER_STATE_PICKING,		//  ピックアップ作業中状態．
	GRIPPER_STATE_HOLDING,		//  物体を把持している状態．
	GRIPPER_STATE_RELEASING,	//  リリース作業中状態．
};

enum GRIPPER_ERROR{						// グリッパのエラー状態．
	GRIPPER_ERROR_NO_ERROR,				//  正常．
	GRIPPER_ERROR_ARM_IS_BUSY,			//  アームが移動中につき，指令を実行できない．
	GRIPPER_ERROR_NOT_EMPTY,			//  グリッパが物体を把持しているので，ピックアップできない
	GRIPPER_ERROR_NOT_HOLDING,			//  グリッパが何も把持していないので，リリースできない
	GRIPPER_ERROR_FAILED_TO_PICK,		//  ピックアップ失敗．
	GRIPPER_ERROR_FAILED_TO_RELEASE,	//  物体が干渉するのでリリースできない．
};

struct Position_{
	double x;	// [m]
	double y;	// [m]
};

struct ColorRGB_{
	int r;	// 0 ~ 255
	int g;	// 0 ~ 255
	int b;	// 0 ~ 255
};

typedef enum SYSTEM_ERROR  SystemError;
typedef enum ARM_STATE     ArmState;
typedef enum ARM_ERROR     ArmError;
typedef enum GRIPPER_STATE GripperState;
typedef enum GRIPPER_ERROR GripperError;

typedef struct Position_ Position;
typedef struct ColorRGB_ ColorRGB;

bool initialize_robot( unsigned int object_num );	// 初期化
bool finalize_robot();								// 終了
bool update_robot();								// 更新

unsigned int get_update_robot_step();		// 実行時間ステップ
double       get_system_time_in_sec();		// 経過時間[s]
unsigned int get_object_num_max();			// 配置可能な物体の上限値
unsigned int get_object_num_min();			// 配置可能な物体の下限値
unsigned int get_update_robot_step_max();	// update_robot()の呼出し回数の上限値
unsigned int get_object_num();				// 物体の個数

SystemError  get_system_error();
ArmState     get_arm_state();
ArmError     get_arm_error();
GripperState get_gripper_state();
GripperError get_gripper_error();

const char* get_system_error_str  ( SystemError error );
const char* get_arm_state_str     ( ArmState status );
const char* get_arm_error_str     ( ArmError error );
const char* get_gripper_state_str ( GripperState status );
const char* get_gripper_error_str ( GripperError error );

Position get_object_position  ( unsigned int object_index );	// 物体の位置
ColorRGB get_object_color_rgb ( unsigned int object_index );	// 物体の色情報
double   get_object_radius    ( unsigned int object_index );	// 物体の半径[m]
double   get_held_object_weight();								// 把持物体の重量[g]
Position get_arm_position();									// アームの位置

bool set_command_move_arm_to( Position target_position );	// アーム移動指令
bool set_command_pick_up_object();							// ピックアップ作業指令
bool set_command_release_object();							// リリース作業指令

 */


/* ================================================================
   型定義 Type definition
   ================================================================ */

/*!
　システムのエラー状態．
　initialize_robot()とupdate_robot()の呼出し後に更新され，get_system_error()で取得できる．
 */
enum SYSTEM_ERROR{
	SYSTEM_ERROR_NO_ERROR,						//!< 正常．
	SYSTEM_ERROR_THE_INPUT_IS_OUT_OF_RANGE,		//!< 入力値が範囲外．
	SYSTEM_ERROR_FAILED_TO_INITIALIZE_DISPLAY,	//!< モニタ初期化失敗．
	SYSTEM_ERROR_MAXIMUM_UPDATE_STEPS,			//!< シミュレーション実行時間ステップ超過．
};

/*!
　アームの状態遷移．
　get_arm_state()で取得できる．
 */
enum ARM_STATE{
	/*!
	　停止状態．
	　この状態のときに，グリッパへの指令であるset_command_pick_up_object()と
	set_command_release_object()が正常に指令できる．
	 */
	ARM_STATE_STOP,
	
	/*!
	　移動状態．
	　アームへの指令であるset_command_move_arm_to()が指令されると，この状態になる．
	　グリッパへの指令であるset_command_pick_up_object()とset_command_release_object()は，この状態の
	ときには正常に指令できない．
	 */
	ARM_STATE_MOVING,
	
	/*!
	　グリッパ稼働状態．
	　グリッパへの指令であるset_command_pick_up_object()とset_command_release_object()が
	指令されると，アームはこの状態になる．
	　アームへの指令であるset_command_move_arm_to()は，この状態のときには正常に指令できない．
	 */
	ARM_STATE_HANDLING_OBJECT,
};

/*!
　アームのエラー状態．
　set_command_move_arm_to()の呼出し後に更新され，get_arm_error()で取得できる．
 */
enum ARM_ERROR{
	ARM_ERROR_NO_ERROR,			//!< 正常．
	ARM_ERROR_OUT_OF_RANGE,		//!< 入力値がアームの可動範囲外．
	ARM_ERROR_GRIPPER_IS_BUSY,	//!< グリッパが稼働中につき，アームが移動できない．
};

/*!
　グリッパの状態遷移．
　get_gripper_state()で取得できる．
 */
enum GRIPPER_STATE{
	/*!
	　何も把持していない状態．
	　ARM_STATE_STOPであれば，set_command_pick_up_object()を正常に指令できる．
	*/
	GRIPPER_STATE_EMPTY,
	
	/*!
	　ピックアップ作業中状態．
	　set_command_pick_up_object()が正常に指令できると，この状態に遷移する．
	　この状態である間は，グリッパが自動的に一定時間でピックアップ作業を行う．
	　ピックアップ作業は，アーム位置近傍に物体がある場合，必ず (for librobo.a) または、
	60％の確率で (for librobo2.a) 成功する．アーム位置近傍に
	物体がない場合は失敗する．成功し，グリッパが元の位置に戻ると，グリッパの状態は自動的に
	GRIPPER_STATE_HOLDINGに遷移する．失敗し，グリッパが元の位置に戻ると，グリッパの状態は自動的に
	GRIPPER_STATE_EMPTYに遷移する．
	*/
	GRIPPER_STATE_PICKING,
	
	/*!
	　物体を把持している状態．
	　set_command_pick_up_object()を正常に指令でき，ピックアップ作業が成功すると，
	グリッパの状態は自動的にこの状態に遷移する．
	　ARM_STATE_STOPであれば，set_command_release_object()を正常に指令できる．
	　get_held_object_weight()は，この状態かつARM_STATE_STOP状態のときのみ，
	把持物体の正確な重量を返す．
	*/
	GRIPPER_STATE_HOLDING,
	
	/*!
	　リリース作業中状態．
	　set_command_release_object()が正常に指令できると，この状態に遷移する．
	　この状態である間は，グリッパが自動的に一定時間でリリース作業を行う．
	　リリース作業は，リリースされる物体の位置が，地面上の他の物体と干渉しなければ、必ず
	(for librobo.a)、または、60%の確率で (for librobo2.a) 成功する．
	成功し，グリッパが元の位置に戻ると，グリッパの状態は自動的にGRIPPER_STATE_EMPTYに遷移する．
	失敗し，グリッパが元の位置に戻ると，グリッパの状態は自動的にGRIPPER_STATE_HOLDINGに遷移する．
	　リリース作業が成功した場合，その直前に把持されていた物体は，そのときのアームの位置に置かれる．
	*/
	GRIPPER_STATE_RELEASING,
};

/*!
　グリッパのエラー状態．
　set_command_pick_up_object()，set_command_release_object()の呼出し後，ピックアップ作業中，
もしくはリリース作業中に更新され，get_gripper_error()で取得できる．
 */
enum GRIPPER_ERROR{
	GRIPPER_ERROR_NO_ERROR,				//!< 正常．
	GRIPPER_ERROR_ARM_IS_BUSY,			//!< アームが移動中につき，指令を実行できない．
	GRIPPER_ERROR_NOT_EMPTY,			//!< グリッパが物体を把持しているので，ピックアップできない
	GRIPPER_ERROR_NOT_HOLDING,			//!< グリッパが何も把持していないので，リリースできない
	GRIPPER_ERROR_FAILED_TO_PICK,		//!< ピックアップ失敗．
	GRIPPER_ERROR_FAILED_TO_RELEASE,	//!< 物体が干渉するのでリリースできない．
};

typedef enum SYSTEM_ERROR   SystemError;
typedef enum ARM_STATE      ArmState;
typedef enum ARM_ERROR      ArmError;
typedef enum GRIPPER_STATE  GripperState;
typedef enum GRIPPER_ERROR  GripperError;

struct Position_{
	double x;	// [m]
	double y;	// [m]
};

struct ColorRGB_{
	int r;	// 0 ~ 255
	int g;	// 0 ~ 255
	int b;	// 0 ~ 255
};
typedef struct Position_ Position;
typedef struct ColorRGB_ ColorRGB;


/* ================================================================
   関数定義 Function definition
   ================================================================ */

/* ----------------------------------------------------------------
   ロボットおよびシミュレーション環境の設定 System settings
   ---------------------------------------------------------------- */

/*!
　ロボットおよびシミュレーション環境をを初期化する．
　引数は，環境中に配置される物体の数である．
　環境中に配置可能な物体数の上限値・下限値は，get_object_num_max()，get_object_num_min()で取得できる．

@retval true  : 初期化に成功．
@retval false : 初期化に失敗．
　どのような原因で失敗したかは，この関数呼出しの直後に，get_system_error()から取得できる．
　SYSTEM_ERROR_THE_INPUT_IS_OUT_OF_RANGE : 引数の値が既定範囲外．
　SYSTEM_ERROR_FAILED_TO_INITIALIZE_DISPLAY : モニタの初期化に失敗．
 */
bool initialize_robot( unsigned int object_num );

/*!
　ロボットおよびシミュレーション環境を終了する．
　返り値のチェックは，今回の課題では省略してもよいです．．．
*/
bool finalize_robot();

/*!
　ロボットおよびシミュレーション環境を更新する．
　これを繰り返し呼び出すことでシミュレーションを実行する．この関数呼出しは，
理想的には10[ms]置きとなる．つまり，この関数の実行周期は理想的には10[ms]となる．
　1回の関数呼出しにより，シミュレーションの実行時間ステップは1だけ増加する．get_update_robot_step()
で現在の実行時間ステップを取得できる．

@return 更新処理の成否
@retval true  : 成功．
@retval false : 失敗．初期化せずにこの関数を呼び出した場合や，この関数の呼出し回数が上限を超えた
場合に，falseが返る．この関数の呼出し回数の上限値はget_update_robot_step_max()で得られる．
 */
bool update_robot();

/*!
　シミュレーションの実行時間ステップを取得する．1ステップ＝10[ms]．
 */
unsigned int get_update_robot_step();

/*!
　シミュレーションの経過時間を秒単位で取得する．
　この経過時間は実時間ではなく，get_update_robot_step() * 0.01 [s] である．
 */
double get_system_time_in_sec();

/*!
　シミュレーション環境中に配置可能な物体の上限値を返す．
 */
unsigned int get_object_num_max();

/*!
　シミュレーション環境中に配置可能な物体の下限値を返す．
 */
unsigned int get_object_num_min();

/*!
　update_robot()の呼出し回数の上限値を返す．
 */
unsigned int get_update_robot_step_max();

/*!
　initialize_robot()後にこれを呼び出すと，シミュレーション環境中の物体の個数を返す．
initialize_robot()がtrueを返したのなら，この値はinitialize_robot()の引数の値と同じになる．
 */
unsigned int get_object_num();


/* ----------------------------------------------------------------
   エラー状態およびロボット状態の取得
   Getting error status & robot status
   ---------------------------------------------------------------- */

SystemError     get_system_error();
ArmState        get_arm_state();
ArmError        get_arm_error();
GripperState    get_gripper_state();
GripperError    get_gripper_error();

// エラーや状態の文字列化
const char* get_system_error_str  ( SystemError error );
const char* get_arm_state_str     ( ArmState status );
const char* get_arm_error_str     ( ArmError error );
const char* get_gripper_state_str ( GripperState status );
const char* get_gripper_error_str ( GripperError error );


/* ----------------------------------------------------------------
   物体情報およびアーム位置の取得
   Getting object information & arm position
   ---------------------------------------------------------------- */

/*!
　環境中に置かれた物体の位置を返す．
　シミュレーションにおいて物体の位置観測は，update_robot()の呼び出し毎に行われ，システム内部において
この結果を保持している．
　引数であるobject_indexには，0以上get_object_num()未満の値を指定する．この範囲外の引数が与えられた
とき，この関数はでたらめな値を返す．
　システムはinitialize_robot()の実行後に，シミュレーション環境中の物体の位置を観測し，システム内部
の物体情報の配列に書き込む．この関数の引数は，
この配列におけるインデックスを意味する．システム内部の物体情報の配列は，位置観測がupdate_robot()
呼出し毎に行われるのに関わらず，何らかの画像処理技術により，変わることがない，としている．つまり，
物体の位置移動操作をしても，get_object_position()，get_object_color_rgb()，get_object_radius()で
取得できる物体情報は一貫性を持つ．もっと言えば，update_robot()後も，物体についての同一性を諸君らが
気にする必要はない．
　シミュレーション環境中に置かれた全ての物体の中心はは，X，Yともに±1.0[m]の正方形平面内に配置
される．
 */
Position get_object_position( unsigned int object_index );

/*!
　物体の色情報を返す．
 */
ColorRGB get_object_color_rgb( unsigned int object_index );

/*!
　物体の半径[m]を返す．
 */
double get_object_radius( unsigned int object_index );

/*!
　GRIPPER_STATE_HOLDINGのときのみ，把持物体の重量[g]を返す．
　それ以外のときは，でたらめな値を返す．
 */
double get_held_object_weight();

/*!
　アームの位置を返す．
 */
Position get_arm_position();


/* ----------------------------------------------------------------
   アームおよびグリッパへの指令
   Commands to arm & gripper
   ---------------------------------------------------------------- */

/*!
　アームを指定位置まで移動するよう指令する．
　引数は絶対座標系である．アーム位置からの相対座標ではない．
　ARM_STATE_HANDLING_OBJECTのとき，もしくは，引数がアームの可動範囲外であるとき，falseを返す．
ARM_ERROR_GRIPPER_IS_BUSY，ARM_ERROR_OUT_OF_RANGEがアームのエラー状態にセットされる．
　ARM_STATE_STOPのときに指令すると，引数が可動範囲内であれば，ARM_STATE_MOVINGに状態遷移して，
移動距離に応じた時間で，引数の位置まで移動する．引数の位置の近傍に到達すると，自動的にARM_STATE_STOP
に状態遷移する．
　ARM_STATE_MOVINGのときに指令すると，引数が可動範囲内であれば，同様に移動する．
　アームの可動範囲はX，Yともに±1.0[m]内の正方形平面内である．
 */
bool set_command_move_arm_to( Position target_position );

/*!
　アームの現在位置において，ピックアップ作業を行うよう指令する．
　ARM_STATE_STOPでないとき，もしくは，GRIPPER_STATE_EMPTYでないとき，falseを返す．このとき，
GRIPPER_ERROR_ARM_IS_BUSY，GRIPPER_ERROR_NOT_EMPTYがグリッパのエラー状態にセットされる．
　後は，GRIPPER_STATE_PICKINGの説明を参照すること．
 */
bool set_command_pick_up_object();

/*!
　アームの現在位置において，リリース作業を行うよう指令する．
　ARM_STATE_STOPでないとき，もしくは，GRIPPER_STATE_HOLDINGでないとき，falseを返す．このとき，
GRIPPER_ERROR_ARM_IS_BUSY，GRIPPER_ERROR_NOT_HOLDINGがグリッパのエラー状態にセットされる．
　後は，GRIPPER_STATE_RELEASINGの説明を参照すること．
 */
bool set_command_release_object();

#endif	// End of ROBOT_H
