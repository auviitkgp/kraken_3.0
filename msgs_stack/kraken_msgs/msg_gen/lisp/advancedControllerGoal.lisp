; Auto-generated. Do not edit!


(cl:in-package kraken_msgs-msg)


;//! \htmlinclude advancedControllerGoal.msg.html

(cl:defclass <advancedControllerGoal> (roslisp-msg-protocol:ros-message)
  ((x
    :reader x
    :initarg :x
    :type cl:float
    :initform 0.0)
   (y
    :reader y
    :initarg :y
    :type cl:float
    :initform 0.0)
   (depth
    :reader depth
    :initarg :depth
    :type cl:float
    :initform 0.0))
)

(cl:defclass advancedControllerGoal (<advancedControllerGoal>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <advancedControllerGoal>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'advancedControllerGoal)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kraken_msgs-msg:<advancedControllerGoal> is deprecated: use kraken_msgs-msg:advancedControllerGoal instead.")))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <advancedControllerGoal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:x-val is deprecated.  Use kraken_msgs-msg:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <advancedControllerGoal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:y-val is deprecated.  Use kraken_msgs-msg:y instead.")
  (y m))

(cl:ensure-generic-function 'depth-val :lambda-list '(m))
(cl:defmethod depth-val ((m <advancedControllerGoal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:depth-val is deprecated.  Use kraken_msgs-msg:depth instead.")
  (depth m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <advancedControllerGoal>) ostream)
  "Serializes a message object of type '<advancedControllerGoal>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'depth))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <advancedControllerGoal>) istream)
  "Deserializes a message object of type '<advancedControllerGoal>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'y) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'depth) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<advancedControllerGoal>)))
  "Returns string type for a message object of type '<advancedControllerGoal>"
  "kraken_msgs/advancedControllerGoal")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'advancedControllerGoal)))
  "Returns string type for a message object of type 'advancedControllerGoal"
  "kraken_msgs/advancedControllerGoal")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<advancedControllerGoal>)))
  "Returns md5sum for a message object of type '<advancedControllerGoal>"
  "d52d05f2b70dc2c113fbc54289a1056a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'advancedControllerGoal)))
  "Returns md5sum for a message object of type 'advancedControllerGoal"
  "d52d05f2b70dc2c113fbc54289a1056a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<advancedControllerGoal>)))
  "Returns full string definition for message of type '<advancedControllerGoal>"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#goal~%float32 x~%float32 y~%float32 depth~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'advancedControllerGoal)))
  "Returns full string definition for message of type 'advancedControllerGoal"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#goal~%float32 x~%float32 y~%float32 depth~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <advancedControllerGoal>))
  (cl:+ 0
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <advancedControllerGoal>))
  "Converts a ROS message object to a list"
  (cl:list 'advancedControllerGoal
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
    (cl:cons ':depth (depth msg))
))
