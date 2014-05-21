; Auto-generated. Do not edit!


(cl:in-package kraken_msgs-msg)


;//! \htmlinclude controllerFeedback.msg.html

(cl:defclass <controllerFeedback> (roslisp-msg-protocol:ros-message)
  ((heading_error
    :reader heading_error
    :initarg :heading_error
    :type cl:float
    :initform 0.0)
   (forward_error
    :reader forward_error
    :initarg :forward_error
    :type cl:float
    :initform 0.0)
   (rightmove_error
    :reader rightmove_error
    :initarg :rightmove_error
    :type cl:float
    :initform 0.0)
   (depth_error
    :reader depth_error
    :initarg :depth_error
    :type cl:float
    :initform 0.0))
)

(cl:defclass controllerFeedback (<controllerFeedback>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <controllerFeedback>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'controllerFeedback)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kraken_msgs-msg:<controllerFeedback> is deprecated: use kraken_msgs-msg:controllerFeedback instead.")))

(cl:ensure-generic-function 'heading_error-val :lambda-list '(m))
(cl:defmethod heading_error-val ((m <controllerFeedback>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:heading_error-val is deprecated.  Use kraken_msgs-msg:heading_error instead.")
  (heading_error m))

(cl:ensure-generic-function 'forward_error-val :lambda-list '(m))
(cl:defmethod forward_error-val ((m <controllerFeedback>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:forward_error-val is deprecated.  Use kraken_msgs-msg:forward_error instead.")
  (forward_error m))

(cl:ensure-generic-function 'rightmove_error-val :lambda-list '(m))
(cl:defmethod rightmove_error-val ((m <controllerFeedback>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:rightmove_error-val is deprecated.  Use kraken_msgs-msg:rightmove_error instead.")
  (rightmove_error m))

(cl:ensure-generic-function 'depth_error-val :lambda-list '(m))
(cl:defmethod depth_error-val ((m <controllerFeedback>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:depth_error-val is deprecated.  Use kraken_msgs-msg:depth_error instead.")
  (depth_error m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <controllerFeedback>) ostream)
  "Serializes a message object of type '<controllerFeedback>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'heading_error))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'forward_error))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'rightmove_error))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'depth_error))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <controllerFeedback>) istream)
  "Deserializes a message object of type '<controllerFeedback>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'heading_error) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'forward_error) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'rightmove_error) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'depth_error) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<controllerFeedback>)))
  "Returns string type for a message object of type '<controllerFeedback>"
  "kraken_msgs/controllerFeedback")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'controllerFeedback)))
  "Returns string type for a message object of type 'controllerFeedback"
  "kraken_msgs/controllerFeedback")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<controllerFeedback>)))
  "Returns md5sum for a message object of type '<controllerFeedback>"
  "b9dd750ab5213f9119cdfd27373882fc")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'controllerFeedback)))
  "Returns md5sum for a message object of type 'controllerFeedback"
  "b9dd750ab5213f9119cdfd27373882fc")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<controllerFeedback>)))
  "Returns full string definition for message of type '<controllerFeedback>"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#feedback~%float32 heading_error~%float32 forward_error~%float32 rightmove_error~%float32 depth_error~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'controllerFeedback)))
  "Returns full string definition for message of type 'controllerFeedback"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#feedback~%float32 heading_error~%float32 forward_error~%float32 rightmove_error~%float32 depth_error~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <controllerFeedback>))
  (cl:+ 0
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <controllerFeedback>))
  "Converts a ROS message object to a list"
  (cl:list 'controllerFeedback
    (cl:cons ':heading_error (heading_error msg))
    (cl:cons ':forward_error (forward_error msg))
    (cl:cons ':rightmove_error (rightmove_error msg))
    (cl:cons ':depth_error (depth_error msg))
))
