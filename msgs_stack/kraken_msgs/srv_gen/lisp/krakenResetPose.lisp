; Auto-generated. Do not edit!


(cl:in-package kraken_msgs-srv)


;//! \htmlinclude krakenResetPose-request.msg.html

(cl:defclass <krakenResetPose-request> (roslisp-msg-protocol:ros-message)
  ((pose
    :reader pose
    :initarg :pose
    :type kraken_msgs-msg:krakenPose
    :initform (cl:make-instance 'kraken_msgs-msg:krakenPose)))
)

(cl:defclass krakenResetPose-request (<krakenResetPose-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <krakenResetPose-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'krakenResetPose-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kraken_msgs-srv:<krakenResetPose-request> is deprecated: use kraken_msgs-srv:krakenResetPose-request instead.")))

(cl:ensure-generic-function 'pose-val :lambda-list '(m))
(cl:defmethod pose-val ((m <krakenResetPose-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-srv:pose-val is deprecated.  Use kraken_msgs-srv:pose instead.")
  (pose m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <krakenResetPose-request>) ostream)
  "Serializes a message object of type '<krakenResetPose-request>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'pose) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <krakenResetPose-request>) istream)
  "Deserializes a message object of type '<krakenResetPose-request>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'pose) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<krakenResetPose-request>)))
  "Returns string type for a service object of type '<krakenResetPose-request>"
  "kraken_msgs/krakenResetPoseRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'krakenResetPose-request)))
  "Returns string type for a service object of type 'krakenResetPose-request"
  "kraken_msgs/krakenResetPoseRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<krakenResetPose-request>)))
  "Returns md5sum for a message object of type '<krakenResetPose-request>"
  "5ad83e5d3eeb79d5efc751e157bf87aa")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'krakenResetPose-request)))
  "Returns md5sum for a message object of type 'krakenResetPose-request"
  "5ad83e5d3eeb79d5efc751e157bf87aa")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<krakenResetPose-request>)))
  "Returns full string definition for message of type '<krakenResetPose-request>"
  (cl:format cl:nil "krakenPose pose~%~%================================================================================~%MSG: kraken_msgs/krakenPose~%Header header~%float32[15] data~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.secs: seconds (stamp_secs) since epoch~%# * stamp.nsecs: nanoseconds since stamp_secs~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'krakenResetPose-request)))
  "Returns full string definition for message of type 'krakenResetPose-request"
  (cl:format cl:nil "krakenPose pose~%~%================================================================================~%MSG: kraken_msgs/krakenPose~%Header header~%float32[15] data~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.secs: seconds (stamp_secs) since epoch~%# * stamp.nsecs: nanoseconds since stamp_secs~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <krakenResetPose-request>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'pose))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <krakenResetPose-request>))
  "Converts a ROS message object to a list"
  (cl:list 'krakenResetPose-request
    (cl:cons ':pose (pose msg))
))
;//! \htmlinclude krakenResetPose-response.msg.html

(cl:defclass <krakenResetPose-response> (roslisp-msg-protocol:ros-message)
  ((check
    :reader check
    :initarg :check
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass krakenResetPose-response (<krakenResetPose-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <krakenResetPose-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'krakenResetPose-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kraken_msgs-srv:<krakenResetPose-response> is deprecated: use kraken_msgs-srv:krakenResetPose-response instead.")))

(cl:ensure-generic-function 'check-val :lambda-list '(m))
(cl:defmethod check-val ((m <krakenResetPose-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-srv:check-val is deprecated.  Use kraken_msgs-srv:check instead.")
  (check m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <krakenResetPose-response>) ostream)
  "Serializes a message object of type '<krakenResetPose-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'check) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <krakenResetPose-response>) istream)
  "Deserializes a message object of type '<krakenResetPose-response>"
    (cl:setf (cl:slot-value msg 'check) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<krakenResetPose-response>)))
  "Returns string type for a service object of type '<krakenResetPose-response>"
  "kraken_msgs/krakenResetPoseResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'krakenResetPose-response)))
  "Returns string type for a service object of type 'krakenResetPose-response"
  "kraken_msgs/krakenResetPoseResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<krakenResetPose-response>)))
  "Returns md5sum for a message object of type '<krakenResetPose-response>"
  "5ad83e5d3eeb79d5efc751e157bf87aa")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'krakenResetPose-response)))
  "Returns md5sum for a message object of type 'krakenResetPose-response"
  "5ad83e5d3eeb79d5efc751e157bf87aa")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<krakenResetPose-response>)))
  "Returns full string definition for message of type '<krakenResetPose-response>"
  (cl:format cl:nil "bool check~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'krakenResetPose-response)))
  "Returns full string definition for message of type 'krakenResetPose-response"
  (cl:format cl:nil "bool check~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <krakenResetPose-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <krakenResetPose-response>))
  "Converts a ROS message object to a list"
  (cl:list 'krakenResetPose-response
    (cl:cons ':check (check msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'krakenResetPose)))
  'krakenResetPose-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'krakenResetPose)))
  'krakenResetPose-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'krakenResetPose)))
  "Returns string type for a service object of type '<krakenResetPose>"
  "kraken_msgs/krakenResetPose")