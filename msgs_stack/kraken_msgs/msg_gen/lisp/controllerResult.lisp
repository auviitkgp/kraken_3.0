; Auto-generated. Do not edit!


(cl:in-package kraken_msgs-msg)


;//! \htmlinclude controllerResult.msg.html

(cl:defclass <controllerResult> (roslisp-msg-protocol:ros-message)
  ((heading_final
    :reader heading_final
    :initarg :heading_final
    :type cl:float
    :initform 0.0)
   (forward_final
    :reader forward_final
    :initarg :forward_final
    :type cl:float
    :initform 0.0)
   (rightmove_final
    :reader rightmove_final
    :initarg :rightmove_final
    :type cl:float
    :initform 0.0)
   (depth_final
    :reader depth_final
    :initarg :depth_final
    :type cl:float
    :initform 0.0))
)

(cl:defclass controllerResult (<controllerResult>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <controllerResult>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'controllerResult)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kraken_msgs-msg:<controllerResult> is deprecated: use kraken_msgs-msg:controllerResult instead.")))

(cl:ensure-generic-function 'heading_final-val :lambda-list '(m))
(cl:defmethod heading_final-val ((m <controllerResult>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:heading_final-val is deprecated.  Use kraken_msgs-msg:heading_final instead.")
  (heading_final m))

(cl:ensure-generic-function 'forward_final-val :lambda-list '(m))
(cl:defmethod forward_final-val ((m <controllerResult>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:forward_final-val is deprecated.  Use kraken_msgs-msg:forward_final instead.")
  (forward_final m))

(cl:ensure-generic-function 'rightmove_final-val :lambda-list '(m))
(cl:defmethod rightmove_final-val ((m <controllerResult>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:rightmove_final-val is deprecated.  Use kraken_msgs-msg:rightmove_final instead.")
  (rightmove_final m))

(cl:ensure-generic-function 'depth_final-val :lambda-list '(m))
(cl:defmethod depth_final-val ((m <controllerResult>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:depth_final-val is deprecated.  Use kraken_msgs-msg:depth_final instead.")
  (depth_final m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <controllerResult>) ostream)
  "Serializes a message object of type '<controllerResult>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'heading_final))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'forward_final))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'rightmove_final))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'depth_final))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <controllerResult>) istream)
  "Deserializes a message object of type '<controllerResult>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'heading_final) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'forward_final) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'rightmove_final) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'depth_final) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<controllerResult>)))
  "Returns string type for a message object of type '<controllerResult>"
  "kraken_msgs/controllerResult")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'controllerResult)))
  "Returns string type for a message object of type 'controllerResult"
  "kraken_msgs/controllerResult")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<controllerResult>)))
  "Returns md5sum for a message object of type '<controllerResult>"
  "c4ed7083844e87c15cbd06b69847e794")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'controllerResult)))
  "Returns md5sum for a message object of type 'controllerResult"
  "c4ed7083844e87c15cbd06b69847e794")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<controllerResult>)))
  "Returns full string definition for message of type '<controllerResult>"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#result~%float32 heading_final~%float32 forward_final~%float32 rightmove_final~%float32 depth_final~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'controllerResult)))
  "Returns full string definition for message of type 'controllerResult"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#result~%float32 heading_final~%float32 forward_final~%float32 rightmove_final~%float32 depth_final~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <controllerResult>))
  (cl:+ 0
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <controllerResult>))
  "Converts a ROS message object to a list"
  (cl:list 'controllerResult
    (cl:cons ':heading_final (heading_final msg))
    (cl:cons ':forward_final (forward_final msg))
    (cl:cons ':rightmove_final (rightmove_final msg))
    (cl:cons ':depth_final (depth_final msg))
))
