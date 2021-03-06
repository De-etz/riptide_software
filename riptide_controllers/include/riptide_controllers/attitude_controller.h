#ifndef ATTITUDE_CONTROLLER_H
#define ATTITUDE_CONTROLLER_H

#include "ros/ros.h"
#include "control_toolbox/pid.h"
#include "geometry_msgs/Accel.h"
#include "geometry_msgs/Vector3.h"
#include "riptide_msgs/AttitudeCommand.h"
#include "riptide_msgs/Imu.h"
#include "riptide_msgs/ResetControls.h"
#include "riptide_msgs/ControlStatus.h"
#include "riptide_msgs/ControlStatusAngular.h"
using namespace std;

class AttitudeController
{
  private:
    // Comms
    ros::NodeHandle nh;
    ros::Subscriber imu_sub, cmd_sub, reset_sub;
    ros::Publisher cmd_pub, status_pub;

    control_toolbox::Pid roll_controller_pid;
    control_toolbox::Pid pitch_controller_pid;
    control_toolbox::Pid yaw_controller_pid;

    geometry_msgs::Vector3 ang_accel_cmd;
    riptide_msgs::ControlStatusAngular status_msg;
    double MAX_ROLL_ERROR, MAX_PITCH_ERROR, MAX_YAW_ERROR;
    double MAX_ROLL_LIMIT, MAX_PITCH_LIMIT;

    // IIR Filter variables for error_dot
    double PID_IIR_LPF_bandwidth, dt_iir, alpha, imu_filter_rate;

    //PID
    double roll_error, pitch_error, yaw_error;
    double roll_error_dot, pitch_error_dot, yaw_error_dot;
    double roll_cmd, pitch_cmd, yaw_cmd, last_roll_cmd, last_pitch_cmd, last_yaw_cmd;

    geometry_msgs::Vector3 current_attitude, ang_vel, last_error, last_error_dot;

    bool pid_attitude_reset, pid_attitude_active;
    bool pid_roll_reset, pid_pitch_reset, pid_yaw_reset;
    bool pid_roll_active, pid_pitch_active, pid_yaw_active;

    ros::Time sample_start;
    ros::Duration sample_duration;
    double dt;

    void InitMsgs();
    void UpdateError();
    double Constrain(double current, double max);
    double SmoothErrorIIR(double input, double prev);
    void ResetRoll(int id);
    void ResetPitch(int id);
    void ResetYaw(int id);

  public:
    AttitudeController();
    template <typename T>
    void LoadParam(string param, T &var);
    void CommandCB(const riptide_msgs::AttitudeCommand::ConstPtr &cmd);
    void ImuCB(const riptide_msgs::Imu::ConstPtr &imu_msg);
    void ResetCB(const riptide_msgs::ResetControls::ConstPtr& reset_msg);
 };

 #endif
