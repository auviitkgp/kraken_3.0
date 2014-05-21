; Auto-generated. Do not edit!


(cl:in-package kraken_msgs-msg)


;//! \htmlinclude controllerGoal.msg.html

(cl:defclass <controllerGoal> (roslisp-msg-protocol:ros-message)
  ((heading_setpoint
    :reader heading_setpoint
    :initarg :heading_setpoint
    :type cl:float
    :initform 0.0)
   (forward_setpoint
    :reader forward_setpoint
    :initarg :forward_setpoint
    :type cl:float
    :initform 0.0)
   (rightmove_setpoint
    :reader rightmove_setpoint
    :initarg :rightmove_setpoint
    :type cl:float
    :initform 0.0)
   (depth_setpoint
    :reader depth_setpoint
    :initarg :depth_setpoint
    :type cl:float
    :initform 0.0))
)

(cl:defclass controllerGoal (<controllerGoal>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <controllerGoal>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'controllerGoal)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kraken_msgs-msg:<controllerGoal> is deprecated: use kraken_msgs-msg:controllerGoal instead.")))

(cl:ensure-generic-function 'heading_setpoint-val :lambda-list '(m))
(cl:defmethod heading_setpoint-val ((m <controllerGoal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:heading_setpoint-val is deprecated.  Use kraken_msgs-msg:heading_setpoint instead.")
  (heading_setpoint m))

(cl:ensure-generic-function 'forward_setpoint-val :lambda-list '(m))
(cl:defmethod forward_setpoint-val ((m <controllerGoal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:forward_setpoint-val is deprecated.  Use kraken_msgs-msg:forward_setpoint instead.")
  (forward_setpoint m))

(cl:ensure-generic-function 'rightmove_setpoint-val :lambda-list '(m))
(cl:defmethod rightmove_setpoint-val ((m <controllerGoal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:rightmove_setpoint-val is deprecated.  Use kraken_msgs-msg:rightmove_setpoint instead.")
  (rightmove_setpoint m))

(cl:ensure-generic-function 'depth_setpoint-val :lambda-list '(m))
(cl:defmethod depth_setpoint-val ((m <controllerGoal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:depth_setpoint-val is deprecated.  Use kraken_msgs-msg:depth_setpoint instead.")
  (depth_setpoint m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <controllerGoal>) ostream)
  "Serializes a message object of type '<controllerGoal>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'heading_setpoint))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'forward_setpoint))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'rightmove_setpoint))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'depth_setpoint))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <controllerGoal>) istream)
  "Deserializes a message object of type '<controllerGoal>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'heading_setpoint) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'forward_setpoint) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'rightmove_setpoint) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'depth_setpoint) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<controllerGoal>)))
  "Returns string type for a message object of type '<controllerGoal>"
  "kraken_msgs/controllerGoal")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'controllerGoal)))
  "Returns string type for a message object of type 'controllerGoal"
  "kraken_msgs/controllerGoal")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<controllerGoal>)))
  "Returns md5sum for a message object of type '<controllerGoal>"
  "5bf62384fe2248574a658e65808d489d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'controllerGoal)))
  "Returns md5sum for a message object of type 'controllerGoal"
  "5bf62384fe2248574a658e65808d489d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<controllerGoal>)))
  "Returns full string definition for message of type '<controllerGoal>"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#goal~%float32 heading_setpoint~%float32 forward_setpoint~%float32 rightmove_setpoint~%float32 depth_setpoint~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'controllerGoal)))
  "Returns full string definition for message of type 'controllerGoal"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#goal~%float32 heading_setpoint~%float32 forward_setpoint~%float32 rightmove_setpoint~%float32 depth_setpoint~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <controllerGoal>))
  (cl:+ 0
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <controllerGoal>))
  "Converts a ROS message object to a list"
  (cl:list 'controllerGoal
    (cl:cons ':heading_setpoint (heading_setpoint msg))
    (cl:cons ':forward_setpoint (forward_setpoint msg))
    (cl:cons ':rightmove_setpoint (rightmove_setpoint msg))
    (cl:cons ':depth_setpoint (depth_setpoint msg))
))
