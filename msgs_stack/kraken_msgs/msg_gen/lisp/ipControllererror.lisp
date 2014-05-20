; Auto-generated. Do not edit!


(cl:in-package kraken_msgs-msg)


;//! \htmlinclude ipControllererror.msg.html

(cl:defclass <ipControllererror> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass ipControllererror (<ipControllererror>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ipControllererror>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ipControllererror)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kraken_msgs-msg:<ipControllererror> is deprecated: use kraken_msgs-msg:ipControllererror instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ipControllererror>) ostream)
  "Serializes a message object of type '<ipControllererror>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ipControllererror>) istream)
  "Deserializes a message object of type '<ipControllererror>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ipControllererror>)))
  "Returns string type for a message object of type '<ipControllererror>"
  "kraken_msgs/ipControllererror")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ipControllererror)))
  "Returns string type for a message object of type 'ipControllererror"
  "kraken_msgs/ipControllererror")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ipControllererror>)))
  "Returns md5sum for a message object of type '<ipControllererror>"
  "d41d8cd98f00b204e9800998ecf8427e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ipControllererror)))
  "Returns md5sum for a message object of type 'ipControllererror"
  "d41d8cd98f00b204e9800998ecf8427e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ipControllererror>)))
  "Returns full string definition for message of type '<ipControllererror>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ipControllererror)))
  "Returns full string definition for message of type 'ipControllererror"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ipControllererror>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ipControllererror>))
  "Converts a ROS message object to a list"
  (cl:list 'ipControllererror
))
