
(cl:in-package :asdf)

(defsystem "kraken_msgs-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :kraken_msgs-msg
)
  :components ((:file "_package")
    (:file "moveAlongLine" :depends-on ("_package_moveAlongLine"))
    (:file "_package_moveAlongLine" :depends-on ("_package"))
    (:file "krakenResetPose" :depends-on ("_package_krakenResetPose"))
    (:file "_package_krakenResetPose" :depends-on ("_package"))
  ))