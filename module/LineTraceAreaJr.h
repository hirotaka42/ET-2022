/**
 * @file LineTraceAreaJr.h
 * @brief ライントレースエリアを攻略するクラス
 * @author Hisataka-Hagiyama,uchyam
 */

#ifndef LINETRACEAREAJr_H
#define LINETRACEAREAJr_H
#include "array"
#include "LineTracer.h"
#include "Pid.h"

#include "StraightRunner.h"
#include "Mileage.h"
#include "Measurer.h"
#include "Controller.h"

/**
 * Lコース/Rコース向けの設定を定義
 * デフォルトはLコース
 */
#if defined(MAKE_RIGHT)
static constexpr bool IS_LEFT_COURSE = false;  // Rコース
#else
static constexpr bool IS_LEFT_COURSE = true;  // Lコース
#endif

//区間の情報を保持する構造体
struct SectionParam {
  double sectionDistance;       //区間の走行距離
  int sectionTargetBrightness;  //区間の目標輝度
  int sectionPwm;               //区間のPWM値
  PidGain sectionPidGain;       //区間のPIDゲイン
};

class LineTraceAreaJr {
 public:
  /**
   *@fn static void runLineTraceAreaJr();
   *@brief ライントレースエリアを走行する
   */
  static void runLineTraceAreaJr();

  /**
   *@fn static void runLineTraceAreaJrShortcut();
   *@brief ライントレースエリアをショートカットしながら走行する
   */
  static void runLineTraceAreaJrShortcut();

 private:
  static const int LEFT_SECTION_SIZE = 8;   // Lコースの区間の数
  static const int RIGHT_SECTION_SIZE = 8;  // Rコースの区間の数
  static const std::array<SectionParam, LEFT_SECTION_SIZE> LEFT_COURSE_INFO;  // Lコースの情報
  static const std::array<SectionParam, RIGHT_SECTION_SIZE> RIGHT_COURSE_INFO;  // Rコースの情報

  /**
   * コンストラクタ
   * インスタンス化を禁止するためにpriveteにし、処理については記述していない
   */
  LineTraceAreaJr();
};

#endif
