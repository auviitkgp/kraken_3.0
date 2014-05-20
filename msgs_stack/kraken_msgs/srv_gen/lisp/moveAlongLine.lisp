; Auto-generated. Do not edit!


(cl:in-package kraken_msgs-srv)


;//! \htmlinclude moveAlongLine-request.msg.html

(cl:defclass <moveAlongLine-request> (roslisp-msg-protocol:ros-message)
  ((type
    :reader type
    :initarg :type
    :type cl:integer
    :initform 0)
   (angle
    :reader angle
    :initarg :angle
    :type cl:float
    :initform 0.0))
)

(cl:defclass moveAlongLine-request (<moveAlongLine-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <moveAlongLine-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'moveAlongLine-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kraken_msgs-srv:<moveAlongLine-request> is deprecated: use kraken_msgs-srv:moveAlongLine-request instead.")))

(cl:ensure-generic-function 'type-val :lambda-list '(m))
(cl:defmethod type-val ((m <moveAlongLine-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-srv:type-val is deprecated.  Use kraken_msgs-srv:type instead.")
  (type m))

(cl:ensure-generic-function 'angle-val :lambda-list '(m))
(cl:defmethod angle-val ((m <moveAlongLine-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-srv:angle-val is deprecated.  Use kraken_msgs-srv:angle instead.")
  (angle m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <moveAlongLine-request>) ostream)
  "Serializes a message object of type '<moveAlongLine-request>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'type)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <moveAlongLine-request>) istream)
  "Deserializes a message object of type '<moveAlongLine-request>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angle) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<moveAlongLine-request>)))
  "Returns string type for a service object of type '<moveAlongLine-request>"
  "kraken_msgs/moveAlongLineRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'moveAlongLine-request)))
  "Returns string type for a service object of type 'moveAlongLine-request"
  "kraken_msgs/moveAlongLineRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<moveAlongLine-request>)))
  "Returns md5sum for a message object of type '<moveAlongLine-request>"
  "83d2c27254d8886c28fb825aec6949dd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'moveAlongLine-request)))
  "Returns md5sum for a message object of type 'moveAlongLine-request"
  "83d2c27254d8886c28fb825aec6949dd")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<moveAlongLine-request>)))
  "Returns full string definition for message of type '<moveAlongLine-request>"
  (cl:format cl:nil "~%~%~%~%uint32 type~%float32 angle~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'moveAlongLine-request)))
  "Returns full string definition for message of type 'moveAlongLine-request"
  (cl:format cl:nil "~%~%~%~%uint32 type~%float32 angle~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <moveAlongLine-request>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <moveAlongLine-request>))
  "Converts a ROS message object to a list"
  (cl:list 'moveAlongLine-request
    (cl:cons ':type (type msg))
    (cl:cons ':angle (angle msg))
))
;//! \htmlinclude moveAlongLine-response.msg.html

(cl:defclass <moveAlongLine-response> (roslisp-msg-protocol:ros-message)
  ((result
    :reader result
    :initarg :result
    :type cl:integer
    :initform 0))
)

(cl:defclass moveAlongLine-response (<moveAlongLine-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <moveAlongLine-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'moveAlongLine-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kraken_msgs-srv:<moveAlongLine-response> is deprecated: use kraken_msgs-srv:moveAlongLine-response instead.")))

(cl:ensure-generic-function 'result-val :lambda-list '(m))
(cl:defmethod result-val ((m <moveAlongLine-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-srv:result-val is deprecated.  Use kraken_msgs-srv:result instead.")
  (result m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <moveAlongLine-response>) ostream)
  "Serializes a message object of type '<moveAlongLine-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'result)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'result)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'result)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'result)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <moveAlongLine-response>) istream)
  "Deserializes a message object of type '<moveAlongLine-response>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'result)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'result)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'result)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'result)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<moveAlongLine-response>)))
  "Returns string type for a service object of type '<moveAlongLine-response>"
  "kraken_msgs/moveAlongLineResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'moveAlongLine-response)))
  "Returns string type for a service object of type 'moveAlongLine-response"
  "kraken_msgs/moveAlongLineResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<moveAlongLine-response>)))
  "Returns md5sum for a message object of type '<moveAlongLine-response>"
  "83d2c27254d8886c28fb825aec6949dd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'moveAlongLine-response)))
  "Returns md5sum for a message object of type 'moveAlongLine-response"
  "83d2c27254d8886c28fb825aec6949dd")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<moveAlongLine-response>)))
  "Returns full string definition for message of type '<moveAlongLine-response>"
  (cl:format cl:nil "~%uint32 result~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'moveAlongLine-response)))
  "Returns full string definition for message of type 'moveAlongLine-response"
  (cl:format cl:nil "~%uint32 result~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <moveAlongLine-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <moveAlongLine-response>))
  "Converts a ROS message object to a list"
  (cl:list 'moveAlongLine-response
    (cl:cons ':result (result msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'moveAlongLine)))
  'moveAlongLine-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'moveAlongLine)))
  'moveAlongLine-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'moveAlongLine)))
  "Returns string type for a service object of type '<moveAlongLine>"
  "kraken_msgs/moveAlongLine")