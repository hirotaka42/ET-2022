/**
 * @file EtRobocon2022.cpp
 * @brief 全体を制御するクラス
 * @author 
 */

#include "EtRobocon2022.h"

 /**
  * シミュレータかどうかの定数を定義します
  * // 1:SIM, 0:実機
  */
 #if defined(MAKE_SIM)
     static const int _SIM = 1;
 #elif defined(MAKE_EV3)
     static const int _SIM = 0;
 #else
    static const int _SIM = 0;
 #endif

void EtRobocon2022::start()
{
  Controller controller;
  Measurer measurer;
  //controller.sleep(10*10);
  msg_f("waiting... stage 1", 2);

  controller.setColorLed(); /* 初期化完了通知 */
  msg_f("Go to the start, ready?",2);
  controller.sleep(10*10);

  if (_SIM)   msg_f("Hit SPACE bar to start",2);
  else        msg_f("Tap Touch Sensor to start",2);

  //タッチセンサが押されるまで待機
  while(!measurer.isPressed()) {
    controller.sleep(10*10);
  }

  //ライントレースエリア攻略開始
  msg_f("starting LineTraceAreaJr!! ", 2);
  LineTraceAreaJr::runLineTraceAreaJr();
  msg_f("end LineTraceAreaJr!! ", 2);

  //シミュレータへ競技の終了を通知する
  if(_SIM == 1) controller.notifyCompletedToSimulator();
}
