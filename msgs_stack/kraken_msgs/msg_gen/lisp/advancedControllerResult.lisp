; Auto-generated. Do not edit!


(cl:in-package kraken_msgs-msg)


;//! \htmlinclude advancedControllerResult.msg.html

(cl:defclass <advancedControllerResult> (roslisp-msg-protocol:ros-message)
  ((final_x
    :reader final_x
    :initarg :final_x
    :type cl:float
    :initform 0.0)
   (final_y
    :reader final_y
    :initarg :final_y
    :type cl:float
    :initform 0.0)
   (final_z
    :reader final_z
    :initarg :final_z
    :type cl:float
    :initform 0.0))
)

(cl:defclass advancedControllerResult (<advancedControllerResult>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <advancedControllerResult>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'advancedControllerResult)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kraken_msgs-msg:<advancedControllerResult> is deprecated: use kraken_msgs-msg:advancedControllerResult instead.")))

(cl:ensure-generic-function 'final_x-val :lambda-list '(m))
(cl:defmethod final_x-val ((m <advancedControllerResult>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:final_x-val is deprecated.  Use kraken_msgs-msg:final_x instead.")
  (final_x m))

(cl:ensure-generic-function 'final_y-val :lambda-list '(m))
(cl:defmethod final_y-val ((m <advancedControllerResult>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:final_y-val is deprecated.  Use kraken_msgs-msg:final_y instead.")
  (final_y m))

(cl:ensure-generic-function 'final_z-val :lambda-list '(m))
(cl:defmethod final_z-val ((m <advancedControllerResult>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:final_z-val is deprecated.  Use kraken_msgs-msg:final_z instead.")
  (final_z m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <advancedControllerResult>) ostream)
  "Serializes a message object of type '<advancedControllerResult>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'final_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'final_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'final_z))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <advancedControllerResult>) istream)
  "Deserializes a message object of type '<advancedControllerResult>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'final_x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'final_y) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'final_z) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<advancedControllerResult>)))
  "Returns string type for a message object of type '<advancedControllerResult>"
  "kraken_msgs/advancedControllerResult")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'advancedControllerResult)))
  "Returns string type for a message object of type 'advancedControllerResult"
  "kraken_msgs/advancedControllerResult")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<advancedControllerResult>)))
  "Returns md5sum for a message object of type '<advancedControllerResult>"
  "eca74fd82b15e9b5ed37f12a99b95b9e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'advancedControllerResult)))
  "Returns md5sum for a message object of type 'advancedControllerResult"
  "eca74fd82b15e9b5ed37f12a99b95b9e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<advancedControllerResult>)))
  "Returns full string definition for message of type '<advancedControllerResult>"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#result~%float32 final_x~%float32 final_y~%float32 final_z~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'advancedControllerResult)))
  "Returns full string definition for message of type 'advancedControllerResult"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#result~%float32 final_x~%float32 final_y~%float32 final_z~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <advancedControllerResult>))
  (cl:+ 0
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <advancedControllerResult>))
  "Converts a ROS message object to a list"
  (cl:list 'advancedControllerResult
    (cl:cons ':final_x (final_x msg))
    (cl:cons ':final_y (final_y msg))
    (cl:cons ':final_z (final_z msg))
))
