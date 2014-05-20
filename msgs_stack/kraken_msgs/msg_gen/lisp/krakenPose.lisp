; Auto-generated. Do not edit!


(cl:in-package kraken_msgs-msg)


;//! \htmlinclude krakenPose.msg.html

(cl:defclass <krakenPose> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (data
    :reader data
    :initarg :data
    :type (cl:vector cl:float)
   :initform (cl:make-array 15 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass krakenPose (<krakenPose>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <krakenPose>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'krakenPose)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kraken_msgs-msg:<krakenPose> is deprecated: use kraken_msgs-msg:krakenPose instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <krakenPose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:header-val is deprecated.  Use kraken_msgs-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <krakenPose>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kraken_msgs-msg:data-val is deprecated.  Use kraken_msgs-msg:data instead.")
  (data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <krakenPose>) ostream)
  "Serializes a message object of type '<krakenPose>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'data))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <krakenPose>) istream)
  "Deserializes a message object of type '<krakenPose>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:setf (cl:slot-value msg 'data) (cl:make-array 15))
  (cl:let ((vals (cl:slot-value msg 'data)))
    (cl:dotimes (i 15)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<krakenPose>)))
  "Returns string type for a message object of type '<krakenPose>"
  "kraken_msgs/krakenPose")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'krakenPose)))
  "Returns string type for a message object of type 'krakenPose"
  "kraken_msgs/krakenPose")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<krakenPose>)))
  "Returns md5sum for a message object of type '<krakenPose>"
  "f994f7995504e934d98c10e28bdef527")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'krakenPose)))
  "Returns md5sum for a message object of type 'krakenPose"
  "f994f7995504e934d98c10e28bdef527")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<krakenPose>)))
  "Returns full string definition for message of type '<krakenPose>"
  (cl:format cl:nil "Header header~%float32[15] data~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.secs: seconds (stamp_secs) since epoch~%# * stamp.nsecs: nanoseconds since stamp_secs~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'krakenPose)))
  "Returns full string definition for message of type 'krakenPose"
  (cl:format cl:nil "Header header~%float32[15] data~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.secs: seconds (stamp_secs) since epoch~%# * stamp.nsecs: nanoseconds since stamp_secs~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <krakenPose>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'data) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <krakenPose>))
  "Converts a ROS message object to a list"
  (cl:list 'krakenPose
    (cl:cons ':header (header msg))
    (cl:cons ':data (data msg))
))
