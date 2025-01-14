/**
 * @file LineTraceAreaJr.cpp
 * @brief ライントレースエリアを攻略するクラスのテスト用
 * @author 
 */

#include "LineTraceAreaJr.h"

// Lコースの情報を初期化する
const std::array<SectionParam, LineTraceAreaJr::LEFT_SECTION_SIZE> LineTraceAreaJr::LEFT_COURSE_INFO
    = { SectionParam{ 300, 23, 60, PidGain(1.2, 0, 0) },    //直線
        SectionParam{ 1050, 23, 70, PidGain(1.9, 0, 0) },   //1st カーブ
        SectionParam{ 1180, 23, 70, PidGain(1.85, 0, 0) },  //Cross前 カーブ
        SectionParam{ 470, 23, 100, PidGain(0, 0, 0) },     //1st Cross 直線
        SectionParam{ 260, 23, 40, PidGain(1.55, 0, 0) },   //Cross後 調整
        SectionParam{ 2300, 23, 80, PidGain(1.75, 0, 0) },
        SectionParam{ 500, 23, 100, PidGain(0, 0, 0) },     //2nd Cross
        SectionParam{ 720, 23, 40, PidGain(1.45, 0, 0) },
        SectionParam{ 1500, 23, 60, PidGain(2.05, 0, 0) },  //GATE4 前のカーブ
        SectionParam{ 1600, 23, 100, PidGain(1.75, 0, 0) }, //直線
        SectionParam{ 700, 23, 60, PidGain(2.05, 0, 0) },
        SectionParam{ 950, 23, 100, PidGain(1.95, 0, 0) }  //Last 直線
        };

// Rコースの情報を初期化する
const std::array<SectionParam, LineTraceAreaJr::RIGHT_SECTION_SIZE> LineTraceAreaJr::RIGHT_COURSE_INFO
    = { SectionParam{ 150, 12, 60, PidGain(0.2, 0.1, 0.1) },
        SectionParam{ 1445, 12, 100, PidGain(3.5, 1, 1) },
        SectionParam{ 670, 12, 80, PidGain(1.7, 0.8, 0.8) },
        SectionParam{ 600, 12, 100, PidGain(3.3, 1, 1) },
        SectionParam{ 580, 12, 80, PidGain(1.6, 0.67, 0.6) },
        SectionParam{ 1450, 12, 100, PidGain(4, 0.3, 0.3) },
        SectionParam{ 400, 12, 60, PidGain(6, 2, 0.5) },
        SectionParam{ 700, 12, 100, PidGain(3, 1, 1.8) } };

void LineTraceAreaJr::runLineTraceAreaJr()
{
  const SectionParam* param;
  bool isLeftEdge;  // true:左エッジ,false:右エッジ

  if(IS_LEFT_COURSE) {
    // Lコースの場合
    param = LEFT_COURSE_INFO.begin();
    //runLineTraceAreaJrShortcut();
    //エッジの設定
    isLeftEdge = !IS_LEFT_COURSE;
    // LineTracerにエッジを与えてインスタンス化する
    LineTracer lineTracer(isLeftEdge);
    // LRに応じて各区間を順番に走らせる
    for(int section = 0; section < (IS_LEFT_COURSE ? LEFT_SECTION_SIZE : RIGHT_SECTION_SIZE); section++) 
    {
      // Linetracerクラスのrun関数に区間の情報を渡して走行させる
      lineTracer.run(param[section].sectionDistance, param[section].sectionTargetBrightness,
                      param[section].sectionPwm, param[section].sectionPidGain);
    }
  } else {
    // Rコースの場合
    param = RIGHT_COURSE_INFO.begin();
    //エッジの設定
    isLeftEdge = IS_LEFT_COURSE;
    // LineTracerにエッジを与えてインスタンス化する
    LineTracer lineTracer(isLeftEdge);
    // LRに応じて各区間を順番に走らせる
    for(int section = 0; section < (IS_LEFT_COURSE ? LEFT_SECTION_SIZE : RIGHT_SECTION_SIZE);section++) 
    {
      // Linetracerクラスのrun関数に区間の情報を渡して走行させる
      lineTracer.run(param[section].sectionDistance, param[section].sectionTargetBrightness,
                      param[section].sectionPwm, param[section].sectionPidGain);
    }
  }

  
}

void LineTraceAreaJr::runLineTraceAreaJrShortcut()
{
  const SectionParam* param;
  bool isLeftEdge;  // true:左エッジ,false:右エッジ

  //エッジの設定
  isLeftEdge = !IS_LEFT_COURSE;

  // LineTracerにエッジを与えてインスタンス化する
  LineTracer lineTracer(isLeftEdge);
  StraightRunner straightRunner;
  Measurer measurer;
  Controller controller;

  int targetBrightness = 12;
  int initialDistance = 0;
  int currentDistance = 0;

  int curveDistance1 = 777;
  int straightDistance1 = 440;
  int curveDistance2 = 777;
  int straightDistance2 = 1114;
  int curveDistance3 = 700;
  int straightDistance3 = 460;

  lineTracer.run(150, targetBrightness, 60, PidGain(0.3, 0.01, 0.01));
  lineTracer.run(1415, targetBrightness, 100, PidGain(3.512, 0.102, 0.221));

  //第一カーブ
  initialDistance = Mileage::calculateMileage(measurer.getRightCount(), measurer.getLeftCount());
  while(true) {
    currentDistance = Mileage::calculateMileage(measurer.getRightCount(), measurer.getLeftCount());
    if(currentDistance - initialDistance >= curveDistance1) {
      break;
    }
    controller.setRightMotorPwm(99);
    controller.setLeftMotorPwm(65);
    controller.sleep();
  }

  straightRunner.runStraightToDistance(straightDistance1, 100);

  //第二カーブ
  initialDistance = Mileage::calculateMileage(measurer.getRightCount(), measurer.getLeftCount());
  while(true) {
    currentDistance = Mileage::calculateMileage(measurer.getRightCount(), measurer.getLeftCount());
    if(currentDistance - initialDistance >= curveDistance2) {
      break;
    }
    controller.setRightMotorPwm(99);
    controller.setLeftMotorPwm(66);
    controller.sleep();
  }

  lineTracer.run(straightDistance2, targetBrightness, 100, PidGain(3, 1.21, 1));

  //第三カーブ
  initialDistance = Mileage::calculateMileage(measurer.getRightCount(), measurer.getLeftCount());
  while(true) {
    currentDistance = Mileage::calculateMileage(measurer.getRightCount(), measurer.getLeftCount());
    if(currentDistance - initialDistance >= curveDistance3) {
      break;
    }
    controller.setRightMotorPwm(63);
    controller.setLeftMotorPwm(99);
    controller.sleep();
  }

  //外れた時、黒線に乗るまで左に旋回
  int count = 0;
  initialDistance = Mileage::calculateMileage(measurer.getRightCount(), measurer.getLeftCount());
  COLOR color = ColorJudge::getColor(measurer.getRawColor());
  if(color == COLOR::WHITE) {
    while(true) {
      COLOR color = ColorJudge::getColor(measurer.getRawColor());
      if(color == COLOR::BLACK) {
        break;
      }
      controller.setRightMotorPwm(90);
      controller.setLeftMotorPwm(80);
      controller.sleep();
    }
  }
  //外れた時、黒線に乗るまでの走行距離
  count = Mileage::calculateMileage(measurer.getRightCount(), measurer.getLeftCount())
          - initialDistance;

  lineTracer.run(straightDistance3 - count, targetBrightness, 100, PidGain(5.5, 1, 1.5));
}