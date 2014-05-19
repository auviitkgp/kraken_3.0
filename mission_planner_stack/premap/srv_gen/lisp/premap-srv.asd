
(cl:in-package :asdf)

(defsystem "premap-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "getLocation" :depends-on ("_package_getLocation"))
    (:file "_package_getLocation" :depends-on ("_package"))
  ))