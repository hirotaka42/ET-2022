/**
 * @file EtRobocon2022.cpp
 * @brief 全体を制御するクラス
 * @author Takahiro55555
 */

#include "EtRobocon2022.h"
void EtRobocon2022::start()
{
  Controller controller;
  Measurer measurer;

  //タッチセンサが押されるまで待機
  while(!measurer.isPressed()) {
    controller.sleep();
  }

  //シミュレータへ競技の終了を通知する
  controller.notifyCompletedToSimulator();
}
