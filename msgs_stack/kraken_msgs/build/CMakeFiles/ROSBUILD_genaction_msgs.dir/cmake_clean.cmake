FILE(REMOVE_RECURSE
  "../msg_gen"
  "../msg_gen"
  "../src/kraken_msgs/msg"
  "CMakeFiles/ROSBUILD_genaction_msgs"
  "../msg/controllerAction.msg"
  "../msg/controllerGoal.msg"
  "../msg/controllerActionGoal.msg"
  "../msg/controllerResult.msg"
  "../msg/controllerActionResult.msg"
  "../msg/controllerFeedback.msg"
  "../msg/controllerActionFeedback.msg"
  "../msg/advancedControllerAction.msg"
  "../msg/advancedControllerGoal.msg"
  "../msg/advancedControllerActionGoal.msg"
  "../msg/advancedControllerResult.msg"
  "../msg/advancedControllerActionResult.msg"
  "../msg/advancedControllerFeedback.msg"
  "../msg/advancedControllerActionFeedback.msg"
  "../msg/orientControllerAction.msg"
  "../msg/orientControllerGoal.msg"
  "../msg/orientControllerActionGoal.msg"
  "../msg/orientControllerResult.msg"
  "../msg/orientControllerActionResult.msg"
  "../msg/orientControllerFeedback.msg"
  "../msg/orientControllerActionFeedback.msg"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genaction_msgs.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
