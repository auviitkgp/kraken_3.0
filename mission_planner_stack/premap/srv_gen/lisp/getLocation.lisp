; Auto-generated. Do not edit!


(cl:in-package premap-srv)


;//! \htmlinclude getLocation-request.msg.html

(cl:defclass <getLocation-request> (roslisp-msg-protocol:ros-message)
  ((id
    :reader id
    :initarg :id
    :type cl:integer
    :initform 0))
)

(cl:defclass getLocation-request (<getLocation-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <getLocation-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'getLocation-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name premap-srv:<getLocation-request> is deprecated: use premap-srv:getLocation-request instead.")))

(cl:ensure-generic-function 'id-val :lambda-list '(m))
(cl:defmethod id-val ((m <getLocation-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader premap-srv:id-val is deprecated.  Use premap-srv:id instead.")
  (id m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <getLocation-request>) ostream)
  "Serializes a message object of type '<getLocation-request>"
  (cl:let* ((signed (cl:slot-value msg 'id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <getLocation-request>) istream)
  "Deserializes a message object of type '<getLocation-request>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'id) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<getLocation-request>)))
  "Returns string type for a service object of type '<getLocation-request>"
  "premap/getLocationRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'getLocation-request)))
  "Returns string type for a service object of type 'getLocation-request"
  "premap/getLocationRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<getLocation-request>)))
  "Returns md5sum for a message object of type '<getLocation-request>"
  "1de201f083ea35a06514be7769efdb42")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'getLocation-request)))
  "Returns md5sum for a message object of type 'getLocation-request"
  "1de201f083ea35a06514be7769efdb42")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<getLocation-request>)))
  "Returns full string definition for message of type '<getLocation-request>"
  (cl:format cl:nil "int32 id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'getLocation-request)))
  "Returns full string definition for message of type 'getLocation-request"
  (cl:format cl:nil "int32 id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <getLocation-request>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <getLocation-request>))
  "Converts a ROS message object to a list"
  (cl:list 'getLocation-request
    (cl:cons ':id (id msg))
))
;//! \htmlinclude getLocation-response.msg.html

(cl:defclass <getLocation-response> (roslisp-msg-protocol:ros-message)
  ((x
    :reader x
    :initarg :x
    :type cl:float
    :initform 0.0)
   (y
    :reader y
    :initarg :y
    :type cl:float
    :initform 0.0))
)

(cl:defclass getLocation-response (<getLocation-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <getLocation-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'getLocation-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name premap-srv:<getLocation-response> is deprecated: use premap-srv:getLocation-response instead.")))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <getLocation-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader premap-srv:x-val is deprecated.  Use premap-srv:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <getLocation-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader premap-srv:y-val is deprecated.  Use premap-srv:y instead.")
  (y m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <getLocation-response>) ostream)
  "Serializes a message object of type '<getLocation-response>"
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
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <getLocation-response>) istream)
  "Deserializes a message object of type '<getLocation-response>"
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
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<getLocation-response>)))
  "Returns string type for a service object of type '<getLocation-response>"
  "premap/getLocationResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'getLocation-response)))
  "Returns string type for a service object of type 'getLocation-response"
  "premap/getLocationResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<getLocation-response>)))
  "Returns md5sum for a message object of type '<getLocation-response>"
  "1de201f083ea35a06514be7769efdb42")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'getLocation-response)))
  "Returns md5sum for a message object of type 'getLocation-response"
  "1de201f083ea35a06514be7769efdb42")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<getLocation-response>)))
  "Returns full string definition for message of type '<getLocation-response>"
  (cl:format cl:nil "float32 x~%float32 y~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'getLocation-response)))
  "Returns full string definition for message of type 'getLocation-response"
  (cl:format cl:nil "float32 x~%float32 y~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <getLocation-response>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <getLocation-response>))
  "Converts a ROS message object to a list"
  (cl:list 'getLocation-response
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'getLocation)))
  'getLocation-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'getLocation)))
  'getLocation-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'getLocation)))
  "Returns string type for a service object of type '<getLocation>"
  "premap/getLocation")