#ifndef ESTIMATOR_H
#define ESTIMATOR_H
#include <pose_server/KrakenPose.h>
#include <boost/circular_buffer.hpp>
#include <cmath>
#include <kraken_msgs/imuData.h>
#include <kraken_msgs/depthData.h>
#include <kraken_msgs/dvlData.h>

namespace kraken_core
{
  class Estimator
  {
    private :
      /*
       * transform matrix
       */
      float _body_to_world_matrix[3][3];
    
    public:
      virtual ~Estimator(){}
      /*
       * Get the estimated pose to be published
       */
      inline KrakenPose& getNextPose()
      {
        return _next_pose_world;_prev_states_world[0];
      }
      /*
       *
       */
      inline float getTime()
      {
        return _time;
      }

      /*
       * Have to overwrite this function to according to estimation algorithm
       */
      virtual void updatePose(kraken_msgs::imuData &)=0;
      virtual void updatePose(kraken_msgs::imuData &,kraken_msgs::depthData&)=0;
      virtual void updatePose(kraken_msgs::imuData &,kraken_msgs::depthData&,kraken_msgs::dvlData&)=0;
      /*
       * To reset the vehicle coordinates
       */
      virtual void resetPose(KrakenPose &)=0;
      
    protected:
      /*
       * size is size of the queue
       */
      Estimator(int size=2,float time=0.05);
      /*
       * To update the transformation matrix to get global coordinate
       */
      void updateTransformMatrix();
      /*
       * Trasforms the verlocity in world frame
       */
      void transformToWorld();
      /*
       * To store the previous states with respect to body
       * May be used in estimation for better filters
       */
      boost::circular_buffer<KrakenPose> _prev_states_body;
      /*
       * To store the previous states with respect to world.
       * May be used in estimation for better filters
       */
      boost::circular_buffer<KrakenPose> _prev_states_world;
      /*
       * Next calculated pose with respect to body
       */
      KrakenPose _next_pose_body;
      /*
       * Next calculated pose with respect to world
       */
      KrakenPose _next_pose_world;
      /*
       * Update time
       */
      const float _time;
      
  };
  
} // end kraken_core

#endif //ESTIMATOR_H
